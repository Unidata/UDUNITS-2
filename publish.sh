# Copies a binary distribution to the download area and ensures existence of
# a source distribution in the download area. Does this if and only if the
# upstream, binary-distribution-creating jobs in the delivery pipeline were
# successful.
#
# This script is complicated by the fact that it will be invoked by every
# upstream job that creates a binary distribution.
#
# Usage:
#     $0 pipeId nJobs binDistroFile srcDistroFile binRepoRelDir docDistroFile
#
# where:
#     pipeId            Unique identifier for the parent delivery pipeline
#                       instance (e.g., top-of-the-pipe job number)
#     nJobs             Number of upstream jobs
#     binDistroFile     Pathname of the binary distribution file
#     srcDistroFile     Pathname of the source distribution file
#     binRepoRelDir     Pathname of the binary repository directory relative to
#                       the root of the binary repository
#     docDistroFile     Pathname of the documentation distribution file

pkgName=udunits                  # Name of package
binRepoHost=spock                # Name of computer hosting binary repository
binRepoRoot=repo                 # Pathname of the root directory of the binary
                                 # repository. A relative pathname is resolved
                                 # against the home directory of $USER on
                                 # $binRepoHost
srcRepoHost=webserver            # Name of computer hosting source repository
srcRepoDir=/web/ftp/pub/$pkgName # Pathname of source repository
webHost=webserver                # Name of computer hosting package website
lockfile=file.lock               # lockfile(1) target

set -e  # exit on failure

#
# Indicates if the outcome of the upstream jobs is decidable.
#
decidable() {
    test `ls $jobId.success $jobId.failure 2>/dev/null | wc -w` -ge $nJobs
}

#
# Indicates if the upstream jobs were successful.
#
success() {
    test `ls $jobId.success 2>/dev/null | wc -w` -ge $nJobs
}

pipeId=${1:?Group ID not specified}
nJobs=${2:?Number of upstream jobs not specified}
binDistroFile=${3:?Binary distribution file not specified}
srcDistroFile=${4:?Source distribution file not specified}
binRepoRelDir=${5:?Relative pathname of binary repository directory not specified}
docDistroFile=${6:?Documentation distribution file not specified}

#
# Ensure valid pathnames.
#
binDistroFile=`ls $binDistroFile`
srcDistroFile=`ls $srcDistroFile`

#
# Form a unique identifier for this invocation.
#
binDistroFileName=`basename $binDistroFile`
jobId=$pipeId-$binDistroFileName

#
# Remove any leftovers from an earlier delivery pipeline.
#
ls *.success *.failure 2>/dev/null | grep -v ^$jobId | xargs rm -rf

#
# Make known to all instances of this script the outcome of the upstream job
# associated with this instance.
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
# Copy the binary distribution to the binary repository.
#
binRepoDir=$binRepoRoot/$binRepoRelDir
trap "ssh $binRepoHost rm -f $binRepoDir/$binDistroFileName; `trap -p ERR`" ERR
success && scp $binDistroFile $binRepoHost:$binRepoDir

#
# Rebuild the binary repository.
#
ssh $binRepoHost $binRepoRoot/rebuild $binRepoRelDir

srcDistroPath=/web/ftp/pub/$pkgName/`basename $srcDistroFile`
#
# If the source repository doesn't have the source distribution,
#
if ! ssh $srcRepoHost test -e $srcDistroPath; then
    #
    # Copy the source distribution to the source repository.
    #
    trap "ssh $srcRepoHost rm -f $srcDistroPath; `trap -p ERR`" ERR
    scp $srcDistroFile $srcRepoHost:$srcDistroPath
fi

pkgId=`basename $docDistroFile | sed 's/^\([^-]*-[0-9.]*\).*/\1/'`
pkgWebDir=/web/content/software/$pkgName/$pkgId
#
# If the package's website doesn't have this version's documentation,
#
if ! ssh $webHost test -e $pkgWebDir; then
    #        
    # Provision the package's website with the documentation distribution. NB:
    # Assumes that the first component of all pathnames in the distribution is
    # "share/".
    #
    trap "ssh $webHost rm -rf $pkgWebDir; `trap -p ERR`" ERR
    gunzip -c $docDistroFile | 
        ssh $webHost "cd `dirname $pkgWebDir` && pax -r -s ';share/;$pkgId/;'"
    ssh $webHost "cd $pkgWebDir && ln -s doc/udunits/udunits2.html index.html"
fi