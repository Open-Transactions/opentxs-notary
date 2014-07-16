/************************************************************
 *
 *  main.cpp
 *
 */

/************************************************************
 -----BEGIN PGP SIGNED MESSAGE-----
 Hash: SHA1

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
 *  Copyright (C) 2010-2013 by "Fellow Traveler" (A pseudonym)
 *
 *  EMAIL:
 *  FellowTraveler@rayservers.net
 *
 *  BITCOIN:  1NtTPVVjDsUfDWybS4BwvHpG2pdS9RnYyQ
 *
 *  KEY FINGERPRINT (PGP Key in license file):
 *  9DD5 90EB 9292 4B48 0484  7910 0308 00ED F951 BB8E
 *
 *  OFFICIAL PROJECT WIKI(s):
 *  https://github.com/FellowTraveler/Moneychanger
 *  https://github.com/FellowTraveler/Open-Transactions/wiki
 *
 *  WEBSITE:
 *  http://www.OpenTransactions.org/
 *
 *  Components and licensing:
 *   -- Moneychanger..A Java client GUI.....LICENSE:.....GPLv3
 *   -- otlib.........A class library.......LICENSE:...LAGPLv3
 *   -- otapi.........A client API..........LICENSE:...LAGPLv3
 *   -- opentxs/ot....Command-line client...LICENSE:...LAGPLv3
 *   -- otserver......Server Application....LICENSE:....AGPLv3
 *  Github.com/FellowTraveler/Open-Transactions/wiki/Components
 *
 *  All of the above OT components were designed and written by
 *  Fellow Traveler, with the exception of Moneychanger, which
 *  was contracted out to Vicky C (bitcointrader4@gmail.com).
 *  The open-source community has since actively contributed.
 *
 *  -----------------------------------------------------
 *
 *   LICENSE:
 *   This program is free software: you can redistribute it
 *   and/or modify it under the terms of the GNU Affero
 *   General Public License as published by the Free Software
 *   Foundation, either version 3 of the License, or (at your
 *   option) any later version.
 *
 *   ADDITIONAL PERMISSION under the GNU Affero GPL version 3
 *   section 7: (This paragraph applies only to the LAGPLv3
 *   components listed above.) If you modify this Program, or
 *   any covered work, by linking or combining it with other
 *   code, such other code is not for that reason alone subject
 *   to any of the requirements of the GNU Affero GPL version 3.
 *   (==> This means if you are only using the OT API, then you
 *   don't have to open-source your code--only your changes to
 *   Open-Transactions itself must be open source. Similar to
 *   LGPLv3, except it applies to software-as-a-service, not
 *   just to distributing binaries.)
 *
 *   Extra WAIVER for OpenSSL, Lucre, and all other libraries
 *   used by Open Transactions: This program is released under
 *   the AGPL with the additional exemption that compiling,
 *   linking, and/or using OpenSSL is allowed. The same is true
 *   for any other open source libraries included in this
 *   project: complete waiver from the AGPL is hereby granted to
 *   compile, link, and/or use them with Open-Transactions,
 *   according to their own terms, as long as the rest of the
 *   Open-Transactions terms remain respected, with regard to
 *   the Open-Transactions code itself.
 *
 *   Lucre License:
 *   This code is also "dual-license", meaning that Ben Lau-
 *   rie's license must also be included and respected, since
 *   the code for Lucre is also included with Open Transactions.
 *   See Open-Transactions/src/otlib/lucre/LUCRE_LICENSE.txt
 *   The Laurie requirements are light, but if there is any
 *   problem with his license, simply remove the Lucre code.
 *   Although there are no other blind token algorithms in Open
 *   Transactions (yet. credlib is coming), the other functions
 *   will continue to operate.
 *   See Lucre on Github:  https://github.com/benlaurie/lucre
 *   -----------------------------------------------------
 *   You should have received a copy of the GNU Affero General
 *   Public License along with this program.  If not, see:
 *   http://www.gnu.org/licenses/
 *
 *   If you would like to use this software outside of the free
 *   software license, please contact FellowTraveler.
 *   (Unfortunately many will run anonymously and untraceably,
 *   so who could really stop them?)
 *
 *   DISCLAIMER:
 *   This program is distributed in the hope that it will be
 *   useful, but WITHOUT ANY WARRANTY; without even the implied
 *   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *   PURPOSE.  See the GNU Affero General Public License for
 *   more details.

 -----BEGIN PGP SIGNATURE-----
 Version: GnuPG v1.4.9 (Darwin)

 iQIcBAEBAgAGBQJRSsfJAAoJEAMIAO35UbuOQT8P/RJbka8etf7wbxdHQNAY+2cC
 vDf8J3X8VI+pwMqv6wgTVy17venMZJa4I4ikXD/MRyWV1XbTG0mBXk/7AZk7Rexk
 KTvL/U1kWiez6+8XXLye+k2JNM6v7eej8xMrqEcO0ZArh/DsLoIn1y8p8qjBI7+m
 aE7lhstDiD0z8mwRRLKFLN2IH5rAFaZZUvj5ERJaoYUKdn4c+RcQVei2YOl4T0FU
 LWND3YLoH8naqJXkaOKEN4UfJINCwxhe5Ke9wyfLWLUO7NamRkWD2T7CJ0xocnD1
 sjAzlVGNgaFDRflfIF4QhBx1Ddl6wwhJfw+d08bjqblSq8aXDkmFA7HeunSFKkdn
 oIEOEgyj+veuOMRJC5pnBJ9vV+7qRdDKQWaCKotynt4sWJDGQ9kWGWm74SsNaduN
 TPMyr9kNmGsfR69Q2Zq/FLcLX/j8ESxU+HYUB4vaARw2xEOu2xwDDv6jt0j3Vqsg
 x7rWv4S/Eh18FDNDkVRChiNoOIilLYLL6c38uMf1pnItBuxP3uhgY6COm59kVaRh
 nyGTYCDYD2TK+fI9o89F1297uDCwEJ62U0Q7iTDp5QuXCoxkPfv8/kX6lS6T3y9G
 M9mqIoLbIQ1EDntFv7/t6fUTS2+46uCrdZWbQ5RjYXdrzjij02nDmJAm2BngnZvd
 kamH0Y/n11lCvo1oQxM+
 =uSzz
 -----END PGP SIGNATURE-----
 **************************************************************/

