import subprocess

# get the machine code of the binary code from where the program was debugged
raw = subprocess.getoutput("objdump -D -b binary -M intel -m i386 runtime/temp_code --insn-width=50").split("\n")
lines = [line.strip() for line in raw]

# seperate opcodes from assembly
ret = lines[-1].split(":")[1].strip()
[hex, asm] = ret.split('\t')

# delete the temp file used
raw = subprocess.getoutput("rm -f temp_code")

print(hex.strip() + " - " + " ".join(asm.split()))

