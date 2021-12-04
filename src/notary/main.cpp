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
#define NOTARY_ARGUMENT_ADVERTISE_NETWORK "advertise_network"
#define NOTARY_OPTION_ONLY_INIT "only_init"
#define NOTARY_OPTION_VERSION "version"

namespace po = boost::program_options;

constexpr auto help_{"help"};

struct Options {
    opentxs::Options args_{};
    bool show_help_{false};
    bool show_version_{false};
    bool only_init_{false};
    bool start_client_{false};
    int network_{1};
};

auto options() noexcept -> const po::options_description&;
auto process_arguments(int argc, char** argv) noexcept -> Options;
auto read_options(int argc, char** argv) noexcept -> bool;
auto variables() noexcept -> po::variables_map&;

auto main(int argc, char* argv[]) -> int
{
    if (false == read_options(argc, argv)) { return 1; }

    const auto options = process_arguments(argc, argv);

    if (options.show_version_) {
        std::cout << "opentxs library-" << opentxs::VersionString() << '\n';

        return 0;
    }

    if (options.show_help_) {
        std::cout << ::options() << '\n' << options.args_.HelpText() << '\n';

        return 0;
    }

    std::unique_ptr<opentxs::notary::Client> client{nullptr};
    opentxs::Signals::Block();
    const auto& ot = opentxs::InitContext(options.args_);
    const auto& server = ot.StartNotarySession(options.args_, 0);
    auto shutdown = opentxs::api::Context::ShutdownCallback{
        [&]() noexcept { client.reset(); }};
    ot.HandleSignals(&shutdown);

    if (options.only_init_) { opentxs::Cleanup(); }

    if (options.start_client_) {
        const auto& otClient = ot.StartClientSession(options.args_, 0);
        client.reset(
            new opentxs::notary::Client(otClient, server, options.network_));
    }

    opentxs::Join();

    return 0;
}

auto options() noexcept -> const po::options_description&
{
    static const auto output = [] {
        auto out = po::options_description{"opentxs-notary options"};
        out.add_options()(help_, "Display this message");
        out.add_options()(
            NOTARY_OPTION_ONLY_INIT,
            po::value<std::string>()->implicit_value("true"),
            "Create or verify notary contract only");
        out.add_options()(
            NOTARY_OPTION_VERSION, "Show application version and exit");
        out.add_options()(
            NOTARY_ARGUMENT_ADVERTISE,
            po::value<std::string>(),
            "Upload the notary contract to the introduction notary");
        out.add_options()(
            NOTARY_ARGUMENT_ADVERTISE_NETWORK,
            po::value<std::string>(),
            "Connection mode to introduction server. 1 = ipv4, 2 = ipv6, 3 = "
            "tor, 4  = i2p");

        return out;
    }();

    return output;
}

auto process_arguments(int argc, char** argv) noexcept -> Options
{
    auto output = Options{};
    auto& args = output.args_;
    args.SetHome(
        opentxs::api::Context::SuggestFolder("opentxs-notary").c_str());
    args.ParseCommandLine(argc, argv);
    auto onlyInit = std::string{};
    auto startClient = std::string{};
    auto network = int{1};

    for (const auto& [name, value] : variables()) {
        if (name == help_) {
            output.show_help_ = true;
        } else if (name == NOTARY_OPTION_VERSION) {
            output.show_version_ = true;
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
        }
    }

    if ("true" == onlyInit) { output.only_init_ = true; }
    if ("true" == startClient) { output.start_client_ = true; }

    return output;
}

auto read_options(int argc, char** argv) noexcept -> bool
{
    try {
        const auto parsed = po::command_line_parser(argc, argv)
                                .options(options())
                                .allow_unregistered()
                                .run();
        po::store(parsed, variables());
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
