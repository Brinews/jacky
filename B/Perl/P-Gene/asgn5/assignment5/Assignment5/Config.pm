package Config; #1.0;
use Exporter; # 'import';
use warnings;
use strict;

#use Readonly;

########################################################################
#	Description:	
#	  Configuration module required for Assignment5
########################################################################

our @EXPORT_OK=qw(getUnigeneDirectory getUnigeneExtension getHostKeywords getErrorString4WrongNumberArguments);


# for test
#Readonly our $UNIGENE_DIR => '../';

# the directory where host files reside which contain *.unigene
our $UNIGENE_DIR = '/data/PROGRAMMING/assignment5';
our $UNIGENE_FILE_ENDING = 'unigene';

our $HOST_KEYWORDS = {
    'Mouse' => 'Mus_musculus',
    'Mus_musculus' => 'Mouse',
    'bos taurus' => 'Bos_taurus',
    'Bos_taurus' => 'Cow',
    'Cow' => 'Bos_taurus',
    'Cows' => 'Bos_taurus',
    'Human' => 'Homo_sapiens',
    'Humans' => 'Homo_sapiens',
    'homo_sapiens' => 'Homo_sapiens',
    'Homo_sapiens' => 'Human',
    'equus_caballus' => 'Equus_caballus',
    'Equus_caballus' => 'Horse',
    'Horse' => 'Equus_caballus',
    'Horses' => 'Equus_caballus',
    'ovis_aries' => 'Ovis_aries',
    'Ovis_aries' => 'Sheep',
    'Sheep' => 'Ovis_aries',
    'Sheeps' => 'Ovis_aries',
    'rattus_norvegicus' => 'Rattus_norvegicus',
    'Rattus_norvegicus' => 'Rat',
    'Rat' => 'Rattus_norvegicus',
    'Rats' => 'Rattus_norvegicus',
};

our $ERROR_STRING_FOR_BAD_NUMBER_ARGUMENTS = 'Arguments error';

sub getUnigeneDirectory {
    return $UNIGENE_DIR;
}

sub getUnigeneExtension {
    return $UNIGENE_FILE_ENDING;
}

sub getHostKeywords {
    return $HOST_KEYWORDS;
}

sub getErrorString4WrongNumberArguments {
    return $ERROR_STRING_FOR_BAD_NUMBER_ARGUMENTS;
}

1;
