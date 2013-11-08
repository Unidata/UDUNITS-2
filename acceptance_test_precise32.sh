# Performs an acceptance-test of a package on a Debian-based VM. Input is the
# compressed tar file of the source-code. Output is a DEB binary distribution.

set -e

ip=${1?IP of VM not specified}
tgz=${2?Pathname of compressed tar file not specified}

pkgId=`basename $tgz .tar.gz`
vmName=precise32        #  Ubuntu's 32-bit "Precise Pangolin"
prefix=/usr             # /usr/local doesn't appear to be viable

echo ip=$ip
echo tgz=$tgz
echo pkgId=$pkgId

pax -zr <$tgz
cd $pkgId

trap "vagrant destroy --force $vmName" 0

vagrant up $vmName

# Build the package from source, test it, install it, test the installation,
# and create a binary distribution.
vagrant ssh $vmName -c "cmake -DCMAKE_INSTALL_PREFIX=$prefix -DCPACK_GENERATOR=DEB /vagrant"
vagrant ssh $vmName -c "cmake --build . -- all test"
vagrant ssh $vmName -c "sudo cmake --build . -- install install_test package"
vagrant ssh $vmName -c 'cp *.deb /vagrant'

vagrant destroy --force $vmName
vagrant up $vmName

# Verify that the package installs correctly from the binary distribution.
vagrant ssh $vmName -c "sudo dpkg --install --instdir=$prefix /vagrant/*.deb"
vagrant ssh $vmName -c "$prefix/bin/udunits2 -A -H km -W m"