// Lic:
// Units/Headers/SlyvBank.hpp
// Slyvina - Banks (header)
// version: 22.12.12
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
#pragma once
#include "Slyvina.hpp"
#include "SlyvEndian.hpp"

namespace Slyvina {
	namespace Units {
		SlyvClass(_Bank, Bank);

		typedef void(* BankPanic)(std::string error);

		class _Bank {
		private:
			char* _buffer;
			size_t _sz;
			size_t _pos{ 0 };
			Endian _endian;
			static BankPanic _DefPanic;
		public:
			BankPanic Panic{ nullptr };
			_Bank(size_t size, Endian SetEndian = Endian::Little); // NEVER create this class regularly! Always use CreateBank() in stead. Otherwise destructors can mess the entire thing up.
			_Bank(char* buf, size_t size, Endian SetEndian = Endian::Little);
			~_Bank();

			void PokeChar(size_t position, char value);
			void PokeByte(size_t position, byte value);
			void PokeInt16(size_t position, int16 value);
			void PokeInt32(size_t position, int32 value); inline void PokeInt(size_t position, int32 value) { PokeInt32(position, value); }
			void PokeInt64(size_t position, int64 value);
			void PokeUInt16(size_t position, uint16 value);
			void PokeUInt32(size_t position, uint32 value);
			void PokeUInt64(size_t position, uint64 value);

			inline void Poke(size_t position, char value) { PokeChar(position, value); }
			inline void Poke(size_t position, byte value) { PokeByte(position, value); }
			inline void Poke(size_t position, int16 value) { PokeInt16(position, value); }
			inline void Poke(size_t position, int32 value) { PokeInt32(position, value); }
			inline void Poke(size_t position, int64 value) { PokeInt64(position, value); }

			inline void Poke(size_t position, uint16 value) { PokeUInt16(position, value); }
			inline void Poke(size_t position, uint32 value) { PokeUInt32(position, value); }
			inline void Poke(size_t position, uint64 value) { PokeUInt64(position, value); }

			char PeekChar(size_t position);
			byte PeekByte(size_t position);
			int16 PeekInt16(size_t position);
			int32 PeekInt32(size_t position); inline int32 PeekInt(size_t position) { return PeekInt32(position); }
			int64 PeekInt64(size_t position);
			uint16 PeekUInt16(size_t position);
			uint32 PeekUInt32(size_t position);
			uint64 PeekUInt64(size_t position);

			inline size_t Size() { return _sz; }
			inline size_t Position() { return _pos; }
			inline void Position(size_t p) { _pos = std::min(p, _sz ); }
			inline bool AtEnd() { return _pos >= _sz; }

			inline char ReadChar() { return PeekChar(_pos++); }
			inline byte ReadByte() { return PeekChar(_pos++); }
			inline int16 ReadInt16() { auto r = PeekInt16(_pos); _pos += sizeof(int16); return r; }
			inline int32 ReadInt32() { auto r = PeekInt32(_pos); _pos += sizeof(int32); return r; }
			inline int64 ReadInt64() { auto r = PeekInt64(_pos); _pos += sizeof(int64); return r; }
			inline uint16 ReadUInt16() { auto r = PeekUInt16(_pos); _pos += sizeof(uint16); return r; }
			inline uint32 ReadUInt32() { auto r = PeekUInt32(_pos); _pos += sizeof(uint32); return r; }
			inline uint64 ReadUInt64() { auto r = PeekUInt64(_pos); _pos += sizeof(uint64); return r; }
			inline int32 ReadInt() { return ReadInt32(); }
			inline bool ReadBoolean() { return ReadByte() > 0; }
			inline bool ReadBool() { return ReadByte() > 0; }

			void chcpy(char* buf, size_t pos,size_t sz);
			void chcpy(char* buf, size_t sz);

			/// <summary>
			/// Reads a string from the bank. Please note, this routine does NOT take null-termination into account!
			/// </summary>
			/// <param name="sz">The length of the string. When set to 0 it reads an uint32 value to get the size</param>
			/// <returns></returns>
			std::string ReadString(size_t sz = 0);

		};


	Bank CreateBank(size_t size, Endian E = Endian::Little);
	Bank CreateBank(char* buf, size_t size, Endian E = Endian::Little);
	Bank CreateBank(std::vector<char>, Endian E = Endian::Little);

	/// <summary>
	/// Takes over a buffer and turns it into a bank. Please note! This routine actually copies the pointer to that buffer! Don't use this unless you know what you are doing. Also note that the memory allocated to this pointer will as a result be disposed automatically once this Bank is being disposed. If you do not want all that, this may not be the best way to go!
	/// </summary>
	/// <param name="buf"></param>
	/// <param name="size"></param>
	/// <param name="E"></param>
	/// <returns></returns>
	Bank TurnToBank(char* buf, size_t size, Endian E = Endian::Little);
	}

}