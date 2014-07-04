#!/bin/sh
# Get and install Open Transactions Sample Data from https://github.com/Monetas/opentxs-sample-data

git clone https://github.com/Monetas/opentxs-sample-data
rm -rf ~/.ot/*
cp -rf opentxs-sample-data/ot-sample-data/* ~/.ot/

opentxs-sample-data/createmint-sample.sh
