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

tgz=${1:?Pathname of compressed source-distribution not specified}

vmName=fedora19_64        # 64-bit Fedora 19
prefix=/usr/local
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
# Start the virtual machine.
#
trap "vagrant destroy --force $vmName" 0
vagrant up $vmName

#
# On the virtual machine, build the package from source, test it, install it,
# test the installation, and create a binary distribution.
#
vagrant ssh $vmName -c \
  "cmake -DCMAKE_INSTALL_PREFIX=$prefix -DCPACK_SYSTEM_NAME=fedora19-x86_64 -DCPACK_GENERATOR=RPM /vagrant"
vagrant ssh $vmName -c "cmake --build . -- all test"
vagrant ssh $vmName -c "sudo cmake --build . -- install install_test package"
rm -rf *.rpm

#
# Copy the binary distribution to the host machine.
#
vagrant ssh $vmName -c 'cp *.rpm /vagrant'

#
# Restart the virtual machine.
#
vagrant destroy --force $vmName
vagrant up $vmName

#
# Verify that the package installs correctly from the binary distribution.
#
vagrant ssh $vmName -c "sudo yum --install /vagrant/*.rpm"
vagrant ssh $vmName -c "$prefix/bin/udunits2 -A -H km -W m"