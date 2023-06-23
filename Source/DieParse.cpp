// Lic:
// Units/Source/DieParse.cpp
// Die Parse
// version: 23.06.23
// Copyright (C) 2023 Jeroen P. Broks
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
#include <DieParse.hpp>
#include <SlyvRandom.hpp>
#include <SlyvString.hpp>

#define DieParseTest

#ifdef DieParseTest
#include <iostream>
#define Chat(abc) std::cout << "TEST>\t"<<abc<<std::endl
#else
#define Chat(abc)
#endif

namespace Slyvina {
	namespace Units {

		typedef byte (* TD)();

		byte D2() { return (byte)Rand.Get(1, 2); }
		byte D4() { return (byte)Rand.Get(1, 4); }
		byte D6() { return (byte)Rand.Get(1, 6); }
		byte D8() { return (byte)Rand.Get(1, 8); }

		byte D10(bool zero) { if (zero) return (byte)Rand.Get(0, 9); else return (byte)Rand.Get(1, 10); }

		byte D12() { return (byte)Rand.Get(1, 12); }
		byte D20() { return (byte)Rand.Get(1, 20); }

		byte D30() { return (byte)Rand.Get(1, 30); }
		byte D100(bool zero) { return (byte)((D10(true) * 10) + D10(zero)); }

		static byte DD10() { return D10(); }
		static byte DD100() { return D100(); }
		
		const byte TDM{ 9 };
		static const TD Dice[] {D2,D4,D6,D8,DD10,D12,D20,D30,DD100};
		static std::map<byte, TD> MDice{ {2, D2}, { 4,D4 }, { 6,D6 }, { 10,DD10 }, { 20,D20 }, { 100,DD100 }};

		byte QDMax{ 10 };
		std::string QDParse(std::string p) {
			std::string ret{ Upper(p) };
			for (byte a = QDMax; a; a--) {
				for (byte di = 100; di; di--) if (MDice.count(di)) {
					auto D{ MDice[di] };
					std::string r{ "" };
					for (byte b = a; b; b--) {
						if (r.size()) r += " + ";
						r += std::to_string(D());
					}
					r = "(" + r + ")";
					//Chat(TrSPrintF("%dD%d", a, di));
					ret = StReplace(ret, TrSPrintF("%dD%d", a, di), r);
				}
			}
			return ret;
		}
		
	}

}

#ifdef DieParseTest
#include <iostream>
int main(int c, char** a) {
	using namespace std;
	using namespace Slyvina;
	using namespace Units;
	cout << QDParse("4d6+8") << endl;
	cout << QDParse("4D6+8") << endl;
	cout << QDParse("10D4+10D20") << endl;
	cout << QDParse("10D10") << endl;
}
#endif