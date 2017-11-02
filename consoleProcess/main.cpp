#include <iostream>
#include <windows.h>

#define MAPPING_FILE_NAME "File.txt"

#define EVENT_TO_CONSOLE "eventToConsole"
#define EVENT_FROM_CONSOLE "eventFromConsole"

#define RANDOM_RESULT_MESSAGE "Console process -- %c\n"


int main() {
    HANDLE semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "mainSemaphore");

    HANDLE file_mapping = OpenFileMapping(FILE_MAP_READ, FALSE, MAPPING_FILE_NAME);
    unsigned char* dataPtr = (unsigned char *) MapViewOfFile(file_mapping, FILE_MAP_READ, 0, 0, 0);

    while (true){
        WaitForSingleObject(semaphore,INFINITE);
//        Sleep(20);
        std::cout << "Console process -- " << dataPtr << std::endl;
        ReleaseSemaphore(semaphore, 1, NULL);
//        std::cout<<"CONSOLE"<<std::endl;
    }
    return 0;
}

