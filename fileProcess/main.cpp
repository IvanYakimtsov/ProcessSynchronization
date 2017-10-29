#include <iostream>
#include <windows.h>
#include <zconf.h>

int main() {
    HANDLE eventFromFileChild = OpenEvent(EVENT_ALL_ACCESS, TRUE, "eventFromFileChild");
    HANDLE eventToFileChild = OpenEvent(EVENT_ALL_ACCESS, TRUE, "eventToFileChild");
    while (true) {
        WaitForSingleObject(eventToFileChild, INFINITE);
//        std::cout << "MINOR 2: OK" << std::endl;
        SetEvent(eventFromFileChild);
    }
    return 0;
}