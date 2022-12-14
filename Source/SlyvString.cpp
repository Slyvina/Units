// Lic:
// Units/Source/SlyvString.cpp
// Slyvina - Quick String Handler
// version: 22.12.14
// Copyright (C) 2022 Jeroen P. Broks
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
#include <cstdarg>
#include <SlyvString.hpp>

namespace Slyvina {
	namespace Units {

		std::wstring ToWString(const std::string& s) {
			/*
			int len;
			int slength = (int)s.length() + 1;
			len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
			wchar_t* buf = new wchar_t[len];
			MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
			std::wstring r(buf);
			delete[] buf;
			return r;
			*/
			// Not the most beautiful method, but what works that works!
			auto l = s.size();
			wchar_t* rbuf = new wchar_t[l + 1];
			for (size_t i = 0; i < l; ++i) rbuf[i] = (wchar_t)s[i];
			rbuf[l] = '\0'; // Null Terminator
			std::wstring ret = rbuf;
			delete[]rbuf;
			return ret;
		}

		
		std::string FromWString(std::wstring wString) {
			//std::string convLPCWSTRtoString(LPCWSTR wString) {
			//auto size = wcslen(wString) + 1;
			auto size = wString.size();
			char* cString = new char[size];
			for (size_t i = 0; i < size; i++) {
				 cString[i] = wString[i];							
			}
			std::string RetString{ cString };
			delete[] cString;
			return RetString;			
		}

		std::string Mid(std::string str, unsigned int start, unsigned int length) {
			{
				std::string ret = "";
				int istart = start - 1;
				if (start < 1) return ""; // NOT ALLOWED!!!
				for (int i = istart; i < istart + length && i < str.size(); i++) {
					ret += str[i];
				}
				return ret;
			}
		}
		std::string Mid(std::string str, unsigned int start, unsigned int length, std::string newstring) {
			{
				std::string ret = str;
				int istart = start - 1;
				for (int i = 0; i < newstring.size(); i++) {
					if (i = ret.size())
						ret += newstring[i];
					else
						ret[i + istart] = newstring[i];
				}
				return ret;
			}
		}

		int FindLast(std::string str, char ch) {
			int i = str.size();
			do {
				i--;
			} while (i > 0 && str[i] != ch);
			return i;
		}

		int FindLast(std::string haystack, std::string needle) {
			int i = haystack.size() - needle.size();
			if (i < 0) return -1;
			do {
				i--;
			} while (i > 0 && Mid(haystack, i, needle.size()) != needle);
			return i;
		}

		// dah fuck?
		std::string ChReplace(std::string mystr, char ori, char subst) {
			for (unsigned int i = 0; i < mystr.size(); i++) {
				if (mystr[i] == ori) mystr[i] = subst;
			}
			return mystr;
		}

		std::string StReplace(std::string mystr, std::string ori, std::string subst) {
			{				
				std::string ret = "";
				auto olen = mystr.size();
				auto slen = ori.size();
				unsigned p = 1;
				while (p <= olen) {
					if ((p - 1) + slen <= olen && Mid(mystr, p, slen) == ori) {
						//cout << p << "\t" << ori << "\t" << subst << "\t" << mystr << "\t" << ret << endl; // debug only
						ret += subst;
						p += slen;
						//cout << p << ":"<<olen<<endl;
					} else {
						ret += mystr[p - 1];
						p++;
					}
				}
				return ret;
				
			}
		}
		std::string CSReplace(std::string mystr, char ori, std::string subst) {
			std::string ret;
			for (unsigned int i = 0; i < mystr.size(); i++) {
				if (mystr[i] == ori)
					ret += subst;
				else
					ret += mystr[i];
			}
			return ret;
		}

		std::string ExtractDir(std::string file) {
			file = ChReplace(file, '\\', '/');
			int lastslash = FindLast(file, '/');
			if (lastslash < -1) return "";
			return Left(file, lastslash);
		}

		std::string StripDir(std::string file) {
			file = ChReplace(file, '\\', '/');
			int lastslash = FindLast(file, '/');
			if (lastslash < -1) return file;
			auto ret = Right(file, file.size() - lastslash);
			while (ret.size() && ret[0] == '/') ret = Right(ret, ret.size() - 1);
			return ret;
		}

		std::string StripExt(std::string file) {
			file = ChReplace(file, '\\', '/');
			int lastdot = FindLast(file, '.');
			int lastslash = FindLast(file, '/');
			if (lastdot < 0 || lastdot < lastslash) return file;
			return Left(file, lastdot);
		}

