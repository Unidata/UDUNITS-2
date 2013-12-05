# This script does the following:
#     1. Ensures correct version information in the CMake configuration-file;
#     2. Commits to the local repository if appropriate;
#     3. Tags the HEAD revision with the current version; and
#     4. Pushes to the remote repository

set -e  # exit if error

#
# Set the package version in the CMake configuration-file.
#
versionId=`awk '{print $1;exit}' CHANGE_LOG`
majorId=`echo $versionId | cut -d . -f 1`
minorId=`echo $versionId | cut -d . -f 2`
patchId=`echo $versionId | cut -d . -f 3`

test "$majorId"
test "$minorId"
test "$patchId"

sed "
/^[ \t]*[Ss][Ee][Tt][ \t]*(VERSION_MAJOR[ \t]/cSET(VERSION_MAJOR $majorId)
/^[ \t]*[Ss][Ee][Tt][ \t]*(VERSION_MINOR[ \t]/cSET(VERSION_MINOR $minorId)
/^[ \t]*[Ss][Ee][Tt][ \t]*(VERSION_PATCH[ \t]/cSET(VERSION_PATCH $patchId)" \
    CMakeLists.txt >CMakeLists.txt.tmp
mv CMakeLists.txt.tmp CMakeLists.txt 

#
# Commit, tag, and push the package.
#
git commit -a || true
git tag -f v$versionId
git push
