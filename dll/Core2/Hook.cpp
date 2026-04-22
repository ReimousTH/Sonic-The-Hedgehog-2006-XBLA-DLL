#include "Hook.h"

std::map<void*, std::vector<HookNew*>>* HookNew::SaveBuffer;
std::map<void*, std::vector<HookNew*>>* HookNew::SaveBufferVFT;

int HookNew::SavedBufferSIZE = 0;


void HookNew::WriteBranch(void* TargetOffset, void* TargetFunction,uint32_t register_choose)
{
	DWORD SAVE_ADDRESS_BUFFER_FP13[4];
	DWORD BranchBuffer[4];
	ZeroMemory(SAVE_ADDRESS_BUFFER_FP13, sizeof(SAVE_ADDRESS_BUFFER_FP13));
	ZeroMemory(BranchBuffer, sizeof(BranchBuffer));

	BranchBuffer[0] = POWERPC_LIS(register_choose, POWERPC_HI((DWORD)TargetFunction));
	BranchBuffer[1] = POWERPC_ORI(register_choose, register_choose, POWERPC_LO((DWORD)TargetFunction));
	BranchBuffer[2] = POWERPC_MTCTR(register_choose);
	BranchBuffer[3] = 0x4E800420;

	memcpy((void*)(TargetOffset), BranchBuffer, sizeof(BranchBuffer));
}

void HookNew::memcpy(void* _Dst, const void* _Src, size_t _Size) {

	DWORD OPrct;//ProtectionFlag
	VirtualProtectEx(GetModuleHandle(NULL), HOOKADDR(_Dst), _Size, PAGE_READWRITE, &OPrct);
	std::memcpy(HOOKADDR(_Dst), _Src, _Size);
	VirtualProtectEx(GetModuleHandle(NULL), HOOKADDR(_Dst), _Size, OPrct, &OPrct);
}

