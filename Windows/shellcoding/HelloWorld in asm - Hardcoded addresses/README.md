## **HelloWorld in asm - Hardcoded addresses**

In this code, we will use hardcoded addresses for `MessageBoxA`, `LoadLibraryA`, and `ExitProcess`, which we will obtain using the resolver.exe tool. The code will display a "Hello World" message box written in assembly.

We will ensure that `user32.dll` is loaded using `LoadLibraryA` (since it is available in `kernel32.dll`), and then call `MessageBoxA`. Finally, we will call `ExitProcess` to terminate the program smoothly.

We will also use the following steps to compile and execute the assembly code:

1. Compile the assembly code:
   ```bash
   nasm -f elf32 test1.asm -o test1.o
   ```

2. Dump the shellcode:
   ```bash
   ./dump.sh test1.o
   ```

3. Include the shellcode in a C program:
   ```c
   #include <Windows.h>

   const char main[] = "shellcode";
   ```

4. Compile the C program:
   ```bash
   mingw32-gcc-6.3.0.exe F:\Junk\tester.c -o tester.exe
   ```

   Note: The `tester.exe` is the compiled version of the same code:
   ```c
   #include <Windows.h>

   const char main[] = "shellcode";
   ```

Once you do that, for resolving the hardcoded addresses, you can use `resolver.exe`. I will provide the .c file as well if you want to see the code, and also the object file if you need that.

You can then visualize what is happening in IDA.