#pragma once

#define break_if(x)		if ( (x) ) { break; }
#define safe_delete(x)	if ( (x) ) { delete x; x = NULL; }
#define safe_delete_array(x) if (x) {delete []x; x = NULL;}

#define degreesToRadian(x)	(M_PI * (x) / 180.0)     