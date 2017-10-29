#include <iostream>
#include <windows.h>

int main() {
    HANDLE eventFromChild = OpenEvent(EVENT_ALL_ACCESS, TRUE, "eventFromChild");
    HANDLE eventToChild = OpenEvent(EVENT_ALL_ACCESS, TRUE, "eventToChild");
    WaitForSingleObject(eventToChild,INFINITE);
    std::cout << "MINOR: OK" << std::endl;
    SetEvent(eventFromChild);
    return 0;
}