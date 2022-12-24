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