/**CFile***********************************************************************//*
This file contains all the functions for maniputions of the xml file content internally. 
In this file we imported several functions from Libxml2( fucntions are copy right of ....
Copyright (C) 1998-2003 Daniel Veillard.All Rights Reserved.)

................................................................................

This functions accepts and xml file and extract the content, making it availble for 
the RelMDD system Internally. The user need not call any of this functions unless u want to modify it .


******************************************************************************/
// Imported functions
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXTOKENS       250
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <time.h>
#include "structures.h"
#include "RelMDD.h"

/***********************************************************************************************************************************
Prototypes decalations of functions in this file.

**************************************************************************************************************************************/
static int len_list;
char *trim(char *s);


complementPtr  complIni();
intersectionPtr interIni();
unionsPtr unionIni();
xmlDocPtr getdoc();
double *readString(unsigned char *s);
compoUnionPtr compoUionIni();
conversePtr coverselIni();
compoInterPtr compoInterIni();
compoInterPtr test();
xmlDocPtr xmlDocument;
/**Function********************************************************************
 Function: relXmlGetComments(xmlDocPtr doc, xmlNodePtr cur)
 This function reads the comments from the comments tag and  return it ..
 

******************************************************************************/

xmlChar *relXmlGetComments(xmlDocPtr doc, xmlNodePtr cur)
{

    xmlChar *comments =NULL;

    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "comment"))) {

	    comments = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	    printf("The comments on this basis file:\n%s ", comments);

	}
	cur = cur->next;

    }

    return (comments);
}



/**Function********************************************************************
 Function: relXmlGetListObject(xmlDocPtr doc, xmlNodePtr cur)
 This fucntion returns the string objects from the xmlfile A,B,C.. unformated
 

******************************************************************************/

xmlChar *relXmlGetListObject(xmlDocPtr doc, xmlNodePtr cur)
{

    xmlChar *object =NULL;

    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "objects"))) {

	    object = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

	}
	cur = cur->next;

    }

    return (object);
}


/**Function********************************************************************
 Function:relXmlReturnObjects(xmlDocPtr doc, xmlNodePtr root)
 this functions displays the list of objects from the object tag.

******************************************************************************/

void relXmlReturnObjects(xmlDocPtr doc, xmlNodePtr root)
{
    root = root->xmlChildrenNode;
    char *string;
    char **tokens = NULL;
    while (root != NULL) {
	if ((!xmlStrcmp(root->name, (const xmlChar *) "relation"))) {
	    string = (char *)relXmlGetListObject(doc, root);

	    printf("list of objects from the object in basis file %s\n",
		   string);
	    tokens = (char**)split(string);
	}

	root = root->next;
    }

    int i = 0;

    for (i = 0; tokens[i] != NULL; i++)

	printf("%02dobjects broken into token: %s\n", i, tokens[i]);

    for (i = 0; tokens[i] != NULL; i++)
	free(tokens[i]);

    free(tokens);

}


/**Function********************************************************************
 Function: split(char *string)
 split string into tokens, return token array it serve as tokenizer 
 

******************************************************************************/

char **split(char *string)
{

    char *delim = ".,:;`'\"+-_(){}[]<>*&^%$#@!?~/|\\= \t\r\n";
    char **tokens = NULL;
    char *working = NULL;
    char *token = NULL;
    int idx = 0;
    int len = strlen(string) + 1;

    tokens = malloc(sizeof(char *) * len);
    if (tokens == NULL)
	return NULL;
    working = malloc(sizeof(char) * strlen(string) + 1);
    if (working == NULL)
	return NULL;

    /* to make sure, copy string to a safe place */
    strcpy(working, string);
    for (idx = 0; idx < len; idx++)
	tokens[idx] = NULL;

    token = strtok(working, delim);
    idx = 0;

    /* always keep the last entry NULL termindated */
    while ((idx < (len - 1)) && (token != NULL)) {
	tokens[idx] = malloc(sizeof(char) * strlen(token) + 1);
	if (tokens[idx] != NULL) {
	    strcpy(tokens[idx], token);
	    idx++;
	    token = strtok(NULL, delim);
	}
    }

    free(working);
    return tokens;
}



