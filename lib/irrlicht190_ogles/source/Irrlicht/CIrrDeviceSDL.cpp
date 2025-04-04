// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"

#ifdef _IRR_COMPILE_WITH_SDL_DEVICE_

#include "CIrrDeviceSDL.h"
#include "IEventReceiver.h"
#include "IFileSystem.h"
#include "irrList.h"
#include "os.h"
#include "CTimer.h"
#include "irrString.h"
#include "Keycodes.h"
#include "COSOperator.h"
#include <stdio.h>
#include <stdlib.h>
#include "SIrrCreationParameters.h"
#include <SDL/SDL_syswm.h>
#include <SDL/SDL_video.h>

#ifdef _IRR_EMSCRIPTEN_PLATFORM_
#ifdef _IRR_COMPILE_WITH_OGLES2_
#include "CEGLManager.h"
#endif
#include <emscripten.h>
#endif

#ifdef _MSC_VER
#pragma comment(lib, "SDL.lib")
#endif // _MSC_VER

static int SDLDeviceInstances = 0;

namespace irr
{
	namespace video
	{
		#ifdef _IRR_COMPILE_WITH_DIRECT3D_9_
		IVideoDriver* createDirectX9Driver(const irr::SIrrlichtCreationParameters& params,
			io::IFileSystem* io, HWND window);
		#endif

		#ifdef _IRR_COMPILE_WITH_OPENGL_
		IVideoDriver* createOpenGLDriver(const SIrrlichtCreationParameters& params,
				io::IFileSystem* io, CIrrDeviceSDL* device);
		#endif

		#if defined(_IRR_COMPILE_WITH_OGLES2_) && defined(_IRR_EMSCRIPTEN_PLATFORM_)
		IVideoDriver* createOGLES2Driver(const irr::SIrrlichtCreationParameters& params, io::IFileSystem* io, IContextManager* contextManager);
		#endif

		#if defined(_IRR_COMPILE_WITH_WEBGL1_) && defined(_IRR_EMSCRIPTEN_PLATFORM_)
		IVideoDriver* createWebGL1Driver(const irr::SIrrlichtCreationParameters& params, io::IFileSystem* io, IContextManager* contextManager);
		#endif
	} // end namespace video

} // end namespace irr


namespace irr
{
#ifdef _IRR_EMSCRIPTEN_PLATFORM_
EM_BOOL CIrrDeviceSDL::MouseUpDownCallback(int eventType, const EmscriptenMouseEvent * event, void* userData)
{
	// We need this callback so far only because otherwise "emscripten_request_pointerlock" calls will
	// fail as their request are infinitely deferred.
	// Not exactly certain why, maybe SDL does catch those mouse-events otherwise and not pass them on.
	return EM_FALSE;
}

EM_BOOL CIrrDeviceSDL::MouseEnterCallback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
	CIrrDeviceSDL * This = static_cast<CIrrDeviceSDL*>(userData);

	SEvent irrevent;

	irrevent.EventType = irr::EET_MOUSE_INPUT_EVENT;
	irrevent.MouseInput.Event = irr::EMIE_MOUSE_ENTER_CANVAS;
	This->MouseX = irrevent.MouseInput.X = mouseEvent->canvasX;
	This->MouseY = irrevent.MouseInput.Y = mouseEvent->canvasY;
	This->MouseXRel = mouseEvent->movementX; // should be 0 I guess? Or can it enter while pointer is locked()?
	This->MouseYRel = mouseEvent->movementY;
	irrevent.MouseInput.ButtonStates = This->MouseButtonStates;	// TODO: not correct, but couldn't figure out the bitset of mouseEvent->buttons yet.
	irrevent.MouseInput.Shift = mouseEvent->shiftKey;
	irrevent.MouseInput.Control = mouseEvent->ctrlKey;

	This->postEventFromUser(irrevent);

	return EM_FALSE;
}

EM_BOOL CIrrDeviceSDL::MouseLeaveCallback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
	CIrrDeviceSDL * This = static_cast<CIrrDeviceSDL*>(userData);

	SEvent irrevent;

	irrevent.EventType = irr::EET_MOUSE_INPUT_EVENT;
	irrevent.MouseInput.Event = irr::EMIE_MOUSE_LEAVE_CANVAS;
	This->MouseX = irrevent.MouseInput.X = mouseEvent->canvasX;
	This->MouseY = irrevent.MouseInput.Y = mouseEvent->canvasY;
	This->MouseXRel = mouseEvent->movementX; // should be 0 I guess? Or can it enter while pointer is locked()?
	This->MouseYRel = mouseEvent->movementY;
	irrevent.MouseInput.ButtonStates = This->MouseButtonStates;	// TODO: not correct, but couldn't figure out the bitset of mouseEvent->buttons yet.
	irrevent.MouseInput.Shift = mouseEvent->shiftKey;
	irrevent.MouseInput.Control = mouseEvent->ctrlKey;

	This->postEventFromUser(irrevent);

	return EM_FALSE;
}

bool CIrrDeviceSDL::isNoUnicodeKey(EKEY_CODE key) const
{
	switch ( key )
	{
		// keys which  should not be mapped to a Unicode char
		case KEY_UNKNOWN:
		case KEY_SHIFT:
		case KEY_CONTROL:
		case KEY_MENU:
		case KEY_PAUSE:
		case KEY_CAPITAL:
		case KEY_ESCAPE:
		case KEY_PRIOR:
		case KEY_NEXT:
		case KEY_END:
		case KEY_HOME:
		case KEY_LEFT:
		case KEY_UP:
		case KEY_RIGHT:
		case KEY_DOWN:
		case KEY_PRINT:
		case KEY_SNAPSHOT:
		case KEY_INSERT:
		case KEY_DELETE:
		case KEY_HELP:
		case KEY_LWIN:
		case KEY_RWIN:
		case KEY_APPS:
		case KEY_SLEEP:
		case KEY_F1:
		case KEY_F2:
		case KEY_F3:
		case KEY_F4:
		case KEY_F5:
		case KEY_F6:
		case KEY_F7:
		case KEY_F8:
		case KEY_F9:
		case KEY_F10:
		case KEY_F11:
		case KEY_F12:
		case KEY_F13:
		case KEY_F14:
		case KEY_F15:
		case KEY_F16:
		case KEY_F17:
		case KEY_F18:
		case KEY_F19:
		case KEY_F20:
		case KEY_F21:
		case KEY_F22:
		case KEY_F23:
		case KEY_F24:
		case KEY_NUMLOCK:
		case KEY_SCROLL:
		case KEY_LSHIFT:
		case KEY_RSHIFT:
		case KEY_LCONTROL:
		case KEY_RCONTROL:
		case KEY_LMENU:
		case KEY_RMENU:
		return true;

	default:
		return false;
	}
}
#endif

