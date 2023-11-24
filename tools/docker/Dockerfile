ARG OPENTXS_NOTARY_REPO="https://github.com/Open-Transactions/opentxs-notary"
ARG OPENTXS_VERSION="latest"
ARG OPENTXS_BUILD_URL="opentransactions/fedora-devel:$OPENTXS_VERSION"
ARG OPENTXS_RUN_URL="opentransactions/fedora:$OPENTXS_VERSION"

FROM fedora:39 AS base

FROM base AS git
RUN --mount=type=tmpfs,target=/var/cache/dnf dnf install -y git

FROM git AS download
ARG OPENTXS_NOTARY_REPO
ARG OPENTXS_NOTARY_COMMIT
RUN mkdir -p /usr/src && git clone --recursive "${OPENTXS_NOTARY_REPO}" /usr/src/opentxs-notary && cd /usr/src/opentxs-notary && git reset --hard "${OPENTXS_NOTARY_COMMIT}"

ARG OPENTXS_BUILD_URL
FROM $OPENTXS_BUILD_URL AS compile
COPY --from=download /usr/src/opentxs-notary /usr/src/opentxs-notary
RUN mkdir -p /tmp/opentxs-notary && cd /tmp/opentxs-notary \
    && cmake \
        -GNinja \
        -DCMAKE_C_FLAGS="-O2" \
        -DCMAKE_CXX_FLAGS="-O2" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_UNITY_BUILD=ON \
        -DCMAKE_UNITY_BUILD_BATCH_SIZE=0 \
        /usr/src/opentxs-notary \
    && cmake --build . \
    && cmake --install . \
    && rm -rf /tmp/opentxs-notary*

ARG OPENTXS_RUN_URL
FROM $OPENTXS_RUN_URL AS run
COPY --from=compile /usr/local/bin/opentxs-notary /usr/bin/opentxs-notary
ENTRYPOINT [ "/usr/bin/opentxs-notary", "--ot_home=/var/lib/opentxs-notary" ]
CMD []
