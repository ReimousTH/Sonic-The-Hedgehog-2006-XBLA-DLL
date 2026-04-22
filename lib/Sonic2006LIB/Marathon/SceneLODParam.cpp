#include "SceneLODParam.h"
using namespace Sonicteam;

SceneLODParam::SceneLODParam()
{
	this->LinkArray.InitLink();
}

SceneLODParam::~SceneLODParam()
{

}
const char* SceneLODParam::GetParamName()
{
	return "LOD";
}

DESTRUCTION_CPP(SceneLODParam);
