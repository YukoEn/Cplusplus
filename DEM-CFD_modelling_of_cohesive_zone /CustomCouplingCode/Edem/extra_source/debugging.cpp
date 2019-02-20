	FILE* outputfile;
	SYSTEMTIME st1,st2;

	EXPORT_MACRO void writeOut(char* msg, double dat)
	{
		GetSystemTime (&st1);
		outputfile=fopen(DEBUGFILE,"a");
		fprintf(outputfile,"%d.%d @ %d:%d:%d:%d | " ,st1.wDay,st1.wMonth, st1.wHour,st1.wMinute,
		st1.wSecond,st1.wMilliseconds);
		fprintf(outputfile,msg);
		fprintf(outputfile," %20.13f\n",(double)(dat));
		fclose(outputfile);
	}

	inline void forceWriteOut(const char* msg)
	{
		outputfile=fopen(DEBUGFILE,"a");
		fprintf(outputfile,"forcing writeOut: ");
		fprintf(outputfile,msg);
		fprintf(outputfile,"\n");
		fclose(outputfile);
	}

	inline void writeOut(const char* msg)
	{
		GetSystemTime (&st1);
		outputfile=fopen(DEBUGFILE,"a");
		fprintf(outputfile,"%d:%d:%d:%d | " ,st1.wHour,st1.wMinute,st1.wSecond,st1.wMilliseconds);
		fprintf(outputfile,msg);
		fprintf(outputfile,"\n");
		fclose(outputfile);
	}

   EXPORT_MACRO int showDebug(char ident)
   {
		return EDEMdebug;
   }
	EXPORT_MACRO int showDebugF(char ident)
   {
		 return FLUENTdebug;
   }

   EXPORT_MACRO void setFLUENTDebug(int deb)
   {
	FLUENTdebug=deb;
	return;
   }

   EXPORT_MACRO void debugOutputData(void)
   {
	   int i=0,j=0;
		outputfile=fopen(DEBUGFILE,"a");

		fprintf(outputfile,"-----------------PRINTING PARTICLE DATA-----------------\n");
		for(i=0;i<lastParticle;i++){
			fprintf(outputfile,
			"particle id=%d; pos in array=%d, volume=%f, x=%f, y=%f, z=%f, particletype=%f, reactionrate=%f\n",
			(int)partData[i].myId,i,(float)partData[i].volume,(float)partData[i].pos[0],
			(float)partData[i].pos[1],(float)partData[i].pos[2],(float)partData[i].particleType,
			(float)partData[i].reactionDegree);
		}
		
		fprintf(outputfile,"-----------------PRINTING CELL DATA -------------\n");
		for(i=0;i<lastCell+1;i++){
				fprintf(outputfile,"cell=%d: alpha=%f, customPropAveraged=%f\n",(int)i,
				(float)cellData[i][0],(float)cellData[i][1]);
		}
		fprintf(outputfile,"-----------------PRINTING EDEM PTR LIST-----------------\n");
		for(i=0;i<maxParticleID;i++){ 
			fprintf(outputfile,"partPtr[%d]=%d; \n",(int)i,(int)DEMpartPtr[i]);
		}

		fclose(outputfile);
   }