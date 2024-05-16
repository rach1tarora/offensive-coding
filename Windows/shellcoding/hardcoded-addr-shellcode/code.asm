BITS 32

global _start


%define MessageBoxA  0x763FAE40
%define LoadLibraryA 0x76560F30
%define ExitProcess 0x765574D0
section .text

_start:


        push "ll"
        push "32.d"
        push "user"
        push esp

        mov eax, LoadLibraryA
        call eax


        ; string is hello world
        push "o!"
        push "hell"
        push esp
        pop esi

        push "d!"
        push "worl"
        push esp
        pop edi

        xor ebx, ebx
        push ebx
        push esi
        push edi
        push ebx
        mov eax, MessageBoxA
        call eax

        xor  ebx, ebx
        push ebx
        mov eax, ExitProcess
        call eax
