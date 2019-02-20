#include "stdafx.h"
#include "PluginParticleBodyForceCore.h"
#include "CExchangeProperties.h"

using namespace NApiPbf;

EXPORT_MACRO IPluginParticleBodyForce* GETPBFINSTANCE()
{
    return new CExchangeProperties();
}

EXPORT_MACRO void RELEASEPBFINSTANCE(IPluginParticleBodyForce* plugin)
{
    if (0 != plugin)
    {
        delete plugin;
    }
}

EXPORT_MACRO int GETEFINTERFACEVERSION()
{
    static const int INTERFACE_VERSION_MAJOR = 0x02;
    static const int INTERFACE_VERSION_MINOR = 0x01;
    static const int INTERFACE_VERSION_PATCH = 0x00;

    return (INTERFACE_VERSION_MAJOR << 16 |
            INTERFACE_VERSION_MINOR << 8 |
            INTERFACE_VERSION_PATCH);
}