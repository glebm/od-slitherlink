#!/usr/bin/env bash

set -euo pipefail
source ./targets.sh

usage() {
  echo "Usage: package-opk.sh <target> [build dir] [output OPK path]"
  usage_target
}

if ! check_target "$@"; then
  usage
  exit 64
fi

declare -r TARGET="${1}"
declare -r BUILD_DIR="${2:-"build-${TARGET}"}"
declare -r OUT="${3:-"$BUILD_DIR/slitherlink-${TARGET}.opk"}"

main() {
  local ext="$TARGET"
  if [[ $TARGET == rg350 ]]; then
    ext=gcw0
  fi
  local -a files=(
    "opkg/default.$ext.desktop"
    opkg/slitherlink.png
    res/*.*
    "$BUILD_DIR/slitherlink"
    "$OUT"
  )

  set -x
  mksquashfs "${files[@]}" \
    -all-root -no-xattrs -noappend -no-exports
}

main
