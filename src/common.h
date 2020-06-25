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

#include "../lib/irrlicht/include/irrlicht.h"
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

// Screen
#define ASSCREEN_WINDOWED 1
#define ASSCREEN_RESIZABLE 2
#define ASSCREEN_VSYNC 4

// Features
#define ASFEATURE_RENDERTOTEXTURE 0
#define ASFEATURE_BILINEARFILTER 1
#define ASFEATURE_HARDWARETNL 2
#define ASFEATURE_MIPMAP 3
#define ASFEATURE_STENCILBUFFER 4
#define ASFEATURE_VERTEXSHADER11 5
#define ASFEATURE_VERTEXSHADER20 6
#define ASFEATURE_VERTEXSHADER30 7
#define ASFEATURE_PIXELSHADER11 8
#define ASFEATURE_PIXELSHADER12 9
#define ASFEATURE_PIXELSHADER13 10
#define ASFEATURE_PIXELSHADER14 11
#define ASFEATURE_PIXELSHADER20 12
#define ASFEATURE_PIXELSHADER30 13
#define ASFEATURE_ARB_VERTEXPROGRAM1 14
#define ASFEATURE_ARB_FRAGMENTPROGRAM1 15

// Gadget types
#define ASCONTROL_BUTTON 1
#define ASCONTROL_CHECKBOX 2
#define ASCONTROL_COMBOBOX 3
#define ASCONTROL_EDITBOX 4
#define ASCONTROL_IMAGE 5
#define ASCONTROL_LISTBOX 6
#define ASCONTROL_MENU 7
#define ASCONTROL_SCROLLBAR 8
#define ASCONTROL_TABBAR 9
#define ASCONTROL_TEXT 10
#define ASCONTROL_TOOLBAR 11
#define ASCONTROL_WINDOW 12

// Gadget events
#define ASCONTROL_LOSTFOCUS 0
#define ASCONTROL_ENTER 1
#define ASCONTROL_EXIT 2
#define ASCONTROL_ACTION 3

// EditBox types
#define ASEDITBOX_SIMPLE 0
#define ASEDITBOX_PASSWORD 1
#define ASEDITBOX_MULTILINE 2

// Blend modes
#define ASMATERIAL_SOLID 0
#define ASMATERIAL_LIGHTMAP 1
#define ASMATERIAL_LIGHTMAP2X 2
#define ASMATERIAL_LIGHTMAP4X 3
#define ASMATERIAL_DETAIL 4
#define ASMATERIAL_ADD 5
#define ASMATERIAL_ALPHA 6
#define ASMATERIAL_VERTEXALPHA 7
#define ASMATERIAL_MASKED 8
#define ASMATERIAL_SPHERE 9
#define ASMATERIAL_REFLECTION 10
#define ASMATERIAL_NORMAL 11
#define ASMATERIAL_NORMALADD 12
#define ASMATERIAL_PARALLAX 13
#define ASMATERIAL_PARALLAXADD 14

// Render modes
#define ASRENDER_NORMAL 0
#define ASRENDER_WIREFRAME 1
#define ASRENDER_POINTCLOUD 2

// Lighting modes
#define ASLIGHTING_DISABLED 0
#define ASLIGHTING_GOURAUD 1
#define ASLIGHTING_FLAT 2

// Filtering modes
#define ASFILTER_DISABLED 0
#define ASFILTER_BILINEAR 1
#define ASFILTER_TRILINEAR 2
#define ASFILTER_ANISOTROPIC 3

// Light types
#define ASLIGHT_DIRECTIONAL 0
#define ASLIGHT_POINT 1
#define ASLIGHT_SPOT 2

// Collision types
#define ASCOLLISION_NONE 0
#define ASCOLLISION_BOX 1
#define ASCOLLISION_MESH 2
#define ASCOLLISION_OCTREE 3

// Entity types
#define ASENTITY_EMPTY 0
#define ASENTITY_SPRITE 1
#define ASENTITY_CAMERA 2
#define ASENTITY_EMITTER 3
#define ASENTITY_LIGHT 4
#define ASENTITY_MESH 5
#define ASENTITY_MESH_OCTREE 6
#define ASENTITY_TERRAIN 7
#define ASENTITY_WATER 8

// Mouse buttons
#define ASBUTTON_LEFT 1
#define ASBUTTON_RIGHT 2
#define ASBUTTON_MIDDLE 3

