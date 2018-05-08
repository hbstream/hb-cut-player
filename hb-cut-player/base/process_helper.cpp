/*******************************************************************************
 * process_helper.cpp
 * Copyright: (c) 2014 Haibin Du(haibindev.cnblogs.com). All rights reserved.
 * -----------------------------------------------------------------------------
 *
 * -----------------------------------------------------------------------------
 * 2018-03-19 12:25 - Created (Haibin Du) from NginxMonitor/util.cpp
 ******************************************************************************/

#include "process_helper.h"

#include <tlhelp32.h>

#include <sstream>

//#include "base/str_helper.h"

namespace processhelper {
	
int ProcessExec(const std::wstring& commandLine, bool isWait)
{
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFOW siStartInfo;
    DWORD dwCreateflag = CREATE_NO_WINDOW;

    siStartInfo.cb = sizeof(STARTUPINFOA);
    siStartInfo.lpReserved = NULL;
    siStartInfo.lpReserved2 = NULL;
    siStartInfo.cbReserved2 = 0;
    siStartInfo.lpDesktop = NULL;
    siStartInfo.dwFlags = 0;
    siStartInfo.lpTitle = L"";

    if (FALSE == ::CreateProcessW(NULL, (LPWSTR)commandLine.c_str(), 
        NULL, NULL, 0, dwCreateflag,
        NULL, NULL, &siStartInfo, &piProcInfo) )
    {
        return -1;
    }

    if (false == isWait)
        return 0;

    if (WAIT_OBJECT_0 == ::WaitForSingleObject(piProcInfo.hProcess, INFINITE))
    {
        DWORD exit_code;
        ::GetExitCodeProcess(piProcInfo.hProcess, &exit_code);
        return exit_code;
    }
    else
    {
        return -2;
    }
}

std::wstring StrToLower(const std::wstring& str)
{
    std::wstring retstr = L"";
    for (int i = 0; i < str.length(); ++i)
    {
        retstr += (tolower(str[i]));
    }
    return retstr;
}

bool KillProcess(const std::wstring& exename, bool lowerCheck)
{
    bool is_find = false;

    HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    std::wstring exe_name = exename;
    if (lowerCheck)
        exe_name = StrToLower(exename);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return is_find;
    }
    PROCESSENTRY32W process_entry;
    process_entry.dwSize = sizeof(PROCESSENTRY32W);
    DWORD curr_pid = ::GetCurrentProcessId();

    if (::Process32FirstW(hProcessSnap, &process_entry))
    {
        do
        {
            if (0 == process_entry.th32ProcessID
                || process_entry.th32ProcessID == curr_pid)
            {
                continue;
            }
            else
            {
                std::wstring process_name(process_entry.szExeFile);
                if (lowerCheck)
                    process_name = StrToLower(process_name);
                if (exe_name == process_name)
                {
                    HANDLE ProcessHandle;
                    ProcessHandle = ::OpenProcess(
                        PROCESS_TERMINATE, FALSE, process_entry.th32ProcessID);
                    ::TerminateProcess(ProcessHandle, 0);
                    ::CloseHandle(ProcessHandle);
                    //::CloseHandle(hProcessSnap);
                    
                    is_find = true;
                }
            }
        } while (::Process32Next(hProcessSnap, &process_entry));
    }
    ::CloseHandle(hProcessSnap);

    return is_find;
}

void FindProcessName(const std::wstring& procName,
    std::vector<ProcessFindInfo>* findInfos)
{
    if (findInfos == NULL) return;

    HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe = { sizeof(pe) };
    for (BOOL fOk = ::Process32First(hSnapshot, &pe); fOk;
        fOk = ::Process32Next(hSnapshot, &pe))
    {
        std::wstring exename(pe.szExeFile);
        if (exename != procName)
        {
            continue;
        }

        std::wstring fullpath = GetProcessModulePath(pe.th32ProcessID, exename);

        ProcessFindInfo info;
        info.proc_id = pe.th32ParentProcessID;
        info.proc_name = procName;
        info.proc_module_path = fullpath;

        findInfos->push_back(info);

//         SHFILEINFOW shFileInfo = { 0 };
//         ::SHGetFileInfoW(fullpath.c_str(), 0, &shFileInfo, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX);
// 
//         CString main_txt;
//         HWND mainwnd = ProcessMainWindow(pe.th32ProcessID, main_txt);
// 
//         int item_index = pid_list_.GetItemCount();
//         if (mainwnd)
//         {
//             pid_list_.InsertItem(item_index, _T(""), shFileInfo.iIcon);
// 
//             pid_list_.SetItemText(item_index, 0, main_txt);
//             CString tmp; tmp.Format(_T("%d"), pe.th32ProcessID);
//             pid_list_.SetItemText(item_index, 1, tmp);
//         }
//         else
//         {
//             //pid_list_.InsertItem(item_index, _T(""));
//         }
// 
//         if (pe.th32ProcessID == select_pid)
//         {
//             pid_list_.SetItemState(item_index, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);
//         }
    }
    ::CloseHandle(hSnapshot);
}

std::wstring GetProcessModulePath(DWORD pid, const std::wstring& exeName)
{
    std::wstring module_path;

    HANDLE hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (hModuleSnap == INVALID_HANDLE_VALUE)
    {
        return module_path;
    }

    MODULEENTRY32W me32;
    me32.dwSize = sizeof(MODULEENTRY32);

    for (BOOL fOk = ::Module32FirstW(hModuleSnap, &me32); fOk;
        fOk = ::Module32NextW(hModuleSnap, &me32))
    {
        std::wstring tmp(me32.szModule);
        if (tmp == exeName)
        {
            module_path = me32.szExePath;
        }
    }

    ::CloseHandle(hModuleSnap);

    return module_path;
}

} // namespace processhelper
