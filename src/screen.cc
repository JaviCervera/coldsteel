#include "../lib/sdl/include/SDL/SDL.h"
#include "../lib/sdl/include/SDL/SDL_mixer.h"
#include "core.h"
#include "gui.h"
#include "input.h"
#include "listener.h"
#include "screen.h"


class csEventReceiver : public IEventReceiver {
    virtual bool OnEvent(const SEvent& event) {
        switch (event.EventType) {
            case EET_GUI_EVENT:
                switch (event.GUIEvent.EventType) {
                    case EGET_ELEMENT_FOCUS_LOST:
                        _asPostEvent(ASCONTROL_LOSTFOCUS, event.GUIEvent.Caller);
                        break;
                    case EGET_ELEMENT_HOVERED:
                        _asPostEvent(ASCONTROL_ENTER, event.GUIEvent.Caller);
                        break;
                    case EGET_ELEMENT_LEFT:
                        _asPostEvent(ASCONTROL_EXIT, event.GUIEvent.Caller);
                        break;
                    case EGET_BUTTON_CLICKED:
                        _asPostEvent(ASCONTROL_ACTION, event.GUIEvent.Caller);
                        break;
                    case EGET_SCROLL_BAR_CHANGED:
                        _asPostEvent(ASCONTROL_ACTION, event.GUIEvent.Caller);
                        break;
                    case EGET_CHECKBOX_CHANGED:
                        _asPostEvent(ASCONTROL_ACTION, event.GUIEvent.Caller);
                        break;
                    case EGET_LISTBOX_CHANGED:
                        _asPostEvent(ASCONTROL_ACTION, event.GUIEvent.Caller);
                        break;
                    case EGET_LISTBOX_SELECTED_AGAIN:
                        _asPostEvent(ASCONTROL_ACTION, event.GUIEvent.Caller);
                        break;
                    case EGET_TAB_CHANGED:
                        _asPostEvent(ASCONTROL_ACTION, event.GUIEvent.Caller);
                        break;
                    case EGET_EDITBOX_ENTER:
                        _asPostEvent(ASCONTROL_ACTION, event.GUIEvent.Caller);
                        break;
                    case EGET_MENU_ITEM_SELECTED:
                        _asPostEvent(ASCONTROL_ACTION, event.GUIEvent.Caller);
                        break;
                    case EGET_COMBO_BOX_CHANGED:
                        _asPostEvent(ASCONTROL_ACTION, event.GUIEvent.Caller);
                        break;
                }
                break;
            case EET_JOYSTICK_INPUT_EVENT:
                _asSetJoystickEvent(event.JoystickEvent);
                break;
            case EET_KEY_INPUT_EVENT:
                _asSetKeyDown(event.KeyInput.Key, event.KeyInput.PressedDown);
                break;
            case EET_MOUSE_INPUT_EVENT:
                switch ( event.MouseInput.Event ) {
                    case EMIE_MOUSE_MOVED:
                        _asSetCursorMoved();
                        break;
                    case EMIE_LMOUSE_PRESSED_DOWN:
                        _asSetButtonDown(ASBUTTON_LEFT, true);
                        break;
                    case EMIE_RMOUSE_PRESSED_DOWN:
                        _asSetButtonDown(ASBUTTON_RIGHT, true);
                        break;
                    case EMIE_MMOUSE_PRESSED_DOWN:
                        _asSetButtonDown(ASBUTTON_MIDDLE, true);
                        break;
                    case EMIE_LMOUSE_LEFT_UP:
                        _asSetButtonDown(ASBUTTON_LEFT, false);
                        break;
                    case EMIE_RMOUSE_LEFT_UP:
                        _asSetButtonDown(ASBUTTON_RIGHT, false);
                        break;
                    case EMIE_MMOUSE_LEFT_UP:
                        _asSetButtonDown(ASBUTTON_MIDDLE, false);
                        break;
                }
                break;
        }
        return false;
    }
};