		VecString Split(std::string str, char spltchar) {
			auto ret=NewVecString();
			unsigned int idx = 0;
			for (int i = 0; i < str.size(); i++) {
				if (idx >= ret->size()) ret->push_back("");
				if (str[i] == spltchar)
					idx++;
				else
					(*ret)[idx] += str[i];
			}
			return ret;
		}

		VecString Split(std::string str, char spltchar, int max) {
			auto ret = NewVecString();
			unsigned int idx = 0;
			for (int i = 0; i < str.size(); i++) {
				if (idx >= ret->size()) ret->push_back("");
				if (str[i] == spltchar && idx < max)
					idx++;
				else
					(*ret)[idx] += str[i];
			}
			return ret;
		}

		VecString StringToLines(std::string str) {
			auto ret = NewVecString();
			unsigned int idx = 0;
			for (int i = 0; i < str.size(); i++) {
				if (idx >= ret->size()) ret->push_back("");
				if (str[i] == '\n')
					idx++;
				else if (str[i] != '\r')
					(*ret)[idx] += str[i];
			}
			return ret;
		}

		std::string Trim(std::string str) {
			std::string ret = "";
			bool begun = false;
			// before!
			for (int i = 0; i < str.size(); ++i) {
				begun = begun || (str[i] != ' ' && str[i] != '\t' && str[i] != '\r' && str[i] != '\n');
				if (begun)
					ret += str[i];
			}
			if (ret == "") return ""; // No need to go on if the std::string's empty now!
			
			while (
				ret[ret.size() - 1] == ' ' ||
				ret[ret.size() - 1] == '\t' ||
				ret[ret.size() - 1] == '\n' ||
				ret[ret.size() - 1] == '\t'
				) {
				// cout << ret << " (" << ret.size() << ")\n";
				ret = Left(ret, ret.size() - 1);
				if (ret == "") return "";
			}
			return ret; // Should never happen, but at least this prevents crashes, exceptions, whatever!
		}
		long long FindString(std::string Needle, std::string HayStack) {
			auto NS = Needle.size();
			if (Needle.size() > HayStack.size()) return -1;
			if (Needle == HayStack) return 0;
			for (long long i = 0; i < HayStack.size() - Needle.size(); i++) {
				if (Mid(HayStack, i, NS) == Needle) return i;
			}
			return -1;
		}
		std::string ExtractExt(std::string file) {
			file = ChReplace(file, '\\', '/');
			int lastdot = FindLast(file, '.');
			int lastslash = FindLast(file, '/');
			if (lastdot < 0 || lastdot < lastslash) return "";
			return file.substr(lastdot + 1); //left(file, lastdot);
		}



#pragma region TrSprintF
		static const char* scanformat(const char* strfrmt, char* form) {
			const char* L_FMTFLAGS{ "-+ #0" };
			const char* p = strfrmt;
			while (*p != '\0' && strchr(L_FMTFLAGS, *p) != NULL) p++;  /* skip flags */
			if ((size_t)(p - strfrmt) >= sizeof(L_FMTFLAGS) / sizeof(char)) {
				//000001193 | luaL_error(L, "invalid format (repeated flags)");
				std::cout << "Invalid format (repeated flags)\n";
				return NULL;
			}
			if (isdigit((unsigned char)(*p))) p++;  /* skip width */
			if (isdigit((unsigned char)(*p))) p++;  /* (2 digits at most) */
			if (*p == '.') {
				p++;
				if (isdigit((unsigned char)(*p))) p++;  /* skip precision */
				if (isdigit((unsigned char)(*p))) p++;  /* (2 digits at most) */
			}
			if (isdigit((unsigned char)(*p))) {
				//000001202 | luaL_error(L, "invalid format (width or precision too long)");
				std::cout << "invalid format (width or precision too long)\n";
				return NULL;
			}
			*(form++) = '%';
			memcpy(form, strfrmt, ((p - strfrmt) + 1) * sizeof(char));
			form += (p - strfrmt) + 1;
			*form = '\0';
			return p;
		}

