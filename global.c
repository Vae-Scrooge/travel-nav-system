#include "global.h"

int locate(ALGraph graph, const char *e)
{
	int i=0;
        if(e == NULL)
        {
                return -1;
        }
	for(i=0;i<graph.nodenum;i++)
	{
		if(strcmp(e,graph.roadlist[i].data)==0)
		{
			return i;
		}
	}
        return -1;
}