//! constructor
CIrrDeviceSDL::CIrrDeviceSDL(const SIrrlichtCreationParameters& param)
	: CIrrDeviceStub(param),
	Screen((SDL_Surface*)param.WindowId), SDL_Flags(SDL_ANYFORMAT),
	MouseX(0), MouseY(0), MouseXRel(0), MouseYRel(0), MouseButtonStates(0),
	Width(param.WindowSize.Width), Height(param.WindowSize.Height),
	Resizable(param.WindowResizable == 1 ? true : false), WindowMinimized(false)
{
	#ifdef _DEBUG
	setDebugName("CIrrDeviceSDL");
	#endif

	if ( ++SDLDeviceInstances == 1 )
	{
		// Initialize SDL... Timer for sleep, video for the obvious, and
		// noparachute prevents SDL from catching fatal errors.
		if (SDL_Init( SDL_INIT_TIMER|SDL_INIT_VIDEO|
#if defined(_IRR_COMPILE_WITH_JOYSTICK_EVENTS_)
					SDL_INIT_JOYSTICK|
#endif
					SDL_INIT_NOPARACHUTE ) < 0)
		{
			os::Printer::log("Unable to initialize SDL!", SDL_GetError());
			Close = true;
		}
		else
		{
			os::Printer::log("SDL initialized", ELL_INFORMATION);
		}

#if defined(_IRR_WINDOWS_)
		SDL_putenv("SDL_VIDEODRIVER=directx");
#elif defined(_IRR_OSX_PLATFORM_)
		SDL_putenv("SDL_VIDEODRIVER=Quartz");
#elif !defined(_IRR_EMSCRIPTEN_PLATFORM_)
		SDL_putenv("SDL_VIDEODRIVER=x11");
#endif
	}

//	SDL_putenv("SDL_WINDOWID=");

	SDL_VERSION(&Info.version);

#ifndef _IRR_EMSCRIPTEN_PLATFORM_
	SDL_GetWMInfo(&Info);
#endif //_IRR_EMSCRIPTEN_PLATFORM_
	core::stringc sdlversion = "SDL Version ";
	sdlversion += Info.version.major;
	sdlversion += ".";
	sdlversion += Info.version.minor;
	sdlversion += ".";
	sdlversion += Info.version.patch;

	Operator = new COSOperator(sdlversion);
	if ( SDLDeviceInstances == 1 )
	{
		os::Printer::log(sdlversion.c_str(), ELL_INFORMATION);
	}

	// create keymap
	createKeyMap();
	// enable key to character translation
	SDL_EnableUNICODE(1);

	(void)SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	if ( CreationParams.Fullscreen )
		SDL_Flags |= SDL_FULLSCREEN;
	else if ( Resizable )
		SDL_Flags |= SDL_RESIZABLE;
	if (CreationParams.DriverType == video::EDT_OPENGL)
		SDL_Flags |= SDL_OPENGL;
	else if (CreationParams.Doublebuffer)
		SDL_Flags |= SDL_DOUBLEBUF;
#ifdef _IRR_EMSCRIPTEN_PLATFORM_
	SDL_Flags |= SDL_OPENGL;
#endif //_IRR_EMSCRIPTEN_PLATFORM_

	// create window
	if (CreationParams.DriverType != video::EDT_NULL)
	{
		// create the window, only if we do not use the null device
		createWindow();
	}

	// create cursor control
	CursorControl = new CCursorControl(this);

	#ifdef EMSCRIPTEN
	FileSystem->addFileArchive("data.bin", true, false, io::EFAT_ZIP);
	#endif

	// create driver
	createDriver();

	if (VideoDriver)
		createGUIAndScene();
}


//! destructor
CIrrDeviceSDL::~CIrrDeviceSDL()
{
	if ( --SDLDeviceInstances == 0 )
	{
#if defined(_IRR_COMPILE_WITH_JOYSTICK_EVENTS_)
		const u32 numJoysticks = Joysticks.size();
		for (u32 i=0; i<numJoysticks; ++i)
			SDL_JoystickClose(Joysticks[i]);
#endif
		SDL_Quit();

		os::Printer::log("Quit SDL", ELL_INFORMATION);
	}
}

void CIrrDeviceSDL::logAttributes()
{
	core::stringc sdl_attr("SDL attribs:");
	int value = 0;
	if ( SDL_GL_GetAttribute( SDL_GL_RED_SIZE, &value ) == 0 )
		sdl_attr += core::stringc(" r:") + core::stringc(value);
	if ( SDL_GL_GetAttribute( SDL_GL_GREEN_SIZE, &value ) == 0 )
		sdl_attr += core::stringc(" g:") + core::stringc(value);
	if ( SDL_GL_GetAttribute( SDL_GL_BLUE_SIZE, &value ) == 0 )
		sdl_attr += core::stringc(" b:") + core::stringc(value);
	if ( SDL_GL_GetAttribute( SDL_GL_ALPHA_SIZE, &value ) == 0 )
		sdl_attr += core::stringc(" a:") + core::stringc(value);

	if ( SDL_GL_GetAttribute( SDL_GL_DEPTH_SIZE, &value) == 0 )
		sdl_attr += core::stringc(" depth:") + core::stringc(value);
	if ( SDL_GL_GetAttribute( SDL_GL_STENCIL_SIZE, &value ) == 0 )
		sdl_attr += core::stringc(" stencil:") + core::stringc(value);
	if ( SDL_GL_GetAttribute( SDL_GL_DOUBLEBUFFER, &value ) == 0 )
		sdl_attr += core::stringc(" doublebuf:") + core::stringc(value);
	if ( SDL_GL_GetAttribute( SDL_GL_MULTISAMPLEBUFFERS, &value ) == 0 )
		sdl_attr += core::stringc(" aa:") + core::stringc(value);
	if ( SDL_GL_GetAttribute( SDL_GL_MULTISAMPLESAMPLES, &value ) == 0 )
		sdl_attr += core::stringc(" aa-samples:") + core::stringc(value);

	os::Printer::log(sdl_attr.c_str());
}

bool CIrrDeviceSDL::createWindow()
{
#ifdef _IRR_EMSCRIPTEN_PLATFORM_
	if ( Width != 0 || Height != 0 )
		emscripten_set_canvas_size( Width, Height);
	else
	{
		int w, h, fs;
		emscripten_get_canvas_size(&w, &h, &fs);
		Width = w;
		Height = h;
	}

	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, CreationParams.WithAlphaChannel?8:0 );

	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, CreationParams.ZBufferBits);
	SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, CreationParams.Stencilbuffer ? 8 : 0);
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, CreationParams.Doublebuffer ? 1 : 0);

	if (CreationParams.AntiAlias>1)
	{
		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, CreationParams.AntiAlias );
	}
	else
	{
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
	}

	Screen = SDL_SetVideoMode( 0, 0, 32, SDL_Flags); // 0,0 will use the canvas size

	logAttributes();

	// "#canvas" is for the opengl context
	emscripten_set_mousedown_callback("#canvas", (void*)this, true, MouseUpDownCallback);
    emscripten_set_mouseup_callback("#canvas", (void*)this, true, MouseUpDownCallback);
    emscripten_set_mouseenter_callback("#canvas", (void*)this, false, MouseEnterCallback);
    emscripten_set_mouseleave_callback("#canvas", (void*)this, false, MouseLeaveCallback);

	return true;
