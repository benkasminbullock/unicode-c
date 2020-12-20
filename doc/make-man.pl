#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use utf8;
use FindBin '$Bin';
use JSON::Parse 'json_file_to_perl';
use Template;

my $data = json_file_to_perl ("$Bin/doc.json");
my $tt = Template->new (
    STRICT => 1,
    ABSOLUTE => 1,
    INCLUDE_PATH => [$Bin,],
) or die "". Template->error ();
my $outfile = 'unicode.3';
$tt->process ("$outfile.tmpl", $data, $outfile, binmode => 'utf8')
    or die '' . $tt->error ();
exit;
