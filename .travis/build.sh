#!/bin/sh

cd /build
./configure                     \
    --prefix=/usr/local         \
    --datadir=/usr/local/share  \
    --sysconfdir=/etc
make
