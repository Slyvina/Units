// License:
// 	Units/Source/SlyvRequestFile.cpp
// 	Slyvina - Request File
// 	version: 24.12.18
// 
// 	Copyright (C) 2022, 2024 Jeroen P. Broks
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
// Lic:
// Units/Source/SlyvRequestFile.cpp
// Slyvina - Request File
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

#include <SlyvRequestFile.hpp>
#include <SlyvString.hpp>
#include <SlyvConInput.hpp>

#include <stdio.h>

#ifdef SlyvWindows
#include <Windows.h>
#include <atlstr.h>
#include <ShlObj.h>


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

		 String RequestFile(String Caption, String InitDir, String Filter, bool Save) {
			 auto UDF{ RequestFileDriver::Using().UseRequestFile };
			 if (!UDF) {
				 _LFQError = "No ReqestFile function defined!";
				 return "";
			 }
			 return UDF(Caption, InitDir, Filter, Save);
		 }

		 String RequestDir(String Caption, String InitDir) {
			 auto UDD{ RequestFileDriver::Using().UseRequestDir };
			 if (!UDD) {
				 _LFQError = "No ReqestDir function defined!";
				 return "";
			 }
			 return UDD(Caption, InitDir);
		 }
#pragma region "Default Windows functions"
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

#ifdef SlyvWindows
		 static String Win_RequestFile(String Caption, String InitDir, String Filter, bool save) {
			 _LFQError = "";
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

			 //printf("Requestfile Debug: %s\n", buf);
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
//#else
//			 _LFQError = "File requester not (yet) available on this platform";
//			 return "";
		 }

		 static int CALLBACK BrowseForFolderCallbackA(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData) {
			 char szPath[MAX_PATH];
			 switch (uMsg) {
			 case BFFM_INITIALIZED:
				 SendMessageA(hwnd, BFFM_SETSELECTIONA, TRUE, pData);
				 break;
			 case BFFM_SELCHANGED:
				 if (SHGetPathFromIDListA((LPITEMIDLIST)lp, szPath)) {
					 SendMessageA(hwnd, BFFM_SETSTATUSTEXTA, 0, (LPARAM)szPath);
				 }
				 break;
			 }
			 return 0;
		 }

		 String Win_RequestDir(String Caption, String InitDir) {
			 String str = "";
			 String dir = InitDir;
			 String text = Caption;
			 /*
			 bool _usew{ false };
			 if (_usew) {
				  LPMALLOC shm;
				  ITEMIDLIST * idlist;
				  BROWSEINFOW bi = { 0 };
				  wchar_t buf[MAX_PATH], * p;

				 GetFullPathNameW(bbTmpWString(dir), MAX_PATH, buf, &p);

					 bi.hwndOwner = GetActiveWindow();
				 bi.lpszTitle = bbTmpWString(text);
				 bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
				 bi.lpfn = BrowseForFolderCallbackW;
				 bi.lParam = (LPARAM)buf;
				|
				 beginPanel();
				 idlist = SHBrowseForFolderW(&bi);
				 endPanel();
				|
				 if (idlist) {
				 SHGetPathFromIDListW(idlist, buf);
				 str = bbStringFromWString(buf);
									 //SHFree( idlist );     //?!?



			 } else //*/
			 {
				 LPMALLOC shm;
				 ITEMIDLIST* idlist;
				 BROWSEINFOA bi = { 0 };
				 char buf[MAX_PATH], * p;
				 // GetFullPathNameA(bbTmpCString(dir), MAX_PATH, buf, &p);
				 GetFullPathNameA(dir.c_str(), MAX_PATH, buf, &p);
				 bi.hwndOwner = GetActiveWindow();
				 bi.lpszTitle = text.c_str(); //bbTmpCString(text);
				 bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
				 bi.lpfn = BrowseForFolderCallbackA;
				 bi.lParam = (LPARAM)buf;
				 beginPanel();
				 idlist = SHBrowseForFolderA(&bi);
				 endPanel();
				 if (idlist) {
					 SHGetPathFromIDListA(idlist, buf);
					 str = buf; //bbStringFromCString(buf);
					 //SHFree( idlist );     //?!?

				 }

			 }
			 return str;
		 }
#endif
#pragma endregion

#pragma region "Text Console Driver"
		 static String Txt_RequestFile(String Caption, String InitDir, String Filter, bool Save) {			 
			 _LFQError = "";
			 return Trim(ReadLine(Caption + ": "));
		 }
		 static String Txt_RequestDir(String Caption, String InitDir) {
			 _LFQError = "";
			 return Trim(ReadLine(Caption + ": "));
		 }

		 static RequestFileDriver RequestFileText{ Txt_RequestFile,Txt_RequestDir };
		 void RQF_Text() { RequestFileText.Use(); }
#pragma enregion

#pragma region "popen drive"
/*
 class pop { public: String Need{}; RequestFileDriver Drv; };
		static std::map<String, pop> popenreg{};
		static String popend{};
		 static String POPEN_RF(String Caption, String InitDir, String Filter, bool Save) {
			 _LFQError = "";
			 popend = Upper(popend);
			 if (!popenreg.count(popend)) { _LFQError = "Unknown popen"; return ""; }
			 auto& Drv{ popenreg[popend] };
			 char retvalue[500];
			 String Call{Drv.Drv.Data("CALL")};
			 FILE* f = popen(Call.c_str(), "r");
			 fgets(retvalue, 500, f);
			 int ret = pclose(f);
			 if (ret < 0) {
				 //perror("file_name_dialog()");
				 _LFQError = "open requestfile failed";
				 return "";
			 }
			 return retvalue;
		 }
//*/
#pragma endregion

		 void RequestFileDriver::Use(RequestFileDriver drv) { _Using = drv; }

#if defined(SlyvWindows)
		 RequestFileDriver RequestFileDriver::_Using{Win_RequestFile, Win_RequestDir};
#elif defined(SlyvLinux)
		 RequestFileDriver RequestFileDriver::_Using{ RequestFileText}; // For now this will have to do.
#else
		 RequestFileDriver RequestFileDriver::_Using{nullptr, nullptr};
#pragma message "WARNING! This platform is not known to Slyvina's File Request function"
#endif
	 }
 }
