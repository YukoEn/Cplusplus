/*-------------------------------------------
   ChemApp - FLUENT: IJPellet-Reference case
   18 Nov 2011				             
-------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "udf.h"
#include "cacint.h"
#include "math.h"

/* pressure */
#define PRESSURE 1

DEFINE_ON_DEMAND( ca_init )
{
	LI   i, j;
	LI   noerr, unitno;
	LI   num_pc, num_sc, num_p;
	LI   numcon;
	LI   version;

	char name_sc[ TQSTRLEN ];
	char name_p[ TQSTRLEN ], name_pc[ TQSTRLEN ];
	char name_m[ TQSTRLEN ];

	CX_Message( "\n start: ca_init \n\n" );

	/* ChemApp initialization */
	tqini( &noerr );
	tqvers( &version, &noerr );	
	CX_Message("\nChemApp version is: %d\n\n", version);
	
	tqgio( "file", &unitno, &noerr );
	
	/* unit */
	tqcsu( "Pressure", "atm", &noerr );
	tqcsu( "Temperature", "C", &noerr );
	tqcsu( "Energy", "J", &noerr );
	tqcsu( "Amount", "kg", &noerr );
	tqcsu( "Volume", "m3", &noerr );

	//tqopnt( "Cu2SSGTE.cst", unitno, &noerr );
	tqopnt( "EquiIJPelletRefN.cst", unitno, &noerr );	
	tqrcst( &noerr );
	tqclos( unitno, &noerr );

	/* print the thermochemical data */
	tqnosc( &num_sc, &noerr );
	CX_Message( "Number of System Component: %d\n", num_sc );

	for( i = 1; i <= num_sc; i++) {
		tqgnsc( i, name_sc, &noerr );
		CX_Message( "%d %s\n", i, name_sc );
	}

	tqnop( &num_p, &noerr );

	for( i = 1; i <= num_p; i++ ) {

		tqnopc( i, &num_pc, &noerr );
		tqgnp( i, name_p, &noerr );
		tqmodl( i, name_m, &noerr );

		CX_Message("Phase %d %s %s\n", i, name_p, name_m );

		for( j = 1; j <= num_pc; j++ ) {

			tqgnpc( i, j, name_pc, &noerr );		
			CX_Message( "ID:%d %s\n", j, name_pc );

		}

		CX_Message( "\n", j, name_pc );
	}
}


