import subprocess

# get the machine code of the binary code from where the program was debugged
raw = subprocess.getoutput("objdump -D -b binary -M intel -m i386 temp_code --insn-width=50").split("\n")
lines = [line.strip() for line in raw]

# get the find the last command process was executing (index -1)
for (index, line) in enumerate(lines):
	if line[:3] == "3c:": # data starts 60 (0x3c) bytes before RIP, so command on RIP is always on offset 0x3c
		break

# seperate opcodes from assembly
ret = lines[index-1].split(":")[1].strip()
[hex, asm] = ret.split('\t')

# delete the temp file used
raw = subprocess.getoutput("rm -f temp_code")

print(hex.strip() + " - " + " ".join(asm.split()))

