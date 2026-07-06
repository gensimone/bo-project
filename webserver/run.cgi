#!/usr/bin/perl

use strict;
use warnings;
use IPC::Open2;
use JSON::PP;

print "Content-Type: text/plain\n\n";

my $length = $ENV{'CONTENT_LENGTH'} || 0;
my $input = '';
read(STDIN, $input, $length);

my $req = decode_json($input);

die "invalid columns" unless $req->{columns} =~ /^\d+$/ && $req->{columns} > 0;
die "invalid width"   unless $req->{width}   =~ /^\d+$/ && $req->{width}   > 0;
die "invalid gap"     unless $req->{gap}     =~ /^\d+$/ && $req->{gap}     >= 0;
die "invalid lines"   unless $req->{lines}   =~ /^\d+$/ && $req->{lines}   > 0;

my @cmd = (
    "/opt/ftext/ftext",
    "--columns", $req->{columns},
    "--width",   $req->{width},
    "--gap",     $req->{gap},
    "--lines",   $req->{lines},
);

my ($reader, $writer);
my $pid = open2($reader, $writer, @cmd);

print $writer $req->{text};
close($writer);

print while (<$reader>);

close($reader);
waitpid($pid, 0);
