#!/home/ben/software/install/bin/perl
use warnings;
use strict;
use utf8;
use FindBin '$Bin';
use File::Slurper 'read_text';
use List::Util 'uniq';
my @files = <$Bin/*.pl>;
my $self = __FILE__;
@files = grep !/\Q$self/, @files;
#print "@files\n";
my @use;
for my $file (@files) {
    my $text = read_text ($file);
    while ($text =~ /^\s*use\s+((?:\w+|:)+)(\s|;)/gsm) {
	push @use, $1;
    }
}
# FindBin and the others come with Perl
@use = grep !/warnings|utf8|strict|FindBin/, @use;
for (@use) {
    if (/^[a-z]/) {
	warn "Possible pragma $_ in use";
    }
}
@use = sort (@use);
@use = uniq (@use);
print "cpanm @use\n";
