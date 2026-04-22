cmake_minimum_required(VERSION 3.8)
project(patch_xex)


set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_RELEASE "")
set(CMAKE_C_FLAGS "")
set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_C_FLAGS_RELEASE "")


set(TOOLS_DIR "${CMAKE_SOURCE_DIR}/tools")
message(STATUS "Tools directory: ${TOOLS_DIR}")


if(EXISTS "${TOOLS_DIR}/ppu-lv2-as.exe")
    message(STATUS "Found ppu-lv2-as.exe")
    set(PPU_AS "${TOOLS_DIR}/ppu-lv2-as.exe")
else()
    message(WARNING "ppu-lv2-as.exe not found in ${TOOLS_DIR}")
endif()

if(EXISTS "${TOOLS_DIR}/ppu-lv2-objcopy.exe")
    message(STATUS "Found ppu-lv2-objcopy.exe")
    set(PPU_OBJCOPY "${TOOLS_DIR}/ppu-lv2-objcopy.exe")
endif()


if(PPU_AS AND PPU_OBJCOPY)
    message(STATUS "Building ASM patches...")
    

    file(GLOB ASM_FILES "${CMAKE_SOURCE_DIR}/patches/*.s")
    message(STATUS "Found ASM files: ${ASM_FILES}")
    
    foreach(asm_file ${ASM_FILES})
        get_filename_component(patch_name ${asm_file} NAME_WE)
        message(STATUS "Processing: ${patch_name}")
        
        set(obj_file "${CMAKE_BINARY_DIR}/patches/${patch_name}.o")
        set(bin_file "${CMAKE_BINARY_DIR}/patches/${patch_name}.bin")
        

        add_custom_command(
            OUTPUT "${CMAKE_BINARY_DIR}/patches"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/patches"
            COMMENT "Creating patches directory"
            VERBATIM
        )
        
        # .s -> .o
        add_custom_command(
            OUTPUT ${obj_file}
            COMMAND "${PPU_AS}"
                "${asm_file}"
                -o "${obj_file}"
            DEPENDS "${asm_file}" "${CMAKE_BINARY_DIR}/patches"
            COMMENT "Assembling ${patch_name}.s"
            VERBATIM
        )
        
        # .o -> .bin
        add_custom_command(
            OUTPUT ${bin_file}
            COMMAND "${PPU_OBJCOPY}"
                -O binary
                -j .text
                "${obj_file}"
                "${bin_file}"
            DEPENDS ${obj_file}
            COMMENT "Extracting ${patch_name}.bin"
            VERBATIM
        )
        
        list(APPEND PATCH_BIN_FILES ${bin_file})
    endforeach()

    if(PATCH_BIN_FILES)
        add_custom_target(asm_patches ALL DEPENDS ${PATCH_BIN_FILES})
    endif()
    
else()
    message(STATUS "Skipping ASM patches - tools not found")
endif()


add_executable(patch_xex patch_xex.cpp)

target_compile_options(patch_xex PRIVATE
    /nologo   
    /O2       
    /MT       
    /EHsc     
    /W3       
    /D_CRT_SECURE_NO_WARNINGS  
)

target_link_options(patch_xex PRIVATE
    /nologo
    /SUBSYSTEM:CONSOLE
)


set_target_properties(patch_xex PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# 0x81FFD000
set(PATCH_CONFIG
    # Entry point patches
    "0x4AF7D0"  "${CMAKE_BINARY_DIR}/patches/entry_branch.bin"
    "0x558E00"  "${CMAKE_BINARY_DIR}/patches/load_dll.bin"
    "0x9E69CC"  "${CMAKE_BINARY_DIR}/patches/DbgBreakPointToXLoadImageA.bin"
    
    # NOP patches at various offsets
    "0x4AF7E4"  "${CMAKE_BINARY_DIR}/patches/nop.bin"
    "0x4AF7E8"  "${CMAKE_BINARY_DIR}/patches/nop.bin"
    "0x4AF7EC"  "${CMAKE_BINARY_DIR}/patches/nop.bin"
    "0x4AF7F0"  "${CMAKE_BINARY_DIR}/patches/nop.bin"
    "0x4AF7F8"  "${CMAKE_BINARY_DIR}/patches/nop.bin"
    "0x4AF7FC"  "${CMAKE_BINARY_DIR}/patches/nop.bin"
    "0x4AF808"  "${CMAKE_BINARY_DIR}/patches/nop.bin"
    "0x4AF810"  "${CMAKE_BINARY_DIR}/patches/nop.bin"
    "0x4AF814"  "${CMAKE_BINARY_DIR}/patches/nop.bin"
    "0x4AF81C"  "${CMAKE_BINARY_DIR}/patches/nop.bin"
)


set(INPUT_XEX "${CMAKE_SOURCE_DIR}/files/default.xex")
set(OUTPUT_XEX "${CMAKE_SOURCE_DIR}/files/default_core.xex")

# Create the patch command
add_custom_command(
    TARGET patch_xex 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Patching XEX file..."
    COMMAND $<TARGET_FILE:patch_xex>
        "${INPUT_XEX}"
        "${OUTPUT_XEX}"
        ${PATCH_CONFIG}
    COMMENT "Applying patches to default.xex"
    DEPENDS patch_xex
    VERBATIM
)