HookNew* HookNew::CreateHook(void* Target, void* TargetMap, void* Original, char fp13, int fp13_number, lua_State* fp13_state, char RunPosition, bool block, int register_choose) {


	DWORD SAVE_ADDRESS_BUFFER_FP13[4];
	DWORD BranchBuffer[4];
	ZeroMemory(SAVE_ADDRESS_BUFFER_FP13, sizeof(SAVE_ADDRESS_BUFFER_FP13));
	ZeroMemory(BranchBuffer, sizeof(BranchBuffer));


	const DWORD Buffer = reinterpret_cast<DWORD>(HookNew::SavedBufferFunc);
	const DWORD BufferSize = HookNew::SavedBufferSIZE;
	const DWORD BufferPoint = Buffer + BufferSize;
	DWORD GenCopyInCount = 4;
	if (fp13) {
		GenCopyInCount = GenCopyInCount + (sizeof(SAVE_ADDRESS_BUFFER_FP13) / sizeof(DWORD));
	}
	DWORD GenFuncSize = (GenCopyInCount * 0x4);


	HookNew* hook = new HookNew();
	hook->OriginalFunc = reinterpret_cast<void*>(BufferPoint);
	hook->TargetFunc = Target;
	hook->RunPosition = RunPosition;
	hook->Block = block;

	AddMessage("hook : target: %x : original : %x", BufferPoint, Original);
	// Check if the original function is already hooked
	if (HookNew::SaveBuffer->find(Original) == HookNew::SaveBuffer->end()) {
		HookNew::SaveBuffer->insert(std::make_pair(Original, std::vector<HookNew*>()));


		int StartPos = BufferPoint; // Start position for copying instructions
		int OriginalPos = reinterpret_cast<int>(Original); // Pointer to original function

		// Copy instructions from the original function

		if (fp13) {

			while (true) {

				DWORD Instruction = *reinterpret_cast<int*>(OriginalPos);
				DWORD Instruction_OP = Instruction & 0xFC000000;

				if (Instruction_OP == POWERPC_OPCODE_B)
				{

				}

				OriginalPos += 4; // Move to the next instruction
				StartPos += 4;
			}

		}
		else {
			for (int i = 0; i < GenCopyInCount; ++i) {
				DWORD Instruction = *reinterpret_cast<int*>(OriginalPos);

				// Check if it's a branch instruction
				if ((Instruction & 0xFC000000) == POWERPC_OPCODE_B) {
					char linked = (Instruction & 1);
					INT32 BranchOffset = (Instruction & (MASK_N_BITS(24) << 2));

					// Sign-extend the branch offset if necessary
					if (BranchOffset >> (26 - 1)) {
						BranchOffset |= ~MASK_N_BITS(26);
					}
					int address_to = OriginalPos + BranchOffset;

					/*
					//instead-doing  __savegprlr_14 bl , we just add code of it 
					if (address_to >= 0x826083D0 && address_to <= 0x82608420) {

						DWORD __savegprlr_pos = address_to - 0x826083D0;
						for (int j = address_to; j < 0x826DE8E0 - 4; j += 4) {

							memcpy((void*)(StartPos), (void*)(j), 4); // Copy data to hook address
							StartPos += 4;
							//AddMessage("pointer : %x : %x",Original,StartPos);
						}
					} //__savegprlr_14
					else */{

						// Prepare branch instructions

						BranchBuffer[0] = POWERPC_LIS(register_choose, POWERPC_HI(address_to));
						BranchBuffer[1] = POWERPC_ORI(register_choose, register_choose, POWERPC_LO(address_to));
						BranchBuffer[2] = POWERPC_MTCTR(register_choose);
						BranchBuffer[3] = 0x4E800420 | linked;

						memcpy((void*)(StartPos), BranchBuffer, sizeof(BranchBuffer)); // Copy data to hook address
						StartPos += sizeof(BranchBuffer);
					}

				}
				else {
					memcpy((void*)(StartPos), &Instruction, sizeof(Instruction)); // Copy data to hook address
					StartPos += sizeof(Instruction);
				}
				OriginalPos += 4; // Move to the next instruction
			}
		}

		if (!fp13) {
			// Finalize the branch to the target function
			BranchBuffer[0] = POWERPC_LIS(register_choose, POWERPC_HI(reinterpret_cast<DWORD>(Original) + GenFuncSize));
			BranchBuffer[1] = POWERPC_ORI(register_choose, register_choose, POWERPC_LO(reinterpret_cast<DWORD>(Original) + GenFuncSize));
			BranchBuffer[2] = POWERPC_MTCTR(register_choose);
			BranchBuffer[3] = 0x4E800420;
			memcpy((void*)(StartPos), BranchBuffer, sizeof(BranchBuffer)); // Final copy for target
			StartPos += sizeof(BranchBuffer);
		}


		// Update saved buffer size
		HookNew::SavedBufferSIZE += (StartPos - BufferPoint);

		// Replace original function with branch to target map function

		int MoveOriginal = (int)Original;

		if (fp13) {

			//-----------------------------------IN WORK----------------------------------------(FP13)


			SAVE_ADDRESS_BUFFER_FP13[0] = POWERPC_LIS(register_choose, POWERPC_HI(reinterpret_cast<DWORD>(Original)));
			SAVE_ADDRESS_BUFFER_FP13[1] = POWERPC_ORI(register_choose, register_choose, POWERPC_LO(reinterpret_cast<DWORD>(Original)));
			SAVE_ADDRESS_BUFFER_FP13[2] = POWERPC_STW(register_choose, -8, 1); //stw reg,-8(r1)
			SAVE_ADDRESS_BUFFER_FP13[3] = POWERPC_LFS(13, -8, 1); //lfs f13,-8(r1) 
			memcpy((void*)(MoveOriginal), SAVE_ADDRESS_BUFFER_FP13, sizeof(SAVE_ADDRESS_BUFFER_FP13));
			MoveOriginal += sizeof(SAVE_ADDRESS_BUFFER_FP13);
			//-----------------------------------------------------------------------------------------
		}

		BranchBuffer[0] = POWERPC_LIS(register_choose, POWERPC_HI((DWORD)TargetMap));
		BranchBuffer[1] = POWERPC_ORI(register_choose, register_choose, POWERPC_LO((DWORD)TargetMap));
		BranchBuffer[2] = POWERPC_MTCTR(register_choose);
		BranchBuffer[3] = 0x4E800420;

		memcpy((void*)(MoveOriginal), BranchBuffer, sizeof(BranchBuffer)); // Final copy for original hook
	}

	(*HookNew::SaveBuffer)[Original].push_back(hook);

	return hook;

	// Optionally flush instruction cache if necessary
}

