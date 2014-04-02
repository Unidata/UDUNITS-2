# This script does the following:
#     1. Builds the package;
#     2. Tests the package; and
#     3. Creates a source-distribution.

set -e  # exit if error

# Get the static release variables
#
. ./release-vars.sh

#
# Build and test the package using CMake.
#
rm -rf build
mkdir build
cd build
cmake ..
make all test package_source

# Build and test the package and create a source-distribution using autotools.
autoreconf -i --force
./configure --prefix=/tmp/$PKG_ID &>configure.log
make distcheck
