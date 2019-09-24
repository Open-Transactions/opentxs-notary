// Copyright (c) 2018 The Open-Transactions developers
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
#define OT_METHOD "opentxs_notary::"

namespace po = boost::program_options;

void cleanup_globals();
po::variables_map& variables();
po::options_description& options();

static po::variables_map* variables_{};
static po::options_description* options_{};

po::variables_map& variables()
{
    if (nullptr == variables_) { variables_ = new po::variables_map; }

    return *variables_;
}

po::options_description& options()
{
    if (nullptr == options_) {
        options_ = new po::options_description{"Options"};
    }

    return *options_;
}

void cleanup_globals()
{
    if (nullptr != variables_) {
        delete variables_;
        variables_ = nullptr;
    }

    if (nullptr != options_) {
        delete options_;
        options_ = nullptr;
    }
}

void process_arguments(
    int argc,
    char* argv[],
    opentxs::ArgList& args,
    bool& version,
    bool& onlyInit,
    bool& startClient,
    int& network,
    std::chrono::seconds& gcInterval);
void read_options(int argc, char** argv);

int main(int argc, char* argv[])
{
    read_options(argc, argv);
    opentxs::ArgList args;
    bool onlyInit{false};
    bool version{false};
    bool startClient{false};
    int network{1};
    std::chrono::seconds gc{0};
    std::unique_ptr<opentxs::notary::Client> client{nullptr};
    process_arguments(
        argc, argv, args, version, onlyInit, startClient, network, gc);

    if (version) {
        opentxs::LogNormal("opentxs library-")(OPENTXS_VERSION_STRING).Flush();

        return 0;
    }

    opentxs::Signals::Block();
    const auto& ot = opentxs::InitContext(args, gc);
    const auto& server = ot.StartServer(args, 0);
    auto shutdown =
        opentxs::api::Context::ShutdownCallback{[&] { client.reset(); }};
    ot.HandleSignals(&shutdown);

    if (onlyInit) { opentxs::Cleanup(); }

    if (startClient) {
        const auto& otClient = ot.StartClient(args, 0);
        client.reset(new opentxs::notary::Client(otClient, server, network));
    }

    opentxs::Join();
    cleanup_globals();

    return 0;
}

void process_arguments(
    int argc,
    char* argv[],
    opentxs::ArgList& args,
    bool& version,
    bool& onlyInit,
    bool& startClient,
    int& network,
    std::chrono::seconds& gcInterval)
{
    for (const auto& [name, value] : variables()) {
        args[name].emplace(value.as<std::string>());
    }

    const auto gcIt = args.find(OPENTXS_ARG_GC);
    const auto storageIt = args.find(OPENTXS_ARG_STORAGE_PLUGIN);
    const auto backupIt = args.find(OPENTXS_ARG_BACKUP_DIRECTORY);
    const auto advertiseIt = args.find(NOTARY_ARGUMENT_ADVERTISE);
    const auto networkIt = args.find(NOTARY_ARGUMENT_ADVERTISE_NETWORK);

    if (args.end() != gcIt) {
        OT_ASSERT(2 > gcIt->second.size())
        OT_ASSERT(0 < gcIt->second.size())
        const auto& gc = *gcIt->second.cbegin();
        try {
            gcInterval = std::chrono::seconds(std::stoll(gc));
            opentxs::LogOutput(OT_METHOD)(__FUNCTION__)(
                ": * Setting storage garbage collection interval to ")(
                gcInterval.count())(" seconds.")
                .Flush();
        } catch (const std::invalid_argument&) {
        } catch (const std::out_of_range&) {
        }
    }

    if (args.end() != storageIt) {
        OT_ASSERT(2 > storageIt->second.size())
        OT_ASSERT(0 < storageIt->second.size())
        const auto& storage = *storageIt->second.cbegin();
        opentxs::LogOutput(OT_METHOD)(__FUNCTION__)(
            ": * Setting primary storage plugin to ")(storage)(".")
            .Flush();
    }

    if (args.end() != backupIt) {
        OT_ASSERT(2 > backupIt->second.size())
        OT_ASSERT(0 < backupIt->second.size())
        const auto& backup = *backupIt->second.cbegin();
        opentxs::LogOutput(OT_METHOD)(__FUNCTION__)(
            ": * Setting backup directory to ")(backup)(".")
            .Flush();
    }

    if (args.end() != advertiseIt) {
        OT_ASSERT(2 > advertiseIt->second.size())
        OT_ASSERT(0 < advertiseIt->second.size())
        const auto& advertise = *advertiseIt->second.cbegin();

        if (advertise == "true") {
            opentxs::LogOutput(OT_METHOD)(__FUNCTION__)(
                ": * Advertising notary contract on introduction server.")
                .Flush();
            startClient = true;
        } else {
            startClient = false;
        }
    }

    if (args.end() != networkIt) {
        OT_ASSERT(2 > networkIt->second.size())
        OT_ASSERT(0 < networkIt->second.size())
        const auto& networkValue = *networkIt->second.cbegin();

        try {
            network = std::stoi(networkValue);
        } catch (...) {
            network = 1;
        }
    } else {
        network = 1;
    }

    version = false;
    onlyInit = false;

    for (int i = 1; i < argc; ++i) {
        const std::string arg(argv[i]);

        if (0 == arg.compare("--version")) {
            version = true;
        } else if (0 == arg.compare("--only-init")) {
            onlyInit = true;
        }
    }
}

