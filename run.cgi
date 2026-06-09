#!/usr/bin/perl

use strict;
use warnings;
use IPC::Open2;

print "Content-Type: text/plain\n\n";

my $length = $ENV{'CONTENT_LENGTH'};

my $input = '';
read(STDIN, $input, $length);

my ($reader, $writer);

my $pid = open2($reader, $writer, "/opt/ftext/ftext");

print $writer $input;
close($writer);

print while(<$reader>);

close($reader);

waitpid($pid, 0);
