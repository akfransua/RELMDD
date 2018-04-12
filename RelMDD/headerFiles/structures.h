#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
//#include "RelMDD.h"
#define MAXA 500
typedef struct relation{// structure type to retrieve the relation item from the xml file
 int *number; 
 char   **source;
 char    ** target;
}relation, *relationPtr;
 

typedef struct identity{// structure type to retrieve the identity from the xml file
 char **object; 
 double   *rel ;
 int len;
}identity, *identityPtr; 

typedef struct bottom{// structure type to retrieve the bottom from the xml file

char **source;
char ** target;
double *rel;
 int len;
}bottom, *bottomPtr;

typedef struct top{// structure type to retrieve the top from the xml file

char **source;
char ** target;
double *rel;
int len;
}top, *topPtr;

typedef struct unions{// structure type to retrieve the unions from the xml file
  double *content; 
  char   **source;
  char    ** target;
  int len;
  int *len_Content;
}unions, *unionsPtr;

typedef struct intersection{// structure type to retrieve the intersection from the xml file
  double *content; 
  char   **source;
  char    ** target;
  int len;
  int *len_Content;
}intersection, *intersectionPtr;

typedef struct compoUnion{// structure type to retrieve the compostion_Union from the xml file
  double *content;
  char **source;
  char **target;
  int len;
  int *len_Content;
 }compoUnion, *compoUnionPtr;
 
 typedef struct compoInter{//structure type to retrieve the composition_intersection from the xml file
  double *content;
  char **source;
  char **target;
  int len;
  int *len_Content;
 }compoInter, *compoInterPtr;
 
 
 
 typedef struct converse{ // structure type to retrieve the converse from the xml file
  double *content;
  char **source;
  char **target;
  int len;
  int *len_Content;
 }converse, *conversePtr;
 
 typedef struct complement{ // structure type to retrieve the complement from the xml file
  double *content; 
  char **source;
  char **target;
  int len;
  int *len_Content;
 }complement, *complementPtr;
 
 
// Functions for retriving data from the xmlfile 
extern xmlDocPtr xmlDocument;
extern const char *xmlfile;
extern int RelMDDValidateXmlFile(const char *xmlPath);
extern int RelMDDParseXmlFile(const char *xmlFilename);
extern unionsPtr RelXmlGetUnion();
extern intersectionPtr  RelXmlGetIntersection();
extern complementPtr RelXmlGetComplement();
extern compoUnionPtr RelXmlGetComposition_Union();
extern compoInterPtr RelXmlGetComposition_Intersection();
extern conversePtr RelXmlGetConverse();
extern  topPtr RelXmlGetTop();
extern bottomPtr RelXmlGetBottom();
extern identityPtr RelXmlGetIdentity();
extern relationPtr RelXmlGetRelation();
extern void relXmlDisplayComments();
extern char ** relXmlGetObjects();
extern double *mat1;
extern double *mat2;
extern char **source_comp;
extern char **target_Comp;
extern int row_comp;
extern int col_comp;
extern double *element_comp;
extern int *row_trackComp;
extern int *col_trackComp;
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
extern compoInterPtr compoInterIni();
extern int validationXml (const char *xsdPath , const char *xmlPath);
extern int RelMDDValidateXmlFile(const char *xmlPath);
extern int RelMDDParseXmlFile(const char *xmlFilename);
extern unionsPtr RelXmlGetUnion();
extern intersectionPtr RelXmlGetIntersection();
extern complementPtr RelXmlGetComplement();
extern compoUnionPtr RelXmlGetComposition_Union();
extern compoInterPtr RelXmlGetComposition_Intersection();
extern conversePtr RelXmlGetConverse();
extern topPtr RelXmlGetTop();
extern bottomPtr RelXmlGetBottom();
extern identityPtr RelXmlGetIdentity();
extern relationPtr RelXmlGetRelation();
extern void relXmlDisplayComments();
extern char **relXmlGetObjects();
extern intersectionPtr relxmlReadIntersection(xmlNode * root, xmlDocPtr doc);
extern compoUnionPtr relxmlReadCompostion_Union(xmlNode * root, xmlDocPtr doc);
extern compoInterPtr relxmlReadCompostion_Inter(xmlNode * root, xmlDocPtr doc);
extern conversePtr relxmlReadTransposition(xmlNode * root, xmlDocPtr doc);
extern complementPtr relxmlReadComplement(xmlNode * root, xmlDocPtr doc); 
extern unionsPtr relxmlReadUnion(xmlNode * root, xmlDocPtr doc);
xmlChar *relXmlGetComments(xmlDocPtr doc, xmlNodePtr cur);
//void relXmlDisplayComments(xmlDocPtr doc, xmlNodePtr root);
extern xmlChar *relXmlGetListObject(xmlDocPtr doc, xmlNodePtr cur);
extern relationPtr relxmlReadrelations(xmlNode * root, xmlDocPtr doc);
extern char **relXmlGetSourceRelation(xmlChar * pt, int i);
extern char **relXmlGetTargetRelation(xmlChar * pt, int i);
extern char **relXmlGetNumberRelation(xmlChar * pt, int i);
extern identityPtr relxmlReadIdentity(xmlNode * root, xmlDocPtr doc);
extern char **relXmlGetObjectIdentity(xmlChar * pt, int i);
extern double *relXmlGetRelationIdentity(xmlChar * pt, int i);
extern topPtr relxmlReadTop(xmlNode * root, xmlDocPtr doc);
extern bottomPtr relxmlReadBottom(xmlNode * root, xmlDocPtr doc);
extern xmlChar *relXmlGetTopSource(xmlChar * pt, int i);
extern xmlChar *relXmlGetTopTarget(xmlChar * pt, int i);
extern xmlChar *relXmlGetTopRelation(xmlChar * pt, int i);
extern char **relXmlGetSource(xmlChar * pt, int i);
extern char **relXmlGetTarget(xmlChar * pt, int i);
extern char **split(char *string);

