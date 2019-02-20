#include "stdafx.h"
#include "CExchangeProperties.h"
#include "CFactoryCharge.h"
#include "Helpers.h"

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include <locale.h>
#include <tchar.h>
#include <time.h>
#include <string>
#include <math.h>
#include <iostream>
#include <sstream>

using namespace std;
using namespace NApi;
using namespace NApiCore;
using namespace NApiPbf;
using namespace NApiFactory;

//***********************************************
//PREFERENCES
#include "extra_headers\coupling_preferences.h"
#include "extra_headers\expert_settings.h"
//***********************************************

//***********************************************
//INTER_PROCESS COMMUNICATION
#include "extra_headers\process_communication.h"
#include "extra_source\debugging.cpp"
//***********************************************

//***********************************************
//FUNCTIONS TO HANDLE THE DATA COUPLING
#include "extra_source\data_handling.cpp"
//***********************************************

//***********************************************
//EDEM CUSTOM FACTORY
#include "extra_source\customFactory.cpp"
//***********************************************

//***********************************************
//EDEM PARTICLE BODY FORCE
#include "extra_source\particleBodyForce.cpp"
//***********************************************
