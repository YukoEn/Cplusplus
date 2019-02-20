#include "stdafx.h"

#if !defined(CExchangeProperties_h)
#define CExchangeProperties_h

#include <string>

#include "IPluginParticleBodyForceV2_1_0.h"

// This class provides an implementation of IPluginParticleBodyForceV2_1_0

class CExchangeProperties : public NApiPbf::IPluginParticleBodyForceV2_1_0
{
    public:
        /**
         * Name of the properties
         */
		static const std::string TEMPERATURE_PROPERTY;
		static const std::string PROPERTY_TO_EXCHANGE1;
		static const std::string PROPERTY_TO_EXCHANGE2;
		static const std::string PROPERTY_TO_EXCHANGE3;
		static const std::string PROPERTY_TO_EXCHANGE4;
		static const std::string PROPERTY_TO_EXCHANGE5;

        /**
         * Constructor, does nothing
         */
        CExchangeProperties();

        /**
         * Destructor, does nothing
         */
        virtual ~CExchangeProperties();

        virtual void getPreferenceFileName(char prefFileName[NApi::FILE_PATH_MAX_LENGTH]);

        virtual bool isThreadSafe();

        virtual bool usesCustomProperties();

        virtual bool setup(NApiCore::IApiManager_1_0& apiManager,
                               const char                 prefFile[]);

        virtual bool starting(NApiCore::IApiManager_1_0& apiManager);

        virtual void stopping(NApiCore::IApiManager_1_0& apiManager);

        virtual NApi::ECalculateResult externalForce(
                                               double       time,
                                               double       timestep,
                                               int          id,
                                               const char   type[],
                                               double       mass,
                                               double       volume,
                                               double       posX,
                                               double       posY,
                                               double       posZ,
                                               double       velX,
                                               double       velY,
                                               double       velZ,
                                               double       angVelX,
                                               double       angVelY,
                                               double       angVelZ,
                                               double       charge,
                                               const double orientation[9],
                                               NApiCore::ICustomPropertyDataApi_1_0* particlePropData,
                                               NApiCore::ICustomPropertyDataApi_1_0* simulationPropData,
                                               double&      calculatedForceX,
                                               double&      calculatedForceY,
                                               double&      calculatedForceZ,
                                               double&      calculatedTorqueX,
                                               double&      calculatedTorqueY,
                                               double&      calculatedTorqueZ);

			virtual unsigned int getNumberOfRequiredProperties(
                                     const NApi::EPluginPropertyCategory category);

            virtual bool getDetailsForProperty(
                             unsigned int                    propertyIndex,
                             NApi::EPluginPropertyCategory   category,
                             char                            name[NApi::CUSTOM_PROP_MAX_NAME_LENGTH],
                             NApi::EPluginPropertyDataTypes& dataType,
                             unsigned int&                   numberOfElements,
                             NApi::EPluginPropertyUnitTypes& unitType);

    private:

};

#endif
