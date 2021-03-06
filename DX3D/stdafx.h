#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#define NOMINMAX    // for std::numeric_limits<T>::max() and min(), windows MIN and MAX macro is ambiguous with this methods
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <cassert>
#include <atlstr.h>

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <set>
using std::set;

#include <list>
using std::list;

#include <string>
using std::to_string;
using std::string;

#include <fstream>

#include <chrono>
using std::chrono::system_clock;
using std::chrono::duration;

#include <unordered_set>
using std::unordered_multiset;
using std::unordered_set;

#include <iterator>
using std::advance;
using std::next;
using std::prev;

#include <iostream>
using std::cout;
using std::endl;

#include <deque>
using std::deque;

#include <unordered_map>
using std::unordered_multimap;
using std::unordered_map;

#include <limits>
using std::numeric_limits;

#include <utility>
using std::make_pair;
using std::pair;

#include <type_traits>
using std::is_base_of_v;
using std::enable_if_t;

#include <functional>
using std::hash;

#include <bitset>
using std::bitset;

#include <typeindex>
using std::type_index;

#include <ostream>
using std::ostream;

#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

extern HWND g_hWnd;

#include "GlobalDefinition.h"
#include "D3DUtility.h"

#include "Debug.h"              // for Debug
#include "Application.h"        // for g_pApp
#include "DeviceManager.h"      // for g_pDeviceManager and g_pDevice
#include "ObjectManager.h"      // for g_pObjectManager
#include "TextureManager.h"     // for g_pTextureManager
#include "TimeManager.h"        // for g_pTimeManager
#include "SceneManager.h"       // for g_pSceneManager
#include "IScene.h"             // for g_pCurrentScene
#include "FontManager.h"        // for g_pFontManager
#include "CameraManager.h"      // for g_pCameraManager
#include "ICamera.h"            // for g_pCurrentCamera
#include "MapManager.h"         // for g_pMapManager
#include "IMap.h"               // for g_pCurrentMap
#include "KeyboardManager.h"    // for g_pKeyboardManager (legacy)
#include "keyManager.h"         // for g_pKeyManager
#include "UIManager.h"          // for g_pUIManager
#include "CollisionManager.h"   // for g_pCollisionManager
#include "SoundManager.h"       // for g_pSoundManager

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
#endif
#define D3DX_DEBUG


