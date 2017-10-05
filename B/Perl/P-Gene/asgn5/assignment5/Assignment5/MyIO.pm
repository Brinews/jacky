package MyIO;

use Exporter;

use warnings;
use strict;
use feature qw(say);

our @EXPORT_OK = qw(getFh);

########################################################################
# $fileHandle = getFh($lessThanOrGreaterThan , $file);
#
# returns a file handle.
########################################################################

sub getFh{
	my (@params)=@_;

	if(scalar(@params)<2){
		die "Requires 2 parameters.\nex.getFH('<',fasta.txt')\n\n";
	}

	my $carrot=$params[0];
	my $file=$params[1];

	unless($carrot=~/^(<|>{1,2})$/){
		die "First parameter must be '>', '<', '>>', or '<<'";
	}

	if($carrot eq '<'){
		unless( -e $file){
			die "File $file does not exist";
		}
	}

	my $fhandle;

	unless(open($fhandle,$carrot,$file)){
		die "Error opening file $file.";
	}
	return $fhandle;

}
1;
__END__
