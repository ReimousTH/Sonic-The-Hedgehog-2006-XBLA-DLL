#include "ArcFix.h"

#include <xtl.h>
#include <Core2/Hook.h>
#include <Core2/Debug.h>
#include <ArcFileHandle.h>
#include <ArcFileLoader.h>
#include <Sox/FileSystemARC.h>
#include <Sox/FileHandleARC.h>
#include <Sox/FileLoaderNULL.h>
#include <ArcManager.h>
#include <FileLoaderARC.h>
#include <string>
#include <algorithm>
#include <MyModelStrategyNN.h.h>
#include <Sox/Graphics/Frame.h>
#include <MyModel.h>



using namespace Patch::ArcFix;

std::vector<std::string> Patch::ArcFix::Globals::ms_Arcs;

void LoadARC(std::string& path)
{
    std::string path2 = path;
    REF_TYPE(Sonicteam::SoX::ArcHandle) m_pRet;
    std::replace(path2.begin(), path2.end(), '\\', '/');

    HookBranchTo(0x82144750, int, &m_pRet, Sonicteam::FileLoaderARC::getInstance().m_spArcManager.get(), &path2);
    HookBranchTo(0x82143070, int, Sonicteam::FileLoaderARC::getInstance().m_spArcManager.get(), &path2);
    
    if (m_pRet.get()) 
    {

        if (!m_pRet.get()->IsDownloadContent)
        {
            FILE* handle_real = fopen(m_pRet.get()->m_ArcFilePath.c_str(), "rb");
            if (handle_real)
            {
                fclose(handle_real);
                m_pRet.get()->IsDownloadContent = 1;
                m_pRet.get()->m_Field44->ReadArcHeader();
                HookBranchTo(0x824F6088, int, &m_pRet.get()->m_ArcFile, m_pRet.get()->m_Field44->GetHandle());
            }
        }

        if (m_pRet.get()->IsDownloadContent)
        {
            AddMessage2(Logging::ms_LoggingArcLoading,"Arc : %s was Loaded", path2.c_str());
        }
        else
        {
            Sonicteam::FileLoaderARC::getInstance().m_spArcManager->m_mspHandles.erase(Sonicteam::FileLoaderARC::getInstance().m_spArcManager->m_mspHandles.find(path2));
            AddMessage2(Logging::ms_LoggingArcLoading,"Arc : %s Failed to load", path2.c_str());
        }
    }
    else
    {
        AddMessage2(Logging::ms_LoggingArcLoading,"Arc : %s Failed to load", path2.c_str());
    }
}

HOOKV3(0x8297A0D0, void*, sub_8297A0D0,
    (int,int,int,int), (a1,a2,a3,a4),
    int a1, int a2, int a3, int a4)
{
    
    for (std::vector<std::string>::iterator fullPath = Globals::ms_Arcs.begin(); fullPath != Globals::ms_Arcs.end(); fullPath++)
    {
        LoadARC(*fullPath);
    }
 
    /*
    LoadARC(std::string("archives/kdv_a0.arc.arc"));
    LoadARC(std::string("archives/kdv_a1.arc.arc"));
    LoadARC(std::string("archives/kdv_a2.arc.arc"));
    LoadARC(std::string("archives/kdv_a3.arc.arc"));
    LoadARC(std::string("archives/kdv_a4.arc.arc"));
    */

   
    return 0;
}


//path1 =
int ComparePathComponentsX(const char* str1, const char* str2) {

    while (*str2 != '\0') {
        char c1 = tolower(*str1);
        char c2 = tolower(*str2);

        if (c1 != c2) {
            return 0;
        }

        str1++;
        str2++;
    }

    char last_char = (*str1);
    if (last_char == '/' || last_char == '\0') {
        return 1;
    }

    return 0;
}




size_t SearchChunkEntryE1(Sonicteam::SoX::ArcFile* arcFile, const char* path) {

    std::string test = path;



    if (!arcFile || !path || !arcFile->ChunkArray || !arcFile->ChunkStringOffset)
        return -1;

    const char* current = path;
    size_t current_index = arcFile->RootIndex;

    // Skip leading slashes
    while (*current == '/') current++;
    if (*current == '\0') return current_index;

    while (true)
    {
        // Get current directory bounds
        size_t start_index = current_index + 1;
        size_t end_index = arcFile->ChunkArray[current_index].CompressedSize;
        bool found = false;

        // Search through current directory entries
        for (size_t i = start_index; i < end_index; )
        {

            Sonicteam::SoX::ArcFileChunk* chunk_i = &arcFile->ChunkArray[i];

            const char* name = (const char*)((size_t)arcFile->ChunkStringOffset + (size_t)chunk_i->Info.NameOffset);
            bool is_dir = arcFile->ChunkArray[i].Info.Type == Sonicteam::SoX::_ChunkInfo::Directory;

            if (ComparePathComponentsX(current, name))
            {
                // Move past the matched component
                current += strlen(name);

                // Skip any slashes after component
                while (*current == '/') current++;

                if (*current == '\0')
                {
                    // Found our final target
                    return i;
                }

                if (!is_dir)
                {
                    // Path continues but entry isn't a directory
                    return -1;
                }

                // Descend into subdirectory
                current_index = i;
                found = true;
                break;
            }

            // Move to next entry (skip directory contents if this is a directory)
            i = is_dir ? arcFile->ChunkArray[i].CompressedSize : i + 1;
        }

        if (!found) return -1;
    }
}

