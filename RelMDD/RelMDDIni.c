#include "util.h"
#include "cuddInt.h"
#include "structures.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <time.h>
#include "RelMDD.h"
void RelMDD_int(){
 dd = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);
 Cudd_AutodynDisable(dd);
 bck=  Cudd_ReadPlusInfinity( dd );
 Cudd_SetBackground( dd,bck ); 
}
