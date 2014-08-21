#ifndef __OPENTXS_MAINFILE_HPP__
#define __OPENTXS_MAINFILE_HPP__

#include <opentxs/core/OTString.hpp>

namespace opentxs
{

class OTServer;

class MainFile
{
public:
    explicit MainFile(OTServer* server);

    bool CreateMainFile();
    bool LoadMainFile(bool readOnly = false);
    bool LoadServerUserAndContract();
    bool SaveMainFile();
    bool SaveMainFileToString(OTString& filename);

private:
    OTString m_strVersion;
    OTServer* server_; // TODO: remove when feasible
};

} // namespace opentxs

#endif // __OPENTXS_MAINFILE_HPP__