/**Function********************************************************************
 Function: relxmlReadrelations(xmlNode * root, xmlDocPtr doc)
 This fucntion extract the content of the relation tag
 

******************************************************************************/
relationPtr relxmlReadrelations(xmlNode * root, xmlDocPtr doc)
{

    xmlNode *cur_node, *child_node;
    xmlChar *sourceAtrr, *targetAtrr, *numberAtrr;
    int i;
    relationPtr ret;
    ret = (relationPtr) malloc(sizeof(relationPtr));
    ret->number = (int *) malloc(sizeof(int) * 100);// modify this if the number of objects are more than 100
    ret->source = (char **) malloc(sizeof(char));
    ret->target = (char **) malloc(sizeof(char));
    for (cur_node = root->children; cur_node != NULL;
	 cur_node = cur_node->next) {

	if (cur_node->type == XML_ELEMENT_NODE &&
	    !xmlStrcmp(cur_node->name, (const xmlChar *) "relation")) {

	    for (child_node = cur_node->children, i = 0;
		 child_node != NULL; child_node = child_node->next) {

		if (cur_node->type == XML_ELEMENT_NODE &&
		    !xmlStrcmp(child_node->name,
			       (const xmlChar *) "relations")) {

		    //get the source attribute
		    sourceAtrr =
			xmlGetProp(child_node, (const xmlChar *) "source");
		    if (sourceAtrr) {

			ret->source[i] = (char *)sourceAtrr;
		    }
		    // get the target attributeprop = xmlGetNsProp(cur, ("name"), NULL);
		    targetAtrr =
			xmlGetProp(child_node, (const xmlChar *) "target");
		    if (targetAtrr) {

			ret->target[i] = (char *)sourceAtrr;
		    }
		    numberAtrr =
			xmlGetProp(child_node, (const xmlChar *) "number");
		    if (numberAtrr) {

			ret->number[i] = atoi((char*)numberAtrr);

		    }
		    i++;

		}

	    }

	}
    }
//xmlCleanupParser();
    return ret;
}


/**Function********************************************************************
 Function: relXmlGetNumberRelation(xmlDocPtr doc, xmlNodePtr cur)
 returns list of number of relatinos from the relations tag , you must free the return value after use:
 

******************************************************************************/

char **relXmlGetNumberRelation(xmlChar *pt, int i)
{   //char *string
    char **number;
    number = split((char *)pt);

    for (i = 0; number[i] != NULL; i++)

	printf("number %s\n", number[i]);

    return number;
}

/**Function********************************************************************
 Function: relXmlReadIdentity(xmlDocPtr doc, xmlNodePtr cur)
 returns the identity list from the identity tag:
 to get the identity relation

******************************************************************************/


identityPtr relxmlReadIdentity(xmlNode * root, xmlDocPtr doc)
{

    xmlNode *cur_node, *child_node;
    xmlChar *sourceAtrr, *targetAtrr;
    int i;
    identityPtr ret;
    ret = (identityPtr) malloc(sizeof(identity) * 100); // increase this if you are dealing with large data set
    ret->rel = (double *) malloc(sizeof(double) * 100);
    ret->object = (char **) malloc(sizeof(char));
    for (cur_node = root->children; cur_node != NULL;
	 cur_node = cur_node->next) {

	if (cur_node->type == XML_ELEMENT_NODE &&
	    !xmlStrcmp(cur_node->name, (const xmlChar *) "relation")) {

	    for (child_node = cur_node->children, i = 0;
		 child_node != NULL; child_node = child_node->next) {

		if (cur_node->type == XML_ELEMENT_NODE &&
		    !xmlStrcmp(child_node->name,
			       (const xmlChar *) "identity")) {

		    //get the object attribute
		    sourceAtrr =
			xmlGetProp(child_node, (const xmlChar *) "object");
		    if (sourceAtrr) {

			ret[i].object = relXmlGetSource(sourceAtrr, i);
			ret->len = i + 1;
		    }
		    // get the relations attribute of the identity.
		    targetAtrr =
			xmlGetProp(child_node,
				   (const xmlChar *) "relation");
		    if (targetAtrr) {
			ret->rel[i] = atof((char *)targetAtrr);

		    }
		    xmlFree(sourceAtrr);
		    xmlFree(targetAtrr);
		    i++;
		}

	    }

	}
    }
    //int j;
    // for(j = 0; j<ret->len; j++)

//printf("ok \t%s\n",*ret[j].object);
    return ret;
}/*End of  relxmlReadIdentity */

/**Function********************************************************************
 Function: relXmlGetSource(xmlChar * pt, int i)
 get the object attribute of

******************************************************************************/

char **relXmlGetSource(xmlChar * pt, int i)
{
    char **tokens;
    tokens = split((char *)pt);
    return tokens;
}/*End of relxmlGetSource*/

/**Function********************************************************************
 Function: relxmlReadBottom(xmlNode * root, xmlDocPtr doc)
 read the bottom relation or tranverse the bottom relations
 

******************************************************************************/

