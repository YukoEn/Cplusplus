#include "udf.h" 
#include "dpm.h"   
#include "math.h"
#include "stdlib.h" 
#include "surf.h" 
#include "turb.h"
#include "stdio.h"  

#include "sg_mphase.h"
#include "mem.h"
#include "sg.h"
#include "sg_udms.h"


DEFINE_ADJUST(CFD_reac_Adjust,d)
{
	Thread *t;
	cell_t c;

	Thread *pri_th, *sec_th;

	int i;
	int timestep;
    int MaxSpeciesPrim=3,IndexSpecies_co2=0;
	int IndexSpecies_co=1,IndexSpecies_n2=2;

	double Rg=0.08205746;		    // [atm m3/K kmol]
    double Rg_cal=1.9858775;		// [cal/K mol]
	double timeStepSize;
    
	// particle properties change here 
	double d_coke=0.03;				// coke diameter [m]
	double d_ore=0.02,d_int,RD=0.;	// ore diameter [m]
	double cokeDensity=1000.;		// coke density [kg/m3]
    double oreDensity=4000.;			//ore density [kg/m3]
	double Gcoke,Gore;
	double rate_CO2,rate_CO2_IRw;
	double vrate_CO2,vrate_CO2_IRw;
	double mrate_C,mrate_FeO;
	double dCokeSize;
	double cokeNumDensity,oreNumDensity;
	// gas diffusion
	double diff_coef_co2,diff_eff_co2;
	double eps_c=0.45,xi_c=0.147;
	double diff_coef_co;
	double k_boud,k_red,Ef,m;
    
	double massC_coke, massC_ore;
	double Sc_GasC, Rep_GasC, V_Gas;
	double sum=0.,mw[3],xco2Tot,xcoTot;
	double xcoInco_co2,xco2Inco_co2,xco2Inco_co2E,xco2Inco_co2wE;
	double T_pri, T_solid;		// gas and solid temperature  
	double p_co,p_co2,p_co_co2,p_co2E,p_co2wE;
	double Kco_co2,Kw_co_co2;
	double RDA=0.34,dRDA;
	
	timestep = RP_Get_Integer("time-step");
	timeStepSize = CURRENT_TIMESTEP;

	thread_loop_c(t,d)
	{ 	
	pri_th = THREAD_SUB_THREAD(t,0);    // fluid phase	

		begin_c_loop(c,t)
		{

			if (timestep > 10) {
				d_coke = C_UDMI(c,t,18);
				RDA = C_UDMI(c,t,16);}    

		RD = (3.*RDA -1.)/2.;
		d_int = (pow((1. - RD), 1./3.)) * d_ore;
        		
		if (C_VOLUME(c,t) > 0.) {
		oreNumDensity=C_UDMI(c,t,24)/C_VOLUME(c,t);  //[#/m3]
		cokeNumDensity=C_UDMI(c,t,25)/C_VOLUME(c,t); //[#/m3]
		}

	///////////////////////
		T_pri=C_T(c, pri_th);
		T_solid=T_pri;

			if (timestep > 10) {
				T_solid=C_UDMI(c,t,26);}

		// Diffusion coefficient of CO2 [m2/s]     
		diff_coef_co2 = 2.236e-6 * (pow(T_pri, 1.78))/3600.;       
		diff_eff_co2 = diff_coef_co2 * eps_c * xi_c;
		
		// Diffusion coefficient of CO [m2/s]
		diff_coef_co = 2.592e-6 * (pow(T_pri, 2.0))/3600.; // T_pri > 848. K 	

		// Reaction constants
		k_boud = 2.988e+13 * exp(-80000./ Rg_cal/ T_solid) / 3600.;   // [m3/kg s]
		k_red = exp(12.70 - 29.91*1.e+3/ Rg_cal/ T_solid) * 0.01;     //[m/s]
	
		// Reaction Effectiveness Factor
		m = d_coke/ 2. * sqrt(k_boud * cokeDensity / diff_eff_co2);
		Ef = 3. * (m * (cosh(m) / sinh(m)) -1.) /m /m;
	
		V_Gas=C_U(c, pri_th)*C_U(c, pri_th) + C_V(c, pri_th)*C_V(c, pri_th) 
			  + C_W(c, pri_th)*C_W(c, pri_th);
		V_Gas=pow(V_Gas, 0.5);
		Rep_GasC=(C_R(c, pri_th)/C_MU_L(c, pri_th))*V_Gas;	
		Sc_GasC=C_MU_L(c, pri_th)/C_R(c, pri_th);

		massC_coke=2.0 + 0.55 * (pow((Rep_GasC*d_coke), 0.5))
			       *(pow((Sc_GasC/diff_coef_co2), 1./3.));
		massC_coke=(massC_coke)*(diff_coef_co2)/(d_coke);

		massC_ore=2.0 + 0.55 * (pow((Rep_GasC*d_ore), 0.5))
			      *(pow((Sc_GasC/diff_coef_co), 1./3.));
		massC_ore=(massC_ore)*(diff_coef_co)/(d_ore);

		// Species mole weight
		mw[0]=44.00995;
		mw[1]=28.01055;
		mw[2]=28.0134;
		sum=0.;

		for (i=0; i < MaxSpeciesPrim; i++)
		{
			sum += (C_YI(c, pri_th, i)/ mw[i]);
		} 

		xco2Tot = C_YI(c, pri_th, IndexSpecies_co2)/ mw[IndexSpecies_co2] / sum;
		xcoTot = C_YI(c, pri_th, IndexSpecies_co)/ mw[IndexSpecies_co] / sum;

		// Assume total pressure = 1 atm
		p_co = xcoTot;
		p_co2 = xco2Tot;
		p_co_co2 =p_co + p_co2;
		xcoInco_co2 = xcoTot/(xcoTot + xco2Tot);
		xco2Inco_co2 = 1.-xcoInco_co2;

		//Equilibrium constants
		Kco_co2 = pow(10.,((8809./T_pri)-9.093)); //co-co2
		Kw_co_co2 = exp(-3.127 + (2879./ T_pri)); //w-co-co2

		// Constrains by coke gasification
	    xco2Inco_co2E = (pow(xcoInco_co2, 2.))* Kco_co2 * p_co_co2;
		p_co2E =(xcoTot + xco2Tot)* xco2Inco_co2E;

		// Constrains by indirect(w-Fe) reduction
		xco2Inco_co2wE = xcoInco_co2* Kw_co_co2;
		p_co2wE = (xcoTot + xco2Tot)*xco2Inco_co2wE;
 
		//COKE GASIFICATION
		// CO2 consumption flux [kmol/m2 s]
		if (d_coke > 0.) {		
			rate_CO2 = (1.0 * xco2Tot/ Rg/ T_pri) / 
			((massC_coke) + (6./ Ef/ d_coke/ cokeDensity/ k_boud)); }
		else {
			rate_CO2 = 0.;}
        // avoid denominator =0.
		
		// Mass dissolution flux of C [kg/m2 s]	
		mrate_C = 12. * rate_CO2;

		// Linear dissolution rate of C [m/s]
		Gcoke = (-1.)*(2./cokeDensity*mrate_C);

		//INDIRECT REDUCTION	
		// CO2 production flux [kmol/m2 s]
		rate_CO2_IRw = (1.0 * ((Kw_co_co2 * xcoTot) - xco2Tot) / Rg/ T_pri) / 
		((massC_ore) + (1./ pow((1. - RD), 2./3.) / k_red / (1. + (1./ Kw_co_co2)))); 

		// Mass dissolution flux of FeO [kg/m2 s]
		mrate_FeO = 71.85 * rate_CO2_IRw;

		// Linear dissolution rate of FeO [m/s]
		Gore = (-1.)*(2./oreDensity*mrate_FeO);

		rate_CO2 = (-1.)*cokeDensity*Gcoke/24.;
		rate_CO2_IRw = (-1.)*oreDensity*Gore/(2. * 71.85);

		if (p_co2 < p_co2E)
		{
			Gcoke =0.;
			rate_CO2 = 0.;
		}

		if (p_co2 > p_co2wE)
		{
			Gore =0.;
			rate_CO2_IRw = 0.;
		}

		if (d_coke < 0.)
		{
			Gcoke =0.;
			rate_CO2 = 0.;
		}

		if (RD < 0.001) {
			Gore =0.;
			rate_CO2_IRw = 0.;
		}

		if (RD > 1.) {
			Gore =0.;
			rate_CO2_IRw = 0.;
		}

		if (Gore > 0.) {
			Gore =0.;
			rate_CO2_IRw = 0.;
		}

		dCokeSize = Gcoke * timeStepSize;		// negative
	    d_int += Gore * timeStepSize;			
		
		if (Gore < 0.) {
			dRDA = 1. - (pow((d_int/d_ore), 3.)); //RDnew
			dRDA =(1. + 2.*dRDA)/3.;		// RDAnew
			dRDA -=RDA;}
		// positive
		else {
			dRDA = 0.;}

		vrate_CO2 = rate_CO2 * M_PI * d_coke * d_coke * cokeNumDensity; //[kmol/m3 s]		
		vrate_CO2_IRw = rate_CO2_IRw * M_PI * d_ore * d_ore * oreNumDensity; // [kmol/m3 s]
		
		C_UDMI(c,t,17)=dRDA;
		C_UDMI(c,t,19)=dCokeSize;
		C_UDMI(c,t,20)=vrate_CO2;
		C_UDMI(c,t,21)=vrate_CO2_IRw;
		
		}
		end_c_loop(c,t)

	}

}


