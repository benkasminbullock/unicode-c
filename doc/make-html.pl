#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use utf8;
use feature 'signatures';
no warnings 'experimental::signatures';
use FindBin '$Bin';
use JSON::Parse 'read_json';
use HTML::Make;
use HTML::Make::Page 'make_page';
use File::Slurper 'write_text';
use Convert::Moji 'make_regex';
use Table::Readable 'read_table';
use Perl::Build qw!get_info get_commit!;
use URI::Escape 'uri_escape_utf8';
use Carp;
use Template;

my $infile = "$Bin/doc.json";
my $docs = read_json ($infile);
my $title = 'A C Unicode and UTF-8 library';
my ($page, $body) = make_page (title => $title);
my $outfile = "$Bin/unicode.html";
$body->push ('h1', text => $title);
$body->push ('p', text => $docs->{description}, class => 'description');
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
info_line ($info, 'date', $commit->{date});
my $email = $data->{email};
$email = make_mailto ($email);
info_line ($info, 'email', $email);
info_line ($info, 'licence', 'BSD 3 Clause, GNU GPL, Perl Artistic');
my $repolink = HTML::Make->new (
    'a',
    attr => {href => $repo},
    text => $repo,
);
info_line ($info, 'repository', $repolink->text ());

my @functions = @{$docs->{functions}};
my @macros = @{$docs->{macros}};
my @return_macros = @{$docs->{return_macros}};
my %flink = map {$_->{name} => 1} (@functions, @macros, @return_macros);
my $fre = make_regex (keys %flink);
my $nav = $body->push ('nav', id => 'contents');
$nav->push ('h2', text => 'Contents', id => 'contents-header');

my $ul = $nav->push ('ul', id => 'contents-list');

contents_header ($ul, 'Functions', 'functions', \@functions);
contents_header ($ul, 'Return values', 'return-values', \@return_macros);
contents_header ($ul, 'Constants', 'constants', \@macros);
contents_header ($ul, 'See also', 'see-also');

my $main = $body->push ('main');

$main->push ('h2', text => 'Functions', id => 'functions');
for my $f (@functions) {
    my $name = $f->{name};
    my $fdiv = $main->push ('section', attr => {class => 'function'});
    $fdiv->push ('h3', text => $name, attr => {id => $name});
    my $fpre = $fdiv->push (
	'pre',
	text => "$f->{type} <b>$name</b> (",
	class => 'prototype',
    );
    $fpre->add_text (join ', ', @{$f->{args}});
    $fpre->add_text (");\n");
    my $desc = improve ($f->{desc});
    $fdiv->push ('p', text => $desc);
}

$main->push ('h2', text => 'Return values', id => 'return-values');
for my $m (@return_macros) {
    my $mdiv = $main->push ('section', attr => {class => 'macro'});
    my $name = $m->{name};
    $mdiv->push ('h3', text => "$name ($m->{value})", attr => {id => $name});
    $mdiv->push ('p', text => improve ($m->{meaning}));
}

$main->push ('h2', text => 'Constants', id => 'constants');
for my $m (@macros) {
    my $mdiv = $main->push ('section', attr => {class => 'macro'});
    my $name = $m->{name};
    $mdiv->push ('h3', text => "$name ($m->{value})", attr => {id => $name});
    $mdiv->push ('p', text => improve ($m->{meaning}));
}
my $see_also;
my $tt = Template->new (
    STRICT => 1,
    ABSOLUTE => 1,
    INCLUDE_PATH => [$Bin],
    ENCODING => 'UTF-8',
);
my %vars;
$vars{data} = $data;
$tt->process ('see-also.html.tmpl', \%vars, \$see_also, encoding => 'utf8')
    or die '' . $tt->error ();
$body->add_text ($see_also);
write_text ($outfile, $page->text ());
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
    my $tr = $element->push ('tr', class => 'info-row');
    my $th = $tr->push ('th', class => 'info-header');
    $th->add_text (ucfirst ($name));
    $tr->push ('td', text => $contents, class => 'info-contents');
}

sub make_mailto
{
    my ($emails) = @_;
    my $subject = uri_escape_utf8 ('A C Unicode and UTF-8 library');
    my $main = uri_escape_utf8 ('A C Unicode and UTF-8 library');
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

sub contents_header ($ul, $title, $key, $list = undef)
{
    my $li = $ul->push ('li', class => 'contents-item');
    $li->push ('a', text => $title, href => "#$key");
    if ($list) {
	contents_list ($li, $list);
    }
}

sub contents_list ($li, $list)
{
    my $ulc = $li->push ('ul');
    for my $item (@$list) {
	my $li = $ulc->push ('li', class => 'contents-item');
	my $name = $item->{name};
	$li->push ('a', text => $name, href => "#$name");
    }
}
