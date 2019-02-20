	double t_sim_FLUENTNEW = timestep_sim_FLUENT * 0.0005;

	double time_EDEMNEW = (time * 1.e+4) + 0.5;
	time_EDEMNEW = floor(time_EDEMNEW)/1.e+4;

	double timestep_EDEMNEW = (timestep * 1.e+5) + 0.5;
	timestep_EDEMNEW = floor(timestep_EDEMNEW)/1.e+5;

			//write particles that exited from buffer to external file
			ofstream outfile(outpFile,ios::out|ios::app); //open file for write, mode "append"
			outfile.write(buffer.c_str(),buffer.length());//append particles from buffer to file
			outfile.close();
			//reset buffer
			buffer="";
			//strcpy (buffer,nulstr);

			EDEMdebug=2;
			
			if((t_sim_FLUENTNEW-2*timestep_EDEMNEW*1.0000001<=time_EDEMNEW)
				&&(t_sim_FLUENTNEW-1*timestep_EDEMNEW*1.0000001>time_EDEMNEW)) 
			//2 EDEM time-steps before coupling step
			{
				writeOut("prepareForDataExchange, t ",time_EDEMNEW);
				prepareForDataExchange=true; 
			}
            
			if (iterSinceLastCouple==0)  //just returned from a coupling step
			{
				updateCustomProps=true;
				writeOut("Fluent time ",t_sim_FLUENTNEW);
				writeOut("setting updateCustomProps to true at t ",time_EDEMNEW);
				iterSinceLastCouple=1;

			}
			else updateCustomProps=false;
			
			EDEMdebug=3;

			if (t_sim_FLUENTNEW-1*timestep_EDEMNEW*1.0000001<=time_EDEMNEW) 
			//1 EDEM time-steps before coupling step
			{
				prepareForDataExchange=false;
				iterSinceLastCouple=0;	//reset EDEM duty cycle counter
				//writeOut("ededm will soon hand over to fluent, t ",time);
			}
			EDEMdebug=7;
