/**CFile***********************************************************************



This file contains the major functios for manipulating relations.
It includes functions imported from CUDD and Limxml. Those that are imported from these libraries have their copy right owned to those libraries and I do no bear ownership of those functions.
***Extenal functions are those that the user can call to manipulate realtions.
***Intenal functions are used within RelMDD
 
This file contains the major operations and their subsidiary functions.  

******************************************************************************/

#include "util.h"
#include "cuddInt.h"
#include "structures.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <time.h>
#include "RelMDD.h"
#define MAXA 500
DdManager * dd;
DdNode *bck;
static double **holdconverse;
static double **holdcomplement;
static double **holdMeet;
static double **holdJoin;
static double **holdId;
static double **holdBottom;
static double **holdTop;
static double **value_comp;
int *ini;
static int no_vars;
static int  cno_vars;
DdNode *tempNode;
/************************************************************************************************************/
// declaration for identity relations
static char **idSource;
static char **idTarget;
static int id_id_track;
static int *inifor_id;
static int id_row;
static int id_col;
/************************************************************************************************************/
// bottom declation
static char **BottomSource;
static char **BottomTarget;
static int Bottom_track;
static int *inifor_Bottom;
//static int num_list;
static int Bottom_row;
static int Bottom_col;

/************************************************************************************************************/

static char **topSource;
static char **topTarget;
static int Top_track;
static int *inifor_Top;
static int Top_row;
static int Top_col;


double setSequence(double value);
static int mrow;
static int mcol;
static int tran_col;
static int tran_row;
static char **compleSource;
static char **compleTarget;
static char **tran_Source;
static char **tran_Target;
static int track;
static int track_trans;
static int *iniforcompl;
static int *iniforTrans;
static double *ComplementMat2;
static double *TranposeMat;
DdNode *node_Complement(DdNode * mat);
static DdNode **summation_var ;

/************************************************************************************************************/
static int track_union;
static int track_Inters;
static int *iniforUnion;
static int *iniforInters;
static double *unionMat1;
static double *UnionMat2;
static double *interMat1;
static double *interMat2;
static char **unionSource;
static char **unionTarget;
static char **interSource;
static char **interTarget;
static int urol;
static int ucol;
static int interrol;
static int interrcol;
//static double *mat1;
//static double *mat2;
/*************************************************************************************************************

  Function: void RelMDD_int();
  To be used for the initialization of RelMDD by initialization the the cudd package and setting the background
  Value to PlusInfinity. To use RelMDD  you must call this functions to initialize it before any other functions 
  could be called. If this function is not called you will get error like segmentation fualt. 
************************************************************************************************************/

void RelMDD_Int()
{
    dd = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    Cudd_AutodynDisable(dd);
    bck = Cudd_ReadPlusInfinity(dd);
    Cudd_SetBackground(dd, bck);
    //RelMDD_intt();
}/*End of void RelMDD_int()*/

void RelMDD_Quit(){
Cudd_Quit(dd);
}
 /* *******************************************************************************************************************
Prototypes decalations of functions in this file.
   function: RelMDDConverse_Operation(int tra_rn, int tra_col, char **srelation, char **trelation, double *matrimx)
   Initializes the variables for the converse operations. This is an external functions. It calls other functions 
   and uses them to compute the converse of a given relation. It returns the list of relations, the source and target 
   objects and the other field defined in the structure RelMDDConverse.
   tran_rn is the size of row of the Matrix
   tran_col is the size of col of the Matrix
   followed by the source and target object 
   then finally the matrix itself
 ***************************************************************************************************************/
RelMDDConversePtr RelMDDConverse_Operation(int tra_rn, int tra_col, char **source_object, char **target_object, double *matrimx)
{  
    
    int move;
    int  row = tra_rn;
    int col = tra_col;
    RelMDDConversePtr ret= NULL; 
  //  holdconverse = (double *) malloc(sizeof(double) * row *col *2);
    holdconverse= (double **) calloc(row*col*2, sizeof(double*));
    for(move=0;move<row*col*2;move++){
    holdconverse[move] =(double *) calloc(row*col*2, sizeof(double*));
    }
    ret = (RelMDDConversePtr) malloc(sizeof(RelMDDConversePtr)*row *col *2);
    ret->source_Objects= (char*)malloc(sizeof(char)*row *col *2); 
    ret->target_Objects= (char*)malloc(sizeof(char)*row *col *2); 
    ret->relation= (double*)malloc(sizeof(double)*row *col *2); 
    
    tran_row = tra_rn;
    tran_col = tra_col;
    TranposeMat = matrimx;
    track_trans = (tra_rn * tra_col) - 1;
    
    tran_Source = (char **) malloc(sizeof(char) * tra_rn*tran_col*2);
    tran_Target = (char **) malloc(sizeof(char) * tra_rn*tran_col*2);
    for (move = 0; move < tra_rn; move++) {
	tran_Source[move] =source_object[move];
	//printf("%s\t%d\n", tran_Source[move], move);
	//ret[move].source_Objects = source_object[move];
	// printf ("%s\t%d\n", tran_Source[move],move);
    }
    for (move = 0; move < tra_col; move++) {
	
	tran_Target[move] = target_object[move];
	//printf("%s\t%d\n", tran_Target[move], move);
	
    }
    DdNode *Matt2;
    DdNode *Matt = RelMDD_SwapVariables(dd, tra_rn, tra_col);// the transpose matrix
   //Cudd_PrintDebug(dd,Matt,3,40);
        iniforTrans = RelMDD_ListSequence(Matt, tra_rn, tra_col);
   
   Matt2= Cudd_addMonadicApply(dd, Transpose, Matt); //creating the new relations from the xml file
    ret->node= Matt2; //return the ADD here
   
     
    for (move = 0; move < tran_col; move++) {
        ret[move].source_Objects = target_object[move]; // printf ("%s\t%d\n", ret[move].source_Objects,move);
	
	 }
     for (move = 0; move <tran_row; move++) {
        
	ret[move].target_Objects = source_object[move];  //printf ("%s\t%d\n", ret[move].target_Objects,move);
	 }
     int i, j,c =0; 
     for(i=0;i<tran_col;i++){
     
     for(j=0;j<tran_row;j++){
     
         ret->relation[c]= holdconverse[i][j];// return the relation here
        //printf("%f",ret->relation[c]);
         c++;
     }
     	} 
    return ret ;
}/*End of RelMDDConverse_Operation*/



/*************************************************************************************************************
   function: transposeRelation(CUDD_VALUE_TYPE F)
   This function does the converse operations by going into the file and selecting the converse of a given 
   element matching it with the source and target object
   it is called by DdNode *Transpose(DdManager * dd, DdNode * f) to perform manipulation of DD.
   It is an Internal fucntion.
   
************************************************************************************************************/
 