extern "C" {


static int _screenFrameMsecs = 0;


EXPORT void CALL asOpenScreen(int width, int height, int depth, int flags) {
    asOpenScreenEx(width, height, depth, flags, NULL);
}


EXPORT void CALL asOpenScreenEx(int width, int height, int depth, int flags, void* win) {
    bool fullscreen = (flags & ASSCREEN_WINDOWED) == 0;
    bool vsync = (flags & ASSCREEN_VSYNC) == ASSCREEN_VSYNC;
    //bool antialias = (flags & ASSCREEN_ANTIALIAS) == ASSCREEN_ANTIALIAS;
    
    SIrrlichtCreationParameters params;
    //params.AntiAlias = antialias;
    params.Bits = depth;
    params.DriverType = EDT_OPENGL;
    params.EventReceiver = new csEventReceiver;
    params.LoggingLevel = ELL_NONE;
    params.Fullscreen = fullscreen;
    params.Stencilbuffer = true;
    params.Vsync = vsync;
    params.WindowId = win;
    params.WindowSize.Width = width;
    params.WindowSize.Height = height;

    // Init device
    _asSetDevice(createDeviceEx(params));
    _asDevice()->setResizable((flags & ASSCREEN_RESIZABLE) == ASSCREEN_RESIZABLE);
    _asDevice()->getVideoDriver()->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);

    // Init audio
    if (SDL_WasInit(SDL_INIT_EVERYTHING) == 0) SDL_Init(SDL_INIT_AUDIO);
    if (SDL_WasInit(SDL_INIT_AUDIO) == 0) SDL_InitSubSystem(SDL_INIT_AUDIO);
    Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096);
    asSetListener(0, 0, 0, 0);
    
    // Init gui skin
    IGUISkin* skin = _asDevice()->getGUIEnvironment()->createSkin(EGST_WINDOWS_CLASSIC);
    for (int i = 0; i < EGDC_COUNT; ++i) {
        SColor col = skin->getColor((EGUI_DEFAULT_COLOR)i);
        col.setAlpha(255);
        skin->setColor((EGUI_DEFAULT_COLOR)i, col);
    }
    _asDevice()->getGUIEnvironment()->setSkin(skin);
    skin->drop();
}


EXPORT void CALL asCloseScreen() {
    Mix_Quit();
    _asSetDevice(NULL);
}


EXPORT void CALL asSetScreenTitle(const char* caption) {
    _asDevice()->setWindowCaption(_WSTR(caption));
}


EXPORT void CALL asSetScreenSize(int width, int height) {
    _asDevice()->getVideoDriver()->OnResize(dimension2du(width, height));
}


EXPORT bool_t CALL asScreenActive() {
    return _asDevice()->isWindowActive();
}


EXPORT int CALL asScreenWidth() {
    return _asDevice()->getVideoDriver()->getScreenSize().Width;
}


EXPORT int CALL asScreenHeight() {
    return _asDevice()->getVideoDriver()->getScreenSize().Height;
}


EXPORT void CALL asSetScreenFPS(int fps) {
    _screenFrameMsecs = (fps > 0)
        ? (1000 / fps)
        : 0;
}


EXPORT int CALL asScreenFPS() {
    return _asDevice()->getVideoDriver()->getFPS();
}


EXPORT bool_t CALL asFeatureSupported(int feature) {
    return _asDevice()->getVideoDriver()->queryFeature((E_VIDEO_DRIVER_FEATURE)feature);
}


EXPORT int CALL asScreenNumModes() {
    return _asDevice()->getVideoModeList()->getVideoModeCount();
}


EXPORT int CALL asScreenModeWidth(int index) {
    return _asDevice()->getVideoModeList()->getVideoModeResolution(index).Width;
}


EXPORT int CALL asScreenModeHeight(int index) {
    return _asDevice()->getVideoModeList()->getVideoModeResolution(index).Height;
}


EXPORT int CALL asScreenModeDepth(int index) {
    return _asDevice()->getVideoModeList()->getVideoModeDepth(index);
}


EXPORT int CALL asDesktopWidth() {
    return _asDevice()->getVideoModeList()->getDesktopResolution().Width;
}


EXPORT int CALL asDesktopHeight() {
    return _asDevice()->getVideoModeList()->getDesktopResolution().Height;
}


EXPORT int CALL asDesktopDepth() {
    return _asDevice()->getVideoModeList()->getDesktopDepth();
}


int _asScreenFrameMsecs() {
    return _screenFrameMsecs;
}


} // extern "C"
