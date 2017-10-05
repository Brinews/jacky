#!/usr/bin/perl
use warnings;
use strict;
use Getopt::Long;

use lib './';
use Data::Dumper;
use Assignment4::MyIO qw(getFh);

###########################################################################
#   Desc:
#      Find common gene in two file
###########################################################################

my $file1="chr21_genes.txt";
my $file2="HUGO_genes.txt";

my $help=0;

my $outFile="OUTPUT/intersectionOutput.txt";

GetOptions(	"file1=s"=> \$file1, "file2=s" => \$file2, "help" =>\$help,) or usage();

sub usage{
	my $helpText="\nOptions:\n\t-file1\topen the chromosome 21 gene data (chr21_genes.txt)\n\t-file2\topen the HUGO genes data file (HUGO_genes.txt)\n\t-help\tShow this message\n\n";
	print $helpText;
	return;
}

if($help){ usage(); exit; }

my $arr1=readinGene($file1);
my $arr2=readinGene($file2);

my $fd= Assignment4::MyIO::getFh(">",$outFile);

print "Output is in $outFile\n";
findCommonGene({arrRef1 => $arr1, arrRef2 => $arr2, outfile => $fd, });

close($fd);

###########################################################################
#	readinGene
#input:
#	file 		- input file
#output:
#   array       - gene name array
###########################################################################

sub readinGene{

	my ($infile)=@_;
	my $delim= '\t';

	my $fd = Assignment4::MyIO::getFh('<' , $infile); 

	my @array; # to return array
    my $lineno = 0;

	while(<$fd>){
		chomp;
		my $line=$_;
		my @lineArray=split($delim,$line);

		if ($lineno > 0) {
		    my $value=$lineArray[0];

			if($value ne ""){
				push(@array,$value);
			}
		} 
        $lineno = $lineno+1;
	}

	close($fd);

	return \@array;
}

###########################################################################
#	findCommonGene
#input:
#   @param array1 - gene name array 1
#   @param array2 - gene name array 2
#output:
#   @return void
###########################################################################

sub findCommonGene {

	my ($arrays)=@_;
	my $arrRef1= $arrays->{arrRef1};
	my $arrRef2= $arrays->{arrRef2};
    my $fd = $arrays->{outfile};

	my @arr1=@$arrRef1;
	my @arr2=@$arrRef2;

    my %bothHash;

	foreach my $gene(@arr1){
        $bothHash{$gene} = 1;
	}

    foreach my $gene1(@arr2) {
        if (exists $bothHash{$gene1}) {
            $bothHash{$gene1} = 2;
        }
    }

    my $common = 0;
    foreach my $key (sort {$a cmp $b} keys(%bothHash)) {
        if ($bothHash{$key} > 1) {
            print $fd $key."\n";
            $common = $common+1;
        }
    }

    print "\nFound: $common common genes.\n";
}

