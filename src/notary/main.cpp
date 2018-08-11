// Copyright (c) 2018 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <opentxs/opentxs.hpp>

#include <boost/program_options.hpp>

#include <chrono>
#include <map>
#include <memory>
#include <string>

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
    std::chrono::seconds& gcInterval);
void read_options(int argc, char** argv);

int main(int argc, char* argv[])
{
    read_options(argc, argv);
    opentxs::ArgList args;
    bool onlyInit{false};
    bool version{false};
    std::chrono::seconds gc{0};
    process_arguments(argc, argv, args, version, onlyInit, gc);

    if (version) {
        opentxs::otOut << "opentxs server " << OPENTXS_SERVER_VERSION_STRING
                       << std::endl;
        opentxs::otOut << "opentxs library " << OPENTXS_VERSION_STRING
                       << std::endl;
        opentxs::otOut << "Copyright (C) 2017 Open Transactions Developers"
                       << std::endl;

        return 0;
    }

    opentxs::Signals::Block();
    opentxs::OT::ServerFactory(args, gc);
    opentxs::OT::App().HandleSignals();

    if (onlyInit) { opentxs::OT::Cleanup(); }

    opentxs::OT::Join();

    cleanup_globals();

    return 0;
}

void process_arguments(
    int argc,
    char* argv[],
    opentxs::ArgList& args,
    bool& version,
    bool& onlyInit,
    std::chrono::seconds& gcInterval)
{
    for (const auto& [name, value] : variables()) {
        args[name].emplace(value.as<std::string>());
    }

    const auto gcIt = args.find(OPENTXS_ARG_GC);
    const auto storageIt = args.find(OPENTXS_ARG_STORAGE_PLUGIN);
    const auto backupIt = args.find(OPENTXS_ARG_BACKUP_DIRECTORY);

    if (args.end() != gcIt) {
        OT_ASSERT(2 > gcIt->second.size());
        OT_ASSERT(0 < gcIt->second.size());
        const auto& gc = *gcIt->second.cbegin();
        try {
            gcInterval = std::chrono::seconds(std::stoll(gc));
            opentxs::otErr
                << ": Setting storage garbage collection interval to "
                << gcInterval.count() << " seconds" << std::endl;
        }
        catch (const std::invalid_argument&) {
        }
        catch (const std::out_of_range&) {
        }
    }

    if (args.end() != storageIt) {
        OT_ASSERT(2 > storageIt->second.size());
        OT_ASSERT(0 < storageIt->second.size());
        const auto& storage = *storageIt->second.cbegin();
        opentxs::otErr << ": Setting primary storage plugin to " << storage
                       << std::endl;
    }

    if (args.end() != backupIt) {
        OT_ASSERT(2 > backupIt->second.size());
        OT_ASSERT(0 < backupIt->second.size());
        const auto& backup = *backupIt->second.cbegin();
        opentxs::otErr << ": Setting backup directory to " << backup
                       << std::endl;
    }

    version = false;
    onlyInit = false;

    for (int i = 1; i < argc; ++i) {
        const std::string arg(argv[i]);

        if (0 == arg.compare("--version")) {
            version = true;
        }
        else if (0 == arg.compare("--only-init")) {
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
        OPENTXS_ARG_EEP,
        po::value<std::string>(),
        "Public I2P endpoint")(
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
        OPENTXS_ARG_NAME,
        po::value<std::string>(),
        "Server name")(
        OPENTXS_ARG_NOTIFICATIONPORT,
        po::value<std::string>(),
        "Public notification port number")(
        OPENTXS_ARG_ONION,
        po::value<std::string>(),
        "Public Tor endpoint")(
        OPENTXS_ARG_STORAGE_PLUGIN,
        po::value<std::string>(),
        "Primary storage plugin")(
        OPENTXS_ARG_TERMS,
        po::value<std::string>(),
        "Server terms and conditions");

    try {
        po::store(po::parse_command_line(argc, argv, options()), variables());
        po::notify(variables());
    } catch (po::error& e) {
        std::cerr << "ERROR: " << e.what() << "\n\n" << options() << std::endl;
    }
}