bottomPtr relxmlReadBottom(xmlNode * root, xmlDocPtr doc)
{
   
    xmlNode *cur_node, *child_node;
    xmlChar *sourceAtrr, *targetAtrr, *numberAtrr;
    int i;
    bottomPtr ret;
    ret = (bottomPtr) malloc(sizeof(bottom) * 100);//increase this if needed
    ret->rel = (double *) malloc(sizeof(double) * 100);// increase this if needed
    ret->source = (char **) malloc(sizeof(char));
    ret->target = (char **) malloc(sizeof(char));
    for (cur_node = root->children; cur_node != NULL;
	 cur_node = cur_node->next) {

	if (cur_node->type == XML_ELEMENT_NODE &&
	    !xmlStrcmp(cur_node->name, (const xmlChar *) "relation")) {

	    for (child_node = cur_node->children, i = 0;
		 child_node != NULL; child_node = child_node->next) {

		if (cur_node->type == XML_ELEMENT_NODE &&
		    !xmlStrcmp(child_node->name,
			       (const xmlChar *) "bottom")) {

		    //get the object attribute
		    sourceAtrr =
			xmlGetProp(child_node, (const xmlChar *) "source");
		    if (sourceAtrr) {
			ret[i].source = relXmlGetSource(sourceAtrr, i);
			// ret->source[i] =sourceAtrr;    
		    }
		    // get the relations attribute of the identity.
		    targetAtrr =
			xmlGetProp(child_node, (const xmlChar *) "target");
		    if (targetAtrr) {
			//relXmlGetBottomTarget(targetAtrr,i);
			//ret->target[i] =targetAtrr;  
			ret[i].target = relXmlGetSource(targetAtrr, i);
		    }

		    numberAtrr =
			xmlGetProp(child_node,
				   (const xmlChar *) "relation");
		    if (numberAtrr) {
			ret->rel[i] = atof((char *)numberAtrr);
			ret->len = i + 1;
		    }

		    xmlFree(sourceAtrr);
		    xmlFree(targetAtrr);
		    xmlFree(numberAtrr);
		    i++;

		}

	    }

	}

    }

   /* int j;
    for (j = 0; j < ret->len; j++)

	printf("bottom \t%s\t%s\t %f\n", *ret[j].source, *ret[j].target,
	       ret->rel[j]);*/

    return ret;
}/*End of relxmlReadBottom */


/**Function********************************************************************
 Function: topPtr relxmlReadTop(xmlNode * root, xmlDocPtr doc)
 This fucntion reads the top relation or tranverse the top relations
 

******************************************************************************/
 
topPtr relxmlReadTop(xmlNode * root, xmlDocPtr doc)
{

    xmlNode *cur_node, *child_node;
    xmlChar *sourceAtrr, *targetAtrr, *numberAtrr;
    int i;
    topPtr ret;
    ret = (topPtr) malloc(sizeof(top) * 100); //increase
    ret->rel = (double *) malloc(sizeof(double) * 100);
    ret->source = (char **) malloc(sizeof(char));
    ret->target = (char **) malloc(sizeof(char));
    for (cur_node = root->children; cur_node != NULL;
	 cur_node = cur_node->next) {

	if (cur_node->type == XML_ELEMENT_NODE &&
	    !xmlStrcmp(cur_node->name, (const xmlChar *) "relation")) {

	    for (child_node = cur_node->children, i = 0;
		 child_node != NULL; child_node = child_node->next) {

		if (cur_node->type == XML_ELEMENT_NODE &&
		    !xmlStrcmp(child_node->name, (const xmlChar *) "top"))
		{

		    //get the object attribute
		    sourceAtrr =
			xmlGetProp(child_node, (const xmlChar *) "source");
		    if (sourceAtrr) {

			//ret->source[i] =sourceAtrr;    
			ret[i].source = relXmlGetSource(sourceAtrr, i);
		    }
		    // get the relations attribute of the identity.
		    targetAtrr =
			xmlGetProp(child_node, (const xmlChar *) "target");
		    if (targetAtrr) {
			ret[i].target = relXmlGetSource(targetAtrr, i);
			//ret->target[i] =targetAtrr;         
		    }
		    numberAtrr =
			xmlGetProp(child_node,
				   (const xmlChar *) "relation");
		    if (numberAtrr) {

			ret->rel[i] = atof((char *)numberAtrr);
			ret->len = i + 1;
		    }

		    xmlFree(sourceAtrr);
		    xmlFree(targetAtrr);
		    xmlFree(numberAtrr);

		    i++;

		}

	    }

	}
    }
   
    // for(j = 0; j<ret->len; j++)

//printf("bottom \t%s\t%s\t %f\n",*ret[j].source,*ret[j].target,ret->rel[j]);

    return ret;
}/*End of relxmlReadTop*/


/**Function********************************************************************
 Function:unionsPtr relxmlReadUnion(xmlNode * root, xmlDocPtr doc)
 This functions returns the union data from the xml file ....
 
******************************************************************************/

