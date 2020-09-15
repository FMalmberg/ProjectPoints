/////////////////////////////////////////////////////////////
// CINEMA 4D SDK                                           //
/////////////////////////////////////////////////////////////
// (c) 1989-2004 MAXON Computer GmbH, all rights reserved  //
/////////////////////////////////////////////////////////////

// Starts the plugin registration

#include "c4d.h"

// forward declarations

Bool RegisterProjectPoints(void);

//Bool RegisterPointTool(void);

Bool PluginStart(void)
{
	if (!RegisterProjectPoints()) return FALSE;
	//if (!RegisterPointTool()) return FALSE;

	return TRUE;
}

void PluginEnd(void)
{
}

Bool PluginMessage(Int32 id, void *data)
{

	switch (id)
	{
	case C4DPL_INIT_SYS:
		if (!g_resource.Init()) return FALSE; // don't start plugin without resourc
		return TRUE;

	}

	return FALSE;
}
