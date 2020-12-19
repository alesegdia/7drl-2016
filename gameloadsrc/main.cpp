#include <windows.h>
#include <iostream>

/* Define a function pointer for our imported
 * function.
 * This reads as "introduce the new type f_funci as the type:
 *                pointer to a function returning an int and
 *                taking no arguments.
 *
 * Make sure to use matching calling convention (__cdecl, __stdcall, ...)
 * with the exported function. __stdcall is the convention used by the WinAPI
 */
typedef int(__stdcall* f_funci)();

int main()
{
    std::cout << "hi1" << std::endl;
    HINSTANCE hGetProcIDDLL = LoadLibrary("C:\\Users\\Alejandro\\phackman\\build\\phackman-game.dll");

    std::cout << "hi2" << std::endl;
    if (!hGetProcIDDLL) {
        std::cout << "could not load the dynamic library" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "hi3" << std::endl;

    // resolve function address here
    f_funci funci = (f_funci)GetProcAddress(hGetProcIDDLL, "janio");
    if (!funci) {
        std::cout << "could not locate the function" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "funci() returned " << funci() << std::endl;
    return EXIT_SUCCESS;
}
