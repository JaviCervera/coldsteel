#pragma once

#ifdef _WIN32
#if defined(DLLEXPORT)
#define EXPORT __declspec(dllexport)
#elif defined(DLLIMPORT)
#define EXPORT __declspec(dllimport)
#else
#define EXPORT
#endif
#define CALL __stdcall
#else
#define EXPORT
#define CALL
#endif

#ifndef SWIG
#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
#endif

// Colors
#define COLOR_RED -65536
#define COLOR_GREEN -16711936
#define COLOR_BLUE -16776961
#define COLOR_CYAN -16711681
#define COLOR_MAGENTA -65281
#define COLOR_YELLOW -256
#define COLOR_BLACK -16777216
#define COLOR_WHITE -1
#define COLOR_GRAY -8355712
#define COLOR_LIGHTGRAY -4210753
#define COLOR_DARKGRAY -12566464
#define COLOR_ORANGE -23296
#define COLOR_BROWN -7650029

// Screen
#define SCREEN_FULLSCREEN 1
#define SCREEN_RESIZABLE 2
#define SCREEN_VSYNC 4

// Features
#define FEATURE_RENDERTOTEXTURE 0
#define FEATURE_BILINEARFILTER 1
#define FEATURE_HARDWARETNL 2
#define FEATURE_MIPMAP 3
#define FEATURE_STENCILBUFFER 4
#define FEATURE_VERTEXSHADER11 5
#define FEATURE_VERTEXSHADER20 6
#define FEATURE_VERTEXSHADER30 7
#define FEATURE_PIXELSHADER11 8
#define FEATURE_PIXELSHADER12 9
#define FEATURE_PIXELSHADER13 10
#define FEATURE_PIXELSHADER14 11
#define FEATURE_PIXELSHADER20 12
#define FEATURE_PIXELSHADER30 13
#define FEATURE_ARB_VERTEXPROGRAM1 14
#define FEATURE_ARB_FRAGMENTPROGRAM1 15

// Gadget types
#define CONTROL_BUTTON 1
#define CONTROL_CHECKBOX 2
#define CONTROL_COMBOBOX 3
#define CONTROL_EDITBOX 4
#define CONTROL_IMAGE 5
#define CONTROL_LISTBOX 6
#define CONTROL_MENU 7
#define CONTROL_SCROLLBAR 8
#define CONTROL_TABBAR 9
#define CONTROL_TEXT 10
#define CONTROL_TOOLBAR 11
#define CONTROL_WINDOW 12

// Gadget events
#define CONTROL_LOSTFOCUS 0
#define CONTROL_ENTER 1
#define CONTROL_EXIT 2
#define CONTROL_ACTION 3

// EditBox types
#define EDITBOX_SIMPLE 0
#define EDITBOX_PASSWORD 1
#define EDITBOX_MULTILINE 2

// Blend modes
#define MATERIAL_SOLID 0
#define MATERIAL_LIGHTMAP 1
#define MATERIAL_LIGHTMAP2X 2
#define MATERIAL_LIGHTMAP4X 3
#define MATERIAL_DETAIL 4
#define MATERIAL_ADD 5
#define MATERIAL_ALPHA 6
#define MATERIAL_VERTEXALPHA 7
#define MATERIAL_MASKED 8
#define MATERIAL_SPHERE 9
#define MATERIAL_REFLECTION 10
#define MATERIAL_NORMAL 11
#define MATERIAL_NORMALADD 12
#define MATERIAL_PARALLAX 13
#define MATERIAL_PARALLAXADD 14

// Render modes
#define RENDER_FILLED 0
#define RENDER_WIREFRAME 1
#define RENDER_POINTCLOUD 2

// Lighting modes
#define LIGHTING_DISABLED 0
#define LIGHTING_GOURAUD 1
#define LIGHTING_FLAT 2

// Filtering modes
#define FILTER_DISABLED 0
#define FILTER_BILINEAR 1
#define FILTER_TRILINEAR 2
#define FILTER_ANISOTROPIC 3

// Light types
#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2

// Collision types
#define COLLISION_NONE 0
#define COLLISION_BOX 1
#define COLLISION_MESH 2
#define COLLISION_OCTREE 3

// Entity types
#define ENTITY_EMPTY 0
#define ENTITY_SPRITE 1
#define ENTITY_CAMERA 2
#define ENTITY_EMITTER 3
#define ENTITY_LIGHT 4
#define ENTITY_MESH 5
#define ENTITY_MESH_OCTREE 6
#define ENTITY_TERRAIN 7
#define ENTITY_WATER 8

// Mouse buttons
#define BUTTON_LEFT 1
#define BUTTON_RIGHT 2
#define BUTTON_MIDDLE 3

