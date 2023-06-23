// Lic:
// Units/Headers/SlyvMath.hpp
// Extra Math Routines
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
#pragma once
#include <cmath>

namespace Slyvina {
	namespace Units {

		const double pi{ 3.14159265359 };

		inline double DegSin(double i) { return (sin(i * (180 * pi))); }
		inline double DegSin(int i) { return DegSin((double)i); }
		inline double DegCos(double i) { return (cos(i * (180 * pi))); }
		inline double DegCos(int i) { return DegCos((double)i); };
	}
}