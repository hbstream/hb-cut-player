/*
 *  locate which platform we are on
 */

#ifndef __HB_PLATFORM_H__
#define __HB_PLATFORM_H__

/* ---------- os ---------- */
// linux
#if defined linux || defined __linux || defined __linux__
#define HB_PLATFORM_LINUX
// win32
#elif defined _WIN32 || defined __WIN32__ || defined WIN32
#define HB_PLATFORM_WIN32
// not support
#else
#error "Not Support platform"
#endif

#endif // __HB_PLATFORM_H__
