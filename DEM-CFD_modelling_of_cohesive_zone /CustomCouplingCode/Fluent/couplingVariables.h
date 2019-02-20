//oooooooooooooooooooooooooooooooooooooooo
// STRUCT DEFINITIONS
//oooooooooooooooooooooooooooooooooooooooo
	typedef struct particleStruct{		
		double pos[3];
		double volume;
		double temperature;
		double particleType;
		double reactionDegree;
		double cokeSize;
		double softViscosity;
		int cellId;		//FLUENT cell where particle is located
		int myId;		//ID of the particle (*nId in EDEM)
	}particleStruct;
//oooooooooooooooooooo  END OF STRUCT DEF
//oooooooooooooooooooooooooooooooooooooooo
// VARIABLES
static int nCells=0; //number of cells in FLUENT
static int itersBefTs=0;
static int prnt=1;
static int timeStp=0;

//oooooooooooooooooooooooooooooooooooooooo
// IMPORT DLL MACROS FOR FUNCTIONS AND VARS
//oooooooooooooooooooooooooooooooooooooooo
IMPORT_MACRO void setFLUENTDebug(int);
IMPORT_MACRO void writeOut(const char*, double);
IMPORT_MACRO double t_sim_FLUENT;
IMPORT_MACRO int timestep_sim_FLUENT;

//contains particle-based information
IMPORT_MACRO particleStruct partData[maxPart]; 
IMPORT_MACRO int lastParticle; //indicates end of particleData array

//contains mesh-based solids fraction and averaged customPropetry
//0:solids fr	1:averaged customPropetry
IMPORT_MACRO double cellData[nCellsMax][2];
IMPORT_MACRO int lastCell;
//oooooooooooooooooooo  END IMPROT DLL MACROS
