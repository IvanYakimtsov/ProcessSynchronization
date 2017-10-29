#include <iostream>
#include <windows.h>
#include <zconf.h>
#include <fstream>

int main() {
    HANDLE eventFromFileChild = OpenEvent(EVENT_ALL_ACCESS, TRUE, "eventFromFileChild");
    HANDLE eventToFileChild = OpenEvent(EVENT_ALL_ACCESS, TRUE, "eventToFileChild");

 //   fout.close();
    HANDLE file_mapping = OpenFileMappingA(FILE_MAP_READ, FALSE, "File.txt");
    LPVOID view_mapping = MapViewOfFile(file_mapping, FILE_MAP_READ, 0, 0, 0);
    while (true) {
        std::ofstream fout("log.txt");;
        WaitForSingleObject(eventToFileChild, INFINITE);
//        std::cout << "MINOR 2: OK" << std::endl;
        fout << "P2 --" << /* *static_cast<LPCWSTR>(view_mapping) <<*/ std::endl;
        fout.close();
        SetEvent(eventFromFileChild);
    }

    return 0;
}