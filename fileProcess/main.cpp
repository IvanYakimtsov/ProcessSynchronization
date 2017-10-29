#include <iostream>
#include <windows.h>

int main() {
    HANDLE eventFromFileChild = OpenEvent(EVENT_ALL_ACCESS, TRUE, "eventFromFileChild");
    HANDLE eventToChild = OpenEvent(EVENT_ALL_ACCESS, TRUE, "eventToChild");
    WaitForSingleObject(eventToChild,INFINITE);
    std::cout << "MINOR 2: OK" << std::endl;
    SetEvent(eventFromFileChild);
    return 0;
}