#else // !_IRR_EMSCRIPTEN_PLATFORM_
	if ( Close )
		return false;

	if (CreationParams.DriverType == video::EDT_OPENGL)
	{
		if (CreationParams.Bits==16)
		{
			SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 4 );
			SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 4 );
			SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 4 );
			SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, CreationParams.WithAlphaChannel?1:0 );
		}
		else
		{
			SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
			SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
			SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
			SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, CreationParams.WithAlphaChannel?8:0 );
		}
		SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, CreationParams.ZBufferBits);
		if (CreationParams.Doublebuffer)
			SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
		SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, CreationParams.Stencilbuffer ? 8 : 0);
		if (CreationParams.Stereobuffer)
			SDL_GL_SetAttribute( SDL_GL_STEREO, 1 );
		if (CreationParams.AntiAlias>1)
		{
			SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
			SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, CreationParams.AntiAlias );
		}
		if ( !Screen )
			Screen = SDL_SetVideoMode( Width, Height, CreationParams.Bits, SDL_Flags );
		if ( !Screen && CreationParams.AntiAlias>1)
		{
			while (--CreationParams.AntiAlias>1)
			{
				SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, CreationParams.AntiAlias );
				Screen = SDL_SetVideoMode( Width, Height, CreationParams.Bits, SDL_Flags );
				if (Screen)
					break;
			}
			if ( !Screen )
			{
				SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 0 );
				SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 0 );
				Screen = SDL_SetVideoMode( Width, Height, CreationParams.Bits, SDL_Flags );
				if (Screen)
					os::Printer::log("AntiAliasing disabled due to lack of support!" );
			}
		}
	}
	else if ( !Screen )
		Screen = SDL_SetVideoMode( Width, Height, CreationParams.Bits, SDL_Flags );

	if ( !Screen && CreationParams.Doublebuffer)
	{
		// Try single buffer
		if (CreationParams.DriverType == video::EDT_OPENGL)
			SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
		SDL_Flags &= ~SDL_DOUBLEBUF;
		Screen = SDL_SetVideoMode( Width, Height, CreationParams.Bits, SDL_Flags );
	}
	if ( !Screen )
	{
		os::Printer::log("Could not initialize display!" );
		return false;
	}

	return true;
#endif // !_IRR_EMSCRIPTEN_PLATFORM_
}


//! create the driver
void CIrrDeviceSDL::createDriver()
{
	switch(CreationParams.DriverType)
	{
	case video::DEPRECATED_EDT_DIRECT3D8_NO_LONGER_EXISTS:
		os::Printer::log("DIRECT3D8 Driver is no longer supported in Irrlicht. Try another one.", ELL_ERROR);
		break;

	case video::EDT_DIRECT3D9:
		#ifdef _IRR_COMPILE_WITH_DIRECT3D_9_
		os::Printer::log("SDL device does not support DIRECT3D9 driver. Try another one.", ELL_ERROR);
		#else
		os::Printer::log("DIRECT3D9 Driver was not compiled into this dll. Try another one.", ELL_ERROR);
		#endif // _IRR_COMPILE_WITH_DIRECT3D_9_

		break;

	case video::EDT_SOFTWARE:
		#ifdef _IRR_COMPILE_WITH_SOFTWARE_
		VideoDriver = video::createSoftwareDriver(CreationParams.WindowSize, CreationParams.Fullscreen, FileSystem, this);
		#else
		os::Printer::log("No Software driver support compiled in.", ELL_ERROR);
		#endif
		break;

	case video::EDT_BURNINGSVIDEO:
		#ifdef _IRR_COMPILE_WITH_BURNINGSVIDEO_
		VideoDriver = video::createBurningVideoDriver(CreationParams, FileSystem, this);
		#else
		os::Printer::log("Burning's video driver was not compiled in.", ELL_ERROR);
		#endif
		break;

	case video::EDT_OPENGL:
		#ifdef _IRR_COMPILE_WITH_OPENGL_
		VideoDriver = video::createOpenGLDriver(CreationParams, FileSystem, this);
		#else
		os::Printer::log("No OpenGL support compiled in.", ELL_ERROR);
		#endif
		break;

	case video::EDT_OGLES2:
#if defined(_IRR_COMPILE_WITH_OGLES2_) && defined(_IRR_EMSCRIPTEN_PLATFORM_)
		{
			video::SExposedVideoData data;

			ContextManager = new video::CEGLManager();
			ContextManager->initialize(CreationParams, data);

			VideoDriver = video::createOGLES2Driver(CreationParams, FileSystem, ContextManager);
		}
#else
		os::Printer::log("No OpenGL-ES2 support compiled in.", ELL_ERROR);
#endif
		break;

	case video::EDT_WEBGL1:
#if defined(_IRR_COMPILE_WITH_WEBGL1_) && defined(_IRR_EMSCRIPTEN_PLATFORM_)
		{
			video::SExposedVideoData data;

			ContextManager = new video::CEGLManager();
			ContextManager->initialize(CreationParams, data);

			VideoDriver = video::createWebGL1Driver(CreationParams, FileSystem, ContextManager);
		}
#else
		os::Printer::log("No WebGL1 support compiled in.", ELL_ERROR);
#endif
		break;

	case video::EDT_NULL:
		VideoDriver = video::createNullDriver(FileSystem, CreationParams.WindowSize);
		break;

	default:
		os::Printer::log("Unable to create video driver of unknown type.", ELL_ERROR);
		break;
	}

	// In case we got the size from the canvas
	if ( VideoDriver && CreationParams.WindowSize.Width == 0 && CreationParams.WindowSize.Height == 0 && Width > 0 && Height > 0 )
	{
#ifdef _IRR_EMSCRIPTEN_PLATFORM_
		Screen = SDL_SetVideoMode( Width, Height, 32, SDL_Flags );
#else //_IRR_EMSCRIPTEN_PLATFORM_
		Screen = SDL_SetVideoMode( Width, Height, 0, SDL_Flags );
#endif //_IRR_EMSCRIPTEN_PLATFOR
		VideoDriver->OnResize(core::dimension2d<u32>(Width, Height));
	}
}


