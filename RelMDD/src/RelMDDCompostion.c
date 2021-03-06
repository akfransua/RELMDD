/**CFile***********************************************************************

******************************************************************************/
//Before Using RElMDD delete the file cuddMatMult.c from CUDD package
#include "util.h"
#include "cuddInt.h"

#include "structures.h"
/*---------------------------------------------------------------------------*/
/* Constant declarations                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Stucture declarations                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Type declarations                                                         */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Variable declarations                                                     */
/*---------------------------------------------------------------------------*/

#ifndef lint
static char rcsid[] DD_UNUSED =
    "$Id: cuddMatMult.c,v 1.17 2004/08/13 18:04:50 fabio Exp $";
#endif

/*---------------------------------------------------------------------------*/
/* Macro declarations                                                        */
/*---------------------------------------------------------------------------*/

/**AutomaticStart*************************************************************/

/*---------------------------------------------------------------------------*/
/* Static function prototypes                                                */
/*---------------------------------------------------------------------------*/
CUDD_VALUE_TYPE union_OperationComposition(CUDD_VALUE_TYPE F,
					   CUDD_VALUE_TYPE G);
CUDD_VALUE_TYPE interserction_OperationComposition(CUDD_VALUE_TYPE F,
						   CUDD_VALUE_TYPE G);
static DdNode *addMMRecompostion(DdManager * dd, DdNode * A, DdNode * B,
				 int topP, int *vars);
DdNode *Cudd_addUnionPlus(DdManager * dd, DdNode ** f, DdNode ** g);
void inter(CUDD_VALUE_TYPE F, CUDD_VALUE_TYPE G);
CUDD_VALUE_TYPE valF1();
double *mat1;
double *mat2;
char **source_comp;
char **target_Comp;
double *element_comp;
int *row_trackComp;
int *col_trackComp;
//static double arrya[1000];	// malloc this and initialization
static int t = 1;
int row_comp;
int col_comp;

/**AutomaticEnd***************************************************************/

/*---------------------------------------------------------------------------*/
/* Definition of exported functions                                          */
/*---------------------------------------------------------------------------*/

/**Function********************************************************************

  Synopsis [Calculates the product of two matrices represented as
  ADDs.]

  Description [Calculates the product of two matrices, A and B,
  represented as ADDs. This procedure implements the quasiring multiplication
  algorithm.  A is assumed to depend on variables x (rows) and z
  (columns).  B is assumed to depend on variables z (rows) and y
  (columns).  The product of A and B then depends on x (rows) and y
  (columns).  Only the z variables have to be explicitly identified;
  they are the "summation" variables.  Returns a pointer to the
  result if successful; NULL otherwise.]

  SideEffects [None]

  SeeAlso     [Cudd_addTimesPlus Cudd_addTriangle Cudd_bddAndAbstract]
  
  This is Funtion is a direct modification of Cudd_addMatrixMultiply. 
  Basically, replacing addition operation with union operation and multiplication with intersection.
  Copyright goes to Fabio not me please and just using to get my MSc 
******************************************************************************/

DdNode *Cudd_addMatrixComposition(DdManager * dd,
				  DdNode * A,
				  DdNode * B, DdNode ** z, int nz)
{
    int i, nvars, *vars;
    DdNode *res;

    /* Array vars says what variables are "summation" variables. */
    nvars = dd->size;
    vars = ALLOC(int, nvars);
    if (vars == NULL) {
	dd->errorCode = CUDD_MEMORY_OUT;
	return (NULL);
    }
    for (i = 0; i < nvars; i++) {
	vars[i] = 0;
    }
    for (i = 0; i < nz; i++) {
	vars[z[i]->index] = 1;
    }

    do {
	dd->reordered = 0;
	res = addMMRecompostion(dd, A, B, -1, vars);
    } while (dd->reordered == 1);
    FREE(vars);
    return (res);

}				/* end of Cudd_addMatrixMultiply */

/*---------------------------------------------------------------------------*/
/* Definition of internal functions                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Definition of static functions                                            */
/*---------------------------------------------------------------------------*/