#include "MessageProcessor.hpp"
#include "OTServer.hpp"

#include <opentxs/core/OTCachedKey.hpp>
#include <opentxs/core/OTCommon.hpp>
#include <opentxs/core/OTCrypto.hpp>
#include <opentxs/core/OTDataFolder.hpp>
#include <opentxs/core/OTLog.hpp>

#include <cassert>

#define SERVER_CONFIG_KEY "server"

using namespace opentxs;

namespace
{

class __ot_server_
{
    OTServer* server_;

public:
    OTServer* GetServer()
    {
        return server_;
    }
    __ot_server_() : server_(nullptr)
    {
// This is optional! (I, of course, am using it in this test app...)
#if defined(OT_SIGNAL_HANDLING)
        OTLog::SetupSignalHandler();
#endif
        // I instantiate this here (instead of globally) so that I am
        // assured that any globals and other
        // setup is already done before we instantiate the server object
        // itself.
        OT_ASSERT_MSG(nullptr == server_,
                      "server main(): ASSERT: nullptr == server_.");
        server_ = new OTServer;

        OT_ASSERT_MSG(
            nullptr != server_,
            "server main(): ASSERT: Unable to instantiate OT server.\n");

        {
            bool setupPathsSuccess = false;
            if (!OTDataFolder::Init(SERVER_CONFIG_KEY)) {
                OT_FAIL;
            }
            else {
                setupPathsSuccess = true;
            }
            OT_ASSERT_MSG(setupPathsSuccess,
                          "main(): Assert failed: Failed to set OT Path");
        }
        OTCrypto::It()->Init();
    }

    ~__ot_server_()
    {
        OTLog::vOutput(0,
                       "\n\n OT version %s, shutting down and cleaning up.\n",
                       OTLog::Version());

        if (server_) {
            delete server_;
            server_ = nullptr;
        }
        OTCachedKey::Cleanup();
        OTCrypto::It()->Cleanup();
#ifdef _WIN32
        WSACleanup();
#endif
    }
};

} // namespace