//! runs the device. Returns false if device wants to be deleted
bool CIrrDeviceSDL::run()
{
	os::Timer::tick();

	SEvent irrevent;
	SDL_Event SDL_event;

	while ( !Close && SDL_PollEvent( &SDL_event ) )
	{
		// os::Printer::log("event: ", core::stringc((int)SDL_event.type).c_str(),   ELL_INFORMATION);	// just for debugging

		switch ( SDL_event.type )
		{
		case SDL_MOUSEMOTION:
			irrevent.EventType = irr::EET_MOUSE_INPUT_EVENT;
			irrevent.MouseInput.Event = irr::EMIE_MOUSE_MOVED;
			MouseX = irrevent.MouseInput.X = SDL_event.motion.x;
			MouseY = irrevent.MouseInput.Y = SDL_event.motion.y;
			MouseXRel = SDL_event.motion.xrel;
			MouseYRel = SDL_event.motion.yrel;
			irrevent.MouseInput.ButtonStates = MouseButtonStates;

			postEventFromUser(irrevent);
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:

			irrevent.EventType = irr::EET_MOUSE_INPUT_EVENT;
			irrevent.MouseInput.X = SDL_event.button.x;
			irrevent.MouseInput.Y = SDL_event.button.y;

			irrevent.MouseInput.Event = irr::EMIE_MOUSE_MOVED;


#ifdef _IRR_EMSCRIPTEN_PLATFORM_
			// Handle mouselocking in emscripten in Windowed mode.
			// In fullscreen SDL will handle it.
			// The behavior we want windowed is - when the canvas was clicked then
			// we will lock the mouse-pointer if it should be invisible.
			// For security reasons this will be delayed until the next mouse-up event.
			// We do not pass on this event as we don't want the activation click to do anything.
			if ( SDL_event.type == SDL_MOUSEBUTTONDOWN && !isFullscreen() )
			{
				EmscriptenPointerlockChangeEvent pointerlockStatus; // let's hope that test is not expensive ...
				if ( emscripten_get_pointerlock_status(&pointerlockStatus) == EMSCRIPTEN_RESULT_SUCCESS )
				{
					if ( CursorControl->isVisible() && pointerlockStatus.isActive )
					{
						emscripten_exit_pointerlock();
						return !Close;
					}
					else if ( !CursorControl->isVisible() && !pointerlockStatus.isActive )
					{
						emscripten_request_pointerlock(0, true);
						return !Close;
					}
				}
			}
#endif

			switch(SDL_event.button.button)
			{
			case SDL_BUTTON_LEFT:
				if (SDL_event.type == SDL_MOUSEBUTTONDOWN)
				{
					irrevent.MouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN;
					MouseButtonStates |= irr::EMBSM_LEFT;
				}
				else
				{
					irrevent.MouseInput.Event = irr::EMIE_LMOUSE_LEFT_UP;
					MouseButtonStates &= !irr::EMBSM_LEFT;
				}
				break;

			case SDL_BUTTON_RIGHT:
				if (SDL_event.type == SDL_MOUSEBUTTONDOWN)
				{
					irrevent.MouseInput.Event = irr::EMIE_RMOUSE_PRESSED_DOWN;
					MouseButtonStates |= irr::EMBSM_RIGHT;
				}
				else
				{
					irrevent.MouseInput.Event = irr::EMIE_RMOUSE_LEFT_UP;
					MouseButtonStates &= ~irr::EMBSM_RIGHT;
				}
				break;

			case SDL_BUTTON_MIDDLE:
				if (SDL_event.type == SDL_MOUSEBUTTONDOWN)
				{
					irrevent.MouseInput.Event = irr::EMIE_MMOUSE_PRESSED_DOWN;
					MouseButtonStates |= irr::EMBSM_MIDDLE;
				}
				else
				{
					irrevent.MouseInput.Event = irr::EMIE_MMOUSE_LEFT_UP;
					MouseButtonStates &= ~irr::EMBSM_MIDDLE;
				}
				break;

			case SDL_BUTTON_WHEELUP:
				irrevent.MouseInput.Event = irr::EMIE_MOUSE_WHEEL;
				irrevent.MouseInput.Wheel = 1.0f;
				break;

			case SDL_BUTTON_WHEELDOWN:
				irrevent.MouseInput.Event = irr::EMIE_MOUSE_WHEEL;
				irrevent.MouseInput.Wheel = -1.0f;
				break;
			}

			irrevent.MouseInput.ButtonStates = MouseButtonStates;

			if (irrevent.MouseInput.Event != irr::EMIE_MOUSE_MOVED)
			{
				postEventFromUser(irrevent);

				if ( irrevent.MouseInput.Event >= EMIE_LMOUSE_PRESSED_DOWN && irrevent.MouseInput.Event <= EMIE_MMOUSE_PRESSED_DOWN )
				{
					u32 clicks = checkSuccessiveClicks(irrevent.MouseInput.X, irrevent.MouseInput.Y, irrevent.MouseInput.Event);
					if ( clicks == 2 )
					{
						irrevent.MouseInput.Event = (EMOUSE_INPUT_EVENT)(EMIE_LMOUSE_DOUBLE_CLICK + irrevent.MouseInput.Event-EMIE_LMOUSE_PRESSED_DOWN);
						postEventFromUser(irrevent);
					}
					else if ( clicks == 3 )
					{
						irrevent.MouseInput.Event = (EMOUSE_INPUT_EVENT)(EMIE_LMOUSE_TRIPLE_CLICK + irrevent.MouseInput.Event-EMIE_LMOUSE_PRESSED_DOWN);
						postEventFromUser(irrevent);
					}
				}
			}
			break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			{
				SKeyMap mp;
				mp.SDLKey = SDL_event.key.keysym.sym;
				s32 idx = KeyMap.binary_search(mp);

				EKEY_CODE key;
				if (idx == -1)
					key = (EKEY_CODE)0;
				else
					key = (EKEY_CODE)KeyMap[idx].Win32Key;

#ifdef _IRR_WINDOWS_API_
				// handle alt+f4 in Windows, because SDL seems not to
				if ( (SDL_event.key.keysym.mod & KMOD_LALT) && key == KEY_F4)
				{
					Close = true;
					break;
				}
#endif
				irrevent.EventType = irr::EET_KEY_INPUT_EVENT;
				irrevent.KeyInput.Char = SDL_event.key.keysym.unicode;
				irrevent.KeyInput.Key = key;
#ifdef _IRR_EMSCRIPTEN_PLATFORM_
				// On emscripten SDL does not (yet?) return 0 for invalid keysym.unicode's.
				// Instead it sets keysym.unicode to keysym.sym.
				// But we need to distinguish control keys from characters here as that info
				// is necessary in other places like the editbox.
				if ( isNoUnicodeKey(key) )
					irrevent.KeyInput.Char = 0;
#endif
				irrevent.KeyInput.PressedDown = (SDL_event.type == SDL_KEYDOWN);
				irrevent.KeyInput.Shift = (SDL_event.key.keysym.mod & KMOD_SHIFT) != 0;
				irrevent.KeyInput.Control = (SDL_event.key.keysym.mod & KMOD_CTRL ) != 0;
				postEventFromUser(irrevent);
			}
			break;

		case SDL_QUIT:
			Close = true;
			break;

		case SDL_ACTIVEEVENT:
			if (SDL_event.active.state == SDL_APPACTIVE)
				WindowMinimized = (SDL_event.active.gain!=1);
			break;

		case SDL_VIDEORESIZE:
			if ((SDL_event.resize.w != (int)Width) || (SDL_event.resize.h != (int)Height))
			{
				Width = SDL_event.resize.w;
				Height = SDL_event.resize.h;
#ifdef _IRR_EMSCRIPTEN_PLATFORM_
				Screen = SDL_SetVideoMode( 0, 0, 32, SDL_Flags ); // 0,0 will use the canvas size
#else //_IRR_EMSCRIPTEN_PLATFORM_
 				Screen = SDL_SetVideoMode( Width, Height, 0, SDL_Flags );
#endif //_IRR_EMSCRIPTEN_PLATFOR
				if (VideoDriver)
					VideoDriver->OnResize(core::dimension2d<u32>(Width, Height));
			}
			break;

		case SDL_USEREVENT:
			irrevent.EventType = irr::EET_USER_EVENT;
			irrevent.UserEvent.UserData1 = reinterpret_cast<uintptr_t>(SDL_event.user.data1);
			irrevent.UserEvent.UserData2 = reinterpret_cast<uintptr_t>(SDL_event.user.data2);

			postEventFromUser(irrevent);
			break;

		default:
			break;
		} // end switch

	} // end while

#if defined(_IRR_COMPILE_WITH_JOYSTICK_EVENTS_)
	// TODO: Check if the multiple open/close calls are too expensive, then
	// open/close in the constructor/destructor instead

	// update joystick states manually
	SDL_JoystickUpdate();
	// we'll always send joystick input events...
	SEvent joyevent;
	joyevent.EventType = EET_JOYSTICK_INPUT_EVENT;
	for (u32 i=0; i<Joysticks.size(); ++i)
	{
		SDL_Joystick* joystick = Joysticks[i];
		if (joystick)
		{
			int j;
			// query all buttons
			const int numButtons = core::min_(SDL_JoystickNumButtons(joystick), 32);
			joyevent.JoystickEvent.ButtonStates=0;
			for (j=0; j<numButtons; ++j)
				joyevent.JoystickEvent.ButtonStates |= (SDL_JoystickGetButton(joystick, j)<<j);

			// query all axes, already in correct range
			const int numAxes = core::min_(SDL_JoystickNumAxes(joystick), (int)SEvent::SJoystickEvent::NUMBER_OF_AXES);
			joyevent.JoystickEvent.Axis[SEvent::SJoystickEvent::AXIS_X]=0;
			joyevent.JoystickEvent.Axis[SEvent::SJoystickEvent::AXIS_Y]=0;
			joyevent.JoystickEvent.Axis[SEvent::SJoystickEvent::AXIS_Z]=0;
			joyevent.JoystickEvent.Axis[SEvent::SJoystickEvent::AXIS_R]=0;
			joyevent.JoystickEvent.Axis[SEvent::SJoystickEvent::AXIS_U]=0;
			joyevent.JoystickEvent.Axis[SEvent::SJoystickEvent::AXIS_V]=0;
			for (j=0; j<numAxes; ++j)
				joyevent.JoystickEvent.Axis[j] = SDL_JoystickGetAxis(joystick, j);

			// we can only query one hat, SDL only supports 8 directions
			if (SDL_JoystickNumHats(joystick)>0)
			{
				switch (SDL_JoystickGetHat(joystick, 0))
				{
					case SDL_HAT_UP:
						joyevent.JoystickEvent.POV=0;
						break;
					case SDL_HAT_RIGHTUP:
						joyevent.JoystickEvent.POV=4500;
						break;
					case SDL_HAT_RIGHT:
						joyevent.JoystickEvent.POV=9000;
						break;
					case SDL_HAT_RIGHTDOWN:
						joyevent.JoystickEvent.POV=13500;
						break;
					case SDL_HAT_DOWN:
						joyevent.JoystickEvent.POV=18000;
						break;
					case SDL_HAT_LEFTDOWN:
						joyevent.JoystickEvent.POV=22500;
						break;
					case SDL_HAT_LEFT:
						joyevent.JoystickEvent.POV=27000;
						break;
					case SDL_HAT_LEFTUP:
						joyevent.JoystickEvent.POV=31500;
						break;
					case SDL_HAT_CENTERED:
					default:
						joyevent.JoystickEvent.POV=65535;
						break;
				}
			}
			else
			{
				joyevent.JoystickEvent.POV=65535;
			}

			// we map the number directly
			joyevent.JoystickEvent.Joystick=static_cast<u8>(i);
			// now post the event
			postEventFromUser(joyevent);
			// and close the joystick
		}
	}
#endif
	return !Close;
}

