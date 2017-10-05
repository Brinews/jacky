#!/usr/bin/perl -w

#
# Date 2017-10-01
#

use strict;

#if (scalar(@ARGV) != 1) {
#    print "Usage: pypl <pythonfile>\n";
#    exit;
#}

#open(FILE, "<", $ARGV[0]);

my $whitespaceCounter = 0; #global variable? Its outside the loop...

my %varTokens; # store the variables
my $tabCounter = 0;
my %arrTokens;

# main entry
while (my $line = <STDIN>) {
    mainProgram($line);
}

sub mainProgram {

    my $line = $_[0];

# 1.0 translate #! line 
    if ($line =~ /^#!/ && $. == 1) {
        print "#!/usr/bin/perl -w\n";

# 2.0 Blank & comment lines (unchanged)
    } elsif ($line =~ /^\s*#/ || $line =~ /^\s*$/) {
        print $line;

# 3.0 print statement with newline
    } elsif ($line =~ /^\s*print\s*\((.*)\)[\s]*\n$/) {
        printLine($1);

# 11.0
    } elsif ($line =~ /^\s*sys\.stdout\.write\((.*)\)\s*\n$/) {
        whitespacePrinter($tabCounter);
        print "print $1;\n";

# 6.0 while statement
    } elsif ($line =~ /^\s*while\s*(.*):(.+)\n$/) {
        onelineCondStmt("while", $1, $2);

# 7.0 if statement
    } elsif ($line =~ /^\s*if\s*(.*):(.+)\n$/) {
        onelineCondStmt("if", $1, $2);

# 12.0 if .. elif .. else statement
    } elsif ($line =~ /^\s*if\s*(.*):\s*\n$/) {
        mutlineCondStmt("if", $1);

    } elsif ($line =~ /^\s*elif\s*(.*):\s*\n$/) {
        mutlineCondStmt("elsif", $1);

    } elsif ($line =~ /^\s*else\s*:\s*\n$/) {
        mutlineCondStmt("else", '');

# 8.0 for .. in statement
    } elsif ($line =~ /^\s*for\s+(.*)\s+in\s*range\s*\((.*),\s*(.*)\):\s*\n$/) {
        foreachStmt('range', $1, $2, $3);

    } elsif ($line =~ /^\s*for\s+(.*)\s+in\s*sys\.stdin:\s*\n$/) {
        foreachStmt('stdin', $1, '<STDIN>', '');

    } elsif ($line =~ /^\s*for\s+(.*)\s+in\s*sys\.argv\[1:\]:\s*\n$/) {
        foreachStmt('stdin', $1, '@ARGV', '');

# 9.0 for .. in statement
    } elsif ($line =~ /^\s*while\s*(.*):\s*\n$/) {
        mutlineCondStmt("while", $1);

#12.0 readlines 
    } elsif  ($line =~ /^\s*(.*)\s*=\s*sys\.stdin\.readlines\(\s*\)\s*$/) {
        whitespacePrinter($tabCounter);
        print "while (<STDIN>) {\n";
        whitespacePrinter($tabCounter+1);
        print "push \@$1, \$_;\n";
        whitespacePrinter($tabCounter);
        print "}\n";


# 4.0 arithmetic operations
    } elsif ($line =~ /^\s*(.*)\s*=(.*)\s*\n$/) {
        if ($line =~ /^\s*\@(.*)\s*=\s*(.*);$/) {#arrays are dealt with seperately
            next;
        } else {
            expressLine($line); # line and assignment variable
            print ";\n";
        }

# 13.0 break/continue	
    } elsif ($line =~ /^\s*break\s*$/) {
        whitespacePrinter($tabCounter);
        print "last;\n";

    } elsif ($line =~ /^\s*continue\s*$/) {
        whitespacePrinter($tabCounter);
        print "next;\n";

# 14.0 push
    } elsif ($line =~ /^\s*(.*)\.append\s*\((.*)\)\s*$/) {
        whitespacePrinter($tabCounter);
        if (exists($arrTokens{$1})) {
            $arrTokens{$1}++;
        } else {
            $arrTokens{$1} = 1;
        }

        print "push \@$1, (";
        my $ele = $2;
        expressLine($ele);
        print ");\n";
#split
    } elsif ($line =~ /^\s*(.*)\s*=\s*split\(\/(.*)\/,\s*\$(.*)\)\s*;/) {
        my $string = $3;
        my $delineator = $2;
        my $assignmentVariable = $1;
        &whitespacePrinter($whitespaceCounter);
        print "$assignmentVariable = $string.split(\"$delineator\")\n";

#join
    } elsif ($line =~ /^\s*(.*)\s*=\s*join\(\'(.*)\'\,\s*(.*)\)\s*;$/) {
        my $assignmentVariable = $1;
        my $string = $3;
        my $delineator = $2;
        &whitespacePrinter($whitespaceCounter);
        print "$assignmentVariable = '$delineator'.join([$string])";

#ARRAY HANDLING
#array conversion
    } elsif ($line =~ /^\s*(.*)\s*\@(.*)\s*(.*)\s*;$/) { #array in the line	
        my $arrayName = $2; 
        if (/^\s*(.*)\s*\@(.*)\s*=\s*((.*))\s*;$/) { #declaring the array
            $line =~ s/\@//;
            $line =~ s/\(/\[/;
            $line =~ s/\)/\]/;
            $line =~ s/\"/\'/g;
            $line =~ s/\;//;
            print "$line";
        } else { #accessing the array
            #  $arrayName[0 or whatever];
            $line =~ s/\@//;
            $line =~ s/\;//;
            print "$line";
        }
#pop
    } elsif ($line =~ /^\s*pop\s*\@(.*);$/) {
        &whitespacePrinter($whitespaceCounter);
        print "$1.pop\n";

#unshift
    } elsif ($line =~ /^\s*unshift\s*\@(.*)\,\s*(.*)\s*;$/) {
        &whitespacePrinter($whitespaceCounter);
        print "$1.unshift($2)\n";

#pop
    } elsif ($line =~ /^\s*shift\s*\@(.*);$/) {
        &whitespacePrinter($whitespaceCounter);
        print "$1.shift\n";

# Lines we can't translate are turned into comments
    } else { 
        print "#$line\n";
    }
}

sub whitespacePrinter {
	my $whitespace = $_[0];
	for (my $x = 0; $x < $whitespace; $x ++) {
		print '    ';
	}
}

# 5.0 variables translate
sub expressLine { 

    my $var = $_[0];

    if ($var =~ /^\s*(.*)\s*=/) {
        $var = $1;
        $var =~ s/\s+//g;

        # add variables to hash
        if (exists $varTokens{$var}) {
            $varTokens{$var}++;
        } else {
            $varTokens{$var} = 1;
        }
    }

	# $#
	$_[0] =~ s/len\(sys\.argv\)/\@ARGV+1/;
    $_[0] =~ s/\blen\((.*)\)/\@$1/;

    #$_[0] =~ s/\$//g;
    #$_[0] =~ s/\@//g;

    $_[0] =~ s/\n//g;
    $_[0] =~ s/sys\.stdin\.readline\(\s*\)/<STDIN>/g;

	#add $ before variables
    foreach my $key (keys %varTokens) {
        $_[0] =~ s/\b$key\b/\$$key/g;
    }

	#stdin
	$_[0] =~ s/float\((.*)\)/$1/g;
	$_[0] =~ s/int\((.*)\)/$1/g;

	#and/or/not
	$_[0] =~ s/and /\&\&/g;
	$_[0] =~ s/or /\|\|/g;
    $_[0] =~ s/not /!/g;

    #$_[0] =~ s/ == / eq /g;
    #$_[0] =~ s/ != / ne /g;
    #$_[0] =~ s/ > / gt /g;
    #$_[0] =~ s/ < / lt /g;
    #$_[0] =~ s/ >= / ge /g;
    #$_[0] =~ s/ <= / le /g;	

	#division
	$_[0] =~ s/\/\//\//g;

    # len of array
    $_[0] =~ s/\@\$/\@/;
	#add semicolon
	print $_[0];
}

# 3.0 print line
sub printLine {
    my $printInput = $_[0];

    whitespacePrinter($tabCounter);

    if ($printInput =~ /\s*"(.*)"\s*$/) {
        print "print \"$1\\n\"\;\n"
    } 
    elsif ($printInput =~ /\s*"(.*)"\s*%\s*(.*)\s*$/) {
        my $args = $2;
        foreach my $key (keys %varTokens) {
            $args =~ s/\b$key\b/\$$key/g;
        }

        my $out = "printf \"$1\\n\",$args;\n";
        print $out;
    }
    else {
        my $out = $1;
        foreach my $key (keys %varTokens) {
            $out =~ s/\b$key\b/\$$key/g;
        }
        if ($out =~ /[+*\/-]/) {
            $out = "print $out, \"\\n\"\;\n";
        } else {
            $out = "print \"$out\\n\"\;\n";
        }
        $out =~ s/sys\.argv\[(.*)\]/\$ARGV\[$1-1\]/g;
        $out =~ s/, end=\'(.*)\'\\n/$1/g;
        print $out;
    }
}

# 6.0 while statement
sub onelineCondStmt {
    my $key = $_[0];
    my $cond = $_[1];
    my $body = $_[2];

    whitespacePrinter($tabCounter);
    print "$key ";
    print "(";
    expressLine($cond);
    print ") {\n";
    # maybe multi-stmt
    my @stmt = split /;/, $body;

    $tabCounter++;

    foreach my $exp (@stmt) {
        whitespacePrinter($tabCounter);

        $exp =~ s/^\s+//;
        if ($exp =~ /^\s*print\s*\((.*)\)[\s]*$/) {
            printLine($1);
        } 
        elsif ($exp =~ /^\s*(.*)\s*=(.*)\s*$/) {
            expressLine($exp);
            print ";\n";
        }
    }

    $tabCounter--;

    whitespacePrinter($tabCounter);
    print "}\n";
}

sub foreachStmt {
    my $var = $_[1];
    my $start = $_[2];
    my $end = $_[3];

    whitespacePrinter($tabCounter);
    print "foreach \$$var ";
    
    print "(";
    if ($_[0] eq 'range') {
        expressLine($start);
        print "..";
        $end = $end."-1";
        expressLine($end);
    } elsif ($_[0] eq 'stdin') {
        print "$start";
    }
    print ")";

    print "{\n";

    if (exists $varTokens{$var}) {
        $varTokens{$var}++;
    } else {
        $varTokens{$var} = 1;
    }

    my $line = '';

    $tabCounter++;
    my $tabc = $tabCounter*4;
    my $outloop = 0;

    while ($line = <STDIN>) {

        if ($line =~ /\s{$tabc}(.+)/) {
            mainProgram($line);
        } else {
            $outloop = 1;
            last;
        }
    }
    $tabCounter--;

    if (exists $varTokens{$var}) {
        delete $varTokens{$var};
    }

    whitespacePrinter($tabCounter);
    print "}\n";

    if ($outloop == 1) {
        mainProgram($line);
    }
}

# 9.0 while statement
sub mutlineCondStmt {
    my $key = $_[0];
    my $cond = $_[1];

    whitespacePrinter($tabCounter);
    print "$key ";
    if ($cond ne '') {
        print "(";
        expressLine($cond);
        print ")";
    }
    print " {\n";

    $tabCounter++;
    my $tabc = $tabCounter*4;

    #print $tabc."==\n";
    my $line = '';
    my $outloop = 0;

    while ($line = <STDIN>) {
        if ($line =~ /\s{$tabc}(.+)/) {
            mainProgram($line);
        } else {
            $outloop = 1;
            last;
        }
    }

    $tabCounter--;

    whitespacePrinter($tabCounter);
    print "}\n";

    if ($outloop == 1) {
        mainProgram($line); # dispose the extra read line
    }
}

