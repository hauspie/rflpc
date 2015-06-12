#!/usr/bin/perl -w
use strict;
use warnings;

my @notes;

printf("int notes_to_freq[] = {\n");
while (<>)
{
    my ($note_name, $note_freq) = split(',');
    printf("%d, /* %s */\n", $note_freq * 1000, $note_name);
}
printf("};\n");
