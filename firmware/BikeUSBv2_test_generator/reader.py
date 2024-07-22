avg_over_n=10
max_possible=1023

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

def disp_val(avg_val,min_val,max_val):
    avg_val=int(round(avg_val))
    min_val=int(round(min_val))
    max_val=int(round(max_val))
    
    size = os.get_terminal_size()
    width = size[0]
    width -= 4+1+1 # subtract used up chars
    pad_before = int(round(width * (avg_val/max_possible)))
    line = ""
    line+='|'
    line+='-'*pad_before
    line+="%04d"%(avg_val)
    line+='-'*(width-pad_before)
    line+='|'
    print(line)

while True:
    min_val=max_possible
    max_val=0
    avg_val=0
    for n in range(0,avg_over_n):
        b = ser.read(2)
        i = struct.unpack('<H',b)[0]
        max_val = max(max_val,i)
        min_val = min(min_val,i)
        avg_val+=i
    avg_val /= avg_over_n
    disp_val(avg_val,min_val,max_val)
