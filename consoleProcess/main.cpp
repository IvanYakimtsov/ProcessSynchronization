#include <iostream>
#include <windows.h>

#define MAPPING_FILE_NAME "File.txt"

#define EVENT_TO_CONSOLE "eventToConsole"
#define EVENT_FROM_CONSOLE "eventFromConsole"

#define RANDOM_RESULT_MESSAGE "Console process -- %c\n"


int main() {
    HANDLE eventFromConsole = OpenEvent(EVENT_ALL_ACCESS, FALSE, EVENT_FROM_CONSOLE);
    HANDLE eventToConsole = OpenEvent(EVENT_ALL_ACCESS, FALSE, EVENT_TO_CONSOLE);

    HANDLE file_mapping = OpenFileMapping(FILE_MAP_READ, FALSE, MAPPING_FILE_NAME);
    unsigned char* dataPtr = (unsigned char *) MapViewOfFile(file_mapping, FILE_MAP_READ, 0, 0, 0);

    while (true){
        WaitForSingleObject(eventToConsole,INFINITE);
        printf(RANDOM_RESULT_MESSAGE, dataPtr);
        SetEvent(eventFromConsole);
    }
    return 0;
}

