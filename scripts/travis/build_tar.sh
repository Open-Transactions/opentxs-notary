#!/usr/bin/env bash
set -e

os="$1"
if [[ -z "$1" ]] ; then
    os="linux"
fi

mkdir opentxs-server/

# opentxs-server
cp -rf build/bin/opentxs-server opentxs-server/

version=`cat VERSION`
compiler=${CXX}
package="opentxs-server-${version}-${os}-${compiler}.tar.gz"

echo "Creating package ${package}"
tar -vpczf ${package} opentxs-server/

cp ${package} s3/