/**Function********************************************************************

  Synopsis    [Performs the recursive step of Cudd_addMatrixMultiply.]

  Description [Performs the recursive step of Cudd_addMatrixComposition.
  Returns a pointer to the result if successful; NULL otherwise.]

  SideEffects [None]

******************************************************************************/
static DdNode *addMMRecompostion(DdManager * dd,
				 DdNode * A,
				 DdNode * B, int topP, int *vars)
{
    DdNode *zero, *At,		/* positive cofactor of first operand */
    *Ae,			/* negative cofactor of first operand */
    *Bt,			/* positive cofactor of second operand */
    *Be,			/* negative cofactor of second operand */
    *t,				/* positive cofactor of result */
    *e,				/* negative cofactor of result */
    *scaled,			/* scaled result */
    *add_scale,			/* ADD representing the scaling factor */
    *res;
    int i;			/* loop index */
    double scale;		/* scaling factor */
    int index;			/* index of the top variable */
    CUDD_VALUE_TYPE value;
    unsigned int topA, topB, topV;
    DD_CTFP cacheOp;

    statLine(dd);
    zero = DD_ZERO(dd);

    if (A == zero || B == zero) {
	return (zero);
    }

    if (cuddIsConstant(A) && cuddIsConstant(B)) {
	/* Compute the scaling factor. It is 2^k, where k is the
	 ** number of summation variables below the current variable.
	 ** Indeed, these constants represent blocks of 2^k identical
	 ** constant values in both A and B.
	 */
//value = cuddV(A) * cuddV(B); printf("\n%f\t%f\t%f\n", value, cuddV(A) ,cuddV(B));
	//inter(cuddV(A) ,cuddV(B));
	value = union_OperationComposition(cuddV(A), cuddV(B));
	//value =vv;
	//vv++;
	
	for (i = 0; i < dd->size; i++) {
	    if (vars[i]) {
		if (dd->perm[i] > topP) {
		    value *= (CUDD_VALUE_TYPE) 2;
		}
	    }
	}
	res = cuddUniqueConst(dd, value);
	return (res);
    }

    /* Standardize to increase cache efficiency. Clearly, A*B != B*A
     ** in matrix multiplication. However, which matrix is which is
     ** determined by the variables appearing in the ADDs and not by
     ** which one is passed as first argument.
     */
    if (A > B) {
	DdNode *tmp = A;
	A = B;
	B = tmp;
    }

    topA = cuddI(dd, A->index);
    topB = cuddI(dd, B->index);
    topV = ddMin(topA, topB);

    cacheOp = (DD_CTFP) addMMRecompostion;
    res = cuddCacheLookup2(dd, cacheOp, A, B);
    if (res != NULL) {
	/* If the result is 0, there is no need to normalize.
	 ** Otherwise we count the number of z variables between
	 ** the current depth and the top of the ADDs. These are
	 ** the missing variables that determine the size of the
	 ** constant blocks.
	 */
	if (res == zero)
	    return (res);
	scale = 1.0;
	for (i = 0; i < dd->size; i++) {
	    if (vars[i]) {
		if (dd->perm[i] > topP && (unsigned) dd->perm[i] < topV) {
		    scale *= 2;
		}
	    }
	}
	if (scale > 1.0) {
	    cuddRef(res);
	    add_scale = cuddUniqueConst(dd, (CUDD_VALUE_TYPE) scale);
	    if (add_scale == NULL) {
		Cudd_RecursiveDeref(dd, res);
		return (NULL);
	    }
	    cuddRef(add_scale);
	    scaled = cuddAddApplyRecur(dd, Cudd_addTimes, res, add_scale);
	    if (scaled == NULL) {
		Cudd_RecursiveDeref(dd, add_scale);
		Cudd_RecursiveDeref(dd, res);
		return (NULL);
	    }
	    cuddRef(scaled);
	    Cudd_RecursiveDeref(dd, add_scale);
	    Cudd_RecursiveDeref(dd, res);
	    res = scaled;
	    cuddDeref(res);
	}
	return (res);
    }

    /* compute the cofactors */
    if (topV == topA) {
	At = cuddT(A);
	Ae = cuddE(A);
    } else {
	At = Ae = A;
    }
    if (topV == topB) {
	Bt = cuddT(B);
	Be = cuddE(B);
    } else {
	Bt = Be = B;
    }

    t = addMMRecompostion(dd, At, Bt, (int) topV, vars);
    if (t == NULL)
	return (NULL);
    cuddRef(t);
    e = addMMRecompostion(dd, Ae, Be, (int) topV, vars);
    if (e == NULL) {
	Cudd_RecursiveDeref(dd, t);
	return (NULL);
    }
    cuddRef(e);

    index = dd->invperm[topV];
    if (vars[index] == 0) {
	/* We have split on either the rows of A or the columns
	 ** of B. We just need to connect the two subresults,
	 ** which correspond to two submatrices of the result.
	 */
	res = (t == e) ? t : cuddUniqueInter(dd, index, t, e);
	if (res == NULL) {
	    Cudd_RecursiveDeref(dd, t);
	    Cudd_RecursiveDeref(dd, e);
	    return (NULL);
	}
	cuddRef(res);
	cuddDeref(t);
	cuddDeref(e);
    } else {
	/* we have simultaneously split on the columns of A and
	 ** the rows of B. The two subresults must be added.
	 */
	res = cuddAddApplyRecur(dd, Cudd_addUnionPlus, t, e);
	if (res == NULL) {
	    Cudd_RecursiveDeref(dd, t);
	    Cudd_RecursiveDeref(dd, e);
	    return (NULL);
	}
	cuddRef(res);
	Cudd_RecursiveDeref(dd, t);
	Cudd_RecursiveDeref(dd, e);
    }

    cuddCacheInsert2(dd, cacheOp, A, B, res);

    /* We have computed (and stored in the computed table) a minimal
     ** result; that is, a result that assumes no summation variables
     ** between the current depth of the recursion and its top
     ** variable. We now take into account the z variables by properly
     ** scaling the result.
     */
    if (res != zero) {
	scale = 1.0;
	for (i = 0; i < dd->size; i++) {
	    if (vars[i]) {
		if (dd->perm[i] > topP && (unsigned) dd->perm[i] < topV) {
		    scale *= 2;
		}
	    }
	}
	if (scale > 1.0) {
	    add_scale = cuddUniqueConst(dd, (CUDD_VALUE_TYPE) scale);
	    if (add_scale == NULL) {
		Cudd_RecursiveDeref(dd, res);
		return (NULL);
	    }
	    cuddRef(add_scale);
	    scaled = cuddAddApplyRecur(dd, Cudd_addTimes, res, add_scale);
	    if (scaled == NULL) {
		Cudd_RecursiveDeref(dd, res);
		Cudd_RecursiveDeref(dd, add_scale);
		return (NULL);
	    }
	    cuddRef(scaled);
	    Cudd_RecursiveDeref(dd, add_scale);
	    Cudd_RecursiveDeref(dd, res);
	    res = scaled;
	}
    }
    cuddDeref(res);
    return (res);

}				/* end of addMMRecur */

