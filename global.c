#include "global.h"

//¶¨Î»
int locate(ALGraph graph,char * e)
{
	int i=0;
	for(i=0;i<graph.nodenum;i++)
	{
		if(strcmp(e,graph.roadlist[i].data)==0)
		{
			return i;
		}
	}
	return 0;
}
