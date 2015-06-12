#!/usr/bin/perl -w
use strict;
use warnings;

my @notes;

printf("enum {\n");
while (<>)
{
    my ($note_name) = split(',');
    if ( $note_name =~ /\/([^\s]*)/ )
    {
        printf("NOTE_%s,\n", $1);
    }
    else
    {
        printf("NOTE_%s,\n",$note_name);
    }
}
printf("};\n");
