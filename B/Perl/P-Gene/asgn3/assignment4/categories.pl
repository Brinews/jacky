#!/usr/bin/perl

use warnings;
use strict;

use Getopt::Long;
use lib './';
use Assignment4::MyIO qw(getFh);

###########################################################################
#   Desc:
#      Count genes number in each category.
#      Input: gene file and catagory file
#      Ouput: categories count file
###########################################################################

my $genefile="chr21_genes.txt";
my $catlogfile="chr21_genes_categories.txt";
my $help=0;

my $outFile="OUTPUT/categories.txt";

GetOptions(	"file1=s" => \$genefile, "file2=s" => \$catlogfile, "help" =>\$help,) 
    or usage();

sub usage{
	my $helpText="\nOptions:\n\t-file1\topen the chromosome 21 gene data (chr21_genes.txt)\n\t-file2\topen the chromosome 21 gene category data (chr21_genes_categories.txt)\n\t-help\tShow this message\n\n";
	print $helpText;
	return;
}

if ($help){
	usage(); exit; 
}

# Hash <categoryid, categoryHeader>
my $geneCateHash=getGeneHash({file => $catlogfile, keyCol=> 0, valCol => 1, });

# Hash <categoryid, geneCount>
my $countHash=getGeneHash({file => $genefile, keyCol => 2, headLineNum => 1, });

print "Categories result in file: $outFile";

# output <categoryid, geneCount, categoryHeader>
my $fh=Assignment4::MyIO::getFh(">",$outFile);
outputCategories($geneCateHash, $countHash, $fh);
close($fh);


###########################################################################
#   outputCategories
#input:
#	@param hashRef1    - <categoryid, categoryHeader> hash
#	@param hashRef2    - <categoryid, geneCount> hash
#	@param fd          - output file handle
###########################################################################

sub outputCategories{
	my (@refParameters)=@_;

	my $cateHash= $refParameters[0];
	my $countHash= $refParameters[1];
	my $fd= $refParameters[2];

	my %myCateHash=%$cateHash;
	my %myCountHash=%$countHash;

	foreach my $key (sort {$a <=> $b} keys(%myCountHash)){
		if (exists $myCateHash{$key}) {
			print $fd "$key\t$myCountHash{$key}\t$myCateHash{$key}\n";
		}
	}

	return;
}

###########################################################################
#	getGeneHash
#input:
#	@param file 		- input file
#	@param headLineNum	- header lines number
#	@param keyCol	    - keys in which column
#	@param valCol	    - value in which column, -1 means no value column
#output:
#   @return hash        - <key, value> hash reference
###########################################################################

sub getGeneHash{

	my ($refParameters)=@_;

	my $infile= $refParameters->{file};

	my $headLineNum= 0;
    if (exists $refParameters->{headLineNum}) {
        $headLineNum = $refParameters->{headLineNum};
    }

	my $delim= '\t';

	my $keyCol= $refParameters->{keyCol};
	my $valCol= -1;

    if (exists $refParameters->{valCol}) {
        $valCol = $refParameters->{valCol};
    }

	my $fd = Assignment4::MyIO::getFh('<' , $infile); 

	my %hash; # to return hash
    my $lineNo = 0;

	while (<$fd>) {
		chomp;
		my $line=$_;
		my @items=split($delim,$line);
		my $key;

        $lineNo = $lineNo + 1;

        if ($keyCol >= scalar(@items)) { $key = ""; }
        else { $key=$items[$keyCol]; }

		if ($lineNo > $headLineNum and $key ne "") {
			if ($valCol <0){
			    # <keys, count>
                if (exists $hash{$key}) { $hash{$key}++; }
                else { $hash{$key}=1; }
			} else {
                # <keys, value>
                $hash{$key} = $items[$valCol];
			}
		} 
	}
	close($fd);

	return \%hash;
}
