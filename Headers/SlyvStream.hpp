// License:
// 	Units/Headers/SlyvStream.hpp
// 	Slyvina - Quick Stream Handler
// 	version: 25.03.01
// 
// 	Copyright (C) 2020, 2021, 2022, 2023, 2024, 2025 Jeroen P. Broks
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
#include <string>
#include <vector>
#include <fstream>

#include "Slyvina.hpp"




namespace Slyvina {
	namespace Units {
		//using namespace std;
		bool FileExists(char* file);
		bool FileExists(std::string file);
		std::string FLoadString(std::string file,bool NoNull=false); // The F prefix is because MicroSoft has a macro with the same name.
		VecString LoadLines(std::string file);
		void LoadChars(std::vector<char>* vec, std::string file);
		void LoadBytes(std::vector<char>* vec, std::string file);
		void SaveString(std::string file, std::string stringvalue);

		/// <summary>
		/// Loads a character buffer. Use with care, as the system does allocate memory which should be released after!
		/// </summary>
		char* LoadCharBuf(std::string File);

		bool DirectoryExistsC(const char* folderName);
		bool DirectoryExists(std::string folderName);

		bool MakeDir(std::string dir);
		bool MakeDirC(const char* dir);
		bool FileDelete(std::string file, bool noerrormsg = false);

		std::string CurrentDir();
		void ChangeDir(std::string dir);

		/// <summary>
		/// Check the size of a file
		/// </summary>
		/// <param name="filename"></param>
		/// <returns>File size in bytes</returns>
		std::ifstream::pos_type FileSize(std::string filename);
		std::string FileDate(std::string FileName);
		time_t FileTimeStamp(std::string FileName);

		/// <summary>
		/// Copy a file
		/// </summary>
		/// <param name="original"></param>
		/// <param name="tar"></param>
		/// <returns>True is succesful</returns>
		bool FileCopy(std::string original, std::string tar);


		class True_OutFile;
		typedef std::shared_ptr<True_OutFile> OutFile;
		OutFile WriteFile(std::string fname, int endian = 1);
		OutFile AppendFile(std::string fname, int endian = 1);

		class True_OutFile {
		private:
			std::string FileName;
			std::ofstream stream;
			bool closed{ false };
			int endian{ 1 }; // 0 = don't check! 1 = little endian! 2 = big endian!
			int sysendian;
			bool endmatch();
			unsigned long long Written{ 0 };
		public:

			/// <summary>
			/// When set true (default) this automatically closes a file when this instance is disposed from the memory
			/// Only set to false when you know what you are doing. This class is to be used as a shared pointer only after all.
			/// </summary>
			bool AutoClose{ true };

			True_OutFile(std::string _filename, int endian = 1,bool append=false);
			~True_OutFile();
			void Write(char c);
			void Write(unsigned char c);

			// Some syntax checking editors (like Visual Studio) won't like the way this is defined and show warning lines.
			// These lines warning that the actual function doesn't exist, while in fact, it does. But I was lazy, so I chose a dirty way to do this!
			void Write(int i);
			void Write(unsigned int i);
			void Write(long long i);
			void Write(unsigned long long i);
			void Write(std::string s, bool raw = false);
			void Write(std::vector<char> buf, bool storelength = false);
			void Write(std::vector<byte> buf, bool storelength = false);

			void WriteCString(const char* str);

			// Writes lines (Unix style so only <LF> at the end).
			void WriteLine(std::string s="");

			void WriteBytes(byte* b, size_t L);
			void WriteChars(char* b, size_t L);


			unsigned long long Size();
			inline std::streampos Position() { return stream.tellp(); }
			void Position(uint64 p);

			void Close();
		};

		class True_InFile;
		typedef std::shared_ptr<True_InFile> InFile;
		InFile ReadFile(std::string fname, int endian = 1);


		class True_InFile {
		private:
			std::string FileName;
			std::ifstream stream;
			bool closed{ false };
			int endian{ 1 }; // 0 = don't check! 1 = little endian! 2 = big endian!
			int sysendian;
			bool endmatch();
			unsigned long long read{ 0 };
			unsigned long long size{ 0 };
		public:
			uint64 Size();
			void Seek(uint64 position);
			inline void Position(uint64 p) { Seek(p); read = p; }
			uint64 Position();
			True_InFile(std::string _filename, int endian = 1);
			~True_InFile();
			void Close();
			char ReadChar();
			byte ReadByte();
			inline byte ReadByte(byte& b) { b = ReadByte(); return b; } // I know this looks a bit odd, but I don't like "while(a=St->ReadByte())" counting on it to go right.
			int32 ReadInt();
			int16 ReadInt16();
			int64 ReadLong();
			uint32 ReadUInt();
			uint16 ReadUInt16();
			uint64 ReadUInt64();
			std::string ReadLine();
			std::string ReadString(int l = 0);
			void ReadChars(char* c, int size = 0);
			std::shared_ptr<std::vector<char>> ReadChars(int size);
			std::shared_ptr<std::vector<byte>> ReadBytes(int size);
			void ReadCString(char* c); // Unsafe C string reader
			void ReadCString(char* c, int size, bool stoponnull=false); // Safe (sort of) C string reader
			std::string ReadCString();
			void ReadBytes(std::vector<byte>& Buf,int size, bool clear=true);
			bool EndOfFile();
		};


	}
}
