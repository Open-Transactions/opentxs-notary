// Copyright (c) 2011-2021 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <opentxs/opentxs.hpp>

#include <boost/program_options.hpp>

#include "Client.hpp"

#include <chrono>
#include <map>
#include <memory>
#include <string>

#define NOTARY_ARGUMENT_ADVERTISE "advertise"
#define NOTARY_ARGUMENT_ADVERTISE_NETWORK "advertise-network"
#define NOTARY_OPTION_ONLY_INIT "only-init"
#define NOTARY_OPTION_VERSION "version"

namespace po = boost::program_options;

constexpr auto help_{"help"};
constexpr auto home_{"data_dir"};

struct Options {
    opentxs::ArgList args_{};
    bool show_help_{false};
    bool show_version_{false};
    bool only_init_{false};
    bool start_client_{false};
    int network_{1};
};

auto options() noexcept -> po::options_description&;
auto process_arguments() noexcept -> Options;
auto read_options(int argc, char** argv) noexcept -> bool;
auto variables() noexcept -> po::variables_map&;

auto main(int argc, char* argv[]) -> int
{
    if (false == read_options(argc, argv)) { return 1; }

    const auto options = process_arguments();

    if (options.show_version_) {
        std::cout << "opentxs library-" << OPENTXS_VERSION_STRING << '\n';

        return 0;
    }

    if (options.show_help_) {
        std::cout << ::options() << '\n';

        return 0;
    }

    std::unique_ptr<opentxs::notary::Client> client{nullptr};
    opentxs::Signals::Block();
    const auto& ot = opentxs::InitContext(options.args_);
    const auto& server = ot.StartServer(options.args_, 0);
    auto shutdown = opentxs::api::Context::ShutdownCallback{
        [&]() noexcept { client.reset(); }};
    ot.HandleSignals(&shutdown);

    if (options.only_init_) { opentxs::Cleanup(); }

    if (options.start_client_) {
        const auto& otClient = ot.StartClient(options.args_, 0);
        client.reset(
            new opentxs::notary::Client(otClient, server, options.network_));
    }

    opentxs::Join();

    return 0;
}

auto options() noexcept -> po::options_description&
{
    static auto output = po::options_description{};

    return output;
}

auto process_arguments() noexcept -> Options
{
    auto output = Options{};
    auto& args = output.args_;
    auto home = std::string{};
    auto onlyInit = std::string{};
    auto startClient = std::string{};
    auto logLevel = int{0};
    auto network = int{1};

    for (const auto& [name, value] : variables()) {
        if (name == help_) {
            output.show_help_ = true;
        } else if (name == home_) {
            try {
                home = value.as<decltype(home)>();
            } catch (...) {
            }
        } else if (name == NOTARY_OPTION_VERSION) {
            output.show_version_ = true;
        } else if (name == OPENTXS_ARG_LOGLEVEL) {
            try {
                logLevel = value.as<decltype(logLevel)>();
            } catch (...) {
            }
        } else if (name == NOTARY_OPTION_ONLY_INIT) {
            try {
                onlyInit = value.as<decltype(onlyInit)>();
            } catch (...) {
            }
        } else if (name == NOTARY_ARGUMENT_ADVERTISE) {
            try {
                startClient = value.as<decltype(startClient)>();
            } catch (...) {
            }
        } else if (name == NOTARY_ARGUMENT_ADVERTISE_NETWORK) {
            try {
                network = value.as<decltype(network)>();
            } catch (...) {
            }
        } else {
            try {
                args[name].emplace(value.as<std::string>());
            } catch (...) {
                continue;
            }
        }
    }

    args[OPENTXS_ARG_LOGLEVEL].emplace(std::to_string(logLevel));
    args[OPENTXS_ARG_HOME].emplace(home);
    args[NOTARY_OPTION_ONLY_INIT].emplace(onlyInit);
    args[NOTARY_ARGUMENT_ADVERTISE].emplace(startClient);
    args[NOTARY_ARGUMENT_ADVERTISE_NETWORK].emplace(std::to_string(network));

    if ("true" == onlyInit) { output.only_init_ = true; }
    if ("true" == startClient) { output.start_client_ = true; }

    return output;
}

auto read_options(int argc, char** argv) noexcept -> bool
{
    options().add_options()(help_, "Display this message");
    options().add_options()(
        OPENTXS_ARG_LOGLEVEL,
        po::value<int>(),
        "Log verbosity. Valid values are -1 through 5. Higher numbers are more "
        "verbose. Default value is 0");
    options().add_options()(
        home_,
        po::value<std::string>()->default_value(
            opentxs::api::Context::SuggestFolder("opentxs-notary")),
        "Path to data directory");
    options().add_options()(
        OPENTXS_ARG_BACKUP_DIRECTORY,
        po::value<std::string>(),
        "Path to directory for storage backup plugin");
    options().add_options()(
        OPENTXS_ARG_BINDIP,
        po::value<std::string>(),
        "Local IP address to which to bind");
    options().add_options()(
        OPENTXS_ARG_COMMANDPORT,
        po::value<std::string>(),
        "Public primary port number");
    options().add_options()(
        OPENTXS_ARG_EEP, po::value<std::string>(), "Public I2P endpoint");
    options().add_options()(
        OPENTXS_ARG_GC,
        po::value<std::string>(),
        "Seconds between storage garbage collection operation");
    options().add_options()(
        OPENTXS_ARG_EXTERNALIP,
        po::value<std::string>(),
        "Public ipv4 endpoint for server");
    options().add_options()(
        OPENTXS_ARG_LISTENCOMMAND,
        po::value<std::string>(),
        "Local primary port number");
    options().add_options()(
        OPENTXS_ARG_LISTENNOTIFY,
        po::value<std::string>(),
        "Local notification port number");
    options().add_options()(
        OPENTXS_ARG_NAME, po::value<std::string>(), "Server name");
    options().add_options()(
        OPENTXS_ARG_NOTIFICATIONPORT,
        po::value<std::string>(),
        "Public notification port number");
    options().add_options()(
        OPENTXS_ARG_ONION, po::value<std::string>(), "Public Tor endpoint");
    options().add_options()(
        OPENTXS_ARG_STORAGE_PLUGIN,
        po::value<std::string>(),
        "Primary storage plugin");
    options().add_options()(
        OPENTXS_ARG_TERMS,
        po::value<std::string>(),
        "Server terms and conditions");
    options().add_options()(
        NOTARY_OPTION_ONLY_INIT,
        po::value<std::string>()->implicit_value("true"),
        "Create or verify notary contract only");
    options().add_options()(
        NOTARY_OPTION_VERSION, "Show application version and exit");
    options().add_options()(
        NOTARY_ARGUMENT_ADVERTISE,
        po::value<std::string>(),
        "Upload the notary contract to the introduction notary");
    options().add_options()(
        NOTARY_ARGUMENT_ADVERTISE_NETWORK,
        po::value<std::string>(),
        "Connection mode to introduction server. 1 = ipv4, 2 = ipv6, 3 = tor, "
        "4  = i2p");

    try {
        po::store(po::parse_command_line(argc, argv, options()), variables());
        po::notify(variables());

        return true;
    } catch (po::error& e) {
        std::cerr << "ERROR: " << e.what() << "\n\n" << options() << std::endl;

        return false;
    }
}

auto variables() noexcept -> po::variables_map&
{
    static auto output = po::variables_map{};

    return output;
}