CUDD_VALUE_TYPE transposeRelation(CUDD_VALUE_TYPE F)
{
    int c;

    char *source_Object1[tran_col];//source object
    char *target_Object2[tran_row];// target object
    for (c = 0; c < tran_col; c++) {

	source_Object1[c] = tran_Target[c]; // initialization of source
	//printf("\n++%s++\n ", source_Object1[c]);	

    }
    for (c = 0; c < tran_row ; c++) {

	target_Object2[c] = tran_Source[c]; // initialization of target
      // printf("\n++%s++\n ",target_Object2[c] );	
    }

//char *source_Object1[]={"A","B","A","A"};
//char *target_Object2[]={"A","A","B","B"};

    int listob1 = (sizeof(source_Object1) / sizeof(char)) / 4;
    int listob2 = (sizeof(target_Object2) / sizeof(char)) / 4;
    CUDD_VALUE_TYPE value=0;
    int ste[listob2][listob1];
    conversePtr ret;
    ret = (conversePtr) coverselIni();
    int size_struct = ret->len;
    int  l, j;
    int rn=0, cn=0;
    static int i, k;
    int yy = 1;
   for (i = 0; i < listob2; i++) {

	for (k = 0; k < listob1; k++) {

	    ste[i][k] = yy;
	    yy++;
	// printf("\nN%d%d%dN\n",i,k,ste[i][k]);
	}
    }

    for (i = 0; i < listob2; i++) {

	for (k = 0; k < listob1; k++) {

	    //  if(ste[i][k]==trackt[track]){
	    if (ste[i][k] == iniforTrans[track_trans]) {
		//printf("track%d\n", track_trans);
		// printf("\n%d\t%d\n", i,k);
		
		rn = i;
		cn = k;	
		//printf("\n%d\t%d\t%f\n", k,i, F);
		//printf("\n++%s\t%s++\n ", source_Object1[cn], target_Object2[rn]);	
	    }

	}
    }

    for (j = 0; j < size_struct; j++) {

	if (!(strcmp(source_Object1[cn], *ret[j].source))
	    && !(strcmp(target_Object2[rn], *ret[j].target))) {
	    //printf("\n++%s\t%s++\n ", *ret[j].source, *ret[j].target);
	    for (l = 0; l < ret->len_Content[j]; l++) {
		int ma = (int) (F - 1);
		if (l % 2 == 0) {
		    if (ret[j].content[l] == TranposeMat[ma]) {
		    // printf("<<%f\t%f>>",F,TranposeMat[ma]);
			holdconverse[cn][rn]= ret[j].content[l+1];
			// printf("<<%d\t%f\t>>",ma,holdconverse[cn][rn]);
			value = F;
			track_trans--;
		    }
		}
		//}

		//   printf("\n" );
	    }

	}
    }				

//track_trans--;
    return value;
}/*End of transposeRelation(CUDD_VALUE_TYPE F)*/

/*************************************************************************************************************
   function: *Transpose(DdManager * dd, DdNode * f)
   It does the ADD manipulation of Coverse operation. It is called by Cudd_addMonadicApply(); to compute the 
   the transpose of a relation.
   It is an Internal fucntion.
**************************************************************************************************************/

DdNode *Transpose(DdManager * dd, DdNode * f)
{
    CUDD_VALUE_TYPE value=0;
    DdNode *res;
    if (f == bck) {
	return (bck);
    }

    else {
	if (cuddIsConstant(f) || f == 0) {
	   transposeRelation(cuddV(f));
	   //printf("<<%f>>", cuddV(f));
	    value = setSequence(value);
	    res = cuddUniqueConst(dd, value);
	    return (res);
	}
    }
    return (NULL);

}/*Tranpose*/



DdNode *node_Complement(DdNode * mat)
{

    iniforcompl = RelMDD_ListSequence(mat, mrow, mcol);
    DdNode *tmp1 = Cudd_addMonadicApply(dd, Complement, mat);
    free(iniforcompl);		// check this if memory link
    return tmp1;
}

 /*************************************************************************************************************
   function:RelMDDComplementPtr RelMDDComplement_Operation(int com_Row, int com_Col,
				char **com_Rowlist, char **com_Collist,
				double *matrimx)
   Initializes the variables for the complement operations. This is an external functions. It calls other functions 
   and uses them to compute the complement of a given relation. It returns the list of relations, the source and target 
   objects and the other field defined in the structure RelMDDComplementPtr
  com_Row is the size of row of the Matrix
   com_Col is the size of col of the Matrix
   followed by the source and target object 
   then finally the matrix itself
 *************************************************************************************************************/
 
RelMDDComplementPtr RelMDDComplement_Operation(int com_Row, int com_Col,
				char **com_Rowlist, char **com_Collist,
				double *matrimx)
{  
    RelMDDComplementPtr ret;
    int move;
    mrow = com_Row;
    mcol = com_Col;
    ComplementMat2 = matrimx;
    track = (com_Row * com_Col) - 1;
    holdcomplement= (double **) calloc(mrow * mcol*2, sizeof(double*));
    for(move=0;move<mrow * mcol;move++){
    holdcomplement[move] =(double *) calloc(mrow * mcol, sizeof(double*));
    }
    ret = (RelMDDComplementPtr) malloc(sizeof(RelMDDComplementPtr) *  mrow * mcol*2);
    ret->source_Objects = (char *) malloc(sizeof(char) *  mrow * mcol*2);
    ret->target_Objects = (char *) malloc(sizeof(char)* mrow * mcol*2);
    ret->relation = (double *) malloc(sizeof(double) * mrow * mcol*2);

    compleSource = (char **) malloc(sizeof(char) * com_Row * com_Col*2);
    compleTarget = (char **) malloc(sizeof(char) * com_Row * com_Col*2);

    for (move = 0; move < com_Row; move++) {
	ret[move].source_Objects = com_Rowlist[move];
	//printf("%s\t%d\n", ret[move].source_Objects, move);

	//printf("%f\t%d\n", ComplementMat2[move], move);
	compleSource[move] = com_Rowlist[move];
	//printf("%s\t%d\n", compleSource[move], move);

    }

    for (move = 0; move < com_Col; move++) {

	ret[move].target_Objects = com_Rowlist[move];
	//printf("%s\t%d\n", ret[move].target_Objects, move);

	compleTarget[move] = com_Collist[move];
	//printf("%s\t%d\n", compleTarget[move], move);

    }


    DdNode *M2 = RelMDD_RenameRelation(dd, mrow,mcol);
    iniforcompl = RelMDD_ListSequence(M2,  mrow,   mcol);
    Cudd_addMonadicApply(dd, Complement, M2);
    ret->node=M2;
    
     int i, j,c =0; 
     for(i=0;i<com_Row;i++){
     
     for(j=0;j<com_Col;j++){
     
         ret->relation[c]= holdcomplement[i][j];// return the relation here
         //printf("%f",ret->relation[c]);
         c++;
     }
     	} 
   // ret->relation= holdcomplement;
    
    return ret;
}/*End of  RelMDDComplementPtr RelMDDComplement_Operation()  */

/**************************************************************************************************************
   function: CUDD_VALUE_TYPE complementRelation(CUDD_VALUE_TYPE F)
   This function does the complement operations by going into the file and selecting the complement of a given 
   element matching it with the source and target object
   it is called by DdNode DdNode *Complement(DdManager * dd, DdNode * f)  to perform manipulations of DD.
   it returns a CUDD_VALUE_TYPE of the relation or index 
   It is an Internal fucntion.
 **************************************************************************************************************/

