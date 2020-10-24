#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <winuser.h>
#include <iostream>
#include <accctrl.h>
#include <aclapi.h>
#include <cstdlib>



#pragma comment(lib)

using namespace std;
//
//class FileSeek
//{
//public:
//	BOOL InitializeSecurityAttributes(LPSECURITY_ATTRIBUTES lpAttributes);
//	void _load();
//
//	SECURITY_ATTRIBUTES sa;
//	PSID pSID;
//	PSID pSIDAdmin;
//	PACL pACL;
//
//
//}fs;
//
//
//BOOL FileSeek::InitializeSecurityAttributes(LPSECURITY_ATTRIBUTES lpAttributes)
//{
//	SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
//	SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;
//
//	if (!AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &pSID))
//		return FALSE;
//
//	if (!AllocateAndInitializeSid(&SIDAuthNT, 1, DOMAIN_GROUP_RID_ADMINS || DOMAIN_GROUP_RID_ADMINS, DOMAIN_ALIAS_RID_ADMINS,
//		0, 0, 0, 0, 0, 0, &pSIDAdmin))
//		return false;
//	// Initialize an EXPLICIT_ACCESS structure for an ACE
//	EXPLICIT_ACCESS ea[2];
//	//  ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
//	  //ea.grfAccessPermissions = EVENT_ALL_ACCESS;
//	  //ea.grfAccessMode = SET_ACCESS;
//	  //ea.grfInheritance = NO_INHERITANCE;
//	  //ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
//	  //ea.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
//	  //ea.Trustee.ptstrName = (LPTSTR)pSID;
//
//	ZeroMemory(&ea, 2 * sizeof(EXPLICIT_ACCESS));
//	ea[0].grfAccessPermissions = GENERIC_READ;
//	ea[0].grfAccessMode = SET_ACCESS;
//	ea[0].grfInheritance = NO_INHERITANCE;
//	ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
//	ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
//	ea[0].Trustee.ptstrName = (LPTSTR)pSID;
//
//	ea[1].grfAccessPermissions = GENERIC_ALL;
//	ea[1].grfAccessMode = SET_ACCESS;
//	ea[1].grfInheritance = NO_INHERITANCE;
//	ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
//	ea[1].Trustee.TrusteeType = TRUSTEE_IS_GROUP;
//	ea[1].Trustee.ptstrName = (LPTSTR)pSIDAdmin;
//
//	if (SetEntriesInAcl(2, ea, NULL, &pACL) != ERROR_SUCCESS)
//		return FALSE;
//	PSECURITY_DESCRIPTOR pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
//	if (NULL == pSD)
//		return FALSE;
//
//	if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION))
//		return FALSE;
//
//	if (!SetSecurityDescriptorDacl(pSD, TRUE, pACL, FALSE))
//		return FALSE;
//
//	lpAttributes->nLength = sizeof(SECURITY_ATTRIBUTES);
//	lpAttributes->lpSecurityDescriptor = pSD;
//	lpAttributes->bInheritHandle = FALSE;
//
//	return TRUE;
//}
//
//void FileSeek::_load()
//{
//
//
//	if (!fs.InitializeSecurityAttributes(&sa)) {
//		MessageBoxW(NULL, L"NE ochen'", L"Error", MB_OK);
//		_getch();
//		return;
//	}
//	HKEY hKey;
//	DWORD dwDisposition = KEY_CREATE_SUB_KEY;
//
//	TCHAR lpData[1024] = { 5 };
//	DWORD buffersize = sizeof(lpData);
//
//	if (RegCreateKeyExW(HKEY_CURRENT_USER, TEXT("Software\\test1"), 0, NULL, KEY_CREATE_SUB_KEY, 0, 0, &hKey, &dwDisposition) == ERROR_SUCCESS)
//		//if (RegCreateKeyExW(HKEY_CURRENT_USER, TEXT("Software\\test1"), 0, NULL, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, &sa, &hKey, &dwDisposition) == ERROR_SUCCESS)
//	{
//
//		if (RegSetValueExW(hKey, TEXT("t1"), NULL, NULL, (LPBYTE)lpData, buffersize) == ERROR_SUCCESS)
//		{
//			cout << "\nYour current Internet start page is ";
//		}
//		else
//			cout << "\nError getting the specified value.\n";
//	}
//	else
//		cout << "\nError opening the specified subkey path (doesn't exist?).\n";
//	int a = GetLastError();
//
//	RegCloseKey(hKey);
//
//
//
//
//
//}
//

