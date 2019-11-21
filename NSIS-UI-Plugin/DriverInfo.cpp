/*******************************************************************************************************************************************************
  #@@        *@    *@@%#@@#    &@    #@%@@,         @(        &@   .@.     @@@@@@@%     @         @(        &@     .@@@%&@@&     &@    @@#        %@
  #@/@       *@   *@      @%   &@   %@      @/      @(        &@   .@.     @,     ,@    @         @(        &@    @@        @*   &@    @,&@       %@
  #@  @(     *@   ,@           &@   #@              @(        &@   .@.     @,      @*   @         @(        &@   @&              &@    @, *@      %@
  #@   &@    *@     @@@,       &@    *@@%           @(        &@   .@.     @,     &@    @         @(        &@   @,              &@    @,   @(    %@
  #@    ,@   *@         .@@.   &@         (@@       @(        &@   .@.     @@@@@@%      @         @(        &@   @,     @@@@@&   &@    @,    @@   %@
  #@      @/ *@           *@   &@           &@      @(        @@   .@.     @,           @         @(        @@   @&         &&   &@    @,     (@. %@
  #@       @@,@   @@      (@   &@   @#      &@      (@       (@.   .@.     @,           @         (@       (@.    @@        &&   &@    @,       @%%@
  #@        *@@    (@@%#&@&    &@    %@@#%@@(         @@@%&@@(     .@.     @,           @@@@@@@@    @@@%&@@(        @@@%&@@@     &@    @,        %@@

* Copyright (C) 2018 - 2020, winsoft666, <winsoft666@outlook.com>.
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
*
* Expect bugs
*
* Please use and enjoy.
* Please let me know of any bugs/improvements that you have found/implemented and I will fix/incorporate them into this file.
********************************************************************************************************************************************************/


#include "DriverInfo.h"
#include <assert.h>
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <direct.h>


float DriveInfo::GetFreeMB(int nDrive) {
    std::wstring sRoot;
    unsigned long totalClusters, freeClusters, sectors, bytes;

    assert(nDrive > 0 && nDrive <= 26);

    if (IsDriveAvailable(nDrive)) {
        sRoot = GetRoot(nDrive);

        ::GetDiskFreeSpaceW(sRoot.c_str(), &sectors, &bytes, &freeClusters, &totalClusters);

        // do maths like this to avoid truncation
        // errors
        float fSpace = (float)sectors;
        fSpace *= bytes;
        fSpace *= freeClusters;
        fSpace /= 1048576;

        return fSpace;
    }

    return 0.0f;
}

std::wstring DriveInfo::GetVolume(int nDrive) {
    std::wstring sVolume, sRoot;

    assert(nDrive > 0 && nDrive <= 26);



    if (IsDriveAvailable(nDrive)) {
        sRoot = GetRoot(nDrive);

        wchar_t buf[20] = { 0 };
        GetVolumeInformationW(sRoot.c_str(), buf, 20, NULL, NULL, NULL, NULL, 0);

        std::wstring strVolume = buf;
        FormatName(sVolume);

        return sVolume;
    }

    return L"";
}

std::wstring DriveInfo::GetRoot(int nDrive) {
    std::wstring sRoot;

    sRoot = GetLetter(nDrive);
    sRoot += L":\\";

    return sRoot;
}

std::wstring DriveInfo::GetFullName(int nDrive) {
    std::wstring sFullName, sLetter, sVolume;

    assert(nDrive > 0 && nDrive <= 26);

    if (IsDriveAvailable(nDrive)) {
        sLetter = GetLetter(nDrive);
        sVolume = GetVolume(nDrive);

        sFullName = sVolume + L" (" + sLetter + L":)";

        return sFullName;
    }

    return L"";
}

float DriveInfo::GetTotalMB(int nDrive) {
    std::wstring sRoot;
    unsigned long totalClusters, freeClusters, sectors, bytes;

    assert(nDrive > 0 && nDrive <= 26);

    if (IsDriveAvailable(nDrive)) {
        sRoot = GetRoot(nDrive);

        ::GetDiskFreeSpaceW(sRoot.c_str(), &sectors, &bytes, &freeClusters, &totalClusters);

        // do maths like this to avoid truncation
        // errors
        float fSpace = (float)sectors;
        fSpace *= bytes;
        fSpace *= totalClusters;
        fSpace /= 1048576;

        return fSpace;
    }

    return 0.0f;
}

