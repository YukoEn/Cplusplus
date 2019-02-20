        //settings for removing particles; 
		//PARTICLES MUST BE REMOVED BY THIS CONDITION, NOT BY LEAVING THE DOMAIN IN EDEM!!
		#define removeCondition (posZ<-0.06) 
		#define outpFile "c:\\particle_statistics.txt" //write the deleted particles		

        //maxmimum number of EDEM particles to be handled
		#define maxPart 500000

        //maximum particle ID in EDEM
		#define maxParticleID 2000000

		//max. number of cells in FLUENT  to be handled
		#define nCellsMax 200000

		//debugging file
		#define DEBUGFILE "c:\\debug_edem.txt"

        //number of different particle types (e.g. coke, burden)
	    #define nParticletypes 2
        #define particleTypesDef {"burden2","burden"}
