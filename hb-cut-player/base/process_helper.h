/*******************************************************************************
 * process_helper.h
 * Copyright: (c) 2014 Haibin Du(haibindev.cnblogs.com). All rights reserved.
 * -----------------------------------------------------------------------------
 *
 *
 *
 * -----------------------------------------------------------------------------
 * 2018-03-19 12:25 - Created (Haibin Du) from NginxMonitor/util.h
 ******************************************************************************/

#ifndef _HDEV_PROCESS_HELPER_H_
#define _HDEV_PROCESS_HELPER_H_

#include "base/base.h"

#include <string>
#include <vector>

namespace processhelper {

int ProcessExec(const std::wstring& commandLine, bool isWait);

bool KillProcess(const std::wstring& exename, bool lowerCheck);

struct ProcessFindInfo
{
    std::wstring proc_name;
    int proc_id;
    std::wstring proc_module_path;
};

void FindProcessName(const std::wstring& procName,
    std::vector<ProcessFindInfo>* findInfos);

std::wstring GetProcessModulePath(DWORD pid, const std::wstring& exeName);

} // namespace processhelper

#endif // _HDEV_PROCESS_HELPER_H_
