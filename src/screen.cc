#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
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
                        _csPostEvent(CSCONTROL_LOSTFOCUS, event.GUIEvent.Caller);
                        break;
                    case EGET_ELEMENT_HOVERED:
                        _csPostEvent(CSCONTROL_ENTER, event.GUIEvent.Caller);
                        break;
                    case EGET_ELEMENT_LEFT:
                        _csPostEvent(CSCONTROL_EXIT, event.GUIEvent.Caller);
                        break;
                    case EGET_BUTTON_CLICKED:
                        _csPostEvent(CSCONTROL_ACTION, event.GUIEvent.Caller);
                        break;
                    case EGET_SCROLL_BAR_CHANGED:
                        _csPostEvent(CSCONTROL_ACTION, event.GUIEvent.Caller);
                        break;
                    case EGET_CHECKBOX_CHANGED:
                        _csPostEvent(CSCONTROL_ACTION, event.GUIEvent.Caller);
                        break;
                    case EGET_LISTBOX_CHANGED:
                        _csPostEvent(CSCONTROL_ACTION, event.GUIEvent.Caller);
                        break;
                    case EGET_LISTBOX_SELECTED_AGAIN:
                        _csPostEvent(CSCONTROL_ACTION, event.GUIEvent.Caller);
                        break;
                    case EGET_TAB_CHANGED:
                        _csPostEvent(CSCONTROL_ACTION, event.GUIEvent.Caller);
                        break;
                    case EGET_EDITBOX_ENTER:
                        _csPostEvent(CSCONTROL_ACTION, event.GUIEvent.Caller);
                        break;
                    case EGET_MENU_ITEM_SELECTED:
                        _csPostEvent(CSCONTROL_ACTION, event.GUIEvent.Caller);
                        break;
                    case EGET_COMBO_BOX_CHANGED:
                        _csPostEvent(CSCONTROL_ACTION, event.GUIEvent.Caller);
                        break;
                }
                break;
            case EET_JOYSTICK_INPUT_EVENT:
                _csSetJoystickEvent(event.JoystickEvent);
                break;
            case EET_KEY_INPUT_EVENT:
                _csSetKeyDown(event.KeyInput.Key, event.KeyInput.PressedDown);
                break;
            case EET_MOUSE_INPUT_EVENT:
                switch ( event.MouseInput.Event ) {
                    case EMIE_MOUSE_MOVED:
                        _csSetCursorMoved();
                        break;
                    case EMIE_LMOUSE_PRESSED_DOWN:
                        _csSetButtonDown(CSBUTTON_LEFT, true);
                        break;
                    case EMIE_RMOUSE_PRESSED_DOWN:
                        _csSetButtonDown(CSBUTTON_RIGHT, true);
                        break;
                    case EMIE_MMOUSE_PRESSED_DOWN:
                        _csSetButtonDown(CSBUTTON_MIDDLE, true);
                        break;
                    case EMIE_LMOUSE_LEFT_UP:
                        _csSetButtonDown(CSBUTTON_LEFT, false);
                        break;
                    case EMIE_RMOUSE_LEFT_UP:
                        _csSetButtonDown(CSBUTTON_RIGHT, false);
                        break;
                    case EMIE_MMOUSE_LEFT_UP:
                        _csSetButtonDown(CSBUTTON_MIDDLE, false);
                        break;
                }
                break;
        }
        return false;
    }
};


extern "C" {


static int _screenFrameMsecs = 0;


EXPORT void CALL csOpenScreen(int width, int height, int depth, int flags) {
    csOpenScreenEx(width, height, depth, flags, NULL);
}


EXPORT void CALL csOpenScreenEx(int width, int height, int depth, int flags, void* win) {
    bool fullscreen = (flags & CSSCREEN_WINDOWED) == 0;
    bool vsync = (flags & CSSCREEN_VSYNC) == CSSCREEN_VSYNC;
    //bool antialias = (flags & CSSCREEN_ANTIALIAS) == CSSCREEN_ANTIALIAS;
    
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
    _csSetDevice(createDeviceEx(params));
    _csDevice()->setResizable((flags & CSSCREEN_RESIZABLE) == CSSCREEN_RESIZABLE);
    _csDevice()->getVideoDriver()->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);

    // Init audio
    if (SDL_WasInit(SDL_INIT_EVERYTHING) == 0) SDL_Init(SDL_INIT_AUDIO);
    if (SDL_WasInit(SDL_INIT_AUDIO) == 0) SDL_InitSubSystem(SDL_INIT_AUDIO);
    Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096);
    csSetListener(0, 0, 0, 0);
    
    // Init gui skin
    IGUISkin* skin = _csDevice()->getGUIEnvironment()->createSkin(EGST_WINDOWS_CLASSIC);
    for (int i = 0; i < EGDC_COUNT; ++i) {
        SColor col = skin->getColor((EGUI_DEFAULT_COLOR)i);
        col.setAlpha(255);
        skin->setColor((EGUI_DEFAULT_COLOR)i, col);
    }
    _csDevice()->getGUIEnvironment()->setSkin(skin);
    skin->drop();
}


EXPORT void CALL csCloseScreen() {
    Mix_Quit();
    _csSetDevice(NULL);
}


EXPORT void CALL csSetScreenTitle(const char* caption) {
    _csDevice()->setWindowCaption(_WSTR(caption));
}


EXPORT void CALL csSetScreenSize(int width, int height) {
    _csDevice()->getVideoDriver()->OnResize(dimension2du(width, height));
}


EXPORT bool_t CALL csScreenActive() {
    return _csDevice()->isWindowActive();
}


EXPORT int CALL csScreenWidth() {
    return _csDevice()->getVideoDriver()->getScreenSize().Width;
}


EXPORT int CALL csScreenHeight() {
    return _csDevice()->getVideoDriver()->getScreenSize().Height;
}


EXPORT void CALL csSetScreenFPS(int fps) {
    _screenFrameMsecs = (fps > 0)
        ? (1000 / fps)
        : 0;
}


EXPORT int CALL csScreenFPS() {
    return _csDevice()->getVideoDriver()->getFPS();
}


EXPORT bool_t CALL csFeatureSupported(int feature) {
    return _csDevice()->getVideoDriver()->queryFeature((E_VIDEO_DRIVER_FEATURE)feature);
}


EXPORT int CALL csScreenNumModes() {
    return _csDevice()->getVideoModeList()->getVideoModeCount();
}


EXPORT int CALL csScreenModeWidth(int index) {
    return _csDevice()->getVideoModeList()->getVideoModeResolution(index).Width;
}


EXPORT int CALL csScreenModeHeight(int index) {
    return _csDevice()->getVideoModeList()->getVideoModeResolution(index).Height;
}


EXPORT int CALL csScreenModeDepth(int index) {
    return _csDevice()->getVideoModeList()->getVideoModeDepth(index);
}


EXPORT int CALL csDesktopWidth() {
    return _csDevice()->getVideoModeList()->getDesktopResolution().Width;
}


EXPORT int CALL csDesktopHeight() {
    return _csDevice()->getVideoModeList()->getDesktopResolution().Height;
}


EXPORT int CALL csDesktopDepth() {
    return _csDevice()->getVideoModeList()->getDesktopDepth();
}


int _csScreenFrameMsecs() {
    return _screenFrameMsecs;
}


} // extern "C"
