#ifndef SONICTEAM__PLAYER__ITESTCASE
#define SONICTEAM__PLAYER__ITESTCASE

#include "Defs.h"
#include <Marathon.inl>

namespace Sonicteam {
	namespace Player {
		namespace Unit {
			class ITestCase {
			public:  // Add public access specifier

				ITestCase(const char* CaseName);
				~ITestCase(); // Make the destructor virtual
				MARATHON_DESTRUCTION_H(ITestCase);; // This defines the DestroyObject method
			};
		} // namespace Unit
	} // namespace Player
} // namespace Sonicteam

#endif
