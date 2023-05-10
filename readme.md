### CELR - Cheat Engine Linux Replacement

CELR is a Linux tool that mimics "Cheat Engine" and allows users to find addresses of useful variables and opcodes and change them to their will.
CELR uses advanced techniques such ass attaching as a debugger (with ptrace Syscall) and using the CPU debug registers

to use:
- simply run the CELR executable

example use:
- open doom and attach to it, start a new game.
- scan for an integer with the exact value of your ammo.
- shoot, scan again. repeat until you find the address of your ammo.
- save the address, rename to ammo.
- write 500 into it and see you have 500 ammo to ensure you found the right address.
- find what writes to the ammo address with 1 scan. shoot, and replace the opcode that changed the ammo with NOPS.
- now when you shoot your ammo will not go down. you can repeat for health, armor etc.
- enjoy!

requirements:
- OS - Linux Ubuntu (works on many others but not guaranteed)
- python 3
- PyQt5 (pip isntall PyQt5)

can be used without GUI (not reccommended) by running the tracer (/code/tracer) in a terminal