DEFINE_EXECUTE_AT_END( exec_ca ) 
{

	Domain* d;
	Thread* t;
	cell_t c;
	DB x[ ND_ND ];

	LI i, j, k;
	LI noerr;
	LI numcon;
	LI timestep;

	LI	 islite;
	LI   lint;

	LI  kk;
	LI	timeStepValue;

	DB   darray2[2];
	DB   d1;
	DB   vals[2];

	DB	equi_slag_MgO, equi_slag_FeO, equi_slag_SiO2, equi_slag_Al2O3, 
		equi_slag_Fe2O3;
	DB  equi_AMON_FeO, equi_AMON_Fe2O3;
	DB  equi_Olivine;
	DB  equi_Fe_bcc, equi_Fe_fcc;

	DB	c_Fe, c_Si, c_Al, c_Mg, c_O;

	DB  TEMPERATURE;
	DB	ReacD;														// [-]
	DB	MwFeO=71.85, MwFe=55.85, MwO=16.;
	DB	FeOMf, FeMf;												// [wt%]
	DB  MgOSlagMf, FeOSlagMf, SiO2SlagMf, Al2O3SlagMf, Fe2O3SlagMf;	// [wt%]
	DB	SlagTot, CompTot;											// [g]
	DB	AppViscosity;


	d = Get_Domain(1);
	k=0;
	timestep = RP_Get_Integer("time-step");
	CX_Message( "timestep:%d\n", timestep ) ;

	kk=floor((timestep-4000)/6000.);
	timeStepValue = (6000  * kk) + 4002;
	CX_Message( "kk_num:%d\n", kk ) ;


	thread_loop_c( t, d ) 
	{

		begin_c_loop( c, t ) 
		{

			if (timestep > timeStepValue && timestep < (timeStepValue+2))
			{

			C_CENTROID( x, c, t );

			TEMPERATURE=C_UDMI(c,t,26)-273.15; // [C]

				if (C_UDMI(c,t,24) >= 1. && TEMPERATURE >= 1000.){
					
					if (TEMPERATURE > 1537. ){ TEMPERATURE=1537.; }

				k=k+1;
				CX_Message( "exec_ca:%d\n", k ) ;

				ReacD=C_UDMI(c,t,16);	// [-]	

				if (ReacD>1.) {ReacD=1.;}

				c_Fe=(-32.465+97.635*(ReacD)) + 125.6056*(1.-ReacD)*MwFe/MwFeO;
				c_Si=1.640493; 
				c_Al=0.508058; 
				c_Mg=0.81402; 
				c_O=125.6056*(1.-ReacD)*MwO/MwFeO + 2.857429;

				tqremc( 0, &noerr );
				tqsetc( "t", 0, 0, TEMPERATURE, &numcon, &noerr );
				tqsetc( "p", 0, 0, PRESSURE, &numcon, &noerr );

				tqsetc("ia", 0, 1, c_Fe,&numcon,&noerr);	// Fe
				tqsetc("ia", 0, 2, c_Si,&numcon,&noerr);	// Si
				tqsetc("ia", 0, 3, c_Al,&numcon,&noerr);	// Al
				tqsetc("ia", 0, 4, c_Mg,&numcon,&noerr);	// Mg
				tqsetc("ia", 0, 5, c_O,&numcon,&noerr);		// O


   /// Calculation of Equilibrium amount
  tqlite(&islite, &noerr);
  if (islite) 
    {
      CX_Message("*** Phase target calculations have been omitted here,\n"
             "*** since they are not possible with the\n"
             "*** 'light' version of ChemApp.\n\n");
    }
  else 
    {

			tqce(" ", 0, 0, vals, &noerr );
			if( noerr != 0 ) {
				CX_Message( "noerr:%d\n", noerr ) ;
			}

			tqgetr( "a", 1, 1, &equi_slag_MgO, &noerr ); //slag
			tqgetr( "a", 1, 2, &equi_slag_FeO, &noerr ); //slag
			tqgetr( "a", 1, 3, &equi_slag_SiO2, &noerr ); //slag
			tqgetr( "a", 1, 4, &equi_slag_Al2O3, &noerr ); //slag
			tqgetr( "a", 1, 5, &equi_slag_Fe2O3, &noerr ); //slag

			tqgetr( "a", 3, 1, &equi_AMON_FeO, &noerr ); //AMonoxide
			tqgetr( "a", 3, 2, &equi_AMON_Fe2O3, &noerr ); //AMonoxide

			tqgetr( "a", 4, 0, &equi_Olivine, &noerr ); //Olivine

			tqgetr( "a", 28, 1, &equi_Fe_bcc, &noerr ); //pure
			tqgetr( "a", 29, 1, &equi_Fe_fcc, &noerr ); //pure				


			/*
			/// Calculation of solidus temperature
			 // get index number of phase
			tqinp("ASlag-liq", &lint, &noerr);
			tqsetc("a", lint, 0, 0.0,&numcon,&noerr);
			darray2[0]=1000.0;
			tqce("t",0,0,darray2,&noerr); 
			tqgetr("t", 0, 0, &d1, &noerr);   
			C_UDMI(c,t,4)=d1; // solidus
 
			/// Calculation of liquidus temperature
			// get index number of phase
			tqinp("ASlag-liq", &lint, &noerr);
			tqsetc("a", lint, 0, -1.0,&numcon,&noerr);
			darray2[0]=1000.0;
			tqce("t",0,0,darray2,&noerr); 
			tqgetr("t", 0, 0, &d1, &noerr);
 			C_UDMI(c,t,5)=d1; // liquidus
			*/
			
			SlagTot = equi_slag_MgO + equi_slag_FeO + equi_slag_SiO2 
					+ equi_slag_Al2O3 + equi_slag_Fe2O3; // [g]
			CompTot = SlagTot + equi_AMON_FeO + equi_AMON_Fe2O3 + equi_Olivine
					+ equi_Fe_bcc + equi_Fe_fcc;		// [g]
			FeOMf = equi_AMON_FeO / CompTot;
			FeMf  = (equi_Fe_bcc + equi_Fe_fcc + equi_AMON_Fe2O3 + equi_Olivine)
					/ CompTot;
			MgOSlagMf = equi_slag_MgO / CompTot;
			FeOSlagMf = equi_slag_FeO / CompTot;
			SiO2SlagMf = equi_slag_SiO2 / CompTot;
			Al2O3SlagMf = equi_slag_Al2O3 / CompTot;
			Fe2O3SlagMf = equi_slag_Fe2O3 / CompTot;

			// slag viscosity model

			AppViscosity=(exp(-6.41+(44190./(TEMPERATURE+273.15)))*FeMf
				+ exp(-15.85+(53540./(TEMPERATURE+273.15)))*FeOMf
				+ exp(-63.18+(114540./(TEMPERATURE+273.15)))*(SlagTot/CompTot));
			
			CX_Message( "AppViscosity:%f\n", AppViscosity ) ;			
			
			C_UDMI(c,t,27)=(AppViscosity*0.1); //[Pa s]
			C_UDMI(c,t,22)=equi_AMON_FeO;
			C_UDMI(c,t,28)=equi_Fe_bcc + equi_Fe_fcc;
			C_UDMI(c,t,29)=SlagTot;

    }
				}
		}

		}
		end_c_loop( c, t )
	}

}