# Performs an acceptance-test of a package on a guest VM. Input is the
# compressed tar file of the source-code. Output is a binary distribution.

ip=${1?Guest IP not specified}
tgz=${2?Pathname of compressed tar file not specified}
pkgid=`basename $tgz .tar.gz`

echo ip=$ip
echo tgz=$tgz
echo pkgid=$pkgid

pax -zr <$tgz
cd $pkgid

trap 'vagrant destroy' 0
vagrant up precise32

vagrant ssh precise32 -c "cmake /vagrant"
vagrant ssh precise32 -c "cmake --build . -- all test"
vagrant ssh precise32 -c "sudo cmake --build . -- install"
vagrant ssh precise32 -c "cmake --build . -- install_test package"
vagrant ssh precise32 -c 'cp *.deb /vagrant'