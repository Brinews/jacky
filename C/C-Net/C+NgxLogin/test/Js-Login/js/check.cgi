#!/usr/bin/perl -w
use CGI;
use strict;
use warnings;

# read the CGI params
my $cgi = CGI->new;
my $firstname = $cgi->param("firstname");
my $name = $cgi->param("name");

my $completename = $firstname . $name;

print $cgi->header('text/plain;charset=UTF-8');
print $completename;

