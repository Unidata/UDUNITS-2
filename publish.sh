# Copies a binary distribution to the download area and ensures existence of
# a source distribution in the download area. Does this if and only if the
# upstream, binary-distribution-creating jobs in the delivery pipeline were
# successful.
#
# This script is complicated by the fact that it will be invoked by every
# upstream job that creates a binary distribution.
#
# Usage:
#     $0 pipeId nJobs binDistroPath srcDistroPath [docDistroPath]
#
# where:
#     pipeId            Unique identifier for the parent delivery pipeline
#                       instance (e.g., top-of-the-pipe job number)
#     nJobs             Number of upstream jobs
#     binDistroPath     Glob pattern of the binary distribution file
#     srcDistroPath     Glob pattern of the source distribution file
#     docDistroPath     Pathname of the documentation distribution file

set -e  # exit on failure

#
# Indicates if the outcome of the upstream jobs is decidable.
#
decidable() {
    test `ls *.success *.failure 2>/dev/null | wc -w` -ge $nJobs
}

#
# Indicates if the upstream jobs were successful.
#
success() {
    test `ls *.success 2>/dev/null | wc -w` -ge $nJobs
}

pipeId=${1:?Group ID not specified}
nJobs=${2:?Number of upstream jobs not specified}
binDistroPath=${3:?Binary distribution not specified}
srcDistroPath=${4:?Source distribution not specified}
docDistroPath=${5}

#
# Convert glob patterns to absolute pathnames.
#
binDistroPath=`ls $binDistroPath`
srcDistroPath=`ls $srcDistroPath`

#
# Remove any leftovers from an earlier delivery pipeline.
#
ls *.success *.failure 2>/dev/null | grep -v ^$pipeId- | xargs rm -rf

#
# Form a unique identifier for this invocation.
#
jobId=$pipeId-`basename $binDistroPath`

#
# Publish the outcome of the upstream job.
#
if test -e $binDistroPath; then
    touch $jobId.success
else
    touch $jobId.failure
fi

#
# Wait until the outcome of the upstream jobs can be decided.
#
while ! decidable; do
    sleep 3
done

#
# Copy the binary distribution to the download area.
#
ftpDir=/web/ftp/pub/udunits
trap "`trap -p ERR`; ssh webserver rm -f $ftpDir/$binDistroPath" ERR
success && scp $binDistroPath webserver:$ftpDir

#
# Copy the source distribution to the download area if necessary.
#
srcFtpPath=$ftpDir/`basename $srcDistroPath`
if ! ssh webserver ls $srcFtpPath >/dev/null 2>&1; then
    trap "`trap -p ERR`; ssh webserver rm -f $srcFtpPath" ERR
    scp $srcDistroPath webserver:$srcFtpPath
fi

#
# Copy the documentation to the on-line webpage if appropriate.
#
if test "$docDistroPath"; then
    pkgId=`echo $docDistroPath | sed 's/^\([^-]*-[0-9.]*\).*/\1/'`
    pkgWebDir=/web/content/software/udunits/$pkgId
    trap "`trap -p ERR`; ssh webserver rm -rf $pkgWebDir" ERR
    cat $docDistroPath | 
        ssh webserver "cd `dirname $pkgWebDir` && pax -zr -s ';.*/share/;$pkgId/;' '*/share/'"
fi