// Lic:
// Units/Source/SlyvBank.cpp
// Slyvina - Banking
// version: 23.11.07
// Copyright (C) 2022, 2023 Jeroen P. Broks
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

#include <SlyvEndian.hpp>
#include <SlyvBank.hpp>

using namespace std;

#define MPoke(_position,_type,_fld,_value) \
	__AllNum O,T;\
	O._fld = _value;\
	switch (_endian) {\
		case Endian::Ignore: T=O; break; \
		case Endian::Little: T._fld = ToLittle((_type)_value); break; \
		case Endian::Big: T._fld = ToBig((_type)_value); break; \
		default: Panic("Illegal Endian While Poking"); return;\
	}\
	for(size_t i=0;i<sizeof(_type);i++) PokeChar(i+_position,T.check[i]);\

//std::cout << "MPoke("<<_position<<",<<"<<sizeof(_type)<<">>,"<<_value<<") -> "<<T._fld<<" \n";\
//for(size_t i=0;i<sizeof(_type);i++) std::cout<< i+_position<<":"<<(int)T.check[i]<<"\n";


#define MPeek(_position,_type,_fld) \
	__AllNum O;\
	for(size_t i=0;i<sizeof(_type);i++) O.check[i] = PeekChar(i+_position);\
	switch (_endian) {\
		case Endian::Ignore: return O._fld; break; \
		case Endian::Little: return FromLittle(O._fld); break; \
		case Endian::Big: return FromBig(O._fld); break; \
		default: Panic("Illegal Endian While Peeking"); return 0;\
	}\
	


namespace Slyvina {
	namespace Units {
		static void DefaultPanic(std::string err) { std::cout << "Slyvina Bank Error> " << err << std::endl; }


		BankPanic _Bank::_DefPanic{ DefaultPanic };

		_Bank::_Bank(size_t size,Endian E) {
			_buffer = new char[size];
			_sz = size;
			_endian = E;
			Panic = _DefPanic;
		}
		_Bank::_Bank(char* buf, size_t size, Endian SetEndian) {
			_buffer = buf;
			_sz = size;
			_endian = SetEndian;
			Panic = _DefPanic;
		}

		_Bank::_Bank(Endian SetEndian ) {
			_endian = SetEndian;
			Panic = _DefPanic;
			_Expandable = true;
			_xbuffer = std::make_shared<vector<char>>();			
		}

		_Bank::~_Bank() {
			if (!_Expandable)
				delete[] _buffer;
			// Expandable uses a shared pointer, so no need to release that.
		}
		void _Bank::DefineDefaultBankPanic(BankPanic p) { _DefPanic = p; }
		void _Bank::PokeChar(size_t position, char value) {
			if (_Expandable) {
				while (position >= _xbuffer->size()) _xbuffer->push_back('\0');
				(*_xbuffer)[position] = value;
			} else {
				if (position < _sz) { Panic("Position out of range (" + to_string(position) + "/" + to_string(_sz) + ")"); return; }
				_buffer[position] = value;
			}
		}

		void _Bank::PokeByte(size_t position, byte value) {
			__AllNum B; 
			B.bt = value;
			PokeChar(position,B.ch);
		}

		void _Bank::PokeInt16(size_t position, int16 value) { MPoke(position, int16, i16, value); }
		void _Bank::PokeInt32(size_t position, int32 value) { MPoke(position, int32, i32, value); }
		void _Bank::PokeInt64(size_t position, int64 value) { MPoke(position, int64, i64, value); }
		void _Bank::PokeUInt16(size_t position, uint16 value) { MPoke(position, uint16, i16, value); }
		void _Bank::PokeUInt32(size_t position, uint32 value) { MPoke(position, uint32, i32, value); }
		void _Bank::PokeUInt64(size_t position, uint64 value) { MPoke(position, uint64, i64, value); }

		char _Bank::PeekChar(size_t position) {
			if (_Expandable) {
				auto _sz = _xbuffer->size();
				if (position > _sz ) { Panic("Position out of range (" + to_string(position) + "/" + to_string(_sz) + ")"); return 0; }
				return (*_xbuffer)[position];
			} else {
				if (position > _sz) { Panic("Position out of range (" + to_string(position) + "/" + to_string(_sz) + ")"); return 0; }
				return _buffer[position];
			}
		}

		byte _Bank::PeekByte(size_t position) {
			__AllNum B;
			B.ch = PeekChar(position);
			return B.bt;
		}

