# Performs an acceptance-test of a package on a Debian-based VM. Input is the
# compressed tar file of the source-code. Output is a DEB binary distribution.

set -e

ip=${1?IP of VM not specified}
tgz=${2?Pathname of compressed tar file not specified}

vmBase=precise32        # Ubuntu's 32-bit "Precise Pangolin"
vmDevel=${vmBase}_devel
vmRun=${vmBase}_run
prefix=/usr             # Debian's "/usr/local" doesn't appear to be viable

echo ip=$ip
echo tgz=$tgz

pax -zr <$tgz
cd `basename $tgz .tar.gz`

trap "vagrant destroy --force $vmDevel" 0
vagrant up $vmDevel

# Build the package from source, test it, install it, test the installation,
# and create a binary distribution.
vagrant ssh $vmDevel -c "cmake -DCMAKE_INSTALL_PREFIX=$prefix -DCPACK_SYSTEM_PROCESSOR=i386 -DCPACK_GENERATOR=DEB /vagrant"
vagrant ssh $vmDevel -c "cmake --build . -- all test"
vagrant ssh $vmDevel -c "sudo cmake --build . -- install install_test package"
vagrant ssh $vmDevel -c 'cp *.deb /vagrant'

vagrant destroy --force $vmDevel

trap "vagrant destroy --force $vmRun" 0
vagrant up $vmRun

# Verify that the package installs correctly from the binary distribution.
# Bug in dpkg(1) causes following to fail if option "--instdir=$prefix" used.
vagrant ssh $vmRun -c "sudo dpkg --install /vagrant/*.deb"
vagrant ssh $vmRun -c "$prefix/bin/udunits2 -A -H km -W m"