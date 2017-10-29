#include <iostream>
#include <windows.h>




int main() {
    HANDLE eventFromConsoleChild = CreateEvent( NULL, FALSE, FALSE, "eventFromConsoleChild");
    HANDLE eventFromFileChild = CreateEvent( NULL, FALSE, FALSE, "eventFromFileChild");
    HANDLE eventToConsoleChild = CreateEvent( NULL, FALSE, FALSE, "eventToConsoleChild");
    HANDLE eventToFileChild = CreateEvent( NULL, FALSE, FALSE, "eventToFileChild");

    HANDLE childEvents[2];
    childEvents[0] = eventFromConsoleChild;
    childEvents[1] = eventFromFileChild;

    std::cout<<"start"<<std::endl;

    STARTUPINFO StartupInfo1;
    PROCESS_INFORMATION ProcInfo1;

    ZeroMemory( &StartupInfo1, sizeof(StartupInfo1) );
    StartupInfo1.cb = sizeof(StartupInfo1);
    ZeroMemory( &ProcInfo1, sizeof(ProcInfo1) );


    if( !CreateProcess( NULL, // Не используется имя модуля
                        "D:\\ProjectsC\\ProcessSynchronization\\consoleProcess\\cmake-build-debug\\consoleProcess.exe",   // Командная строка
                        NULL,                 // Дескриптор процесса не наследуется.
                        NULL,                 // Дескриптор потока не наследуется.
                        FALSE,                // Установка описателей наследования
                        0,                    // Нет флагов создания процесса
                        NULL,                 // Блок переменных окружения родительского процесса
                        NULL,                 // Использовать текущий каталог родительского процесса
                        &StartupInfo1,         // Указатель на структуру  STARTUPINFO.
                        &ProcInfo1 )           // Указатель на структуру информации о процессе.
            ) printf( "CreateProcess failed.\n" );



    STARTUPINFO StartupInfo2;
    PROCESS_INFORMATION ProcInfo2;


    ZeroMemory( &StartupInfo2, sizeof(StartupInfo2) );
    StartupInfo2.cb = sizeof(StartupInfo2);
    ZeroMemory( &ProcInfo2, sizeof(ProcInfo2) );


    if( !CreateProcess( NULL, // Не используется имя модуля
                        "D:\\ProjectsC\\ProcessSynchronization\\fileProcess\\cmake-build-debug\\fileProcess.exe",   // Командная строка
                        NULL,                 // Дескриптор процесса не наследуется.
                        NULL,                 // Дескриптор потока не наследуется.
                        FALSE,                // Установка описателей наследования
                        0,                    // Нет флагов создания процесса
                        NULL,                 // Блок переменных окружения родительского процесса
                        NULL,                 // Использовать текущий каталог родительского процесса
                        &StartupInfo2,         // Указатель на структуру  STARTUPINFO.
                        &ProcInfo2 )           // Указатель на структуру информации о процессе.
            ) printf( "CreateProcess failed.\n" );



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



    // Закрыть описатели процесса и потока
    CloseHandle( ProcInfo1.hProcess );
    CloseHandle( ProcInfo1.hThread );

    CloseHandle( ProcInfo2.hProcess );
    CloseHandle( ProcInfo2.hThread );
}