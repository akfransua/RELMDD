#include "util.h"
#include "cuddInt.h"
	
extern DdManager * dd;
extern DdNode *bck;
unsigned int cacheSize;// maxMemory=0;
unsigned int nvars,nslots;

extern void RelMDD_Int();
extern void RelMDD_Quit();
extern DdNode *tempNode;

typedef struct RelMDDUnion{
 double *relation;
 DdNode *node;
 char *source_Objects; //for the list of source objects 
 char *target_Objects; // for the list of target objects
   
}RelMDDUnion, *RelMDDUnionPtr; 

typedef struct RelMDDIntersection{
 double *relation;
 DdNode *node;
 char *source_Objects; //for the list of source objects 
 char *target_Objects; // for the list of target objects
   
}RelMDDIntersection, *RelMDDIntersectionPtr;


typedef struct RelMDDComplement{
 double *relation;
 DdNode *node;
 char *source_Objects; //for the list of source objects 
 char *target_Objects; // for the list of target objects
   
}RelMDDComplement, *RelMDDComplementPtr; 


typedef struct RelMDDConverse{
 double *relation;
 DdNode *node;
 char *source_Objects; //for the list of source objects 
 char *target_Objects; // for the list of target objects
   
}RelMDDConverse, *RelMDDConversePtr;

typedef struct RelMDDIdentity{
 double *relation;
 DdNode *node;
 char *object; //for the list of source objects  
}RelMDDIdentity, *RelMDDIdentityPtr;


typedef struct RelMDDBottom{
 double *relation;
 DdNode *node;
 char *source_Objects; //for the list of source objects 
 char *target_Objects; // for the list of target objects
   
}RelMDDBottom, *RelMDDBottomPtr;

typedef struct RelMDDTop{
 double *relation;
 DdNode *node;
 char *source_Objects; //for the list of source objects 
 char *target_Objects; // for the list of target objects
   
}RelMDDTop, *RelMDDTopPtr;


typedef struct RelMDDComposition{
 double *relation;
 DdNode *node;
 char *source_Objects; //for the list of source objects 
 char *target_Objects; // for the list of target objects
   
}RelMDDComposition, *RelMDDCompositionPtr;


typedef struct fileInfor{
 double   *rel ;
 int r;
 int c;
 char *sourcelist;
 char  *targetlist;
 int *rowid;
 int *rowida;
}fileInfor, *fileInforPtr;
extern DdNode *Transpose(DdManager * dd,  DdNode * f);
extern CUDD_VALUE_TYPE  transposeRelation( CUDD_VALUE_TYPE F);
extern DdNode *RelMDD_SwapVariables(DdManager * dd  , int roww,  int collh);
extern DdNode *Complement(DdManager * dd, DdNode * f);
extern CUDD_VALUE_TYPE  complementRelation( CUDD_VALUE_TYPE F);
extern RelMDDComplementPtr RelMDDComplement_Operation(int toi, int gom, char **srelation , char **trelation, double *matrimx) ;
extern RelMDDConversePtr RelMDDConverse_Operation(int tra_rn, int tra_col, char **source_Object , char **target_object, double *matrimx) ; 
extern fileInforPtr RelMdd_ReadFile(char *filename);
extern double setSequence(double value);
extern DdNode *RelMDD_RenameRelation(DdManager * dd , int roww ,int collh );
extern CUDD_VALUE_TYPE  complementRelation( CUDD_VALUE_TYPE F);	
extern int  *RelMDD_ListSequence(DdNode  *E, int roww, int coll);
//static FILE *open_file (char *filename, const char *mode);
extern DdNode *Cudd_addUnion_Meet(DdManager * dd, DdNode ** f,DdNode ** g);
extern DdNode *Cudd_addIntersection(DdManager * dd,DdNode ** f,DdNode ** g);
extern RelMDDIntersectionPtr RelMDDintersectOperation(int no_rows, int no_cols, char **source_Object , char **target_object, double *matrix1, double *matrix2 );
extern RelMDDUnionPtr RelMDDUnionOperation(int no_rows, int no_cols, char **source_Object , char **target_object, double *matrix1, double *matrix2);
extern DdNode * node_Complement(DdNode * mat);
extern CUDD_VALUE_TYPE  unrelation( CUDD_VALUE_TYPE F, CUDD_VALUE_TYPE G);
extern int RelMdd_CreateRelation(
 DdManager * dd /* DD manager */,
  DdNode ** E /* characteristic function of the graph */,
  DdNode *** x /* array of row variables */,
  DdNode *** y /* array of column variables */,
  DdNode *** xn /* array of complemented row variables */,
  DdNode *** yn_ /* array of complemented column variables */,
  int * nx /* number or row variables */,
  int * ny /* number or column variables */,
  int * m /* number of rows */,
  int * n /* number of columns */,
  int  bx /* first index of row variables */,
  int  sx /* step of row variables */,
  int  by /* first index of column variables */,
  int  sy /* step of column variables */,
  int row_nn,/*num of rows of the list */
  int col_nn,/*num of column of the list */
  int *row_n,/*num of list of sequence  of the row*/
  int *col_n,/*num of list of sequence  of the colum */
  double *id_relation /*list of relation in Matrix form */
  );

extern CUDD_VALUE_TYPE identityRelation( CUDD_VALUE_TYPE F);
extern RelMDDIdentityPtr  RelMDDIdentityRelation(int row, int col, char **object);
extern RelMDDBottomPtr RelMDDBottomRelation(int row, int col, char **source,
			  char **target );
extern CUDD_VALUE_TYPE BottomRelation( CUDD_VALUE_TYPE F);
extern RelMDDTopPtr RelMDDTopRelation(int row, int col, char **source,
		       char **target);
extern CUDD_VALUE_TYPE topRelation(CUDD_VALUE_TYPE F);

extern DdNode *RelMDD_CreateCompostionVariables(DdManager * dd, int roww, int collh, int r, int c);
extern RelMDDCompositionPtr RelMDDCompositionOperation(int no_rowofMat1, int no_colsofMat1, int no_colsofMat2,char **source_Object, char **intermidiate_Object, char **target_Object2, double *matrix1, double *matrix2);
extern  double **multiplication(int row, int col);
void poo();
extern compoUnionPtr compoUionIni();
extern conversePtr coverselIni();
extern compoInterPtr compoInterIni();
extern complementPtr  complIni();
extern intersectionPtr interIni();
extern unionsPtr unionIni();
extern compoInterPtr test();
extern identityPtr idet();
extern bottomPtr bott();
extern topPtr topp();





