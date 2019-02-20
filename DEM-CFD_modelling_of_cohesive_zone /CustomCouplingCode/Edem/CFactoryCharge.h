#include "stdafx.h"

#if !defined(cfactorycharge_h)
#define cfactorycharge_h

#include <string>

#include "IPluginParticleFactoryV2_0_0.h"

class CFactoryCharge : public NApiFactory::IPluginParticleFactoryV2_0_0
{
        public:	

			static const std::string TEMPERATURE_PROPERTY;
			static const std::string PROPERTY_TO_EXCHANGE1;
			static const std::string PROPERTY_TO_EXCHANGE2;
			static const std::string PROPERTY_TO_EXCHANGE3;
			static const std::string PROPERTY_TO_EXCHANGE4;
			static const std::string PROPERTY_TO_EXCHANGE5;
			static const std::string PROPERTY_TO_EXCHANGE6;

			/**
             * Constructor, does nothing
             */
            CFactoryCharge();
            /**
             * Destructor, does nothing
             */
            virtual ~CFactoryCharge();

			virtual void getPreferenceFileName(char prefFileName[NApi::FILE_PATH_MAX_LENGTH]);

            virtual bool usesCustomProperties();

            virtual bool setup(NApiCore::IApiManager_1_0& apiManager,
                               const char                 prefFile[]);

            virtual bool starting(NApiCore::IApiManager_1_0& apiManager);

            virtual void stopping(NApiCore::IApiManager_1_0& apiManager);

            virtual NApi::ECalculateResult createParticle(
                                               double  time,
                                               bool&   particleCreated,
                                               bool&   additionalParticleRequired,
                                               char    type[NApi::API_BASIC_STRING_LENGTH],
                                               double& scale,
                                               double& posX,
                                               double& posY,
                                               double& posZ,
                                               double& velX,
                                               double& velY,
                                               double& velZ,
                                               double& angVelX,
                                               double& angVelY,
                                               double& angVelZ,
                                               double  orientation[9],
                                               NApiCore::ICustomPropertyDataApi_1_0* propData);

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