CUDD_VALUE_TYPE complementRelation(CUDD_VALUE_TYPE F)
{

    CUDD_VALUE_TYPE value=0;
    int c;

    char *source_Object1[mrow];
    char *target_Object2[mcol];
    for (c = 0; c < mrow; c++) {

	source_Object1[c] = compleSource[c];

    }

    for (c = 0; c < mcol; c++) {

	target_Object2[c] = compleTarget[c];

    }

    int listob1 = (sizeof(source_Object1) / sizeof(char)) / 4;
    int listob2 = (sizeof(target_Object2) / sizeof(char)) / 4;
    int ste[listob1][listob2];

    complementPtr ret;
    ret = (complementPtr)complIni();
    int size_struct = ret->len;
    int  l, j;
    int rn=0, cn=0;
    static int i, k;
    int yy = 1;
    for (i = 0; i < listob1; i++) {

	for (k = 0; k < listob2; k++) {

	    ste[i][k] = yy;
	    yy++;
	    //printf("%d",ste[i][k]);
	}
    }

    for (i = 0; i < listob1; i++) {

	for (k = 0; k < listob2; k++) {

	    //  if(ste[i][k]==trackt[track]){
	    if (ste[i][k] == iniforcompl[track]) {
		//printf("track%d\n", track);
		//  printf("\n%d\t%d\n", i,k);
		rn = i;
		cn = k;		//
	    }

	}
    }
   

    for (j = 0; j < size_struct; j++) {

	if (!(strcmp(source_Object1[rn], *ret[j].source))
	    && !(strcmp(target_Object2[cn], *ret[j].target))) {
	  //  printf("\n%s\t%s\n ", *ret[j].source, *ret[j].target);
	    for (l = 0; l < ret->len_Content[j]; l++) {
		int ma = (int) (F - 1);
		if (l % 2 == 0) {
		    if (ret[j].content[l] == ComplementMat2[ma]) {
			holdcomplement[rn][cn]= ret[j].content[l+1];
			//printf("\n%f\n", holdcomplement[ma]);
			value = F;
			//printf("\n..%f \t%d\t %f\n", F, ma, pop);
			track--;
		    }
		}
		//}

		//   printf("\n" );
	    }

	}
    }				//}

    return value;
}

/* *************************************************************************************************************
   function: *Complement(DdManager * dd, DdNode * f)
   It does the ADD manipulation of Complement operation. It is called by Cudd_addMonadicApply(); to compute the 
   the complement of a relation.
   It returns the complement of an element from the file
   It is an Internal fucntion.
 **************************************************************************************************************/
DdNode *Complement(DdManager * dd, DdNode * f)
{
    CUDD_VALUE_TYPE value=0;
    DdNode *res;
    if (f == bck) {
	return (bck);
    }

    else {
	if (cuddIsConstant(f) || f == 0) {
	    complementRelation(cuddV(f));
	    value = setSequence(value);
	    res = cuddUniqueConst(dd, value);
	    return (res);
	}
    }
    return (NULL);

}

double setSequence(double value)
{

    return value;

}


/**************************************************************************************************************
   function:RelMDDIntersectionPtr RelMDDintersectOperation(int no_rows, int no_cols, char **ssrelation, char **ttrelation, double *matrix1,
					       double *matrix2)
   Initializes the variables for the intersection  operations. This is an external functions. It calls other functions 
   and uses them to compute the meet of two given relation or matrices. It returns the list of relations, the source and target 
   objects and the other fields defined in the structure RelMDDIntersection:
   no_rows is the size of row of the Matrix
   no_cols is the size of col of the Matrix
   followed by the source and target object 
   then finally the first matrix and second matrix respectively 
 **************************************************************************************************************/
RelMDDIntersectionPtr RelMDDintersectOperation(int no_rows, int no_cols,
					       char **source_Object,
					       char **target_object,
					       double *matrix1,
					       double *matrix2)
{
    
    int mrow1= no_rows;
    int mcol1 = no_cols;
    int move;
    holdMeet= (double **) calloc(mrow1 * mcol1*2, sizeof(double*));
    for(move=0;move< mrow1 * mcol1*2;move++){
    holdMeet[move] =(double *) calloc( mrow1 * mcol1*2, sizeof(double*));
    }
    RelMDDIntersectionPtr ret;
    ret =(RelMDDIntersectionPtr) malloc(sizeof(RelMDDIntersection) *mrow1 * mcol1*2);
    ret->relation = (double *) malloc(sizeof(double) * mrow1 * mcol1*2);
    ret->source_Objects =(char *) malloc(sizeof(char) * mrow1 * mcol1*2);
    ret->target_Objects =(char *) malloc(sizeof(char) * mrow1 * mcol1*2);
    ret->node = (DdNode *) malloc(sizeof(DdNode) *mrow1 * mcol1*2);
    DdNode *M1;
    DdNode *M2;
    DdNode *tmp;
    int move1;
    track_Inters = (no_cols * no_rows) - 1;
    interMat1 = matrix1;
    interMat2 = matrix2;
    interrol = no_rows;
    interrcol = no_cols;
    interSource = (char **) malloc(sizeof(char) * mrow1 * mcol1*2);
    interTarget = (char **) malloc(sizeof(char) * mrow1 * mcol1*2);
    for (move1 = 0; move1 < mrow1; move1++) {
	interSource[move1] = source_Object[move1];	//printf("\n..%s\t%d..\n",interSource[move1],move1); 
	ret[move1].source_Objects = source_Object[move1];
    }
     
      for (move1 = 0; move1 < mcol1; move1++) {
	interTarget[move1] = target_object[move1];	//printf("\n..%s\t%d..\n",interTarget[move1],move1); 
	ret[move1].target_Objects = target_object[move1];
    }
    
   
    // DD mamupulation
    M1 = RelMDD_RenameRelation(dd, no_rows, no_cols);
    Cudd_Ref(M1);
    M2 = RelMDD_RenameRelation(dd, no_rows, no_cols);
    iniforInters = RelMDD_ListSequence(RelMDD_RenameRelation(dd, no_rows, no_cols), no_rows, no_cols);
    tmp = Cudd_addApply(dd, Cudd_addIntersection, M1, M2);
    ret->node= tmp;
    
    int i, j,c =0; 
     for(i=0;i<mrow1;i++){
     
     for(j=0;j<mcol1;j++){
     
         ret->relation[c]= holdMeet[i][j];// return the relation here
         //printf("%f",holdMeet[i][j]);
         c++;
     }
     //printf("\n");
   }
    return ret;
}
/**************************************************************************************************************
   function: CUDD_VALUE_TYPE interrelation(CUDD_VALUE_TYPE F, CUDD_VALUE_TYPE G)
   This function does the meet operations by going into the file and selecting the complement of a given 
   element matching it with the source and target object
   it is called by Cudd_addIntersection to perform manipulations of DD.
   it returns a CUDD_VALUE_TYPE of the relation or index 
   It is an Internal fucntion.
 ************************************************************************************************************* */