unionsPtr relxmlReadUnion(xmlNode * root, xmlDocPtr doc)
{

    xmlNode *cur_node, *child_node;
    xmlChar *content_union;
    xmlChar *sourceAtrr, *targetAtrr;
    unions *ret = NULL;
    ret = (unions *) malloc(sizeof(unions) * 100);
    ret->content = (double *) malloc(sizeof(double) * 100);
    ret->len_Content = (int *) malloc(sizeof(int) * 100);
    ret->source = (char **) malloc(sizeof(char));
    ret->target = (char **) malloc(sizeof(char));
    // basis *ret = (basis*) calloc(40,sizeof(basis));
    if (ret == NULL) {
	/* Memory could not be allocated, so print an error and exit. */
	fprintf(stderr, "Couldn't allocate memory\n");
	exit(EXIT_FAILURE);
    }
    int i;
    for (cur_node = root->children; cur_node != NULL;
	 cur_node = cur_node->next) {

	if (cur_node->type == XML_ELEMENT_NODE &&
	    !xmlStrcmp(cur_node->name, (const xmlChar *) "relation")) {

	    for (child_node = cur_node->children, i = 0;
		 child_node != NULL; child_node = child_node->next) {

		if (cur_node->type == XML_ELEMENT_NODE &&
		    !xmlStrcmp(child_node->name,
			       (const xmlChar *) "union")) {

		    //get the source attribute
		    sourceAtrr =
			xmlGetProp(child_node, (const xmlChar *) "source");
		    if (sourceAtrr) {
			ret[i].source = relXmlGetSource(sourceAtrr, i);
		    }
		    // get the target attributeprop = xmlGetNsProp(cur, ("name"), NULL);
		    targetAtrr =
			xmlGetProp(child_node, (const xmlChar *) "target");
		    if (targetAtrr) {
			ret[i].target = relXmlGetTarget(targetAtrr, i);
			ret->len = i + 1;
		    }
		    content_union = xmlNodeGetContent(child_node);

		    if (content_union) {

			ret[i].content =
			    readString(xmlNodeListGetString
				       (doc, child_node->xmlChildrenNode,
					1));

			ret->len_Content[i] = len_list;
			//printf("cfvffd....%d\n..............................................dfdfd",  ret->len_Content[i] );
			len_list = 0;
			i++;
		    }

		    xmlFree(sourceAtrr);
		    xmlFree(targetAtrr);
		    xmlFree(content_union);

		}

	    }

	}
    }

    /*int k,kk;

       for(kk=0;kk<ret->len;kk++){
       printf("\nstarting union here so please be read thank you God\n ");

       printf("  %s || \t%s \n\t..%d..", *ret[kk].source, *ret[kk].target,  ret->len_Content[kk]);
       for(k=0;k<ret->len_Content[kk];k++)
       printf("  %f\n", ret[kk].content[k]);} */

    xmlCleanupParser();

    return ret;
}/*End relxmlReadUnion */

char **relXmlGetTarget(xmlChar * pt, int i)
{
    char **tokens;
    tokens = split((char *)pt);
    return tokens;
}

/**Function********************************************************************
 Function: relxmlReadIntersection(xmlNode * root, xmlDocPtr doc)
 This functions returns the intersection data from the xml file ....
 

******************************************************************************/

intersectionPtr relxmlReadIntersection(xmlNode * root, xmlDocPtr doc)
{

    xmlNode *cur_node, *child_node;
    xmlChar *content_union;
    xmlChar *sourceAtrr, *targetAtrr;
    intersection *ret = NULL;
    ret = (intersection *) malloc(sizeof(intersection) * 100);
    if (ret == NULL) {
	/* Memory could not be allocated, so print an error and exit. */
	fprintf(stderr, "Couldn't allocate memory\n");
	exit(EXIT_FAILURE);
    }
    ret->content = (double *) malloc(sizeof(double) * 100);
    ret->len_Content = (int *) malloc(sizeof(int) * 100);
    ret->source = (char **) malloc(sizeof(char));
    ret->target = (char **) malloc(sizeof(char));
    int i;
    for (cur_node = root->children; cur_node != NULL;
	 cur_node = cur_node->next) {

	if (cur_node->type == XML_ELEMENT_NODE &&
	    !xmlStrcmp(cur_node->name, (const xmlChar *) "relation")) {

	    for (child_node = cur_node->children, i = 0;
		 child_node != NULL; child_node = child_node->next) {

		if (cur_node->type == XML_ELEMENT_NODE &&
		    !xmlStrcmp(child_node->name,
			       (const xmlChar *) "intersection")) {

		    //get the source attribute
		    sourceAtrr =
			xmlGetProp(child_node, (const xmlChar *) "source");
		    if (sourceAtrr) {
			ret[i].source = relXmlGetSource(sourceAtrr, i);
		    }
		    // get the target attributeprop = xmlGetNsProp(cur, ("name"), NULL);
		    targetAtrr =
			xmlGetProp(child_node, (const xmlChar *) "target");
		    if (targetAtrr) {
			ret[i].target = relXmlGetTarget(targetAtrr, i);
			ret->len = i + 1;
		    }
		    content_union = xmlNodeGetContent(child_node);

		    if (content_union) {

			ret[i].content =
			    readString(xmlNodeListGetString
				       (doc, child_node->xmlChildrenNode,
					1));

			ret->len_Content[i] = len_list;
			//printf("cfvffd....%d\n..............................................dfdfd",  ret->len_Content[i] );
			len_list = 0;
			i++;

		    }

		    xmlFree(sourceAtrr);
		    xmlFree(targetAtrr);
		    xmlFree(content_union);

		}

	    }

	}
    }
    /* int k,kk;
       printf("\n.................intersection ...................................\n ");

       for(kk=0;kk<ret->len;kk++){

       printf("  %s || \t%s \n", *ret[kk].source, *ret[kk].target );
       for(k=0;k<ret->len_Content[kk];k++)
       printf("  %f\n", ret[kk].content[k] );} */
    xmlCleanupParser();

    return ret;
}/*End of relxmlReadIntersection*/


