use strict;
use warnings;
use ExtUtils::MakeMaker;

my $pm = 'lib/Unicode/C.pm';
my $repo = 'https://github.com/benkasminbullock/Unicode-C';

WriteMakefile (
    NAME => 'Unicode::C',
    VERSION_FROM => $pm,
    ABSTRACT_FROM => $pm,
    AUTHOR => 'Ben Bullock <bkb@cpan.org>',
    LICENSE => 'perl',
    PREREQ_PM => {
    },
    META_MERGE => {
        resources => {
            repository => $repo,
            bugtracker => "$repo/issues",
        },
    },
    OBJECTS => "C.o unicode.o"
);