uint64_t sub_82140CE0(Sonicteam::SoX::FileHandleARC* handle)
{
    if (handle->IsLoaded)
    {
        if (handle->m_IsCompressed)
            return handle->m_DecompressedSize;

        return handle->m_CompressedSize;
    }
    return 0;
}

void* sub_82140CC0(Sonicteam::SoX::FileHandleARC* handle)
{
    if (handle->IsLoaded)
    {
        if (handle->m_IsCompressed)
            return (void* )handle->m_Field58;
        return (void* )handle->m_Buffer;
    }
    return 0;
}

void* sub_82142C20(Sonicteam::SoX::FileHandleARC* handle, uint32_t flag)
{
    if (handle->m_Buffer)
    {
        free((void*)handle->m_Buffer);
        handle->m_Buffer = 0;
    }
    return HookBranchTo(0x82142C20, void*, handle,flag);
}

// Do not use external buffer
bool sub_82145828(Sonicteam::SoX::FileHandleARC* handle)
{
    Sonicteam::FileLoaderARC* pArc = &Sonicteam::FileLoaderARC::getInstance();
    std::string v19 = handle->GetARCChunkPath(pArc, handle->m_Path);
    REF_TYPE(Sonicteam::SoX::ArcHandle) spArcHandle;

    // ? (need to edit, how this method works, replace with mine)

    /*
    HookBranchTo(0x82144CE0, int, &spArcHandle, pArc->m_spArcManager.get(), &v19, &handle->m_Buffer);
    */

    for (std::map<std::string, REF_TYPE(Sonicteam::SoX::ArcHandle)>::iterator it = pArc->m_spArcManager->m_mspHandles.begin() ;it != pArc->m_spArcManager->m_mspHandles.end();it++)
    {
    
        Sonicteam::SoX::ArcHandle* pFindHandle = it->second.get();
        if (!pFindHandle->m_Field44.get())
        {
            pFindHandle->m_Field44 = pFindHandle->m_Field8->m_pFileLoaderE3->LoadHandle(pFindHandle->m_ArcFilePath);
        }

   
        auto index = SearchChunkEntryE1(&pFindHandle->m_ArcFile, v19.c_str());
        if (index == -1)
        {
            continue;
        }


        Sonicteam::SoX::ArcFileChunk fileDataARC = pFindHandle->m_ArcFile.ChunkArray[index];
        handle->m_Buffer = (uint32_t)fileDataARC.Offset;

    
        memset(&handle->m_CompressedSize, 0, 8);
        memset(&handle->m_DecompressedSize, 0, 8);

        handle->m_CompressedSize = fileDataARC.CompressedSize;
        handle->m_DecompressedSize = fileDataARC.DecompressedSize;
        handle->m_IsCompressed = fileDataARC.DecompressedSize != 0;

        pFindHandle->m_ArcFilePath = it->first;
        spArcHandle = pFindHandle;
        break;

    }

    handle->m_spHandle = spArcHandle;
    if (handle->m_spHandle.get())
    {   
        if (handle->m_IsCompressed)
        {

            auto buffer = (uint32_t)handle->m_spHandle.get()->m_Field44->m_Field24;
            void* buffer_file = malloc(handle->m_CompressedSize);
            FILE* handle_real = fopen(spArcHandle.get()->m_ArcFilePath.c_str(), "rb");
            if (handle_real)
            {
                fseek(handle_real, handle->m_Buffer - 0, SEEK_SET);  // 0 = SEEK_SET (beginning)
                if (fread(buffer_file, handle->m_CompressedSize, 1, handle_real) == 1)
                {

                }
                fclose(handle_real);
            }
            handle->m_Buffer = (uint32_t)buffer_file;

            if (handle->m_Field58)
                free((void*)handle->m_Field58);

            handle->m_Field58 = (uint32_t)malloc(handle->m_DecompressedSize);
            HookBranchTo(0x824F5E10, int, handle->m_Field58, handle->m_DecompressedSize, handle->m_Buffer, handle->m_CompressedSize); // Decompress
            handle->IsProcess = handle->m_Field48((void*)handle->m_Field58, handle->m_DecompressedSize);


           // free(buffer_file);
        }
        else
        {
            auto buffer = (uint32_t)handle->m_spHandle.get()->m_Field44->m_Field24;
            void* buffer_file = malloc(handle->m_CompressedSize);
            FILE* handle_real = fopen(spArcHandle.get()->m_ArcFilePath.c_str(), "rb");
            if (handle_real)
            {
                fseek(handle_real, handle->m_Buffer - 0, SEEK_SET);  // 0 = SEEK_SET (beginning)
                if (fread(buffer_file, handle->m_CompressedSize, 1, handle_real) == 1)
                {

                }
                fclose(handle_real);
            }
        
            handle->m_Buffer = (uint32_t)buffer_file;
            handle->IsProcess = handle->m_Field48((void*)handle->m_Buffer, handle->m_CompressedSize);
            //free(buffer_file);
        }


        AddMessage2(Logging::ms_LoggingFileArcLoading,"%s - %d (FileLoaderARC - m_IsCompressed %d), File %s loaded - %d", __FILE__, __LINE__, handle->m_IsCompressed,v19.c_str(), handle->IsProcess);
        handle->IsLoaded = 1;
        return handle->IsProcess;
    }

    handle->m_Buffer = 0;

    AddMessage2(Logging::ms_LoggingFileArcLoading,"%s - %d (FileLoaderARC), Failed to load file %s", __FILE__, __LINE__, v19.c_str());

    if (!handle->m_Field6 && !Debug::ms_EnableNULLLoader)
    {
        handle->IsLoaded = 1;
        return handle->IsProcess;
    }

    if (handle->m_spHandle.get() == 0)
    {
        //  AddMessage("sub_82145828 Failed To find %s, try native load (ms_EnableNULLLoader = true)", v19.c_str());
        Sonicteam::SoX::FileLoaderNULL* pNULL = &Sonicteam::SoX::FileLoaderNULL::getInstance();
        boost::shared_ptr<Sonicteam::SoX::IFileHandle> pHandle = pNULL->LoadFile(handle->m_Path, handle->m_Field48);;

        if (pHandle->LoadHandle())
        {
            handle->IsProcess = 1;
            handle->IsLoaded = 1;
            AddMessage2(Logging::ms_LoggingFileArcLoading,"%s - %d (FileLoaderNULL(Native Load)), File %s loaded", __FILE__, __LINE__, v19.c_str());
            return handle->IsProcess;
        }
        AddMessage2(Logging::ms_LoggingFileArcLoading,"%s - %d (%s), %s - %s", __FILE__, __LINE__, "FileLoaderNULL(Native Load)", "Failed to load file", v19.c_str());
    }
    else
    {
        handle->IsLoaded = 1;
        return handle->IsProcess;
    }
    return 0;
}


