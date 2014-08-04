#ifndef __OPENTXS_SERVERLOADER_HPP__
#define __OPENTXS_SERVERLOADER_HPP__

#include "OTServer.hpp"
#include <opentxs/core/OTCachedKey.hpp>
#include <opentxs/core/OTDataFolder.hpp>
#include <opentxs/core/OTCrypto.hpp>
#include <opentxs/core/OTLog.hpp>

extern "C" {
#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif
}

#define SERVER_CONFIG_KEY "server"

namespace opentxs
{

class ServerLoader
{
public:
    ServerLoader() : server_(nullptr)
    {
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

        OT_ASSERT_MSG(!bWinsock,
                      "Could not find a usable version of Winsock.dll\n");

        /* The Winsock DLL is acceptable. Proceed to use it. */
        /* Add network programming using Winsock here */
        /* then call WSACleanup when done using the Winsock dll */
        OTLog::vOutput(0, "The Winsock 2.2 dll was found okay\n");
#endif // _WIN32
#endif // OT_ZMQ_2_MODE

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

            if (!OTDataFolder::IsInitialized()) {
                OT_FAIL;
            }
        }
        OTCrypto::It()->Init();

        // OTServer::Init loads up server's nym so it can decrypt messages sent
        // in envelopes. It also does various other initialization work.
        //
        // NOTE: Envelopes prove that ONLY someone who actually had the server
        // contract, and had loaded it into his wallet, could ever connect to
        // the server or communicate with it. And if that person is following
        // the contract, there is only one server he can connect to, and one
        // key he can use to talk to it.
        //
        // Keys, etc are loaded here. Assumes main path is set!
        server_->Init();

        // A heartbeat for recurring transactions, such as markets, payment
        // plans, and smart contracts.
        server_->ActivateCron();
    }

    ~ServerLoader()
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

    OTServer* getServer()
    {
        return server_;
    }

    int getPort() const
    {
        // We're going to listen on the same port that is listed in our server
        // contract. The hostname of this server, according to its own contract.
        OTString hostname;
        int port = 0;
        bool connectInfo = server_->GetConnectInfo(hostname, port);

        OT_ASSERT_MSG(connectInfo,
                      "server main: Unable to find my own connect "
                      "info (which SHOULD be in my server contract, "
                      "BTW.) Perhaps you failed trying to open that "
                      "contract? Have you tried the test password? "
                      "(\"test\")\n");
        return port;
    }

private:
    ServerLoader(const ServerLoader&);
    ServerLoader& operator=(const ServerLoader&);

    OTServer* server_;
};

} // namespace opentxs

#endif // __OPENTXS_SERVERLOADER_HPP__