#include <windows.h>
#include <stdio.h>
#include <aclapi.h>
#include <tchar.h>

void main()
{
    wchar_t mus[] = { L"" };
    DWORD dwRes, dwDisposition;
    PSID pEveryoneSID = NULL, pAdminSID = NULL;
    PACL pACL = NULL;
    PSECURITY_DESCRIPTOR pSD = NULL;
    EXPLICIT_ACCESS ea[2];
    SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
    SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;
    SECURITY_ATTRIBUTES sa;
    LONG lRes;
    HKEY hkSub = NULL;

    // Create a well-known SID for the Everyone group.
    if (!AllocateAndInitializeSid(&SIDAuthWorld, 1,
        SECURITY_WORLD_RID,
        0, 0, 0, 0, 0, 0, 0,
        &pEveryoneSID))
    {
        wprintf(L"AllocateAndInitializeSid Error %u\n", GetLastError());
        goto Cleanup;
    }

    // Initialize an EXPLICIT_ACCESS structure for an ACE.
    // The ACE will allow Everyone read access to the key.
    ZeroMemory(&ea, 2 * sizeof(EXPLICIT_ACCESS));
    ea[0].grfAccessPermissions = GENERIC_WRITE;
    ea[0].grfAccessMode = SET_ACCESS;
    ea[0].grfInheritance = NO_INHERITANCE;
    ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    ea[0].Trustee.ptstrName = (LPTSTR)pEveryoneSID;

    // Create a SID for the BUILTIN\Administrators group.
    if (!AllocateAndInitializeSid(&SIDAuthNT, 2,
        SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0,
        &pAdminSID))
    {
       wprintf(L"AllocateAndInitializeSid Error %u\n", GetLastError());
        goto Cleanup;
    }

    // Initialize an EXPLICIT_ACCESS structure for an ACE.
    // The ACE will allow the Administrators group full access to
    // the key.
    ea[1].grfAccessPermissions = GENERIC_WRITE;
    ea[1].grfAccessMode = SET_ACCESS;
    ea[1].grfInheritance = NO_INHERITANCE;
    ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[1].Trustee.TrusteeType = TRUSTEE_IS_GROUP;
    ea[1].Trustee.ptstrName = (LPTSTR)pAdminSID;

    // Create a new ACL that contains the new ACEs.
    dwRes = SetEntriesInAcl(2, ea, NULL, &pACL);
    if (ERROR_SUCCESS != dwRes)
    {
        wprintf(L"SetEntriesInAcl Error %u\n", GetLastError());
        goto Cleanup;
    }

    // Initialize a security descriptor.  
    pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR,
        SECURITY_DESCRIPTOR_MIN_LENGTH);
    if (NULL == pSD)
    {
        wprintf(L"LocalAlloc Error %u\n", GetLastError());
        goto Cleanup;
    }

    if (!InitializeSecurityDescriptor(pSD,
        SECURITY_DESCRIPTOR_REVISION))
    {
        wprintf(L"InitializeSecurityDescriptor Error %u\n",  GetLastError());
        goto Cleanup;
    }

    //// Add the ACL to the security descriptor. 
    //if (!SetSecurityDescriptorDacl(pSD,
    //    TRUE,     // bDaclPresent flag   
    //    pACL,
    //    FALSE))   // not a default DACL 
    //{
    //    wprintf(L"SetSecurityDescriptorDacl Error %u\n", GetLastError());
    //    goto Cleanup;
    //}

    // Initialize a security attributes structure.
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = pSD;
    sa.bInheritHandle = FALSE;

    // Use the security attributes to set the security descriptor 
    // when you create a key.

    lRes = RegCreateKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\test21", 0, mus, 0,
       /* KEY_READ | KEY_WRITE |*/ GENERIC_ALL, &sa, &hkSub, &dwDisposition);
    wprintf(L"RegCreateKeyEx result %u\n", lRes);

