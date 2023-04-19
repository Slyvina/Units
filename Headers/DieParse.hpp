#pragma once
#include <Slyvina.hpp>

namespace Slyvina {
	namespace Units {
		byte D2();
		byte D4();
		byte D6();
		byte D8();
		byte D10(bool zero = true);
		byte D12();
		byte D20();
		byte D30();
		byte D100(bool zero = true);


		// Quick and Dirty Parse! It's not fast or anything, but it works, and that's what matters.
		extern byte QDMax;
		std::string QDParse(std::string p);
	}
}
