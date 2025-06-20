// License:
// 	Units/Headers/SlyvTime.hpp
// 	Slyvina - Time (header)
// 	version: 25.03.01
// 
// 	Copyright (C) 2021, 2022, 2023, 2024, 2025 Jeroen P. Broks
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

#pragma once

#include <time.h>
#include <string>
#include <Slyvina.hpp>

namespace Slyvina {
	namespace Units {
		//std::string GetTimeBuff();
		std::string CurrentDate();
		std::string CurrentTime();
		std::string QTimeF(const char* f);
		int CurrentYear(); // The year
		int CurrentMonth(); // The number of the month
		std::string CurrentMonthName();
		int CurrentDay(); // Day in the month
		int DayOfYear(); // Day of the year
		int CurrentHour(); // Hour
		int CurrentMinute(); // Minute
		int CurrentSecond(); // Second
		std::string WeekDay(); // Day of the week fully written out.
		inline std::string Now() { return CurrentDate()+"; "+CurrentTime(); }
#ifdef SlyvWindows
		tm _localtime(time_t* fuck);
#endif
		tm LocalTime();
		time_t TimeStamp();
	}
}

