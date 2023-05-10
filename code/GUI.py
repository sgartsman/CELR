import sys
sys.path.append('./runtime')
sys.path.append('./GUI')
import time

import subprocess
from PyQt5 import QtCore, QtGui, QtWidgets
from tracer_comm import tracer
from main_window import Ui_MainWindow
from assets import *

global backend
global ui

def main():
	global backend
	global ui
	
	# set up main window
	app = QtWidgets.QApplication(sys.argv)
	MainWindow = QtWidgets.QMainWindow()
	MainWindow.setWindowIcon(QtGui.QIcon("./GUI/logo.png"))
	ui = Ui_MainWindow()
	
	# connect events ("slots") to functions
	MainWindow.scan = scan
	MainWindow.reset_scan = reset_scan
	MainWindow.add_selected_address = add_selected_address
	MainWindow.write = write
	MainWindow.rename = rename
	MainWindow.remove = remove
	MainWindow.fww = fww
	MainWindow.update_value = update_value

	
	ui.setupUi(MainWindow)
	MainWindow.setWindowTitle("CELR")
	
	MainWindow.show()

	print("\n"*50) # "hide" the ugly QT5 warning

	# open the communications with the backend in the background
	pid = pid_finder().find_pid()
	print("\n\nopening tracer subprocess, console now in debug mode\n\n")
	backend = tracer(pid)
	backend.read_until_input()
	
	# until window is closed, functions are called on button presses.
	
	# on window close - exit
	sys.exit(app.exec_())


#scan with selected options
def scan():
	global backend
	global ui

	data_type = get_current_type()
	comparison = str(ui.combo_box_comapre_type.currentText()[-2])
	compare_to = ui.line_edit_compare_to.text()
	backend.perform_command("scan", data_type, comparison,  compare_to)
	lines = backend.read_until_input()
	
	rows = [str(hex(int(line.split()[1][:-1]))) + "   -   " + line.split()[-1] for line in lines]
	
	ui.list_scanned_addrs.clear()
	ui.list_scanned_addrs.addItems(rows)


# reset the scanner
def reset_scan():
	global backend
	global ui

	backend.perform_command("reset")
	lines = backend.read_until_input()

	ui.list_scanned_addrs.clear()


# save an address
def add_selected_address():
	global backend
	global ui
	
	index = ui.list_scanned_addrs.currentRow()
	item = ui.list_scanned_addrs.item(index)
	if item is not None:
		raw = item.text()
		(addr, val) = raw.split("-")
		print(addr,"-",val,sep='')
		(addr, val) = addr.strip(), val.strip()
		data_type = get_current_type()
		label = "placeholder"
		
		new = " - ".join((label, addr, data_type, val)) 

		ui.saved_addresses.addItems((new,))


# write new value in a selected address
def write():
	global ui
	global backend
	
	index = ui.saved_addresses.currentRow()
	item = ui.saved_addresses.item(index)
	if item is not None:
		raw = item.text().split('-')
		addr = str(int(raw[1].strip(), 16))
		data_type = raw[2].strip()
		value, accepted = QtWidgets.QInputDialog.getText(QtWidgets.QWidget(), '', 'input new data:')
		if accepted:
			backend.perform_command("write", addr, data_type, value)
			backend.read_until_input()


# rename a saved address
def rename():
	global ui
	index = ui.saved_addresses.currentRow()
	item = ui.saved_addresses.item(index)
	if item is not None:
		raw = item.text().split('-')[1:]
		value, accepted = QtWidgets.QInputDialog.getText(QtWidgets.QWidget(), '', 'input new data:')
		if accepted:
			item.setText(value + " - " + "-".join(raw))
	

# removes an item fom saved values
def remove():
	global ui
	index = ui.saved_addresses.currentRow()
	item = ui.saved_addresses.item(index)
	if item is not None:
		ui.saved_addresses.takeItem(index)


# return selected type by the type combo box
def get_current_type():
	global ui
	
	return str(ui.combo_box_data_type.currentText()[0])

	
# find what writes to a saved address
# allows the option to replace an opcode with NOPs
def fww():
	global ui
	global backend
	
	index = ui.saved_addresses.currentRow()
	item = ui.saved_addresses.item(index)
	if item is not None:
		scans, accepted = QtWidgets.QInputDialog.getInt(QtWidgets.QWidget(), '', 'how many scans?')
		if accepted:
			args = item.text().split('-')
			backend.perform_command("fww", str(int(args[1].strip(), 16)), args[2].strip(), str(scans))
			output = backend.read_until_input()
			hex_add = [str(hex(int(line.split(":")[0]))) + ":" + line.split(":")[-1] for line in output]
			
			dialog = fww_dialog(hex_add)
			if dialog.exec():
				text, opcode = dialog.getInputs()
				if text == "NOP":
					op_len = len("".join(opcode.split(":")[1].split("-")[0].split()))//2
					start_addr = int(opcode.split(":")[0] ,16) - op_len
					for i in range(op_len):
						backend.perform_command("write", str(start_addr), 'c', "-112")
						backend.read_until_input()
						start_addr += 1
			

# update value of selected saved address
def update_value():
	global ui
	global backend
	
	index = ui.saved_addresses.currentRow()
	item = ui.saved_addresses.item(index)
	if item is not None:
		text = item.text().split(" - ")
		addr = str(int(text[1], 16))
		data_type = text[2]
		backend.perform_command('read', addr, data_type)
		text[-1] = backend.read_until_input()[0]		
		item.setText(" - ".join(text))
		

main()

