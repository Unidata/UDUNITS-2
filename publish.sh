# Copies a binary distribution to the download area and ensures existence of
# a source distribution in the download area. Does this if and only if the
# upstream, binary-distribution-creating jobs in the delivery pipeline were
# successful.
#
# This script is complicated by the fact that it will be invoked by every
# upstream job that creates a binary distribution.
#
# Usage:
#     $0 pipeId nJobs binDistroFile srcDistroFile repoDir [docDistroFile]
#
# where:
#     pipeId            Unique identifier for the parent delivery pipeline
#                       instance (e.g., top-of-the-pipe job number)
#     nJobs             Number of upstream jobs
#     binDistroFile     Glob pattern of the binary distribution file
#     srcDistroFile     Glob pattern of the source distribution file
#     repoDir           Pathname of the repository directory
#     docDistroFile     Pathname of the documentation distribution file

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
binDistroFile=${3:?Binary distribution file not specified}
srcDistroFile=${4:?Source distribution file not specified}
repoDir=${5:?Repository directory not specified}
docDistroFile=${6}

#
# Convert glob patterns to absolute pathnames.
#
binDistroFile=`ls $binDistroFile`
srcDistroFile=`ls $srcDistroFile`

#
# Remove any leftovers from an earlier delivery pipeline.
#
ls *.success *.failure 2>/dev/null | grep -v ^$pipeId- | xargs rm -rf

#
# Form a unique identifier for this invocation.
#
jobId=$pipeId-`basename $binDistroFile`

#
# Publish the outcome of the upstream job.
#
if test -e $binDistroFile; then
    touch $jobId.success
else
    touch $jobId.failure
fi

#
# Wait until the outcome of all the upstream jobs can be decided.
#
while ! decidable; do
    sleep 3
done

#
# Copy the binary distribution to the repository.
#
trap "ssh webserver rm -f $repoDir/$binDistroFile; `trap -p ERR`" ERR
success && scp $binDistroFile webserver:$repoDir

#
# If the repository doesn't have the source distribution,
#
srcDistroPath=$repoDir/`basename $srcDistroFile`
if ! ssh webserver ls $srcDistroPath >/dev/null 2>&1; then
    #
    # Copy the source distribution to the repository.
    #
    trap "ssh webserver rm -f $srcDistroPath; `trap -p ERR`" ERR
    scp $srcDistroFile webserver:$srcDistroPath
fi

#
# If the documentation distribution was specified,
#
if test "$docDistroFile"; then
    #        
    # Provision the website with the documentation.
    #
    pkgId=`basename $docDistroFile | sed 's/^\([^-]*-[0-9.]*\).*/\1/'`
    pkgWebDir=/web/content/software/udunits/$pkgId
    trap "ssh webserver rm -rf $pkgWebDir; `trap -p ERR`" ERR
    cat $docDistroFile | 
        ssh webserver "cd `dirname $pkgWebDir` && pax -zr -s ';.*/share/;$pkgId/;' '*/share/'"
fi