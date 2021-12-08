#!/usr/bin/env bash

set -euo pipefail
source ./targets.sh

usage() {
  echo "Usage: build.sh [target]"
  usage_target
}

if ! check_target "$@"; then
  usage
  exit 64
fi

declare -r TARGET="${1}"

check_buildroot() {
  if ! [[ -d $BUILDROOT ]]; then
    echo "Please set the BUILDROOT environment variable"
    exit 1
  fi
}

make_buildroot() {
  cd "$BUILDROOT"
  # Check dependencies manually as it's much faster than `make`.
  local -a deps=()
  if ! [[ -f output/staging/usr/include/SDL/SDL.h ]]; then
    deps+=(sdl)
  fi
  if ! [[ -f output/staging/usr/include/SDL/SDL_image.h ]]; then
    deps+=(sdl_image)
  fi
  if ! [[ -f output/staging/usr/include/SDL/SDL_mixer.h ]]; then
    deps+=(sdl_mixer)
  fi
  if ! [[ -f output/staging/usr/include/SDL/SDL_ttf.h ]]; then
    deps+=(sdl_ttf)
  fi
  if ! [[ -f output/host/usr/share/buildroot/toolchainfile.cmake ]]; then
    deps+=(toolchain)
  fi
  if (( ${#deps[@]} )); then
    make "${deps[@]}" BR2_JLEVEL=0
  fi
  TOOLCHAIN="$BUILDROOT/output/host"
  cd -
}

build() {
  BUILD_DIR="build-$TARGET"
  cmake -S. -B"$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DTARGET_PLATFORM="$TARGET" \
    -DCMAKE_TOOLCHAIN_FILE="${TOOLCHAIN}/usr/share/buildroot/toolchainfile.cmake"
  cmake --build "$BUILD_DIR" -j $(getconf _NPROCESSORS_ONLN)
}

strip_bin() {
	"${TOOLCHAIN}/usr/bin/"*-linux-strip "${BUILD_DIR}/slitherlink"
}

package_opk() {
  ./package-opk.sh "$TARGET"
}

main() {
  # If a TOOLCHAIN environment variable is set, just use that.
  if [[ -z ${TOOLCHAIN:-} ]]; then
    check_buildroot
    set -x
    make_buildroot
  fi
  build
  strip_bin
  package_opk
}

main
