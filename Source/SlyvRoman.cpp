// Lic:
// Units/Source/SlyvRoman.cpp
// Roman Numbers
// version: 24.10.08
// Copyright (C) 2021, 2022, 2023, 2024 Jeroen P. Broks
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
#include <math.h>

namespace Slyvina {
	namespace Units {
		std::string ToRoman(int num) {
			using namespace std;

			if (num == 0) return "O";

			int
				thousand = 0,
				hundred = 0,
				ten = 0,
				one = 0,
				rest = 0;
			string ret = "";

			// Cut it up first
			thousand = (int)floor((double)num / 1000);
			rest = num % 1000;
			hundred = (int)floor((double)rest / 100);
			rest = rest % 100;
			ten = (int)floor((double)rest / 10);
			one = rest % 10;
#ifdef showbreakdown
			Console.Write("Debug:\t", thousand, "\t", hundred, "\t", ten, "\t", one, "\n");
#endif


			// When there are too many thousands, let's cut that up then
			if (thousand > 5)
				ret = "(" + ToRoman(thousand) + " * M) + ";
			else
				for (int i = 1; i <= thousand; ++i)
					ret += "M";
			//end
			//end

			switch (hundred) {
			case 1:
			case 2:
			case 3:
				for (int i = 1; i <= hundred; ++i)
					ret += "C";
				//end
				break;
			case 4:
				ret += "CD";
				break;
			case 5:
				ret += "D";
				break;
			case 6:
			case 7:
			case 8:
				ret += "D";
				for (int i = 1; i <= hundred - 5; ++i)
					ret += "C";
				//end
				break;
			case 9:
				ret += "CM";
				break;
			}

			switch (ten) {
			case 1:
			case 2:
			case 3:
				for (int i = 1; i <= ten; ++i)
					ret += "X";
				//end
				break;
			case 4:
				ret += "XL";
				break;
			case 5:
				ret += "L";
				break;
			case 6:
			case 7:
			case 8:
				ret += "L";
				for (int i = 1; i <= ten - 5; ++i)
					ret += "X";
				//end
				break;
			case 9:
				ret += "XC";
				break;

			}

			switch (one) {
			case 1:
			case 2:
			case 3:
				for (int i = 1; i <= one; ++i)
					ret += "I";
				//end
				break;
			case 4:
				ret += "IV";
				break;
			case 5:
				ret += "V";
				break;
			case 6:
			case 7:
			case 8:
				ret += "V";
				for (int i = 1; i <= one - 5; ++i)
					ret += "I";
				//end
				break;
			case 9:
				ret += "IX";
				break;
			}

			return ret;
		}
	}
}