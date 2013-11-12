# Copies a binary distribution to the download area and ensures existence of
# a source distribution in the download area if and only if the upstream,
# binary-distribution-creating jobs were successful.
#
# Usage:
#     deliver.sh n binDistroPath srcDistroPath
#
# where:
#     n                 Number of upstream jobs
#     binDistroPath     Pathname of the binary distribution file
#     srcDistroPath     Pathname of the source distribution file

set -e  # exit on failure

# Indicates if the outcome of the upstream jobs is decidable.
decidable() {
    test `ls *.success *.failure 2>/dev/null | wc -w` -ge $n
}

# Indicates if the upstream jobs were successful.
success() {
    test `ls *.success 2>/dev/null | wc -w` -ge $n
}

n=${1?Number of upstream jobs not specified}
binDistroPath=${2?Binary distribution not specified}
srcDistroPath=${3?Source distribution not specified}

binDistroName=`basename $binDistroPath`
rm -rf $binDistroName.*

if test -e $binDistroPath; then
    touch $binDistroName.success
else
    touch $binDistroName.failure
fi

while ! decidable; do
    sleep 3
done

ftpDir=/web/ftp/pub/udunits

success && scp $binDistroPath webserver:$ftpDir

ssh webserver ls $ftpDir/`basename $srcDistroPath` >/dev/null 2>&1 ||
    scp $srcDistroPath webserver:$ftpDir