// Key codes
#define ASKEY_LBUTTON 0x01
#define ASKEY_RBUTTON 0x02
#define ASKEY_CANCEL 0x03
#define ASKEY_MBUTTON 0x04
#define ASKEY_XBUTTON1 0x05
#define ASKEY_XBUTTON2 0x06
#define ASKEY_BACK 0x08
#define ASKEY_TAB 0x09
#define ASKEY_CLEAR 0x0C
#define ASKEY_RETURN 0x0D
#define ASKEY_ENTER 0x0D
#define ASKEY_SHIFT 0x10
#define ASKEY_CONTROL 0x11
#define ASKEY_MENU 0x12
#define ASKEY_PAUSE 0x13
#define ASKEY_CAPITAL 0x14
#define ASKEY_KANA 0x15
#define ASKEY_HANGUEL 0x15
#define ASKEY_HANGUL 0x15
#define ASKEY_JUNJA 0x17
#define ASKEY_FINAL 0x18
#define ASKEY_HANJA 0x19
#define ASKEY_KANJI 0x19
#define ASKEY_ESC 0x1B
#define ASKEY_ESCAPE 0x1B
#define ASKEY_CONVERT 0x1C
#define ASKEY_NONCONVERT 0x1D
#define ASKEY_ACCEPT 0x1E
#define ASKEY_MODECHANGE 0x1F
#define ASKEY_SPACE 0x20
#define ASKEY_PRIOR 0x21
#define ASKEY_NEXT 0x22
#define ASKEY_END 0x23
#define ASKEY_HOME 0x24
#define ASKEY_LEFT 0x25
#define ASKEY_UP 0x26
#define ASKEY_RIGHT 0x27
#define ASKEY_DOWN 0x28
#define ASKEY_SELECT 0x29
#define ASKEY_PRINT 0x2A
#define ASKEY_EXECUT 0x2B
#define ASKEY_SNAPSHOT 0x2C
#define ASKEY_INSERT 0x2D
#define ASKEY_DELETE 0x2E
#define ASKEY_HELP 0x2F
#define ASKEY_0 0x30
#define ASKEY_1 0x31
#define ASKEY_2 0x32
#define ASKEY_3 0x33
#define ASKEY_4 0x34
#define ASKEY_5 0x35
#define ASKEY_6 0x36
#define ASKEY_7 0x37
#define ASKEY_8 0x38
#define ASKEY_9 0x39
#define ASKEY_A 0x41
#define ASKEY_B 0x42
#define ASKEY_C 0x43
#define ASKEY_D 0x44
#define ASKEY_E 0x45
#define ASKEY_F 0x46
#define ASKEY_G 0x47
#define ASKEY_H 0x48
#define ASKEY_I 0x49
#define ASKEY_J 0x4A
#define ASKEY_K 0x4B
#define ASKEY_L 0x4C
#define ASKEY_M 0x4D
#define ASKEY_N 0x4E
#define ASKEY_O 0x4F
#define ASKEY_P 0x50
#define ASKEY_Q 0x51
#define ASKEY_R 0x52
#define ASKEY_S 0x53
#define ASKEY_T 0x54
#define ASKEY_U 0x55
#define ASKEY_V 0x56
#define ASKEY_W 0x57
#define ASKEY_X 0x58
#define ASKEY_Y 0x59
#define ASKEY_Z 0x5A
#define ASKEY_LWIN 0x5B
#define ASKEY_RWIN 0x5C
#define ASKEY_APPS 0x5D
#define ASKEY_SLEEP 0x5F
#define ASKEY_NUMPAD0 0x60
#define ASKEY_NUMPAD1 0x61
#define ASKEY_NUMPAD2 0x62
#define ASKEY_NUMPAD3 0x63
#define ASKEY_NUMPAD4 0x64
#define ASKEY_NUMPAD5 0x65
#define ASKEY_NUMPAD6 0x66
#define ASKEY_NUMPAD7 0x67
#define ASKEY_NUMPAD8 0x68
#define ASKEY_NUMPAD9 0x69
#define ASKEY_MULTIPLY 0x6A
#define ASKEY_ADD 0x6B
#define ASKEY_SEPARATOR 0x6C
#define ASKEY_SUBTRACT 0x6D
#define ASKEY_DECIMAL 0x6E
#define ASKEY_DIVIDE 0x6F
#define ASKEY_F1 0x70
#define ASKEY_F2 0x71
#define ASKEY_F3 0x72
#define ASKEY_F4 0x73
#define ASKEY_F5 0x74
#define ASKEY_F6 0x75
#define ASKEY_F7 0x76
#define ASKEY_F8 0x77
#define ASKEY_F9 0x78
#define ASKEY_F10 0x79
#define ASKEY_F11 0x7A
#define ASKEY_F12 0x7B
#define ASKEY_F13 0x7C
#define ASKEY_F14 0x7D
#define ASKEY_F15 0x7E
#define ASKEY_F16 0x7F
#define ASKEY_F17 0x80
#define ASKEY_F18 0x81
#define ASKEY_F19 0x82
#define ASKEY_F20 0x83
#define ASKEY_F21 0x84
#define ASKEY_F22 0x85
#define ASKEY_F23 0x86
#define ASKEY_F24 0x87
#define ASKEY_NUMLOCK 0x90
#define ASKEY_SCROLL 0x91
#define ASKEY_LSHIFT 0xA0
#define ASKEY_RSHIFT 0xA1
#define ASKEY_LCONTROL 0xA2
#define ASKEY_RCONTROL 0xA3
#define ASKEY_LMENU 0xA4
#define ASKEY_RMENU 0xA5
#define ASKEY_COMMA 0xBC
#define ASKEY_PLUS 0xBB
#define ASKEY_MINUS 0xBD
#define ASKEY_PERIOD 0xBE
#define ASKEY_ATTN 0xF6
#define ASKEY_CRSEL 0xF7
#define ASKEY_EXSEL 0xF8
#define ASKEY_EREOF 0xF9
#define ASKEY_PLAY 0xFA
#define ASKEY_ZOOM 0xFB
#define ASKEY_PA1 0xFD
#define ASKEY_OEM_CLEAR 0xFE

// Texture flags
#define ASTEXTURE_MIPMAPS 1
#define ASTEXTURE_16BITS 2

// XML node types
#define ASXML_NULL 0
#define ASXML_NODE 1
#define ASXML_NODEEND 2
#define ASXML_TEXT 3
#define ASXML_CDATA 5
#define ASXML_UNKNOWN 6

#define TRUE 1
#define FALSE 0

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
