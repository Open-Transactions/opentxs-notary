#!/usr/bin/env bash
set -e

compiler="$1"
if [[ -z "$1" ]] ; then
    compiler="g++"
fi

os="$2"
if [[ -z "$2" ]] ; then
    os="linux"
fi

aws_url="https://s3-eu-west-1.amazonaws.com"

version="0.93.0"

package="opentxs-${version}-${os}-${compiler}.tar.gz"
url=`echo "${aws_url}/monetas-builds/monetas/opentxs/develop/${package}" | sed 's/+/%2B/g'`

echo "Downloading: ${url}"
wget ${url}

tar vzxf ${package}
