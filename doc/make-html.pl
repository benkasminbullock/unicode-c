#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use utf8;
use FindBin '$Bin';
use JSON::Parse 'json_file_to_perl';
use HTML::Make;
use File::Slurper 'write_text';
use Convert::Moji 'make_regex';
my $infile = "$Bin/doc.json";
my $docs = json_file_to_perl ($infile);
my $page = HTML::Make->new ('html');
my $head = $page->push ('head');
my $title = 'A C Unicode and UTF-8 library';
$head->push ('title', text => $title);
my $outfile = "$Bin/unicode.html";
my $body = $page->push ('body');
$body->push ('h1', text => $title);
$body->push ('h2', text => 'Functions');
my @functions = @{$docs->{functions}};
my @macros = @{$docs->{macros}};
my %flink = map {$_->{name} => 1} (@functions, @macros);
my $fre = make_regex (keys %flink);
for my $f (@functions) {
    my $name = $f->{name};
    my $fdiv = $body->push ('div', attr => {class => 'function'});
    $fdiv->push ('h3', text => $name, attr => {id => $name});
    my $fpre = $fdiv->push ('pre', text => "int <b>$name</b> (");
    $fpre->add_text (join ', ', @{$f->{args}});
    $fpre->add_text (");\n");
    my $desc = improve ($f->{desc});
    $fdiv->push ('p', text => $desc);
}
$body->push ('h2', text => 'Constants and return values');
for my $m (@macros) {
    my $mdiv = $body->push ('div', attr => {class => 'macro'});
    my $name = $m->{name};
    $mdiv->push ('h3', text => "$name ($m->{value})", attr => {id => $name});
    $mdiv->push ('p', text => improve ($m->{meaning}));
}
write_text ($outfile, $page->text ());
system ("cp -f $outfile /usr/local/www/data/");
exit;

sub improve
{
    my ($desc) = @_;
    $desc =~ s!$fre!<a href="#$1">$1</a>!g;
    $desc =~ s!"((?:\*+\s*)?[\w\d_]+)"!<code>$1</code>!g;
    $desc =~ s!<code>(\*+)\s+([\w\d_]+)</code>!<code>$1$2</code>!g;
    $desc =~ s,\b(https?://[-\w/~:@.%#+$?=]+\w),<A HREF=\"$1\">$1</A>,g;
    $desc =~ s!\n\n!\n</p>\n<p>\n!g;
    return $desc;
}
