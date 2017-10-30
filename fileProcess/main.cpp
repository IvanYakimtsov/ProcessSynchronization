#include <iostream>
#include <windows.h>
#include <fstream>
#include <sstream>

int main() {
    HANDLE eventFromFileChild = OpenEvent(EVENT_ALL_ACCESS, TRUE, "eventFromFileChild");
    HANDLE eventToFileChild = OpenEvent(EVENT_ALL_ACCESS, TRUE, "eventToFileChild");

    HANDLE file_mapping = OpenFileMappingA(FILE_MAP_READ, FALSE, "File.txt");
    unsigned char* view_mapping = (unsigned char *) MapViewOfFile(file_mapping, FILE_MAP_READ, 0, 0, 0);
    std::ofstream fout;
    fout.open("log.txt");

    while (true) {

        WaitForSingleObject(eventToFileChild, INFINITE);
        fout<<view_mapping<<std::endl;

        SetEvent(eventFromFileChild);
    }

    return 0;
}