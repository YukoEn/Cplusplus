	setFLUENTDebug(1);
		
	//process particle data
	initProcessParticleData();
	processParticleData();
	finishProcessParticleData();

	t_sim_FLUENT=CURRENT_TIME;
	timestep_sim_FLUENT=N_TIME;
	