		std::string TrSPrintF(const char* fmt, ...) {
			const char L_ESC{ '%' };
			const int MAX_FORMAT{ 32 };
			const int MAX_ITEM{ 120 };
			const int MAX_ITEMF{ 300 };
			va_list args;
			va_start(args, fmt);
			// Quick Alternate sprintf command (based on the Lua std::string.format() function)
			// int top = lua_gettop(L);
			std::string ret{ "" };
			//int arg = 1;
			size_t sfl{ strlen(fmt) };
			const char* strfrmt = fmt; // luaL_checklstd::string(L, arg, &sfl);
			const char* strfrmt_end = strfrmt + sfl;
			//luaL_Buffer b;
			//luaL_buffinit(L, &b);
			while (strfrmt < strfrmt_end) {
				if (*strfrmt != L_ESC)
					ret += (*strfrmt++); //luaL_addchar(&b, *strfrmt++);
				else if (*++strfrmt == L_ESC)
					ret += (*strfrmt++); //luaL_addchar(&b, *strfrmt++);  /* %% */
				else { /* format item */
					char form[MAX_FORMAT];  /* to store the format ('%...') */
					int maxitem = MAX_ITEM;
					char addret[MAX_ITEMF]{ "" };
					//char* buff = luaL_prepbuffsize(&b, maxitem);  /* to put formatted item */
					int nb = 0;  /* number of bytes in added item */
					//if (++arg > top)
					//	return luaL_argerror(L, arg, "no value");
					strfrmt = scanformat(strfrmt, form);
					if (!strfrmt) { va_end(args); return ""; }
					switch (*strfrmt++) {
					case 'c': {
						//nb = l_sprintf(buff, maxitem, form, (int)luaL_checkinteger(L, arg));
						//#define l_sprintf(s,sz,f,i)	((void)(sz), sprintf(s,f,i)) 
						//nb = ((void)(sz), sprintf_s(s, f, i));
						auto i{ va_arg(args, int) };
						sprintf_s(addret, form, i);
						break;
					}
					case 'd': case 'i':
					case 'o': case 'u': case 'x': case 'X': {
						//lua_Integer n = luaL_checkinteger(L, arg);
						//addlenmod(form, LUA_INTEGER_FRMLEN);
						//nb = l_sprintf(buff, maxitem, form, (LUAI_UACINT)n);
						auto i{ va_arg(args, int) };
						sprintf_s(addret, form, i);
						break;
					}
							/*
							case 'a': case 'A':
								addlenmod(form, LUA_NUMBER_FRMLEN);
								nb = lua_number2strx(L, buff, maxitem, form,
									luaL_checknumber(L, arg));
								break;
							//*/
					case 'f':
						maxitem = MAX_ITEMF;  /* extra space for '%f' */
						//buff = luaL_prepbuffsize(&b, maxitem);
						/* FALLTHROUGH */
					case 'e': case 'E': case 'g': case 'G': {
						//lua_Number n = luaL_checknumber(L, arg);
						//addlenmod(form, LUA_NUMBER_FRMLEN);
						//nb = l_sprintf(buff, maxitem, form, (LUAI_UACNUMBER)n);
						auto n{ va_arg(args,double) };
						sprintf_s(addret, form, n);
						break;
					}
							//case 'p': {
							//	const void* p = lua_topointer(L, arg);
							//	if (p == NULL) {  /* avoid calling 'printf' with argument NULL */
							//		p = "(null)";  /* result */
							//		form[strlen(form) - 1] = 's';  /* format it as a std::string */
							//	}
							//	nb = l_sprintf(buff, maxitem, form, p);
							//	break;
							//}
							//case 'q': {
							//	if (form[2] != '\0')  /* modifiers? */
							//		return luaL_error(L, "specifier '%%q' cannot have modifiers");
							//	addliteral(L, &b, arg);
							//	break;
							//}
					case 's': {
						const char* s{ va_arg(args,char*) }; //= luaL_tolstd::string(L, arg, &l);
						size_t l{ strlen(s) };
						if (form[2] == '\0') { /* no modifiers? */
							//luaL_addvalue(&b);  /* keep entire std::string */
							std::string s2; s2 = std::string(s);
							strcpy_s(addret, s2.c_str());
							//strcpy_s(addret, s);
						} else {
							//luaL_argcheck(L, l == strlen(s), arg, "std::string contains zeros");
							if (l != strlen(s)) {
								std::cout << "std::string contains zeros\n"; va_end(args); return "";
							}
							if (!strchr(form, '.') && l >= 100) {
								/* no precision and std::string is too long to be formatted */
								strcpy_s(addret, s); //luaL_addvalue(&b);  /* keep entire std::string */
							} else {  /* format the std::string into 'buff' */
								//nb = l_sprintf(buff, maxitem, form, s);
								//lua_pop(L, 1);  /* remove result from 'luaL_tolstd::string' */
								sprintf_s(addret, form, s);
							}
						}
						break;
					}
					default: {  /* also treat cases 'pnLlh' */
						//return luaL_error(L, "invalid conversion '%s' to 'format'", form);
						printf("invalid conversion '%s' to 'format'", form);
						va_end(args);
						return "";
					}
					}
					//lua_assert(nb < maxitem);
					//luaL_addsize(&b, nb);
					ret += addret;
				}
			}
			//luaL_pushresult(&b);
			//return 1;
			return ret;
		}
#pragma endregion
	}
}