#!/usr/bin/env bash
set -e

os="$1"
if [[ -z "$1" ]] ; then
    os="linux"
fi

mkdir s3

# opentxs
cp -rf build/opentxs-server s3

# deps
version=`cat VERSION`
compiler=${CXX}
package="opentxs-server-${version}-${os}-${compiler}.tar.gz"

echo "Creating package ${package}"
tar -vpczf ${package} opentxs-server/

cp ${package} s3/
