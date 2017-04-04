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
		      [^U]+
		      (?:\(?U[-\+]([0-9A-F]+)\)?)?
#		      (U)?
		      .*
		      [:=]
		      \h+
		      "((?:\x00|[^"])+)"
		      \h*
		      \|?
		  !xg) {
    my $id = $1;
    my $expect = $2;
    my $bytes = $3;
    $id =~ s!\.!_!g;
    $bytes =~ s/([\x00-\x20\x7F-\xFF])/sprintf ("\\x%02X", ord ($1))/ge;
    print "kuhn_$id = \"$bytes\";\n";
    if ($expect) {
	print "$expect\n";
    }
    else {
	print "No expect\n";
    }
}