CUDD_VALUE_TYPE interrelation(CUDD_VALUE_TYPE F, CUDD_VALUE_TYPE G)
{
    int c;
    char *source_Object1[interrol];
    char *target_Object2[interrcol];
    for (c = 0; c < interrol; c++) {
	source_Object1[c] = interSource[c];
    }
    for (c = 0; c < interrcol; c++) {
	target_Object2[c] = interTarget[c];
    }
    int listob1 = (sizeof(source_Object1) / sizeof(char)) / 4;
    int listob2 = (sizeof(target_Object2) / sizeof(char)) / 4;
    int state[listob1][listob2];	// malloc here
    CUDD_VALUE_TYPE value=0;
    intersectionPtr ret;
    ret = (intersectionPtr) interIni();
    int size_struct = ret->len;
    int  l, j;
    int rn=0, cn=0;
    static int i, k;
    int yy = 1;
    for (i = 0; i < listob1; i++) {
	for (k = 0; k < listob2; k++) {
	    state[i][k] = yy;
	    yy++;
	  //  printf("%d",state[i][k]);
	}
    }
    
    for (i = 0; i < listob1; i++) {

	for (k = 0; k < listob2; k++) {
	    if (state[i][k] == iniforInters[track_Inters]) {
		//printf("track%d\n", track_Inters);
		//  printf("\n%d\t%d\n", i,k);
		rn = i;
		cn = k;		//
		//  printf("\ndd%fdd\n",result_Union[i]);
		 //printf("\n%s\t%s\n ", source_Object1[rn], target_Object2[cn]);
	    }

	}

    }

    for (j = 0; j < size_struct; j++) {

	if (!(strcmp(source_Object1[rn], *ret[j].source))
	    && !(strcmp(target_Object2[cn], *ret[j].target))) {
	   // printf("\n%s\t%s\n ", *ret[j].source, *ret[j].target);}}
	    
	    // printf("\n%f\t%f\t%f\n" ,F,G, value);
	    for (l = 0; l < ret->len_Content[j]; l++) {
		int ma = (int) (F - 1);
		int ka = (int) (G - 1);
		if (l % 3 == 0) {
		    if ((ret[j].content[l] == interMat1[ma])
			&& (ret[j].content[l + 1] == interMat2[ka])) {
			holdMeet[rn][cn] = ret[j].content[l + 2];
			//printf("\n%f\t%f\t%f\n", F, G, valufe);
			value = F;
			track_Inters--;

		    }

		}

	    }

	}
    }
    return value;
}
/* *************************************************************************************************************
   function: DdNode *Cudd_addIntersection(DdManager * dd, DdNode ** f, DdNode ** g)
   It does the ADD manipulation of meet operation. It is called by Cudd_addApply(); to compute the 
   the meet of a relation.
   It returns the meet of an element from the file
   It is an Internal fucntion.
**************************************************************************************************************/
DdNode *Cudd_addIntersection(DdManager * dd, DdNode ** f, DdNode ** g)
{
    DdNode *res;
    DdNode *F, *G;
    CUDD_VALUE_TYPE value=0;
    F = *f;
    G = *g;
    //if ((F == DD_ZERO(dd))||(G == DD_ZERO(dd))) return(DD_ZERO(dd));
    // if (G == DD_ZERO(dd)) return(F);
    if ((F == bck) || (G == bck)) {
	return (bck);
    }

    else {
	if (cuddIsConstant(F) && cuddIsConstant(G)) {
	   value = interrelation(cuddV(F), cuddV(G));
	    res = cuddUniqueConst(dd, value);
	    return (res);
	}
    }
    if (F > G) {		/* swap f and g */
	*f = G;
	*g = F;
    }
    return (NULL);

}				/* end of Cudd_addPlus */
/* *************************************************************************************************************
   function:RelMDDUnionPtr RelMDDUnionOperation(int no_rows, int no_cols, char **ssrelation, char **ttrelation, double *matrix1,
					       double *matrix2)
   Initializes the variables for the join  operations. This is an external functions. It calls other functions 
   and uses them to compute the join of two given relation or matrices. It returns the list of relations, the source and target 
   objects and the other fields defined in the structure RelMDDUnion:
   no_rows is the size of row of the Matrix
   no_cols is the size of col of the Matrix
   followed by the source and target object 
   then finally the first matrix and second matrix respectively
**************************************************************************************************************/
RelMDDUnionPtr RelMDDUnionOperation(int no_rows, int no_cols,
				   char **source_Object , char **target_object,
				    double *matrix1, double *matrix2)
{
    int unrow = no_rows;
    int uncol = no_cols;
  //  holdJoin = (double *) malloc(sizeof(double) * unrow*uncol*2);
    int move;
    holdJoin= (double **) calloc(unrow * uncol*2, sizeof(double*));
    for(move=0;move< unrow * uncol*2;move++){
    holdJoin[move] =(double *) calloc( unrow * uncol*2, sizeof(double*));
    }
    RelMDDUnionPtr ret;

    ret =
	(RelMDDUnionPtr) malloc(sizeof(RelMDDUnionPtr) * unrow *uncol *2);
    ret->relation = (double *) malloc(sizeof(double) * unrow*uncol*2);
    ret->source_Objects =
	(char *) malloc(sizeof(char) * unrow*uncol);
    ret->target_Objects =
	(char *) malloc(sizeof(char) * unrow*uncol);
    ret->node = (DdNode *) malloc(sizeof(DdNode) *unrow*uncol*2);
    int move1;
    track_union = (unrow*uncol) - 1;
    unionMat1 = matrix1;
    UnionMat2 = matrix2;
    urol = no_rows;
    ucol = no_cols;
   // track_union = (no_rows * no_cols) - 1;
    unionSource = (char **) malloc(sizeof(char) * unrow*uncol);
    unionTarget = (char **) malloc(sizeof(char) * unrow*uncol);
    for (move1 = 0; move1 < unrow; move1++) {
	unionSource[move1] = source_Object[move1];	//printf("%s\t%d\n",unionSource[move1],move1);
	ret[move1].source_Objects = source_Object[move1];	// printf ("%s\t%d\n", ret[move1].source_Objects,move1);

    }
    for (move1 = 0; move1 < uncol; move1++) {
	unionTarget[move1] = target_object[move1];	//printf("%s\t%d\n",unionTarget[move1],move1);
	ret[move1].target_Objects = target_object[move1];	// printf ("%s\t%d\n", ret[move].target_Objects,move);

    }

    DdNode *M1;
    DdNode *M2;
    DdNode *tmp;
    M1 = RelMDD_RenameRelation(dd, unrow, uncol);
    Cudd_Ref(M1);
    //Cudd_RecursiveDeref(manager,f);
    M2 = RelMDD_RenameRelation(dd,  unrow, uncol);
    iniforUnion =
	RelMDD_ListSequence(RelMDD_RenameRelation(dd,  unrow, uncol),
			     unrow, uncol);
    tmp = Cudd_addApply(dd, Cudd_addUnion_Meet, M1, M2);
    ret->node = tmp;
    
     int i, j,c =0; 
     for(i=0;i<unrow;i++){
     
     for(j=0;j<uncol;j++){
     
         ret->relation[c]= holdJoin[i][j];// return the relation here
       // printf("%f",ret->relation[c]);
         c++;
     }
     	} 
    
     if (ret == NULL) {
	printf("\n out of memory\n");
	return NULL;

    }
    return ret;
}/*End RelMDDUnionOperation */


/* *************************************************************************************************************
   function: CUDD_VALUE_TYPE unrelation(CUDD_VALUE_TYPE F, CUDD_VALUE_TYPE G)
   This function does the join operations by going into the file and selecting the join of a given 
   element matching it with the source and target object
   it is called by DdNode *Cudd_addUnion_Meet(DdManager * dd, DdNode ** f, DdNode ** g) to perform manipulations of DD.
   it returns a CUDD_VALUE_TYPE of the relation or index 
   It is an Internal fucntion.
 *************************************************************************************************************/
