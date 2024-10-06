// Lic:
// ***********************************************************
// Units/Headers/SlyvSilly.hpp
// This particular file has been released in the public domain
// and is therefore free of any restriction. You are allowed
// to credit me as the original author, but this is not
// required.
// This file was setup/modified in:
// 2024
// If the law of your country does not support the concept
// of a product being released in the public domain, while
// the original author is still alive, or if his death was
// not longer than 70 years ago, you can deem this file
// "(c) Jeroen Broks - licensed under the CC0 License",
// with basically comes down to the same lack of
// restriction the public domain offers. (YAY!)
// ***********************************************************
// Version 24.10.05
// EndLic

#pragma once

#include "Slyvina.hpp"

// Just a bunch of beyond silly console routines which C++ does not appear to support without a little help.

namespace Slyvina {
	namespace Units {
		inline void pause() {
#ifdef SlyvWindows
			system("pause");
#else
			system("read -n1 -r -p \"Press any key to continue...\" key");
#endif
		}

		inline void cls() {
#ifdef SlyvWindows
			system("cls");
#else
			system("clear");
#endif
		}

		inline void Beep() { printf("%c", '\x07'); }
	}
}