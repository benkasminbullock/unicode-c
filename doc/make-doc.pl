#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use utf8;
use BKB::Stuff;
use FindBin '$Bin';
use C::Tokenize ':all';
use JSON::Create 'create_json';
use Deploy 'write_ro_file';
my $in = "$Bin/../unicode.c";
my $text = whole ($in);

my %macros;
my %m2c;

$text =~ /^\s*($comment_re)/;
my $maincomment = $1;

while ($text =~ /($trad_comment_re)?\s*^\#
		 \s*
		define
		\s+
		($word_re)
		\s+
		(-?[0-9A-Fa-fx]+)
		\s*$/xsmg) {
    my $macro = $2;
    my $value = $3;
    if ($1) {
	my $comment = decomment ($1);
	$m2c{$macro} = $comment;
#	print "$macro $comment\n";
    }
    $macros{$macro} = $value;
}
my $macro_re = hash2re (\%macros);
#exit;
my @functions;
while ($text =~ /($trad_comment_re)
		 \s+
		 ^int
		 \s*
		 ($word_re)
		 \s*
		 (\([^)]+\))
		/gmsx) {
    my $function = $2;
    my $comment = decomment ($1);
    my $args = $3;
    $comment =~ s/\n\s+/ /g;
#    print "$macro_re\n";
    $comment =~ s/($macro_re)/$1 ($macros{$1})/g;
    $args =~ s/^\s*\(|\)\s*$//g;
    my @args = split /,\s*/, $args;
    push @functions, {
	name => $function,
	args => \@args,
	desc => $comment,
    };
}
my $outfile = "$Bin/doc.json";
my %doc = (
    functions => \@functions,
    description => decomment ($maincomment),
    date => scalar (gmtime ((stat ($in))[9])),
);
my $json = create_json (\%doc);
#print "$json\n";
write_ro_file ($outfile, $json);
