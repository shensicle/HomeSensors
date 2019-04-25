# OPCFile.py
import sys

# this is a pointer to the module object instance itself.
this = sys.modules[__name__]

this.FileName = None

# Open the OPC file for binary reading
def open_file (name):
    if (this.FileName is None):
        this.FileName = name
    else:
        msg = "There is already an OPC file open."
        raise RuntimeError(msg.format(this.FileName))

	with open(this.FileName, mode='rb') as file: # b is important -> binary
		fileContent = file.read(10)    # number of bytes in OPC header