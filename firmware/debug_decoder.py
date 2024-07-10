import sys, serial, re, os
if (len(sys.argv) != 3):
    print("debug_decoder.py <port> <baudrate>")
    quit(1)
port = sys.argv[1]
baud = int(sys.argv[2])

abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)

##################################################################################
# Taken from PyParsing examples                                                  #
# https://github.com/pyparsing/pyparsing/blob/master/examples/cpp_enum_parser.py #
# MIT Licensed, Posted by Mark Tolonen on comp.lang.python in August, 2009       #
# modified by H3                                                                 #
##################################################################################

import pyparsing as pp

# syntax we don't want to see in the final parse tree
LBRACE, RBRACE, EQ, COMMA = pp.Suppress.using_each("{}=,")
_enum = pp.Suppress("enum")
identifier = pp.Word(pp.alphas + "_", pp.alphanums + "_")
integer = pp.Word(pp.nums)
enumValue = pp.Group(identifier("name") + pp.Optional(EQ + integer("value")))
enumList = pp.Group(enumValue + (COMMA + enumValue)[...])
enum = _enum + identifier("enum") + LBRACE + enumList("names") + RBRACE

# find instances of enums ignoring other syntax
def parse_enum(text):
    enums = {}
    for item, start, stop in enum.scan_string(text):
        entries = {}
        idx = 0
        for entry in item.names:
            if entry.value != "":
                idx = int(entry.value)
            #print(f"{item.enum.upper()}_{entry.name.upper()} = {idx}")
            entries[idx] = entry.name.upper()
            idx += 1
        enums[item.enum.upper()] = entries
    return enums

###############
# C++ parsing #
###############
dbg_codes = {}

print("Reading C++ files")

with open('BikeUSBv2/debug.h','r') as dbg_f:
    text = dbg_f.read()
    enums = parse_enum(text)
    dbg_codes = enums["DEBUG_CODE_T"]

def lookup_dbg_code(code):
    idx = int(code,16)
    try:
        return dbg_codes[idx]
    except KeyError:
        return code


fusb_registers = {
    0x01: 'FUSB_DEVICE_ID',
    0x02: 'FUSB_SWITCHES0',
    0x03: 'FUSB_SWITCHES1',
    0x04: 'FUSB_MEASURE',
    0x06: 'FUSB_CONTROL0',
    0x07: 'FUSB_CONTROL1',
    0x08: 'FUSB_CONTROL2',
    0x09: 'FUSB_CONTROL3',
    0x0A: 'FUSB_MASK',
    0x0B: 'FUSB_POWER',
    0x0C: 'FUSB_RESET',
    0x0E: 'FUSB_MASKA',
    0x0F: 'FUSB_MASKB',
    0x3C: 'FUSB_STATUS0A',
    0x3D: 'FUSB_STATUS1A',
    0x3E: 'FUSB_INTERRUPTA',
    0x3F: 'FUSB_INTERRUPTB',
    0x40: 'FUSB_STATUS0',
    0x41: 'FUSB_STATUS1',
    0x42: 'FUSB_INTERRUPT',
    0x43: 'FUSB_FIFOS'
}

#with open('/BikeUSBv2/lib/PDFriend/fusb.h','r') as reg_f:
#

def lookup_fusb_reg(reg):
    idx = int(reg,16)
    try:
        return fusb_registers[idx]
    except KeyError:
        return reg

##################
# serial parsing #
##################

dbg_re = re.compile('^([0-9A-F]{2}) => ([0-9A-F]*)', re.IGNORECASE) #0 = code, 1 = value
i2c_re = re.compile('^r([0-9A-F]{2})([RW])([0-9A-F]{2})', re.IGNORECASE) # 0 = register, 1 = R/W, 2 = value

rw_dir_sym = {'R': '->', 'W': '<='}

def parse_line(line):
    if len(line) >= 6: # len("XX -> ") == 6 == len("rXXxXX")
        dbg_match = dbg_re.match(line)
        if dbg_match is not None:
            g = dbg_match.groups()
            return "DBG %s -> %s" % (lookup_dbg_code(g[0]), g[1])
        
        i2c_match = i2c_re.match(line)
        if i2c_match is not None:
            g = i2c_match.groups()
            return "I2C {} {} {:08b}".format(lookup_fusb_reg(g[0]), rw_dir_sym[g[1].upper()], int(g[2],16))
    return "UNK " + line

print("Opening %s at %d baud" % (port,baud))
ser = serial.Serial(port,baud)
ser.timeout = 3

while True:
    line = ser.read_until()
    if len(line) > 3:
        try:
            print(parse_line(line.decode('ascii')))
        except:
            print("Decode ERROR")