wchar_t DriveInfo::GetLetter(int nDrive) {
    assert(nDrive > 0 && nDrive <= 26);

    return (wchar_t)(nDrive + L'A' - 1);
}

int DriveInfo::GetType(int nDrive) {
    std::wstring sVolume;

    assert(nDrive > 0 && nDrive <= 26);

    // shortcut to avoid floppy access
    if (nDrive == 1 || nDrive == 2)
        return DRIVE_REMOVABLE;

    return ::GetDriveTypeW(GetRoot(nDrive).c_str());
}

int DriveInfo::GetPathType(const wchar_t *szPathName) {
    int nDrive = GetDrive(szPathName);

    if (nDrive >= 0)
        return GetType(nDrive);

    else if (IsRemotePath(szPathName) > 0)
        return DRIVE_REMOTE;

    else
        return DRIVE_UNKNOWN;
}

void DriveInfo::FormatName(std::wstring &sFilename) {
    //std::wstring sTemp, sChar;
    //int nLen, nChar;
    //wchar_t cChar, cLastChar = ' '; // space

    //// this function accepts pathnames and names with spaces
    //sFilename.MakeLower();
    //nLen = sFilename.GetLength();

    //// for each word make the first letter upper case
    //for (nChar = 0; nChar < nLen; nChar++)
    //{
    //    cChar = sFilename[nChar];

    //    if (cLastChar == ' ' || cLastChar == '\\')
    //    {
    //        sChar = std::wstring(cChar);
    //        sChar.MakeUpper();
    //        cChar = sChar[0];
    //    }

    //    sTemp += cChar;
    //    cLastChar = cChar;
    //}

    //sFilename = sTemp;
}

bool DriveInfo::IsDriveAvailable(int nDrive) {
    int nCurDrive;
    int nRes;

    // save cur drive and try to change to drive
    nCurDrive = _getdrive();
    nRes = _chdrive(nDrive);

    // if change successful change back before return
    if (nRes == 0)
        _chdrive(nCurDrive);

    return (nRes == 0) ? true : false;
}

bool DriveInfo::IsUNCPath(const wchar_t *szPathName) {
    return (wcsstr(szPathName, L"\\\\") == szPathName);
}

int DriveInfo::GetDrive(const wchar_t *szPathName) {
    int nDrive = 0;

    if (wcsstr(szPathName, L":") == szPathName + 1) {
        wchar_t cDrive = szPathName[0];
        cDrive = (wchar_t)toupper(cDrive);
        nDrive = cDrive - 64;

    }

    return nDrive ? nDrive : -1;
}

bool DriveInfo::IsMappedPath(const wchar_t *szPathName) {
    int nDrive;

    nDrive = GetDrive(szPathName);

    if (nDrive <= 0)
        return FALSE;

    return (GetType(nDrive) == DRIVE_REMOTE);
}

int DriveInfo::IsRemotePath(const wchar_t *szPathName, bool bAllowFileCheck) {
    if (bAllowFileCheck) {
        DWORD dwAttr = ::GetFileAttributesW(szPathName);

        if (dwAttr == 0xffffffff)
            return -1;
    }

    return (IsUNCPath(szPathName) || IsMappedPath(szPathName));
}

bool DriveInfo::IsFixedPath(const wchar_t *szPathName) {
    int nDrive = GetDrive(szPathName);

    if (nDrive == -1) // unknown
        return FALSE;

    return (GetType(nDrive) == DRIVE_FIXED);
}

bool DriveInfo::IsRemovablePath(const wchar_t *szPathName) {
    int nDrive = GetDrive(szPathName);

    if (nDrive == -1) // unknown
        return FALSE;

    return (GetType(nDrive) == DRIVE_REMOVABLE);
}

int DriveInfo::IsReadonlyPath(const wchar_t *szPathName) {
    DWORD dwAttr = ::GetFileAttributesW(szPathName);

    if (dwAttr == 0xffffffff)
        return -1;

    // else
    return (dwAttr & FILE_ATTRIBUTE_READONLY);
}

unsigned long DriveInfo::GetSerialNumber(int nDrive) {
    if (GetType(nDrive) != DRIVE_FIXED)
        return 0;

    DWORD dwHDSerialNum = 0;

    if (!::GetVolumeInformationW(GetRoot(nDrive).c_str(), NULL, 0, &dwHDSerialNum, NULL, NULL, NULL, 0))
        return (DWORD) - 1;

    return dwHDSerialNum;
}
