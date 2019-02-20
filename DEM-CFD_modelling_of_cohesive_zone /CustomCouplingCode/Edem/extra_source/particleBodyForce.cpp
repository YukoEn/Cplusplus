
const string CExchangeProperties::TEMPERATURE_PROPERTY = "Temperature";
const string CExchangeProperties::PROPERTY_TO_EXCHANGE1 = "particleType";
const string CExchangeProperties::PROPERTY_TO_EXCHANGE2 = "reactionDegree";
const string CExchangeProperties::PROPERTY_TO_EXCHANGE3 = "cokeSize";
const string CExchangeProperties::PROPERTY_TO_EXCHANGE4 = "deformation";
const string CExchangeProperties::PROPERTY_TO_EXCHANGE5 = "softViscosity";


CExchangeProperties::CExchangeProperties()
{
    ;
}

CExchangeProperties::~CExchangeProperties()
{
    ;
}

void CExchangeProperties::getPreferenceFileName(char prefFileName[NApi::FILE_PATH_MAX_LENGTH])
{
	prefFileName[0] = '\0';
}

bool CExchangeProperties::isThreadSafe()
{
    // thread safe
    return false;
}

bool CExchangeProperties::usesCustomProperties()
{
	return true;
}

bool CExchangeProperties::setup(NApiCore::IApiManager_1_0& apiManager,
                               const char                 prefFile[])
{
	return true;
}

bool CExchangeProperties::starting(NApiCore::IApiManager_1_0& apiManager)
{
	return true;
}

void CExchangeProperties::stopping(NApiCore::IApiManager_1_0& apiManager)
{
	;
}

unsigned int CExchangeProperties::getNumberOfRequiredProperties(
                                     const NApi::EPluginPropertyCategory category)
{
	if (eParticle == category)
	{
		return 6;
	}
	else
	{
		return 0;
	}
}

bool CExchangeProperties::getDetailsForProperty(
                             unsigned int                    propertyIndex,
                             NApi::EPluginPropertyCategory   category,
                             char                            name[NApi::CUSTOM_PROP_MAX_NAME_LENGTH],
                             NApi::EPluginPropertyDataTypes& dataType,
                             unsigned int&                   numberOfElements,
                             NApi::EPluginPropertyUnitTypes& unitType)
{
	if (eParticle == category && 0 == propertyIndex)
	{
		strcpy(name, TEMPERATURE_PROPERTY.c_str());
		dataType	= eDouble;
		numberOfElements = 1;
		unitType		= eTemperature;
		return true;
	}
	else if (eParticle == category && 1 == propertyIndex)
	{
		strcpy(name, PROPERTY_TO_EXCHANGE1.c_str());
		dataType	= eDouble;
		numberOfElements = 1;
		unitType		= eNone;
		return true;	
	}
	else if (eParticle == category && 2 == propertyIndex)
	{
		strcpy(name, PROPERTY_TO_EXCHANGE2.c_str());
		dataType	= eDouble;
		numberOfElements = 1;
		unitType		= eNone;
		return true;
	}
	else if (eParticle == category && 3 == propertyIndex)
	{
		strcpy(name, PROPERTY_TO_EXCHANGE3.c_str());
		dataType	= eDouble;
		numberOfElements = 1;
		unitType		= eNone;
		return true;
	}
	else if (eParticle == category && 4 == propertyIndex)
	{
		strcpy(name, PROPERTY_TO_EXCHANGE4.c_str());
		dataType	= eDouble;
		numberOfElements = 1;
		unitType		= eNone;
		return true;
	}
	else if (eParticle == category && 5 == propertyIndex)
	{
		strcpy(name, PROPERTY_TO_EXCHANGE5.c_str());
		dataType	= eDouble;
		numberOfElements = 1;
		unitType		= eNone;
		return true;
	}


	else
	{
		return false;
	} 
}


ECalculateResult CExchangeProperties::externalForce(
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
                                               double&      calculatedTorqueZ)
{
	bool remove_FromEdem=false;

		//***********************************************
		//once per iteration
		
		if ((time>act_time))
		{
			act_time=time;
			#include "extra_source\once_a_timestep.cpp"		
			cokeRemovalCount += 1;


			if (cokeRemovalCount == cokeRemovalFreq){
				cokeRemovalMTime = true;
				cokeRemovalPTime = true;
				cokeRemovalCount=0;
				cokeMNN=0;
				cokePNN=0;}		

		}

		//***********************************************
		//for each particle
        #include "extra_source\for_each_particle.cpp"
		//***********************************************

    return eSuccess;
}