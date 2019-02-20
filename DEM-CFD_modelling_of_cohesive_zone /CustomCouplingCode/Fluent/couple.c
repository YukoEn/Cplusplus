#include "udf.h" 
#include "dpm.h"   
#include "math.h"
#include "stdlib.h" 
#include "surf.h" 
#include "turb.h"
#include "stdio.h"  
#include "sg_mphase.h"
#include "mem.h"
#include "sg.h"
#include "sg_udms.h"

//oooooooooooooooooooooooooooooooooooooooo 
// INCLUDE PROPERTIES AND PREFERENCES
//oooooooooooooooooooooooooooooooooooooooo
#include "propPrefs.h"
//oooooooooooooooooooooooooooooooooooooooo
          
//oooooooooooooooooooooooooooooooooooooooo
// INCLUDE VARIABLE DEFINITIONS FOR COUPLING
//oooooooooooooooooooooooooooooooooooooooo 
#include "couplingVariables.h"
//oooooooooooooooooooooooooooooooooooooooo
                       
//oooooooooooooooooooooooooooooooooooooooo
// ALGORITHM TO DETERMINE FLUENT CELL IN WHICH A PARTICLE IS
	#include "cellFind.h" 
//#endif
//oooooooooooooooooooooooooooooooooooooooo
   
//oooooooooooooooooooooooooooooooooooooooo  
// DEM-CFD  COUPLING CODE 
//oooooooooooooooooooooooooooooooooooooooo
#include "DEM_CFD_coupling.h"   

DEFINE_INIT(EDEM_couplingInit,d)
{
	couplingInit();	  
}   

DEFINE_EXECUTE_ON_LOADING(EDEM_couplingInit_onLoad,libudf)
{
	couplingInit();
}

// MAIN  COUPLING CODE
DEFINE_EXECUTE_AT_END(EDEMcoupling_atEnd)
{
	#include "mainCoupling.h"
} 
//oooooooo END OF DEM-CFD COUPLING CODE 