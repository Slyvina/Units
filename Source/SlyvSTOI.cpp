// License:
// 	Units/Source/SlyvSTOI.cpp
// 	Slyvina - Alternate STOI
// 	version: 24.10.20
// 
// 	Copyright (C) 2021, 2022, 2023, 2024 Jeroen P. Broks
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
// Lic:
// Units/Source/SlyvSTOI.cpp
// Slyvina - Alternate STOI
// version: 23.06.23
// Copyright (C) 2021, 2022, 2023 Jeroen P. Broks
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
#undef TrickySTOI_DEBUG
#ifdef TrickySTOI_DEBUG
#include <iostream>
#endif
#include <string>
#include <SlyvString.hpp>
#include <math.h>
#include "SlyvSTOI.hpp"

#define eReturn(ErrMsg,retvalue) {STOI_Error = ErrMsg; return retvalue;}

namespace Slyvina {
	namespace Units {

		std::string STOI_Error{ "" };

		int TrueToInt(std::string str, int scale) {
			STOI_Error = "";
			if (!str.size()) eReturn("No string to convert", 0);
			if (scale > 16 || scale < 2) eReturn("Invalid Scale", 0); // Invalid scale
			int l = str.size() - 1;
			auto s = Upper(str);
			int ret{ 0 };
			for (int p = 0; p <= l; ++p) {
				int digi{ 0 };
				auto ch = s[l - p];
				if (ch >= '0' && ch <= '9') digi = (int)ch - 48;
				else if (ch >= 'A' && ch <= 'F') digi = (int)ch - 55;
				else eReturn(std::string("Character '") + ch + "' not recognized in string to integer conversion!", 0);
				if (digi >= scale) eReturn(std::string("Digit ") + std::to_string(digi) + std::string(" beyond scale (") + std::to_string(scale) + ")", 0);
				if (p == 0)
					ret = digi;
				else
					ret += digi * (pow(scale, p));
			}
			eReturn("", ret);
		}

		int ToInt(std::string s) {
			if (!s.size()) return 0;
			s = Trim(s);
			if (!s.size()) return 0;
			auto r = Right(s, s.size() - 1);
			if (s[s.size() - 1] == '-') return -((int)abs(ToInt(Left(s, s.size() - 1))));
			switch (s[0]) {
			case '-':       return -TrueToInt(r, 10);
			case '$':
			case 'x':       return TrueToInt(r, 16);
			case '%':
			case 'b':       return TrueToInt(r, 2);
			case 'o':       return TrueToInt(r, 8);
			default:        return TrueToInt(s, 10);
			}
		}

		unsigned int ToUInt(std::string s) {
			if (!s.size()) return 0;
			s = Trim(s);
			if (!s.size()) return 0;
			auto r = Right(s, s.size() - 1);
			switch (s[0]) {
			case '$':
			case 'x':       return (unsigned int)TrueToInt(r, 16);
			case '%':
			case 'b':       return (unsigned int)TrueToInt(r, 2);
			case 'o':       return (unsigned int)TrueToInt(r, 8);
			default:        return (unsigned int)TrueToInt(s, 10);
			}
		}

		bool IsInt(std::string s) {
			if (!s.size()) return false;
			for (size_t i = 0; i < s.size(); i++) {
				if (s[i] < '0' || s[i]>'9') {
					if (i > 0 && s[i] != '-')
						return false;
				}
			}
			return true;
		}

		bool IsNum(std::string s) {
			bool point{ false };
			if (!s.size()) return false;
			for (size_t i = 0; i < s.size(); i++) {
				if (s[i] < '0' || s[i]>'9') {
					if (s[i] == '.') {
						if (point) return false;
						point = true;
					} else if (i > 0 && s[i] != '-') {
						return false;
					}
				}
			}
			return true;
		}
		




	}
}

#ifdef TrickySTOI_DEBUG
using namespace Slyvina::Units;
int main(int an, char** A) {
	std::cout << TrueToInt("123", 10) << "\n";
	std::cout << TrueToInt("3F", 16) << "\n";
	std::cout << ToInt("10") << "\t" << ToInt("$10") << "\t" << ToInt("o10") << "\t" << ToInt("b10") << "\t" << ToInt("-10") << std::endl;
	std::cout << ToInt("b11111111") << "\n";
}
#endif
