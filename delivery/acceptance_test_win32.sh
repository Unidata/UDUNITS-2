ssh vagrant@192.168.56.101 'rm -rf udunits-2.1.26-Source'
scp udunits-2.1.26-Source.zip vagrant@192.168.56.101: &&
ssh vagrant@192.168.56.101 '"/cygdrive/c/Program Files (x86)/7-Zip/7z"' \
    x -y udunits-2.1.26-Source.zip &&
ssh vagrant@192.168.56.101 'cd udunits-2.1.26-Source && ./acceptance_test.cmd'   