CUDD_VALUE_TYPE unrelation(CUDD_VALUE_TYPE F, CUDD_VALUE_TYPE G)
{
    int c;
    char *source_Object1[urol];
    char *target_Object2[ucol];
    for (c = 0; c < urol; c++) {

	source_Object1[c] = unionSource[c];

    }
    for (c = 0; c < ucol; c++) {

	target_Object2[c] = unionTarget[c];

    }
   
    int listob1 = (sizeof(source_Object1) / sizeof(char)) / 4;
    int listob2 = (sizeof(target_Object2) / sizeof(char)) / 4;
    int ste[listob1][listob2];
    CUDD_VALUE_TYPE value=0;
    unionsPtr ret;
    ret = (unionsPtr)unionIni();
    int size_struct = ret->len;
    int  l, j;
    int rn=0, cn=0;
    static int i, k;
    int yy = 1;
    for (i = 0; i < listob1; i++) {

	for (k = 0; k < listob2; k++) {

	    ste[i][k] = yy;
	    yy++;
	    //printf("%d",ste[i][k]);
	}
    }

    for (i = 0; i < listob1; i++) {

	for (k = 0; k < listob2; k++) {

	    //  if(ste[i][k]==trackt[track]){
	    if (ste[i][k] == iniforUnion[track_union]) {
		//printf("track%d\n", track_union);
		//  printf("\n%d\t%d\n", i,k);
		rn = i;
		cn = k;		//
		
	    }

	}
    }

    for (j = 0; j < size_struct; j++) {

	if (!(strcmp(source_Object1[rn], *ret[j].source))
	    && !(strcmp(target_Object2[cn], *ret[j].target))) {
	   // printf("\n%s\t%s\n ", *ret[j].source, *ret[j].target);
	    // printf("\n%f\t%f\t%f\n" ,F,G, value);
	    for (l = 0; l < ret->len_Content[j]; l++) {
		int ma = (int) (F - 1);
		int ka = (int) (G - 1);
		if (l % 3 == 0) {
		    if ((ret[j].content[l] == unionMat1[ma])
			&& (ret[j].content[l + 1] == UnionMat2[ka])) {
			holdJoin[rn][cn] = ret[j].content[l + 2];
			value = F;
			track_union--;

		    }

		}

	    }

	}
    }
    return value;
}
/* *************************************************************************************************************
   function:DdNode *Cudd_addUnion_Meet(DdManager * dd, DdNode ** f, DdNode ** g)
   It does the ADD manipulation of meet operation. It is called by Cudd_addApply(); to compute the 
   the join of a relation.
   It returns the join of an element from the file
   It is an Internal fucntion.
************************************************************************************************************* */
DdNode *Cudd_addUnion_Meet(DdManager * dd, DdNode ** f, DdNode ** g)
{
    DdNode *res;
    DdNode *F, *G;
    CUDD_VALUE_TYPE value=0;
    F = *f;
    G = *g;
    if ((F == bck) || (G == bck)) {
	return (bck);
    } else {
	if (cuddIsConstant(F) && cuddIsConstant(G)) {
	    value = unrelation(cuddV(F), cuddV(G));
	    res = cuddUniqueConst(dd, value);
	    return (res);
	}
    }
    if (F > G) {		/* swap f and g */
	*f = G;
	*g = F;
    }
    return (NULL);

}				/* end of Cudd_addPlus */
/**************************************************************************************************************
   function:DdNode *DdNode *RelMDD_RenameRelation(DdManager * dd, int roww, int collh)
   This function renames relation since for instance 2 with objects A to B is not the same as 2 with object B to A
   Internal
 **************************************************************************************************************/

DdNode *RelMDD_RenameRelation(DdManager * dd, int roww, int collh)
{
    DdNode *E;
    DdNode **x;
    DdNode **y;
    DdNode **xn;
    DdNode **yn_;
    int nx;
    int ny;
    int maxnx;
    int maxny;
    int m;
    int n;
    int ok;
    int ll;
    int f, j, ww = 0;

    // dd = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);
    int rows[roww * collh];	//malloc
    int cols[roww * collh];	//malloc  
    double id_relation[roww * collh];
     double count = 1;
    //int *colum = coll_id( roww);
    //int *rows=  rol_id(coll);  
    //int uu [] ={0,0,1,1,2,2};// write methods for this
    //int vv [] ={0,1,0,1,0,1};// write methods for this    
    for (f = 0; f < roww; f++) {
	for (j = 0; j < collh; j++) {
	    //printf("\n%d%d\n",f,j);
	    rows[ww] = f;
	    cols[ww] = j;
	    //printf("\n%d%d\n",rows[ww],cols[ww]);
	    ww++;
	}

    }
    for (ll = 0; ll < roww * collh; ll++) {
	id_relation[ll] = count;	//printf(" %f",id_relation[ll]);  
	count = count + 1.00;

    }
    x = y = xn = yn_ = NULL;
    maxnx = maxny = 0;
    nx = maxnx;
    ny = maxny;
    ok = RelMdd_CreateRelation(dd, &E, &x, &y, &xn, &yn_, &nx, &ny, &m, &n, 0, 2, 1, 2, roww, collh, rows, cols, id_relation);	//read matrix 1
     // Cudd_PrintDebug(dd,E,nx + ny,40); //Cudd_RecursiveDeref(dd, E);
     if(ok)
    no_vars = nx;
    // node_Complement( E);
    return E;
}

int *RelMDD_ListSequence(DdNode * E, int roww, int coll)
{
    // dd = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);
    ini = (int *) malloc(sizeof(int) * roww * coll);	// check here for memory leak
    int *cube;
    int t = 0;
    CUDD_VALUE_TYPE value;
    DdGen *gen;
    int q;
    //printf("Testing iterator on cubes:\n");
    Cudd_ForeachCube(dd, E, gen, cube, value) {
	for (q = 0; q < no_vars; q++) {
	}
	//(void) printf(" %g\t",value);
	// trackt[t] = value;
	ini[t] = value;
	t++;
    }
    return ini;
}
// to swap DD variables for converse
DdNode *RelMDD_SwapVariables(DdManager * dd, int roww, int collh)
{
    DdNode *E;
    DdNode **x;
    DdNode **y;
    DdNode **xn;
    DdNode **yn_;
    int nx;
    int ny;
    int maxnx;
    int maxny;
    int m;
    int n;
    int ll;
    int f, j, ww = 0;
    int cont=0;
    // dd = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);
    int rows[roww * collh];	//malloc
    int cols[roww * collh];	//malloc  
    double id_relation[roww * collh];
    int lll = 1;    
    for (f = 0; f < roww; f++) {
	for (j = 0; j < collh; j++) {
	    //printf("\n%d%d\n",f,j);
	    rows[ww] = f;
	    cols[ww] = j;
	    //printf("\n%d%d\n",rows[ww],cols[ww]);
	    ww++;
	}

    }
    for (ll = 0; ll < roww * collh; ll++) {
	id_relation[ll] = lll;	//printf(" %f",id_relation[ll]);  
	lll = lll + 1;

    }
    x = y = xn = yn_ = NULL;
    maxnx = maxny = 0;
    nx = maxnx;
    ny = maxny;
   RelMdd_CreateRelation(dd, &E, &x, &y, &xn, &yn_, &nx, &ny, &m, &n, 0, 2, 1, 2, roww, collh, rows, cols, id_relation);	//read matrix 1
    // Cudd_PrintDebug(dd,E,nx + ny,40); 
     //Cudd_RecursiveDeref(dd, E);
   // if(ok)
    
    if ( nx>ny){
    cont=ny;
    no_vars = ny;
    }
    
    else { 
    cont = nx;
    no_vars = nx;
    }
    
    DdNode *P = Cudd_addSwapVariables(dd, E, x, y, cont);
  //  Cudd_PrintDebug(dd,P,nx + ny,40);
    return P;
}


