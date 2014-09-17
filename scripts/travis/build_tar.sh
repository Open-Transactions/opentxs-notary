#!/usr/bin/env bash
set -e

os="$1"
if [[ -z "$1" ]] ; then
    os="linux"
fi

cp -r opentxs opentxs-notary
cp -rf build/bin/opentxs-notary opentxs-notary/bin/
cp -rf build/bin/createmint opentxs-notary/bin/

version=`cat VERSION`
compiler=${CXX}
package="opentxs-notary-${version}-${os}-${compiler}.tar.gz"

echo "Creating package ${package}"
tar -vpczf ${package} opentxs-notary

mkdir s3
cp ${package} s3/
cp VERSION s3/
