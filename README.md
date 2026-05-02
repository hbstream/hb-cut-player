# HB Cut Player

一个轻量的 Windows 视频播放 + 剪辑小工具。最初是几年前写的基于 VLC 的简易播放器，后来在实际使用里又陆续添了倍速、剪切片段、批量任务这些功能。**2026 年完成了一次完整的现代化重做**：从 VS2013 + Qt 5 32 位工程迁到 Qt 6.10 + CMake 64 位，重画了暗色主题的界面，换上了规范的播放器图标，并适配了 Windows 11。

适合的场景：

- 看本地视频时想顺手把片头片尾、广告或不想保留的片段剪掉
- 给视频做不重新编码的快速截取（基于 FFmpeg `-c copy`）
- 倍速播放，加快看长视频 / 录屏 / 网课的节奏

定位是“**够用、不重**”，不是要做专业 NLE。

## 截图

![界面截图](imgs/HBCutPlayer-modern.png)

## 技术栈

- **界面**：Qt 6.10 / Widgets，全 SVG 矢量图标，QSS 暗色主题
- **播放**：libVLC（默认链接系统已装的 VLC 3.x，免去单独打包播放内核）
- **剪辑**：调用 `ffmpeg.exe` 命令行做无重编码切片
- **构建**：CMake 3.21+ / MSVC 2022 / Windows x64

整体数据流（旧版本架构图，仍然适用）：

![整体框架](imgs/HBCutPlayer2.jpg)

## 构建（Windows x64）

### 依赖

| 组件 | 版本 / 位置 |
|------|------------|
| Visual Studio 2022 | MSVC v143 工具链 |
| Qt | 6.10.x，msvc2022_64，默认 `C:/Qt/6.10.3/msvc2022_64` |
| VLC | 3.x 64 位桌面版，默认 `C:/Program Files/VideoLAN/VLC` |
| CMake | 3.21+ |

### 一键脚本

```powershell
# 1) 从系统 VLC 现场生成 64 位导入库（首次构建必须执行一次）
powershell -NoProfile -ExecutionPolicy Bypass -File scripts/gen_vlc_import_libs.ps1

# 2) 配置 + 编译 + windeployqt 自动部署
powershell -NoProfile -ExecutionPolicy Bypass -File scripts/build.ps1
```

产物在 `build/Release/hb-cut-player.exe`，Qt 运行时与 VLC 插件目录都已经一并部署，双击即可运行。

> 仓库里 `hb-cut-player/vlc/*.lib` 是历史 32 位 VS2013 工程的残留，新构建链路不会再用到；保留只是为了让旧解决方案在存档意义上仍可打开。

### 老 VS2013 工程

`hb-cut-player.sln` / `hb-cut-player.vcxproj` 仅作存档保留，新开发请使用 CMake 路径。

## 操作

- **打开视频**：点左下角文件夹图标，或者直接把文件拖到窗口里
- **播放控制**：空格 = 播放 / 暂停
- **进度跳转**：← / → = ±5s，Ctrl + ← / → = ±30s，Shift + ← / → = ±60s
- **倍速**：±5%、±10% 微调，⟳ 一键回到 1.0×
- **全屏**：双击视频区域切换；Esc 退出全屏
- **剪切**：用“设定开始 / 设定结束”捕捉当前时间点，点“添加剪切任务”
  即可加入下方的任务列表；点“开始剪切”按列表顺序逐个剪切
- **合并**：当前源视频已经剪好的所有片段，点“开始合并”一键拼成一个新文件

### 输出文件命名规则

| 操作 | 命名 |
|------|------|
| 添加剪切任务 | 与源视频同目录，`<源文件名>_1.<ext>`、`_2`、`_3` … 递增；同名直接覆盖；切换源视频时计数器归零 |
| 开始合并 | `<源文件名>_new.<ext>`；若已存在则改为 `_new_1`、`_new_2` … 不会覆盖 |

例如源视频是 `C:/x.mp4`：`x_1.mp4`、`x_2.mp4` … 加完任务并完成剪切后，再点
“开始合并”得到 `C:/x_new.mp4`（已存在则 `x_new_1.mp4`）。

## 开发计划

- 把 ffmpeg 进程调用改为 ffmpeg API
- 视频解码改走 ffmpeg + 自渲染，去掉对系统 VLC 的依赖
- 跨平台支持

## 源码

<https://github.com/hbstream/hb-cut-player>

---

![](imgs/hbstream.jpg) **合作请联系 QQ。（转载请注明作者和出处）**
