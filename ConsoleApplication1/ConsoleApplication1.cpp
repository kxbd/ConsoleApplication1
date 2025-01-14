﻿#define UNICODE 1
#define _UNICODE 1

/* The code of interest is in the subroutine GetDriveGeometry. The
   code in main shows how to interpret the results of the call. */

#include <windows.h>
#include <winioctl.h>
#include <stdio.h>

#define wszDrive L"\\\\.\\PhysicalDrive0"

BOOL GetDriveGeometry(LPWSTR wszPath, DISK_GEOMETRY* pdg)
{
    HANDLE hDevice = INVALID_HANDLE_VALUE;  // handle to the drive to be examined 
    BOOL bResult = FALSE;                 // results flag
    DWORD junk = 0;                     // discard results
    BYTE buffer[512];
    hDevice = CreateFileW(wszPath,          // drive to open
        GENERIC_READ,                // no access to the drive
        FILE_SHARE_READ | // share mode
        FILE_SHARE_WRITE,
        NULL,             // default security attributes
        OPEN_EXISTING,    // disposition
        0,                // file attributes
        NULL);            // do not copy file attributes

    if (hDevice == INVALID_HANDLE_VALUE)    // cannot open the drive
    {
        return (FALSE);
    }

    bResult = DeviceIoControl(hDevice,                       // device to be queried
        IOCTL_DISK_GET_DRIVE_GEOMETRY, // operation to perform
        NULL, 0,                       // no input buffer
        pdg, sizeof(*pdg),            // output buffer
        &junk,                         // # bytes returned
        (LPOVERLAPPED)NULL);          // synchronous I/O
    if (bResult)
    {
        bResult = ReadFile(hDevice, buffer, sizeof(buffer), &junk, NULL);
        if (bResult && junk == sizeof(buffer))
        {
            for (DWORD i = 0; i < junk; i++)
            {
                printf("%02X ", buffer[i]);
            }
        }
        printf("\n");
    }

    CloseHandle(hDevice);

    return (bResult);
}

int wmain(int argc, wchar_t* argv[])
{
    DISK_GEOMETRY pdg = { 0 }; // disk drive geometry structure
    BOOL bResult = FALSE;      // generic results flag
    ULONGLONG DiskSize = 0;    // size of the drive, in bytes
    wchar_t wszDrie[] = L"\\\\.\\PhysicalDrive0";
    bResult = GetDriveGeometry(wszDrie, &pdg);
    if (bResult)
    {
        wprintf(L"Drive path      = %ws\n", wszDrive);
        wprintf(L"Cylinders       = %I64d\n", pdg.Cylinders);
        wprintf(L"Tracks/cylinder = %ld\n", (ULONG)pdg.TracksPerCylinder);
        wprintf(L"Sectors/track   = %ld\n", (ULONG)pdg.SectorsPerTrack);
        wprintf(L"Bytes/sector    = %ld\n", (ULONG)pdg.BytesPerSector);

        DiskSize = pdg.Cylinders.QuadPart * (ULONG)pdg.TracksPerCylinder *
            (ULONG)pdg.SectorsPerTrack * (ULONG)pdg.BytesPerSector;
        wprintf(L"Disk size       = %I64d (Bytes)\n"
            L"                = %.2f (Gb)\n",
            DiskSize, (double)DiskSize / (1024 * 1024 * 1024));
    }
    else
    {
        wprintf(L"GetDriveGeometry failed. Error %ld.\n", GetLastError());
    }

    return ((int)bResult);
}