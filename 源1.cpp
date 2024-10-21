#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
int main() {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pe32.dwSize = sizeof(PROCESSENTRY32);
    // Retrieve information about the first process
    Process32First(hProcessSnap, &pe32);
    // Now walk the snapshot of processes, and display information about each process in turn
    do {
        _tprintf(TEXT("\nProcessID = 0x%08X"), pe32.th32ProcessID);
        _tprintf(TEXT("\nThread count = %d"), pe32.cntThreads);
        _tprintf(TEXT("\nParent processID = 0x%08X"), pe32.th32ParentProcessID);
        _tprintf(TEXT("\nPriority base = %d"), pe32.pcPriClassBase);
        _tprintf(TEXT("\nProcess Name: %s"), pe32.szExeFile);
    } while (Process32Next(hProcessSnap, &pe32));

    return 0;
}