/**Function********************************************************************
 Function: relxmlReadCompostion_Union(xmlDocPtr doc, xmlNodePtr cur)
 This functions returns the Compostion_Union data from the xml file ....
 

******************************************************************************/
compoUnionPtr relxmlReadCompostion_Union(xmlNode * root, xmlDocPtr doc)
{

    xmlNode *cur_node, *child_node;
    xmlChar *content_union;
    xmlChar *sourceAtrr, *targetAtrr;
    compoUnion *ret = NULL;
    ret = (compoUnion *) malloc(sizeof(compoUnion) * 100);
    if (ret == NULL) {
	/* Memory could not be allocated, so print an error and exit. */
	fprintf(stderr, "Couldn't allocate memory\n");
	exit(EXIT_FAILURE);
    }
    ret->content = (double *) malloc(sizeof(double) * 100);
    ret->len_Content = (int *) malloc(sizeof(int) * 100);
    ret->source = (char **) malloc(sizeof(char));
    ret->target = (char **) malloc(sizeof(char));
    int i;
    for (cur_node = root->children; cur_node != NULL;
	 cur_node = cur_node->next) {

	if (cur_node->type == XML_ELEMENT_NODE &&
	    !xmlStrcmp(cur_node->name, (const xmlChar *) "relation")) {

	    for (child_node = cur_node->children, i = 0;
		 child_node != NULL; child_node = child_node->next) {

		if (cur_node->type == XML_ELEMENT_NODE &&
		    !xmlStrcmp(child_node->name,
			       (const xmlChar *) "composition_Union")) {

		    //get the source attribute
		    sourceAtrr =
			xmlGetProp(child_node, (const xmlChar *) "source");
		    if (sourceAtrr) {
			ret[i].source = relXmlGetSource(sourceAtrr, i);
		    }
		    // get the target attributeprop = xmlGetNsProp(cur, ("name"), NULL);
		    targetAtrr =
			xmlGetProp(child_node, (const xmlChar *) "target");
		    if (targetAtrr) {
			ret[i].target = relXmlGetTarget(targetAtrr, i);
                        ret->len=i+1;
		    }
		    content_union = xmlNodeGetContent(child_node);

		    if (content_union) {

			ret[i].content =
			    readString(xmlNodeListGetString
				       (doc, child_node->xmlChildrenNode,
					1));

			ret->len_Content[i] = len_list;
			//printf("cfvffd....%d\n..............................................dfdfd",  ret->len_Content[i] );
			len_list = 0;
			i++;
		    }

		    xmlFree(sourceAtrr);
		    xmlFree(targetAtrr);
		    xmlFree(content_union);

		}

	    }

	}
    }
    xmlCleanupParser();

    return ret;
}/*End of relxmlReadCompostion_Union*/


/**Function********************************************************************
 Function: relxmlReadCompostion_Inter(xmlNode * root, xmlDocPtr doc)
This functions returns the Compostion_Intersection data from the xml file ....
 

******************************************************************************/
compoInterPtr relxmlReadCompostion_Inter(xmlNode * root, xmlDocPtr doc)
{

    xmlNode *cur_node, *child_node;
    xmlChar *content_union;
    xmlChar *sourceAtrr, *targetAtrr;
    compoInter *ret = NULL;
    ret = (compoInter *) malloc(sizeof(compoInter) * 100);
    if (ret == NULL) {
	/* Memory could not be allocated, so print an error and exit. */
	fprintf(stderr, "Couldn't allocate memory\n");
	exit(EXIT_FAILURE);
    }
    ret->content = (double *) malloc(sizeof(double) * 100);
    ret->len_Content = (int *) malloc(sizeof(int) * 100);
    ret->source = (char **) malloc(sizeof(char));
    ret->target = (char **) malloc(sizeof(char));
    int i;
    for (cur_node = root->children; cur_node != NULL;
	 cur_node = cur_node->next) {

	if (cur_node->type == XML_ELEMENT_NODE &&
	    !xmlStrcmp(cur_node->name, (const xmlChar *) "relation")) {

	    for (child_node = cur_node->children, i = 0;
		 child_node != NULL; child_node = child_node->next) {

		if (cur_node->type == XML_ELEMENT_NODE &&
		    !xmlStrcmp(child_node->name, (const xmlChar *)
			       "composition_Intersection")) {

		    //get the source attribute
		    sourceAtrr =
			xmlGetProp(child_node, (const xmlChar *) "source");
		    if (sourceAtrr) {
			ret[i].source = relXmlGetSource(sourceAtrr, i);
		    }
		    // get the target attributeprop = xmlGetNsProp(cur, ("name"), NULL);
		    targetAtrr =
			xmlGetProp(child_node, (const xmlChar *) "target");
		    if (targetAtrr) {
			ret[i].target = relXmlGetTarget(targetAtrr, i);
			ret->len = i + 1;
		    }
		    content_union = xmlNodeGetContent(child_node);

		    if (content_union) {

			ret[i].content =
			    readString(xmlNodeListGetString
				       (doc, child_node->xmlChildrenNode,
					1));

			ret->len_Content[i] = len_list;
			//printf("cfvffd....%d\n..............................................dfdfd",  ret->len_Content[i] );
			len_list = 0;
			i++;
		    }

		    xmlFree(sourceAtrr);
		    xmlFree(targetAtrr);
		    xmlFree(content_union);

		}

	    }

	}
    }

    xmlCleanupParser();

    return ret;
}/*End of relxmlReadCompostion_Inter*/


