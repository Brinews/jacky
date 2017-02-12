#!/usr/bin/perl
use warnings;
use strict;
use Getopt::Long;
use feature qw(say);
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
#	Title - pdbFastaSplitter.pl
#	created		9/29/2014	amacleay
#
#
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
my $infile="";
my $help=0;
GetOptions(	"infile=s"	=> \$infile,
		"help"		=>\$help,
	) or usage();
if($help){
	usage();
	exit; 
}
if($infile eq ""){
	say "\n\nDying...Make sure to give a file name of a sequence in FASTA format";
	usage();
	die;
}
#get file handle to sequences in the fasta file
my $fhIn = getFh('<' , $infile); 
#loop over fasta filehandle and store the data in two arrays, one for the header line and the other for the sequence data
my ($refArrHeader, $refArrSeqs) = getHeaderAndSequenceArrayRefs($fhIn); 

close($fhIn);
#open two outfiles named: pdbProtein.fasta and pdbSS.fasta
my $proteinFile="pdbProtein.fasta";
my $structureFile="pdbSS.fasta";
my $fhProteinOut = getFh('>' , $proteinFile);
my $fhStructureOut = getFh('>' , $structureFile);
my $protCount=0;
my $structCount=0;

#process the two arrays, go over the header array, and if it matches the amino acid sequence print it to pdbProtein.fasta, otherwise print it to pdbSS.fasta.
my $fh;
for(my $i=0;$i<scalar(@$refArrHeader);$i++){
	if($refArrHeader->[$i] =~ m/.*sequence.*/){
		$fh=$fhProteinOut;
		$protCount++;
	} else {
		$fh=$fhStructureOut;
		$structCount++;
	}
	print $fh $refArrHeader->[$i].$refArrSeqs->[$i];	
}

print STDERR "# of Protein Structures: $protCount \n# of Secondary Structures: $structCount\n";
close($fhProteinOut);
close($fhStructureOut);

#~~~~~~~~~~~~~~~~~~~~ SUBROUTINES ~~~~~~~~~~~~~~~~~

#-----------------------------------------------------------------------
# $fileHandle = getFh($lessThanOrGreaterThan , $file);
#-----------------------------------------------------------------------
sub getFh{
	my (@params)=@_;
	if(scalar(@params)<2){
		die "Subroutine getFH requires 2 parameters.\nex.getFH('<',fasta.txt')\n\n";
	}
	my $carrot=$params[0];
	my $file=$params[1];
	if($carrot ne '<' and $carrot ne '>'){
		die "First parameter in getFh must be '>' or '<'";
	}
	if($carrot eq '<'){
		unless( -e $file){
			die "File $file does not exist";
		}
	}
	my $fhandle;
	unless(open($fhandle,$carrot,$file)){
		die "Unknown error opening file $file.";
	}
	return $fhandle;

}

#-----------------------------------------------------------------------
# ($refToHeaderArray, $refToSequenceArray) = getHeaderAndSequenceArrayRefs($inputFileHandle); 
#-----------------------------------------------------------------------
sub getHeaderAndSequenceArrayRefs{
	my ($fh)=@_;
	my $line;
	my @headers;
	my @sequences;
	my $sequence;
	while(my $line=<$fh>){
		if($line =~ m/^>/){
			push(@headers,$line);
			if(defined $sequence){
				push(@sequences,$sequence);
			}
			$sequence='';
		} else {
			$sequence.=$line;
		}
	}
	push(@sequences,$sequence);
	##check to make sure data looks good
	_checkSizeOfArrayRefs (\@headers, \@sequences);

	return (\@headers,\@sequences);
}

#-----------------------------------------------------------------------
# _checkSizeOfArrayRefs( \@array1, \@array2);
#-----------------------------------------------------------------------
sub _checkSizeOfArrayRefs{
	my ($arrayRef1, $arrayRef2)=@_;
	if(scalar @$arrayRef1 != scalar @$arrayRef2){
		die "Error: Arrays should be the same size.  Exiting";
	}
	return;

}

#-----------------------------------------------------------------------
# usage();
#-----------------------------------------------------------------------
sub usage{
	my $helpText="\n\nOptions:\n\n\t-infile\tGive a the fasta sequence file name to do the splitting, this file contains two entries for each sequence, one with the protein sequence data, and one with the SS information\n\t-help\tShow this message\n\n";
	print $helpText;
	return;
}

