// Lic:
// Units/Source/SlyvOpenURL.cpp
// Slyvina - Open URL
// version: 23.07.22
// Copyright (C) 2021, 2023 Jeroen P. Broks
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software
// in a product, an acknowledgment in the product documentation would be
// appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
// EndLic

#include "../Headers/SlyvOpenURL.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define Voor_Windows
#endif


// Windows only includes
#ifdef Voor_Windows
	#include <windows.h>
	#include <shellapi.h>
#else
	#include <string>
	#include <iostream>
#endif

namespace Slyvina {
	namespace Units {

#ifdef Voor_Windows

		static const wchar_t* GetWC(const char* c) {
			const size_t cSize = strlen(c) + 1;
			size_t outSize;
			wchar_t* wc = new wchar_t[cSize];
			//mbstowcs(wc, c, cSize); // Microsoft "deprecated" this (read: already removed! Microsoft never really understood termology).
			mbstowcs_s(&outSize, wc, cSize, c, cSize - 1);
			return wc;
		}
#endif

		void OpenURL(const char* site) {
#ifdef Voor_Windows
			auto wsite = GetWC(site);
			ShellExecute(0, 0, wsite, 0, 0, SW_SHOW);
			delete[] wsite;
#elif TARGET_OS_MAC
			std::string msite{ "open " };
			msite += site;
			system(msite.c_str());
#elif __linux__
			std::string msite{ "xdg-open " };
			msite += site;
			system(msite.c_str());
#else
			std::cout << "\7OpenURL(\"" << site << "\"): Not yet supported for this platform!\n";
#endif
		}
	}
}