void couplingInit(void)
{
	Domain *domain;  Thread *t;  cell_t c;
	
	#if !RP_HOST
		domain = Get_Domain(domain_ID);
		thread_loop_c(t,domain){
			nCells=0;
			begin_c_loop(c,t){
				nCells++;		   
			}end_c_loop(c,t)
			if (nCells>lastCell) lastCell=nCells;
			if (nCells>nCellsMax) Message("\nERROR: Please increase nCellsMax for coupling to at least %d and re-compile\n",nCells);
		}	
	#endif
}


void initProcessParticleData()
{
	Domain *domain;  Thread *t;  cell_t c;
	int i;
	
		domain = Get_Domain(domain_ID);
	
		setFLUENTDebug(8);
		thread_loop_c(t,domain){
			   begin_c_loop_int(c,t){  

				   setFLUENTDebug(9);
				   C_UDMI(c,t,13)=0.;
				   C_UDMI(c,t,14)=0.;
				   C_UDMI(c,t,15)=0.;
				   C_UDMI(c,t,16)=0.;    // +ADD+
				   C_UDMI(c,t,18)=0.;    // +ADD+
				   C_UDMI(c,t,24)=0.;    // +ADD+
			   	   C_UDMI(c,t,25)=0.;    // +ADD+
			   	   C_UDMI(c,t,26)=0.;    // +ADD+
				   cellData[(int)c][0]=0.;//set volume fraction to zero
				   cellData[(int)c][1]=0.;//set custom property to zero
				   
			   }end_c_loop_int(c,t);
		}
}

void processParticleData()
{
	real  pPos[3]={0.};
	Domain *domain;  Thread *t;  cell_t c,ci;
	int cii,i;

	domain = Get_Domain(domain_ID);
	
	thread_loop_c(t,domain){
		   setFLUENTDebug(10);
		   for(i=0;i<=lastParticle;i++) //loop all particles
		   {
			   
				setFLUENTDebug(99);
				//fetch particle position
				//find cell that the particle is in; 
				//use cell particle was in lately as starting position
   				ci=findCellFluent(i,t,nCells);
				cii=((int)ci);

				setFLUENTDebug(100);
				//remember where the particle was found
				partData[i].cellId=(int)cii;

				//particle center was located;
				if (cii>0)
				{
				//add solids fraction contribution of particle
				C_UDMI(ci,t,13)+=(double)(((partData[i]).volume)/C_VOLUME(ci,t));
				C_UDMI(ci,t,26)+=(double)(((partData[i]).volume)*((partData[i]).temperature));

				if(partData[i].particleType==0.0 || partData[i].particleType==2.0) 
				{
					C_UDMI(ci,t,14)+=(double)(((partData[i]).volume)/C_VOLUME(ci,t));
					C_UDMI(ci,t,16)+=(double)(((partData[i]).volume)*((partData[i]).reactionDegree));
					C_UDMI(ci,t,24)+=1.;
				}
				if(partData[i].particleType==1.0) 
				{
					C_UDMI(ci,t,15)+=(double)(((partData[i]).volume)/C_VOLUME(ci,t));
                    C_UDMI(ci,t,18)+=(double)(((partData[i]).volume)*((partData[i]).cokeSize));
					C_UDMI(ci,t,25)+=1.;
				}


			   }
		   }
	}
}


//Modified by Yuko

void finishProcessParticleData()		
{
	Domain *domain;  Thread *t;  cell_t c,ci; int i,cii;
	double pVolSumF=0., xSrcSumF=0.,ySrcSumF=0,zSrcSumF=0.;//sources set in fluent
	int k=0;
	int actTimeStep=0;
	int kk;
	int timeStep=N_TIME;
	int timeStepValue;
	
	kk=floor((timeStep-4000)/6000.);
	timeStepValue = (6000  * kk) + 4002;
	//CX_Message( "CFD kk_num:%d\n", kk ) ;

		domain = Get_Domain(domain_ID);
		thread_loop_c(t,domain){
			   setFLUENTDebug(11);
			   begin_c_loop_int(c,t){   
				   setFLUENTDebug(90);

				   if(fabs(C_UDMI(c,t,13))>0.00001)
				   {   
					   C_UDMI(c,t,26)=C_UDMI(c,t,26)/(C_UDMI(c,t,13)*C_VOLUME(c,t));
				   }
				   else 
				   {   
					   C_UDMI(c,t,26)=0.;
				   }

				   //finish calculation for the averaged reaction degree of 
				   //all the particles in a cell
				   if(fabs(C_UDMI(c,t,14))>0.00001)
				   {   
					   C_UDMI(c,t,16)=C_UDMI(c,t,16)/(C_UDMI(c,t,14)*C_VOLUME(c,t));
				   }
				   else 
				   {   
					   C_UDMI(c,t,16)=0.;
				   }

				   if(fabs(C_UDMI(c,t,15))>0.00001)
				   {   
					   C_UDMI(c,t,18)=C_UDMI(c,t,18)/(C_UDMI(c,t,15)*C_VOLUME(c,t));
				   }
				   else 
				   {   
					   C_UDMI(c,t,18)=0.;
				   }

				   if(C_UDMI(c,t,13)>0.98){
				   	C_UDMI(c,t,13)=0.98;
				   }

				   if(C_UDMI(c,t,15)>0.98){
				   	C_UDMI(c,t,15)=0.98;
				   }

				   cellData[(int)c][0]=C_UDMI(c,t,13);//set volume fraction
				   cellData[(int)c][1]=C_UDMI(c,t,14);//set custom property

			   }end_c_loop_int(c,t);

		}


	thread_loop_c(t,domain){

		   for(i=0;i<=lastParticle;i++) //loop all particles
		   {

			  //fetch particle position
			  //find cell that the particle is in;
			  //use cell particle was in lately as starting position
   			  ci=findCellFluent(i,t,nCells);
			  cii=((int)ci);

			   //remember where the particle was found
			   partData[i].cellId=(int)cii;

			   //particle center was located;
			   if (cii>-1)
			   {
				//add solids fraction contribution of particle

				if(partData[i].particleType==0.0 || partData[i].particleType==2.0) 
				{	(partData[i]).reactionDegree += C_UDMI(ci,t,17); 
					
					if (timeStep > timeStepValue && timeStep < (timeStepValue+2)){				
					(partData[i]).softViscosity = C_UDMI(ci,t,27);
						if (timeStep > actTimeStep){
						actTimeStep=timeStep;
						CX_Message( "softViscUpdate:%d\n", timeStep );				
						}
					}
				}
				
				if(partData[i].particleType==1.0) (partData[i]).cokeSize += C_UDMI(ci,t,19);			

			   }
		   }
	}

		Message("processing particle data end\n");
}
