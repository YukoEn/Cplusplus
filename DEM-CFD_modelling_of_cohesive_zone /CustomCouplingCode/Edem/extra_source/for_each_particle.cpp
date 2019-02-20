	const double* temperatureValue = particlePropData->getValue(TEMPERATURE_PROPERTY.c_str());
	const double* particleTypeValue=particlePropData->getValue(PROPERTY_TO_EXCHANGE1.c_str());
	const double* reactionDegreeValue=particlePropData->getValue(PROPERTY_TO_EXCHANGE2.c_str());
	const double* cokeSizeValue=particlePropData->getValue(PROPERTY_TO_EXCHANGE3.c_str());
	const double* deformValue=particlePropData->getValue(PROPERTY_TO_EXCHANGE4.c_str());
	const double* softViscosityValue=particlePropData->getValue(PROPERTY_TO_EXCHANGE5.c_str());

//***********************************************
	//Count the number of coke/ ore particles
	if (strcmp(type,"burden")==0 && posZ>2.4 && posZ<2.5) {
		cokeCount++;
	}

	if (strcmp(type,"burden2")==0 && posZ>2.4 && posZ<2.5) {
		oreCount++;
	}

//***********************************************
	//CREATE NEW PARTICLE IN DLL OR REMOVE IF NECESSARY
	//Check if particle is not yet in the DLL data structure. 
	//If so, generate new particle
	//Check if particle is to be removed. This code removes it from the DLL,
	//EDEM will remove it from its data structure the next time-step 
	//because of a huge force applied at the end of the function 

		// ****PARTICLE INSERTION****

		if ((DEMpartPtr[id]==0)&&(partData[0].myId!=id)&&(lastParticle<maxPart)) 
			//not in DLL yet, create new particle at last position of array
			//particles that change from DPM to DEM have already been handled in factory
			//writeOut("inserting particle",(float)*nId);
		{
			EDEMdebug=10;
			pNewData.volume=volume;
			pNewData.temperature=temperatureValue[0];
			pNewData.particleType=particleTypeValue[0];
			pNewData.reactionDegree=reactionDegreeValue[0];
			pNewData.cokeSize=cokeSizeValue[0];
			pNewData.softViscosity=softViscosityValue[0];
			(pNewData.pos)[0]=posX;(pNewData.pos)[1]=posY;(pNewData.pos)[2]=posZ;

			EDEMdebug=11;
			pNewData.cellId=startCellSearch;
			pNewData.myId=id;
			
			DEMpartPtr[id]=lastParticle+1;
			
			lastParticle++;
		}
		else if ((DEMpartPtr[id]==0)&&(partData[0].myId!=id)) 
				{writeOut("ERROR: Maximum number of particles exceeded");}
		
		// ****PARTICLE DELETE/REMOVE****
		if (removeCondition){ 
			
			EDEMdebug=12;
			//writeOut("removing particle ",(float)*nId);
			deleteDEMparticle(id);
			remove_FromEdem=true;//assures that particle will be removed from EDEM datastructure
		}
		
	// Reduce removing rate above Raceway

	if (cokeMNN >= 2){
	cokeRemovalMTime = false;}
	if (cokePNN >= 2){
	cokeRemovalPTime = false;}

	// Remove in Raceway
	if (cokeRemovalMTime) {

		// coke remove at every "cokeRemovalFreq"
		if (strcmp(type,"burden")==0 && posZ<0.275 && posZ>0.175) {
			if (posY<0.025 && posY>-0.025) {

				if ( (posX<-0.2 && posX>-0.4) ){
					deleteDEMparticle(id);
					remove_FromEdem=true;

					cokeMNN += 1;

					if (cokeMNN >= 2){
					cokeRemovalMTime = false;}

				}

			}
	
		}

	}


	if (cokeRemovalPTime) {

		// coke remove at every "cokeRemovalFreq" 
		if (strcmp(type,"burden")==0 && posZ<0.275 && posZ>0.175) {
			if (posY<0.025 && posY>-0.025) {

				if ( (posX>0.2 && posX<0.4) ){
					deleteDEMparticle(id);
					remove_FromEdem=true;

					cokePNN += 1;

					if (cokePNN >= 2){
					cokeRemovalPTime = false;}

				}

			}
	
		}

	}

		if (strcmp(type,"burden2")==0 && posZ<0.175 && posZ>0.125) {
			if (posY<0.025 && posY>-0.025) {
				if (posX<-0.2 || posX>0.2){
				deleteDEMparticle(id);
				remove_FromEdem=true;
				}
			}
		}
		
		if (strcmp(type,"burden2")==0 && (deformValue[0] > 0.5)) {
				deleteDEMparticle(id);
				remove_FromEdem=true;		
		}

		if (strcmp(type,"oreDef1")==0 && (temperatureValue[0] >= 1811.)) {
				deleteDEMparticle(id);
				remove_FromEdem=true;		
		}

		if (strcmp(type,"oreDef1")==0 && (deformValue[0] > 1.0)) {
				deleteDEMparticle(id);
				remove_FromEdem=true;		
		}
		
		//***********************************************

	//***********************************************
	//if coupling with FLUENT was performed the last time-step, get updated reaction degree
	if (updateCustomProps)
	{	
		double* reactionDegreeDelta = particlePropData->getDelta(PROPERTY_TO_EXCHANGE2.c_str());
		reactionDegreeDelta[0] =pData.reactionDegree-reactionDegreeValue[0];
							
		double* cokeSizeDelta = particlePropData->getDelta(PROPERTY_TO_EXCHANGE3.c_str());
		cokeSizeDelta[0] =pData.cokeSize-cokeSizeValue[0];

		double* softViscosityDelta = particlePropData->getDelta(PROPERTY_TO_EXCHANGE5.c_str());
		softViscosityDelta[0] =(pData.softViscosity)-softViscosityValue[0];

	}
	//***********************************************

	//***********************************************
	//if coupling with FLUENT will be performed the next time-step, 
	//calculate all field values for data exchange
	if (prepareForDataExchange)
	{	

			if (pData.myId!=id){
					writeOut("WARNING, INCONSITENT PARTICLE DATA IN ARRAY FOR ARRAY INDEX",
					DEMpartPtr[id]);
					writeOut("EDEM id:",id);
					writeOut("pData.myId:",pData.myId);
			}

			if (pData.myId=id){
			EDEMdebug=17;
			//update particle position and velocity
			pData.pos[0]=posX; pData.pos[1]=posY; pData.pos[2]=posZ;

			//update volume, particleType, reactionDegree;
			pData.volume=volume;
			pData.temperature=temperatureValue[0];
			pData.particleType=particleTypeValue[0];
			pData.reactionDegree=reactionDegreeValue[0];
			pData.cokeSize=cokeSizeValue[0];
			}

	}
	//***********************************************

    EDEMdebug=1;

	//***********************************************
	//check if to be removed from EDEM, if so-->apply huge force
	if (remove_FromEdem){
			//do this here at the end so the force value can not get to FLUENT!

				calculatedForceX=1.e+12;
				calculatedForceY=1.e+12;
				calculatedForceZ=1.e+12;

	}
	//***********************************************
