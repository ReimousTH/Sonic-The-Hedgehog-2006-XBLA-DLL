#pragma once

#include <vector>
#include <string>

namespace Patch
{
    namespace ArcFix
    {
        class Globals
        {
        public:
            static std::vector<std::string> ms_Arcs;
        };
       
        void INSTALL();
    }
}