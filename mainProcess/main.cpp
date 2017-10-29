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

LPSTR getProcessPath(std::string process_name) {
    std::stringstream stream;
    char buffer[MAX_PATH];
    GetModuleFileName(NULL,buffer, sizeof(buffer));
    stream << buffer;
    std::string path = stream.str();
    unsigned long position = path.find(MAIN_PROCESS_NAME);
    path = path.replace(position, path.length(), process_name + "\\cmake-build-debug\\" + process_name + ".exe");
    return const_cast<char *>(path.c_str());
}

WINBOOL create_process(std::string process_name, Process_data *process_data) {
    return CreateProcess( NULL, getProcessPath(process_name), NULL, NULL, FALSE, 0, NULL, NULL,
                          &process_data->startupinfo,
                          &process_data->process_information);
}

Process_data * initialize_process_data() {
    Process_data *process_data = new Process_data;
    ZeroMemory(&process_data->startupinfo, sizeof(process_data->startupinfo));
    ZeroMemory(&process_data->process_information, sizeof(process_data->process_information));
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
    HANDLE eventFromConsoleChild = CreateEvent( NULL, FALSE, FALSE, "eventFromConsoleChild");
    HANDLE eventFromFileChild = CreateEvent( NULL, FALSE, FALSE, "eventFromFileChild");
    HANDLE eventToConsoleChild = CreateEvent( NULL, FALSE, FALSE, "eventToConsoleChild");
    HANDLE eventToFileChild = CreateEvent( NULL, FALSE, FALSE, "eventToFileChild");

    HANDLE childEvents[2];
    childEvents[0] = eventFromConsoleChild;
    childEvents[1] = eventFromFileChild;

    std::cout<<"start"<<std::endl;

    Process_data *console_process_data = initialize_process_data();
    Process_data *file_process_data = initialize_process_data();

    if (!create_process(CONSOLE_PROCESS_NAME, console_process_data)) {
        print_err_message(CREATE_PROCESS_ERR);
    }

    if (!create_process(FILE_PROCESS_NAME, file_process_data)) {
        print_err_message(CREATE_PROCESS_ERR);
    }


    // Ждать окончания дочернего процесса
   // WaitForSingleObject( ProcInfo.hProcess, INFINITE );
    //WaitForSingleObject( eventFromConsoleChild, INFINITE );
    int iteration = 0;
    while(iteration < 10){
        std::cout<<"new iteration"<<std::endl;
        //TODO: add shared buffer here
        SetEvent( eventToConsoleChild );
        SetEvent( eventToFileChild );
        WaitForMultipleObjects(2, childEvents,TRUE,INFINITE);
        printf( "Main ok.\n" );
        iteration++;
    }


    close_process_data(console_process_data);
    close_process_data(file_process_data);
}