// Key codes
#define KEY_LBUTTON 0x01
#define KEY_RBUTTON 0x02
#define KEY_CANCEL 0x03
#define KEY_MBUTTON 0x04
#define KEY_XBUTTON1 0x05
#define KEY_XBUTTON2 0x06
#define KEY_BACK 0x08
#define KEY_TAB 0x09
#define KEY_CLEAR 0x0C
#define KEY_RETURN 0x0D
#define KEY_ENTER 0x0D
#define KEY_SHIFT 0x10
#define KEY_CONTROL 0x11
#define KEY_MENU 0x12
#define KEY_PAUSE 0x13
#define KEY_CAPITAL 0x14
#define KEY_KANA 0x15
#define KEY_HANGUEL 0x15
#define KEY_HANGUL 0x15
#define KEY_JUNJA 0x17
#define KEY_FINAL 0x18
#define KEY_HANJA 0x19
#define KEY_KANJI 0x19
#define KEY_ESC 0x1B
#define KEY_ESCAPE 0x1B
#define KEY_CONVERT 0x1C
#define KEY_NONCONVERT 0x1D
#define KEY_ACCEPT 0x1E
#define KEY_MODECHANGE 0x1F
#define KEY_SPACE 0x20
#define KEY_PRIOR 0x21
#define KEY_NEXT 0x22
#define KEY_END 0x23
#define KEY_HOME 0x24
#define KEY_LEFT 0x25
#define KEY_UP 0x26
#define KEY_RIGHT 0x27
#define KEY_DOWN 0x28
#define KEY_SELECT 0x29
#define KEY_PRINT 0x2A
#define KEY_EXECUT 0x2B
#define KEY_SNAPSHOT 0x2C
#define KEY_INSERT 0x2D
#define KEY_DELETE 0x2E
#define KEY_HELP 0x2F
#define KEY_0 0x30
#define KEY_1 0x31
#define KEY_2 0x32
#define KEY_3 0x33
#define KEY_4 0x34
#define KEY_5 0x35
#define KEY_6 0x36
#define KEY_7 0x37
#define KEY_8 0x38
#define KEY_9 0x39
#define KEY_A 0x41
#define KEY_B 0x42
#define KEY_C 0x43
#define KEY_D 0x44
#define KEY_E 0x45
#define KEY_F 0x46
#define KEY_G 0x47
#define KEY_H 0x48
#define KEY_I 0x49
#define KEY_J 0x4A
#define KEY_K 0x4B
#define KEY_L 0x4C
#define KEY_M 0x4D
#define KEY_N 0x4E
#define KEY_O 0x4F
#define KEY_P 0x50
#define KEY_Q 0x51
#define KEY_R 0x52
#define KEY_S 0x53
#define KEY_T 0x54
#define KEY_U 0x55
#define KEY_V 0x56
#define KEY_W 0x57
#define KEY_X 0x58
#define KEY_Y 0x59
#define KEY_Z 0x5A
#define KEY_LWIN 0x5B
#define KEY_RWIN 0x5C
#define KEY_APPS 0x5D
#define KEY_SLEEP 0x5F
#define KEY_NUMPAD0 0x60
#define KEY_NUMPAD1 0x61
#define KEY_NUMPAD2 0x62
#define KEY_NUMPAD3 0x63
#define KEY_NUMPAD4 0x64
#define KEY_NUMPAD5 0x65
#define KEY_NUMPAD6 0x66
#define KEY_NUMPAD7 0x67
#define KEY_NUMPAD8 0x68
#define KEY_NUMPAD9 0x69
#define KEY_MULTIPLY 0x6A
#define KEY_ADD 0x6B
#define KEY_SEPARATOR 0x6C
#define KEY_SUBTRACT 0x6D
#define KEY_DECIMAL 0x6E
#define KEY_DIVIDE 0x6F
#define KEY_F1 0x70
#define KEY_F2 0x71
#define KEY_F3 0x72
#define KEY_F4 0x73
#define KEY_F5 0x74
#define KEY_F6 0x75
#define KEY_F7 0x76
#define KEY_F8 0x77
#define KEY_F9 0x78
#define KEY_F10 0x79
#define KEY_F11 0x7A
#define KEY_F12 0x7B
#define KEY_F13 0x7C
#define KEY_F14 0x7D
#define KEY_F15 0x7E
#define KEY_F16 0x7F
#define KEY_F17 0x80
#define KEY_F18 0x81
#define KEY_F19 0x82
#define KEY_F20 0x83
#define KEY_F21 0x84
#define KEY_F22 0x85
#define KEY_F23 0x86
#define KEY_F24 0x87
#define KEY_NUMLOCK 0x90
#define KEY_SCROLL 0x91
#define KEY_LSHIFT 0xA0
#define KEY_RSHIFT 0xA1
#define KEY_LCONTROL 0xA2
#define KEY_RCONTROL 0xA3
#define KEY_LMENU 0xA4
#define KEY_RMENU 0xA5
#define KEY_COMMA 0xBC
#define KEY_PLUS 0xBB
#define KEY_MINUS 0xBD
#define KEY_PERIOD 0xBE
#define KEY_ATTN 0xF6
#define KEY_CRSEL 0xF7
#define KEY_EXSEL 0xF8
#define KEY_EREOF 0xF9
#define KEY_PLAY 0xFA
#define KEY_ZOOM 0xFB
#define KEY_PA1 0xFD
#define KEY_OEM_CLEAR 0xFE

// Texture flags
#define TEXTURE_MIPMAPS 1
#define TEXTURE_16BITS 2

// XML node types
#define XML_NULL 0
#define XML_NODE 1
#define XML_NODEEND 2
#define XML_TEXT 3
#define XML_CDATA 5
#define XML_UNKNOWN 6

#define TRUE 1
#define FALSE 0

#ifndef SWIG

typedef int bool_t;

inline const wchar_t *_WSTR(const char *string)
{
  static wchar_t wstring[1024];
  mbstowcs(wstring, string, 1024);
  return wstring;
}

inline const char *_CSTR(const wchar_t *wstring)
{
  static char string[1024];
  wcstombs(string, wstring, 1024);
  return string;
}

#endif
