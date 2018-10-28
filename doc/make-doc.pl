#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use utf8;

use FindBin '$Bin';

# All of these modules are available on CPAN as of 2018-10-25

use C::Tokenize ':all';
use Convert::Moji 'make_regex';
use File::Slurper qw/read_text write_text/;
use JSON::Create 'create_json';
use List::UtilsBy 'sort_by';
use Text::LineNumber;

my $in = "$Bin/../unicode.c";
my $text = read_text ($in);

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
    my $meaning = $1;
    my $macro = $2;
    my $value = $3;
    if ($1) {
	my $comment = decomment ($1);
    # Remove excessive spaces from the comment and trim
    # leading/trailing whitespace.
    $comment =~ s/\n\s+/ /g;
    $comment =~ s/^\s+|\s+$//g;
	$m2c{$macro} = $comment;
#	print "$macro $comment\n";
    }
    $macros{$macro} = $value;
}
my $macro_re = make_regex (keys %macros);
#exit;
my @functions;
my $tln = Text::LineNumber->new($text);
while ($text =~ /($trad_comment_re)?
		 \s+
		 ^(int32_t|const\s+char\s+\*)
		 \s*
		 ($word_re)
		 \s*
		 (\([^)]+\))
		/gmsx) {
    my $comment = $1;
    my $type = $2;
    my $function = $3;
    my $args = $4;
    if (! $comment) {
	my $line = $tln->off2lnr (pos ($text));
	warn "$in:$line: No comment for $function.\n";
	$comment = '';
    }
    $comment = decomment ($comment);
    # Convert double spaces into paragraph markers.
    $comment =~ s/\n\s*\n/\n.Pp\n/g;
    # Remove excessive spaces from the comment and trim
    # leading/trailing whitespace.
    $comment =~ s/\n\s+/ /g;
    $comment =~ s/\.Pp\s(\S)/.Pp\n$1/g;
    $comment =~ s/^\s+|\s+$//g;
#    print "$macro_re\n";
    $comment =~ s/($macro_re)/$1 ($macros{$1})/g;
    $args =~ s/^\s*\(|\)\s*$//g;
    my @args = split /,\s*/, $args;
    push @functions, {
	name => $function,
	args => \@args,
	desc => $comment,
	type => $type,
    };
}
if (! @functions) {
warn "No functions found at all";
}
@functions = sort_by {$_->{name}} @functions;
my @macros;
for my $m (sort keys %macros) {
    if ($m =~ /_SUR_|LOW|BUFFSIZE|TEN_BITS/) {
	next;
    }
    my $meaning = $m2c{$m};
    if (! $meaning) {
	$meaning = 'Undocumented.';
    }
    push @macros, {
	name => $m,
	value => $macros{$m},
	meaning => $meaning,
    };
}
my $outfile = "$Bin/doc.json";
my %doc = (
    functions => \@functions,
    description => decomment ($maincomment),
    macros => \@macros,
    date => scalar (gmtime ((stat ($in))[9])),
);
my $json = create_json (\%doc);
#print "$json\n";
if (-f $outfile) {
    chmod 0644, $outfile or die $!;
}
write_text ($outfile, $json);
chmod 0444, $outfile or die $!;
