# Performs an acceptance-test of a package on a Linux system. Creates a binary
# distribution file and a documentation distribution file.
#
# The following environment variables must be set:
#     SOURCE_DISTRO     Glob pattern of the compressed tar file of the source
#                       distribution
#     VM_NAME           Name of the Vagrant virtual machine (e.g.,
#                       "centos64_64", "precise32")
#     PKG_FILE_NAME     Value for the variable CPACK_PACKAGE_FILE_NAME (e.g.,
#                       "udunits-2.2.0.x86_64")
#     GENERATOR         Name of the CPack binary package generator (e.g., "RPM",
#                       "DEB")
#     EXT               Extension of the binary package file (e.g., "rpm",
#                       "deb")
#     INSTALL           Command to install from the package file (e.g., "rpm 
#                       --install", "dpkg --install")

set -e

: SOURCE_DISTRO=${SOURCE_DISTRO:?Path of source-distribution not specified}
: VM_NAME=${VM_NAME:?Name of virtual machine not specified}
: PKG_FILE_NAME=${PKG_FILE_NAME:?Package filename not specified}
: GENERATOR=${GENERATOR:?Name of CPack package generator not specified}
: EXT=${EXT:?Package extension not specified}
: INSTALL=${INSTALL:?Installation command not specified}

prefix=/usr/local
SOURCE_DISTRO=`ls $SOURCE_DISTRO`

#
# Remove any leftover artifacts from an earlier job.
#
rm -rf *

#
# Unpack the source distribution.
#
pax -zr <$SOURCE_DISTRO

#
# Make the source directory the current working directory.
#
cd `basename $SOURCE_DISTRO .tar.gz`

#
# Start the virtual machine. Ensure that each virtual machine is started
# separately because vagrant(1) doesn't support concurrent "vagrant up" 
# invocations.
#
type vagrant 
trap "vagrant destroy --force $VM_NAME; `trap -p EXIT`" EXIT
flock "$SOURCE_DISTRO" -c "vagrant up \"$VM_NAME\""

#
# On the virtual machine, build the package from source, test it, install it,
# test the installation, and create a binary distribution.
#
vagrant ssh $VM_NAME -c "cmake --version"
vagrant ssh $VM_NAME -c \
    "cmake -DCMAKE_INSTALL_PREFIX=$prefix -DCPACK_PACKAGE_FILE_NAME=$PKG_FILE_NAME -DCPACK_GENERATOR=$GENERATOR /vagrant"
vagrant ssh $VM_NAME -c "make all test"
vagrant ssh $VM_NAME -c "sudo make install install_test package"

#
# Copy the binary distribution to the host machine.
#
rm -rf *.$EXT
vagrant ssh $VM_NAME -c "cp *.$EXT /vagrant"

#
# Restart the virtual machine.
#
vagrant destroy --force $VM_NAME
vagrant up $VM_NAME

#
# Verify that the package installs correctly from the binary distribution.
#
vagrant ssh $VM_NAME -c "sudo $INSTALL /vagrant/*.$EXT"
vagrant ssh $VM_NAME -c "$prefix/bin/udunits2 -A -H km -W m"

#
# Create a distribution of the documentation in case it's needed by a
# subsequent job. NB: The first component of all pathnames in the distribution
# is "share/".
#
pkgId=`basename $SOURCE_DISTRO .tar.gz | sed 's/^\([^-]*-[0-9.]*\).*/\1/'`
vagrant ssh $VM_NAME -c \
    "pax -zw -s ';$prefix/;;' $prefix/share/doc/udunits $prefix/share/udunits >$pkgId-doc.tar.gz"
vagrant ssh $VM_NAME -c "cp $pkgId-doc.tar.gz /vagrant"