#!/usr/bin/perl -w

use Image::PNG::Libpng ':all';
use Image::PNG::Const ':all';     # just in case
use File::Basename;

use OPCFile;

my $inputFile = $ARGV[0];


my $opcFile = new OPCFile ($inputFile);

# Open the OPC file and verify the file header and the image configuration header
my %imageConfig = $opcFile->open();

print "Image configuration is "; print "$_ $imageConfig{$_}\n" for keys %imageConfig; print "\n";

# Each row in the image is returned as a binary string in this array
@rows = $opcFile->get_rows();

# Done with the OPC file
$opcFile->close();

# Make an output file name from the input file name by replacing the .OPC extension with .png
my $outputFile = $inputFile;
$outputFile =~ s/\.OPC$/\.png/g;


# On to the PNG output
my $png = create_write_struct ();

my $ihdr = { width => $imageConfig{ImageWidth}, height => $imageConfig{ImageHeight}, bit_depth => $imageConfig{BitDepth},
             color_type => $imageConfig{ColourType} };

$png->set_IHDR ($ihdr);
$png->set_rows (\@rows);

open my $output, ">:raw", $outputFile;
$png->init_io ($output);
$png->write_png ();
close $output;

