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

typedef struct File_mapping {
    HANDLE hFile;
    HANDLE hMapping;
    size_t file_size;
    unsigned char *data_prt;
};

std::string replaceString( std::string subject, std::string const & search, std::string const & replace ) {
    size_t pos = 0;
    while ( ( pos = subject.find( search, pos ) ) != std::string::npos ) {
        subject.replace( pos, search.length(), replace );
        pos += replace.length();
    }
    return subject;
}

std::string getProcessPath(std::string process_name) {
    char buffer[MAX_PATH];
    GetModuleFileName(NULL,buffer, sizeof(buffer));
    std::string path = replaceString(std::string(buffer), MAIN_PROCESS_NAME, process_name);
    return path;
}

WINBOOL create_process(std::string process_name, Process_data *process_data) {
    return CreateProcess( NULL, (char*) getProcessPath(process_name).c_str(), NULL, NULL, FALSE, 0, NULL, NULL,
                          &(process_data->startupinfo),
                          &(process_data->process_information));
}

Process_data *initialize_process_data() {
    Process_data *process_data = (Process_data*) malloc(sizeof(Process_data));
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



File_mapping * initialise_file_mapping(char *file_name) {
    HANDLE hFile = CreateFile(file_name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        print_err_message("Create file err!");
        return NULL;
    }

    DWORD dw_file_size = GetFileSize(hFile, NULL);
    if (dw_file_size == INVALID_FILE_SIZE) {
        print_err_message("Get file size err!");
        return NULL;
    }

    HANDLE hMapping = CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, 0,
                                        nullptr);
    if(hMapping == nullptr) {
        print_err_message("Create file mapping err!");
        CloseHandle(hFile);
        return NULL;
    }

    unsigned char* dataPtr = (unsigned char*)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, dw_file_size);
    if(dataPtr == nullptr) {
        print_err_message("Create data prt err!");
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return nullptr;
    }

    File_mapping* mapping = (File_mapping*) malloc(sizeof(File_mapping));
    if(mapping == nullptr) {

        UnmapViewOfFile(dataPtr);
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return nullptr;
    }

    mapping->hFile = hFile;
    mapping->hMapping = hMapping;
    mapping->data_prt = dataPtr;
    mapping->file_size = (size_t)dw_file_size;

    return mapping;
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

    HANDLE file_mapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(int), "File.txt");

    LPVOID view_mapping = MapViewOfFile(file_mapping,FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

    int number;
    int iteration = 0;
    while(iteration < 10){
        std::cout<<"new iteration"<<std::endl;
        number = (rand());
        CopyMemory(view_mapping, &number, sizeof(int));
        std::cout << "P -- " <<  *static_cast<LPCWSTR>(view_mapping) << std::endl;

        SetEvent( eventToConsoleChild );
        SetEvent( eventToFileChild );
        WaitForMultipleObjects(2, childEvents,TRUE,INFINITE);
        printf( "Main ok.\n" );
        iteration++;
    }


    close_process_data(console_process_data);
    close_process_data(file_process_data);
    CloseHandle(file_mapping);
    CloseHandle(view_mapping);
}