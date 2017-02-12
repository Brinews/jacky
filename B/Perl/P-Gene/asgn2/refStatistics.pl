#!/usr/bin/perl
use warnings;
use strict;
use Getopt::Long;
use feature qw(say);
#
#	Title - nucleotideStatisticsFromFasta.pl
#
my $infile="";
my $outfile="";
my $help=0;
GetOptions(		"infile=s"	=> \$infile,
		"outfile=s"	=> \$outfile,
		"help"		=>\$help,
	) or usage();
if($help){
	usage();
	exit; 
}
if($infile eq ""){ 
	say "\n\nDying...Make sure to give a file name of a sequence in FASTA format.";
	usage();
	die;
}
if($outfile eq ""){
	say "\n\nDying...Make sure to give an outfile for the stats";
	usage();
	die;
}

#get file handle to sequences in the fasta file
my $fhIn = getFh('<' , $infile); 

#loop over fasta filehandle and store the data in two arrays, one for the header line and the other for the sequence data
my ($refArrHeader, $refArrSeqs) = getHeaderAndSequenceArrayRefs($fhIn); 

close($fhIn);

#open two outfiles named: pdbProtein.fasta and pdbSS.fasta
my $fhOut = getFh('>' , $outfile);

#send of the array references
##process the sequences and print out the data
printSequenceStats($refArrHeader, $refArrSeqs, $fhOut);


#~~~~~~~~~~~~~~~~~~~~ SUBROUTINES ~~~~~~~~~~~~~~~~~

#-----------------------------------------------------------------------
# printSequenceStats(\@headers, \@sequences, $fileHandle);
#-----------------------------------------------------------------------
sub printSequenceStats{
	my ($refHeader, $refSeqs, $fh)=@_;
	my $ct=0;
	print $fh "Number\tAccession\tA's\tG's\tC's\tT's\tN's\tLength\tGC%\n";
	for(my $i=0;$i<scalar(@$refHeader);$i++){
		$ct++;
		my $header=$refHeader->[$i];
		my $acc = _getAccession($header);
		my $seq = $refSeqs->[$i];
		my $A = _getNtOccurrence('A', \$seq);
		my $C = _getNtOccurrence('C', \$seq);
		my $G = _getNtOccurrence('G', \$seq);
		my $T = _getNtOccurrence('T', \$seq);
		my $N = _getNtOccurrence('N', \$seq);
		my $length=length($seq);
		my $gc=0;
		if($length!=0){
			$gc=($C+$G)*100/$length;
		}
		printf $fh "$ct\t$acc\t$A\t$G\t$C\t$T\t$N\t$length\t%%%.2f\n",$gc;
	}
	#  Sample output
	#Number   Accession     A's   G's    C's    T'   N's   Length   GC%
	#1      EU521893      20     20     20     20    0      80      50
}

#-----------------------------------------------------------------------
# $count = _getNtOccurrence( $letterToCount, \$sequence);
#-----------------------------------------------------------------------
sub _getNtOccurrence{
	my ($nt, $seqRef)=@_;
	my $transliterated=$$seqRef;
        my $replace="[".lc($nt)."|".uc($nt)."]";
        $transliterated=~s/$replace//g;
	my $count=length($$seqRef)-length($transliterated);
	return $count;
}

#-----------------------------------------------------------------------
# $accessionNumber = _getAccession($header);
#-----------------------------------------------------------------------
sub _getAccession{
	my ($string)=@_;
	my ($accession)=split(" ",$string);
	return substr($accession,1,length($accession)-1);
}

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
	my ($fh,@junk)=@_;
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
	my $helpText="\n\nnucleotideStatisctsFromFasta.pl [options]\n\nOptions:\n\n\t-infile\t\tGive a the fasta sequence file name to do the splitting, this file contains two entries for each sequence, one with the protein sequence data, and one with the SS information\n\t-outfile\tProvide an output file to put the stats into\n\t-help\t\tShow this message\n\n";
	print $helpText;
	return;
}

