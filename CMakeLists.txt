cmake_minimum_required(VERSION 3.21)
project(hb-cut-player LANGUAGES CXX)

# 由 8 年前的 VS2013 + Qt5 工程迁移到 Qt 6 + 现代 CMake 构建。
# 默认链接系统已安装的 Qt 6.10.3 / msvc2022_64 与系统 VLC 3.x。

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(MSVC)
    add_compile_options(/utf-8 /W3 /wd4244 /wd4267)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS NOMINMAX)
endif()

# Qt 6 自动发现：优先使用环境变量 / 本机默认安装位置。
if(NOT DEFINED CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH "C:/Qt/6.10.3/msvc2022_64")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/hb-cut-player)

set(SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/XAVIPlayer.cpp
    ${SRC_DIR}/XAVIPlayer.h
    ${SRC_DIR}/XAVIPlayer.ui
    ${SRC_DIR}/XAVIPlayer.qrc
    ${SRC_DIR}/VLCInstance.cpp
    ${SRC_DIR}/VLCInstance.h
    ${SRC_DIR}/VLCPlayer.cpp
    ${SRC_DIR}/VLCPlayer.h
    ${SRC_DIR}/file_cut_thread.cpp
    ${SRC_DIR}/file_cut_thread.h
    ${SRC_DIR}/base/SimpleThread.cpp
    ${SRC_DIR}/base/SimpleThread.h
    ${SRC_DIR}/base/file_helper.cpp
    ${SRC_DIR}/base/file_helper.h
    ${SRC_DIR}/base/process_helper.cpp
    ${SRC_DIR}/base/process_helper.h
    ${SRC_DIR}/base/str_helper.cpp
    ${SRC_DIR}/base/str_helper.h
)

qt_add_executable(hb-cut-player WIN32 ${SOURCES})

target_include_directories(hb-cut-player PRIVATE
    ${SRC_DIR}
    ${SRC_DIR}/vlc
)

# VLC 64 位导入库由 scripts/gen_vlc_import_libs.ps1 从系统 VLC 现场生成。
set(VLC_LIB_DIR ${SRC_DIR}/vlc/x64)
target_link_libraries(hb-cut-player PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets
    ${VLC_LIB_DIR}/libvlc.lib
    ${VLC_LIB_DIR}/libvlccore.lib
    Urlmon Shlwapi
)

# 同步部署运行时所需 Qt 插件 / VLC dll，避免找不到平台插件。
set(VLC_RUNTIME_DIR "C:/Program Files/VideoLAN/VLC")
add_custom_command(TARGET hb-cut-player POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${VLC_RUNTIME_DIR}/libvlc.dll"
        "${VLC_RUNTIME_DIR}/libvlccore.dll"
        $<TARGET_FILE_DIR:hb-cut-player>
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${VLC_RUNTIME_DIR}/plugins"
        "$<TARGET_FILE_DIR:hb-cut-player>/plugins"
)

# 部署原仓库自带的 ffmpeg / MediaInfo 工具集（剪切 / 合并依赖 ffmpeg.exe）。
set(BUNDLED_TOOLS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Win32/Debug/tools)
if(EXISTS ${BUNDLED_TOOLS_DIR})
    add_custom_command(TARGET hb-cut-player POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${BUNDLED_TOOLS_DIR}"
            "$<TARGET_FILE_DIR:hb-cut-player>/tools"
    )
endif()

if(WIN32)
    find_program(QT_DEPLOY_EXECUTABLE windeployqt
        HINTS ${Qt6_DIR}/../../../bin)
    if(QT_DEPLOY_EXECUTABLE)
        add_custom_command(TARGET hb-cut-player POST_BUILD
            COMMAND ${QT_DEPLOY_EXECUTABLE}
                --no-translations --no-system-d3d-compiler --no-opengl-sw
                $<TARGET_FILE:hb-cut-player>
            COMMENT "Running windeployqt")
    endif()
endif()