/**Function********************************************************************
 Function: relxmlReadTransposition(xmlNode * root, xmlDocPtr doc)
This functions returns the transposition data from the xml file ....
 

******************************************************************************/


conversePtr relxmlReadTransposition(xmlNode * root, xmlDocPtr doc)
{

    xmlNode *cur_node, *child_node;
    xmlChar *content_union;
    xmlChar *sourceAtrr, *targetAtrr;
    converse *ret = NULL;
    ret = (converse *) malloc(sizeof(converse) * 100);
    if (ret == NULL) {
	/* Memory could not be allocated, so print an error and exit. */
	fprintf(stderr, "Couldn't allocate memory\n");
	exit(EXIT_FAILURE);
    }

    ret->content = (double *) malloc(sizeof(double) * 100);
    ret->len_Content = (int *) malloc(sizeof(int) * 100);
    ret->source = (char **) malloc(sizeof(char));
    ret->target = (char **) malloc(sizeof(char));
    int i;
    for (cur_node = root->children; cur_node != NULL;
	 cur_node = cur_node->next) {

	if (cur_node->type == XML_ELEMENT_NODE &&
	    !xmlStrcmp(cur_node->name, (const xmlChar *) "relation")) {

	    for (child_node = cur_node->children, i = 0;
		 child_node != NULL; child_node = child_node->next) {

		if (cur_node->type == XML_ELEMENT_NODE &&
		    !xmlStrcmp(child_node->name,
			       (const xmlChar *) "transposition")) {

		    //get the source attribute
		    sourceAtrr =
			xmlGetProp(child_node, (const xmlChar *) "source");
		    if (sourceAtrr) {
			ret[i].source = relXmlGetSource(sourceAtrr, i);
		    }
		    // get the target attributeprop = xmlGetNsProp(cur, ("name"), NULL);
		    targetAtrr =
			xmlGetProp(child_node, (const xmlChar *) "target");
		    if (targetAtrr) {
			ret[i].target = relXmlGetTarget(targetAtrr, i);
			ret->len = i + 1;

		    }
		    content_union = xmlNodeGetContent(child_node);

		    if (content_union) {

			ret[i].content =
			    readString(xmlNodeListGetString
				       (doc, child_node->xmlChildrenNode,
					1));

			ret->len_Content[i] = len_list;
			//printf("cfvffd....%d\n..............................................dfdfd",  ret->len_Content[i] );
			len_list = 0;
			i++;
		    }

		    xmlFree(sourceAtrr);
		    xmlFree(targetAtrr);
		    xmlFree(content_union);

		}

	    }

	}
    }

    xmlCleanupParser();

    return ret;
}/*End of transposition*/


/**Function********************************************************************
 Function: relxmlReadComplement(xmlNode * root, xmlDocPtr doc)
This functions returns the Complement data from the xml file ....
 

******************************************************************************/
complementPtr relxmlReadComplement(xmlNode * root, xmlDocPtr doc)
{

    xmlNode *cur_node, *child_node;
    xmlChar *content_union;
    xmlChar *sourceAtrr, *targetAtrr;
    complement *ret = NULL;
    ret = (complement *) malloc(sizeof(complement) * 100);
    if (ret == NULL) {
	/* Memory could not be allocated, so print an error and exit. */
	fprintf(stderr, "Couldn't allocate memory\n");
	exit(EXIT_FAILURE);
    }
    ret->content = (double *) malloc(sizeof(double) * 100);
    ret->len_Content = (int *) malloc(sizeof(int) * 100);
    ret->source = (char **) malloc(sizeof(char));
    ret->target = (char **) malloc(sizeof(char));
    int i;
    for (cur_node = root->children; cur_node != NULL;
	 cur_node = cur_node->next) {

	if (cur_node->type == XML_ELEMENT_NODE &&
	    !xmlStrcmp(cur_node->name, (const xmlChar *) "relation")) {

	    for (child_node = cur_node->children, i = 0;
		 child_node != NULL; child_node = child_node->next) {

		if (cur_node->type == XML_ELEMENT_NODE &&
		    !xmlStrcmp(child_node->name,
			       (const xmlChar *) "complement")) {

		    //get the source attribute
		    sourceAtrr =
			xmlGetProp(child_node, (const xmlChar *) "source");
		    if (sourceAtrr) {
			ret[i].source = relXmlGetSource(sourceAtrr, i);
		    }
		    // get the target attributeprop = xmlGetNsProp(cur, ("name"), NULL);
		    targetAtrr =
			xmlGetProp(child_node, (const xmlChar *) "target");
		    if (targetAtrr) {
			ret[i].target = relXmlGetTarget(targetAtrr, i);
			ret->len = i + 1;

		    }
		    content_union = xmlNodeGetContent(child_node);

		    if (content_union) {

			ret[i].content =
			    readString(xmlNodeListGetString
				       (doc, child_node->xmlChildrenNode,
					1));

			ret->len_Content[i] = len_list;
			//printf("cfvffd....%d\n..............................................dfdfd",  ret->len_Content[i] );
			len_list = 0;
			i++;
		    }

		    xmlFree(sourceAtrr);
		    xmlFree(targetAtrr);
		    xmlFree(content_union);

		}

	    }

	}
    }
    /* int k,kk;

       for(kk=0;kk<ret->len;kk++){
       printf("\nstarting union here so please be read thank you God\n ");

       printf("  %s || \t%s \n\t..%d..", *ret[kk].source, *ret[kk].target,  ret->len_Content[kk]);
       for(k=0;k<ret->len_Content[kk];k++)
       printf("  %f\n", ret[kk].content[k]);} */
    xmlCleanupParser();

    return ret;
}/*End of complement*/

