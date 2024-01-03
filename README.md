# opentxs-notary

[![format](https://github.com/open-transactions/opentxs-notary/workflows/format/badge.svg)](https://github.com/Open-Transactions/opentxs-notary/actions)
[![iwyu](https://github.com/open-transactions/opentxs-notary/workflows/iwyu/badge.svg)](https://github.com/Open-Transactions/opentxs-notary/actions)
[![compile](https://github.com/open-transactions/opentxs-notary/workflows/compile/badge.svg)](https://github.com/Open-Transactions/opentxs-notary/actions)

opentxs-notary is the server component of the OTX protocol described in the [Open-Transactions whitepaper](http://www.opentransactions.org/open-transactions.pdf).

## Build Instructions

The recommended way of deploying opentxs-notary is via [Docker](tools/docker).

opentxs-notary may also be built standalone using the CMake build system. The basic steps are:

    mkdir build
    cd build
    cmake ..
    cmake --build .
    cmake --install .

## Usage

The most current list of supported chains and their associated command line arguments, as well as the full list of libopentxs arguments, can be obtained by passing --help.

### First time setup

The following arguments should be considered when running the notary for the first time. ```--only-init``` will immediately exit after creating the contract.

```
  --notary_name arg                (only when creating a new notary contract)
                                   notary name
  --notary_terms arg               (only when creating a new notary contract)
                                   notary terms and conditions
  --notary_public_eep arg          (only when creating a new notary contract)
                                   public eep address to advertise in contract
  --notary_public_ipv4 arg         (only when creating a new notary contract)
                                   public ipv4 address to advertise in contract
  --notary_public_ipv6 arg         (only when creating a new notary contract)
                                   public ipv6 address to advertise in contract
  --notary_public_onion arg        (only when creating a new notary contract)
                                   public onion address to advertise in
                                   contract
  --notary_command_port arg        (only when creating a new notary contract)
                                   public listening port
```

### Normal operation

opentxs-notary will listen on a single tcp port as specified by the ```--notary_bind_port``` argument. Remote users will always attempt to connect to the notary using port 7085. This port must be accessible via the endpoints specified at contract creation time using the ```--notary_public_*``` arguments

```
  --notary_bind_ip arg             Local IP address for the notary to listen on
  --notary_bind_port arg           Local TCP port for the notary to listen on
```
