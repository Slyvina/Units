// License:
// 	Units/Source/SlyvTime.cpp
// 	Slyvina - Time
// 	version: 24.12.07 II
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

#include <string>
#include <time.h>
#include "SlyvTime.hpp"
#include <Slyvina.hpp>
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

#ifdef SlyvWindows
		tm _localtime(time_t* fuck) {
			tm r;
			//cout << fuck << "\n";
			localtime_s(&r, fuck);
			//cout << "well?\n";
			//localtime(fuck);
			return r;
		}
#endif

		tm LocalTime() {
#ifdef SlyvWindows
			time_t t;
			time(&t);
			return _localtime(&t);
#else
			time_t current_time;

			// get the current time using time()
			current_time = time(NULL);

			// call the function localtime() that takes timestamp and convert it into localtime representation
			tm* tm_local = localtime(&current_time);
			tm ret{ *tm_local };
			return ret;
#endif
		}

		time_t TimeStamp() {
			time_t t;
			time(&t);
			return t;
		}

		string CurrentDate() {
			char buff[256];
#ifdef SlyvWindows
			time_t t;
			time(&t);
			auto loctime{ _localtime(&t) };
#else 
			auto loctime{ LocalTime() };
#endif
			strftime(buff, 256, "%d %b %Y", &loctime);
			//cout << "Out date\n";
			return buff;
		}

		int CurrentYear() {
			char buff[256];
#ifdef SlyvWindows
			time_t t;
			time(&t);
			auto loctime{ _localtime(&t) };
#else 
			auto loctime{ LocalTime() };
#endif			
			strftime(buff, 256, "%Y", &loctime);
			try {
				return stoi(buff);
			} catch (exception e) {
				return 0;
			}
		}

		int CurrentMonth() {
			char buff[256];
#ifdef SlyvWindows
			time_t t;
			time(&t);
			auto loctime{ _localtime(&t) };
#else 
			auto loctime{ LocalTime() };
#endif			
			strftime(buff, 256, "%m", &loctime);
			try {
				return stoi(buff);
			} catch (exception e) {
				return 0;
			}
		}

		std::string CurrentMonthName() { return QTimeF("%B"); }

		int CurrentDay() {
			char buff[256];
#ifdef SlyvWindows
			time_t t;
			time(&t);
			auto loctime{ _localtime(&t) };
#else 
			auto loctime{ LocalTime() };
#endif			
			strftime(buff, 256, "%d", &loctime);
			try {
				return stoi(buff);
			} catch (exception e) {
				return 0;
			}
		}

		int CurrentHour() {
			try {
				return stoi(QTimeF("%H"));
			} catch (exception e) {
				return 0;
			}

		}
		int CurrentMinute() {
			try {
				return stoi(QTimeF("%M"));
			} catch (exception e) {
				return 0;
			}

		}
		int CurrentSecond() {
			try {
				return stoi(QTimeF("%S"));
			} catch (exception e) {
				return 0;
			}

		}

		std::string WeekDay() { return QTimeF("%A"); }

	

		int DayOfYear() {		
			try {
				return stoi(QTimeF("%j"));
			} catch (exception e) {
				return 0;
			}
		}


		string CurrentTime() {
			char buff[256];
#ifdef SlyvWindows
			time_t t;
			time(&t);
			auto loctime{ _localtime(&t) };
#else 
			auto loctime{ LocalTime() };
#endif						
			strftime(buff, 256, "%H:%M:%S", &loctime);
			//cout << "Out Time\n";
			return buff;
		}

		std::string QTimeF(const char* f) {
			char buff[256];
#ifdef SlyvWindows
			time_t t;
			time(&t);
			auto loctime{ _localtime(&t) };
#else 
			auto loctime{ LocalTime() };
#endif						
			strftime(buff, 256, f, &loctime);
			return buff;
		}


	}
}
