#include <iostream>
#include <windows.h>
#include <fstream>
#include <sstream>

#define MAPPING_FILE_NAME "File.txt"
#define LOG_FILE_NAME "log.txt"

#define RANDOM_RESULT_MESSAGE "File process -- iteration number %d -- random number %c\n"

#define EVENT_TO_FILE "eventToFile"
#define EVENT_FROM_FILE "eventFromFile"

int main() {
    HANDLE semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, "mainSemaphore");

    HANDLE eventFromFile = OpenEvent(EVENT_ALL_ACCESS, TRUE, EVENT_FROM_FILE);
    HANDLE eventToFile = OpenEvent(EVENT_ALL_ACCESS, TRUE, EVENT_TO_FILE);

    HANDLE file_mapping = OpenFileMappingA(FILE_MAP_READ, FALSE, MAPPING_FILE_NAME);
    unsigned char* view_mapping = (unsigned char *) MapViewOfFile(file_mapping, FILE_MAP_READ, 0, 0, 0);

    std::ofstream file_out (LOG_FILE_NAME);
    int iteration = 0;
    bool active = true;
    LONG count = 0;
    while (true) {
        WaitForSingleObject(semaphore, INFINITE);
//        Sleep(20);
        file_out<<"File process -- iteration number "<<iteration<<" -- RANDOM NUMBER -- "<<view_mapping<<std::endl;
        iteration++;
//        ReleaseSemaphore(semaphore, 1, &count);
//        std::cout << "FILE" << std::endl;
    }

    return 0;
}