int main()
{
    if (!OTLog::Init(SERVER_CONFIG_KEY, 0)) {
        assert(false);
    }

    OTLog::vOutput(
        0, "\n\nWelcome to Open Transactions... Test Server -- version %s\n"
           "(transport build: OTMessage -> OTEnvelope -> ZMQ )\n\n",
        OTLog::Version());

// WINSOCK WINDOWS
#ifdef OT_ZMQ_2_MODE
#ifdef _WIN32

    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    int32_t err = WSAStartup(wVersionRequested, &wsaData);

    /* Tell the user that we could not find a usable        */
    /* Winsock DLL.                                            */

    OT_ASSERT_MSG((err == 0), "WSAStartup failed!\n");

    /*    Confirm that the WinSock DLL supports 2.2.            */
    /*    Note that if the DLL supports versions greater        */
    /*    than 2.2 in addition to 2.2, it will still return    */
    /*    2.2 in wVersion since that is the version we        */
    /*    requested.                                            */

    bool winsock =
        (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2);

    /* Tell the user that we could not find a usable */
    /* WinSock DLL.                                  */

    if (!winsock) WSACleanup(); // do cleanup.
    OT_ASSERT_MSG((!bWinsock),
                  "Could not find a usable version of Winsock.dll\n");

    /* The Winsock DLL is acceptable. Proceed to use it. */
    /* Add network programming using Winsock here */
    /* then call WSACleanup when done using the Winsock dll */
    OTLog::vOutput(0, "The Winsock 2.2 dll was found okay\n");
#endif // _WIN32
#endif // OT_ZMQ_2_MODE

    __ot_server_ the_server_obj;
    OTServer* server = the_server_obj.GetServer();

    //    OTString strCAFile, strDHFile, strKeyFile;  //, strSSLPassword;
    //    strCAFile. Format("%s%s%s", OTLog::Path(), OTLog::PathSeparator(),
    // CA_FILE);
    //    strDHFile. Format("%s%s%s", OTLog::Path(), OTLog::PathSeparator(),
    // DH_FILE);
    //    strKeyFile.Format("%s%s%s", OTLog::Path(), OTLog::PathSeparator(),
    // KEY_FILE);
    //
    // UPDATE: This was moved to OTLog::OT_Init(), which is called above, by the
    // nested cleanup class.
    //
    // Initialize SSL -- This MUST occur before any Private Keys are loaded!
    //    SSL_library_init();
    //    SSL_load_error_strings();

    // OTServer::Init loads up server's nym so it can decrypt messages sent in
    // envelopes. It also does various other initialization work.
    //
    // (Envelopes prove that ONLY someone who actually had the server contract,
    //  and had loaded it into his wallet, could ever connect to the server or
    //  communicate with it. And if that person is following the contract, there
    //  is only one server he can connect to, and one key he can use to talk to
    // it.)
    //
    OTLog::vOutput(0, "\nNow loading the server nym, which will also ask you "
                      "for a password, to unlock\n"
                      "its private key.\n");

    server->Init(); // Keys, etc are loaded here. Assumes main path is set!

    // We're going to listen on the same port that is listed in our server
    // contract. The hostname of this server, according to its own contract.
    OTString hostname;
    // The port of this server according to its own contract
    int port = 0;

    bool connectInfo = server->GetConnectInfo(hostname, port);

    OT_ASSERT_MSG(connectInfo, "server main: Unable to find my own connect "
                               "info (which SHOULD be in my server contract, "
                               "BTW.) Perhaps you failed trying to open that "
                               "contract? Have you tried the test password? "
                               "(\"test\")\n");

    // OT CRON
    //
    // A heartbeat for recurring transactions, such as markets, payment plans,
    // and smart contracts.

    server->ActivateCron();

    // NOTE: Currently we trigger OT Cron's processing internally, but there's
    // no reason why, in the
    // future, we can't make an actual cron job that triggers a script, that
    // fires a message
    // to OT, causing OT to process its Cron (even if we were single-threaded we
    // could do this...)
    //
    // Have to put some thought into it...
    //
    // Wouldn't require much security, since OT can still be smart enough not to
    // process cron any
    // more often than X minutes, no matter HOW many times the ProcessCron
    // script fires.
    // Thing is, though, that with this setup, we can't really guarantee that
    // cron will EVER be
    // triggered -- whereas the way OT is now, at least we know it WILL fire
    // every X seconds.

    if (!OTDataFolder::IsInitialized()) {
        OT_FAIL;
    };

    MessageProcessor processor(server, port);
    processor.run();

    return 0;
}
