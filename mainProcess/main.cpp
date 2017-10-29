#include <iostream>
#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE eventFromChild = CreateEvent( NULL, FALSE, FALSE, "eventFromChild");
    HANDLE eventToChild = CreateEvent( NULL, FALSE, FALSE, "eventToChild");

    STARTUPINFO StartupInfo;
    PROCESS_INFORMATION ProcInfo;
    TCHAR CommandLine[] = TEXT("sleep");


    ZeroMemory( &StartupInfo, sizeof(StartupInfo) );
    StartupInfo.cb = sizeof(StartupInfo);
    ZeroMemory( &ProcInfo, sizeof(ProcInfo) );

    printf("Main process running 1000 ms\n");
    Sleep(1000);

    if( !CreateProcess( NULL, // Не используется имя модуля
                        "D:\\ProjectsC\\MultiProcess\\consoleProcess\\cmake-build-debug\\consoleProcess.exe",   // Командная строка
                        NULL,                 // Дескриптор процесса не наследуется.
                        NULL,                 // Дескриптор потока не наследуется.
                        FALSE,                // Установка описателей наследования
                        0,                    // Нет флагов создания процесса
                        NULL,                 // Блок переменных окружения родительского процесса
                        NULL,                 // Использовать текущий каталог родительского процесса
                        &StartupInfo,         // Указатель на структуру  STARTUPINFO.
                        &ProcInfo )           // Указатель на структуру информации о процессе.
            )

        printf( "CreateProcess failed.\n" );

    SetEvent( eventToChild );
    // Ждать окончания дочернего процесса
   // WaitForSingleObject( ProcInfo.hProcess, INFINITE );
    WaitForSingleObject( eventFromChild, INFINITE );
    printf( "Main ok.\n" );

    // Закрыть описатели процесса и потока
    CloseHandle( ProcInfo.hProcess );
    CloseHandle( ProcInfo.hThread );
}