#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine.h"
#include "script.h"


enum CompilerMode {
    MODE_BUILD,
    MODE_CHECK,
    MODE_RUN
};


struct CompilerConfig {
    CompilerMode mode;
    stringc sourceFilename;
    stringc path;
};


static CompilerConfig ParseCommandLine(int argc, char* argv[]);


int main(int argc, char* argv[]) {
    Init();
    const CompilerConfig config = ParseCommandLine(argc, argv);
    //if (config.path != "") ChangeDir(config.path.c_str());
    Script script;
    if (!script.Load(config.sourceFilename)) {
        printf("%s\n", script.Error().c_str());
    }
    if (script.FunctionExists("Loop")) {
        bool ok = true;
        while (Run() && ok) {
            ok = script.CallVoidFunction("Loop");
        }
        if (!ok) printf("%s\n", script.Error().c_str());
    } else {
        Run();
    }
    return 0;
}


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
