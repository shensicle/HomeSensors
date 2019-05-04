package OPCFile;
 
use strict;
use warnings;
 
our %ImageConfig = {Version => undef, ImageWidth => undef, ImageHeight => undef, BitDepth => undef,
                     ColourType => undef, Unused1 => undef, FilterMethod => undef, ScanType => undef };

# init with file name
sub new
{
    my ($class,$fn) = @_;
  
    my $self = {_fileName => $fn};
    bless $self, $class
}

# ==============================================================================
# Open the file and validate the file header. Also read capture parameters from
# the image header
sub open
{
   my $self = shift; # object reference
 
   my $expectedHeader = pack('CCCCCCCC', 0x89, ord 'O', ord 'P', ord 'C', 0x0d, 0x0a, 0x1a, 0x0a);
   open  $self->{_fh}, '<:raw', $self->{_fileName};

   # OPC file header is 8 bytes
   my $bytes_read = read $self->{_fh}, my $bytes, 8;
   if ($bytes_read != 8) {
        die 'Unable to read OPC file header';
   }
   
   # Fail if the header is not intact
   die 'OPC file header is invalid' unless $bytes eq $expectedHeader;
   
   # Next read the image configuration
   $bytes_read = read $self->{_fh}, $bytes, 17;
   die 'Unable to read image configuration' unless $bytes_read == 17;
  
  
   ($ImageConfig{Version}, $ImageConfig{ImageWidth}, $ImageConfig{ImageHeight}, 
    $ImageConfig{BitDepth}, $ImageConfig{ColourType}, $ImageConfig{Unused1},
    $ImageConfig{FilterMethod}, $ImageConfig{ScanType})  = unpack( "IIICCCCC", "$bytes" );
    
    # FIX BIT DEPTH FOR NOW - DELETE THIS CODE
    $ImageConfig{BitDepth} = ($ImageConfig{BitDepth}+1)*8;
    
   $self->{_imageConfig} = \%ImageConfig; 
    
   return %ImageConfig;
}

# ==============================================================================
sub get_rows
{
   my $self = shift; # object reference
   
   my $config = $self->{_imageConfig};
   my $width = $config->{ImageWidth};
   my $height = $config->{ImageHeight};
   my $depth = $config->{BitDepth};
   my @rowList;
  
   my $rowCount = 0;
   while ($rowCount < $height)
   {
       read $self->{_fh}, $rowList[$rowCount], $width*2;
       
       # Account for ZigZag encoding - CHECK SCAN TYPE IS ZERO - SHOULD BE 1
       if (0 == $rowCount % 2) # && ($config->{ScanType} == 0))
       {
           $rowList[$rowCount] = scalar reverse $rowList[$rowCount]
       }
       $rowCount ++;
   }

   return @rowList;
}


# ==============================================================================
sub close
{
   my $self = shift; # object reference
   close  $self->{_fh};
}


1;