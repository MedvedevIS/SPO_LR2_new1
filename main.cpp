#include <iostream>
#define WINVER 0x0502
#include <windows.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <sddl.h>
#include  "functions.cpp"

using namespace std;

static PSECURITY_DESCRIPTOR create_security_descriptor()
{
const char* sddl ="D:(A;OICI;GRGW;;;AU)(A;OICI;GA;;;BA)";
PSECURITY_DESCRIPTOR security_descriptor = NULL;
ConvertStringSecurityDescriptorToSecurityDescriptor(sddl, SDDL_REVISION_1, &security_descriptor, NULL);
return security_descriptor;
}


int main()
{
	
BOOL WINAPI Create_MailSlot(LPCTSTR lpSlotName, HANDLE& hslot, bool& isserver);
BOOL Get_Messages(HANDLE hSlot, DWORD* cbMessage, DWORD* cMessage);	
BOOL Write_Message(HANDLE hSlot, LPCTSTR lpszMessage);	
BOOL Read_Message(HANDLE hfSlot);	
//---------------------------------------------------------------------------------------------------------------------------------------------------------------	
DWORD* cbMessage;
DWORD* cMessage;
cbMessage = cMessage = 0;
string InputName;

setlocale(LC_ALL, "Russian");

cout << "Enter mailbox:";
cin >> InputName;

LPCTSTR SlotName = InputName.c_str();

HANDLE hSlot;
bool isserver;

bool res=Create_MailSlot(SlotName, hSlot, isserver);
if (res==FALSE)
return 0;

printf("Process started-");
printf((isserver) ? "Server\n" : "Client\n");
printf("Command List:\n[check] - getting information about the number of messages\n");
printf((isserver) ? "[read] - reading received messages\n" : "[write] - print messages\n");
printf("[quit] - Exit\n");

string command;
while (1)
{
cout << ">";
cin >> command;
if (command == "check")
{
Get_Messages(hSlot, cbMessage, cMessage);
}
else if (command == "quit")
{
CloseHandle(hSlot);
return 0;
}
else if ((command == "read") && (isserver))
{
Read_Message(hSlot);
}
else if ((command == "write") && (!isserver))
{
printf("Enter your message\n");

string message, str;

getline(cin, str);
do
{
getline(cin, str);
message += str + '\n';
} while (!str.empty());

Write_Message(hSlot, (LPCTSTR)message.c_str());
}
else if ((command != "write") || (command != "check") || (command != "read") || (command == "quit"))
{
printf("You entered the command incorrectly.Please enter one of the commands suggested in the list:\n[check]-getting information about the number of messages\n");
printf((isserver) ? "[read]-reading received messages\n" : "[write]- print messages\n");
printf("[quit]-Exit\n");
}
}
return 0;
}
