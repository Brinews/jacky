package Assignment4::MyIO; 

use Exporter; # 'import';

use warnings;
use strict;

###########################################################################
#	Exported functions:
#		Assignement4::MyIO::getFh('>', $fileName)
###########################################################################

our @EXPORT_OK = qw(getFh);

###########################################################################
# getFH
#input:
#   @param mode open mode
#   @param file file name
#output:
#   @returns opened file handle
###########################################################################

sub getFh {
	my (@params)=@_;
	if(scalar(@params)<2){
		die "getFH need 2 parameters.\n";
	}

	my $mode=$params[0];
	my $file=$params[1];

	if ($mode ne '<' and $mode ne '>'){
		die "First parameter should be '>', '<'\n";
	}

	if ($mode eq '<'){
		unless( -e $file){
			die "File $file does not exist\n";
		}
	}

	my $fd;
	unless(open($fd,$mode,$file)){
		die "Error: open file $file.\n";
	}

	return $fd;
}
1;
__END__
