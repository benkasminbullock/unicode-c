#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use utf8;
use FindBin '$Bin';
use File::Slurper 'read_binary';
use Text::LineNumber;
my $input = read_binary ("$Bin/UTF-8-test.txt");
my $tln = Text::LineNumber->new ($input);
while ($input =~ m!
		      ([0-9]+\.[0-9]+\.[0-9]+)
		      .*
		      [:=]
		      \s+
		      "(.+)"
		      \s*
		      \|
		  !xg) {
    my $id = $1;
    my $bytes = $2;
    $id =~ s!\.!_!g;
    $bytes =~ s/([\x80-\xFF])/sprintf ("\\x%02X", ord ($1))/ge;
    print "kuhn_$id = \"$bytes\";\n";
#    exit;
}
