// Copyright (c) 2018 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "Client.hpp"

#define OT_METHOD "opentxs::notary::Client::"

namespace opentxs::notary
{
Client::Client(
    const opentxs::api::client::Manager& client,
    const opentxs::api::server::Manager& server,
    const int network)
    : client_(client)
    , server_(server)
    , network_(network)
    , server_nym_callback_(network::zeromq::ListenCallback::Factory(
          std::bind(&Client::server_nym_updated, this, std::placeholders::_1)))
    , server_nym_subscriber_(
          server_.ZeroMQ().SubscribeSocket(server_nym_callback_))
{
    const auto started =
        server_nym_subscriber_->Start(server_.Endpoints().NymDownload());

    OT_ASSERT(started)

    test_nym();
    migrate_contract();
    set_address_type();
    client_.OTX().StartIntroductionServer(server_.NymID());
    client_.OTX().PublishServerContract(
        server_.NymID(), client_.OTX().IntroductionServer(), server_.ID());
}

void Client::import_nym() const
{
    const auto serverNym = server_.Wallet().Nym(server_.NymID());

    OT_ASSERT(serverNym)

    proto::HDPath path{};
    const auto havePath = serverNym->Path(path);

    OT_ASSERT(havePath)

    const auto seedID = Identifier::Factory(path.root());
    OTPassword words{}, passphrase{};
    words.setPassword(server_.Seeds().Words(seedID->str()));
    passphrase.setPassword(server_.Seeds().Passphrase(seedID->str()));
    const auto imported = client_.Seeds().ImportSeed(words, passphrase);

    OT_ASSERT(imported == seedID->str())
    OT_ASSERT(2 == path.child_size())

    // TODO const auto index = path.child(1);

    // TODO OT_ASSERT(0 == index)

    {
#if OT_CRYPTO_SUPPORTED_KEY_HD
        NymParameters nymParameters(proto::CREDTYPE_HD);
        nymParameters.SetSeed(seedID->str());
        nymParameters.SetNym(0);
        nymParameters.SetDefault(false);
#else
        NymParameters nymParameters(proto::CREDTYPE_LEGACY);
#endif
        auto clientNym = client_.Wallet().Nym(nymParameters);

        OT_ASSERT(clientNym)
        OT_ASSERT(clientNym->CompareID(server_.NymID()))
    }
}

void Client::migrate_contract() const
{
    const auto serverContract = server_.Wallet().Server(server_.ID());

    OT_ASSERT(serverContract)

    auto clientContract =
        client_.Wallet().Server(serverContract->PublicContract());

    OT_ASSERT(clientContract)
}

void Client::migrate_nym() const
{
    const auto serverNym = server_.Wallet().Nym(server_.NymID());

    OT_ASSERT(serverNym)

    auto clientNym = client_.Wallet().mutable_Nym(server_.NymID());
    clientNym.SetContactData(serverNym->Claims().Serialize());
}

void Client::server_nym_updated(const network::zeromq::Message& message) const
{
    if (1 > message.Body().size()) {
        LogOutput(OT_METHOD)(__FUNCTION__)(": Missing nym ID.").Flush();

        return;
    }

    const auto& frame = message.Body_at(0);
    const auto id = Identifier::Factory(frame);

    if (server_.NymID() == id) { migrate_nym(); }
}

void Client::set_address_type() const
{
    if (network_ != client_.ZMQ().DefaultAddressType()) {
        bool notUsed{false};
        client_.Config().Set_long(
            String::Factory("Connection"),
            String::Factory("preferred_address_type"),
            network_,
            notUsed);
        client_.Config().Save();
    }
}

void Client::test_nym() const
{
    const auto clientNym = client_.Wallet().Nym(server_.NymID());

    if (false == bool(clientNym)) { import_nym(); }

    migrate_nym();
}
}  // namespace opentxs::notary
