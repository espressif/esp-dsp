#!/bin/bash
#
# Build all examples from the examples directory.
#
# -----------------------------------------------------------------------------
# Safety settings (see https://gist.github.com/ilg-ul/383869cbb01f61a51c4d).

if [[ ! -z ${DEBUG_SHELL} ]]
then
    set -x # Activate the expand mode if DEBUG is anything but empty.
fi

set -o errexit # Exit if command failed.
set -o pipefail # Exit if pipe failed.
set -o nounset # Exit if variable not set.

# Remove the initial space and instead use '\n'.
IFS=$'\n\t'

STARS='***************************************************'

# -----------------------------------------------------------------------------

die() {
    echo "${1:-"Unknown Error"}" 1>&2
    exit 1
}

pushd examples

EXAMPLES=$(find . -maxdepth 1 -mindepth 1 -type d | cut -d '/' -f 2)

for NAME in ${EXAMPLES}
do
    echo "$STARS"
    echo "Building example $NAME with Make"
    pushd $NAME
    # -j option will be set via MAKEFLAGS in .gitlab-ci.yml
    make defconfig && make || die "Make build for ${NAME} has failed"
    rm -rf build
    echo "$STARS"
    echo "Building example $NAME with CMake for Esp32"
    idf.py set-target esp32
    idf.py build || die "CMake build for ${NAME} has failed for Esp32"
    echo "Building example $NAME with CMake for Esp32-s2"
    idf.py clean
    idf.py set-target esp32s2
    idf.py build || die "CMake build for ${NAME} has failed for Esp32-s2"
    popd
done

popd
