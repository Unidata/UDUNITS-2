# This script does the following:
#     1. Adds a timestamp to CHANGE_LOG;
#     2. Sets the package version in the CMake configuration-file;
#     3. Builds the package;
#     4. Tests the package; and
#     5. Creates a source-distribution.

set -e  # exit if error

# Add a timestamp to CHANGE_LOG.
sed '1s/^\([^         ]*\).*/\1       '"`date --rfc-3339=seconds`/" \
    CHANGE_LOG >CHANGE_LOG.tmp
mv CHANGE_LOG.tmp CHANGE_LOG 

# Set the package version in the CMake configuration-file.
versionId=`head -1 CHANGE_LOG`
majorId=`echo $versionId | cut -d . -f 1`
minorId=`echo $versionId | cut -d . -f 2`
patchId=`echo $versionId | cut -d . -f 3`

test "$majorId"
test "$minorId"
test "$patchId"

cat <<EOF >CMakeLists.txt.tmp
SET(VERSION_MAJOR $majorId)
SET(VERSION_MINOR $minorId)
SET(VERSION_PATCH $patchId)
EOF 

cat CMakeLists.txt >>CMakeLists.txt.tmp
mv CMakeLists.txt.tmp CMakeLists.txt 

# Build and test the package and create a source-distribution.
rm -rf build
mkdir build
cd build
cmake ..
cmake --build . -- all test package_source