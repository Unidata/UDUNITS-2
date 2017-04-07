# This script does the following:
#     1. Builds the package;
#     2. Tests the package; and
#     3. Creates a source-distribution.
#
# Preconditions:
#     - The current working directory is the top-level source-directory; and
#     - File `release-vars.sh` is in the same directory as this script.

set -e  # exit if error

# Get the static release variables
#
. `dirname $0`/release-vars.sh

#
# Build and test the package using CMake.
#
rm -rf build
mkdir build
pushd build
cmake ..
make all test package_source
popd

# Build and test the package and create a source-distribution using autotools.
autoreconf -i --force
./configure --prefix=/tmp/$PKG_ID &>configure.log
make distcheck
