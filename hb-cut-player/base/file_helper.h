/*******************************************************************************
 * file_helper.h
 * Copyright: (c) 2014 Haibin Du(haibindev.cnblogs.com). All rights reserved.
 * -----------------------------------------------------------------------------
 *
 *
 *
 * -----------------------------------------------------------------------------
 * 2016-6-8 10:29 - Created (Haibin Du)
 ******************************************************************************/

#ifndef _HDEV_FILE_HELPER_H_
#define _HDEV_FILE_HELPER_H_

#include "base/base.h"

#include <string>
#include <vector>

namespace filehelper {

std::string ThisProcessDir();

/***
    *  创建目录
    */
void MakeAllDir(const std::string path);

/***
    *  删除某个目录（及该目录下所有子目录和文件）
    *  @param delDirPath : 欲删除的目录绝对路径
    *  @param isDelThis ：是否同时删除这个目录
    */
void DelDir(const std::string& delDirPath, bool isDelThis);

/***
    *  获取目录下所有文件和目录
    */
void GetDirFileList(const std::string& dir,
    std::vector<std::string>& fileList, std::vector<std::string>& dirList);

/***
    *  获取目录下所有文件，不包含目录
    */
void GetDirFileList(const std::string& dir, std::vector<std::string>& fileList);

} // namespace filehelper

#endif // _HDEV_FILE_HELPER_H_
