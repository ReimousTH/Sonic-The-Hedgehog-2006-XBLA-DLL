#include "ResourceManager.h"

#include <typeinfo>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <Sox/FileSystemARC.h>
#include <FileLoaderARC.h>
#include <Sox/FileHandleARC.h>

using namespace Sonicteam::SoX;
DESTRUCTION_CPP(ResourceManager);

Sonicteam::SoX::ResourceManager::ResourceManager()
{

}

Sonicteam::SoX::ResourceManager::~ResourceManager()
{

}



//Sonicteam::SoX::IResourceMgrParam -> empty struct, defined for each MGR later on like , ArcHandleMgrParam:IResourceMgrParam{.....} etc
REF_TYPE(Sonicteam::SoX::IResource) ResourceManager::LoadResourceFromArc(Sonicteam::SoX::IResourceMgr* mgr,std::string& file,std::string& file_in_arc,Sonicteam::SoX::IResourceMgrParam* params,bool CheckInFileSystem)
{
	/*
	//PushBreakPoint(__FILE__,__LINE__," ResourceManager::LoadResourceFromArc,params : %x : %x : %x ",params,lr,lr1);

	bool loaded = false;
	REF_TYPE(Sonicteam::SoX::IResource) resource = 
		Sonicteam::SoX::ResourceManager::LoadResource(mgr, file, &loaded, params);

	if (!resource.get() || !loaded) 
		return 0;

	Sonicteam::SoX::IResource* res = resource.get();
	res->ResourceStr3 = file_in_arc;

	// Resource handling
	if (res->ResourceType()) 
	{
		std::string res_path = res->GetResourceName(file_in_arc);

		if (CheckInFileSystem && 
			!Sonicteam::SoX::FileSystemArc::getInstance().FSPathExist(res_path)) 
		{
			return 0;
		}

		// Create callback manually
			IFileLoader_BFUNCTION_TYPE callback = boost::bind(&Sonicteam::SoX::IResource::ResourceLoadFinal, res, _1, _2);

		REF_TYPE(Sonicteam::SoX::IFileHandle) file_handle = 
			Sonicteam::FileLoaderARC::getInstance().LoadFile(res_path, callback);

		Sonicteam::SoX::FileHandleARC* arc_handle = 
			static_cast<Sonicteam::SoX::FileHandleARC*>(file_handle.get());

		if (!arc_handle || !arc_handle->LoadHandle()) 
			return 0;

		return resource;
	}
	// Archive handling
	else 
	{
		return res->ResourceLoad(0, 0) ? resource : 0;
	}

*/
	return 0;
}

// not 1:1 but close
extern "C" REF_TYPE(Sonicteam::SoX::IResource) ResourceManager::LoadResource(Sonicteam::SoX::IResourceMgr* mgr,std::string& file,bool* result ,Sonicteam::SoX::IResourceMgrParam* params)
{
	unsigned int* index = ResourceManager::getInstance().InitializeManagerIndex(mgr);
	std::string resource_f_name = mgr->GetMgrResourceName(file);


	std::map<std::string, Sonicteam::SoX::IResource*>::iterator resource = 
		ResourceManager::getInstance().FindResource(index, resource_f_name);


	*result = false;

	// 1:1 with PPC assembly (matches lbz instruction at 82582434)
	if (true)
	{
		*result = true;
		if (resource->second == NULL) {
			Sonicteam::SoX::IResource* new_res = mgr->GetMgrResource(params);
			if (new_res) {
				new_res->SetResourceManagerMeta(index,resource_f_name,file);
				resource->second = new_res;
				return resource->second;
			}
			else {
				ResourceManager::getInstance().RemoveResource(index, resource_f_name);
				return NULL;
			}
		}

		// Matches AddReference call (825824E4)
		if (resource->second) {
			*result = true;
			return resource->second;
		}
	}

	// Failure path (matches loc_82582508)
	ResourceManager::getInstance().RemoveResource(index, resource_f_name);
	return NULL;

}


extern "C" void IResourceMgrF01(Sonicteam::SoX::IResourceMgr* mgr){
	mgr->IResourceMgr03impl();
}

extern "C" void IResourceMgrF02(Sonicteam::SoX::IResourceMgr* mgr){
	mgr->IResourceMgr04impl();
}