/* *************************************************************************************************************
   function:RelMDDIdentityRelation(int row, int col, char **object)
   Initializes the variables for the for creating the identity relation. This is an external functions. It calls      other functions  and uses them to create the identity relation based on the object provided. It returns the list of relations, the objectd  and the other fields defined in the structure RelMDDIdentityPtr:
   row is the size of row of the Matrix
   col is the size of column of the Matrix
   followed by list of object 
   
 *************************************************************************************************************/
RelMDDIdentityPtr RelMDDIdentityRelation(int row, int col, char **object)
{  
    int move;
    holdId = (double **) calloc(row*col*2, sizeof(double*));
    for(move=0;move<row*col*2;move++){
    holdId[move] =(double *) calloc(row*col*2, sizeof(double*));
    }
    RelMDDIdentityPtr ret;
    ret =(RelMDDIdentityPtr) malloc(sizeof(RelMDDIdentityPtr) * row*col);
    ret->relation = (double *) malloc(sizeof(double) * row*col*2);
    ret->object =(char *) malloc(sizeof(char) * row*col*2);
    ret->node = (DdNode *) malloc(sizeof(DdNode) * row*col);
    id_row = row;
    id_col = col;
    id_id_track = (row*col) - 1;
    idSource = (char **) malloc(sizeof(char) * row*col);
    idTarget = (char **) malloc(sizeof(char) * row*col);
    for (move = 0; move<row; move++) {
	idSource[move] = object[move];
	//printf("p%s\t%dp\n", idSource[move], move);
    }
    for (move = 0; move < col; move++) {
	idTarget[move] = object[move];
	ret[move].object = object[move];
	//printf("p%s\t%dp\n", idTarget[move], move);
    }
    int i, j,y=0;

    inifor_id = RelMDD_ListSequence(RelMDD_RenameRelation(dd, id_row, id_col), id_row,id_col);
    ret->node=RelMDD_RenameRelation(dd, id_row, id_col);
    for (i = 0; i < row*col; i++) {
	identityRelation(inifor_id[i]);
    }
    for (j = 0; j < row; j++) {
	for (i = 0; i < col; i++) {
	    //printf(">>%f>>\t", holdId[i][j]);
	    ret->relation[y] =holdId[i][j];
	    y++;
	}
	//printf("\n");
    }
    return ret;
}/*End of RelMDDIdentityRelation(int row, int col, char **object)*/

/* *************************************************************************************************************
   function:CUDD_VALUE_TYPE identityRelation(CUDD_VALUE_TYPE F)
   creates the indentity relation 
 *************************************************************************************************************/
CUDD_VALUE_TYPE identityRelation(CUDD_VALUE_TYPE F)
{

    int c;
    char *source_Object1[id_row];
    char *target_Object2[id_col];

    for (c = 0; c < id_row; c++) {
	source_Object1[c] = idSource[c];
    }
    for (c = 0; c < id_col; c++) {
	target_Object2[c] = idTarget[c];
    }
    int listob1 = (sizeof(source_Object1) / sizeof(char)) / 4;
    int listob2 = (sizeof(target_Object2) / sizeof(char)) / 4;
    CUDD_VALUE_TYPE value=0;
    int ste[listob1][listob2];
    identityPtr ret;
    ret = (identityPtr) idet();
    bottomPtr het;
    het = (bottomPtr) bott();
    int size_bot = het->len;
    int size_struct = ret->len;
    int j;
    int rn=0, cn=0;
    static int i, k;
    int yy = 1;
    for (i = 0; i < listob1; i++) {

	for (k = 0; k < listob2; k++) {

	    ste[i][k] = yy;
	    yy++;
	    //printf("%d",ste[i][k]);
	}
    }

    for (i = 0; i < listob1; i++) {

	for (k = 0; k < listob2; k++) {

	    //  if(ste[i][k]==id_id_trackt[id_id_track]){
	    if (ste[i][k] == inifor_id[id_id_track]) {
		//printf("id_id_track%d%d\n", id_id_track,
		     //  inifor_id[id_id_track]);
		//printf("\n%d\t%d\n", i, k);
		rn = i;
		cn = k;		//
	    }

	}
    }
//if (!(strcmp(source_Object1[rn],target_Object2[cn] )))
    if (rn == cn) 
     {
	for (j = 0; j < size_struct; j++) {

	    if (!(strcmp(source_Object1[cn], *ret[j].object))) {
		//printf("id \t%s \t %f\n ", *ret[j].object, ret->rel[j]);
	         holdId[rn][cn] = ret->rel[j];
	  //       printf(".........%f\t\n", holdId[cn][rn]);
	    }

	}
    }

    else {
	for (j = 0; j < size_bot; j++) {
	    if (!(strcmp(source_Object1[rn], *het[j].source))
		&& !(strcmp(target_Object2[cn], *het[j].target))) {
		//printf(".........%s\t%s\n", *het[j].source,
		    //   *het[j].target);
		// rel_list1[l] = (double)cur->rel[j];
		//perror("gome");
		
		holdId[cn][rn] = het->rel[j];	//check the assignment for cn and rn 
		// l++;
		//printf(".........%f\t\n", holdId[cn][rn]);

	    }
	}
    }
    id_id_track--;

    return value;
}

/* *************************************************************************************************************
   functionRelMDDBottomPtr RelMDDBottomRelation(int row, int col, char **source,
			  char **target)
   Initializes the variables for the for creating the Bottom relation. This is an external functions. It calls      other functions  and uses them to create the bottom relation based on the object provided. It returns the list of relations, the objectd  and the other fields defined in the structure RelMDDBottomPtr:
    row is the size of row of the Matrix
   col is the size of column of the Matrix
   followed by list of source and target objects respectively 
 **************************************************************************************************************/
RelMDDBottomPtr RelMDDBottomRelation(int row, int col, char **source,
			  char **target)
{
    int move;
    holdBottom = (double **) calloc(row*col*2, sizeof(double*));
    for(move=0;move<row*col*2;move++){
    holdBottom[move] =(double *) calloc(row*col*2, sizeof(double*));
    }
    RelMDDBottomPtr ret;
    ret =(RelMDDBottomPtr) malloc(sizeof(RelMDDBottomPtr) * row*col);
    ret->relation = (double *) malloc(sizeof(double) * row*col*2);
    ret->source_Objects =(char *) malloc(sizeof(char) * row*col*2);
    ret->target_Objects =(char *) malloc(sizeof(char) * row*col*2);
    ret->node = (DdNode *) malloc(sizeof(DdNode) * row*col);
    Bottom_row = row;
    Bottom_col = col;
    Bottom_track = (row * col) - 1;

    BottomSource = (char **) malloc(sizeof(char) *row * col);
    BottomTarget = (char **) malloc(sizeof(char) * row * col);
    for (move = 0; move < Bottom_row; move++) {

	BottomSource[move] = source[move];
	ret[move].source_Objects=source[move];
	//printf("p%s\t%dp\n", BottomSource[move], move);

    }

    for (move = 0; move < Bottom_col; move++) {

	BottomTarget[move] = target[move];
	ret[move].target_Objects=target[move];
	//printf("p%s\t%dp\n", BottomTarget[move], move);

    }
    int i, j,u=0;

    inifor_Bottom =
	RelMDD_ListSequence(RelMDD_RenameRelation(dd, Bottom_row, Bottom_col),Bottom_row, Bottom_col);
   // for (i = 0; i < row * col; i++) {
	//printf("%d\t", inifor_Bottom[i]);

   // }
    for (i = 0; i < row * col; i++) {
	BottomRelation(inifor_Bottom[i]);
    }
    ret->node= RelMDD_RenameRelation(dd, Bottom_row, Bottom_col);
    for (j = 0; j < Bottom_row; j++) {
	for (i = 0; i < Bottom_col; i++) {
	    ret->relation[u]=holdBottom[i][j];
	    u++;
	}
	//printf("\n");
    }
    return ret;
}
/* *************************************************************************************************************
   function:CUDD_VALUE_TYPE BottomRelation(CUDD_VALUE_TYPE F)
   creates the the Bottom relation relation: returns the a value from the xml file 
   internal file
 **************************************************************************************************************/

