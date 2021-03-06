// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// TODO: reference additional headers your program requires here
#include <windows.h>
#include "CommonHeader.h"

#include "CGSFNet/BasePacket.h"

#include <tchar.h>
#include "Macro.h"
#include "SFString.h"
#include "SFObjectPool.h"
#include "SFLogicGateway.h"
#include "LogicEntry.h"
#include "SFPacket.h"
#include "CGSFNet/ErrorCode.h"
#include "SFPacketProtocol.H"

#define GOOGLE_GLOG_DLL_DECL
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "glog/logging.h"

#define BOOST_ALL_NO_LIB 

#ifdef _DEBUG
#pragma comment(lib, "boost_thread-vc140-mt-gd-1_60.lib")
#pragma comment(lib, "boost_system-vc140-mt-gd-1_60.lib")
#else
#pragma comment(lib, "boost_thread-vc140-mt-1_60.lib")
#pragma comment(lib, "boost_system-vc140-mt-1_60.lib")
#endif