/*---------------------------------------------------------------------------*/
/* Definition of internal functions                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Definition of static functions                                            */
/*---------------------------------------------------------------------------*/

/**Function********************************************************************

  The union operation from the xml is done here 

******************************************************************************/

DdNode *Cudd_addUnionPlus(DdManager * dd, DdNode ** f, DdNode ** g)
{
    DdNode *res;
    DdNode *F, *G;
    CUDD_VALUE_TYPE value;

    F = *f;
    G = *g;
    if (F == DD_ZERO(dd))
	return (G);
    if (G == DD_ZERO(dd))
	return (F);
    if (cuddIsConstant(F) && cuddIsConstant(G)) {
	//  printf("\n..........................................................................................\n");
	//cInterserction(cuddV(F), cuddV(G));
	//value = cuddV(F)+cuddV(G);

	value = interserction_OperationComposition(cuddV(F), cuddV(G));
	///printf("\n+++ %f%f\t%f+++\n", value, cuddV(F), cuddV(G));
	res = cuddUniqueConst(dd, value);
	return (res);
    }
    // printf("\n");
    if (F > G) {		/* swap f and g */
	*f = G;
	*g = F;
    }
    return (NULL);

}

 /*---------------------------------------------------------------------------*/
/* Definition of internal functions                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Definition of static functions                                            */
/*---------------------------------------------------------------------------*/

/**Function********************************************************************

Mapping the relations to thier objects

******************************************************************************/

