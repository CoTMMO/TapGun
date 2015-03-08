
#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "Ios_Func.h"

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "Platform/Android_Func.h"

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#include "Platform/Win32_Func.h"

#endif

#endif // __PLATFORM_H__
