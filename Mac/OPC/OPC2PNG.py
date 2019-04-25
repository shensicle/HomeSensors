
import sys
import OPCFile

if len(sys.argv) != 2:
	print "Usage: OPC2PNG <OPC file>\n"
	sys.exit()
	
fileName = sys.argv[1];
print "Processing OPC file", fileName, "\n"

OPCFile.open_file (fileName)