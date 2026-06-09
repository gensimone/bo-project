# Stack Buffer Overflow.

## NISTIR 7298 defines a buffer overflow, also known as a buffer overrun as follows:
Buffer Overrun: A condition at an interface under which more input can be placed into a buffer or data holding area
than the capacity allocated, overwriting other information. Attackers exploit such a condition to crash a system or
to insert specially crafted code that allows them to gain control of the system.

## Exploit
To exploit any type of buffer overflow, the attackers needs:
    1) To identify a buffer overflow vulnerability in some program that can be triggered using externally sourced data
       under the attackers control, and
    2) To understand how that buffer will be stored in the processes memory, and hence the potential for corrupting
       adjacent memory locations and potentially altering the flow of execution of the program.

Important notes:
- The shellcode must be able to run no matter where in memory it is located.
- The attacker is not able to precisely specify the starting address of the instructions in the shellcode.
- If the shellcode is copied using string manipulation routines, it cannot contain NULL values in the middle of it.
