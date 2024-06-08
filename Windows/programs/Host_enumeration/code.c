#include <Windows.h>
#include <stdio.h>
#include <Psapi.h>
#include <LM.h>

typedef NTSYSAPI NTSTATUS(WINAPI* procRtlGetVersion)(PRTL_OSVERSIONINFOW lpVersionInformation);
#define STATUS_SUCCESS 0x00000000

int main() {
    // Get the username 
    {
        char userName[UNLEN + 1];
        DWORD userNameLength = UNLEN + 1;

        if (GetUserNameA(userName, &userNameLength)) {
            printf("User: %s\n", userName);
        }
        else {
            printf("[-] Error: %d\n", GetLastError());
        }
    }

    // Get  hostname
    {
        char hostName[UNLEN + 1];
        DWORD hostNameLength = UNLEN + 1;

        if (GetComputerNameExA(ComputerNameDnsFullyQualified, hostName, &hostNameLength)) {
            printf("Hostname: %s\n", hostName);
        }
        else {
            printf("[-] Error: %d\n", GetLastError());
        }
    }

    // Get the OS version
    {
        RTL_OSVERSIONINFOW osversioninfow = { 0 };
        HMODULE hntdll = GetModuleHandleA("ntdll.dll");
        procRtlGetVersion myRtlGetVersion = (procRtlGetVersion)GetProcAddress(hntdll, "RtlGetVersion");
        if (myRtlGetVersion(&osversioninfow) == STATUS_SUCCESS) {
            printf("Windows Version: %lu.%lu\n", osversioninfow.dwMajorVersion, osversioninfow.dwMinorVersion);
            printf("Windows Build: %lu\n", osversioninfow.dwBuildNumber);
        }
        else {
            printf("[-] Error: %d\n", GetLastError());
        }
    }

    // Get the current directory
    {
        char currentDir[MAX_PATH + 1];
        DWORD currentDirLength = MAX_PATH + 1;

        if (GetCurrentDirectoryA(currentDirLength, currentDir) != 0) {
            printf("Current Directory: %s\n", currentDir);
        }
        else {
            printf("[-] Error: %d\n", GetLastError());
        }
    }

    // Get the system uptime
    {
        DWORD tickCount = (DWORD)GetTickCount64();
        DWORD dcuptime = ((tickCount / 1000) / 60);
        printf("Uptime: %lu minutes\n", dcuptime);
    }

    // Enumerate device drivers
    {
        LPVOID drivers[1024];
        DWORD cbNeeded;
        int cDrivers, i;

        if (EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded) && cbNeeded < sizeof(drivers)) {
            CHAR szDriver[1024];
            cDrivers = cbNeeded / sizeof(drivers[0]);

            printf("[+] %d drivers found\n", cDrivers);
            for (i = 0; i < cDrivers; i++) {
                if (GetDeviceDriverBaseNameA(drivers[i], szDriver, sizeof(szDriver) / sizeof(szDriver[0]))) {
                    printf("[.] %s => %p\n", szDriver, drivers[i]);
                }
                else {
                    printf("[-] E: %d\n", GetLastError());
                }
            }
        }
    }

    // Enumerate services
    {
        SC_HANDLE hRemoteSvcManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
        if (hRemoteSvcManager == NULL) {
            printf("[-] E: %d\n", GetLastError());
        }
        else {
            DWORD BytesNeeded;
            DWORD NumberOfServices;
            DWORD ResumeHandleSize = 0;
            DWORD ServiceStructSize;
            LPENUM_SERVICE_STATUSA lpServiceEnumStruct;

            INT errVal = EnumServicesStatusA(hRemoteSvcManager, SERVICE_WIN32, SERVICE_STATE_ALL, NULL, 0, &BytesNeeded, &NumberOfServices, &ResumeHandleSize);
            if ((errVal == ERROR_INVALID_HANDLE) || (errVal == ERROR_ACCESS_DENIED) || (errVal == ERROR_INVALID_PARAMETER)) {
                printf("[-] E: %d\n", GetLastError());
            }
            else {
                ServiceStructSize = BytesNeeded + sizeof(ENUM_SERVICE_STATUSA);
                lpServiceEnumStruct = (LPENUM_SERVICE_STATUSA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ServiceStructSize);
                if (lpServiceEnumStruct == NULL) {
                    printf("[-] E: %d\n", GetLastError());
                }
                else {
                    errVal = EnumServicesStatusA(hRemoteSvcManager, SERVICE_WIN32, SERVICE_STATE_ALL, lpServiceEnumStruct, ServiceStructSize, &BytesNeeded, &NumberOfServices, &ResumeHandleSize);
                    if ((errVal != 0)) {
                        printf("\n\n[+] Enumerating Services [%lu]\n", NumberOfServices);
                        for (int i = 0; i < (int)NumberOfServices; i++) {
                            SC_HANDLE hSvc = OpenServiceA(hRemoteSvcManager, lpServiceEnumStruct[i].lpServiceName, SERVICE_QUERY_CONFIG);
                            if (hSvc != NULL) {
                                LPQUERY_SERVICE_CONFIGA lpServiceConfig;
                                DWORD scbytesNeeded;
                                DWORD ScdwBytes;
                                QueryServiceConfigA(hSvc, NULL, 0, &scbytesNeeded);
                                ScdwBytes = scbytesNeeded;
                                lpServiceConfig = (LPQUERY_SERVICE_CONFIGA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ScdwBytes);
                                if (lpServiceConfig != NULL) {
                                    errVal = QueryServiceConfigA(hSvc, lpServiceConfig, ScdwBytes, &scbytesNeeded);

                                    if (errVal != 0) {
                                        printf("[+] Display Name: %s\n", lpServiceEnumStruct[i].lpDisplayName);
                                        printf("[.] Service Name: %s\n", lpServiceEnumStruct[i].lpServiceName);
                                        printf("[.] Service State: %lu\n", lpServiceEnumStruct[i].ServiceStatus.dwCurrentState);
                                        printf("[.] Service Path: %s\n", lpServiceConfig->lpBinaryPathName);
                                        printf("[.] Service User: %s\n", lpServiceConfig->lpServiceStartName);
                                        printf("[.] Service Type: %lu\n\n", lpServiceEnumStruct[i].ServiceStatus.dwServiceType);
                                        HeapFree(GetProcessHeap(), 0, lpServiceConfig);
                                        CloseServiceHandle(hSvc);
                                    }
                                    else {
                                        printf("[-] E: %d\n", GetLastError());
                                        HeapFree(GetProcessHeap(), 0, lpServiceConfig);
                                        CloseServiceHandle(hSvc);
                                    }
                                }
                            }
                            else {
                                printf("[-] E: %d\n", GetLastError());
                            }
                        }
                    }
                    else {
                        printf("[-] E: %d\n", GetLastError());
                    }
                    HeapFree(GetProcessHeap(), 0, lpServiceEnumStruct);
                }
            }
            CloseServiceHandle(hRemoteSvcManager);
        }
    }

    return 0;
}