xmlChar *targetObjects(xmlChar * pt, int i)
{

    xmlChar *ptt[4];
    ptt[i] = pt;
//printf(" target objects%s%d \n", ptt[i],i);
    return *ptt;
}

double *readString(unsigned char *s)
{
    float num;
    int nc, i = 0;
    double *list = (double *) malloc((strlen((char *)s) + 1) * sizeof(double));
//sscanf(s, "%[^]%n", &num1, &nd);

    while (sscanf((char *)s, "%f%n", &num, &nc) == 1
	   || sscanf((char *)s, "%*[,;]%f%n", &num, &nc) == 1) {

	s += nc;
	list[i] = (double) num;

	i++;
    }
    //printf("the leng in fuction%d",i);
    //for(j=0;j<i;j++)
    //printf("\n%f\n", list[j]);
    //printf("new list%d",i);
    len_list = i;
    //printf("....%d\n..............................................", i);
    return list;
}


conversePtr coverselIni()
{

    //xmlDocPtr doc;
   // doc = xmlParseFile("w3.xml");

    if (xmlDocument== NULL)
	printf("error: could not parse file file.xml\n");
    xmlNode *root;
    root = xmlDocGetRootElement(xmlDocument);

    conversePtr ret;

    // memset(ret, 0, sizeof(identityPtr));
    ret = relxmlReadTransposition(root,xmlDocument);
    //int k,kk;
// printf("\n................. in main...................................\n ");

    //  for(kk=0;kk<4;kk++){

//printf("  %s || \t%s \n", ret->source[kk], ret->target[kk] );
//for(k=0;k<9;k++)
//printf("  %f\n", ret[kk].content[k] );
//}

    return ret;
}

complementPtr complIni()
{

    //xmlDocPtr doc;
    //doc = xmlParseFile("w3.xml");

    if (xmlDocument == NULL)
	printf("error: could not parse file file.xml\n");
    xmlNode *root;
    root = xmlDocGetRootElement(xmlDocument);

    complementPtr ret;

    // memset(ret, 0, sizeof(identityPtr));
    ret = relxmlReadComplement(root, xmlDocument);
    //int k,kk;
// printf("\n................. in main...................................\n ");

    //  for(kk=0;kk<4;kk++){

//printf("  %s || \t%s \n", ret->source[kk], ret->target[kk] );
//for(k=0;k<9;k++)
//printf("  %f\n", ret[kk].content[k] );
//}

    return ret;
}


intersectionPtr interIni()
{

  //  xmlDocPtr doc;
   // doc = xmlParseFile("w3.xml");

    if (xmlDocument == NULL)
	printf("error: could not parse file file.xml\n");
    xmlNode *root = NULL;
    root = xmlDocGetRootElement(xmlDocument);

    intersectionPtr ret;

    // memset(ret, 0, sizeof(identityPtr));
    ret = relxmlReadIntersection(root, xmlDocument);
    //int k,kk;
// printf("\n................. in main...................................\n ");

    //  for(kk=0;kk<4;kk++){

//printf("  %s || \t%s \n", ret->source[kk], ret->target[kk] );
//for(k=0;k<9;k++)
//printf("  %f\n", ret[kk].content[k] );
//}
    if (!root || !root->name || xmlStrcmp(root->name, (xmlChar *)"RelationBasis")) {
	xmlFreeDoc(xmlDocument);
	return NULL;
    }

    return ret;
}

unionsPtr unionIni()
{

   // xmlDocPtr doc;
   // doc = xmlParseFile("w3.xml");

    if (xmlDocument == NULL)
	printf("error: could not parse file file.xml\n");
    xmlNode *root = NULL;
    root = xmlDocGetRootElement(xmlDocument);

    unionsPtr ret;

    // memset(ret, 0, sizeof(identityPtr));
    ret = relxmlReadUnion(root, xmlDocument);
    //int k,kk;
// printf("\n................. in main...................................\n ");

    //  for(kk=0;kk<4;kk++){

//printf("  %s || \t%s \n", ret->source[kk], ret->target[kk] );
//for(k=0;k<9;k++)
//printf("  %f\n", ret[kk].content[k] );
//}
    if (!root || !root->name || xmlStrcmp(root->name, (xmlChar *)"RelationBasis")) {
	xmlFreeDoc(xmlDocument);
	return NULL;
    }

    return ret;
}