//! Activate any joysticks, and generate events for them.
bool CIrrDeviceSDL::activateJoysticks(core::array<SJoystickInfo> & joystickInfo)
{
#if defined(_IRR_COMPILE_WITH_JOYSTICK_EVENTS_)
	joystickInfo.clear();

	// we can name up to 256 different joysticks
	const int numJoysticks = core::min_(SDL_NumJoysticks(), 256);
	Joysticks.reallocate(numJoysticks);
	joystickInfo.reallocate(numJoysticks);

	int joystick = 0;
	for (; joystick<numJoysticks; ++joystick)
	{
		Joysticks.push_back(SDL_JoystickOpen(joystick));
		SJoystickInfo info;

		info.Joystick = joystick;
		info.Axes = SDL_JoystickNumAxes(Joysticks[joystick]);
		info.Buttons = SDL_JoystickNumButtons(Joysticks[joystick]);
		info.Name = SDL_JoystickName(joystick);
		info.PovHat = (SDL_JoystickNumHats(Joysticks[joystick]) > 0)
						? SJoystickInfo::POV_HAT_PRESENT : SJoystickInfo::POV_HAT_ABSENT;

		joystickInfo.push_back(info);
	}

	for(joystick = 0; joystick < (int)joystickInfo.size(); ++joystick)
	{
		char logString[256];
		(void)sprintf(logString, "Found joystick %d, %d axes, %d buttons '%s'",
		joystick, joystickInfo[joystick].Axes,
		joystickInfo[joystick].Buttons, joystickInfo[joystick].Name.c_str());
		os::Printer::log(logString, ELL_INFORMATION);
	}

	return true;

#endif // _IRR_COMPILE_WITH_JOYSTICK_EVENTS_

	return false;
}



//! pause execution temporarily
void CIrrDeviceSDL::yield()
{
	SDL_Delay(0);
}


