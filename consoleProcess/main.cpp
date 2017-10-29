#include <iostream>
#include <windows.h>
#include <zconf.h>

int main() {
    HANDLE eventFromConsoleChild = OpenEvent(EVENT_ALL_ACCESS, TRUE, "eventFromConsoleChild");
    HANDLE eventToConsoleChild = OpenEvent(EVENT_ALL_ACCESS, TRUE, "eventToConsoleChild");

    HANDLE file_mapping = OpenFileMappingA(FILE_MAP_READ, FALSE, "File.txt");
    LPVOID view_mapping = MapViewOfFile(file_mapping, FILE_MAP_READ, 0, 0, 0);

    while (true){
        WaitForSingleObject(eventToConsoleChild,INFINITE);
        std::cout << "P1 --" <<  *static_cast<LPCWSTR>(view_mapping) << std::endl;

//        std::cout << "MINOR 1: OK" << std::endl;
        SetEvent(eventFromConsoleChild);
    }
    return 0;
}