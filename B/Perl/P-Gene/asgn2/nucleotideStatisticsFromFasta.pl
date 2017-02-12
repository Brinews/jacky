#!/usr/bin/perl
use warnings;
use strict;
use Getopt::Long;

#######################################################################
# Main Function
# Usage:
#   perl nucleotideStatisticsFromFasta.pl [-h/-help] [-infile] filename [-outfile] filename
#######################################################################

my $infile="";
my $outfile="";
my $help=0;
GetOptions( "infile=s"	=> \$infile, "outfile=s" => \$outfile,
		"help"	=> \$help,) or usageInfo();

if ($help) {
	usageInfo();
	exit; 
}

if ($infile eq "") { 
	print STDERR "Dying...Make sure to provide a file name of a sequence in FASTA format\n";
	usageInfo();
	exit;
}
if ($outfile eq "") {
	print STDERR "Dying...Make sure to provide an outfile for the stats\n";
	usageInfo();
	exit;
}

########################################################################
# usageInfo
#   print usage information
########################################################################
sub usageInfo {
	my $helpinfo="nucleotideStatisctsFromFasta.pl [options]\n\nOptions:\n\n\t-infile\tProvide a fasta sequence file name to complete the stats on\n\t-outfile\tProvide a output file to put the stats into\n\t-help\t\tShow this message\n\n";
	print STDERR $helpinfo;

	return;
}

#1. get file handle to sequences in the fasta file
my $fhOutIn = getFh('<' , $infile); 

#2. open one outfile (influenzaStats.txt)
my $fhOutOut = getFh('>' , $outfile);

#3. loop over fasta filehandle and store the data in two arrays, 
# one for the header line and the other for the sequence data
my ($refArrHeader, $refArrSeqs) = getHeaderAndSequenceArrayRefs($fhOutIn); 

#4. send of the array references,
# process the sequences and print out the data
printSequenceStats($refArrHeader, $refArrSeqs, $fhOutOut);

close($fhOutIn);
close($fhOutOut);

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
		die "Header size is not equal to sequence size";
	}

	return;
}


########################################################################
# printSequenceStats
#   @param header array reference
#   @param seq array reference
#   @param file handle of output
########################################################################
sub printSequenceStats{
	my ($refArrHeader, $refArrSeqs, $fhOut)=@_;

	print $fhOut "Number\tAccession\tA's\tG's\tC's\tT's\tN's\tLength\tGC%\n";

	my $no=1; # sequence number
    foreach my $header (@$refArrHeader) {

		my $acc = _getAccession($header);
        my $seq = pop(@$refArrSeqs);

		my $Anum = _getNtOccurrence('A', \$seq);
		my $Cnum = _getNtOccurrence('C', \$seq);
		my $Gnum = _getNtOccurrence('G', \$seq);
		my $Tnum = _getNtOccurrence('T', \$seq);
		my $Nnum = _getNtOccurrence('N', \$seq);

		my $length=length($seq);

        # calculate percentage of G and C
		my $gcPart=0;
		if($length!=0) {
			$gcPart=($Cnum+$Gnum)*100/$length;
		}

		printf $fhOut "$no\t$acc\t$Anum\t$Gnum\t$Cnum\t$Tnum\t$Nnum\t$length\t%%%.2f\n",$gcPart;
		$no++;
	}
}

########################################################################
# _getNtOccurrence
#   @param character to find
#   @param sequence data reference
########################################################################
sub _getNtOccurrence{
	my ($gene, $seqRef)=@_;

    if (index("ACGTN", $gene) eq -1) {
        die "_getNtOccurence: unknown $gene character\n";
    }

	my $seq = uc($$seqRef);
    my $count = ($seq =~ s/$gene/$gene/g);

    if ($count eq "") {
        $count = 0; # no found matched chars
    }

	return $count;
}

########################################################################
# _getAccession
#   @param header string
########################################################################
sub _getAccession {
	my ($header)=@_;
	my ($accession)=split(" ",$header);
	return substr($accession,1,length($accession)-1);
}

