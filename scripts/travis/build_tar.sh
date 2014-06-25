#!/usr/bin/env bash
set -e

os="$1"
if [[ -z "$1" ]] ; then
    os="linux"
fi

cp -r opentxs opentxs-server
cp -rf build/bin/opentxs-server opentxs-server/bin/
cp -rf build/bin/createmint opentxs-server/bin/

version=`cat VERSION`
compiler=${CXX}
package="opentxs-server-${version}-${os}-${compiler}.tar.gz"

echo "Creating package ${package}"
tar -vpczf ${package} opentxs-server

mkdir s3
cp ${package} s3/
cp VERSION s3/
