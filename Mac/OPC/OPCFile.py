# OPCFile.py
import sys
from struct import *
import png


#OPCImageHeaderFields = {'Version': None, 'ImageWidth': None, 'ImageHeight': None, 'BitDepth': None, 'ColourType': None, 
#	                    'Unused1': None, 'FilterMethod': None, 'ScanType': None }

OPCImageHeaderFields = ('Version', 'ImageWidth', 'ImageHeight', 'BitDepth', 'ColourType', 'Unused1', 'FilterMethod', 'ScanType')


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
    
    # Read file header (8 bytes)
    with open(this.FileName, mode='rb') as file: # b is important -> binary

        # Read OPC file header
        fileContent = file.read(8)    # number of bytes in OPC header
		
        print "File content: ",fileContent, "\n"
        if (fileContent == expectedHeader):
            print "They Match!!"
        
        # otherwise should throw an exception
 
 	    # Read image header (17 bytes)
        imageHeader = file.read(17)

        # Convert binary header to a Python list
        OPCImageHeaderValues = unpack ('3I 5B', imageHeader)

        # Create a dictionary of field names (keys) and field values
        zipObj = zip(OPCImageHeaderFields, OPCImageHeaderValues)
        OPCImageHeader = dict(zipObj)
    
        print "Now result is ", OPCImageHeader, "\n"
        
        pngFile = open ('Out.png', 'wb')
        w = png.Writer (OPCImageHeader['ImageWidth']*2, OPCImageHeader['ImageHeight'], greyscale=True, bitdepth=16)
	
        nextLine = file.read(OPCImageHeader['ImageWidth']*2);
        print "data as list is ", list(nextLine), "\n"
        
        binDataList = []
        
        while len(nextLine) != 600:
            lineAsList = list (nextLine)
            binData = [ord(lineAsList[i]) for i in range(len(lineAsList))]
            binDataList.append(binData)
            print "data list length is now ", len(binDataList), "\n"
            nextLine = file.read(OPCImageHeader['ImageWidth']*2);

        print "Full list is ", binDataList, "\n"
        w.write (pngFile, binDataList)  #write needs a list of integers
 
            
        close (pngFile)
        print "Done\n"
	
