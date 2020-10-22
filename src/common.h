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
#include "../lib/irrlicht/include/irrlicht.h"
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
#endif

// Screen
#define CSSCREEN_WINDOWED 1
#define CSSCREEN_RESIZABLE 2
#define CSSCREEN_VSYNC 4

// Features
#define CSFEATURE_RENDERTOTEXTURE 0
#define CSFEATURE_BILINEARFILTER 1
#define CSFEATURE_HARDWARETNL 2
#define CSFEATURE_MIPMAP 3
#define CSFEATURE_STENCILBUFFER 4
#define CSFEATURE_VERTEXSHADER11 5
#define CSFEATURE_VERTEXSHADER20 6
#define CSFEATURE_VERTEXSHADER30 7
#define CSFEATURE_PIXELSHADER11 8
#define CSFEATURE_PIXELSHADER12 9
#define CSFEATURE_PIXELSHADER13 10
#define CSFEATURE_PIXELSHADER14 11
#define CSFEATURE_PIXELSHADER20 12
#define CSFEATURE_PIXELSHADER30 13
#define CSFEATURE_ARB_VERTEXPROGRAM1 14
#define CSFEATURE_ARB_FRAGMENTPROGRAM1 15

// Gadget types
#define CSCONTROL_BUTTON 1
#define CSCONTROL_CHECKBOX 2
#define CSCONTROL_COMBOBOX 3
#define CSCONTROL_EDITBOX 4
#define CSCONTROL_IMAGE 5
#define CSCONTROL_LISTBOX 6
#define CSCONTROL_MENU 7
#define CSCONTROL_SCROLLBAR 8
#define CSCONTROL_TABBAR 9
#define CSCONTROL_TEXT 10
#define CSCONTROL_TOOLBAR 11
#define CSCONTROL_WINDOW 12

// Gadget events
#define CSCONTROL_LOSTFOCUS 0
#define CSCONTROL_ENTER 1
#define CSCONTROL_EXIT 2
#define CSCONTROL_ACTION 3

// EditBox types
#define CSEDITBOX_SIMPLE 0
#define CSEDITBOX_PASSWORD 1
#define CSEDITBOX_MULTILINE 2

// Blend modes
#define CSMATERIAL_SOLID 0
#define CSMATERIAL_LIGHTMAP 1
#define CSMATERIAL_LIGHTMAP2X 2
#define CSMATERIAL_LIGHTMAP4X 3
#define CSMATERIAL_DETAIL 4
#define CSMATERIAL_ADD 5
#define CSMATERIAL_ALPHA 6
#define CSMATERIAL_VERTEXALPHA 7
#define CSMATERIAL_MASKED 8
#define CSMATERIAL_SPHERE 9
#define CSMATERIAL_REFLECTION 10
#define CSMATERIAL_NORMAL 11
#define CSMATERIAL_NORMALADD 12
#define CSMATERIAL_PARALLAX 13
#define CSMATERIAL_PARALLAXADD 14

// Render modes
#define CSRENDER_NORMAL 0
#define CSRENDER_WIREFRAME 1
#define CSRENDER_POINTCLOUD 2

// Lighting modes
#define CSLIGHTING_DISABLED 0
#define CSLIGHTING_GOURAUD 1
#define CSLIGHTING_FLAT 2

// Filtering modes
#define CSFILTER_DISABLED 0
#define CSFILTER_BILINEAR 1
#define CSFILTER_TRILINEAR 2
#define CSFILTER_ANISOTROPIC 3

// Light types
#define CSLIGHT_DIRECTIONAL 0
#define CSLIGHT_POINT 1
#define CSLIGHT_SPOT 2

// Collision types
#define CSCOLLISION_NONE 0
#define CSCOLLISION_BOX 1
#define CSCOLLISION_MESH 2
#define CSCOLLISION_OCTREE 3

// Entity types
#define CSENTITY_EMPTY 0
#define CSENTITY_SPRITE 1
#define CSENTITY_CAMERA 2
#define CSENTITY_EMITTER 3
#define CSENTITY_LIGHT 4
#define CSENTITY_MESH 5
#define CSENTITY_MESH_OCTREE 6
#define CSENTITY_TERRAIN 7
#define CSENTITY_WATER 8

// Mouse buttons
#define CSBUTTON_LEFT 1
#define CSBUTTON_RIGHT 2
#define CSBUTTON_MIDDLE 3

