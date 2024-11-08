// License:
// 	Units/Source/SlyvSharedArray.cpp
// 	Slyvina Shared Array (Error Handling)
// 	version: 24.11.02
// 
// 	Copyright (C) 2024 Jeroen P. Broks
// 
// 	This software is provided 'as-is', without any express or implied
// 	warranty.  In no event will the authors be held liable for any damages
// 	arising from the use of this software.
// 
// 	Permission is granted to anyone to use this software for any purpose,
// 	including commercial applications, and to alter it and redistribute it
// 	freely, subject to the following restrictions:
// 
// 	1. The origin of this software must not be misrepresented; you must not
// 	   claim that you wrote the original software. If you use this software
// 	   in a product, an acknowledgment in the product documentation would be
// 	   appreciated but is not required.
// 	2. Altered source versions must be plainly marked as such, and must not be
// 	   misrepresented as being the original software.
// 	3. This notice may not be removed or altered from any source distribution.
// End License
#include "../Headers/SlyvSharedArray.hpp"

namespace Slyvina {
	namespace Units {

		static SharedArrayPanic _Panic{ nullptr };

		static void BasePanic(std::string f) {
			std::cout << "\x07\x1b[31mError: \x1b[0m;" << f << std::endl;
			throw std::runtime_error(f);
		}

		void SharedArraySetPanic(SharedArrayPanic f) {
			_Panic = f;
		}

		void Slyvina_SharedArray_Panic_56fc8eaeba2f10dbc003e418cdc921b9(std::string f) {
			_Panic ? _Panic(f) : BasePanic(f);
		}

	}
}
