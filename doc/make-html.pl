#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use utf8;
use FindBin '$Bin';
use JSON::Parse 'json_file_to_perl';
use HTML::Make;
use File::Slurper 'write_text';
use Convert::Moji 'make_regex';
use Table::Readable 'read_table';
use Perl::Build qw!get_info get_commit!;
use URI::Escape 'uri_escape_utf8';
use Carp;

my $infile = "$Bin/doc.json";
my $docs = json_file_to_perl ($infile);
my $page = HTML::Make->new ('html');
my $head = $page->push ('head');
my $title = 'A C Unicode and UTF-8 library';
$head->push ('title', text => $title);
my $outfile = "$Bin/unicode.html";
my $body = $page->push ('body');
$body->push ('h1', text => $title);
$body->push ('p', text => $docs->{description});
my $info = $body->push ('table', attr => {class => 'info'});
my %pbinputs = (base => "$Bin/..");
my $pbinfo = get_info (%pbinputs);
my $table = "$Bin/data.txt";
my @data = read_table ($table);
if (@data > 1) {
    die "Too many entries in table '$table'";
}
my $data = $data[0];
my $author = $data->{author};
info_line ($info, 'author', $author);
my $commit = get_commit (%pbinputs);
my $repo = $data->{repository};
my $commit_link = make_commit_link ($commit, $repo);
info_line ($info, 'commit', $commit_link);
info_line ($info, 'date', $docs->{date});
my $email = $data->{email};
$email = make_mailto ($email);
info_line ($info, 'email', $email);
my $repolink = HTML::Make->new('a', attr => {href => $repo}, text => $repo);
info_line ($info, 'repository', $repolink->text ());

my @functions = @{$docs->{functions}};
my @macros = @{$docs->{macros}};
my @return_macros = @{$docs->{return_macros}};
my %flink = map {$_->{name} => 1} (@functions, @macros, @return_macros);
my $fre = make_regex (keys %flink);
$body->push ('h2', text => 'Functions');
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
$body->push ('h2', text => 'Return values');
for my $m (@return_macros) {
    my $mdiv = $body->push ('div', attr => {class => 'macro'});
    my $name = $m->{name};
    $mdiv->push ('h3', text => "$name ($m->{value})", attr => {id => $name});
    $mdiv->push ('p', text => improve ($m->{meaning}));
}
$body->push ('h2', text => 'Constants');
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
    $desc =~ s!\.Pp!<p>!g;
    return $desc;
}

sub info_line
{
    my ($element, $name, $contents) = @_;
    croak unless $contents;
    my $tr = $element->push ('tr');
    my $th = $tr->push ('th');
    $th->add_text (ucfirst ($name) . ':');
    $tr->push ('td', text => $contents);
}

sub make_mailto
{
    my ($emails) = @_;
    my $subject = uri_escape_utf8 ('A C Unicode and UTF-8 library');
    my $body = uri_escape_utf8 ('A C Unicode and UTF-8 library');
    my $content = "subject=$subject&body=$body";
    $emails =~ s!(\w+\@(?:\w|\.)+)!<a href='mailto:$1?$content'>$1</a>!g;
    return $emails;
}

sub make_commit_link
{
    my ($commit, $repo) = @_;
    my $c = $commit->{commit};
    my $url = "$repo/commit/$c";
    my $link = HTML::Make->new ('a', attr => {href => $url}, text => $c);
    return $link->text ();
}
