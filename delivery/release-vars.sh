# Defines environment variables relevant to releasing this package.

# Base definitions:
export PKG_NAME=udunits
export PKG_VERSION=2.2.27.17
export SOURCE_DISTRO_EXT=tar.gz
export ABSPATH_FTP_PUB_DIR=/web/ftp/pub
export SOURCE_REPO_HOST=ftp
export YUM_REPO_HOST=spock
export RELPATH_YUM_REPO_ROOT=repos/yum
export WEB_HOST=www
export ABSPATH_WEB_ROOT=/web/content
export BEGIN_VERSION_LINKS="BEGIN VERSION LINKS"
export END_VERSION_LINKS="END VERSION LINKS"

# Derived definitions:
export PKG_ID="$PKG_NAME-$PKG_VERSION"
export PKG_ID_GLOB="$PKG_NAME-[0-9]*.*.*"
export SOURCE_DISTRO_NAME="$PKG_ID.$SOURCE_DISTRO_EXT"
export RELPATH_DISTRO_SOURCE_DIR="$PKG_ID"
export ABSPATH_DEFAULT_INSTALL_PREFIX=/usr/local
export ABSPATH_WEB_SOFTWARE_ROOT="$ABSPATH_WEB_ROOT/software"
export ABSPATH_SOURCE_REPO_DIR="$ABSPATH_FTP_PUB_DIR/$PKG_NAME"
export ABSPATH_SOURCE_DISTRO="$ABSPATH_SOURCE_REPO_DIR/$SOURCE_DISTRO_NAME"
export ABSPATH_PKG_WEB_DIR="$ABSPATH_WEB_SOFTWARE_ROOT/$PKG_NAME"
export ABSPATH_VERSION_WEB_DIR="$ABSPATH_PKG_WEB_DIR/$PKG_ID"
export RELPATH_DOC_DIR=share/doc/"$PKG_NAME"
