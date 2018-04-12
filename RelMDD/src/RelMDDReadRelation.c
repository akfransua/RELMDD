/**CFile***********************************************************************//*


RelMDD has a special format to read in matrices from files. 
This format adhere to the format of Harwell-Boeing benchmark
suite. This format specifies how matrices should be stored and read from a file. In
our case the item on the file specifies the number of rows and column of the matrix.
The second and third items on the file specify the list of source and target objects
respectively. This is followed by the matrix in which the row and column indices of
each element of the matrix is also specified. Refer to the appendix for sample file.

******************************************************************************/
#include "util.h"
#include "cuddInt.h"
#include "structures.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <time.h>
#include "RelMDD.h"

fileInforPtr RelMdd_ReadFile(char *filename)
{
    FILE *ifp;

    int u, v, err, i;
    
   
    double val;

    ifp = fopen(filename, "r");
    if (ifp == NULL) {
	fprintf(stderr, "Can't open input file in.list!\n");
	exit(1);
    }

    err = fscanf(ifp, "%d %d", &u, &v);
    //printf("\n%d \t %d\n", u,  v);
    fileInforPtr ret;
    ret = (fileInforPtr) malloc(sizeof(fileInfor) * u * v);

    ret->r = u;
    ret->c = v;
    // printf("\n%d \t %d\n", ret->r,   ret->c);
    if (err == EOF) {
	fprintf(stderr, "Can't open input file in.list!\n");
	exit(1);
    } else if (err != 2) {
	fprintf(stderr, "Can't open input file in.list!\n");
	exit(1);
    }

    char *rowlist = NULL;
    rowlist = (char *) malloc(u * 3);

    err = fscanf(ifp, "%s", rowlist);

    if (err == EOF) {
	return (0);
    } else if (err != 1) {

	return (0);
    }

    char *collist = NULL;
    collist = (char *) malloc(v * 3);
    err = fscanf(ifp, "%s", collist);

    if (err == EOF) {
	return (0);
    } else if (err != 1) {

	return (0);
    }

    ret->sourcelist = (char *) malloc(sizeof(char));

    char *pch;
    int l = 0;

    pch = strtok(rowlist, "%[^]% [],.-");
    while (pch != NULL) {
	//  printf ("%s\n",pch);
	ret[l].sourcelist = pch;
	l++;
	pch = strtok(NULL, "[] ,%[^]%n.-");
    }
    

    ret->targetlist = (char *) malloc(sizeof(char));

    char *pchs;
    int ll = 0;

    pchs = strtok(collist, " [],.-");
    while (pchs != NULL) {
	ret[ll].targetlist = pchs;
	ll++;
	pchs = strtok(NULL, "[] ,.-");
    }

    ret->rel = (double *) malloc(sizeof(double) * u * v);

    ret->rowid = (int *) malloc(sizeof(int) * u * v);

    ret->rowida = (int *) malloc(sizeof(int) * u * v);
  
    i = 0;
    while (!feof(ifp)) {

	err = fscanf(ifp, "%d %d %lf", &u, &v, &val);
	// printf("\n%d \t %d \t%f\n ", u,  v, val);
	//printf("%d",err);
	if (err == EOF) {
	    break;
	}
	//else if () {
	//   break;
	else if (err != 3) {

	    break;
	}
	//else if (u >= m || v >= n || u < 0 || v < 0) {
	//   break;

	//}
	ret->rowid[i] = u;	//printf("%d\t ", ret->rowid[i]);
	ret->rowida[i] = v;	// printf("%d\t ", ret->rowida[i]);
	//ret->colid[i]=u; printf("%d\t ", ret->colid[i]);
	ret->rel[i] = val;	//printf("%f\n ", ret->rel[i]);
	i++;
    }

    fclose(ifp);

    return ret;
}
