#pragma comment(lib, "Shlwapi.lib")

#include <Windows.h>
#include <Shlwapi.h>
#include <tchar.h>
#include <cstdio>

typedef struct
{
    const TCHAR * const pPartialTitle;
    const bool bCaseSentitive;
    HWND hFoundHandle;
} WindowInformation;

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
    //Read up to 255 characters of window title
    TCHAR strWindowTitle[255] = { 0 };

    auto success = GetWindowText(hWnd, strWindowTitle, sizeof(strWindowTitle));
    if (success > 0)
    {
        WindowInformation *pWindowInfo = (WindowInformation *)lParam;
        auto isFound = pWindowInfo->bCaseSentitive ?
            StrStr(strWindowTitle, pWindowInfo->pPartialTitle) :
            StrStrI(strWindowTitle, pWindowInfo->pPartialTitle);
        if (isFound)
        {
            pWindowInfo->hFoundHandle = hWnd;
            return FALSE;
        }
    }

    return TRUE;
}

const HWND FindWindowLike(const TCHAR * const pPartialTitle, const bool bCaseSensitive = true)
{
    WindowInformation windowInfo = { pPartialTitle, bCaseSensitive, nullptr };
    (void)EnumWindows(EnumWindowsProc, (LPARAM)&windowInfo);

    if (windowInfo.hFoundHandle == nullptr)
    {
        fprintf(stderr, "Could not find window.\n");
    }

    return windowInfo.hFoundHandle;
}

int main()
{
    auto i = FindWindowLike(L"Notepad");
    auto j = FindWindowLike(L"NoTEpad", false);
    printf("Window handle case sensitive: %X\n"
        "Window handle case insensitive: %X\n",
        i, j);

    return 0;
}