# Performs an acceptance-test of a package on a guest VM. Input is the
# compressed tar file of the source-code. Output is a binary distribution.

ip=${1?Guest IP not specified}
tgzdir=${2?Parent directory of compressed tar file not specified}
pkgpat=${3?Package pattern not specified}

echo ip=$ip
echo tgzdir=$tgzdir
echo pkgpat=$pkgpat

tgzpat=$pkgpat.tar.gz
tgz=`ls $tgzdir/$tgzpat`

echo tgzpat=$tgzpat
echo tgz=$tgz

pax -zr <$tgz
cd $pkgpat

vagrant up precise32
trap 'vagrant destroy' 0

scp $tgz vagrant@${ip}:

vagrant ssh precise32 -c 'pax -zr <$tgzpat'
vagrant ssh precise32 -c 'mkdir build'
vagrant ssh precise32 -c "cd build && cmake ../$pkgpat/"
vagrant ssh precise32 -c 'cd build && cmake --build . -- all test install install_test package'

scp vagrant@${ip}:build/*.deb .