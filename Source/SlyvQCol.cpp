// License:
// 	Units/Source/SlyvQCol.cpp
// 	Slyvina - Quick Colors
// 	version: 24.10.30
// 
// 	Copyright (C) 2022, 2023, 2024 Jeroen P. Broks
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

#include <Slyvina.hpp>
#if defined(_MSC_VER) && defined(SlyvWindows)
#define QCOLVS
#endif

//#ifdef SlyvWindows
#ifdef QCOLVS
#include <Windows.h>
#endif

#include <iostream>
//#include "../Headers/Platform.hpp"
#include <SlyvQCol.hpp>
#include <SlyvString.hpp>


using namespace std;


namespace Slyvina {
	namespace Units {

		bool qColErrorShow{ true };
		static void err(string er) {
			if (qColErrorShow) cout << "QCOL ERROR> " << er << endl;
		}

#pragma region Windows Console
		static void Win_Color(qColor f, qColor b) {
			if ((int)f < 0 || (int)f>15) { err("Foreground out of bounds!"); return; }
			if ((int)b < 0 || (int)b>15) { err("Background out of bounds!"); return; }
			auto
				fg{ (byte)f },
				bg{ (byte)b };
//#ifndef SlyvWindows
#ifndef QCOLVS
			err("This driver only works in Windows when compiled with visual C++!");
			return;
#else
			auto hConsole{ GetStdHandle(STD_OUTPUT_HANDLE) };
			SetConsoleTextAttribute(hConsole, fg + (bg * 16));
#endif

		}

		void Win_Reset() { WinQCol.Color(qColor::Grey, qColor::Black); }
#pragma endregion


#pragma region ANSI
		byte W2A(qColor c) {
			switch (c) {
			case qColor::Black: return 0;
			case qColor::DarkGrey: return 60;
			case qColor::Red: return 1;
			case qColor::Pink: return 61;
			case qColor::Green: return 2;
			case qColor::LightGreen: return 62;
			case qColor::Brown: return 3;
			case qColor::Yellow: return 63;
			case qColor::Blue: return 4;
			case qColor::LightBlue: return 64;
			case qColor::Magenta: return 5;
			case qColor::LightMagenta: return 65;
			case qColor::Cyan: return 6;
			case qColor::LightCyan: return 66;
			case qColor::Grey: return 7;
			case qColor::White: return 67;
			default:
				err("Unknown color! I cannot convert this to ANSI! (" + to_string((byte)c) + ")");
				return 0;
			}
		}

		static void ANSI_Color2(qColor f, qColor b) {
			if ((int)f < 0 || (int)f>15) { err("Foreground out of bounds!"); return; }
			if ((int)b < 0 || (int)b>15) { err("Background out of bounds!"); return; }
			auto
				fg{ W2A(f) },
				bg{ W2A(b) };
			printf("\x1b[%dm\x1b[%dm", 30 + fg, 40 + bg);
		}
		static void ANSI_Color1(qColor f) {
			if ((int)f < 0 || (int)f>15) { err("Foreground out of bounds!"); return; }
			auto
				fg{ W2A(f) };
			printf("\x1b[%dm", 30 + fg);
		}

		static void ANSI_Reset() { printf("\x1b[0m"); }
#pragma endregion

		TmpPlateQCol
			WinQCol{ Win_Color,Win_Reset,"Windows Console" },
			ANSI{ ANSI_Color1, ANSI_Color2,ANSI_Reset, "ANSI" },
//#ifdef SlyvWindows
#ifdef QCOLVS
			* QCol{ &WinQCol };
#else
			#undef QCol
			* QCol{ &ANSI };
			#define QCol Slyvina::Units::QCol
			// GCC wouldn't accept this otherwise, you know!
#endif



		void TmpPlateQCol::Write(qColor c, std::string w) {
			//Color(c, qColor::Black);
			Color(c);
			//printf(StReplace(w,"%","%%").c_str());
			printf("%s", w.c_str());
		}

		void TmpPlateQCol::Doing(string a, string b, string cl) {
			Write(Doing1, a + ":");
			for (auto i = a.size(); i < DoingTab; ++i) printf(" ");
			Write(Doing2, b);
			printf("%s",cl.c_str()); // GCC whines over this one (although it doesn't throw an error.
		}
		void TmpPlateQCol::Doing(string a, int b, string cl) { Doing(a, to_string(b), cl); }

		void TmpPlateQCol::Error(string msg) {
			Write(Error1, "ERROR> ");
			Write(Error2, msg);
			cout << endl;
		}
		void TmpPlateQCol::Warn(string msg) {
			Write(Error1, "WARNING> ");
			Write(Error2, msg);
			cout << endl;
		}

		void TmpPlateQCol::Green(string a) { Write(qColor::Green, a); }
		void TmpPlateQCol::LGreen(string a) { Write(qColor::LightGreen, a); }
		void TmpPlateQCol::Red(string a) { Write(qColor::Red, a); }
		void TmpPlateQCol::Pink(string a) { Write(qColor::Pink, a); }
		void TmpPlateQCol::Dark(string a) { Write(qColor::DarkGrey, a); }
		void TmpPlateQCol::Yellow(string a) { Write(qColor::Yellow, a); }
		void TmpPlateQCol::Cyan(string a) { Write(qColor::Cyan, a); }
		void TmpPlateQCol::LCyan(string a) { Write(qColor::LightCyan, a); }
		void TmpPlateQCol::Blue(string a ) { Write(qColor::Blue, a); }
		void TmpPlateQCol::LBlue(string a ) { Write(qColor::LightBlue, a); }
		void TmpPlateQCol::Magenta(string a) { Write(qColor::Magenta, a); }
		void TmpPlateQCol::LMagenta(std::string a) { Write(qColor::LightMagenta, a); }
		void TmpPlateQCol::Grey(string a) { Write(qColor::Grey, a); }
		void TmpPlateQCol::White(std::string a) { Write(qColor::White, a); }

		TmpPlateQCol::TmpPlateQCol(QColColor2 c, QColReset r, string n) {
			Color1 = nullptr;
			Color2 = c;
			Reset = r;
			Name = n;
		}
		TmpPlateQCol::TmpPlateQCol(QColColor1 c1, QColColor2 c2, QColReset r, std::string n) {
			Color1 = c1;
			Color2 = c2;
			Reset = r;
			Name = n;
		}
	}
}
