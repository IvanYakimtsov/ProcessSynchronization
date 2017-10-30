#include <iostream>
#include <windows.h>
#include <sstream>

#define MAIN_PROCESS_NAME "mainProcess"
#define CONSOLE_PROCESS_NAME "consoleProcess"
#define FILE_PROCESS_NAME "fileProcess"
#define CREATE_PROCESS_ERR "Create process err!"

typedef struct Process_data {
    STARTUPINFO startupinfo;
    PROCESS_INFORMATION process_information;
} Process_data;


std::string replaceString(std::string subject, std::string const &search, std::string const &replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}

std::string getProcessPath(std::string process_name) {
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, sizeof(buffer));
    std::string path = replaceString(std::string(buffer), MAIN_PROCESS_NAME, process_name);
    return path;
}

WINBOOL create_process(std::string process_name, Process_data *process_data) {
    return CreateProcess(NULL, (char *) getProcessPath(process_name).c_str(), NULL, NULL, FALSE, 0, NULL, NULL,
                         &(process_data->startupinfo),
                         &(process_data->process_information));
}

Process_data *initialize_process_data() {
    Process_data *process_data = (Process_data *) malloc(sizeof(Process_data));
    ZeroMemory(&(process_data->startupinfo), sizeof(process_data->startupinfo));
    ZeroMemory(&(process_data->process_information), sizeof(process_data->process_information));
    process_data->startupinfo.cb = sizeof(process_data->startupinfo);
    return process_data;
}

void print_err_message(std::string message) {
    std::cerr << message << std::endl;
}

void close_process_data(Process_data *process_data) {
    CloseHandle(process_data->process_information.hProcess);
    CloseHandle(process_data->process_information.hThread);
}



int main() {
    HANDLE eventFromConsoleChild = CreateEvent(NULL, FALSE, FALSE, "eventFromConsoleChild");
    HANDLE eventFromFileChild = CreateEvent(NULL, FALSE, FALSE, "eventFromFileChild");
    HANDLE eventToConsoleChild = CreateEvent(NULL, FALSE, FALSE, "eventToConsoleChild");
    HANDLE eventToFileChild = CreateEvent(NULL, FALSE, FALSE, "eventToFileChild");

    HANDLE childEvents[2];
    childEvents[0] = eventFromConsoleChild;
    childEvents[1] = eventFromFileChild;

    std::cout << "start" << std::endl;

    Process_data *console_process_data = initialize_process_data();
    Process_data *file_process_data = initialize_process_data();


    if (create_process(CONSOLE_PROCESS_NAME, console_process_data)
        && create_process(FILE_PROCESS_NAME, file_process_data)) {
        HANDLE file_mapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(int), "File.txt");
        unsigned char *view_mapping = (unsigned char *) MapViewOfFile(file_mapping, FILE_MAP_READ | FILE_MAP_WRITE, 0,
                                                                      0, 0);
        int number;
        int iteration = 0;
        while (iteration < 10) {
            std::cout << "new iteration" << std::endl;
            number = (rand());
            char *str;
            sprintf(str, "%d", number);
            CopyMemory(view_mapping, str, sizeof(int));
            std::cout << "P -- " <<  view_mapping << std::endl;

            SetEvent(eventToConsoleChild);
            SetEvent(eventToFileChild);
            WaitForMultipleObjects(2, childEvents, TRUE, INFINITE);
            printf("Main ok.\n");
            iteration++;
        }


        close_process_data(console_process_data);
        close_process_data(file_process_data);
        CloseHandle(file_mapping);
        CloseHandle(view_mapping);
    } else print_err_message(CREATE_PROCESS_ERR);
}