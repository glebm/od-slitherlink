#!/usr/bin/env bash

set -xeuo pipefail

cmake -S. -Bbuild -DPPU_X=2 -DPPU_Y=2 "$@"
cmake --build build -j $(getconf _NPROCESSORS_ONLN)
ln -sf build/slitherlink slitherlink
