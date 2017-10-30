#include <iostream>
#include <windows.h>
#include <zconf.h>
#include <codecvt>
#include <tchar.h>

int main() {
    HANDLE eventFromConsoleChild = OpenEvent(EVENT_ALL_ACCESS, FALSE, "eventFromConsoleChild");
    HANDLE eventToConsoleChild = OpenEvent(EVENT_ALL_ACCESS, FALSE, "eventToConsoleChild");

    HANDLE file_mapping = OpenFileMapping(FILE_MAP_READ, FALSE, "File.txt");
    unsigned char* dataPtr = (unsigned char *) MapViewOfFile(file_mapping, FILE_MAP_READ, 0, 0, 0);


    while (true){
        WaitForSingleObject(eventToConsoleChild,INFINITE);

        std::cout << "P1 --" << dataPtr << std::endl;

        SetEvent(eventFromConsoleChild);
    }
    return 0;
}

