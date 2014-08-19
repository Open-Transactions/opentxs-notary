#ifndef __OPENTXS_CONFIGLOADER_HPP__
#define __OPENTXS_CONFIGLOADER_HPP__

namespace opentxs
{

class OTString;

struct ConfigLoader
{
    static bool load(OTString& walletFilename);
};

} // namespace opentxs

#endif // __OPENTXS_CONFIGLOADER_HPP__
