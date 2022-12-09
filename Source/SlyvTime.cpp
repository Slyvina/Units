// Lic:
// Units/Source/SlyvTime.cpp
// Slyvina - Time
// version: 22.12.09
// Copyright (C) 2021, 2022 Jeroen P. Broks
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
#include <string>
#include <time.h>
using namespace std;
namespace Slyvina {
	namespace Units {
		union tc {
			char str[256];
			time_t raw;
		};

		/*
		std::string TrickyUnits::GetTimeBuff() {
			tc t;
			time(&t.raw);
			return t.str;
		}
		*/

		// Avoid Microsoft's "unsafe" whinings.

		tm _localtime(time_t* fuck) {
			tm r;
			//cout << fuck << "\n";
			localtime_s(&r, fuck);
			//cout << "well?\n";
			//localtime(fuck);
			return r;
		}

		tm LocalTime() {
			time_t t;
			time(&t);
			return _localtime(&t);
		}

		string CurrentDate() {
			time_t t;
			char buff[256];
			time(&t);
			auto loctime{ _localtime(&t) };
			strftime(buff, 256, "%d %b %Y", &loctime);
			//cout << "Out date\n";
			return buff;
		}

		string CurrentTime() {
			time_t t;
			char buff[256];
			time(&t);
			//strftime(buff, 256, "%H:%M:%S", &_localtime(&t));
			auto loctime{ _localtime(&t) };
			strftime(buff, 256, "%H:%M:%S", &loctime);
			//cout << "Out Time\n";
			return buff;
		}
		std::string QTimeF(const char* f) {
			time_t t;
			char buff[256];
			time(&t);
			//strftime(buff, 256, f, &_localtime(&t));
			auto loctime{ _localtime(&t) };
			strftime(buff, 256, "%d %b %Y", &loctime);
			return buff;
		}


	}
}