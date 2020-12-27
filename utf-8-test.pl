#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use utf8;
use FindBin '$Bin';
use File::Slurper 'read_binary';
use Text::LineNumber;

my $output = "$Bin/utf-8-test-data.c";
open my $out, ">", $output or die $!;
my $input = read_binary ("$Bin/UTF-8-test.txt");
my $tln = Text::LineNumber->new ($input);

if ($input =~ /You should see the Greek word 'kosme':\s*"(\S+)"/) {
    print $out "const char * kuhn_1 = \"$1\";\n";
}
else {
    die;
}

my %ids;
my %expect;

while ($input =~ m!
		      ^([0-9]+\.[0-9]+\.[0-9]+)
		      \s+
		      [^U]*
		      (?:\(?U[-\+]([0-9A-F]+)\)?)?
		      .*?
		      [:=]
		      \s+
		      "((?:\x00|[^"])+)"
		      \s*
		      (\||$)
		  !xgm) {
    my $id = $1;
    my $expect = $2;
    my $bytes = $3;
    $id =~ s!\.!_!g;
    if ($ids{$id}) {
	next;
    }
    $ids{$id} = hexify ($bytes);
    if ($expect) {
	$expect{$id} = $expect;
    }
}



while ($input =~ m!
		      (3\.(?:[0-9]+)\.(?:[0-9]+)).*:\s*"([^"]+)"
		  !xg) {
    my $id = $1;
    my $bytes = $2;
    $id =~ s!\.!_!g;
    if ($ids{$id}) {
	next;
    }
    $ids{$id} = hexify ($bytes);
}

for my $id (sort keys %ids) {
    print $out "const char * kuhn_$id = \"$ids{$id}\";\n";
    if ($expect{$id}) {
	print $out "uint32_t expect_$id = 0x$expect{$id};\n";
    }
}

close $out or die $!;
exit;

sub hexify
{
    my ($bytes) = @_;
    $bytes =~ s/([\x00-\x20\x7F-\xFF])/sprintf ("\\x%02X", ord ($1))/ge;
    return $bytes;
}
