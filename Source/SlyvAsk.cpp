// Lic:
// Units/Source/SlyvAsk.cpp
// Ask
// version: 22.12.25
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
#include <iostream>
#include <SlyvAsk.hpp>
#include <SlyvString.hpp>
#include <SlyvConInput.hpp>
#include <SlyvSTOI.hpp>


using namespace std;

namespace Slyvina {
	namespace Units {
		GINIE AskGINIE = nullptr;
		std::string AskCat{ "" };

		static void Err(string msg) {
			QCol->Error(msg);
			exit(100);
		}

		/// <summary>
		/// Assert
		/// </summary>
		/// <param name="condition"></param>
		/// <param name="msg"></param>
		static void Ass(bool condition, string msg) {
			if (!condition) Err(msg);
		}

		std::string CAsk(std::string Key, std::string Question, std::string DefaultValue) { return Ask(AskGINIE, AskCat, Key, Question, DefaultValue); }

		std::string Ask(std::string Cat, std::string Key, std::string Question, std::string DefaultValue) { return Ask(AskGINIE, Cat, Key, Question, DefaultValue); }

		std::string CAsk(GINIE Data, std::string Key, std::string Question, std::string DefaultValue) { return Ask(Data, AskCat, Key, Question, DefaultValue); }


		//std::string CAsk(std::shared_ptr<GINIE> Data, std::string Key, std::string Question, std::string DefaultValue) { return Ask(Data.get(), AskCat, Key, Question, DefaultValue); }

		//std::string Ask(std::shared_ptr<GINIE> Data, std::string Cat, std::string Key, std::string Question, std::string DefaultValue) { return Ask(Data.get(), Cat, Key, Question, DefaultValue); }

		bool Yes(GINIE Data, std::string Cat, std::string Key, std::string Question) {
			while (!Data->Value(Cat, Key).size()) {
				QCol->Yellow(Question);
				QCol->Cyan(" ? ");
				QCol->LGreen("<Y/N> ");
				Data->Value(Cat, Key, Upper(ReadLine()));
			}
			return (Data->Value(Cat, Key))[0] == 'Y';
		}

		//bool Yes(std::shared_ptr<GINIE> Data, std::string Cat, std::string Key, std::string Question) { return Yes(Data.get(), Cat, Key, Question); }

		int AskInt(GINIE Data, std::string Cat, std::string Key, std::string Question, bool AllowDefault, int DefaultValue) {
			int ret{ 0 };
			if (Data->Value(Cat, Key).size()) {
				ret = ToInt(Data->Value(Cat, Key));
				if (STOI_Error.size()) {
					QCol->Error("Data error in [\"" + Cat + "\", \"" + Key + "\"](\"" + Data->Value(Cat, Key) + "\") -> " + STOI_Error);
					Data->Value(Cat, Key);
				} else return ret;
			}
			while (true) {
				auto DV = std::to_string(DefaultValue); if (!AllowDefault) DV = "";
				auto v = Ask(Data, Cat, Key, Question, DV);
				ret = ToInt(Data->Value(Cat, Key));
				if (STOI_Error.size())
					QCol->Error(STOI_Error);
				else return ret;

			}
			return ret;
		}

		int AskInt(GINIE Data, std::string Cat, std::string Key, std::string Question, int DefaultValue) {
			return AskInt(Data, Cat, Key, Question, true, DefaultValue);
		}

		int AskInt(std::string Cat, std::string Key, std::string Question) { return AskInt(AskGINIE, Cat, Key, Question, false); }
		int AskInt(std::string Cat, std::string Key, std::string Question, int D) { return AskInt(AskGINIE, Cat, Key, Question, D); }

		std::vector<std::string>* AskList(GINIE Data, std::string Cat, std::string Key, std::string Question,uint32 minanswers) {
			if (!Data->HasList(Cat, Key)) {
				QCol->Yellow(Question + "\n");
				QCol->LMagenta("Enter you answers, whiteline will end the sequence. Minimal answers needed: " + std::to_string(minanswers)+"\n");
				//QCol->Doing("Debug", Cat + "::" + Key);
				auto ret = Data->List(Cat, Key);
				do {
					QCol->Cyan("");
					auto answer = Trim(ReadLine(TrSPrintF("%03d> ",ret->size()+1)));
					if (answer.size()) {
						Data->Add(Cat, Key, answer);
					} else if (ret->size() >= minanswers) return ret;
				} while (true);
			}
			return Data->List(Cat, Key);
		}

		std::string Ask(GINIE Data, std::string Cat, std::string Key, std::string Question, std::string DefaultValue) {
			Ass(Data!=nullptr, "Ask to NullPointerGINIE");
			Ass(Cat.size(), "Ask to no category");
			Ass(Key.size(), "Ask to no key");
			if (!Question.size()) Question = ":>";
			while (!Data->Value(Cat, Key).size()) {
				if (DefaultValue.size()) QCol->Magenta(TrSPrintF("[%s] ", DefaultValue.c_str()));
				QCol->Yellow(Question + " ");
				QCol->Cyan("");
				Data->Value(Cat, Key, ReadLine());
				if (!Data->Value(Cat, Key).size()) Data->Value(Cat, Key, DefaultValue);
			}			
			return Data->Value(Cat, Key);
		}
	}
}