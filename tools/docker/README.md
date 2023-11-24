# opentxs-notary docker deployment image

## Usage

### Building the image

```
docker image build --build-arg "OPENTXS_NOTARY_COMMIT=<tag or commit hash>" .
```

This image is available on Docker Hub as [opentransactions/notary
](https://hub.docker.com/r/opentransactions/notary)

To use a specific version of libopentxs add ```--build-arg "OPENTXS_VERSION=<desired tag>"``` where the desired tag is one of the versions listed for [opentransactions/fedora](https://hub.docker.com/r/opentransactions/fedora/tags). The default value is "latest".

### Running the image

Persistent storage must be mounted at /var/lib/notary inside the image and the ```--user``` argument to ```docker run``` should be set to match the ownership of this directory.

#### Example

```
docker run \
    --read-only \
    --mount type=bind,src=/var/lib/notary,dst=/var/lib/notary \
    --user 1000:1000 \
    -p 7085:7085/tcp \
    opentransactions/notary:latest \
    --notary_bind_ip=127.0.0.1 \
    --notary_bind_port=7085
```