HOOKV3(0x821444A0, int, sub_821444A0,
    (Sonicteam::SoX::ArcHandle*), (handle), Sonicteam::SoX::ArcHandle* handle)
{

    if (!handle->IsFixedArchive)
    {
        if (!handle->m_Field44.get())
        {
            handle->m_Field44 = handle->m_Field8->m_pFileLoaderE3->LoadHandle(handle->m_ArcFilePath);
        }

        if (!handle->IsDownloadContent)
        {
            handle->IsDownloadContent = 1;
            handle->m_Field44->ReadArcHeader();
            HookBranchTo(0x824F6088, int, &handle->m_ArcFile, handle->m_Field44->GetHandle());
        }

        handle->m_Field2A = 1;
        handle->IsFixedArchive = 1;
    }

    return handle->m_Field2A;
}

// Readerbody ( we keep it as it )
HOOKV3(0x821415E8, int, sub_821415E8,
    (Sonicteam::ArcFileHandleE3*), (handle), Sonicteam::ArcFileHandleE3* handle)
{

    return 1;
}


void Patch::ArcFix::INSTALL()
{
   
    
  //  INSTALL_HOOKV3EX(sub_82145828, -1, true, 9);

    HookNew::WRITE_VALUE(0x820009CC, sub_82145828);
    HookNew::WRITE_VALUE(0x820009C8, sub_82142C20);
    HookNew::WRITE_VALUE(0x820009D4, sub_82140CC0);
    HookNew::WRITE_VALUE(0x820009D0, sub_82140CE0);

    INSTALL_HOOKV3EX(sub_821444A0, -1, true, 9);
    INSTALL_HOOKV3EX(sub_8297A0D0, -1, false, 9);

    HookNew::WRITE_VALUE(0x820009B4, 0x821416C8); // Always Use Read Header
    
    // Disable XBLA ARCHIVES
    HookNew::WRITE_VALUE(0x82594778, 0x60000000);
    HookNew::WRITE_VALUE(0x825947C0, 0x60000000);
    HookNew::WRITE_VALUE(0x82594808, 0x60000000);
    HookNew::WRITE_VALUE(0x82594850, 0x60000000);
    HookNew::WRITE_VALUE(0x82594898, 0x60000000);
    HookNew::WRITE_VALUE(0x825948E0, 0x60000000);
    

   // HookNew::WRITE_VALUE(0x82144D04, (short)0x4800);
  
}