#include <iostream>
#include <windows.h>
#include <sstream>
#include <time.h>

#define MAIN_PROCESS_NAME "mainProcess"
#define CONSOLE_PROCESS_NAME "consoleProcess"
#define FILE_PROCESS_NAME "fileProcess"

#define MAPPING_FILE_NAME "File.txt"

#define MAIN_PROCESS_START_MESSAGE "Main process start!\n"
#define CREATE_PROCESS_ERR "Create process err!"
#define ITERATION_MESSAGE "Iteration number %d\n"
#define RANDOM_RESULT_MESSAGE "Main process -- %d\n"

#define EVENT_TO_CONSOLE "eventToConsole"
#define EVENT_TO_FILE "eventToFile"
#define EVENT_FROM_CONSOLE "eventFromConsole"
#define EVENT_FROM_FILE "eventFromFile"

#define MAX_NUMBER 9000

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

void close_process_data(Process_data *process_data) {
    CloseHandle(process_data->process_information.hProcess);
    CloseHandle(process_data->process_information.hThread);
}

void print_err_message(std::string message) {
    std::cerr << message << std::endl;
}

int main() {
    srand(time(NULL));
    HANDLE eventFromConsole = CreateEvent(NULL, FALSE, FALSE, EVENT_FROM_CONSOLE);
    HANDLE eventFromFile = CreateEvent(NULL, FALSE, FALSE, EVENT_FROM_FILE);
    HANDLE eventToConsole = CreateEvent(NULL, FALSE, FALSE, EVENT_TO_CONSOLE);
    HANDLE eventToFile = CreateEvent(NULL, FALSE, FALSE, EVENT_TO_FILE);

    HANDLE events[2];
    events[0] = eventFromConsole;
    events[1] = eventFromFile;

    printf(MAIN_PROCESS_START_MESSAGE);

    Process_data *console_process_data = initialize_process_data();
    Process_data *file_process_data = initialize_process_data();

    if (create_process(CONSOLE_PROCESS_NAME, console_process_data)
        && create_process(FILE_PROCESS_NAME, file_process_data)) {

        HANDLE file_mapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
                                                0, sizeof(int), MAPPING_FILE_NAME);

        unsigned char *view_mapping = (unsigned char *) MapViewOfFile(file_mapping, FILE_MAP_READ | FILE_MAP_WRITE, 0,
                                                                      0, 0);
        int number;
        char *str;
        for (int iteration = 0; iteration < 1000; iteration++) {
            printf(ITERATION_MESSAGE, iteration);
            number = (rand() % MAX_NUMBER);
            sprintf(str, "%d", number);
            CopyMemory(view_mapping, str, sizeof(int));
            printf(RANDOM_RESULT_MESSAGE, number);
            SetEvent(eventToConsole);
            SetEvent(eventToFile);
            WaitForMultipleObjects(2, events, TRUE, INFINITE);
            printf("----------------\n");
        }

        close_process_data(console_process_data);
        close_process_data(file_process_data);
        CloseHandle(file_mapping);
        CloseHandle(view_mapping);
    } else {
        print_err_message(CREATE_PROCESS_ERR);
    }
    return 0;
}