Cleanup:

    if (pEveryoneSID)
        FreeSid(pEveryoneSID);
    if (pAdminSID)
        FreeSid(pAdminSID);
    if (pACL)
        LocalFree(pACL);
    if (pSD)
        LocalFree(pSD);
    if (hkSub)
        RegCloseKey(hkSub);

 
    system("regedit");
    return;
}

bool test2()
{
	SECURITY_ATTRIBUTES sa;
	//LPSECURITY_ATTRIBUTES lpAttributes = &sa;
	SID sid;
	PSID pSID = &sid;
	PSID pSIDAdmin;
	PACL pACL;

	SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
	SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;

	if (!AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0,  0, 0, 0, 0, 0, 0, &pSID))
		return FALSE;

	if (!AllocateAndInitializeSid(&SIDAuthNT, 1, SECURITY_BUILTIN_DOMAIN_RID, 0, 0,
		0, 0, 0, 0, 0, &pSIDAdmin))
		return FALSE;

	wchar_t mas[] = { L"11111" };
	EXPLICIT_ACCESS ea[2];
	
	ZeroMemory(&ea, 2 * sizeof(EXPLICIT_ACCESS));
	ea[0].grfAccessPermissions = MAXIMUM_ALLOWED;
	ea[0].grfAccessMode = SET_ACCESS;
	ea[0].grfInheritance = NO_INHERITANCE;
	ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
	ea[0].Trustee.ptstrName = (LPTSTR)pSID;

	ea[1].grfAccessPermissions = GENERIC_ALL;
	ea[1].grfAccessMode = SET_ACCESS;
	ea[1].grfInheritance = NO_INHERITANCE;
	ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ea[1].Trustee.TrusteeType = TRUSTEE_IS_GROUP;
	ea[1].Trustee.ptstrName = (LPTSTR)pSIDAdmin;

	if (SetEntriesInAcl(2, ea, NULL, &pACL) != ERROR_SUCCESS)
		return FALSE;


	//EXPLICIT_ACCESS ea;

	//ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
	//ea.grfAccessPermissions = GENERIC_ALL;
	//ea.grfAccessMode = SET_ACCESS;
	//ea.grfInheritance = NO_INHERITANCE;
	//ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
	//ea.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
	//ea.Trustee.ptstrName = (LPTSTR)pSID;

	//if (SetEntriesInAcl(1, &ea, NULL, &pACL) != ERROR_SUCCESS)
	//	return FALSE;


	PSECURITY_DESCRIPTOR pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);

	if (NULL == pSD)
		return FALSE;

	if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION))
		return FALSE;

	if (!SetSecurityDescriptorDacl(pSD, TRUE, pACL, FALSE))
		return FALSE;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = pSD;
	sa.bInheritHandle = FALSE;

	HKEY hKey;
	char szPath[0x100] = "1231432534563445";

	RegCreateKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\test15", 0, 0, 0, 0, &sa, &hKey, NULL);
	//RegCreateKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\test2", 0, 0, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, &sa, &hKey, NULL);

	if (hKey) {
		if (RegSetValueExA(hKey, "Password", 0, REG_SZ, (LPBYTE)szPath, strlen(szPath)) == ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			system("PAUSE");
		}
		else
			MessageBoxW(NULL, L"Noooooo", L"Error", MB_OK);
	}


	return TRUE;
}



//int main()
//{
//	//fs._load();
//
//	test2();
//	system("regedit");
//	return 0;
//}
//

