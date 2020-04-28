#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/angelscript/include/angelscript.h"
#include "../lib/angelscript/add_on/scriptarray/scriptarray.h"
#include "../lib/angelscript/add_on/scriptbuilder/scriptbuilder.h"
#include "../lib/angelscript/add_on/scriptstdstring/scriptstdstring.h"
#include "engine.h"


enum CompilerMode {
    MODE_BUILD,
    MODE_CHECK,
    MODE_RUN
};


struct CompilerConfig {
    CompilerMode mode;
    stringc sourceFilename;
    stringc bytecodeFilename;
    stringc path;
};


struct FileStream : public asIBinaryStream {
    FILE* f;

    FileStream(const char* filename, bool write = false) { f = fopen(filename, write ? "wb" : "rb"); }
    ~FileStream() { fclose(f); }
    virtual int Write(const void* ptr, asUINT size) { return fwrite(ptr, size, 1, f); }
    virtual int Read(void* ptr, asUINT size) { return fread(ptr, size, 1, f); }
};


CompilerConfig ParseCommandLine(int argc, char* argv[]);
void SetMessageCallback(asIScriptEngine* engine);
bool LoadScript(CScriptBuilder* builder, asIScriptEngine* engine, const char* filename);
void BuildScript(CScriptBuilder* builder);
void LoadBytecode(CScriptBuilder* builder, const char* filename);
void SaveBytecode(CScriptBuilder* builder, const char* filename);
void Run(CScriptBuilder* builder, const stringc& path);
void RegisterAstro(asIScriptEngine* engine);


int main(int argc, char* argv[]) {
    asInit();
    const CompilerConfig config = ParseCommandLine(argc, argv);
    asIScriptEngine *const engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
    SetMessageCallback(engine);
    CScriptBuilder builder;
    const bool scriptLoaded = LoadScript(&builder, engine, config.sourceFilename.c_str());
    RegisterScriptArray(engine, true);
    RegisterStdString(engine);
    RegisterAstro(engine);
    if (scriptLoaded) BuildScript(&builder);
    else LoadBytecode(&builder, config.bytecodeFilename.c_str());
    if (config.mode == MODE_BUILD) SaveBytecode(&builder, config.bytecodeFilename.c_str());
    else if (config.mode == MODE_RUN) Run(&builder, config.path);
    engine->Release();
    return 0;
}


CompilerConfig ParseCommandLine(int argc, char* argv[]) {
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
    config.bytecodeFilename = (config.path != "")
        ? (config.path + "/code.bcd")
        : "code.bcd";
    return config;
}


void MessageCallback(const asSMessageInfo* msg, void*) {
    const char* type =
        (msg->type == asMSGTYPE_ERROR) ? "ERROR" :
        (msg->type == asMSGTYPE_WARNING) ? "WARNING" :
        "";
    if (strcmp(type, "") != 0) {
        printf("%s (%d, %d) %s: %s\n", msg->section, msg->row, msg->col, type, msg->message);
    }
}


void SetMessageCallback(asIScriptEngine* engine) {
    if (engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL) < 0) {
        printf("ERROR: Could not set message callback.\n");
        engine->Release();
        exit(-1);
    }
}


bool LoadScript(CScriptBuilder* builder, asIScriptEngine* engine, const char* filename) {
    if (builder->StartNewModule(engine, "MainModule") < 0) {
        printf("ERROR: Could not start main module.\n");
        engine->Release();
        exit(-1);
    }
    if (strcmp(filename, "") != 0) {
        const char* code = asLoadString(filename);
        if (strcmp(code, "") == 0 || builder->AddSectionFromMemory(filename, code) < 0) {
            printf("ERROR: Could not load script '%s'.\n", filename);
            engine->Release();
            exit(-1);
        }
        return true;
    } else {
        return false;
    }
}


void BuildScript(CScriptBuilder* builder) {
    if (builder->BuildModule() < 0) {
        builder->GetEngine()->Release();
        exit(-1);
    }
}


void LoadBytecode(CScriptBuilder* builder, const char* filename) {
    FileStream stream(filename, false);
    builder->GetModule()->LoadByteCode(&stream);
}


void SaveBytecode(CScriptBuilder* builder, const char* filename) {
    FileStream stream(filename, true);
    builder->GetModule()->SaveByteCode(&stream, true);
}


void Run(CScriptBuilder* builder, const stringc& path) {
    asChangeDir(path.c_str());
    asIScriptContext* context = builder->GetEngine()->CreateContext();
    asIScriptFunction* main = builder->GetModule()->GetFunctionByDecl("void Main()");
    if (!main) {
        printf("ERROR: 'void Main()' function is not defined in the script.");
        exit(-1);
    }
    context->Prepare(main);
    int ret = context->Execute();
    if ( ret != asEXECUTION_FINISHED ) {
        if ( ret == asEXECUTION_EXCEPTION ) {
            printf("ERROR: Uncaught exception \"%s\"\n", context->GetExceptionString());
            exit(-1);
        }
    }
    context->Release();
}
