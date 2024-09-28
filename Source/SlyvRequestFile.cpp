// Lic:
// Units/Source/SlyvRequestFile.cpp
// Slyvina - Request File
// version: 24.09.28
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

// No reason why this should not work, but somehow Visual C++ finds one!
//#include <SlyvRequestFile.hpp>
#include "../Headers/SlyvRequestFile.hpp"
//#include <SlyvString.hpp>
#include "../Headers/SlyvString.hpp"

#ifdef SlyvWindows
#include <Windows.h>
#include <atlstr.h>


static HWND focHwnd;

static void beginPanel() {
	focHwnd = GetFocus();

}

 static void endPanel() {
 SetFocus(focHwnd);
}
#endif

 namespace Slyvina {
	 namespace Units {
		 static String _LFQError{ "No file request done yet" };

		 String RequestFileError() {
			 return _LFQError;
		 }

#ifdef SlyvWindows
		 String OFNA_Err(DWORD a) {
			 //switch (CommDlgExtendedError()) {
			 switch (a) {
			 case CDERR_DIALOGFAILURE: _LFQError = "CDERR_DIALOGFAILURE";   break;
			 case CDERR_FINDRESFAILURE: _LFQError = "CDERR_FINDRESFAILURE";  break;
			 case CDERR_INITIALIZATION: _LFQError = "CDERR_INITIALIZATION";  break;
			 case CDERR_LOADRESFAILURE: _LFQError = "CDERR_LOADRESFAILURE";  break;
			 case CDERR_LOADSTRFAILURE: _LFQError = "CDERR_LOADSTRFAILURE";  break;
			 case CDERR_LOCKRESFAILURE: _LFQError = "CDERR_LOCKRESFAILURE";  break;
			 case CDERR_MEMALLOCFAILURE: _LFQError = "CDERR_MEMALLOCFAILURE"; break;
			 case CDERR_MEMLOCKFAILURE: _LFQError = "CDERR_MEMLOCKFAILURE";  break;
			 case CDERR_NOHINSTANCE: _LFQError = "CDERR_NOHINSTANCE";     break;
			 case CDERR_NOHOOK: _LFQError = "CDERR_NOHOOK";          break;
			 case CDERR_NOTEMPLATE: _LFQError = "CDERR_NOTEMPLATE";      break;
			 case CDERR_STRUCTSIZE: _LFQError = "CDERR_STRUCTSIZE";      break;
			 case FNERR_BUFFERTOOSMALL: _LFQError = "FNERR_BUFFERTOOSMALL";  break;
			 case FNERR_INVALIDFILENAME: _LFQError = "FNERR_INVALIDFILENAME"; break;
			 case FNERR_SUBCLASSFAILURE: _LFQError = "FNERR_SUBCLASSFAILURE"; break;
			 default: _LFQError = "You cancelled.";
			 }
			 return _LFQError;
		 }
#endif

		 String RequestFile(String Caption, String InitDir, String Filter, bool save) {
			 _LFQError = "";
#ifdef SlyvWindows
			 /*
			 char filename[MAX_PATH];
			 std::wstring wCaption = std::wstring(Caption.begin(),Caption.end());
			 std::wstring wFilter = std::wstring(Filter.begin(), Filter.end());
			 WCHAR wFileName[MAX_PATH];

			 OPENFILENAME ofn;
			 //ZeroMemory(&filename, sizeof(filename));
			 ZeroMemory(&wFileName, sizeof(wFileName));
			 ZeroMemory(&ofn, sizeof(ofn));
			 ofn.lStructSize = sizeof(ofn);
			 ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
			 ofn.lpstrFilter = wFilter.c_str();
			 //ofn.lpstrFile = filename;
			 ofn.lpstrFile = wFileName;
			 ofn.nMaxFile = MAX_PATH;
			 ofn.lpstrTitle = wCaption.c_str();
			 ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

			 if (GetOpenFileName(&ofn)) {
				 //std::cout << "You chose the file \"" << filename << "\"\n";
				 std::cout << "Hmmm:" << ofn.lpstrFile << std::endl;
				 std::wstring ws{ ofn.lpstrFile };
				 std::string ret = std::string(ws.begin(), ws.end());
				 //std::string ret{ CW2A(ofn.lpstrFile) };

				 return ret;
			 } else {
				 // All this stuff below is to tell you exactly how you messed up above.
				 // Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
				 switch (CommDlgExtendedError()) {
				 case CDERR_DIALOGFAILURE: _LFQError = "CDERR_DIALOGFAILURE\n";   break;
				 case CDERR_FINDRESFAILURE: _LFQError = "CDERR_FINDRESFAILURE\n";  break;
				 case CDERR_INITIALIZATION: _LFQError = "CDERR_INITIALIZATION\n";  break;
				 case CDERR_LOADRESFAILURE: _LFQError = "CDERR_LOADRESFAILURE\n";  break;
				 case CDERR_LOADSTRFAILURE: _LFQError = "CDERR_LOADSTRFAILURE\n";  break;
				 case CDERR_LOCKRESFAILURE: _LFQError = "CDERR_LOCKRESFAILURE\n";  break;
				 case CDERR_MEMALLOCFAILURE: _LFQError = "CDERR_MEMALLOCFAILURE\n"; break;
				 case CDERR_MEMLOCKFAILURE: _LFQError = "CDERR_MEMLOCKFAILURE\n";  break;
				 case CDERR_NOHINSTANCE: _LFQError = "CDERR_NOHINSTANCE\n";     break;
				 case CDERR_NOHOOK: _LFQError = "CDERR_NOHOOK\n";          break;
				 case CDERR_NOTEMPLATE: _LFQError = "CDERR_NOTEMPLATE\n";      break;
				 case CDERR_STRUCTSIZE: _LFQError = "CDERR_STRUCTSIZE\n";      break;
				 case FNERR_BUFFERTOOSMALL: _LFQError = "FNERR_BUFFERTOOSMALL\n";  break;
				 case FNERR_INVALIDFILENAME: _LFQError = "FNERR_INVALIDFILENAME\n"; break;
				 case FNERR_SUBCLASSFAILURE: _LFQError = "FNERR_SUBCLASSFAILURE\n"; break;
				 default: _LFQError = "You cancelled.\n";
				 }
				 return "";
			 }
		 }
		 //*/
		 // PreWork
			 String
				 file{ "" },
				 exts{ "" },
				 dir{ "" },
				 path{ InitDir };

			 path = ChReplace(path, '/', '\\');

			 auto i = FindLast(path, '\\');
			 if (i != -1) {
				 dir = path.substr(0, i);
				 file = path.substr(i + 1);// path[i + 1..]
			 } else {
				 file = path;
			 }
			 // calculate default index of extension in extension list from path name				
			 String ext; int defext, p, q; //Local ext$, defext, p, q
			 p = FindFirst(path, '.'); //= path.Find(".")
			 if (p > -1) {
				 ext = Lower("," + path.substr(p + 1) + ","); // ext = "," + path[p + 1..].toLower() + ","
				 auto exs = Lower(exts);//Local exs$ = exts.toLower()
				 exs = StReplace(exs, ":", ":,"); //exs = exs.Replace(":", ":,")
				 exs = StReplace(exs, ";", ",;"); //exs = exs.Replace(";", ",;")
				 p = FindFirst(exs, ext); //p = exs.find(ext)
				 if (p > -1) {
					 auto q = -1;
					 defext = 1;
					 while (true) {
						 q = exs.find(";", q + 1);
						 if (q > p) break; // If q > p Exit
						 if (q == -1) { defext = 0; break; }  //000000097 | If q = -1 defext = 0; Exit
						 defext++; //000000098 | defext:+1
					 } // Wend
				 } else {
					 defext = 0; // This ain't BlitzMax after all.
				 } // EndIf
			 } // EndIf

			 if (exts.size()) {
				 if (exts.find(":") == -1) {
					 exts = "Files\0*." + exts; // exts = "Files~0*." + exts
				 } else {
					 exts = StReplace(exts, ":", "\0*."); //exts = exts.Replace(":", "~0*.")
				 } // EndIf
				 exts = StReplace(exts, ";", "\0"); // exts = exts.Replace(";", "~0")
				 exts = StReplace(exts, ",", ";*.") + "\0"; //000000110 | exts = exts.Replace(",", ";*.") + "~0"
			 } // EndIf

					 // Actual work
					 // BBString *bbSystemRequestFile( BBString *text,BBString *exts,int defext,int save,BBString *file,BBString *dir )
			 String
				 str{ "" };
			 char buf[MAX_PATH];
			 OPENFILENAMEA of = { sizeof(of) };
			 strcpy_s(buf, file.c_str());

			 of.hwndOwner = GetActiveWindow();
			 of.lpstrTitle = Caption.c_str();
			 of.lpstrFilter = exts.c_str();
			 of.nFilterIndex = defext;
			 of.lpstrFile = buf;
			 of.lpstrInitialDir = dir.size() ? dir.c_str() : 0;
			 of.nMaxFile = MAX_PATH;
			 of.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR;

			 beginPanel();
			 if (save) {
				 of.lpstrDefExt = "";
				 of.Flags |= OFN_OVERWRITEPROMPT;
				 if (GetSaveFileNameA(&of)) {
					 str = buf; //str = bbStringFromCString(buf);					
				 } else OFNA_Err(CommDlgExtendedError());
			 } else {
				 of.Flags |= OFN_FILEMUSTEXIST;
				 if (GetOpenFileNameA(&of)) {
					 str = buf;//str = bbStringFromCString(buf);					
				 } else OFNA_Err(CommDlgExtendedError());

			 }
			 endPanel();
			 return str;
#else
			 _LFQError = "File requester not (yet) available on this platform";
			 return "";
#endif
		 }

		 void RequestFileDriver::Use(RequestFileDriver drv) { Using = drv; }

#if defined(SlyvWindows)
		 RequestFileDriver RequestFileDriver::Using{nullptr, nullptr};
#elif define(SlyvLinux)
		 RequestFileDriver RequestFileDriver::Using{nullptr, nullptr};
#else
		 RequestFileDriver RequestFileDriver::Using{nullptr, nullptr};
#pragma message "WARNING! This platform is not known to Slyvina's File Request function"
#endif
	 }
 }