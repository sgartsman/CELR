from PyQt5 import QtCore, QtGui, QtWidgets
import subprocess

# a sample dialog, copied modified and pasted for many purposes. not used directly
class InputDialog(QtWidgets.QDialog):
	def __init__(self, parent=None):
		super().__init__(parent)

		self.first = QtWidgets.QLineEdit(self)
		self.second = QtWidgets.QLineEdit(self)
		buttonBox = QtWidgets.QDialogButtonBox(QtWidgets.QDialogButtonBox.Ok | QtWidgets.QDialogButtonBox.Cancel, self);

		layout = QtWidgets.QFormLayout(self)
		layout.addRow("First text", self.first)
		layout.addRow("Second text", self.second)
		layout.addWidget(buttonBox)

		buttonBox.accepted.connect(self.accept)
		buttonBox.rejected.connect(self.reject)

	def getInputs(self):
	        return (self.first.text(), self.second.text())
       
	# use
	"""
	dialog = InputDialog()
	if dialog.exec():
	print(dialog.getInputs())
	"""
        

# a 2-series dialog that fetches process pid from the user
class pid_finder():
	class FirstInputDialog(QtWidgets.QDialog):
		def __init__(self, parent=None):
			super().__init__(parent)

			self.first = QtWidgets.QLineEdit(self)
			buttonBox = QtWidgets.QDialogButtonBox(QtWidgets.QDialogButtonBox.Ok, self);

			layout = QtWidgets.QFormLayout(self)
			layout.addRow("input the program name or a part of it (leave empty to se all programs)", self.first)
			layout.addWidget(buttonBox)

			buttonBox.accepted.connect(self.accept)

		def getInputs(self):
			return (self.first.text())
  	
	class second_dialog(QtWidgets.QDialog):
		def __init__(self, output, parent=None):
			super().__init__(parent)

			self.list_w = QtWidgets.QListWidget()
			self.list_w.addItems(output)
			self.list_w.setCurrentItem(self.list_w.item(0))
			
			self.first = QtWidgets.QLineEdit(self)
			buttonBox = QtWidgets.QDialogButtonBox(QtWidgets.QDialogButtonBox.Ok, self);

			layout = QtWidgets.QFormLayout(self)
			layout.addRow(self.list_w)
			self.list_w.setCurrentItem(self.list_w.item(0))
			layout.addRow("enter a valid pid (or write SELECT to use selected pid):", self.first)
			layout.addWidget(buttonBox)

			buttonBox.accepted.connect(self.accept)

		def getInputs(self):
			index = self.list_w.currentRow()
			item = self.list_w.item(index)
			return (self.first.text(), item.text())
  	
  	
	def find_pid(self):
		dialog = self.FirstInputDialog()
		if dialog.exec():
			name = dialog.getInputs()
		if name != '':
			raw = subprocess.getoutput('ps aux --cols 200 | tail -n +2 | grep ' + name).split("\n")
		else:
			raw = subprocess.getoutput('ps aux --cols 200 | tail -n +2').split("\n")
		matches = ["name: " + match.split()[10].split('/')[-1].ljust(40) + "| user: " + match.split()[0].ljust(8) + "| pid: " + match.split()[1] for match in raw[:-2]]
		
		dialog = self.second_dialog(matches)
		if dialog.exec():
			text, selected = dialog.getInputs()
			pid = text
			if pid == "SELECT":
				pid = selected.split(" ")[-1]
  			
		return str(int(pid))
		
		
		
# the dialog window after "find what writes to this address" (FWW)
class fww_dialog(QtWidgets.QDialog):
		def __init__(self, output, parent=None):
			super().__init__(parent)

			self.list_w = QtWidgets.QListWidget()
			self.list_w.addItems(output)
			self.list_w.setCurrentItem(self.list_w.item(0))
			
			self.first = QtWidgets.QLineEdit(self)
			buttonBox = QtWidgets.QDialogButtonBox(QtWidgets.QDialogButtonBox.Ok, self);

			layout = QtWidgets.QFormLayout(self)
			layout.addRow(self.list_w)
			self.list_w.setCurrentItem(self.list_w.item(0))
			layout.addRow("if you want to replace any of the above with NOPS select it and input \"NOP\"", self.first)
			layout.addWidget(buttonBox)

			buttonBox.accepted.connect(self.accept)

		def getInputs(self):
			index = self.list_w.currentRow()
			item = self.list_w.item(index)
			return (self.first.text(), item.text())
		
			