//! pause execution for a specified time
void CIrrDeviceSDL::sleep(u32 timeMs, bool pauseTimer)
{
	const bool wasStopped = Timer ? Timer->isStopped() : true;
	if (pauseTimer && !wasStopped)
		Timer->stop();

	SDL_Delay(timeMs);

	if (pauseTimer && !wasStopped)
		Timer->start();
}


//! sets the caption of the window
void CIrrDeviceSDL::setWindowCaption(const wchar_t* text)
{
	core::stringc textc = text;
	SDL_WM_SetCaption( textc.c_str( ), textc.c_str( ) );
}


//! presents a surface in the client area
bool CIrrDeviceSDL::present(video::IImage* surface, void* windowId, core::rect<s32>* srcClip)
{
#ifdef _IRR_EMSCRIPTEN_PLATFORM_
	return true;
#else // !_IRR_EMSCRIPTEN_PLATFORM_
	SDL_Surface *sdlSurface = SDL_CreateRGBSurfaceFrom(
			surface->getData(), surface->getDimension().Width, surface->getDimension().Height,
			surface->getBitsPerPixel(), surface->getPitch(),
			surface->getRedMask(), surface->getGreenMask(), surface->getBlueMask(), surface->getAlphaMask());
	if (!sdlSurface)
		return false;
	SDL_SetAlpha(sdlSurface, 0, 0);
	SDL_SetColorKey(sdlSurface, 0, 0);
	sdlSurface->format->BitsPerPixel=surface->getBitsPerPixel();
	sdlSurface->format->BytesPerPixel=surface->getBytesPerPixel();
	if ((surface->getColorFormat()==video::ECF_R8G8B8) ||
			(surface->getColorFormat()==video::ECF_A8R8G8B8))
	{
		sdlSurface->format->Rloss=0;
		sdlSurface->format->Gloss=0;
		sdlSurface->format->Bloss=0;
		sdlSurface->format->Rshift=16;
		sdlSurface->format->Gshift=8;
		sdlSurface->format->Bshift=0;
		if (surface->getColorFormat()==video::ECF_R8G8B8)
		{
			sdlSurface->format->Aloss=8;
			sdlSurface->format->Ashift=32;
		}
		else
		{
			sdlSurface->format->Aloss=0;
			sdlSurface->format->Ashift=24;
		}
	}
	else if (surface->getColorFormat()==video::ECF_R5G6B5)
	{
		sdlSurface->format->Rloss=3;
		sdlSurface->format->Gloss=2;
		sdlSurface->format->Bloss=3;
		sdlSurface->format->Aloss=8;
		sdlSurface->format->Rshift=11;
		sdlSurface->format->Gshift=5;
		sdlSurface->format->Bshift=0;
		sdlSurface->format->Ashift=16;
	}
	else if (surface->getColorFormat()==video::ECF_A1R5G5B5)
	{
		sdlSurface->format->Rloss=3;
		sdlSurface->format->Gloss=3;
		sdlSurface->format->Bloss=3;
		sdlSurface->format->Aloss=7;
		sdlSurface->format->Rshift=10;
		sdlSurface->format->Gshift=5;
		sdlSurface->format->Bshift=0;
		sdlSurface->format->Ashift=15;
	}

	SDL_Surface* scr = (SDL_Surface* )windowId;
	if (!scr)
		scr = Screen;
	if (scr)
	{
		if (srcClip)
		{
			SDL_Rect sdlsrcClip;
			sdlsrcClip.x = srcClip->UpperLeftCorner.X;
			sdlsrcClip.y = srcClip->UpperLeftCorner.Y;
			sdlsrcClip.w = srcClip->getWidth();
			sdlsrcClip.h = srcClip->getHeight();
			SDL_BlitSurface(sdlSurface, &sdlsrcClip, scr, NULL);
		}
		else
			SDL_BlitSurface(sdlSurface, NULL, scr, NULL);
		SDL_Flip(scr);
	}

	SDL_FreeSurface(sdlSurface);
	return (scr != 0);
#endif // !_IRR_EMSCRIPTEN_PLATFORM_
}


//! notifies the device that it should close itself
void CIrrDeviceSDL::closeDevice()
{
	Close = true;
}


//! \return Pointer to a list with all video modes supported
video::IVideoModeList* CIrrDeviceSDL::getVideoModeList()
{
#ifdef _IRR_EMSCRIPTEN_PLATFORM_
	os::Printer::log("VideoModeList not available on the web." , ELL_WARNING);
	return VideoModeList;
#else // !_IRR_EMSCRIPTEN_PLATFORM_
	if (!VideoModeList->getVideoModeCount())
	{
		// enumerate video modes.
		const SDL_VideoInfo *vi = SDL_GetVideoInfo();

		SDL_PixelFormat pixelFormat = *(vi->vfmt);

		core::array<Uint8> checkBitsPerPixel;
		checkBitsPerPixel.push_back(8);
		checkBitsPerPixel.push_back(16);
		checkBitsPerPixel.push_back(24);
		checkBitsPerPixel.push_back(32);
		if ( pixelFormat.BitsPerPixel > 32 )
			checkBitsPerPixel.push_back(pixelFormat.BitsPerPixel);

		for ( u32 i=0; i<checkBitsPerPixel.size(); ++i)
		{
			pixelFormat.BitsPerPixel = checkBitsPerPixel[i];
			SDL_Rect **modes = SDL_ListModes(&pixelFormat, SDL_Flags|SDL_FULLSCREEN);
			if (modes != 0)
			{
				if (modes == (SDL_Rect **)-1)
				{
					core::stringc strLog("All modes available for bit-depth ");
					strLog += core::stringc(pixelFormat.BitsPerPixel);
					os::Printer::log(strLog.c_str());
				}
				else
				{
					for (u32 i=0; modes[i]; ++i)
						VideoModeList->addMode(core::dimension2d<u32>(modes[i]->w, modes[i]->h), vi->vfmt->BitsPerPixel);
				}
			}
		}
	}

	return VideoModeList;
#endif // !_IRR_EMSCRIPTEN_PLATFORM_
}

//! Sets if the window should be resizable in windowed mode.
void CIrrDeviceSDL::setResizable(bool resize)
{
#ifdef _IRR_EMSCRIPTEN_PLATFORM_
	os::Printer::log("Resizable not available on the web." , ELL_WARNING);
	return;
#else // !_IRR_EMSCRIPTEN_PLATFORM_
	if (resize != Resizable)
	{
#if defined(_IRR_COMPILE_WITH_OPENGL_) && defined(_IRR_WINDOWS_)
		if ( SDL_Flags & SDL_OPENGL )
		{
			// For unknown reasons the hack with sharing resources which was added in Irrlicht 1.8.5 for this no longer works in 1.9
			// But at least we got a new WindowResizable flag since Irrlicht 1.9.
			os::Printer::log("setResizable not supported with this device/driver combination. Use SIrrCreationParameters.WindowResizable instead.", ELL_WARNING);
			return;
		}
#endif

		if (resize)
			SDL_Flags |= SDL_RESIZABLE;
		else
			SDL_Flags &= ~SDL_RESIZABLE;

		Screen = SDL_SetVideoMode( 0, 0, 0, SDL_Flags );
		Resizable = resize;
	}
#endif // !_IRR_EMSCRIPTEN_PLATFORM_
}


