#include <iostream>
#include <windows.h>

int main() {
    HANDLE eventFromConsoleChild = OpenEvent(EVENT_ALL_ACCESS, TRUE, "eventFromConsoleChild");
    HANDLE eventToChild = OpenEvent(EVENT_ALL_ACCESS, TRUE, "eventToChild");
    WaitForSingleObject(eventToChild,INFINITE);
    std::cout << "MINOR 1: OK" << std::endl;
    SetEvent(eventFromConsoleChild);
    return 0;
}