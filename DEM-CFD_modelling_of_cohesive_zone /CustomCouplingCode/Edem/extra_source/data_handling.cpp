 #define pData (partData[(DEMpartPtr[id])])
 #define pLastData (partData[lastParticle])
 #define pNewData (partData[lastParticle+1])

   void deleteLastParticle()
   {
			//erase values for lastParticle
			(pLastData.volume)=0.;
			(pLastData.temperature)=0.;
			(pLastData.particleType)=0.;
			(pLastData.reactionDegree)=0.;
			(pLastData.cokeSize)=0.;
			(pLastData.softViscosity)=0.;
			(pLastData.cellId)=0;
			(pLastData.myId)=0;
			for (int i=0;i<3;i++){
				(pLastData.pos)[i]=0.;
			}
			lastParticle--;
   }

   void copyDataFromLastParticleToThisParticle(int index)
   {
		partData[index].volume=pLastData.volume;
		partData[index].temperature=pLastData.temperature;
		partData[index].particleType=(pLastData.particleType);
		partData[index].reactionDegree=(pLastData.reactionDegree);
		partData[index].cokeSize=(pLastData.cokeSize);
		partData[index].softViscosity=(pLastData.softViscosity);
		partData[index].cellId=(pLastData.cellId);
		partData[index].myId=(pLastData.myId);
		for (int i=0;i<3;i++){
			(partData[index].pos)[i]=(pLastData.pos)[i];
		}
   }

   void deleteDEMparticle(int p_id)
   {
			//copy particle data from last particle to deleted particle position
		    copyDataFromLastParticleToThisParticle(DEMpartPtr[p_id]);

			EDEMdebug=13;

			//rearrange pointers
			DEMpartPtr[pLastData.myId]=DEMpartPtr[p_id];
			DEMpartPtr[p_id]=0;

			//erase values for lastParticle
			deleteLastParticle();
   }