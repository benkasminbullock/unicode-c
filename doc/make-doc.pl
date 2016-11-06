#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use utf8;
use BKB::Stuff;
use FindBin '$Bin';
use C::Tokenize ':all';
my $in = "$Bin/../unicode.c";
my $text = whole ($in);

my %macros;
my %m2c;

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
	print "$macro $comment\n";
    }
    $macros{$macro} = $value;
}
my $macro_re = hash2re (\%macros);
#exit;
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
    print "Function:$function $args\nComment: $comment\n\n";
}
