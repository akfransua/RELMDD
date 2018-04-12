
/**CFile***********************************************************************//*
This function is a modified version of cuddAddRead so the copy right gooes to :Copyright   [Copyright (c) 1995-2004, Regents of the University of Colorado and I do not bear ownership of this function. I just used it to help me to my work. Thank Fabio for making it available.
.............................................................................................................. 
This functions is used to creat relations such as identitty, top and bottom. it accepts a list an generate the associated ADD,

******************************************************************************/


#include "util.h"
#include "cuddInt.h"
#include "structures.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <time.h>
#include "RelMDD.h"
 /*void RelMDD_int(){
    dd = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);
    Cudd_AutodynDisable(dd);
    bck=  Cudd_ReadPlusInfinity( dd );

    Cudd_SetBackground( dd,bck ); 
    printf("\ngood.....\n");
    }
  */
int RelMdd_CreateRelation(
			     // FILE * fp /* input file pointer */,
			     DdManager * dd /* DD manager */ ,
			     DdNode **
			     E /* characteristic function of the graph */ ,
			     DdNode *** x /* array of row variables */ ,
			     DdNode *** y /* array of column variables */ ,
			     DdNode ***
			     xn /* array of complemented row variables */ ,
			     DdNode ***
			     yn_
			     /* array of complemented column variables */ ,
			     int *nx /* number or row variables */ ,
			     int *ny /* number or column variables */ ,
			     int *m /* number of rows */ ,
			     int *n /* number of columns */ ,
			     int bx /* first index of row variables */ ,
			     int sx /* step of row variables */ ,
			     int by /* first index of column variables */ ,
			     int sy /* step of column variables */ ,
			     int row_nn,
			     int col_nn,
			     int *row_n, int *col_n, double *id_relation)
{

    // dd = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);
    DdNode *one, *zero;
    DdNode *w, *neW;
    DdNode *minterm1;
    int u, v, i, nv;
    int lnx, lny;
    //CUDD_VALUE_TYPE val;
    // CUDD_VALUE_TYPE *val1; 
    //val1 =(double *) malloc(sizeof(double));
    // val1 = RelMDD_Identity();
    //  save;

    // CUDD_VALUE_TYPE val1 []={1,0,0, 0,0,2,0, 0,0,0,3, 0,0,0,4};
    //CUDD_VALUE_TYPE *val1=RelMDD_Identity();
    CUDD_VALUE_TYPE *val1 = id_relation;
    int kkk=0;

    // int uu [] ={0,0,0,0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,4,4};
    int *uu;
    uu = row_n;
    // int vv [] ={0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0,1,2,3,4};
    int *vv;
    vv = col_n;
    int pp = 0;
    int ppp = 0;
    DdNode **lx, **ly, **lxn, **lyn;

    one = DD_ONE(dd);
    zero = DD_ZERO(dd);

    /* err = fscanf(fp, "%d %d", &u, &v);
       if (err == EOF) {
       return(0);
       } else if (err != 2) {
       return(0);
       } */
    u = row_nn;
    v = col_nn;
    *m = u;
    /* Compute the number of x variables. */
    lx = *x;
    lxn = *xn;
    u--;			/* row and column numbers start from 0 */
    for (lnx = 0; u > 0; lnx++) {
	u >>= 1;
    }
    /* Here we rely on the fact that REALLOC of a null pointer is
     ** translates to an ALLOC.
     */
    if (lnx > *nx) {
	*x = lx = REALLOC(DdNode *, *x, lnx);
	if (lx == NULL) {
	    dd->errorCode = CUDD_MEMORY_OUT;
	    return (0);
	}
	*xn = lxn = REALLOC(DdNode *, *xn, lnx);
	if (lxn == NULL) {
	    dd->errorCode = CUDD_MEMORY_OUT;
	    return (0);
	}
    }

    *n = v;
    /* Compute the number of y variables. */
    ly = *y;
    lyn = *yn_;
    v--;			/* row and column numbers start from 0 */
    for (lny = 0; v > 0; lny++) {
	v >>= 1;
    }
    /* Here we rely on the fact that REALLOC of a null pointer is
     ** translates to an ALLOC.
     */
    if (lny > *ny) {
	*y = ly = REALLOC(DdNode *, *y, lny);
	if (ly == NULL) {
	    dd->errorCode = CUDD_MEMORY_OUT;
	    return (0);
	}
	*yn_ = lyn = REALLOC(DdNode *, *yn_, lny);
	if (lyn == NULL) {
	    dd->errorCode = CUDD_MEMORY_OUT;
	    return (0);
	}
    }

    /* Create all new variables. */
    for (i = *nx, nv = bx + (*nx) * sx; i < lnx; i++, nv += sx) {
	do {
	    dd->reordered = 0;
	    lx[i] = cuddUniqueInter(dd, nv, one, zero);
	} while (dd->reordered == 1);
	if (lx[i] == NULL)
	    return (0);
	cuddRef(lx[i]);
	do {
	    dd->reordered = 0;
	    lxn[i] = cuddUniqueInter(dd, nv, zero, one);
	} while (dd->reordered == 1);
	if (lxn[i] == NULL)
	    return (0);
	cuddRef(lxn[i]);
    }
    for (i = *ny, nv = by + (*ny) * sy; i < lny; i++, nv += sy) {
	do {
	    dd->reordered = 0;
	    ly[i] = cuddUniqueInter(dd, nv, one, zero);
	} while (dd->reordered == 1);
	if (ly[i] == NULL)
	    return (0);
	cuddRef(ly[i]);
	do {
	    dd->reordered = 0;
	    lyn[i] = cuddUniqueInter(dd, nv, zero, one);
	} while (dd->reordered == 1);
	if (lyn[i] == NULL)
	    return (0);
	cuddRef(lyn[i]);
    }
    *nx = lnx;
    *ny = lny;

    *E = dd->background;	/* this call will never cause reordering */
    cuddRef(*E);

    while (val1) {
	/* = fscanf(fp, "%d %d %lf", &u, &v, &val);
	   if (err == EOF) {
	   break;
	   } else if (err != 3) {
	   return(0);
	   } */
	if (uu[pp] >= *m || vv[ppp] >= *n || uu[pp] < 0 || vv[ppp] < 0) {
	    return (0);
	}

	minterm1 = one;
	cuddRef(minterm1);

	/* Build minterm1 corresponding to this arc */
	for (i = lnx - 1; i >= 0; i--) {
	    if (uu[pp] & 1) {
		w = Cudd_addApply(dd, Cudd_addTimes, minterm1, lx[i]);
	    } else {
		w = Cudd_addApply(dd, Cudd_addTimes, minterm1, lxn[i]);
	    }
	    if (w == NULL) {
		Cudd_RecursiveDeref(dd, minterm1);
		return (0);
	    }
	    cuddRef(w);
	    Cudd_RecursiveDeref(dd, minterm1);
	    minterm1 = w;
	    uu[pp] >>= 1;
	}
	for (i = lny - 1; i >= 0; i--) {
	    if (vv[ppp] & 1) {
		w = Cudd_addApply(dd, Cudd_addTimes, minterm1, ly[i]);
	    } else {
		w = Cudd_addApply(dd, Cudd_addTimes, minterm1, lyn[i]);
	    }
	    if (w == NULL) {
		Cudd_RecursiveDeref(dd, minterm1);
		return (0);
	    }
	    cuddRef(w);
	    Cudd_RecursiveDeref(dd, minterm1);
	    minterm1 = w;
	    vv[ppp] >>= 1;
	}
	/* Create new constant node if necessary.
	 ** This call will never cause reordering.
	 */
	neW = cuddUniqueConst(dd, val1[kkk]);
	if (neW == NULL) {
	    Cudd_RecursiveDeref(dd, minterm1);
	    return (0);
	}
	cuddRef(neW);

	w = Cudd_addIte(dd, minterm1, neW, *E);
	if (w == NULL) {
	    Cudd_RecursiveDeref(dd, minterm1);
	    Cudd_RecursiveDeref(dd, neW);
	    return (0);
	}
	cuddRef(w);
	Cudd_RecursiveDeref(dd, minterm1);
	Cudd_RecursiveDeref(dd, neW);
	Cudd_RecursiveDeref(dd, *E);
	*E = w;
	kkk++;
	pp++;
	ppp++;
    }
    return (1);

}
