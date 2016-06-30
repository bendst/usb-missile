#!/usr/bin/python

# Thunder: http://www.dreamcheeky.com/thunder-missile-launcher
# O.I.C Storm: http://www.dreamcheeky.com/storm-oic-missile-launcher

# This script requires:
# * python-usb
# * The ImageTk library. On Debian/Ubuntu 'sudo apt-get install python-imaging-tk'
# Also, unless you want to toggle with udev rules, it needs to be run as root

# Use arrows to aim. Use the left enter to fire. More functions watch "Control"-section

import os
import sys
import time
import usb.core
from Tkinter import * # GUI-Bibliothek

class launchControl(Frame):

	def __init__(self):

		self.dev = usb.core.find(idVendor=0x2123, idProduct=0x1010)
		if self.dev is None:
			raise ValueError('Launcher not found.')
		if self.dev.is_kernel_driver_active(0) is True:
			self.dev.detach_kernel_driver(0)
		self.dev.set_configuration()

		# Control
		# Observe keysim for available keys
		self.KEY_TURN_UP = 'Up'
		self.KEY_TURN_DOWN = 'Down'
		self.KEY_TURN_LEFT = 'Left'
		self.KEY_TURN_RIGHT = 'Right'
		self.KEY_TURN_MIDDLE = 'm'
		self.KEY_FIRE = 'Return'
		self.KEY_LED = 'f'
		self.KEY_CLOSE = 'Escape'

		self.directionByte = 0x00
		self.LEDByte = 0x00

		# Map keysym to device-specific bitposition
		self.directionToBitDic = {
			self.KEY_TURN_DOWN: 1,
			self.KEY_TURN_UP: 2,
			self.KEY_TURN_LEFT: 4,
			self.KEY_TURN_RIGHT: 8
		}

		self.directionByteToMsgDic = {
			0x00: "Stop.",
			0x01: "Turn down.",
			0x02: "Turn up.",
			0x04: "Turn left.",
			0x08: "Turn right.",
			0x05: "Turn down-left.",
			0x06: "Turn up-left.",
			0x09: "Turn down-right.",
			0x0a: "Turn up-right."
		}

		Frame.__init__(self)
		self.pack()
		self.master.title("Launch Control")
		self.master.geometry("300x20")

		self.message1 = StringVar()
		self.line1 = Label(self, textvariable = self.message1)
		self.message1.set("Aim (arrow keys) & Fire (return)!")
		self.line1.pack()

		self.master.bind("<KeyPress-"+ self.KEY_TURN_UP+">", self.turnKeyPressed)
		self.master.bind("<KeyRelease-"+ self.KEY_TURN_UP+">", self.turnKeyReleased)
		self.master.bind("<KeyPress-"+ self.KEY_TURN_DOWN+">", self.turnKeyPressed)
		self.master.bind("<KeyRelease-"+ self.KEY_TURN_DOWN+">", self.turnKeyReleased)
		self.master.bind("<KeyPress-"+ self.KEY_TURN_LEFT+">", self.turnKeyPressed)
		self.master.bind("<KeyRelease-"+ self.KEY_TURN_LEFT+">", self.turnKeyReleased)
		self.master.bind("<KeyPress-"+ self.KEY_TURN_RIGHT+">", self.turnKeyPressed)
		self.master.bind("<KeyRelease-"+ self.KEY_TURN_RIGHT+">", self.turnKeyReleased)
		self.master.bind("<Key-"+ self.KEY_TURN_MIDDLE+">", self.turretMiddle)
		self.master.bind("<Key-"+ self.KEY_FIRE+">", self.turretFire)
		self.master.bind("<Key-"+ self.KEY_LED+">", self.turretLED)
		self.master.bind("<Key-"+ self.KEY_CLOSE+">", self.close)
		

	def turnKeyPressed(self, event):
		self.directionByte += self.directionToBitDic.get(event.keysym)
		self.turretTurn();
		

	def turnKeyReleased(self, event):
		self.directionByte -= self.directionToBitDic.get(event.keysym)
		self.turretTurn();


	def turretTurn(self):
		self.message1.set(self.directionByteToMsgDic.get(self.directionByte, "Turn impossible."))
		self.dev.ctrl_transfer(0x21,0x09,0,0,[0x02,self.directionByte,0x00,0x00,0x00,0x00,0x00,0x00])


	def turretMiddle(self, event):
		# Approximate middle turret-position
		self.message1.set("Turn middle.")
		self.dev.ctrl_transfer(0x21,0x09,0,0,[0x02,0x05,0x00,0x00,0x00,0x00,0x00,0x00])
		time.sleep(5.36)
		self.dev.ctrl_transfer(0x21,0x09,0,0,[0x02,0x08,0x00,0x00,0x00,0x00,0x00,0x00])
		time.sleep(2.68)
		self.dev.ctrl_transfer(0x21,0x09,0,0,[0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x00])
		time.sleep(0.1)
		self.dev.ctrl_transfer(0x21,0x09,0,0,[0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00])


	def turretLED(self, event):
		if self.LEDByte == 0x00:
			self.LEDByte = 0x01;
			self.message1.set("LED on.")
		else:
			self.LEDByte = 0x00;			
			self.message1.set("LED off.")
		self.dev.ctrl_transfer(0x21,0x09,0,0,[0x03,self.LEDByte,0x00,0x00,0x00,0x00,0x00,0x00])
		

	def turretFire(self, event):
		self.message1.set("Launch missile!")
		self.dev.ctrl_transfer(0x21,0x09,0,0,[0x02,0x10,0x00,0x00,0x00,0x00,0x00,0x00])
		# Block interruptions
		time.sleep(3)


	def close(self, event):
		# Stop movements
		if self.directionByte != 0x00:
			self.dev.ctrl_transfer(0x21,0x09,0,0,[0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00])
		# Turn off LED
		if self.LEDByte != 0x00:
			self.dev.ctrl_transfer(0x21,0x09,0,0,[0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00])
		sys.exit();

if __name__ == '__main__':
	if not os.geteuid() == 0:
	    sys.exit("Script must be run as root.")
	launchControl().mainloop()
