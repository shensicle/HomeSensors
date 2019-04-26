# OPCFile.py
import sys
from struct import *


OPCImageHeader = {'Version': None, 'ImageWidth': None, 'ImageHeight': None, 'BitDepth': None, 'ColourType': None, 
	              'Unused1': None, 'FilterMethod': None, 'ScanType': None }


# this is a pointer to the module object instance itself.
this = sys.modules[__name__]

this.FileName = None  # Don't need this to be module global - file handle should be though

# Open the OPC file for binary reading
def open_file (name):
    if (this.FileName is None):
        this.FileName = name
    else:
        msg = "There is already an OPC file open."
        raise RuntimeError(msg.format(this.FileName))

    expectedHeader = "\x89OPC\x0d\x0a\x1a\x0a"
    
    # Read file header (8 bytes? Maybe 10)
    with open(this.FileName, mode='rb') as file: # b is important -> binary

        fileContent = file.read(8)    # number of bytes in OPC header
		
        print "File content: ",fileContent, "\n"
        if (fileContent == expectedHeader):
            print "They Match!!"
        
        # otherwise should throw an exception
 
 	    # Read image header (17 bytes)	
        imageHeader = file.read(17)

#        OPCImageHeader = unpack ('3I 5B', imageHeader)

        print "Now result is ", OPCImageHeader, "(", OPCImageHeader[1], ")\n"
	
#        nextLine = file.read(OPCImageHeader[1]);
	
#	with open("myfile", "rb") as f:
#    byte = f.read(1)
#    while byte != "":
        # Do stuff with byte.
#        byte = f.read(1)