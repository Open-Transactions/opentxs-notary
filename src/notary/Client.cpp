// Copyright (c) 2011-2021 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "Client.hpp"

#include <cassert>
#include <opentxs/client/NymData.hpp>

#define OT_METHOD "opentxs::notary::Client::"

namespace opentxs::notary
{
Client::Client(
    const opentxs::api::session::Client& client,
    const opentxs::api::session::Notary& server,
    const int network)
    : client_(client)
    , server_(server)
    , network_(network)
    , client_reason_(client_.Factory().PasswordPrompt("Notary operation"))
    , server_reason_(client_.Factory().PasswordPrompt("Notary operation"))
    , server_nym_callback_(network::zeromq::ListenCallback::Factory(
          std::bind(&Client::server_nym_updated, this, std::placeholders::_1)))
    , server_nym_subscriber_(
          server_.Network().ZeroMQ().SubscribeSocket(server_nym_callback_))
{
    const auto started =
        server_nym_subscriber_->Start(server_.Endpoints().NymDownload());

    assert(started);

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

    assert(serverNym);

    const auto seedID = server_.Crypto().Seed().DefaultSeed();
    const auto words = [&] {
        auto out = client_.Factory().Secret(0);

        if (false == seedID.empty()) {
            out->AssignText(
                server_.Crypto().Seed().Words(seedID, server_reason_));
        }

        return out;
    }();
    const auto phrase = [&] {
        auto out = client_.Factory().Secret(0);

        if (false == seedID.empty()) {
            out->AssignText(
                server_.Crypto().Seed().Passphrase(seedID, server_reason_));
        }

        return out;
    }();
    const auto imported = client_.Crypto().Seed().ImportSeed(
        words,
        phrase,
        crypto::SeedStyle::BIP39,
        crypto::Language::en,
        client_reason_);

    assert(imported == seedID);

    {
        auto params = crypto::Parameters{seedID, 0};
        auto clientNym =
            client_.Wallet().Nym(params, client_reason_, serverNym->Name());

        assert(clientNym);
        assert(clientNym->CompareID(server_.NymID()));
    }
}

void Client::migrate_contract() const
{
    const auto serverContract = server_.Wallet().Server(server_.ID());

    assert(0 != serverContract->Version());

    const auto proto = [&] {
        auto out = Space{};
        serverContract->Serialize(writer(out), true);

        return out;
    }();
    auto clientContract = client_.Wallet().Server(reader(proto));

    assert(0 != clientContract->Version());
}

void Client::migrate_nym() const
{
    const auto serverNym = server_.Wallet().Nym(server_.NymID());

    assert(serverNym);

    auto clientNym =
        client_.Wallet().mutable_Nym(server_.NymID(), client_reason_);
    const auto proto = [&] {
        auto out = Space{};
        serverNym->Claims().Serialize(writer(out));

        return out;
    }();
    clientNym.SetContactData(reader(proto), client_reason_);
}

void Client::server_nym_updated(const network::zeromq::Message& message) const
{
    if (1 > message.Body().size()) {
        LogError()(OT_METHOD)(__FUNCTION__)(": Missing nym ID.").Flush();

        return;
    }

    const auto& frame = message.Body_at(0);
    const auto id = client_.Factory().NymID(frame);

    if (server_.NymID() == id) { migrate_nym(); }
}

void Client::set_address_type() const
{
    if (static_cast<core::AddressType>(network_) !=
        client_.ZMQ().DefaultAddressType()) {
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
