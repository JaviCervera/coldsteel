#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef LoadString
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#else
#include <unistd.h>
#endif
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "dir.h"
#include "screen.h"
#include "script.h"
#include "string.h"
#include "xml.h"


enum CompilerMode {
    MODE_BUILD,
    MODE_CHECK,
    MODE_RUN
};


struct CompilerConfig {
    CompilerMode mode;
    stringc sourceFilename;
    stringc path;
    int screenWidth;
    int screenHeight;
    int screenFps;
    int screenFlags;
};


static stringc GetBinDir();
static CompilerConfig InitConfig(int argc, char* argv[]);
static void PrintError();
static void MainLoop();
static void EmscriptenMainLoop();
//static CompilerConfig ParseCommandLine(int argc, char* argv[]);


int main(int argc, char* argv[]) {
    _Init();
    const CompilerConfig config = InitConfig(argc, argv);
    //const CompilerConfig config = ParseCommandLine(argc, argv);
#ifdef __APPLE__
    ChangeDir(GetBinDir().c_str());
#endif
    if (config.path != "") ChangeDir(config.path.c_str());
    _OpenScreen(config.screenWidth, config.screenHeight, _DesktopDepth(), config.screenFlags);
    if (!Script::Get().Load(config.sourceFilename)) {
        PrintError();
        return -1;
    }
    if (Script::Get().FunctionExists("Loop")) {
#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(EmscriptenMainLoop, config.screenFps, true);
#else
        SetScreenFPS(config.screenFps);
        MainLoop();
#endif
    } else {
        _Run();
    }
    _CloseScreen();
    _Finish();
    return 0;
}


static stringc ExtractDir(const char* filename) {
    const char* fendp = strrchr(filename, '/');
    const char* bendp = strrchr(filename, '\\');
    const char* endp = (fendp >= bendp) ? fendp : bendp;
    if (!endp) return "";
    const size_t size = endp - filename;
    return Mid(filename, 0, size);
}


static stringc GetBinDir() {
    char path[FILENAME_MAX];
#if defined(_WIN32)
    path[GetModuleFileNameA(NULL, path, FILENAME_MAX)] = 0;
#elif defined(__APPLE__)
    unsigned int size = FILENAME_MAX;
    _NSGetExecutablePath(path, &size);
#else
    path[readlink("/proc/self/exe", path, FILENAME_MAX)] = 0;
#endif
    return ExtractDir(path);
}


static CompilerConfig InitConfig(int argc, char* argv[]) {
    CompilerConfig config;
    config.mode = MODE_RUN;
    config.sourceFilename = "main.lua";
    config.path = (argc > 1) ? (stringc(argv[1]) + "/") : "";
    config.screenWidth = 640;
    config.screenHeight = 480;
    config.screenFps = 0;
    XMLNode* xml = ParseXML("config.xml");
    XMLNode* screenWidth = xml
        ? XMLChildNamed(xml, "screen_width", 0)
        : NULL;
    XMLNode* screenHeight = xml
        ? XMLChildNamed(xml, "screen_height", 0)
        : NULL;
    XMLNode* screenFps = xml
        ? XMLChildNamed(xml, "screen_fps", 0)
        : NULL;
    XMLNode* screenWindowed = xml
        ? XMLChildNamed(xml, "screen_windowed", 0)
        : NULL;
    XMLNode* screenResizable = xml
        ? XMLChildNamed(xml, "screen_resizable", 0)
        : NULL;
    XMLNode* screenVsync = xml
        ? XMLChildNamed(xml, "screen_vsync", 0)
        : NULL;
    if (screenWidth) config.screenWidth = Val(XMLText(screenWidth));
    if (screenHeight) config.screenHeight = Val(XMLText(screenHeight));
    if (screenFps) config.screenFps = Val(XMLText(screenFps));
    int flags = 0;
    if (screenWindowed) {
        if (stringc(XMLText(screenWindowed)) == "true") flags |= SCREEN_WINDOWED;
    } else {
        flags |= SCREEN_WINDOWED;
    }
    if (screenResizable && stringc(XMLText(screenResizable)) == "true") flags |= SCREEN_RESIZABLE;
    if (screenVsync && stringc(XMLText(screenVsync)) == "true") flags |= SCREEN_VSYNC;
    config.screenFlags = flags;
#ifndef EMSCRIPTEN
    if (config.screenWidth == 0) config.screenWidth = _DesktopWidth();
    if (config.screenHeight == 0) config.screenHeight = _DesktopHeight();
#else
    if (config.screenWidth == 0) config.screenWidth = 640;
    if (config.screenHeight == 0) config.screenHeight = 480;
#endif
    if (xml) FreeXML(xml);
    return config;
}


static void PrintError() {
    _Device()->getLogger()->log(Script::Get().Error().c_str(), ELL_ERROR);
#ifdef _WIN32
    MessageBoxA(NULL, Script::Get().Error().c_str(), "Error", MB_OK | MB_ICONERROR);
#endif
}


static void MainLoop() {
    bool ok = true;
    while (_Run() && ok) {
        ok = Script::Get().CallVoidFunction("Loop");
    }
    if (!ok) PrintError();
}


#ifdef EMSCRIPTEN
static void EmscriptenMainLoop() {
    if (_Run()) {
        if (!Script::Get().CallVoidFunction("Loop")) PrintError();
    } else {
        emscripten_cancel_main_loop();
    }
}
#endif


/*
static CompilerConfig ParseCommandLine(int argc, char* argv[]) {
    CompilerConfig config;
    config.mode = MODE_RUN;
    for (int i = 1; i < argc-1; ++i) {
        if (strcmp(argv[i], "-check") == 0) {
            config.mode = MODE_CHECK;
        } else if (strcmp(argv[i], "-build") == 0) {
            config.mode = MODE_BUILD;
        } else if (strcmp(argv[i], "-help") == 0) {
            printf("Usage: runtime [options] [sourcefile]\n");
            printf("Options:\n");
            printf(" -run: Run program (default).\n");
            printf(" -check: Check syntax (do not generate output).\n");
            printf(" -build: Build 'code.bcd' bytecode file.\n");
            printf(" -help: Prints this text.\n");
            printf("When called without arguments, the file 'code.bcd' will be executed.\n");
            exit(-1);
        } else if (strcmp(argv[i], "-run") != 0) {
            printf("Unrecognized argument: %s\n", argv[i]);
            exit(-1);
        }
    }
    config.sourceFilename = (argc > 1) ? argv[argc-1] : "";
    const int lastPathPos = config.sourceFilename.replace("\\", "/").findLast('/');
    config.path = (lastPathPos != -1)
        ? config.sourceFilename.subString(0, lastPathPos)
        : "";
    return config;
}
*/
