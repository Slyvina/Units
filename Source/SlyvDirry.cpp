// Lic:
// Units/Source/SlyvDirry.cpp
// Slyvina - Dirry
// version: 23.01.09
// Copyright (C) 2020, 2022, 2023 Jeroen P. Broks
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
#include <SlyvDirry.hpp>
#include <SlyvString.hpp>

#include <map>
#include <stdlib.h>
#include <iostream>

#define Dirry_VisualStudio
using namespace std;


namespace Slyvina {
	namespace Units {

		static map<string, string> DVar{};
		//static map<string, string> ADrv;

		void DirryInit(bool force = false) {
			static bool notdoneyet = true;
			//DVar = map<string, string>();
			//DVar.clear();
			if (notdoneyet || force) {
				notdoneyet = false;
#ifdef unix
				DVar["HomeDrive"] = "/";
				DVar["Home"] = getenv("HOME");
#elif defined(_WIN32)
#ifdef Dirry_VisualStudio
				char* fuck;
				char* you;
				// Those variable names are a notion to Microschoft!
				size_t bc = 200;
				auto e1 = _dupenv_s(&fuck, &bc, "HOMEDRIVE");
				auto e2 = _dupenv_s(&you, &bc, "HOMEPATH");
				if ((!e1) && (!e2) && fuck && you) {
					string H = fuck; H += you;
					string D = fuck;
					//cout << "Addressing " << D << " to drive and " << H << " to home\n";
					DVar["HomeDrive"] = D; // string(fuck);
					DVar["Home"] = ChReplace(H, '\\', '/');
				} else {
					DVar["HomeDrive"] = "";
					DVar["Home"] = "";
				}
				free(fuck);
				free(you);

#else
				DVar["HomeDrive"] = getenv("HOMEDRIVE");
				DVar["Home"] = TReplace(DVar["HomeDrive"] + getenv("HOMEPATH"), '\\', '/');
#endif
#endif
				DVar["AppSupport"] = DVar["Home"] + "/.Tricky__ApplicationSupport";
			}
		}

		string Dirry(string path) {
			auto ret = path;
			DirryInit();
			for (auto dostuff : DVar) {
				// cout << "Dirry debug: $" << dostuff.first << "$ = " << dostuff.second << endl;
				ret = StReplace(ret, "$" + dostuff.first + "$", dostuff.second);
			}
			// cout << "Dirry over!\n";
			return ret;
		}

		void DirryVar(string var, string value) {
			DirryInit();
			if (value == "") {
				if (DVar.count(value)) DVar.erase(value);
			} else {
				DVar[var] = ChReplace(value, '\\', '/');
			}
		}

		string DirryVar(string var) {
			DirryInit();
			if (DVar.count(var)) return DVar[var];
			return "";
		}

	}
}