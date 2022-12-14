// Lic:
// Units/Source/SlyvVolumes.cpp
// Slyvina - Volumes
// version: 22.12.24
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
#include <SlyvVolumes.hpp>
#include <SlyvString.hpp>

#ifdef SlyvWindows
#include <windows.h>
#endif

namespace Slyvina {
    namespace Units {
        StringMap Volumes() {
            auto ret = NewStringMap();
#ifdef SlyvWindows
            WCHAR szVolumeName[MAX_PATH];
            WCHAR VROOT[5] = L"A:\\";
            for (WCHAR i = 'A'; i <= 'Z'; i++) {
                VROOT[0] = i;
                //std::cout << (char)VROOT[0] << (char)VROOT[1] << (char)VROOT[2] << std::endl; // debug
            
                BOOL bSucceeded = GetVolumeInformationW(VROOT,
                    szVolumeName,
                    MAX_PATH,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0);
                if (bSucceeded) {
                    char VolName[MAX_PATH];
                    unsigned int j = 0;
                    do {
                        VolName[j] = (char)szVolumeName[j];
                        //std::cout << j << "\t" << szVolumeName[j] << "\t" << VolName[j] << "\n";
                    } while (szVolumeName[j++]);
                    char TROOT[5] = "A:\\";
                    TROOT[0] = (char)i;
                    (*ret)[VolName] = TROOT;
                }
            }
#else
#error Volumes is not yet properly set up for this platform.
#endif
            return ret;
        }
        std::string AVolPath(std::string path,bool ignorecase) {
            static auto Vols = Volumes();
            for (auto vk : *Vols) {
                if (
                    (ignorecase && Prefixed(Upper(path), Upper(vk.first + ":"))) ||
                    (Prefixed(path, vk.first + ":"))
                    ) {
                    return StReplace(ChReplace(vk.second + path.substr(vk.first.size() + 1), '\\', '/'),"//","/");
                }
            }
            return path;
        }
    }
}