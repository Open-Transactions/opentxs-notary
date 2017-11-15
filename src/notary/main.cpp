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

#include <opentxs/api/Native.hpp>
#include <opentxs/api/Server.hpp>
#include <opentxs/core/Log.hpp>
#include <opentxs/util/Signals.hpp>
#include <opentxs/OT.hpp>

#include <anyoption/anyoption.hpp>

#include <chrono>
#include <map>
#include <string>

void process_arguments(
    int argc,
    char* argv[],
    std::map<std::string, std::string>& arguments,
    bool& version,
    bool& onlyInit,
    std::chrono::seconds& gcInterval,
    std::string& storagePlugin,
    std::string& backupDirectory);

int main(int argc, char* argv[])
{
    std::map<std::string, std::string> serverArgs{};
    bool onlyInit{false};
    bool version{false};
    std::chrono::seconds gc{0};
    std::string storage{""};
    std::string backup{""};
    process_arguments(
        argc, argv, serverArgs, version, onlyInit, gc, storage, backup);

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
    opentxs::OT::ServerFactory(serverArgs, gc, storage, backup);
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
    std::map<std::string, std::string>& arguments,
    bool& version,
    bool& onlyInit,
    std::chrono::seconds& gcInterval,
    std::string& storagePlugin,
    std::string& backupDirectory)
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
    const std::string createOptions[] = {
        OT_SERVER_OPTION_BACKUP,
        OT_SERVER_OPTION_BINDIP,
        OT_SERVER_OPTION_COMMANDPORT,
        OT_SERVER_OPTION_EEP,
        OT_SERVER_OPTION_GC,
        OT_SERVER_OPTION_EXTERNALIP,
        OT_SERVER_OPTION_LISTENCOMMAND,
        OT_SERVER_OPTION_LISTENNOTIFY,
        OT_SERVER_OPTION_NAME,
        OT_SERVER_OPTION_NOTIFICATIONPORT,
        OT_SERVER_OPTION_ONION,
        OT_SERVER_OPTION_STORAGE,
        OT_SERVER_OPTION_TERMS
    };
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
            arguments[optionName] = value;
        }
    }

    const auto gcIt = arguments.find(OT_SERVER_OPTION_GC);
    const auto storageIt = arguments.find(OT_SERVER_OPTION_STORAGE);
    const auto backupIt = arguments.find(OT_SERVER_OPTION_BACKUP);

    if (arguments.end() != gcIt) {
        try {
            gcInterval = std::chrono::seconds(std::stoll(gcIt->second));
            opentxs::otErr
                << ": Setting storage garbage collection interval to "
                << gcInterval.count() << " seconds" << std::endl;
        } catch (const std::invalid_argument&) {
        } catch (const std::out_of_range&) {
        }
    }

    if (arguments.end() != storageIt) {
        storagePlugin = storageIt->second;
        opentxs::otErr << ": Setting primary storage plugin to "
                       << storagePlugin << std::endl;
    }

    if (arguments.end() != backupIt) {
        backupDirectory = backupIt->second;
        opentxs::otErr << ": Setting backup directory to " << backupDirectory
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
