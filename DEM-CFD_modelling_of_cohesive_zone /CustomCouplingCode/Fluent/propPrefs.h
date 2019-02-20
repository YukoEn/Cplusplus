//maxmimum number of EDEM particles to be handled, must match settings in DLL
#define maxPart 500000

//maximum particle ID in EDEM, must match settings in DLL
#define maxParticleID 2000000

//max. number of cells in FLUENT  to be handled, must match settings in DLL
#define nCellsMax 200000

//definitions
#if (!PARALLEL)
	#define myid 0
#endif

#define domain_ID 1
#define IMPORT_MACRO __declspec(dllimport)
