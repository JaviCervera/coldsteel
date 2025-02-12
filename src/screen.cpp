#include "core.h"
#include "graphics.h"
#include "gui.h"
#include "input.h"
#include "listener.h"
#include "screen.h"

class EventReceiver : public IEventReceiver
{
    virtual bool OnEvent(const SEvent &event)
    {
        switch (event.EventType)
        {
        case EET_GUI_EVENT:
            switch (event.GUIEvent.EventType)
            {
            case EGET_ELEMENT_FOCUS_LOST:
                _PostEvent(CONTROL_LOSTFOCUS, event.GUIEvent.Caller, -1);
                break;
            case EGET_ELEMENT_HOVERED:
                _PostEvent(CONTROL_ENTER, event.GUIEvent.Caller, -1);
                break;
            case EGET_ELEMENT_LEFT:
                _PostEvent(CONTROL_EXIT, event.GUIEvent.Caller, -1);
                break;
            case EGET_BUTTON_CLICKED:
                _PostEvent(CONTROL_ACTION, event.GUIEvent.Caller, -1);
                break;
            case EGET_SCROLL_BAR_CHANGED:
                _PostEvent(CONTROL_ACTION, event.GUIEvent.Caller, -1);
                break;
            case EGET_CHECKBOX_CHANGED:
                _PostEvent(CONTROL_ACTION, event.GUIEvent.Caller, -1);
                break;
            case EGET_LISTBOX_CHANGED:
                _PostEvent(CONTROL_ACTION, event.GUIEvent.Caller, -1);
                break;
            case EGET_LISTBOX_SELECTED_AGAIN:
                _PostEvent(CONTROL_ACTION, event.GUIEvent.Caller, -1);
                break;
            case EGET_TAB_CHANGED:
                _PostEvent(CONTROL_ACTION, event.GUIEvent.Caller, -1);
                break;
            case EGET_EDITBOX_ENTER:
                _PostEvent(CONTROL_ACTION, event.GUIEvent.Caller, -1);
                break;
            case EGET_MENU_ITEM_SELECTED:
                _PostEvent(
                    CONTROL_ACTION,
                    event.GUIEvent.Caller,
                    ((IGUIContextMenu *)event.GUIEvent.Caller)->getItemCommandId(((IGUIContextMenu *)event.GUIEvent.Caller)->getSelectedItem()));
                break;
            case EGET_COMBO_BOX_CHANGED:
                _PostEvent(CONTROL_ACTION, event.GUIEvent.Caller, -1);
                break;
            default:
                break;
            }
            break;
        case EET_JOYSTICK_INPUT_EVENT:
            _SetJoystickEvent(event.JoystickEvent);
            break;
        case EET_KEY_INPUT_EVENT:
            _SetKeyDown(event.KeyInput.Key, event.KeyInput.PressedDown);
            break;
        case EET_MOUSE_INPUT_EVENT:
            switch (event.MouseInput.Event)
            {
            case EMIE_MOUSE_MOVED:
                _SetCursorMoved();
                break;
            case EMIE_LMOUSE_PRESSED_DOWN:
                _SetButtonDown(BUTTON_LEFT, true);
                break;
            case EMIE_RMOUSE_PRESSED_DOWN:
                _SetButtonDown(BUTTON_RIGHT, true);
                break;
            case EMIE_MMOUSE_PRESSED_DOWN:
                _SetButtonDown(BUTTON_MIDDLE, true);
                break;
            case EMIE_LMOUSE_LEFT_UP:
                _SetButtonDown(BUTTON_LEFT, false);
                break;
            case EMIE_RMOUSE_LEFT_UP:
                _SetButtonDown(BUTTON_RIGHT, false);
                break;
            case EMIE_MMOUSE_LEFT_UP:
                _SetButtonDown(BUTTON_MIDDLE, false);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        return false;
    }
};

extern "C"
{

    static int _screenFrameMsecs = 0;

    EXPORT void CALL SetScreenTitle(const char *caption)
    {
        _Device()->setWindowCaption(_WSTR(caption));
    }

    EXPORT void CALL SetScreenSize(int width, int height)
    {
        _Device()->getVideoDriver()->OnResize(dimension2du(width, height));
    }

    EXPORT bool_t CALL ScreenActive()
    {
        return _Device()->isWindowActive();
    }

    EXPORT int CALL ScreenWidth()
    {
        return _Device()->getVideoDriver()->getScreenSize().Width;
    }

    EXPORT int CALL ScreenHeight()
    {
        return _Device()->getVideoDriver()->getScreenSize().Height;
    }

    EXPORT void CALL SetScreenFPS(int fps)
    {
        _screenFrameMsecs = (fps > 0)
                                ? (1000 / fps)
                                : 0;
    }

    EXPORT int CALL ScreenFPS()
    {
        return _Device()->getVideoDriver()->getFPS();
    }

    EXPORT bool_t CALL FeatureSupported(int feature)
    {
        return _Device()->getVideoDriver()->queryFeature((E_VIDEO_DRIVER_FEATURE)feature);
    }

    void _OpenScreen(int width, int height, int depth, int flags)
    {
        _OpenScreenEx(width, height, depth, flags, NULL);
    }

    void _OpenScreenEx(int width, int height, int depth, int flags, void *win)
    {
        bool fullscreen = (flags & SCREEN_WINDOWED) == 0;
        bool vsync = (flags & SCREEN_VSYNC) == SCREEN_VSYNC;
        // bool antialias = (flags & SCREEN_ANTIALIAS) == SCREEN_ANTIALIAS;

        SIrrlichtCreationParameters params;
        // params.AntiAlias = antialias;
        params.Bits = depth;
#ifdef EMSCRIPTEN
        params.DriverType = EDT_WEBGL1;
#else
        params.DriverType = EDT_OPENGL; // EDT_OGLES2
#endif
        params.EventReceiver = new EventReceiver;
        params.LoggingLevel = ELL_ERROR;
        params.Fullscreen = fullscreen;
        params.Stencilbuffer = true;
        params.Vsync = vsync;
        params.WindowId = win;
        params.WindowSize.Width = width;
        params.WindowSize.Height = height;

        // Init device
        _SetDevice(createDeviceEx(params));
        _Device()->setResizable((flags & SCREEN_RESIZABLE) == SCREEN_RESIZABLE);
        _Device()->getVideoDriver()->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);
        _Device()->getSceneManager()->setAmbientLight(_Color(RGB(255, 255, 255)));

        // Init gui skin
        IGUISkin *skin = _Device()->getGUIEnvironment()->createSkin(EGST_WINDOWS_CLASSIC);
        for (int i = 0; i < EGDC_COUNT; ++i)
        {
            SColor col = skin->getColor((EGUI_DEFAULT_COLOR)i);
            col.setAlpha(255);
            skin->setColor((EGUI_DEFAULT_COLOR)i, col);
        }
        _Device()->getGUIEnvironment()->setSkin(skin);
        skin->drop();
    }

    void _CloseScreen()
    {
        _SetDevice(NULL);
    }

    int _ScreenNumModes()
    {
        return _Device()->getVideoModeList()->getVideoModeCount();
    }

    int _ScreenModeWidth(int index)
    {
        return _Device()->getVideoModeList()->getVideoModeResolution(index).Width;
    }

    int _ScreenModeHeight(int index)
    {
        return _Device()->getVideoModeList()->getVideoModeResolution(index).Height;
    }

    int _ScreenModeDepth(int index)
    {
        return _Device()->getVideoModeList()->getVideoModeDepth(index);
    }

    int _DesktopWidth()
    {
        return _Device()->getVideoModeList()->getDesktopResolution().Width;
    }

    int _DesktopHeight()
    {
        return _Device()->getVideoModeList()->getDesktopResolution().Height;
    }

    int _DesktopDepth()
    {
        return _Device()->getVideoModeList()->getDesktopDepth();
    }

    int _ScreenFrameMsecs()
    {
        return _screenFrameMsecs;
    }

} // extern "C"
