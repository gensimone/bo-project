#!/bin/sh

docker run \
    --mount source=coredumps_volume,target=/cores \
    --ulimit core=-1 \
    --privileged \
    -i \
    --rm \
    --name vulnserver \
    -t vulnserver