//! Minimizes window if possible
void CIrrDeviceSDL::minimizeWindow()
{
	SDL_WM_IconifyWindow();
}


//! Maximize window
void CIrrDeviceSDL::maximizeWindow()
{
	// do nothing
}

//! Get the position of this window on screen
core::position2di CIrrDeviceSDL::getWindowPosition()
{
    return core::position2di(-1, -1);
}


//! Restore original window size
void CIrrDeviceSDL::restoreWindow()
{
	// do nothing
}

bool CIrrDeviceSDL::isFullscreen() const
{
#ifdef _IRR_EMSCRIPTEN_PLATFORM_
	return SDL_GetWindowFlags(0) == SDL_WINDOW_FULLSCREEN;
#else

	return CIrrDeviceStub::isFullscreen();
#endif
}


//! returns if window is active. if not, nothing need to be drawn
bool CIrrDeviceSDL::isWindowActive() const
{
#ifdef _IRR_EMSCRIPTEN_PLATFORM_
	// Hidden test only does something in some browsers (when tab in background or window is minimized)
	// In other browsers code automatically doesn't seem to be called anymore.
	EmscriptenVisibilityChangeEvent emVisibility;
	if ( emscripten_get_visibility_status(&emVisibility) == EMSCRIPTEN_RESULT_SUCCESS)
	{
		if ( emVisibility.hidden )
			return false;
	}
#endif
	const Uint8 appState = SDL_GetAppState();
	return (appState&SDL_APPACTIVE && appState&SDL_APPINPUTFOCUS) ? true : false;
}


//! returns if window has focus.
bool CIrrDeviceSDL::isWindowFocused() const
{
	return (SDL_GetAppState()&SDL_APPINPUTFOCUS) ? true : false;
}


//! returns if window is minimized.
bool CIrrDeviceSDL::isWindowMinimized() const
{
	return WindowMinimized;
}


//! Set the current Gamma Value for the Display
bool CIrrDeviceSDL::setGammaRamp( f32 red, f32 green, f32 blue, f32 brightness, f32 contrast )
{
	/*
	// todo: Gamma in SDL takes ints, what does Irrlicht use?
	return (SDL_SetGamma(red, green, blue) != -1);
	*/
	return false;
}

//! Get the current Gamma Value for the Display
bool CIrrDeviceSDL::getGammaRamp( f32 &red, f32 &green, f32 &blue, f32 &brightness, f32 &contrast )
{
/*	brightness = 0.f;
	contrast = 0.f;
	return (SDL_GetGamma(&red, &green, &blue) != -1);*/
	return false;
}

//! returns color format of the window.
video::ECOLOR_FORMAT CIrrDeviceSDL::getColorFormat() const
{
	if (Screen)
	{
		if (Screen->format->BitsPerPixel==16)
		{
			if (Screen->format->Amask != 0)
				return video::ECF_A1R5G5B5;
			else
				return video::ECF_R5G6B5;
		}
		else
		{
			if (Screen->format->Amask != 0)
				return video::ECF_A8R8G8B8;
			else
				return video::ECF_R8G8B8;
		}
	}
	else
		return CIrrDeviceStub::getColorFormat();
}