void read_options(int argc, char** argv)
{
    options().add_options()(
        OPENTXS_ARG_BACKUP_DIRECTORY,
        po::value<std::string>(),
        "Path to directory for storage backup plugin")(
        OPENTXS_ARG_BINDIP,
        po::value<std::string>(),
        "Local IP address to which to bind")(
        OPENTXS_ARG_COMMANDPORT,
        po::value<std::string>(),
        "Public primary port number")(
        OPENTXS_ARG_EEP, po::value<std::string>(), "Public I2P endpoint")(
        OPENTXS_ARG_GC,
        po::value<std::string>(),
        "Seconds between storage garbage collection operation")(
        OPENTXS_ARG_EXTERNALIP,
        po::value<std::string>(),
        "Public ipv4 endpoint for server")(
        OPENTXS_ARG_LISTENCOMMAND,
        po::value<std::string>(),
        "Local primary port number")(
        OPENTXS_ARG_LISTENNOTIFY,
        po::value<std::string>(),
        "Local notification port number")(
        OPENTXS_ARG_NAME, po::value<std::string>(), "Server name")(
        OPENTXS_ARG_NOTIFICATIONPORT,
        po::value<std::string>(),
        "Public notification port number")(
        OPENTXS_ARG_ONION, po::value<std::string>(), "Public Tor endpoint")(
        OPENTXS_ARG_STORAGE_PLUGIN,
        po::value<std::string>(),
        "Primary storage plugin")(
        OPENTXS_ARG_TERMS,
        po::value<std::string>(),
        "Server terms and conditions")(
        NOTARY_OPTION_ONLY_INIT,
        po::value<std::string>()->implicit_value("true"),
        "Create or verify notary contract only")(
        NOTARY_OPTION_VERSION,
        po::value<std::string>()->implicit_value("true"),
        "Display notary version only")(
        NOTARY_ARGUMENT_ADVERTISE,
        po::value<std::string>(),
        "Upload the notary contract to the introduction notary")(
        NOTARY_ARGUMENT_ADVERTISE_NETWORK,
        po::value<std::string>(),
        "Connection mode to introduction server. 1 = ipv4, 2 = ipv6, 3 = tor, "
        "4  = i2p");

    try {
        po::store(po::parse_command_line(argc, argv, options()), variables());
        po::notify(variables());
    } catch (po::error& e) {
        std::cerr << "ERROR: " << e.what() << "\n\n" << options() << std::endl;
    }
}
