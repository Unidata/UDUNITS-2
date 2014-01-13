# This script does the following:
#     1. Builds the package;
#     2. Tests the package; and
#     3. Creates a source-distribution.

set -e  # exit if error

#
# Build and test the package and create a source-distribution.
#
./configure &>configure.log
make distcheck
rm -rf build
mkdir build
cd build
cmake ..
make all test package_source