# -*- coding: utf-8 -*-
"""
Created on Tue May 21 20:43:38 2019

@author: Alan
"""

"""
ldr.py
Display analog data from Arduino using Python (matplotlib)
Author: Mahesh Venkitachalam
Website: electronut.in
"""

import sys, serial, argparse,struct
import numpy as np
from time import sleep
from collections import deque

import matplotlib.pyplot as plt 
import matplotlib.animation as animation

    
# plot class
class AnalogPlot:
  # constr
  def __init__(self, strPort, maxLen):
      # open serial port
      self.ser = serial.Serial(strPort, 230400)

      self.ax = deque([0.0]*maxLen)
      self.ay = deque([0.0]*maxLen)
      self.maxLen = maxLen
      self.CurrentNum = 0

  # add to buffer
  def addToBuf(self, buf, val):
      if len(buf) < self.maxLen:
          buf.append(val)
      else:
          buf.pop()
          buf.appendleft(val)

  # add data
  def add(self, data):
      #assert(len(data) == 2)
      self.CurrentNum += 1
      self.addToBuf(self.ax, data)
      #self.addToBuf(self.ay, data)

  # update plot
  def update(self, frameNum, a0, a1):
      try:
          line = self.ser.read(2)
          data = struct.unpack("H",line)
          # print data
          #if(len(data) == 2):
          self.add(data)
          #a0.set_data(range(self.maxLen), self.ax)
          #a1.set_data(range(self.maxLen), self.ay)
      except KeyboardInterrupt:
          print('exiting')
      
      return a0, 

  # clean up
  def close(self):
      # close serial
      self.ser.flush()
      self.ser.close()    


    

data = [0]*3200; #data abuffer

#plt.ion()
fig = plt.figure() #figure
#ax = fig.add_subplot(1,1,1)
ax = plt.axes(xlim=(0, 3200), ylim=(0, 4095)) #axis
line, = ax.plot(data)

 
plt.show()

def animate(frame):
    data[100]+=1

    line.set_ydata(data)
    #fig.canvas.draw()
    return line

ani = animation.FuncAnimation(fig, animate, blit=True)
