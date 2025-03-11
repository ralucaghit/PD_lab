#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

void QueryKey(HKEY hKey)
{
    TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
    DWORD    cbName = 0;                   // size of name string 
    TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class string 
    DWORD    cSubKeys = 0;               // number of subkeys 
    DWORD    cbMaxSubKey = 0;              // longest subkey size 
    DWORD    cchMaxClass = 0;              // longest class string 
    DWORD    cValues = 0;              // number of values for key 
    DWORD    cchMaxValue = 0;          // longest value name 
    DWORD    cbMaxValueData = 0;       // longest value data 
    DWORD    cbSecurityDescriptor = 0; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 

    DWORD i = 0, j = 0, retCode = 0;

    TCHAR  achValue[MAX_VALUE_NAME] = { '\0' };
    DWORD cchValue = MAX_VALUE_NAME;

    // Get the class name and the value count. 
    retCode = ::RegQueryInfoKey(
        hKey,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime);       // last write time 



    // Enumerate the subkeys, until RegEnumKeyEx fails.

    if (cSubKeys)
    {
        printf("\nNumber of subkeys: %d\n", cSubKeys);

        for (i = 0; i < cSubKeys; i++)
        {
            cbName = MAX_KEY_LENGTH;
            retCode = ::RegEnumKeyEx(hKey, i,
                achKey,
                &cbName,
                NULL,
                NULL,
                NULL,
                &ftLastWriteTime);

            if (retCode == ERROR_SUCCESS)
            {
                // Try to open the subkey
                HKEY hSubKey;
                if (RegOpenKeyEx(hKey, achKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
                {
                    // Check if the "ImagePath" value exists
                    retCode = ::RegQueryValueEx(hSubKey, TEXT("ImagePath"), NULL, NULL, (LPBYTE)achValue, &cchValue);

                    if (retCode == ERROR_SUCCESS)
                    {
                        _tprintf(TEXT("ImagePath: %s\n"), achValue);
                        //_tprintf(TEXT("Subkey: %s - ImagePath: %s\n"), achKey, achValue);
                    }
                    

                    // Close the subkey handle
                    RegCloseKey(hSubKey);
                }
            }
        }
    }

    // Enumerate the key values. 
    /*
    if (cValues)
    {
        printf("\nNumber of values: %d\n", cValues);

        for (i = 0; i < cValues; i++)
        {
            cchValue = MAX_VALUE_NAME;
            achValue[0] = '\0';
            retCode = ::RegEnumValue(hKey, i,
                achValue,
                &cchValue,
                NULL,
                NULL,
                NULL,
                NULL);

            if (retCode == ERROR_SUCCESS)
            {
                _tprintf(TEXT("(%d) %s\n"), i + 1, achValue);
            }
        }
    }*/
}

int __cdecl _tmain()
{
    HKEY hTestKey = 0;

    if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
        TEXT("SYSTEM\\CurrentControlSet\\Services"),
        0,
        KEY_READ,
        &hTestKey) == ERROR_SUCCESS
        )
    {
        QueryKey(hTestKey);
    }

    ::RegCloseKey(hTestKey);

    return 0;
}