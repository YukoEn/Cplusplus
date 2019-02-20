//************************************************
// DATA THAT IS SHARED BETWEEN EDEM AND FLUENT
//************************************************
#pragma data_seg("SHARED") 

    //debugging state
	int EDEMdebug=-1;
	int FLUENTdebug=-1;

	//number of coupled iterations
	int iterCount=0; 

	struct particleStruct{
		double pos[3];
		double volume;
		double temperature;
		double particleType;
		double reactionDegree;
		double cokeSize;
		double softViscosity;
		int cellId;		//FLUENT cell where particle is located
		int myId;		//ID of the particle (*nId in EDEM)
	};
	
	//contains mesh-based solids fraction and averaged customPropetry
	//0:solids fr	1:averaged customPropetry
	EXPORT_MACRO double cellData[nCellsMax][2]={0.};  
	
	//contains particle-based information
	EXPORT_MACRO particleStruct partData[maxPart]={0}; 

	EXPORT_MACRO int lastParticle=-1; 
	//-1=no particle inside the DLL, 10 particle in the domain: lastparticle=9

	//"pointer array" to adress particles from EDEM because *nID can exceed maxPart
	//mod operation could lead to problems because low ID particles do not necessarily
	//leave the domain first
	//contains indices of partDataArray
	static int DEMpartPtr[maxParticleID]={0};  //for access from EDEM, *nId->partData index

	//USED FOR PARALLEL FLUENT COMPUTATION
	EXPORT_MACRO int actParticle=-1; //debugging
	EXPORT_MACRO int lastCell=-1;
	EXPORT_MACRO double t_sim_FLUENT=0.; //total time simulated in FLUENT
	EXPORT_MACRO int timestep_sim_FLUENT=0; //total timestep simulated in FLUENT
	
#pragma data_seg() 
#pragma comment(linker, "/section:SHARED,RWS")
//**************  END SHARED DATA ***************

//************************************************
// DATA THAT IS EDEM INTERNAL
//************************************************
bool prepareForDataExchange=false;
bool updateCustomProps=false;
static int iterSinceLastCouple=0;//EDEM duty cycle counter
static double act_time=0;
static int actBuff=0;
char *particleTypes[] =particleTypesDef;

// Charging
int cokeNumberCreated=0;
int oreNumberCreated=0;
int oreCount=0;
int cokeCount=0;
int cokeCountPre=0;
int oreCountPre=0;
int ICokeFlag=0; //0-ore creation, 1-coke creation
int onceRestart=0;

// Coke removal, 4#/0.1s
bool cokeRemovalMTime=false;
bool cokeRemovalPTime=false;
int cokeRemovalCount=0;
int cokeRemovalFreq=1000;
int cokeMNN=0;
int cokePNN=0;

//***********************************************
//  VARIABLES FOR WRITING OUT PARTICLE DATA FOR
//  PARTICLES THAT EXITED FROM COMP. DOMAIN
string buffer;//char buffer[100000]="";
char tmp[100000]; //string tmp; // 
char nulstr[]="";
//***********************************************