identityPtr idet()
{

    ///xmlDocPtr doc;
    //doc = xmlParseFile("w3.xml");

    if (xmlDocument == NULL)
	printf("error: could not parse file file.xml\n");
    xmlNode *root;
    root = xmlDocGetRootElement(xmlDocument);

    identityPtr ret;

    // memset(ret, 0, sizeof(identityPtr));
    ret = relxmlReadIdentity(root, xmlDocument);
    //int k,kk;
// printf("\n................. in main...................................\n ");

    //  for(kk=0;kk<4;kk++){

//printf("  %s || \t%s \n", ret->source[kk], ret->target[kk] );
//for(k=0;k<9;k++)
//printf("  %f\n", ret[kk].content[k] );
//}

    return ret;
}

bottomPtr bott()
{

   // xmlDocPtr doc;
   // doc = xmlParseFile("w3.xml");

    if (xmlDocument == NULL)
	printf("error: could not parse file file.xml\n");
    xmlNode *root;
    root = xmlDocGetRootElement(xmlDocument);

    bottomPtr ret;

    // memset(ret, 0, sizeof(identityPtr));
    ret = relxmlReadBottom(root, xmlDocument);
    //int k,kk;
// printf("\n................. in main...................................\n ");

    //  for(kk=0;kk<4;kk++){

//printf("  %s || \t%s \n", ret->source[kk], ret->target[kk] );
//for(k=0;k<9;k++)
//printf("  %f\n", ret[kk].content[k] );
//}

    return ret;
}

topPtr topp()
{

    //xmlDocPtr doc;
   // doc = xmlParseFile("w3.xml");
    if (xmlDocument == NULL)
	printf("error: could not parse file file.xml\n");
    xmlNode *root;
    root = xmlDocGetRootElement(xmlDocument);

    topPtr ret;

    // memset(ret, 0, sizeof(identityPtr));
    ret = relxmlReadTop(root, xmlDocument);
    //int k,kk;
// printf("\n................. in main...................................\n ");

    //  for(kk=0;kk<4;kk++){

//printf("  %s || \t%s \n", ret->source[kk], ret->target[kk] );
//for(k=0;k<9;k++)
//printf("  %f\n", ret[kk].content[k] );
//}

    return ret;
    
}

 
compoUnionPtr compoUionIni(){

//xmlDocPtr doc;
  //doc = xmlParseFile("w3.xml");

  if (xmlDocument == NULL) 
        printf("error: could not parse file file.xml\n");
    xmlNode *root = NULL ;
  root = xmlDocGetRootElement(xmlDocument);
   
 
  
   compoUnionPtr ret;

    
   // memset(ret, 0, sizeof(identityPtr));
 ret=  relxmlReadCompostion_Union(  root,xmlDocument);
  //int k,kk;
// printf("\n................. in main...................................\n ");

 //  for(kk=0;kk<4;kk++){
 
//printf("  %s || \t%s \n", ret->source[kk], ret->target[kk] );
//for(k=0;k<9;k++)
//printf("  %f\n", ret[kk].content[k] );
//}
 if( !root || 
      !root->name ||
      xmlStrcmp(root->name,(xmlChar *)"RelationBasis") ) 
  {
     xmlFreeDoc(xmlDocument);
     return NULL;
  }

return ret;
 }
 
 
 
compoInterPtr compoInterIni(){

//xmlDocPtr doc;
 // doc = xmlParseFile("w3.xml");

  if (xmlDocument == NULL) 
        printf("error: could not parse file file.xml\n");
    xmlNode *root = NULL ;
  root = xmlDocGetRootElement(xmlDocument);
   
 
  
   compoInterPtr ret;

    
   // memset(ret, 0, sizeof(identityPtr));
 ret=  relxmlReadCompostion_Inter(  root,xmlDocument);
  //int k,kk;
// printf("\n................. in main...................................\n ");

 //  for(kk=0;kk<4;kk++){
 
//printf("  %s || \t%s \n", ret->source[kk], ret->target[kk] );
//for(k=0;k<9;k++)
//printf("  %f\n", ret[kk].content[k] );
//}
 if( !root || 
      !root->name ||
      xmlStrcmp(root->name, (xmlChar *)"RelationBasis") ) 
  {
     xmlFreeDoc(xmlDocument);
     return NULL;
  }

return ret;
 }
 
 compoInterPtr test(){

//xmlDocPtr doc;
  //doc = xmlParseFile("w3.xml");

  if (xmlDocument== NULL) 
        printf("error: could not parse file file.xml\n");
    xmlNode *root = NULL ;
  root = xmlDocGetRootElement(xmlDocument);
   
 
  
   compoInterPtr ret;

    
   // memset(ret, 0, sizeof(identityPtr));
 ret=  relxmlReadCompostion_Inter(  root,xmlDocument);
  int k,kk;
printf("\n................. in main...................................\n ");

  for(kk=0;kk<4;kk++){
 
printf("  %s || \t%s \n", *ret[kk].source, *ret[kk].target );
for(k=0;k<20;k++)
printf("  %f\n", ret[kk].content[k] );
}
 if( !root || 
      !root->name ||
      xmlStrcmp(root->name,(xmlChar *)"RelationBasis") ) 
  {
     xmlFreeDoc(xmlDocument);
     return NULL;
  }

return ret;
 }
 
 
