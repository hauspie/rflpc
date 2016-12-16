#!/usr/bin/perl -w
use strict;


die "usage: $0 <peripheral_clock_in_Hz> <desired_baud_rate>\n" if (@ARGV !=2 );


my $pclk = shift;
my $br = shift;

my %div = (
    1.0 => "0/1",
    1.000 => "0/1",
    1.250 => "1/4",
    1.500 => "1/2",
    1.750 => "3/4",
    1.067 => "1/15",
    1.267 => "4/15",
    1.533 => "8/15",
    1.769 => "10/13",
    1.071 => "1/14",
    1.273 => "3/11",
    1.538 => "7/13",
    1.778 => "7/9",
    1.077 => "1/13",
    1.286 => "2/7",
    1.545 => "6/11",
    1.786 => "11/14",
    1.083 => "1/12",
    1.300 => "3/10",
    1.556 => "5/9",
    1.800 => "4/5",
    1.091 => "1/11",
    1.308 => "4/13",
    1.571 => "4/7",
    1.818 => "9/11",
    1.100 => "1/10",
    1.333 => "1/3",
    1.583 => "7/12",
    1.833 => "5/6",
    1.111 => "1/9",
    1.357 => "5/14",
    1.600 => "3/5",
    1.846 => "11/13",
    1.125 => "1/8",
    1.364 => "4/11",
    1.615 => "8/13",
    1.857 => "6/7",
    1.133 => "2/15",
    1.375 => "3/8",
    1.625 => "5/8",
    1.867 => "13/15",
    1.143 => "1/7",
    1.385 => "5/13",
    1.636 => "7/11",
    1.875 => "7/8",
    1.154 => "2/13",
    1.400 => "2/5",
    1.643 => "9/14",
    1.889 => "8/9",
    1.167 => "1/6",
    1.417 => "5/12",
    1.667 => "2/3",
    1.900 => "9/10",
    1.182 => "2/11",
    1.429 => "3/7",
    1.692 => "9/13",
    1.909 => "10/11",
    1.200 => "1/5",
    1.444 => "4/9",
    1.700 => "7/10",
    1.917 => "11/12",
    1.214 => "3/14",
    1.455 => "5/11",
    1.714 => "5/7",
    1.923 => "12/13",
    1.222 => "2/9",
    1.462 => "6/13",
    1.727 => "8/11",
    1.929 => "13/14",
    1.231 => "3/13", 
    1.467 => "7/15",
    1.733 => "11/15",
    1.933 => "14/15" );


if ($pclk % (16 * $br))
{
    # not integer
    my $fr = 1.5;
    my $dl;
    do
    {
        $dl = int($pclk/(16 * $br * $fr));
        $fr = $pclk / (16 * $br * $dl);
    } while ($fr < 1.1 || $fr > 1.9);
    foreach my $k (keys(%div))
    {
        if (abs($fr-$k) < 0.005)
        {
            print("DIVADD/MULVAL = " . $div{$k}." (computed fr: $fr)\n");
            last;
        }
    }
    my $dlm = ($dl & 0xff00) >> 8;
    my $dll = $dl & 0xff;
    print "DLM = $dlm\n";
    print "DLL = $dll\n";
}
else
{
    # integer
    print "DIVADD = 0\n";
    print "MULVAL = 1\n";
}