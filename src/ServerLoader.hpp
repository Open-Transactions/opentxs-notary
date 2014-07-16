#ifndef __OPENTXS_SERVERLOADER_HPP__
#define __OPENTXS_SERVERLOADER_HPP__

#include "OTServer.hpp"
#include <opentxs/core/OTCachedKey.hpp>
#include <opentxs/core/OTDataFolder.hpp>
#include <opentxs/core/OTCrypto.hpp>
#include <opentxs/core/OTLog.hpp>

#define SERVER_CONFIG_KEY "server"

namespace opentxs
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

} // namespace opentxs

#endif // __OPENTXS_SERVERLOADER_HPP__
