// License:
// 	Units/Headers/SlyvStringBuilder.hpp
// 	String Builder
// 	version: 25.06.16
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
#pragma once

#include <Slyvina.hpp>

#include <string.h>

#include <memory>


namespace Slyvina {
	namespace Units {

		class _StringBuilder;
		typedef std::shared_ptr<_StringBuilder> StringBuilder;
		typedef std::unique_ptr<_StringBuilder> UStringBuilder;

		class _StringBuilder {
			private:
				char* _ts{nullptr};
				size_t act_size{16};
				size_t str_size{0};
				void _poke(size_t pos,char c);
				void _append(const char* s);
				void _appendchar(char c);
			public:
				_StringBuilder() {}; // NEVER use this! It just must exist or the C++ compiler may go haywire.
				_StringBuilder(size_t s);
				inline char* c_str() { return _ts; }
				inline size_t size() { return str_size; }
				inline size_t npsize() { return strlen(_ts); }
				void operator=(std::string s);
				void operator=(int i);
				inline void operator+=(std::string s) { _append(s.c_str()); }
				inline void operator+=(int s) { _append(std::to_string(s).c_str()); }
				bool operator==(std::string s) { return SlyvStrCmp(	s.c_str(),	_ts	)==0; }
				bool operator==(_StringBuilder s) { return SlyvStrCmp(s._ts,_ts)==0; }
				bool operator==(int i) { return  SlyvStrCmp(	std::to_string(i).c_str(),	_ts); }
				inline void AppendC(char *C) { _append(C); }
		};

		inline StringBuilder NewSB() { return std::shared_ptr<_StringBuilder>(new _StringBuilder(16)); }
		inline StringBuilder NewSB(size_t s) { return std::shared_ptr<_StringBuilder>(new _StringBuilder(s)); }
		inline StringBuilder NewSB(std::string s) { auto r{ std::shared_ptr<_StringBuilder>(new _StringBuilder(s.size())) }; *r=s; return r;}
	}
}
