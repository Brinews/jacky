#!/usr/bin/perl
use warnings;
use strict;
use Getopt::Long;

##########################################################################
# Main Function
# Usage:
#   perl pdbFastaSplitter.pl [-h/-help] [-infile] filename
##########################################################################

my $infile="";
my $help=0;

GetOptions(	"infile=s"	=> \$infile, "help"	=>\$help,) or usage();

if ($help) {
	usage();
	exit; 
}

if($infile eq "") {
	print STDERR "Dying...Make sure to provide a file name of a sequence in FASTA format\n";
	usage();
	exit;
}

########################################################################
# usage
#   print usage
########################################################################

sub usage {
	my $helpinfo="\npdbFastSplitter.pl [options]\n\n\n\nOptions:\n\n\t-infile\tProvide the fasta sequence file name to do the splitting on, this file contains\n\t\ttwo entries for each sequence, one with the protein sequence data, and one with\n\t\tthe SS information\n\t-help\tShow this message\n\n";
	print STDERR $helpinfo;
	return;
}

#1. get file handle to sequenceArr in the fasta file
my $infd = getFh('<' , $infile); 

#2. open two outfiles named: pdbProtein.fasta and pdbSS.fasta 
my $proteinfd = getFh('>' , "pdbProtein.fasta");
my $ssfd = getFh('>' , "pdbSS.fasta");

#3. loop over fasta filehandle and store the data in two arrays, 
# one for the header line and the other for the sequence data

my ($refArrHeaders, $refArrSeqs) = getHeaderAndSequenceArrayRefs($infd); 
my $proteinCnt=0;
my $ssCnt=0;

#4. process the two arrays, go over the header array, and if it matches 
# the amino acid sequence print it to pdbProtein.fasta, otherwise print 
# it to pdbSS.fasta.

my $filehandle;

foreach my $line (@$refArrHeaders) {
    if($line =~ m/.*sequence.*/){
		$filehandle=$proteinfd;
		$proteinCnt++;
	} else {
		$filehandle=$ssfd;
		$ssCnt++;
	}
    my $seq = pop(@$refArrSeqs);
	print $filehandle $line.$seq;
}

print STDERR "Found $proteinCnt protein sequences\nFound $ssCnt ss sequences\n";

close($infd);
close($proteinfd);
close($ssfd);

########################################################################
# getFh
#   @param mode how to open('<' or '>')
#   @param filename to open file name
########################################################################
sub getFh {
	my (@param)=@_;

	my $handle;

	if(scalar(@param)<2){
		die "Parameters not enough\n\n";
	}

	my $mode=$param[0];
	my $file=$param[1];

	if ($mode ne '<' and $mode ne '>'){
		die "First parameter should be '>' or '<'\n";
	}

	open($handle,$mode,$file);

	return $handle;
}

########################################################################
# getHeaderAndSequenceArrayRefs
#   @param in file handle
#   @return header array
#   @return sequence array
########################################################################
sub getHeaderAndSequenceArrayRefs{
	my ($filehandle)=@_;

	my @headerArr;
	my @sequenceArr;

	my $line;
	my $sequence;

	while (my $line=<$filehandle>){
		if ($line =~ m/^>/){
			push (@headerArr,$line);
			if (defined $sequence) {
				push(@sequenceArr,$sequence);
			}
			$sequence='';
		} else {
            # append sequence lines
			$sequence.=$line;
		}
	}

    # last sequence
    if (defined $sequence) {
        push(@sequenceArr,$sequence);
    }

	# check to make sure data looks good
	_checkSizeOfArrayRefs (\@headerArr, \@sequenceArr);

    # return reference of array
	return (\@headerArr,\@sequenceArr);
}

########################################################################
# _checkSizeOfArrayRefs
#   @param  reference of header array
#   @param  reference of seq arry
########################################################################
sub _checkSizeOfArrayRefs{
	my ($headerRef, $seqRef)=@_;

	if(scalar @$headerRef != scalar @$seqRef){
		die "_checkSizeOfArrayRefs: Header size is not equal to sequence size\n";
	}

	return;
}

