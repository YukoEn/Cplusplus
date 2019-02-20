//used to locate the particle
cell_t findCellFluent(int p, Thread *t ,int nCells) //p=particle index to find
{
	   cell_t cp,cStart;
	   int j;
	   int cStep=1000;
	   real pPos[3]={0.};
	   setFLUENTDebug(110);
//
	   if ((partData[p]).pos[1]<-0.125){(partData[p]).pos[1]=-0.125;}
	   if ((partData[p]).pos[1]>0.125){(partData[p]).pos[1]=0.125;}
	   //
	   for (j=0;j<3;j++) pPos[j]=(double)(partData[p]).pos[j];
	   setFLUENTDebug(111);
	   if (partData[p].cellId>-1){ //cell has already been found last timestep
		   cp=Find_Cell_At_Location(t,(cell_t)(partData[p].cellId) , pPos,(int)100);
		   setFLUENTDebug(112);
	   }	
	   else if (partData[p].cellId<=-1)
	   { ///cell not been found last timestep(-1) or first time search(-2): look thoroughly
	   	setFLUENTDebug(113);
		cStart=(cell_t)0;
		cp=Find_Cell_At_Location(t,cStart, pPos,(int)100);
		while ((cp<0)&&(((int)cStart)<nCells))
		{
		   cp=Find_Cell_At_Location(t,cStart, pPos,(int)100);
		   cStart=(cell_t)((int)cStart+(int)cStep);
		}
		setFLUENTDebug(114);
	   }	
	  if (cp<0) Message("Warning: Could not locate grid position of particle #%d at (%f/%f/%f)\n"
						,(int)p,(float)pPos[0],(float)pPos[1],(float)pPos[2]);
	  return (cell_t)cp;
}
