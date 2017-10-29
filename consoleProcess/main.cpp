#include <iostream>
#include <windows.h>

int main() {
    HANDLE eventFromConsoleChild = OpenEvent(EVENT_ALL_ACCESS, TRUE, "eventFromConsoleChild");
    HANDLE eventToConsoleChild = OpenEvent(EVENT_ALL_ACCESS, TRUE, "eventToConsoleChild");
    while (true){
        //TODO: read from shared buffer here
        WaitForSingleObject(eventToConsoleChild,INFINITE);
        std::cout << "MINOR 1: OK" << std::endl;
        SetEvent(eventFromConsoleChild);
    }
    return 0;
}