CUDD_VALUE_TYPE BottomRelation(CUDD_VALUE_TYPE F)
{

    int c;
    char *source_Object1[Bottom_row];
    char *target_Object2[Bottom_col];

    for (c = 0; c < Bottom_row; c++) {

	source_Object1[c] = BottomSource[c];

    }
    for (c = 0; c < Bottom_col; c++) {

	target_Object2[c] = BottomTarget[c];

    }

    int listob1 = (sizeof(source_Object1) / sizeof(char)) / 4;
    int listob2 = (sizeof(target_Object2) / sizeof(char)) / 4;

    CUDD_VALUE_TYPE value=0;
    int ste[listob1][listob2];
    bottomPtr het;
    het = (bottomPtr) bott();
    int size_bot = het->len;
    int  j;
    int rn=0, cn=0;
    static int i, k;
    int yy = 1;
    for (i = 0; i < listob1; i++) {

	for (k = 0; k < listob2; k++) {

	    ste[i][k] = yy;
	    yy++;
	    //printf("%d",ste[i][k]);
	}
    }

    for (i = 0; i < listob1; i++) {

	for (k = 0; k < listob2; k++) {

	    //  if(ste[i][k]==Bottom_Bottom_trackt[Bottom_Bottom_track]){
	    if (ste[i][k] == inifor_Bottom[Bottom_track]) {
		//printf("Bottom_Bottom_track%d%d\n", Bottom_track,
		//       inifor_Bottom[Bottom_track]);
		//printf("\n%d\t%d\n", i, k);
		rn = i;
		cn = k;		//
	    }

	}
    }

    /*  if (rn == cn) {
       for (j = 0; j < size_struct; j++) {

       if (!(strcmp(source_Object1[cn], *ret[j].object))) {

       printf("Bottom \t%s \t %f\n ", *ret[j].object,
       ret->rel[j]);
       Bottom[rn][rn] = ret->rel[j];
       }

       }
       }
     */
    // else {
    for (j = 0; j < size_bot; j++) {
	if (!(strcmp(source_Object1[rn], *het[j].source))
	    && !(strcmp(target_Object2[cn], *het[j].target))) {
	   // printf(".........%s\t%s\n", *het[j].source, *het[j].target);
	    // rel_list1[l] = (double)cur->rel[j];
	    //perror("gome");
	   // printf(".........%f\t\n", het->rel[j]);
	    holdBottom[cn][rn] = het->rel[j];	//check the assignment for cn and rn 
	    // l++;

	    // }
	}
    }
    Bottom_track--;

    return value;
}

/* *************************************************************************************************************
   function: RelMDDBottomPtr RelMDDTopRelation(int row, int col, char **source,
		       char **target)
   Initializes the variables for the for creating the top relation. This is an external functions. It calls      other functions  and uses them to create the top relation based on the object provided. It returns the list of relations, the object  and the other field members defined in the structure RelMDDTop:
    row is the size of row of the Matrix
   col is the size of column of the Matrix
   followed by list of source and target objects respectively
**************************************************************************************************************/
RelMDDTopPtr RelMDDTopRelation(int row, int col, char **source,
		       char **target)
{   
    int move;
    holdTop= (double **) calloc(row*col*2, sizeof(double*));
    for(move=0;move<row*col*2;move++){
    holdTop[move] =(double *) calloc(row*col*2, sizeof(double*));
    }
    RelMDDTopPtr ret;
    ret =(RelMDDTopPtr) malloc(sizeof(RelMDDTopPtr) * row*col);
    ret->relation = (double *) malloc(sizeof(double) * row*col*2);
    ret->source_Objects =(char *) malloc(sizeof(char) * row*col*2);
    ret->target_Objects =(char *) malloc(sizeof(char) * row*col*2);
    ret->node = (DdNode *) malloc(sizeof(DdNode) * row*col);
    Top_row = row;
    Top_col = col;
    Top_track = ( row*col) - 1;
    topSource = (char **) malloc(sizeof(char) *  row*col);
    topTarget = (char **) malloc(sizeof(char) * row*col);
    for (move = 0; move <  row; move++) {

	topSource[move] = source[move];
	ret[move].source_Objects=source[move];
	//printf("p%s\t%dp\n", topSource[move], move);

    }

    for (move = 0; move <  col; move++) {

	topTarget[move] = target[move];
	ret[move].target_Objects=target[move];
	//printf("p%s\t%dp\n", topTarget[move], move);

    }
    int i, j,l=0;

    inifor_Top =
	RelMDD_ListSequence(RelMDD_RenameRelation(dd,Top_row , Top_col ),Top_row , Top_col);
    for (i = 0; i < row*col; i++) {
	topRelation(inifor_Top[i]);
    }
     ret->node=RelMDD_RenameRelation(dd,Top_row , Top_col );
    for (j = 0; j < row; j++) {
	for (i = 0; i < col; i++) {
	  ret->relation[l]=holdTop[i][j];
	  l++;
	}
	
    }
    return ret;
}

/* *************************************************************************************************************
   function: CUDD_VALUE_TYPE topRelation(CUDD_VALUE_TYPE F)
   creates the the top relation relation: returns the a value from the xml file 
   internal file
 **************************************************************************************************************/


CUDD_VALUE_TYPE topRelation(CUDD_VALUE_TYPE F)
{

    int c;
    char *source_Object1[Top_row];
    char *target_Object2[Top_col];

    for (c = 0; c < Top_row; c++) {

	source_Object1[c] = topSource[c];

    }
    for (c = 0; c < Top_col; c++) {

	target_Object2[c] = topTarget[c];

    }

    int listob1 = (sizeof(source_Object1) / sizeof(char)) / 4;
    int listob2 = (sizeof(target_Object2) / sizeof(char)) / 4;

    CUDD_VALUE_TYPE value=0;
    int ste[listob1][listob2];
    topPtr het;
    het = (topPtr) topp();
    int size_bot = het->len;
    int  j;
    int rn=0, cn=0;
    static int i, k;
    int yy = 1;
    for (i = 0; i < listob1; i++) {

	for (k = 0; k < listob2; k++) {

	    ste[i][k] = yy;
	    yy++;
	    //printf("%d",ste[i][k]);
	}
    }

    for (i = 0; i < listob1; i++) {

	for (k = 0; k < listob2; k++) {

	    //  if(ste[i][k]==Top_Top_trackt[Top_Top_track]){
	    if (ste[i][k] == inifor_Top[Top_track]) {
		//printf("Top_Top_track%d%d\n", Top_track,
		     //  inifor_Top[Top_track]);
		//printf("\n%d\t%d\n", i, k);
		rn = i;
		cn = k;		//
	    }

	}
    }

    /*    if(rn==cn) { 
       for(j = 0; j<size_struct; j++){

       if( !(strcmp(source_Object1[cn], *ret[j].object  ))){

       printf("Top \t%s \t %f\n ",*ret[j].object, ret->rel[j]);
       toppp[rn][rn]=ret->rel[j];
       }

       }}
     */
//                             else{
    for (j = 0; j < size_bot; j++) {
	if (!(strcmp(source_Object1[rn], *het[j].source))
	    && !(strcmp(target_Object2[cn], *het[j].target))) {
	   // printf(".........%s\t%s\n", *het[j].source, *het[j].target);
	    // rel_list1[l] = (double)cur->rel[j];
	    //perror("gome");
	   // printf(".........%f\t\n", het->rel[j]);
	    holdTop[cn][rn] = het->rel[j];	//check the assignment for cn and rn 
	    // l++;

	}
    }
//}
    Top_track--;

    return value;
}



