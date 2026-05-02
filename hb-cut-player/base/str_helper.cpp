/*******************************************************************************
 * str_helper.cpp
 * Copyright: (c) 2014 Haibin Du(haibindev.cnblogs.com). All rights reserved.
 * -----------------------------------------------------------------------------
 *
 * 字符串相关帮助函数。
 *
 * -----------------------------------------------------------------------------
 * 2016-6-8 11:06 - Created (Haibin Du)
 ******************************************************************************/
#include "str_helper.h"

#include <algorithm>
#include <cctype>

namespace strhelper {

namespace {
inline bool IsNotSpace(unsigned char c) { return !std::isspace(c); }
} // namespace

void StringTrim(std::string& str)
{
    auto it = std::find_if(str.begin(), str.end(), [](unsigned char c) { return IsNotSpace(c); });
    str.erase(str.begin(), it);

    auto rit = std::find_if(str.rbegin(), str.rend(), [](unsigned char c) { return IsNotSpace(c); });
    str.erase(rit.base(), str.end());
}

std::string StringTrimCopy(const std::string& str)
{
    std::string retstr = str;
    StringTrim(retstr);
    return retstr;
}

bool StartWidth(const std::string& str, const std::string& startStr)
{
    return (str.find(startStr) == 0);
}

void Splite(std::vector<std::string>& result, const std::string& input,
    const std::string& spliter)
{
    std::string::size_type spliter_length = spliter.size();
    std::string::size_type last_pos = 0;

    while (last_pos < input.length())
    {
        std::string::size_type pos = input.find(spliter, last_pos);
        if (pos == std::string::npos)
        {
            result.push_back(input.substr(last_pos));
            return;
        }
        result.push_back(input.substr(last_pos, pos - last_pos));
        last_pos = pos + spliter_length;
    }
}

void StringReplace(std::string &strBase, const std::string& strSrc,
    const std::string& strDes)
{
    std::string::size_type pos = 0;
    std::string::size_type srcLen = strSrc.size();
    std::string::size_type desLen = strDes.size();
    pos = strBase.find(strSrc, pos);
    while ((pos != std::string::npos))
    {
        strBase.replace(pos, srcLen, strDes);
        pos = strBase.find(strSrc, (pos + desLen));
    }
}

#if defined(WIN32)

std::string Wide2Ansi(const std::wstring& _src)
{
    int nBufSize = ::WideCharToMultiByte(GetACP(), 0, _src.c_str(), -1, NULL, 0, 0, FALSE);
    char *szBuf = new char[nBufSize + 1];
    ::WideCharToMultiByte(GetACP(), 0, _src.c_str(), -1, szBuf, nBufSize, 0, FALSE);
    std::string strRet(szBuf);
    delete[] szBuf;
    return strRet;
}

std::wstring Ansi2Wide(const std::string& _src)
{
    int nBufSize = ::MultiByteToWideChar(GetACP(), 0, _src.c_str(), -1, NULL, 0);
    wchar_t *wsBuf = new wchar_t[nBufSize + 1];
    ::MultiByteToWideChar(GetACP(), 0, _src.c_str(), -1, wsBuf, nBufSize);
    std::wstring wstrRet(wsBuf);
    delete[] wsBuf;
    return wstrRet;
}

std::string Wide2Utf8(const std::wstring& _src)
{
    int nBufSize = ::WideCharToMultiByte(CP_UTF8, 0, _src.c_str(), -1, NULL, 0, NULL, NULL);
    char *szBuf = new char[nBufSize + 1];
    ::WideCharToMultiByte(CP_UTF8, 0, _src.c_str(), -1, szBuf, nBufSize, NULL, NULL);
    std::string strRet(szBuf);
    delete[] szBuf;
    return strRet;
}

std::wstring Utf82Wide(const std::string& _src)
{
    int nBufSize = ::MultiByteToWideChar(CP_UTF8, 0, _src.c_str(), -1, NULL, 0);
    wchar_t *wsBuf = new wchar_t[nBufSize + 1];
    ::MultiByteToWideChar(CP_UTF8, 0, _src.c_str(), -1, wsBuf, nBufSize);
    std::wstring wstrRet(wsBuf);
    delete[] wsBuf;
    return wstrRet;
}

#endif // #if defined(WIN32)

} // namespace strhelper
