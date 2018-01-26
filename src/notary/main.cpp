/************************************************************
 *
 *                 OPEN TRANSACTIONS
 *
 *       Financial Cryptography and Digital Cash
 *       Library, Protocol, API, Server, CLI, GUI
 *
 *       -- Anonymous Numbered Accounts.
 *       -- Untraceable Digital Cash.
 *       -- Triple-Signed Receipts.
 *       -- Cheques, Vouchers, Transfers, Inboxes.
 *       -- Basket Currencies, Markets, Payment Plans.
 *       -- Signed, XML, Ricardian-style Contracts.
 *       -- Scripted smart contracts.
 *
 *  EMAIL:
 *  fellowtraveler@opentransactions.org
 *
 *  WEBSITE:
 *  http://www.opentransactions.org/
 *
 *  -----------------------------------------------------
 *
 *   LICENSE:
 *   This Source Code Form is subject to the terms of the
 *   Mozilla Public License, v. 2.0. If a copy of the MPL
 *   was not distributed with this file, You can obtain one
 *   at http://mozilla.org/MPL/2.0/.
 *
 *   DISCLAIMER:
 *   This program is distributed in the hope that it will
 *   be useful, but WITHOUT ANY WARRANTY; without even the
 *   implied warranty of MERCHANTABILITY or FITNESS FOR A
 *   PARTICULAR PURPOSE.  See the Mozilla Public License
 *   for more details.
 *
 ************************************************************/

#include <opentxs/OT.hpp>
#include <opentxs/api/Native.hpp>
#include <opentxs/api/Server.hpp>
#include <opentxs/core/Log.hpp>
#include <opentxs/util/Signals.hpp>

#include <anyoption/anyoption.hpp>

#include <chrono>
#include <map>
#include <string>

void process_arguments(
    int argc,
    char* argv[],
    opentxs::ArgList& args,
    bool& version,
    bool& onlyInit,
    std::chrono::seconds& gcInterval);

int main(int argc, char* argv[])
{
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

    if (onlyInit) {
        opentxs::OT::Cleanup();
    }

    opentxs::OT::Join();

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
    // -------------------------------------------------------
    // Process the command-line options for creating a new server contract.
    //
    // (Not used for most server start-ups, but only used when the server
    // contract is first created.)
    /*
     --terms <human-readable terms of use>
     --externalip <externally-visible hostname>
     --commandport <externally-visible port where opentxs commands can be sent>
     --notificationport <externally-visible port where to listen for push
     notifications>
     --bindip <internal ip address where the listen sockets will be opened>
     --listencommand <internal port number where the opentxs listen socket
     will bind>
     --listennotification <internal port number where the push notification
     socket will bind>
     --name <server name>
     --onion <tor hidden service hostname>
     --eep <i2p eepsite hostname>
     --storage primary storage plugin
     --backup storage backup directory
     */
    const std::string createOptions[] = {OPENTXS_ARG_BACKUP_DIRECTORY,
                                         OPENTXS_ARG_BINDIP,
                                         OPENTXS_ARG_COMMANDPORT,
                                         OPENTXS_ARG_EEP,
                                         OPENTXS_ARG_GC,
                                         OPENTXS_ARG_EXTERNALIP,
                                         OPENTXS_ARG_LISTENCOMMAND,
                                         OPENTXS_ARG_LISTENNOTIFY,
                                         OPENTXS_ARG_NAME,
                                         OPENTXS_ARG_NOTIFICATIONPORT,
                                         OPENTXS_ARG_ONION,
                                         OPENTXS_ARG_STORAGE_PLUGIN,
                                         OPENTXS_ARG_TERMS};
    AnyOption options;

    for (const auto& optionName : createOptions) {
        if (!options.findOption(optionName.c_str())) {
            options.setCommandOption(optionName.c_str());
        }
    }

    options.processCommandArgs(argc, argv);

    for (const auto& optionName : createOptions) {
        const char* value = options.getValue(optionName.c_str());

        if (nullptr != value) {
            args[optionName].emplace(value);
        }
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
        } catch (const std::invalid_argument&) {
        } catch (const std::out_of_range&) {
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
        } else if (0 == arg.compare("--only-init")) {
            onlyInit = true;
        }
    }
}
