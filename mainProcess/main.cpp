#include <iostream>
#include <windows.h>
#include <sstream>
LPSTR getProcessPath(std::string process_name) {
    std::stringstream stream;
    char buffer[MAX_PATH];
    GetModuleFileName(NULL,buffer, sizeof(buffer));
    stream << buffer;
    std::string path = stream.str();
    unsigned long position = path.find("mainProcess");
    path = path.replace(position, path.length(), process_name + "\\cmake-build-debug\\" + process_name + ".exe");
    return const_cast<char *>(path.c_str());
}

int main() {
    HANDLE eventFromChild = CreateEvent( NULL, FALSE, FALSE, "eventFromChild");
    HANDLE eventToChild = CreateEvent( NULL, FALSE, FALSE, "eventToChild");

    STARTUPINFO startupinfo;
    PROCESS_INFORMATION process_information;

    std::cout << getProcessPath("consoleProcess");

    ZeroMemory( &startupinfo, sizeof(startupinfo) );
    startupinfo.cb = sizeof(startupinfo);
    ZeroMemory( &process_information, sizeof(process_information) );

    printf("Main process running 1000 ms\n");
    Sleep(1000);

    if( !CreateProcess( NULL,
                        getProcessPath("consoleProcess"),
                        NULL,
                        NULL,
                        FALSE,
                        0,
                        NULL,
                        NULL,
                        &startupinfo,
                        &process_information )
            )

        printf( "CreateProcess failed.\n" );

    SetEvent( eventToChild );

    WaitForSingleObject( eventFromChild, INFINITE );
    printf( "Main ok.\n" );

    CloseHandle( process_information.hProcess );
    CloseHandle( process_information.hThread );
}