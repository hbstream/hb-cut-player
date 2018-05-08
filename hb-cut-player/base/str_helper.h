/*******************************************************************************
 * str_helper.h
 * Copyright: (c) 2014 Haibin Du(haibindev.cnblogs.com). All rights reserved.
 * -----------------------------------------------------------------------------
 *
 * ×Ö·û´®¸¨Öúº¯Êý
 *
 * -----------------------------------------------------------------------------
 * 2016-6-8 11:06 - Created (Haibin Du)
 ******************************************************************************/

#ifndef _HDEV_STR_HELPER_H_
#define _HDEV_STR_HELPER_H_

#include "base/base.h"

#include <string>
#include <vector>

namespace strhelper {

void StringTrim(std::string& str);

std::string StringTrimCopy(const std::string& str);

bool StartWidth(const std::string& str, const std::string& startStr);

void Splite(std::vector<std::string>& result, const std::string& input,
    const std::string& spliter);

void StringReplace(std::string &strBase, const std::string& strSrc,
    const std::string& strDes);

#if defined(WIN32)

std::string Wide2Ansi(const std::wstring& _src);

std::wstring Ansi2Wide(const std::string& _src);

std::string Wide2Utf8(const std::wstring& _src);

std::wstring Utf82Wide(const std::string& _src);

#endif

} // namespace strhelper

#endif // _HDEV_STR_HELPER_H_
