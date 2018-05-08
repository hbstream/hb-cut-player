/*******************************************************************************
 * file_helper.cpp
 * Copyright: (c) 2014 Haibin Du(haibindev.cnblogs.com). All rights reserved.
 * -----------------------------------------------------------------------------
 *
 * -----------------------------------------------------------------------------
 * 2016-6-8 10:29 - Created (Haibin Du)
 ******************************************************************************/

#include "file_helper.h"

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include "base/str_helper.h"

namespace filehelper {

std::string ThisProcessDir()
{
    static std::string process_path;

    if (process_path.empty())
    {
#if !defined(WIN32)
        char tmpbuf[1024] = { 0 };
        readlink("/proc/self/exe", tmpbuf, 1024);

        process_path = tmpbuf;
        std::string::size_type pos = process_path.rfind("/");
        process_path = process_path.substr(0, pos + 1);
#else
        WCHAR tmp_buf[MAX_PATH];
        ::GetModuleFileNameW(NULL, tmp_buf, MAX_PATH);

        std::wstring exepath = tmp_buf;
        std::wstring::size_type pos = exepath.rfind(L"\\");
        exepath = exepath.substr(0, pos + 1);

        process_path = strhelper::Wide2Ansi(exepath);
#endif
    }

    return process_path;
}

void MakeAllDir(const std::string path)
{
    if (::PathFileExistsA(path.c_str())) return;

    if (!::CreateDirectoryA(path.c_str(), NULL))
    {
        std::string::size_type pos = path.rfind('\\');
        std::string parent_path = path.substr(0, pos);
        MakeAllDir(parent_path);
        ::CreateDirectoryA(path.c_str(), NULL);
    }
}

void DelDir(const std::string& delDirPath, bool isDelThis)
{
    std::string dir_path = delDirPath;
    std::string dir_name = delDirPath;
    std::string::size_type path_pos = dir_path.rfind("\\");
    std::string::size_type name_pos = dir_name.rfind("\\");
    if (path_pos != dir_path.size()-1)
    {
        dir_path.append("\\");
    }
    if (name_pos == dir_name.size()-1)
    {
        dir_name = dir_name.substr(0, name_pos);
    }

    std::vector<std::string> file_list;
    std::vector<std::string> dir_list;

    GetDirFileList(dir_path, file_list, dir_list);

    for (std::vector<std::string>::iterator it = file_list.begin();
        it != file_list.end(); ++it)
    {
        ::DeleteFileA(it->c_str());
    }

    for (std::vector<std::string>::iterator it = dir_list.begin();
        it != dir_list.end(); ++it)
    {
        DelDir(*it, true);
    }

    if (isDelThis)
    {
        ::RemoveDirectoryA(dir_name.c_str());
    }
}

void GetDirFileList(const std::string& dir, 
    std::vector<std::string>& fileList, std::vector<std::string>& dirList)
{
    std::string find_dirstr = dir;
    if (find_dirstr.rfind("\\") != find_dirstr.size()-1)
        find_dirstr.append("\\");

    std::string tmp_find_str = find_dirstr;
    tmp_find_str.append("*");

    WIN32_FIND_DATAA find_data;
    HANDLE hFind = ::FindFirstFileA(tmp_find_str.c_str(), &find_data);
    if (hFind == INVALID_HANDLE_VALUE) 
    {
        return ;
    }

    std::string cfilename;

    do 
    {
        cfilename.assign(find_data.cFileName);
        if((cfilename.size()==2)&&(cfilename.find("..")==0))
            continue;
        if ((cfilename.size()==1)&&(cfilename.find('.')==0))
            continue;
        if ((find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            == FILE_ATTRIBUTE_DIRECTORY)		// 目录
        {
            dirList.push_back(find_dirstr + cfilename + "\\");
        }
        else									// 文件
        {
            fileList.push_back(find_dirstr + cfilename);
        }
    } while (::FindNextFileA(hFind, &find_data) != 0);

    DWORD dwError = ::GetLastError();
    ::FindClose(hFind);
}

void GetDirFileList(const std::string& dir, std::vector<std::string>& fileList)
{
    std::string find_dirstr = dir;
    if (find_dirstr.rfind("\\") != find_dirstr.size()-1)
        find_dirstr.append("\\");

    std::string tmp_find_str = find_dirstr;
    tmp_find_str.append("*");

    WIN32_FIND_DATAA find_data;
    HANDLE hFind = ::FindFirstFileA(tmp_find_str.c_str(), &find_data);
    if (hFind == INVALID_HANDLE_VALUE) 
    {
        return ;
    }

    std::string cfilename;

    do 
    {
        cfilename.assign(find_data.cFileName);
        if((cfilename.size()==2)&&(cfilename.find("..")==0))
            continue;
        if ((cfilename.size()==1)&&(cfilename.find('.')==0))
            continue;
        if ((find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            == FILE_ATTRIBUTE_DIRECTORY)		// 目录
        {
        }
        else									// 文件
        {
            fileList.push_back(find_dirstr + cfilename);
        }
    } while (::FindNextFileA(hFind, &find_data) != 0);

    DWORD dwError = ::GetLastError();
    ::FindClose(hFind);
}

}   // namespace filehelper
