// Lic:
// Units/Source/SlyvDir.cpp
// Slyvina - Directory
// version: 24.09.27
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
#include <algorithm>                                                                                                                  
#include <iostream>                                                                                                                   
#include <SlyvDir.hpp>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)                                                        
#define ForWindows                                                                                                                    
#include <Windows.h>  
#else
#include <filesystem>
#endif
namespace Slyvina {
    namespace Units {


#ifdef ForWindows
        static std::wstring s2ws(const std::string& s) {
            int len;
            int slength = (int)s.length() + 1;
            len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
            wchar_t* buf = new wchar_t[len];
            MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
            std::wstring r(buf);
            delete[] buf;
            return r;
        }

        static std::string convLPCWSTRtoString(LPCWSTR wString) {
            auto size = wcslen(wString) + 1;
            char* cString = new char[size];
            for (unsigned long long i = 0; i < size; i++) {

                cString[i] = wString[i];

            }
            std::string String{ cString };
            delete[] cString;
            return String;
        }
        //std::wstring stemp = s2ws(myString);                                                                                                
        //LPCWSTR result = stemp.c_str();                                                                                                     

#endif                                                                                                                                

#ifndef ForWindows
#include <sys/stat.h>
#endif

        bool IsDir(std::string pth) {
#ifdef ForWindows                                                                                                                     
            using namespace std;
            string search_path = pth;
            WIN32_FIND_DATA fd;
            HANDLE hFind = ::FindFirstFile(s2ws(search_path).c_str(), &fd);
            if (hFind != INVALID_HANDLE_VALUE)
                return ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY));
            else
                return false;
#else                                                                                                                                 
            //#pragma message("WARNING! IsDir is not yet supported by this platform! An empty vector will be returned in stead!")                   
            //return false;
            struct stat s;
            if ( lstat(pth.c_str(), &s) == 0 ) {
                return S_ISDIR(s.st_mode);
            }
            return false;
#endif                                                                                                                                
        }
        bool IsFile(std::string pth) {
#ifdef ForWindows                                                                                                                     
            using namespace std;
            string search_path = pth;
            WIN32_FIND_DATA fd;
            HANDLE hFind = ::FindFirstFile(s2ws(search_path).c_str(), &fd);
            if (hFind != INVALID_HANDLE_VALUE)
                return (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY));
            else
                return false;
#else                                                                                                                                 
            //#pragma message("WARNING! IsFile is not yet supported by this platform! An false therefore will be returned in stead!")               
            //return false;
            struct stat s;
            if ( lstat(pth.c_str(), &s) == 0 ) {
                return S_ISREG(s.st_mode);
            }
            return false;
#endif                                                                                                                                

        }


        VecString FileList(std::string Dir, DirWant Want, bool allowhidden, std::string addprefix) {
#ifdef ForWindows                                                                                                                     
            using namespace std;
            //std::vector < std::string > ret;
            auto ret{ NewVecString() };
            string search_path = Dir + "/*.*";
            WIN32_FIND_DATA fd;
            HANDLE hFind = ::FindFirstFile(s2ws(search_path).c_str(), &fd);
            if (hFind != INVALID_HANDLE_VALUE) {
                do {
                    // read all (real) files in current folder                                                                                
                    // , delete '!' read other 2 default folder . and ..                                                                      
                    auto found{ convLPCWSTRtoString(fd.cFileName) };
                    auto allow{ false };
                    //cout << found << " / " << fd.cFileName << endl;                                                                         
                    switch (Want) {
                    case DirWant::Files:
                        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) { allow = true; }
                        break;
                    case DirWant::Directories:
                        if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) { allow = true; }
                        break;
                    case DirWant::FilesAndDirectories:
                        allow = true;
                        break;
                    case DirWant::Tree:
                        if ((!allowhidden) && found[0] == '.') { allow = false; } else if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                            auto add = FileList(Dir + "/" + found, DirWant::Tree, allowhidden, addprefix + found + "/");
                            for (auto a : *add) ret->push_back(a);
                            allow = false;
                        } else {
                            allow = true;
                        }
                    }
                    if ((!allowhidden) && found[0] == '.') { allow = false; }
                    if (allow) {
                        ret->push_back(addprefix + found);
                    }
                } while (::FindNextFile(hFind, &fd));
                ::FindClose(hFind);
            }
            std::sort(ret->begin(), ret->end());
            return ret;
#else                                                                                                                                 
            //#pragma message("WARNING! FileList is not yet supported by this platform! An empty vector will be returned in stead!")                
            namespace fs = std::filesystem;
            for (const auto & entry : fs::directory_iterator(Dir)) {
                //std::cout << entry.path() << std::endl;
                std::string 
                    ep{entry.path()},
                    cp{ep.substr(Dir.size()}; while(cp.size() && cp[0]=='/') cp=cp.substr(1);
                std::cout<<ep<<" -> "<<cp<<"\n";
            }        
            return NewVecString(); //std::vector<std::string>();
#endif                                                                                                                                
        }

        VecString GetTree(std::string dir, bool allowhidden) {
            return FileList(dir, DirWant::Tree, allowhidden);
        }

    }
}