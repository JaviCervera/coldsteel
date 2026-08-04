#ifdef _WIN32

typedef unsigned long DWORD;
typedef long LONG;
typedef int BOOL;
struct _RTL_CRITICAL_SECTION;
typedef struct _RTL_CRITICAL_SECTION* LPCRITICAL_SECTION;
typedef void* HANDLE;
typedef HANDLE HCURSOR;
typedef DWORD* LPDWORD;
typedef DWORD DWORD_PTR;

typedef struct { LONG x; LONG y; } POINT;
typedef struct {
    DWORD cbSize;
    DWORD flags;
    HCURSOR hCursor;
    POINT ptScreenPos;
} CURSORINFO, *PCURSORINFO, *LPCURSORINFO;

#define TRUE 1
#define FALSE 0

extern "C" {
    void __stdcall InitializeCriticalSection(LPCRITICAL_SECTION);
    DWORD __stdcall GetCurrentThreadId(void);
    void __stdcall SetLastError(DWORD);
    BOOL __stdcall GetCursorPos(POINT*);
    HCURSOR __stdcall GetCursor(void);
}

#define ERROR_INVALID_PARAMETER 87L
#define HANDLE_FLAG_INHERIT 1

extern "C" void __stdcall InitializeCriticalSectionAndSpinCount(
    LPCRITICAL_SECTION lpCriticalSection, DWORD dwSpinCount)
{
    (void)dwSpinCount;
    InitializeCriticalSection(lpCriticalSection);
}

extern "C" LONG __stdcall InterlockedExchangeAdd(
    LONG volatile* Addend, LONG Value)
{
    LONG Result;
    __asm__ __volatile__(
        "lock xaddl %0, %1"
        : "=r"(Result), "=m"(*Addend)
        : "0"(Value), "m"(*Addend)
        : "memory"
    );
    return Result;
}

extern "C" DWORD __stdcall SetCriticalSectionSpinCount(
    LPCRITICAL_SECTION lpCriticalSection, DWORD dwSpinCount)
{
    (void)lpCriticalSection;
    return dwSpinCount;
}

extern "C" BOOL __stdcall TryEnterCriticalSection(
    LPCRITICAL_SECTION lpCriticalSection)
{
    LONG* lockCount = (LONG*)((char*)lpCriticalSection + sizeof(void*));
    LONG prev;
    __asm__ __volatile__(
        "lock cmpxchgl %2, %1"
        : "=a"(prev), "=m"(*lockCount)
        : "r"(0L), "a"(-1L), "m"(*lockCount)
        : "memory"
    );
    if (prev == -1) {
        ((LONG*)((char*)lpCriticalSection + sizeof(void*) * 2))[0] = 1;
        ((LONG*)((char*)lpCriticalSection + sizeof(void*) * 3))[0] = (LONG)GetCurrentThreadId();
        return TRUE;
    }
    return FALSE;
}

extern "C" BOOL __stdcall GetHandleInformation(
    HANDLE hObject, LPDWORD lpdwFlags)
{
    (void)hObject;
    if (lpdwFlags) {
        *lpdwFlags = HANDLE_FLAG_INHERIT;
        return TRUE;
    }
    SetLastError(ERROR_INVALID_PARAMETER);
    return FALSE;
}

extern "C" BOOL __stdcall GetProcessAffinityMask(
    HANDLE hProcess, LPDWORD lpProcessAffinityMask, LPDWORD lpSystemAffinityMask)
{
    (void)hProcess;
    if (lpProcessAffinityMask) *lpProcessAffinityMask = 1;
    if (lpSystemAffinityMask) *lpSystemAffinityMask = 1;
    return TRUE;
}

extern "C" BOOL __stdcall SetProcessAffinityMask(
    HANDLE hProcess, DWORD dwProcessAffinityMask)
{
    (void)hProcess;
    (void)dwProcessAffinityMask;
    return TRUE;
}

extern "C" DWORD_PTR __stdcall SetThreadAffinityMask(
    HANDLE hThread, DWORD_PTR dwThreadAffinityMask)
{
    (void)hThread;
    (void)dwThreadAffinityMask;
    return 1;
}

extern "C" BOOL __stdcall GetCursorInfo(PCURSORINFO pci)
{
    if (!pci) { SetLastError(ERROR_INVALID_PARAMETER); return FALSE; }
    pci->flags = 1;
    pci->hCursor = GetCursor();
    GetCursorPos(&pci->ptScreenPos);
    return TRUE;
}

#endif