DdNode *RelMDD_CreateCompostionVariables(DdManager * dd, int roww, int collh, int r, int c)
{
    DdNode *E;
    DdNode **x;
    DdNode **y;
    DdNode **y1;
    DdNode **xn;
    DdNode **xn1;
    DdNode **yn_;
    DdNode **yn_1;
    int nx;
    int ny;
    int maxnx;
    int maxny;
    int m;
    int n;
    int ll;
    int f, j, ww = 0;
    // dd = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);
    int rows[roww * collh];	//malloc
    int cols[roww * collh];	//malloc  
    double id_relation[roww * collh];
     double count = 1;
    //int *colum = coll_id( roww);
    //int *rows=  rol_id(coll);  
    //int uu [] ={0,0,1,1,2,2};// write methods for this
    //int vv [] ={0,1,0,1,0,1};// write methods for this    
    for (f = 0; f < roww; f++) {
	for (j = 0; j < collh; j++) {
	    //printf("\n%d%d\n",f,j);
	    rows[ww] = f;
	    cols[ww] = j;
	    //printf("\n%d%d\n",rows[ww],cols[ww]);
	    ww++;
	}

    }
    for (ll = 0; ll < roww * collh; ll++) {
	id_relation[ll] = count; 
	count = count + 1.00;

    }
    x = y = xn = yn_ = y1 = xn1 = yn_1=  NULL;
    maxnx = maxny = 0;
    nx = maxnx;
    ny = maxny;
   RelMdd_CreateRelation(dd, &E, &x, &y, &xn, &yn_, &nx, &ny, &m, &n, r, roww, c, collh, roww, collh, rows, cols, id_relation);
   summation_var = y;
   // Cudd_RecursiveDeref(dd, E);
 // ok = RelMdd_CreateRelation(dd, &F, &y, &y1, &xn1, &yn_1, &nx1, &ny1, &m1, &n1, 0, 4, 3, 4, roww, collh, rows, cols, cant);
   //Cudd_PrintDebug(dd,E,nx + ny,40); 
   //Cudd_PrintDebug(dd,F,nx1 + ny1,40); 
     //
   //  if(ok)
    cno_vars = nx;
    // node_Complement( E);
    return E;
}


double  **multiplication(int row, int col){
DdNode *M1;
DdNode *M2;
DdNode *nor;
M1= RelMDD_CreateCompostionVariables(dd,row,col,1,0);
DdNode **yy= summation_var;
M2= RelMDD_CreateCompostionVariables(dd,row,col,0,row-1);
nor=Cudd_addMatrixComposition(dd,M1,M2,yy,cno_vars);
//Cudd_PrintDebug(dd,nor,10,40);
int *list = RelMDD_ListSequence(nor,row, col);
tempNode =nor;
int i,r,c; double rel;
     
for(i=1;i<row*col;i++){

r=row_trackComp[list[i]];
c= col_trackComp[list[i]];
rel= element_comp[list[i]];
value_comp[r][c]=rel;
//printf("<<%f>>",value_comp[r][c]);

}

free(row_trackComp);
free(col_trackComp);
free(element_comp);
return value_comp;

}

/* ........................................................................................................... 
   function:RelMDDCompositionPtr RelMDDCompositionOperation(int no_rowofMat1, int no_colsofMat1, int no_colsofMat2, char **source_Object, char **intermidiate_Object, char **target_Object2, double *matrix1, double *matrix2)
   Initializes the variables for the compostion operations. This is an external functions. It calls other functions 
   and uses them to compute the join of two given relation or matrices. It returns the list of relations, the source and target 
   objects and the other fields defined in the structure RelMDDComposition:
   no_rowofMat1 is the size of row of the first Matrix
   no_colsofMat1, is the size of col of first the Matrix
   no_colsofMat2, is the size of col of second the Matrix
   followed by the source and target object first matrix
   followed by the target object second matrix
   then finally the first matrix and second matrix respectively
 ........................................................................................................... */
RelMDDCompositionPtr RelMDDCompositionOperation(int no_rowofMat1, int no_colsofMat1, int no_colsofMat2, char **source_Object, char **intermidiate_Object, char **target_Object2, double *matrix1, double *matrix2){
 element_comp = (double *) malloc(sizeof(double) * no_rowofMat1*no_colsofMat1*MAXA);
 row_trackComp = (int  *) malloc(sizeof(int) * no_rowofMat1*no_colsofMat1*MAXA);
col_trackComp = (int *) malloc(sizeof(int) * no_rowofMat1*no_colsofMat1*MAXA);
RelMDDCompositionPtr ret;
    ret =(RelMDDCompositionPtr) malloc(sizeof(RelMDDComposition) * no_rowofMat1 * no_colsofMat2*2);
    ret->relation = (double *) malloc(sizeof(double) * no_rowofMat1 * no_colsofMat2*2);
    ret->source_Objects =(char *) malloc(sizeof(char) * no_rowofMat1 * no_colsofMat2*2);
    ret->target_Objects =(char *) malloc(sizeof(char) * no_rowofMat1 * no_colsofMat2*2);
    ret->node = (DdNode *) malloc(sizeof(DdNode) * no_rowofMat1 * no_colsofMat2*2);
int move;
    value_comp= (double **) calloc(no_rowofMat1*no_colsofMat1+1, sizeof(double*));
    for(move=0;move<no_rowofMat1*no_colsofMat1+1;move++){
    value_comp[move] =(double *) calloc(no_rowofMat1*no_colsofMat1+1, sizeof(double*));
    }
    for (move = 0; move < no_rowofMat1; move++) {
	ret[move].source_Objects = source_Object[move];
    }
    for (move = 0; move <no_colsofMat2; move++) {
	ret[move].target_Objects =target_Object2[move];
    }
mat1= matrix1;
mat2=matrix2;
source_comp= source_Object;
target_Comp= target_Object2;
row_comp=no_rowofMat1;
col_comp=no_colsofMat2;
double **temp=multiplication(no_rowofMat1 , no_colsofMat2);
 int i, j, l=0;
 for (j = 0; j < no_rowofMat1; j++) {
	for (i = 0; i < no_colsofMat2; i++) {
	  ret->relation[l]=temp[i][j];
	  l++;
	}
	
    }

 ret->node= tempNode;
 return ret;
}

/*End of File */
