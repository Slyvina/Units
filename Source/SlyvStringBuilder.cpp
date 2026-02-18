// License:
// 	Units/Source/SlyvStringBuilder.cpp
// 	...
// 	version: 25.10.12
// 
// 	Copyright (C) 2025 Jeroen P. Broks
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
#include <SlyvStringBuilder.hpp>
#define UltraMax 50000

#undef MemDebug
#undef AppDebug

#ifdef MemDebug
#define MemChat(a) std::cout << "String Builder Mem: " << a << "\n"
#else
#define MemChat(a)
#endif // MemDebug


#ifdef AppDebug
#define AppChat(a) std::cout << "String Builder App: " << a << "\n"
#else
#define AppChat(a)
#endif // AppDebug


namespace Slyvina {
	namespace Units {

		void _StringBuilder::_poke(size_t p,char c) {
			if (p>=act_size) {
				size_t old_size{act_size};
				if (UltraMax-act_size<act_size) {
					std::cout << "\x07\x1b[91mWARNING!\x1b[37m: String builder size went beyond UltraMax ("<<UltraMax<<")\n";
					act_size=UltraMax;
				} else {
					act_size+=act_size;
				}
				MemChat("Size increased from "<<old_size<<" to "<< act_size << " -> " <<_ts << " "<<((int)c)<<"::"<<c);
				char* newstring{new char[act_size]};
				if (_ts) {
					for(size_t fp=0;fp<old_size;fp++) {
						newstring[fp]=_ts[fp];
						if (!_ts[fp]) break;
					}
					delete[] _ts;
				}
				_ts=newstring;
			}
			if (p<act_size) _ts[p]=c;
		}


		_StringBuilder::_StringBuilder(size_t s) {
			act_size=std::max(s,(size_t)16);
			_ts = new char[act_size];
			_ts[0]=0;
		}

		void _StringBuilder::operator=(std::string s) {
			size_t i{0};
			//size_t sz=s.size();
			MemChat("Defining: "<<s);
			do {
				_poke(i,s[i]);
			} while (s[i++]);
			str_size=s.size();
		}

		void _StringBuilder::operator=(int i) { (*this)=std::to_string(i); }

		void _StringBuilder::_appendchar(char c) {
			AppChat("Appending char: "<<(int)c<<"::"<<c);
			_poke(  str_size,c);
			_poke(++str_size,0);
		}

		void _StringBuilder::_append(const char *c) {
			size_t p{0};
			do { if (c[p]) _appendchar(c[p]); } while ( c[p++] );
			AppChat("Appended string: "<< c << "->" << _ts);
		}
	}
}
