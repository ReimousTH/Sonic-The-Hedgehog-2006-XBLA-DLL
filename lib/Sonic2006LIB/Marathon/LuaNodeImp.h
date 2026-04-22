#ifndef SONICTEAM__LUANODEIMP
#define SONICTEAM__LUANODEIMP
#include <Marathon.inl>
#include "Defs.h"

#include <string>
#include "LuaNode.h"


#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include <Marathon.inl>

namespace Sonicteam{
 
	//Do implementation later :)
	class LuaNodeImp:public LuaNode // : LuaNode + (again hided in destructors omg)
	{
	public:
		LuaNodeImp(lua_State*,char*);	
		~LuaNodeImp(void);

		MARATHON_DESTRUCTION_H(LuaNodeImp);




		//fields



	//	lua_State* Lua_State;
	//	std::string TablePath;  //Registry/DemoCamera =  Registry.DemoCamera in short


	};

}
#endif