DEFINE_SOURCE(mass_source, c, t, dS, eqn)
{	
	double MSc;
	MSc = (15.9994)*C_UDMI(c,t,21) + (12.01115)*C_UDMI(c,t,20);

	return MSc;
}

DEFINE_SOURCE(heat_source, c, t, dS, eqn)
{	
	double MSc;
	double HSc;

	MSc = (15.9994)*C_UDMI(c,t,21) + (12.01115)*C_UDMI(c,t,20);  
	// source due to mass increase [W/m3]	
	HSc = C_CP(c,t)*C_T(c,t)*MSc + (17.5e+6)*C_UDMI(c,t,21) + (-156.5e+6)*C_UDMI(c,t,20); 
	// interphase heat source [W/m3]

	dS[eqn] = C_CP(c,t)*MSc;

	return HSc;
}

DEFINE_SOURCE(spCO_source, c, t, dS, eqn)
{	
	double spCOSc;

	spCOSc = (-28.01055)*C_UDMI(c,t,21) + (56.0211)*C_UDMI(c,t,20);  // [kg/m3 s]	

	return spCOSc;
}

DEFINE_SOURCE(spCO2_source, c, t, dS, eqn)
{	
	double spCO2Sc;

	spCO2Sc = (44.00995)*C_UDMI(c,t,21) + (-44.00995)*C_UDMI(c,t,20);  // [kg/m3 s]	

	return spCO2Sc;
}