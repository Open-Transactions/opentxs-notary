#!/usr/bin/env bash
set -e

version="$1"
if [[ -z "$1" ]] ; then
    echo "Missing opentxs version!"
    exit 1
fi

compiler="$2"
if [[ -z "$2" ]] ; then
    compiler="g++"
fi

os="$3"
if [[ -z "$3" ]] ; then
    os="linux"
fi

aws_url="https://s3-eu-west-1.amazonaws.com"
package="opentxs-${version}-${os}-${compiler}.tar.gz"
url=`echo "${aws_url}/monetas-builds/Open-Transactions/opentxs/develop/${package}" | sed 's/+/%2B/g'`

echo "Downloading: ${url}"
wget ${url}

tar vzxf ${package}

rm ${package}
