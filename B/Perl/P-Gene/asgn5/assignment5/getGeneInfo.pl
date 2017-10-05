#! /usr/bin/perl
return 1 if (caller()); # for testing

use warnings;
use strict;

use Getopt::Long;

#use lib './';

use Assignment5::Config;
use Assignment5::MyIO;

######################################################################
# Desc:
#  	Gets a list of tissues where this gene is expressed in the 
#  	given host.
######################################################################

my $host='Human'; # default
my $gene='TGM1'; # default
my $help=0;

my $usage = "\ngetGeneInfo.pl [options]\n\n\nOptions:\n\n
    -host     Name of the host
    -gene     Gene of interest
    -help     Show this message
	";

sub myusage {
    print $usage;
    return;
}

GetOptions('host=s' => \$host, 'gene=s' => \$gene, help => $help,) or myusage();

if ($help) {
    myusage(); exit;
}

$host = makeFirstCapital($host);

my $folder = modifyHostName($host);
my $file = join("/", Config::getUnigeneDirectory(), $folder, $gene . '.' . Config::getUnigeneExtension());

#print($file);

if ( isValidGeneName($file) ){
    my $myfolder=$folder;
    $myfolder=~s/_/ /;
	print "\nFound Gene $gene for $myfolder\n";

    my $infoRef=getGeneData($gene, $host);
    printOutput($gene,$host,$infoRef);

} else{
    # gene not found
	print "\nNo found $file, exit\n";
	exit;
}

########################################################################
# getGeneData:
#    @param $gene name
#    @param $host name
#    @return array reference
########################################################################
sub getGeneData{
	my ($gene, $host)=@_;

    my $folder = modifyHostName($host);
    my $file = join("/", Config::getUnigeneDirectory(), $folder, $gene . '.' . Config::getUnigeneExtension());

	my $tissues='';

	my @tissueArray;
	my %tissueHash;

	my $input=MyIO::getFh('<',$file);

	while (<$input>){
		chomp;
		my $text=$_;
		@tissueArray=();

		if($text=~/^EXPRESS\s+(.*)/){
		   if(defined $1){
			@tissueArray=split('\|',$1);
		   }
		}

		foreach my $tissue (@tissueArray){
			$tissue=~s/^\s//;
			$tissueHash{lc($tissue)}=1;
		}
	}

	my $i=0;
	@tissueArray=sort {$a cmp $b} keys %tissueHash;

	if(scalar @tissueArray == 0){
		die "No tissues found in file.";
	}

	my $arrRef=\@tissueArray;

	return $arrRef;
}

########################################################################
# printOutut($arrayRef, $geneName, $host)
#     @param $gene name
#     @param $host name
#     @param $arrayRef
########################################################################

sub printOutput{
	my ($geneName,$host,$arrayOutputRef)=@_;
	$host=~s/_/ /;
	my @tissues=@$arrayOutputRef;

	print "In $host, There are ".scalar(@tissues)." tissues that $geneName is expressed in:\n";

	for(my $i=0;$i<scalar(@tissues);$i++){
		printf "%4d. %s\n", $i+1, makeFirstCapital($tissues[$i]);
	}

	print "\n";

	return;
}

sub getCommonName {

	#expect the folder name, ex. Homo_sapiens
	my ($scientific)=@_;
	my $common='';
	my $nameRef=Config::getHostKeywords();
	my %names=%$nameRef;

	unless(exists $names{$scientific}){
		return undef;
	}

	$common=$names{$scientific};

	return $common;
}

########################################################################
# _printHostDirectoriesWhichExist
#       It displays a list of existing directories.  
########################################################################

sub _printHostDirectoriesWhichExist{
	my $ct=0;
	print "\nEither the Host Name you are searching for is not in the database\n \
or If you are trying to use the scientific name please put the name in double quotes:\n\
\"Scientific name\"";
	print "\nHere is a (non-case sensitive) list of available Hosts by scientific name:\n";
	my $unigeneDir=Config::getUnigeneDirectory();

	opendir(DIR, $unigeneDir) or die "$unigeneDir does not exist. Exiting.\n";
	my @dirs=readdir(DIR);
	my @sorted=sort {lc($a) cmp lc($b)} @dirs;

	# Print list of scientific names
	foreach my $file ( @sorted){
		if($file!~m/\./){
			printf "%3d.\t%s\n", ++$ct, makeFirstCapital($file);
		}
	}

	$ct=0;
	print "\nHere is a (non-case sensitive) list of available Hosts by common name:\n";

	# Print list of Common Names
	foreach my $file (@sorted){
		if($file!~m/\./){
			my $commonName = getCommonName($file);
			if(!defined $commonName){
				$commonName='None stored';
			}
			printf "%3d.\t%s\n", ++$ct,$commonName;
		}
	}
	die;
	return;
}

########################################################################
# isValidGeneName:
#   @param $file file name
#   @return boolean exist or not
########################################################################

sub isValidGeneName{
	my ($geneFile)=@_;
	if(-e $geneFile){
        return 1;
	}
    return 0;
}

########################################################################
#	Uses the SpeciesNames module to return the appropriate folder
#	name based on the host input
########################################################################
sub modifyHostName {
    my ($scientific)=@_;

	my $common='';
	my $nameRef=Config::getHostKeywords();
	my %names=%$nameRef;

	unless(exists $names{$scientific}){
        _printHostDirectoriesWhichExist(); 
		return undef;
	}

	$common=$names{$scientific};

	return $common;
}

########################################################################
# makeFirstCapital:
#	   make first letter capital
########################################################################
sub makeFirstCapital {
	my ($string)=@_;
	return uc(substr($string, 0, 1)).lc(substr($string,1,length($string)-1));
}
