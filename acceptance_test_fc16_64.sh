# Performs an acceptance-test of a package on a 64-bit, Fedora 16 system. Input
# is the compressed tar file of the source-code. Output is an RPM binary
# distribution.
#
# Usage:
#     $0 tgz
#
# where:
#     tgz       Glob pattern of the compressed tar file of the source
#               distribution

set -e

tgz=${1:?Pathname of compressed tar file not specified}

#
# Vet the host.
#
uname -r | fgrep fc16.x86_64

prefix=/usr/local
DESTDIR=`pwd`
tgz=`ls $tgz`
echo tgz=$tgz

#
# Remove any leftover artifacts from an earlier job.
#
rm -rf *

#
# Unpack the source distribution.
#
pax -zr <$tgz

#
# Make the source directory the current working directory.
#
cd `basename $tgz .tar.gz`

#
# Build the package from source, test it, install it, test the installation,
# and create a binary distribution.
#
cmake -DCMAKE_INSTALL_PREFIX=$prefix -DCPACK_SYSTEM_NAME=fc16-x86_64 \
    -DCPACK_GENERATOR=RPM .
make DESTDIR=$DESTDIR all test install install_test package
rm -rf $DESTDIR$prefix

#
# Verify that the package installs correctly from the binary distribution.
#
rpm --install --prefix $DESTDIR$prefix *.rpm
$DESTDIR$prefix/bin/udunits2 -A -H km -W m $DESTDIR$prefix/share/udunits