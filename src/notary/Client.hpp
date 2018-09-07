// Copyright (c) 2018 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <opentxs/opentxs.hpp>

namespace opentxs::notary
{
class Client
{
public:
    Client(
        const api::client::Manager& client,
        const api::server::Manager& server,
        const int network);

    ~Client() = default;

private:
    const api::client::Manager& client_;
    const api::server::Manager& server_;
    const int network_{1};
    OTZMQListenCallback server_nym_callback_;
    OTZMQSubscribeSocket server_nym_subscriber_;

    void import_nym() const;
    void migrate_contract() const;
    void migrate_nym() const;
    void server_nym_updated(const network::zeromq::Message& message) const;
    void set_address_type() const;
    void test_nym() const;

    Client() = delete;
    Client(const Client&) = delete;
    Client(Client&&) = delete;
    Client& operator=(const Client&) = delete;
    Client& operator=(Client&&) = delete;
};
}  // namespace opentxs::notary