		int16 _Bank::PeekInt16(size_t position) { MPeek(position, int16, i16); }
		int32 _Bank::PeekInt32(size_t position) { MPeek(position, int32, i32); }
		int64 _Bank::PeekInt64(size_t position) { MPeek(position, int64, i64); }
		uint16 _Bank::PeekUInt16(size_t position) { MPeek(position, uint16, ui16); }
		uint32 _Bank::PeekUInt32(size_t position) { MPeek(position, uint32, ui32); }
		uint64 _Bank::PeekUInt64(size_t position) { MPeek(position, uint64, ui64); }

		void _Bank::chcpy(char* buf, size_t sz) {
			for (size_t i = 0; i < sz; ++i) buf[i] = ReadChar();
		}

		std::string _Bank::ReadString(size_t sz) {
			if (!sz) sz = ReadInt();
			char* c_ret = new char[sz+1];
			chcpy(c_ret, sz);
			c_ret[sz] = '\0';
			std::string ret{ c_ret };
			delete[] c_ret;
			return ret;
		}

		std::string _Bank::ReadNullString(size_t sz) {
			std::string ret{ "" };
			if (sz) {
				auto cstr{ new char[sz] };
				for (size_t i = 0; i < sz; i++) cstr[sz] = ReadChar();
				ret = cstr;
				delete[] cstr;
			} else {
				char ch;
				while (ch = ReadChar()) ret += ch;
			}
			return ret;
		}

		void _Bank::ReadNullString(char* str, size_t bufsize, size_t blocksize) {
			if (blocksize) blocksize = bufsize;
			for (size_t i = 0; i < blocksize; i++) {
				auto ch{ ReadChar() };
				if (i == bufsize - 1) str[i] = 0;
				else if (i < bufsize) str[i] = ch;
			}
			if (bufsize > blocksize) str[blocksize] = 0;
		}

		void _Bank::WriteNullString(std::string str,size_t bufsize) {
			if (!bufsize) bufsize = str.size() + 1;
			for (size_t i = 0; i < bufsize - 1; i++) {
				if (i < str.size()) Write(str[i]); else Write('\0');
			} 
			Write('\0');
		}

		void _Bank::WriteStringMap(StringMap sm) {
			for (auto ism : *sm) {
				WriteByte(1);
				Write(ism.first);
				Write(ism.second);
			}
		}

		void _Bank::ToChar(char* ch) {
			if (!_Expandable)
				for (size_t i = 0; i < _sz; i++)
					ch[i] = _buffer[i];
			else
				for (size_t i = 0; i < _xbuffer->size(); i++)
					ch[i] = (*_xbuffer)[i];
		}

		char* _Bank::ToChar() {
			auto ret = new char[Size()];
			ToChar(ret);
			return ret;
		}

		std::string _Bank::ToString() {
			std::string ret;
			char* retbuf = new char[Size()+1];
			ToChar(retbuf);
			retbuf[Size()] = '\0'; // Make sure there's a null terminator or this won't work!
			ret = retbuf;
			delete[] retbuf;
			return ret;
		}

		void _Bank::chcpy(char* buf, size_t pos, size_t sz) {
			for (size_t i = 0; i < sz; i++) buf[i] = PeekChar(pos + i);
		}

		void _Bank::GetChars(char* buf, size_t sz) {
			for (size_t i = 0; i < sz; i++) buf[i] = ReadChar();
		}
		
		Bank CreateBank(size_t size, Endian E) {
			return std::make_shared<_Bank>(size,E);
		}

		Bank CreateBank(char* buf, size_t size, Endian E) {
			auto ret = CreateBank(size, E);
			for (size_t i = 0; i < size; i++) ret->PokeChar(i,buf[i]);
			return ret;
		}

		Bank CreateBank(std::vector<char> buf, Endian E) {
			auto ret = CreateBank(buf.size(), E);
			for (size_t i = 0; i < buf.size(); i++) ret->PokeChar(i, buf[i]);
			return ret;
		}

		Bank TurnToBank(char* buf, size_t size, Endian E) {
			return std::make_shared<_Bank>(buf, size, E);
		}

		Bank CreateXBank(Endian E) {
			return std::make_shared<_Bank>();
		}

		UBank CreateUXBank() {
			return std::make_unique<_Bank>();
		}


		UBank CreateUBank(size_t size, Endian E) {
			return std::make_unique<_Bank>(size, E);
		}

	}
}