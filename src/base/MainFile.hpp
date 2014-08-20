#ifndef __OPENTXS_MAINFILE_HPP__
#define __OPENTXS_MAINFILE_HPP__

namespace opentxs
{

class OTString;
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
    OTServer* server_; // TODO: remove when feasible
};

} // namespace opentxs

#endif // __OPENTXS_MAINFILE_HPP__
