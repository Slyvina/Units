// Lic:
// Units/Headers/SlyvRequestFile.hpp
// fsdf
// version: 24.09.29
// Copyright (C) 2022, 2024 Jeroen P. Broks
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
#include "SlyvString.hpp"

namespace Slyvina {
	namespace Units {

		typedef String(*DelegateRequestFile) (String Caption, String InitDir, String Filter, bool Save);
		typedef String(*DelegateRequestDir) (String Caption, String InitDir);

		class RequestFileDriver {
		private:
			static RequestFileDriver _Using;
			std::map<std::string, std::string> _Data{};
		public:
			inline void Data(String K, String V) { _Data[Upper(K)] = V; }
			inline void Data(String K, int V) { Data(K, std::to_string(V)); }
			inline String Data(String K) { return _Data[Upper(K)]; }
			DelegateRequestFile UseRequestFile{ nullptr };
			DelegateRequestDir UseRequestDir{ nullptr };
			RequestFileDriver(DelegateRequestFile URF, DelegateRequestDir URD) { UseRequestFile = URF; UseRequestDir = URD; }
			static void Use(RequestFileDriver drv);
			inline static void Use(DelegateRequestFile URF, DelegateRequestDir URD) { Use(RequestFileDriver(URF, URD)); }
			inline void Use() { Use(*this); }
			inline static RequestFileDriver& Using() {return _Using;	}
		};


		/// <summary>
		/// Contains an empty string if the last FileRequest call had no errors, otherwise it contains the error message.
		/// </summary>
		String RequestFileError();

		/// <summary>
		/// Simple file request routine. Will return a file name if succesful, will return an empty string if not. RequestFileError() will then contain the error message.
		/// </summary>
		String RequestFile(String Caption = "Please select a file", String InitDir=".", String Filter = "All:*\0",bool Save=false);

		/// <summary>
		/// /// Simple directory request routine. Will return a file name if succesful, will return an empty string if not. RequestFileError() will then contain the error message.
		/// /// </summary>
		String RequestDir(String Caption = "Please select a file", String InitDir = ".");

		

		/// <summary>
		/// Request a file to be saved
		/// </summary>
		/// <param name="Caption"></param>
		/// <param name="InitDir"></param>
		/// <param name="Filter"></param>
		/// <returns></returns>
		inline String RequestSaveFile(String Caption = "Please enter a file name to save", String InitDir = ".", String Filter = "All:*\0") { return RequestFile(Caption, InitDir, Filter, true); }


		void RQF_Text();
	}
}