bool HookNew::IsEmulated()
{
	MEMORY_BASIC_INFORMATION data;
	SIZE_T result = VirtualQuery((void*)0x92000600, &data, sizeof(data));

	// Check if VirtualQuery succeeded
	if (result == 0) {
		// Handle error (e.g., log it or set a flag)
		UseEmulatedAddress = false;
		return false;
	}

	// Check if the memory is protected (you can adjust the condition as needed)
	if (data.Protect & (PAGE_NOACCESS | PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE)) {
		UseEmulatedAddress = true;
		return true;
	}

	UseEmulatedAddress = false;
	return false;
}

bool HookNew::UseEmulatedAddress;

#define EMPTY_SPAM_10 \
	__asm{blr};\
	__asm{blr};\
	__asm{blr};\
	__asm{blr};\
	__asm{blr};\
	__asm{blr};\
	__asm{blr};\
	__asm{blr};\
	__asm{blr};\
	__asm{blr};\

#define EMPTY_SPAM_20 \
	EMPTY_SPAM_10 \
	EMPTY_SPAM_10 \


#define EMPTY_SPAM_40 \
	EMPTY_SPAM_20 \
	EMPTY_SPAM_20 \

#define EMPTY_SPAM_100 \
	EMPTY_SPAM_40 \
	EMPTY_SPAM_40 \
	EMPTY_SPAM_20 \

#define EMPTY_SPAM_200 \
	EMPTY_SPAM_100 \
	EMPTY_SPAM_100 \

#define EMPTY_SPAM_400 \
	EMPTY_SPAM_200 \
	EMPTY_SPAM_200 \


#define EMPTY_SPAM_800 \
	EMPTY_SPAM_400 \
	EMPTY_SPAM_400

#define EMPTY_SPAM_1600 \
	EMPTY_SPAM_800 \
	EMPTY_SPAM_800 \

#define EMPTY_SPAM_3200 \
	EMPTY_SPAM_1600 \
	EMPTY_SPAM_1600 \

#define EMPTY_SPAM_6400 \
	EMPTY_SPAM_3200 \
	EMPTY_SPAM_3200 \

#define EMPTY_SPAM_12800 \
	EMPTY_SPAM_6400 \
	EMPTY_SPAM_6400 \

#define EMPTY_SPAM_25600 \
	EMPTY_SPAM_12800 \
	EMPTY_SPAM_12800 \

#define EMPTY_SPAM_51200 \
	EMPTY_SPAM_25600 \
	EMPTY_SPAM_25600 \






#pragma optimize("", off) // 
__declspec(naked) void HookNew::SavedBufferFunc() { EMPTY_SPAM_6400 }
__declspec(naked) void HookNew::SavedBufferFunc2() { EMPTY_SPAM_6400 }
__declspec(naked) void HookNew::SavedBufferFunc3() { EMPTY_SPAM_6400 }
__declspec(naked) void HookNew::SavedBufferFunc4() { EMPTY_SPAM_6400 }
__declspec(naked) void HookNew::SavedBufferFunc5() { EMPTY_SPAM_6400 }
__declspec(naked) void HookNew::SavedBufferFunc6() { EMPTY_SPAM_6400 }
__declspec(naked) void HookNew::SavedBufferFunc7() { EMPTY_SPAM_6400 }
__declspec(naked) void HookNew::SavedBufferFunc8() { EMPTY_SPAM_6400 }
#pragma optimize("", on) // Re-enable optimizations

#define lua_pushvoidptr(L,n,ptr,index) \
	lua_pushstring06(L,n);\
	lua_pushlightuserdata(L,(void*)ptr);\
	lua_settable06(L,index-2);\

#define lua_pushfloat(L,n,flt,index) \
	lua_pushstring06(L,n);\
	lua_pushnumber(L,flt);\
	lua_settable06(L,index-2);
