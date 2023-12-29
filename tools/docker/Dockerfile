ARG DOWNLOAD_ALPINE_VERSION="3.18.5"
ARG OPENTXS_NOTARY_REPO="https://github.com/Open-Transactions/opentxs-notary"
ARG OPENTXS_VERSION="latest"
ARG OPENTXS_BUILD_URL="opentransactions/alpine-sdk:$OPENTXS_VERSION"
ARG OPENTXS_RUN_URL="opentransactions/alpine-runtime:$OPENTXS_VERSION"

FROM alpine:${DOWNLOAD_ALPINE_VERSION} AS download
RUN mkdir -p /usr/src
RUN --mount=type=tmpfs,target=/var/cache/apk apk add \
    bzip2 \
    git \
    lzo \
    p7zip \
    rsync \
    wget \
    xz

FROM download AS notary-download
ARG OPENTXS_NOTARY_REPO
ARG OPENTXS_NOTARY_COMMIT
RUN mkdir -p /usr/src && git clone --recursive "${OPENTXS_NOTARY_REPO}" /usr/src/opentxs-notary && cd /usr/src/opentxs-notary && git reset --hard "${OPENTXS_NOTARY_COMMIT}"

FROM $OPENTXS_BUILD_URL AS compile
COPY --from=notary-download /usr/src/opentxs-notary /usr/src/opentxs-notary
RUN --mount=type=tmpfs,target=/tmp/opentxs-notary /opt/cmake/bin/cmake \
        -S /usr/src/opentxs-notary \
        -B /tmp/opentxs-notary \
        -GNinja \
        -DCMAKE_C_FLAGS="-O2" \
        -DCMAKE_CXX_FLAGS="-O2" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_UNITY_BUILD=ON \
        -DCMAKE_UNITY_BUILD_BATCH_SIZE=0 \
    && /opt/cmake/bin/cmake --build /tmp/opentxs-notary \
    && /opt/cmake/bin/cmake --install /tmp/opentxs-notary

FROM $OPENTXS_RUN_URL AS run
COPY --from=compile /usr/local/bin/opentxs-notary /usr/bin/opentxs-notary
ENTRYPOINT [ "/usr/bin/opentxs-notary", "--ot_home=/var/lib/opentxs-notary" ]
CMD []
