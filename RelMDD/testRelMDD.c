/*This is a test program for the RelMdd package......*/
#include "util.h"
#include "cuddInt.h"
#include "structures.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <time.h>
#include "RelMDD.h"
char *file1;
char *file2;
char *file3;

int main(int argc, char **argv)
{
    RelMDD_Int();
    fileInforPtr cur, cat;
    file1 = argv[1];		// first file fisrt matrix
    file2 = argv[2];		// second file second matrix
    file3 = argv[3];		// xmlfile
    cur = RelMdd_ReadFile(file1);	// set this to be ...
    cat = RelMdd_ReadFile(file2);
    int move;
    int ok;

    //validation of xml file
    ok = RelMDDValidateXmlFile(file3);	// validation of xml file
    if (!ok) {
	printf("\nvalidation success\n");
    } else {
	printf("Invalid xml file");
    }
    int i;

    char *source_Object1[cur->r];	// source sobjects, you can also use those read from the file
    char *target_Object2[cur->c];	// = { "A", "A", "B" }; // target objects
    for (i = 0; i < cur->r; i++) {

	source_Object1[i] = cur[i].sourcelist;
    }
    for (i = 0; i < cur->c; i++) {

	target_Object2[i] = cur[i].targetlist;
    }

// initialization of xml content  
    RelMDDParseXmlFile(file3);

    //testing Top relation
    printf("\n......................................\n");
    printf("\nTesting Top relation\n");
    RelMDDTopPtr ret;
    ret =
	RelMDDTopRelation(cur->r, cur->c, source_Object1, target_Object2);
    printf("\nsource \t target\n");
    for (move = 0; move < cur->r; move++) {

	printf("\n%s\t%s\n", ret[move].source_Objects,
	       ret[move].target_Objects);
    }

    printf("\nUniversal Relation \n");
    for (move = 0; move < cur->r * cur->c; move++) {
         if(move%4==0){printf("\n");}
	printf("%f\t", ret->relation[move]);
    }

//testing bottom relation
    printf("\n......................................\n");
    printf("\nTesting zero relation\n");
    RelMDDBottomPtr ret1;
    ret1 =
	RelMDDBottomRelation(cur->r, cur->c, source_Object1,
			     target_Object2);
    printf("\nsource \t target\n");
    for (move = 0; move < cur->r; move++) {

	printf("\n%s\t%s\n", ret1[move].source_Objects,
	       ret1[move].target_Objects);
    }

    printf("\nempty Relation \n");
    for (move = 0; move < cur->r * cur->c; move++) {
          if(move%4==0){printf("\n");}
	printf("%f\t", ret1->relation[move]);
    }

    //testing identity relation
    printf("\n......................................\n");
    printf("\nTesting identity relation\n");
    RelMDDIdentityPtr ret11 =
	RelMDDIdentityRelation(cur->r, cur->c, source_Object1);
    printf("\nobjects\n");
    for (move = 0; move < cur->r; move++) {

	printf("\n%s\n", ret11[move].object);
    }

    printf("\nindentity \n");
    for (move = 0; move < cur->r * cur->c; move++) {
       if(move%4==0){printf("\n");}
	printf("%f\t", ret11->relation[move]);
	
	
    }

//testing converse relaiton
    printf("\n......................................\n");
    printf("\nTesting converse relation\n");

    RelMDDConversePtr ret111;
    ret111 =
	RelMDDConverse_Operation(cur->r, cur->c, source_Object1,
				 target_Object2, cur->rel);
    printf("\nsource \t target\n");
    for (move = 0; move < cur->r; move++) {

	printf("\n%s\t%s\n", ret111[move].source_Objects,ret111[move].target_Objects); 
    }

    printf("\nconverse Relation \n");
    for (move = 0; move < cur->r * cur->c; move++) {
         if(move%4==0){printf("\n");}
	printf("%f\t", ret111->relation[move]);
    }

//testing complement relaiton
    printf("\n......................................\n");
    printf("\nTesting complement relation\n");
    RelMDDComplementPtr reta =
	RelMDDComplement_Operation(cur->r, cur->c, source_Object1,
				   target_Object2, cur->rel);
    printf("\nsource\t target\n");
    for (move = 0; move < cur->r; move++) {
        
	printf("\n%s\t%s\n", reta[move].source_Objects,
	       reta[move].target_Objects);
    }

    printf("\ncomplement Relation \n");
    for (move = 0; move < cur->r * cur->c; move++) {
         if(move%4==0){printf("\n");}
	printf("%f\t", reta->relation[move]);
    }

    //testing meet relaiton
    printf("\n......................................\n");
    printf("\nTesting meet relation\n");
    RelMDDIntersectionPtr reti =
	RelMDDintersectOperation(cur->r, cur->c, source_Object1,
				 target_Object2, cur->rel, cat->rel);
    printf("\nsource\t target\n");
    for (move = 0; move < cur->r; move++) {

	printf("\n%s\t%s\n", reti[move].source_Objects,
	       reti[move].target_Objects);
    }
    printf("\nMeet Relation \n");
    for (move = 0; move < cur->r * cur->c; move++) {
           if(move%4==0){printf("\n");}
	printf("%f\t", reti->relation[move]);
    }

    //testing meet relaiton
    printf("\n......................................\n");
    printf("\nTesting join relation\n");
    RelMDDUnionPtr retu =
	RelMDDUnionOperation(cur->r, cur->c, source_Object1,
			     target_Object2, cur->rel, cat->rel);
    for (move = 0; move < cur->r; move++) {

	printf("\n%s\t%s\n", reti[move].source_Objects,
	       reti[move].target_Objects);
    }
    printf("\njoin Relation \n");
    for (move = 0; move < cur->r * cur->c; move++) {
       if(move%4==0){printf("\n");}
	printf("%f\t", retu->relation[move]);
    }

    //testing compositoin relaiton
    printf("\n......................................\n");
    printf("\nTesting compositon relation\n");
    RelMDDCompositionPtr retr =
	RelMDDCompositionOperation(cur->r, cur->c, cat->c, source_Object1,
				   source_Object1, target_Object2,
				   cat->rel, cur->rel);
    for (move = 0; move < cur->r; move++) {
	printf("\n%s\t%s\n", reti[move].source_Objects,
	       reti[move].target_Objects);
    }

    printf("\ncompostion elements \n");
    for (move = 0; move < cur->r * cur->c; move++) {
         if(move%4==0){printf("\n");}
	printf("%f\t", retr->relation[move]);
    }

    RelMDD_Quit();
    return 0;
}
