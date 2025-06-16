// License:
// 	Units/Source/SlyvVecStringSaver.cpp
// 	Vec String Saver
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


#include <string.h>

#include <SlyvBank.hpp>
#include <SlyvStream.hpp>
#include <SlyvVecStringSaver.hpp>


namespace Slyvina {
	namespace Units {

		#define SVSS_Header "Slyvina VecString\x1a"

		extern "C" {
			// Some C++ compilers (like visual studio) will throw warning if I don't turn this into an enum class.
			// Since pure C doesn't support that, that saves me a lot of fucking crap.
			enum STag {
				ST_End = 0,
				ST_Dictionary = 1,
				ST_LinePure = 2,
				ST_LineDict = 3,
				ST_NumberOfLines = 4  // This could be useful when you want to write a loader in pure C.
			};
		}

//{ Save
		static void _OptiDict(std::map<String,int>* D,std::vector<String>* vs) {
			std::map<String,uint32> Cnt{};
			for(auto s:*vs) Cnt[s]++;
			D->clear();
			int ID{0};
			for(auto cr:Cnt) {
				if(cr.second>=2) (*D)[cr.first]=ID++;
			}
		}

		static void _FullDict(std::map<String,int>* D,std::vector<String>* vs) {
			auto vsc{vs->size()};
			D->clear();
			for(size_t i=0;i<vsc;i++) (*D)[(*vs)[i]]=i;
		}

		void SaveVecString(String File,std::vector<String>* vs,SVSS_Dict D) {
			std::map<String,int> DictIDs{};
			switch(D) {
				case SVSS_Dict::None: break;
				case SVSS_Dict::Optimal: _OptiDict(&DictIDs,vs); break;
				case SVSS_Dict::Full: _FullDict(&DictIDs,vs); break;
				default: std::cout << "\x1b[93mError> \x1b[37mUnknown dictionary request for SaveVecString!\n";
			}
			auto BT{WriteFile(File)};
			BT->Write(SVSS_Header,true);
			BT->Write((Byte)ST_NumberOfLines);
			BT->Write((uint64)vs->size());
			for(auto DI:DictIDs) {
				BT->Write((Byte)ST_Dictionary);
				BT->Write(DI.second);
				BT->Write(DI.first);
			}
			for(auto l:*vs) {
				if (DictIDs.count(l)) {
					BT->Write((Byte)ST_LineDict);
					BT->Write(DictIDs[l]);
				} else {
					BT->Write((Byte)ST_LinePure);
					BT->Write(l);
				}
			}
			BT->Write((Byte)ST_End);
			BT->Close();
		}

//}

//{ Load
	bool lvs_autotxt{false};

	static Byte GByte(Bank BT,Byte*B) {
		*B = BT->ReadByte();
		return (*B);
	}


	VecString LoadVecString(Bank Buffer) {
		if (!Buffer) { throw std::runtime_error("VecString LoadVecString(nullptr)   ->   Cannot get any data from a null pointer!"); return nullptr;}
		size_t hsz{strlen(SVSS_Header)};
		auto allow{Buffer->Size()>hsz};
		for(size_t i=0;allow && i<hsz;i++) {
			Buffer->Position(i);
			auto chk{Buffer->ReadChar()};
			allow=allow && (chk==SVSS_Header[i]);
			//std::cout << i << "\t" << "Read: " << (int)chk << "::"<<chk<<";  Must be: " << (int)SVSS_Header[i]<<"::"<<SVSS_Header[i]<< " -> " << allow << "\n"; // Debug Only!
		}
		if (allow) {
			auto ret{NewVecString()};
			std::map<int,String> Dict{};
			Byte T{0xff};
			while(GByte(Buffer,&T) && (!Buffer->AtEnd())) {
				//std::cout << (int)T << "::" << Buffer->Position() << "::" << Buffer->AtEnd() << "\n";
				switch(T) {
				case ST_End: break; // Should NEVER be possible, but added just in case.
				case ST_Dictionary: {
					auto ID{Buffer->ReadInt()};
					Dict[ID]=Buffer->ReadString();
				} break;
				case ST_LineDict: {
					auto ID{ Buffer->ReadInt() };
					if (!Dict.count(ID)) {
						throw std::runtime_error("VecString LoadVecString(<TBank>)   ->   Undefined dictionary entry: "+std::to_string(ID));
						return nullptr;
					}
					ret->push_back(Dict[ID]);
				} break;
				case ST_LinePure:
					ret->push_back(Buffer->ReadString());
					break;
				case ST_NumberOfLines:
					Buffer->ReadInt64(); // No value in C++, but if this is ever ported to C, this can help ya.
					break;
				default:
					throw std::runtime_error("VecString LoadVecString(<TBank>)   ->   Unknown command tag ("+std::to_string(T)+") -- Data may be corrupted or be created in a later version!");
					return nullptr;
				}
			}
			return ret;
		} else if (lvs_autotxt) {
			Buffer->Position(0);
			auto ret{NewVecString()};
			while(!Buffer->AtEnd()) ret->push_back(Buffer->ReadLine());
			return ret;
		} else {
			std::cout << "VecString LoadVecString(BANK): Buffer does not contain VecString data!\n\n\x07";
			return nullptr;
		}
	}

	VecString LoadVecString(String File) {
		size_t SizeB{FileSize(File)}; if (!SizeB) {std::cout << "VecString LoadVecString(\""<<File<<"\")   ->   Size turned out 0!\n"; return nullptr; }
		/*
		auto PureB{LoadCharBuf(File)};
		auto BankB{TurnToBank(PureB,SizeB)};
		std::cout << "Size:"<<SizeB<<"::"<<BankB->Size()<<"\n"; for (size_t i=0;i<BankB->Size();i++) std::cout << i << "::"<<SizeB<<"::"<<BankB->Size()<<" -> "<<BankB->PeekByte(i) << "\n";
		//*/
		auto BankB{CreateBank(SizeB)};
		auto BT{ReadFile(File)};
		for(size_t p=0;p<SizeB;p++) BankB->Poke(p,BT->ReadByte());
		BT->Close();
		return LoadVecString(BankB);
	}

//}

//{ Check
	bool IsVecString(Bank Buffer) {
		if (!Buffer) { return false; }
		size_t hsz{strlen(SVSS_Header)};
		auto ret{Buffer->Size()>hsz};
		for(size_t i=0;ret && i<hsz;i++) {
			Buffer->Position(i);
			auto chk{Buffer->ReadChar()};
			ret=ret && (chk==SVSS_Header[i]);
			//std::cout << i << "\t" << "Read: " << (int)chk << "::"<<chk<<";  Must be: " << (int)SVSS_Header[i]<<"::"<<SVSS_Header[i]<< " -> " << allow << "\n"; // Debug Only!
		}
		return ret;
	}

	bool IsVecString(String File) {
		if (!FileExists(File)) return false;
		auto Buffer{ReadFile(File)};
		if (!Buffer) { return false; }
		size_t hsz{strlen(SVSS_Header)};
		auto ret{Buffer->Size()>hsz};
		for(size_t i=0;ret && i<hsz;i++) {
			//Buffer->Position(i);
			auto chk{Buffer->ReadChar()};
			ret=ret && (chk==SVSS_Header[i]);
			//std::cout << i << "\t" << "Read: " << (int)chk << "::"<<chk<<";  Must be: " << (int)SVSS_Header[i]<<"::"<<SVSS_Header[i]<< " -> " << allow << "\n"; // Debug Only!
		}
		Buffer->Close();
		return ret;
	}

//}
	}
}
