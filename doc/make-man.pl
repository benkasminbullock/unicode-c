#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use utf8;
use FindBin '$Bin';
use JSON::Parse 'read_json';
use Template;

my $data = read_json ("$Bin/doc.json");
my $tt = Template->new (
    STRICT => 1,
    ABSOLUTE => 1,
    INCLUDE_PATH => [$Bin,],
) or die "". Template->error ();
my $outfile = 'unicode.3';
$tt->process ("$outfile.tmpl", $data, $outfile, binmode => 'utf8')
    or die '' . $tt->error ();
exit;
