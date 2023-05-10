import subprocess

# a class that open the tracer as subprocess and handles all communications with it
class tracer():

	def __init__(self, pid):
		self.proc = subprocess.Popen(['/bin/bash'], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
		self.proc.stdin.write(b'sudo ./runtime/tracer ' + pid.encode() + b"\n")
		self.proc.stdin.flush()
	
	def read_until_input(self):
		lines = []
		while True:
			line = self.proc.stdout.readline().decode().strip()
			print(line)
			if (line.startswith("-==")): # it means input is now required
				self.print_input_request()
				break
			lines.append(line)

		return lines # return lines until input
	
	def perform_command(self, command, *args):
		print(command)
		self.proc.stdin.write(command.encode() + b"\n")
		self.proc.stdin.flush()
		for arg in args:
			line = self.proc.stdout.readline().decode().strip()
			print(line)
			self.print_input_request()		
			print(arg)
		
			self.proc.stdin.write(arg.encode() + b"\n")
			self.proc.stdin.flush()
			
			
	# util functions
	def print_input_request(self):
		# last line: the line with -== XXXX ==-. assumes it was printed
		print('--> ', end = '', flush=True)
		
		
