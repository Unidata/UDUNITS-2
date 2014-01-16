# Copies a binary distribution to the download area and ensures existence of
# a source distribution in the download area. Does this if and only if the
# upstream, binary-distribution-creating jobs in the delivery pipeline were
# successful.
#
# This script is complicated by the fact that it will be invoked by every
# upstream job that creates a binary distribution.
#
# Usage:
#     $0 pipeId nJobs binDistroFile srcDistroFile binRepoRelDir docDistroFile pkgName indexHtml
#
# where:
#     pipeId                Unique identifier for the parent delivery pipeline
#                           instance (e.g., top-of-the-pipe job number)
#     nJobs                 Number of upstream jobs
#     binDistroFile         Pathname of the binary distribution file
#     srcDistroFile         Pathname of the source distribution file
#     binRepoRelDir         Pathname of the binary repository directory relative to
#                           the root of the binary repository
#     docDistroFile         Pathname of the documentation distribution file
#     pkgName               Name of the package (e.g., "udunits")
#     indexHtml             Name of the top-level HTML documentation file (e.g.,
#                           "udunits2.html")

pipeId=${1:?Group ID not specified}
nJobs=${2:?Number of upstream jobs not specified}
binDistroFile=${3:?Binary distribution file not specified}
srcDistroFile=${4:?Source distribution file not specified}
binRepoRelDir=${5:?Relative pathname of binary repository directory not specified}
docDistroFile=${6:?Documentation distribution file not specified}
pkgName=${7:?Package name not specified}
indexHtml=${8:?Top-level HTML documentation-file not specified}

binRepoHost=spock                # Name of computer hosting binary repository
binRepoRoot=repo                 # Pathname of the root directory of the binary
                                 # repository. A relative pathname is resolved
                                 # against the home directory of $USER on
                                 # $binRepoHost
srcRepoHost=webserver            # Name of computer hosting source repository
srcRepoDir=/web/ftp/pub/$pkgName # Pathname of source repository
webHost=webserver                # Name of computer hosting package website

set -e  # exit on failure

# Indicates if the outcome of the upstream jobs is decidable.
#
decidable() {
    test `ls $jobId.success $jobId.failure 2>/dev/null | wc -w` -ge $nJobs
}

# Indicates if the upstream jobs were successful.
#
success() {
    test `ls $jobId.success 2>/dev/null | wc -w` -ge $nJobs
}

# Ensure valid pathnames.
#
binDistroFile=`ls $binDistroFile`
srcDistroFile=`ls $srcDistroFile`


# Form a unique identifier for this invocation.
#
binDistroFileName=`basename $binDistroFile`
jobId=$pipeId-$binDistroFileName

# Remove any leftovers from an earlier delivery pipeline.
#
ls *.success *.failure 2>/dev/null | grep -v ^$jobId | xargs rm -rf

# Make known to all invocations of this script in the delivery pipeline the
# outcome of the upstream job associated with this invocation.
#
if test -e $binDistroFile; then
    touch $jobId.success
else
    touch $jobId.failure
fi

# Wait until the outcome of all the upstream jobs can be decided.
#
while ! decidable; do
    sleep 3
done

# Upload the binary distribution to the binary repository.
#
success && 
    ssh -T $binRepoHost bash -x $binRepoRoot/upload \
            $binRepoRelDir/$binDistroFileName <$binDistroFile

# Set the absolute path to the public source distribution file.
#
srcDistroPath=/web/ftp/pub/$pkgName/`basename $srcDistroFile`

# If the source repository doesn't have the source distribution,
#
if ! ssh $srcRepoHost test -e $srcDistroPath; then
    #
    # Copy the source distribution to the source repository.
    #
    trap "ssh $srcRepoHost rm -f $srcDistroPath; `trap -p ERR`" ERR
    scp $srcDistroFile $srcRepoHost:$srcDistroPath
fi

# Upload the documentation to the package's website.
#
pkgId=`basename $docDistroFile | sed 's/^\([^-]*-[0-9.]*\).*/\1/'`
version=`echo $pkgId | sed 's/^[^-]*-//'`
pkgWebDir=/web/content/software/$pkgName
ssh -T $webHost bash -x $pkgWebDir/upload $version $indexHtml <$docDistroFile