#include "../Headers/Slyvina.hpp"
#include  "../Headers/SlyvWindows.hpp"


#ifdef SlyvWindows
#include <Windows.h>

namespace Slyvina {
	namespace Units {

		void ReleaseConsole() {
			FreeConsole();
		}

	}
}
#endif