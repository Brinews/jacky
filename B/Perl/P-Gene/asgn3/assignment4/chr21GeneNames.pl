#!/usr/bin/perl

use warnings;
use strict;

use Getopt::Long;

use lib './';
use Assignment4::MyIO qw(getFh);

###########################################################################
#   Desc: 
#       Search the description for given gene by user
###########################################################################

my $infile="chr21_genes.txt";
my $help=0;

GetOptions(	"file=s" => \$infile, "help" =>\$help,) or usage();

sub usage {
	my $helpText="\nOptions:\n\t-file\topen the chromosome 21 gene data (chr21_genes.txt)\n\t-help\tShow this message\n";
	print $helpText;
	return;
}

if($help) { usage(); exit; }

# read hash from gene file
my $hashRef= getGeneHash($infile);
my %genes=%$hashRef;

my $userInput="";

# main loop - prompt user to enter a gene symbol
while (1) {

	print "Enter gene name of interest. Type quit to exit: ";

	$userInput=<STDIN>;
	chomp $userInput;
	$userInput=uc($userInput);

	if(lc($userInput) eq "quit"){
        exit;
	}

	if (exists $genes{$userInput}) {
        print "$userInput found! Here is the description:\n";
		print "$genes{$userInput}\n";
	}
	else{
		print STDERR "$userInput not found!\n";
	}
}

###########################################################################
# getGeneHash
#   @param readin file name
#   @return gene hash reference
###########################################################################
sub getGeneHash {

	my ($infile)=@_;
	my $fd = Assignment4::MyIO::getFh('<' , $infile); 

	my %hash; # gene hash<name, description>

	while(<$fd>){
		chomp;
		my $line=$_;
		while ($line=~/(.*?)\t(.*?)\t.*?$/g) {
            my $key=$1;
            $hash{uc($key)}=$2;
		}
	}

	close($fd);

	return \%hash;
}