void CIrrDeviceSDL::createKeyMap()
{
	// I don't know if this is the best method  to create
	// the lookuptable, but I'll leave it like that until
	// I find a better version.

	KeyMap.reallocate(105);

	// buttons missing

	KeyMap.push_back(SKeyMap(SDLK_BACKSPACE, KEY_BACK));
	KeyMap.push_back(SKeyMap(SDLK_TAB, KEY_TAB));
	KeyMap.push_back(SKeyMap(SDLK_CLEAR, KEY_CLEAR));
	KeyMap.push_back(SKeyMap(SDLK_RETURN, KEY_RETURN));

	// combined modifiers missing

	KeyMap.push_back(SKeyMap(SDLK_PAUSE, KEY_PAUSE));
	KeyMap.push_back(SKeyMap(SDLK_CAPSLOCK, KEY_CAPITAL));

	// asian letter keys missing

	KeyMap.push_back(SKeyMap(SDLK_ESCAPE, KEY_ESCAPE));

	// asian letter keys missing

	KeyMap.push_back(SKeyMap(SDLK_SPACE, KEY_SPACE));
	KeyMap.push_back(SKeyMap(SDLK_PAGEUP, KEY_PRIOR));
	KeyMap.push_back(SKeyMap(SDLK_PAGEDOWN, KEY_NEXT));
	KeyMap.push_back(SKeyMap(SDLK_END, KEY_END));
	KeyMap.push_back(SKeyMap(SDLK_HOME, KEY_HOME));
	KeyMap.push_back(SKeyMap(SDLK_LEFT, KEY_LEFT));
	KeyMap.push_back(SKeyMap(SDLK_UP, KEY_UP));
	KeyMap.push_back(SKeyMap(SDLK_RIGHT, KEY_RIGHT));
	KeyMap.push_back(SKeyMap(SDLK_DOWN, KEY_DOWN));

	// select missing
	KeyMap.push_back(SKeyMap(SDLK_PRINT, KEY_PRINT));
	// execute missing
	KeyMap.push_back(SKeyMap(SDLK_PRINT, KEY_SNAPSHOT));

	KeyMap.push_back(SKeyMap(SDLK_INSERT, KEY_INSERT));
	KeyMap.push_back(SKeyMap(SDLK_DELETE, KEY_DELETE));
	KeyMap.push_back(SKeyMap(SDLK_HELP, KEY_HELP));

	KeyMap.push_back(SKeyMap(SDLK_0, KEY_KEY_0));
	KeyMap.push_back(SKeyMap(SDLK_1, KEY_KEY_1));
	KeyMap.push_back(SKeyMap(SDLK_2, KEY_KEY_2));
	KeyMap.push_back(SKeyMap(SDLK_3, KEY_KEY_3));
	KeyMap.push_back(SKeyMap(SDLK_4, KEY_KEY_4));
	KeyMap.push_back(SKeyMap(SDLK_5, KEY_KEY_5));
	KeyMap.push_back(SKeyMap(SDLK_6, KEY_KEY_6));
	KeyMap.push_back(SKeyMap(SDLK_7, KEY_KEY_7));
	KeyMap.push_back(SKeyMap(SDLK_8, KEY_KEY_8));
	KeyMap.push_back(SKeyMap(SDLK_9, KEY_KEY_9));

	KeyMap.push_back(SKeyMap(SDLK_a, KEY_KEY_A));
	KeyMap.push_back(SKeyMap(SDLK_b, KEY_KEY_B));
	KeyMap.push_back(SKeyMap(SDLK_c, KEY_KEY_C));
	KeyMap.push_back(SKeyMap(SDLK_d, KEY_KEY_D));
	KeyMap.push_back(SKeyMap(SDLK_e, KEY_KEY_E));
	KeyMap.push_back(SKeyMap(SDLK_f, KEY_KEY_F));
	KeyMap.push_back(SKeyMap(SDLK_g, KEY_KEY_G));
	KeyMap.push_back(SKeyMap(SDLK_h, KEY_KEY_H));
	KeyMap.push_back(SKeyMap(SDLK_i, KEY_KEY_I));
	KeyMap.push_back(SKeyMap(SDLK_j, KEY_KEY_J));
	KeyMap.push_back(SKeyMap(SDLK_k, KEY_KEY_K));
	KeyMap.push_back(SKeyMap(SDLK_l, KEY_KEY_L));
	KeyMap.push_back(SKeyMap(SDLK_m, KEY_KEY_M));
	KeyMap.push_back(SKeyMap(SDLK_n, KEY_KEY_N));
	KeyMap.push_back(SKeyMap(SDLK_o, KEY_KEY_O));
	KeyMap.push_back(SKeyMap(SDLK_p, KEY_KEY_P));
	KeyMap.push_back(SKeyMap(SDLK_q, KEY_KEY_Q));
	KeyMap.push_back(SKeyMap(SDLK_r, KEY_KEY_R));
	KeyMap.push_back(SKeyMap(SDLK_s, KEY_KEY_S));
	KeyMap.push_back(SKeyMap(SDLK_t, KEY_KEY_T));
	KeyMap.push_back(SKeyMap(SDLK_u, KEY_KEY_U));
	KeyMap.push_back(SKeyMap(SDLK_v, KEY_KEY_V));
	KeyMap.push_back(SKeyMap(SDLK_w, KEY_KEY_W));
	KeyMap.push_back(SKeyMap(SDLK_x, KEY_KEY_X));
	KeyMap.push_back(SKeyMap(SDLK_y, KEY_KEY_Y));
	KeyMap.push_back(SKeyMap(SDLK_z, KEY_KEY_Z));

	KeyMap.push_back(SKeyMap(SDLK_LSUPER, KEY_LWIN));
	KeyMap.push_back(SKeyMap(SDLK_RSUPER, KEY_RWIN));
	// apps missing
	KeyMap.push_back(SKeyMap(SDLK_POWER, KEY_SLEEP)); //??

	KeyMap.push_back(SKeyMap(SDLK_KP0, KEY_NUMPAD0));
	KeyMap.push_back(SKeyMap(SDLK_KP1, KEY_NUMPAD1));
	KeyMap.push_back(SKeyMap(SDLK_KP2, KEY_NUMPAD2));
	KeyMap.push_back(SKeyMap(SDLK_KP3, KEY_NUMPAD3));
	KeyMap.push_back(SKeyMap(SDLK_KP4, KEY_NUMPAD4));
	KeyMap.push_back(SKeyMap(SDLK_KP5, KEY_NUMPAD5));
	KeyMap.push_back(SKeyMap(SDLK_KP6, KEY_NUMPAD6));
	KeyMap.push_back(SKeyMap(SDLK_KP7, KEY_NUMPAD7));
	KeyMap.push_back(SKeyMap(SDLK_KP8, KEY_NUMPAD8));
	KeyMap.push_back(SKeyMap(SDLK_KP9, KEY_NUMPAD9));
	KeyMap.push_back(SKeyMap(SDLK_KP_MULTIPLY, KEY_MULTIPLY));
	KeyMap.push_back(SKeyMap(SDLK_KP_PLUS, KEY_ADD));
//	KeyMap.push_back(SKeyMap(SDLK_KP_, KEY_SEPARATOR));
	KeyMap.push_back(SKeyMap(SDLK_KP_MINUS, KEY_SUBTRACT));
	KeyMap.push_back(SKeyMap(SDLK_KP_PERIOD, KEY_DECIMAL));
	KeyMap.push_back(SKeyMap(SDLK_KP_DIVIDE, KEY_DIVIDE));

	KeyMap.push_back(SKeyMap(SDLK_F1,  KEY_F1));
	KeyMap.push_back(SKeyMap(SDLK_F2,  KEY_F2));
	KeyMap.push_back(SKeyMap(SDLK_F3,  KEY_F3));
	KeyMap.push_back(SKeyMap(SDLK_F4,  KEY_F4));
	KeyMap.push_back(SKeyMap(SDLK_F5,  KEY_F5));
	KeyMap.push_back(SKeyMap(SDLK_F6,  KEY_F6));
	KeyMap.push_back(SKeyMap(SDLK_F7,  KEY_F7));
	KeyMap.push_back(SKeyMap(SDLK_F8,  KEY_F8));
	KeyMap.push_back(SKeyMap(SDLK_F9,  KEY_F9));
	KeyMap.push_back(SKeyMap(SDLK_F10, KEY_F10));
	KeyMap.push_back(SKeyMap(SDLK_F11, KEY_F11));
	KeyMap.push_back(SKeyMap(SDLK_F12, KEY_F12));
	KeyMap.push_back(SKeyMap(SDLK_F13, KEY_F13));
	KeyMap.push_back(SKeyMap(SDLK_F14, KEY_F14));
	KeyMap.push_back(SKeyMap(SDLK_F15, KEY_F15));
	// no higher F-keys

	KeyMap.push_back(SKeyMap(SDLK_NUMLOCK, KEY_NUMLOCK));
	KeyMap.push_back(SKeyMap(SDLK_SCROLLOCK, KEY_SCROLL));
	KeyMap.push_back(SKeyMap(SDLK_LSHIFT, KEY_LSHIFT));
	KeyMap.push_back(SKeyMap(SDLK_RSHIFT, KEY_RSHIFT));
	KeyMap.push_back(SKeyMap(SDLK_LCTRL,  KEY_LCONTROL));
	KeyMap.push_back(SKeyMap(SDLK_RCTRL,  KEY_RCONTROL));
	KeyMap.push_back(SKeyMap(SDLK_LALT,  KEY_LMENU));
	KeyMap.push_back(SKeyMap(SDLK_RALT,  KEY_RMENU));

	KeyMap.push_back(SKeyMap(SDLK_PLUS,   KEY_PLUS));
	KeyMap.push_back(SKeyMap(SDLK_COMMA,  KEY_COMMA));
	KeyMap.push_back(SKeyMap(SDLK_MINUS,  KEY_MINUS));
	KeyMap.push_back(SKeyMap(SDLK_PERIOD, KEY_PERIOD));

	// some special keys missing

	KeyMap.sort();
}

} // end namespace irr

#endif // _IRR_COMPILE_WITH_SDL_DEVICE_

