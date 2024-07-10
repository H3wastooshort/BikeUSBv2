import sys, serial, re, os
if (len(sys.argv) != 3):
    print("debug_decoder.py <port> <baudrate>")
    quit(1)
port = sys.argv[1]
baud = sys.argv[2]

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
            print(f"{item.enum.upper()}_{entry.name.upper()} = {idx}")
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
    print(enums)
    dbg_codes = enums["DEBUG_CODE_T"]

def lookup_dbg_code(code):
    idx = int(code,16)
    if dbg_codes[idx] is None:
        return code
    else:
        return dbg_codes[idx]


fusb_registers = {}
#with open('/BikeUSBv2/lib/PDFriend/fusb.h','r') as reg_f:
#

def lookup_fusb_reg(reg):
    idx = int(reg,16)
    if fusb_registers[idx] is None:
        return reg
    else:
        return fusb_registers[idx]

##################
# serial parsing #
##################

print("Opening %s at %dbaud" % (port,baud))
ser = serial.Serial(port,baud)

dbg_re = re.compile('^([0-9A-F]{2}) -> ([0-9A-F]*?)\n$', re.IGNORECASE) #0 = code, 1 = value
i2c_re = re.compile('^r([0-9A-F]{2})([RW])(([0-9A-F]{2}))\n$', re.IGNORECASE) # 0 = register, 1 = R/W, 2 = value

rw_dir_sym = {'R': ' -> ', 'W': ' <= '}

def parse_line(line):
    if len(line) >= 6: # len("XX -> ") == 6
        dbg_match = dbg_re.match(line)
        if dbg_match is not None:
            g = dbg_match.groups()
            return "DBG %s -> %s" % (lookup_dbg_code(g[0]), g[1])
        
        i2c_match = i2c_re.match(line)
        if dbg_match is not None:
            g = i2c_match.groups()
            return "I2C %s %s %s" % (lookup_fusb_reg(g[0]), rw_dir_sym[upper(g[1])], g[2])
    return line

while True:
    line = ser.read_until()
    print(parse_line(line))
