# Performs an acceptance-test of a package on a Linux system. Creates a binary
# distribution file and a documentation distribution file. The current directory
# must contain the source-distribution file and the release-variables file.
#
# Usage: $0 vmName vmCpu generator ext install [upload binRepoHost binRepoRoot
#               binRepoDir]
#
# where:
#     vmName            Name of the Vagrant virtual machine (e.g.,
#                       "centos64_64", "precise32")
#     vmCpu             Type of VM CPU (e.g., "x86_64")
#     generator         Name of the CPack binary package generator (e.g., "RPM",
#                       "DEB")
#     ext               Extension of the binary package file (e.g., "rpm",
#                       "deb")
#     install           Command to install from the package file (e.g., "rpm 
#                       --install", "dpkg --install")
#     upload            Path of the binary-repository upload script 
#     binRepoHost       Name of the binary repository host (e.g., "spock").
#     binRepoRoot       Path of the root of the binary repository on the
#                       binary repository host relative to the home directory
#                       of the user (e.g., "repo").
#     binRepoDir        Path of the binary repository on the binary repository
#                       host relative to the root of the binary repository
#                       excluding the CPU type (e.g., "CentOS/6").

set -e # terminate on error

# Parse the command-line.
#
VM_NAME=${1:?Name of Vagrant virtual machine not specified}
VM_CPU=${2:?Virtual machine CPU not specified}
GENERATOR=${3:?Name of CPack package generator not specified}
EXT=${4:?Package extension not specified}
INSTALL=${5:?Installation command not specified}
upload=${6}
binRepoHost=${7}
binRepoRoot=${8}
binRepoDir=${9}

# Get the static release variables.
#
. ./release-vars.sh

binDistroName="$PKG_ID.$VM_CPU"
binDistroFilename=$binDistroName.$EXT

# Make the directory that contains this script be the current working directory.
#
cd `dirname $0`

# Start the virtual machine. Ensure that each virtual machine is started
# separately because vagrant(1) doesn't support concurrent "vagrant up" 
# invocations.
#
trap "vagrant destroy --force $VM_NAME; `trap -p EXIT`" EXIT
flock "$SOURCE_DISTRO_NAME" -c "vagrant up \"$VM_NAME\""

# On the virtual machine,
#
vagrant ssh $VM_NAME -- -T <<EOF
    set -e # terminate on error

    # Unpack the source distribution.
    #
    pax -zr </vagrant/$SOURCE_DISTRO_NAME

    # Make the source directory of the unpacked distribution the current working
    # directory.
    #
    cd $RELPATH_DISTRO_SOURCE_DIR

    # Build the package from source, test it, install it, test the installation,
    # and create a binary distribution.
    #
    cmake -DCMAKE_INSTALL_PREFIX=$ABSPATH_DEFAULT_INSTALL_PREFIX \
        -DCPACK_PACKAGE_FILE_NAME=$binDistroName -DCPACK_GENERATOR=$GENERATOR
    make all test
    sudo make install install_test package

    #
    # Copy the binary distribution to the host machine.
    #
    cp $binDistroFilename /vagrant
EOF

#
# Restart the virtual machine.
#
vagrant destroy --force $VM_NAME
flock "$SOURCE_DISTRO_NAME" -c "vagrant up \"$VM_NAME\""

# On the virtual machine,
#
vagrant ssh $VM_NAME -- -T <<EOF
    set -e # terminate on error

    # Verify that the package installs correctly from the binary distribution.
    #
    sudo $INSTALL /vagrant/*.$EXT
    $ABSPATH_DEFAULT_INSTALL_PREFIX/bin/udunits2 -A -H km -W m
EOF

# If desired,
#
if test -n "$upload" -a -n "$binRepoHost" -a -n "binRepoRoot" -a -n "binRepoDir"; then
    # Upload the binary distribution to the binary repository.
    #
    scp $upload $binRepoHost:$binRepoRoot/upload &&        
        ssh -T $binRepoHost bash -x $binRepoRoot/upload \
                $binRepoDir/$VM_CPU/$binDistroFilename <$binDistroFilename
fi