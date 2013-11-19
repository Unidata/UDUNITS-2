# Performs an acceptance-test of a package on a Linux system. Input
# is the compressed tar file of the source-code. Output is a binary
# distribution file.
#
# Usage:
#     $0 tgz vmName sysName generator ext install
#
# where:
#     tgz       Glob pattern of the compressed tar file of the source
#               distribution
#     vmName    Name of the virtual machine (e.g., "fedora19_64", "precise32")
#     sysName   Name of the system for CPack (e.g., "fedora19-x86_64",
#               "ubuntu12_32")
#     generator Name of the CPack package generator (e.g., "RPM", "DEB")
#     ext       Extension of the package file (e.g., "rpm", "deb")
#     install   Command to install from the package file (e.g., "rpm --install",
#               "dpkg --install")

set -e

tgz=${1:?Pathname of compressed source-distribution not specified}
vmName=${2:?Name of virtual machine not specified}
sysName=${3:?Name of system for CPack not specified}
generator=${4:?Name of the CPack package generator not specified}
ext=${5:?Package extension not specified}
install=${6:?Installation command not specified}

prefix=/usr/local
tgz=`ls $tgz`

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
# Start the virtual machine.
#
type vagrant 
trap "vagrant destroy --force $vmName" 0
vagrant up $vmName

#
# On the virtual machine, build the package from source, test it, install it,
# test the installation, and create a binary distribution.
#
vagrant ssh $vmName -c \
  "cmake -DCMAKE_INSTALL_PREFIX=$prefix -DCPACK_SYSTEM_NAME=$sysName -DCPACK_GENERATOR=$generator /vagrant"
vagrant ssh $vmName -c "cmake --build . -- all test"
vagrant ssh $vmName -c "sudo cmake --build . -- install install_test package"

#
# Copy the binary distribution to the host machine.
#
rm -rf *.$ext
vagrant ssh $vmName -c "cp *.$ext /vagrant"

#
# Restart the virtual machine.
#
vagrant destroy --force $vmName
vagrant up $vmName

#
# Verify that the package installs correctly from the binary distribution.
#
vagrant ssh $vmName -c "sudo $install /vagrant/*.$ext"
vagrant ssh $vmName -c "$prefix/bin/udunits2 -A -H km -W m"