CUDD_VALUE_TYPE union_OperationComposition(CUDD_VALUE_TYPE F,
					   CUDD_VALUE_TYPE G)
{
  
    int listob1 = row_comp;
    int listob2 = col_comp;
    CUDD_VALUE_TYPE state[listob1][listob2];	// malloc here
    CUDD_VALUE_TYPE value=0;
    compoUnionPtr ret;
    ret = (compoUnionPtr) compoUionIni();
    int size_struct = ret->len;
    int j, l, rn=0, cn=0;
    static int i, k;
    int yy = 1;
    for (i = 0; i < listob1; i++) {

	for (k = 0; k < listob2; k++) {

	    state[i][k] = (CUDD_VALUE_TYPE) yy;
	    yy++;
	    // printf("%d",state[i][k]);
	}
    }

    for (i = 0; i < listob1; i++) {

	for (k = 0; k < listob2; k++) {

	    if (state[i][k] == F) {

		//printf("\n1- %f\t%f\t%f\n", F, G, state[i][k]);
		//  printf("\n%d\t%d\n", i,k);
		rn = i;
	    }

	    if (state[i][k] == (G)) {	// to G-first of the second matrix

		//printf("\n%f\t%f\t%f\n" ,F,G, state[i][k]);
		//  printf("\n,,%d\t%d,,\n", i,k);
		cn = k;		//
	    }
	}
    }

    for (j = 0; j < size_struct; j++) {

	if (!(strcmp(source_comp[rn], *ret[j].source))
	    && !(strcmp(target_Comp[cn], *ret[j].target))) {
	   // printf("\n2-%s\t%s\n ", *ret[j].source, *ret[j].target);

	    for (l = 0; l < ret->len_Content[j]; l++) {
		int ma = (int) (F - 1);
		int ka = (int) (G - 1);	// to G-first of the second matrix -1
		if (l % 3 == 0) {
		    if ((ret[j].content[l] == mat1[ma])
			&& (ret[j].content[l + 1] == mat2[ka])) {
			//printf("\nREL%f\t%f\t%fREL\n", mat1[ma], mat2[ka], ret[j].content[l + 2]);	//}} }}}
			//row[t] = rn;
			//col[t] = cn;
			row_trackComp[t]=rn;
			col_trackComp[t]=cn;
			//arrya[t] = ret[j].content[l + 2];	
			element_comp[t]= ret[j].content[l + 2];
			//printf("\n,,%f,,\n",arrya[t]);
			//value= ret[j].content[l+2];
			value = (double) (t);
			//printf("\nt-%d \t%d\t %d t-\n", t, row_trackComp[t], col_trackComp[t]);
			t++;
		    }

		}

	}}
    }

    return value;
}

 /*---------------------------------------------------------------------------*/
/* Definition of internal functions                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Definition of static functions                                            */
/*---------------------------------------------------------------------------*/

/**Function********************************************************************

Mapping the relations to thier objects

******************************************************************************/
CUDD_VALUE_TYPE interserction_OperationComposition(CUDD_VALUE_TYPE F,
						   CUDD_VALUE_TYPE G)
{
  
    CUDD_VALUE_TYPE value=0;
    compoInterPtr ret;
    ret = (compoInterPtr)compoInterIni();
    int size_struct = ret->len;
    int j, l, rn, cn;
    rn = row_trackComp[(int) (F)];
    cn = col_trackComp[(int) (F)];
    for (j = 0; j < size_struct; j++) {
	if (!(strcmp(source_comp[rn], *ret[j].source))
	    && !(strcmp(target_Comp[cn], *ret[j].target))) {
	    //printf("\n%s\t%s\n ", *ret[j].source, *ret[j].target);
	    for (l = 0; l < ret->len_Content[j]; l++) {
		int ma = (int) (F);
		int ka = (int) (G);
		if (l % 3 == 0) {
		    if ((ret[j].content[l] ==element_comp[ma])
			&& (ret[j].content[l + 1] == element_comp[ka])) {
			//printf("\n+++%f\t%f\t%f+++\n", element_comp[ma],
			     //  element_comp[ka], ret[j].content[l + 2]);
			//    arrya[t]= ret[j].content[l+2];  
			element_comp[t] = ret[j].content[l + 2];
			//element_comp[t]= ret[j].content[l + 2];
			// printf("\n%f\n",interarr[ma]);
			//row[t] = rn;
			row_trackComp[t]=rn;
			//col[t] = cn;
			col_trackComp[t]=cn;
			value = t;
			//printf("\nt in un-%d \t%d\t %d t-\n", t, row_trackComp[t],
			  //    col_trackComp[t]);
			t++;
			// t++;
		    }

		}

	    }
	}
    }

    return value;
}
