#include <iostream>
#define WINVER 0x0502
#include <windows.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <sddl.h>

using namespace std;



BOOL WINAPI Create_MailSlot(LPCTSTR lpSlotName, HANDLE& hslot, bool& isserver)
{

hslot = CreateMailslot(lpSlotName,NULL,MAILSLOT_WAIT_FOREVER, NULL);

if (hslot == INVALID_HANDLE_VALUE)
{
DWORD err = GetLastError();

if (err == ERROR_ALREADY_EXISTS || err ==ERROR_INVALID_NAME)
{
isserver = FALSE;
hslot = CreateFile(lpSlotName,GENERIC_WRITE,FILE_SHARE_READ,(LPSECURITY_ATTRIBUTES)NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,(HANDLE)NULL);

if (hslot == INVALID_HANDLE_VALUE)
{
DWORD err = GetLastError();
printf("Error while executing a function CreateFile(): %d\n", GetLastError());
return FALSE;
}
}
else
{
printf("Error while executing a function CreateFile(): %d\n", GetLastError());
return FALSE;
}
}
else
{
isserver = TRUE;
}
return TRUE;
}

BOOL Get_Messages(HANDLE hSlot, DWORD* cbMessage, DWORD* cMessage)
{
DWORD cbMessageL, cMessageL;
BOOL hmessage;

hmessage = GetMailslotInfo(hSlot,(LPDWORD) NULL,&cbMessageL,&cMessageL,(LPDWORD) NULL);

if (hmessage)
{
printf("Found %i messages in the mailbox\n", cMessageL);
if (cbMessage != NULL)*cbMessage = cbMessageL;
if (cMessage != NULL)*cMessage = cMessageL;
return TRUE;
}
else
{
printf("Error while executing a function GetMailslotInfo(): %d\n", GetLastError());
return FALSE;
}
}
BOOL Write_Message(HANDLE hSlot, LPCTSTR lpszMessage)
{
BOOL hwrite;
DWORD cbWritten;

hwrite = WriteFile(hSlot,lpszMessage,(lstrlen(lpszMessage) + 1) * sizeof(TCHAR),&cbWritten,(LPOVERLAPPED)NULL);

if (hwrite ==0)
{
printf("Error while executing a function WriteFile(): %d\n", GetLastError());
return FALSE;
}
else
{
printf("Message sent.\n");
return TRUE;
}
}

BOOL Read_Message(HANDLE hfSlot)
{
DWORD cbrMessage, crMessage, cbrRead;
BOOL hread;
cbrMessage = crMessage = cbrRead = 0;
hread = GetMailslotInfo(hfSlot,NULL,&cbrMessage,&crMessage,NULL);

if (cbrMessage == MAILSLOT_NO_MESSAGE)
{
printf("There are no messages in the mailbox.\n");
return TRUE;
}

if (hread==0)
{
printf("Error while executing a function GetMailslotInfo: %d\n", GetLastError());
return FALSE;
}

string Message(cbrMessage,'\0');

hread = ReadFile(hfSlot,&Message[0], Message.size(),&cbrRead,NULL);

if (hread == 0)
{
printf("Error while trying to read the contents of the file: %d\n", GetLastError());
return FALSE;
}
cout << Message;
return TRUE;
}




