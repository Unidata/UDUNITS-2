# Copies a binary distribution to the download area and ensures existence of
# a source distribution in the download area. Does this if and only if the
# upstream, binary-distribution-creating jobs in the delivery pipeline were
# successful.
#
# This script is complicated by the fact that it must assume that it will be
# invoked multiple times by an instance of the delivery pipeline.
#
# Usage:
#     $0 pipeId nJobs binDistroPath srcDistroPath
#
# where:
#     pipeId            Unique identifier for the parent delivery pipeline
#                       instance
#     nJobs             Number of upstream jobs
#     binDistroPath     Glob pattern of the binary distribution file
#     srcDistroPath     Glob pattern of the source distribution file

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

success && scp $binDistroPath webserver:$ftpDir

#
# Copy the source distribution to the download area if necessary.
#
ssh webserver ls $ftpDir/`basename $srcDistroPath` >/dev/null 2>&1 ||
    scp $srcDistroPath webserver:$ftpDir
