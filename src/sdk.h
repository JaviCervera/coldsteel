#ifndef COLDSTEEL_SDK_H_INCLUDED
#define COLDSTEEL_SDK_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void (*RegisterFunction)(const char* name, int (*f)(void*));
    int (*GetBoolArg)(void* context, int index);
    int (*GetIntArg)(void* context, int index);
    float (*GetFloatArg)(void* context, int index);
    const char* (*GetStringArg)(void* context, int index);
    const void* (*GetPointerArg)(void* context, int index);
    void (*PushBool)(void* context, int val);
    void (*PushInt)(void* context, int val);
    void (*PushFloat)(void* context, float val);
    void (*PushString)(void* context, const char* val);
    void (*PushPointer)(void* context, void* val);
} ColdSteelSDK;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* COLDSTEEL_SDK_H_INCLUDED */