// Key codes
#define CSKEY_LBUTTON 0x01
#define CSKEY_RBUTTON 0x02
#define CSKEY_CANCEL 0x03
#define CSKEY_MBUTTON 0x04
#define CSKEY_XBUTTON1 0x05
#define CSKEY_XBUTTON2 0x06
#define CSKEY_BACK 0x08
#define CSKEY_TAB 0x09
#define CSKEY_CLEAR 0x0C
#define CSKEY_RETURN 0x0D
#define CSKEY_ENTER 0x0D
#define CSKEY_SHIFT 0x10
#define CSKEY_CONTROL 0x11
#define CSKEY_MENU 0x12
#define CSKEY_PAUSE 0x13
#define CSKEY_CAPITAL 0x14
#define CSKEY_KANA 0x15
#define CSKEY_HANGUEL 0x15
#define CSKEY_HANGUL 0x15
#define CSKEY_JUNJA 0x17
#define CSKEY_FINAL 0x18
#define CSKEY_HANJA 0x19
#define CSKEY_KANJI 0x19
#define CSKEY_ESC 0x1B
#define CSKEY_ESCAPE 0x1B
#define CSKEY_CONVERT 0x1C
#define CSKEY_NONCONVERT 0x1D
#define CSKEY_ACCEPT 0x1E
#define CSKEY_MODECHANGE 0x1F
#define CSKEY_SPACE 0x20
#define CSKEY_PRIOR 0x21
#define CSKEY_NEXT 0x22
#define CSKEY_END 0x23
#define CSKEY_HOME 0x24
#define CSKEY_LEFT 0x25
#define CSKEY_UP 0x26
#define CSKEY_RIGHT 0x27
#define CSKEY_DOWN 0x28
#define CSKEY_SELECT 0x29
#define CSKEY_PRINT 0x2A
#define CSKEY_EXECUT 0x2B
#define CSKEY_SNAPSHOT 0x2C
#define CSKEY_INSERT 0x2D
#define CSKEY_DELETE 0x2E
#define CSKEY_HELP 0x2F
#define CSKEY_0 0x30
#define CSKEY_1 0x31
#define CSKEY_2 0x32
#define CSKEY_3 0x33
#define CSKEY_4 0x34
#define CSKEY_5 0x35
#define CSKEY_6 0x36
#define CSKEY_7 0x37
#define CSKEY_8 0x38
#define CSKEY_9 0x39
#define CSKEY_A 0x41
#define CSKEY_B 0x42
#define CSKEY_C 0x43
#define CSKEY_D 0x44
#define CSKEY_E 0x45
#define CSKEY_F 0x46
#define CSKEY_G 0x47
#define CSKEY_H 0x48
#define CSKEY_I 0x49
#define CSKEY_J 0x4A
#define CSKEY_K 0x4B
#define CSKEY_L 0x4C
#define CSKEY_M 0x4D
#define CSKEY_N 0x4E
#define CSKEY_O 0x4F
#define CSKEY_P 0x50
#define CSKEY_Q 0x51
#define CSKEY_R 0x52
#define CSKEY_S 0x53
#define CSKEY_T 0x54
#define CSKEY_U 0x55
#define CSKEY_V 0x56
#define CSKEY_W 0x57
#define CSKEY_X 0x58
#define CSKEY_Y 0x59
#define CSKEY_Z 0x5A
#define CSKEY_LWIN 0x5B
#define CSKEY_RWIN 0x5C
#define CSKEY_APPS 0x5D
#define CSKEY_SLEEP 0x5F
#define CSKEY_NUMPAD0 0x60
#define CSKEY_NUMPAD1 0x61
#define CSKEY_NUMPAD2 0x62
#define CSKEY_NUMPAD3 0x63
#define CSKEY_NUMPAD4 0x64
#define CSKEY_NUMPAD5 0x65
#define CSKEY_NUMPAD6 0x66
#define CSKEY_NUMPAD7 0x67
#define CSKEY_NUMPAD8 0x68
#define CSKEY_NUMPAD9 0x69
#define CSKEY_MULTIPLY 0x6A
#define CSKEY_ADD 0x6B
#define CSKEY_SEPARATOR 0x6C
#define CSKEY_SUBTRACT 0x6D
#define CSKEY_DECIMAL 0x6E
#define CSKEY_DIVIDE 0x6F
#define CSKEY_F1 0x70
#define CSKEY_F2 0x71
#define CSKEY_F3 0x72
#define CSKEY_F4 0x73
#define CSKEY_F5 0x74
#define CSKEY_F6 0x75
#define CSKEY_F7 0x76
#define CSKEY_F8 0x77
#define CSKEY_F9 0x78
#define CSKEY_F10 0x79
#define CSKEY_F11 0x7A
#define CSKEY_F12 0x7B
#define CSKEY_F13 0x7C
#define CSKEY_F14 0x7D
#define CSKEY_F15 0x7E
#define CSKEY_F16 0x7F
#define CSKEY_F17 0x80
#define CSKEY_F18 0x81
#define CSKEY_F19 0x82
#define CSKEY_F20 0x83
#define CSKEY_F21 0x84
#define CSKEY_F22 0x85
#define CSKEY_F23 0x86
#define CSKEY_F24 0x87
#define CSKEY_NUMLOCK 0x90
#define CSKEY_SCROLL 0x91
#define CSKEY_LSHIFT 0xA0
#define CSKEY_RSHIFT 0xA1
#define CSKEY_LCONTROL 0xA2
#define CSKEY_RCONTROL 0xA3
#define CSKEY_LMENU 0xA4
#define CSKEY_RMENU 0xA5
#define CSKEY_COMMA 0xBC
#define CSKEY_PLUS 0xBB
#define CSKEY_MINUS 0xBD
#define CSKEY_PERIOD 0xBE
#define CSKEY_ATTN 0xF6
#define CSKEY_CRSEL 0xF7
#define CSKEY_EXSEL 0xF8
#define CSKEY_EREOF 0xF9
#define CSKEY_PLAY 0xFA
#define CSKEY_ZOOM 0xFB
#define CSKEY_PA1 0xFD
#define CSKEY_OEM_CLEAR 0xFE

// Texture flags
#define CSTEXTURE_MIPMAPS 1
#define CSTEXTURE_16BITS 2

// XML node types
#define CSXML_NULL 0
#define CSXML_NODE 1
#define CSXML_NODEEND 2
#define CSXML_TEXT 3
#define CSXML_CDATA 5
#define CSXML_UNKNOWN 6

#define TRUE 1
#define FALSE 0

#ifndef SWIG

typedef int bool_t;


inline const wchar_t* _WSTR(const char* string) {
    static wchar_t wstring[1024];
    mbstowcs(wstring, string, 1024);
    return wstring;
}


inline const char* _CSTR(const wchar_t* wstring) {
    static char string[1024];
    wcstombs(string, wstring, 1024);
    return string;
}

#endif
