# Performs an acceptance-test of a package on a Linux system. Creates a binary
# distribution file and a documentation distribution file. The directory that
# contains this script must also contain the files
#     - release-vars.sh
#     - Vagrantfile
#     - repo_add
#
# Usage: $0 tarball vmName vmCpu generator ext install [binRepoDir]
#
# where:
#     tarball           Pathname of the source distribution file.
#     vmName            Name of the Vagrant virtual machine (e.g.,
#                       "centos64_64", "precise32")
#     vmCpu             Type of VM CPU (e.g., "x86_64")
#     generator         Name of the CPack binary package generator (e.g., "RPM",
#                       "DEB")
#     ext               Extension of the binary-distribution file (e.g., "rpm",
#                       "deb")
#     install           Command to install from the binary-distribution file
#                       (e.g., "rpm --install", "dpkg --install")
#     binRepoDir        Path of the platform-specific binary-repository
#                       directory (excluding the CPU type) relative to the root
#                       directory of the package-manager-specific
#                       binary-repository (e.g., "CentOS/6"). The
#                       binary-distribution will be placed in directory
#                       "$binRepoDir/$vmCpu".

set -e # terminate on error

# Parse the command-line.
#
tarball=${1:?Pathname of tarball not specified}
vmName=${2:?Name of Vagrant virtual-machine not specified}
vmCpu=${3:?Virtual-machine CPU not specified}
generator=${4:?Name of CPack package-generator not specified}
ext=${5:?Package extension not specified}
install=${6:?Installation command not specified}
binRepoDir=${7:?Platform-specific binary-repository directory not specified}

# Set the build directory to the directory that contains this script.
builddir=`dirname $0`

# Move the tarball to the build directory so that it appears in the "/vagrant"
# directory.
#
mv $tarball $builddir

# Make the build directory the current working directory.
#
cd $builddir

# Set the release-variables.
. ./release-vars.sh

binDistroName="$PKG_ID.$vmCpu"
binDistroFilename=$binDistroName.$ext

# Start the virtual machine. Ensure that each virtual machine is started
# separately because vagrant(1) doesn't support concurrent "vagrant up" 
# invocations in the same directory.
#
trap "vagrant destroy --force $vmName; `trap -p EXIT`" EXIT
flock "$SOURCE_DISTRO_NAME" -c "vagrant up \"$vmName\""

# On the virtual machine,
#
vagrant ssh $vmName -- -T <<EOF
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
        -DCPACK_PACKAGE_FILE_NAME=$binDistroName -DCPACK_GENERATOR=$generator
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
vagrant destroy --force $vmName
flock "$SOURCE_DISTRO_NAME" -c "vagrant up \"$vmName\""

# On the virtual machine,
#
vagrant ssh $vmName -- -T <<EOF
    set -e # terminate on error

    # Verify that the package installs correctly from the binary distribution.
    #
    sudo $install /vagrant/$binDistroFilename
    $ABSPATH_DEFAULT_INSTALL_PREFIX/bin/udunits2 -A -H km -W m

    # Add the binary-distribution to the yum(1) binary-repository.
    #
    bash -x /vagrant/repo_add /repo $binRepoDir/$vmCpu \
            /vagrant/$binDistroFilename
EOF

# Move the binary distribution to the workspace.
#
mv $binDistroFilename ../..