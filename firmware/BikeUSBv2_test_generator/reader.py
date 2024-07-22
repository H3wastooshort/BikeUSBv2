max_val=1023

import sys, serial, struct, os
if (len(sys.argv) not in [2,3]):
    print("debug_decoder.py <port> <baudrate>")
    quit(1)
port = sys.argv[1]
baud = 115200
if len(sys.argv) >= 3:
    baud = int(sys.argv[2])

print("Opening %s at %d baud" % (port,baud))
ser = serial.Serial(port,baud)
ser.timeout = 3

def disp_val(val):
    size = os.get_terminal_size()
    width = size[0]
    width -= 4+1+1 # subtract used up chars
    pad_before = int(round(width * (val/max_val)))
    line = ""
    line+='|'
    line+='-'*pad_before
    line+="%04d"%(val)
    line+='-'*(width-pad_before)
    line+='|'
    print(line)

while True:
    b = ser.read(2)
    t = struct.unpack('<H',b)
    disp_val(t[0])
