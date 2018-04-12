/**CFile***********************************************************************//*
This file contains all the functions for maniputions of the xml file content. 
In this file we imported several functions from Libxml2( fucntions are copy right of ....
Copyright (C) 1998-2003 Daniel Veillard.All Rights Reserved.)

................................................................................

This functions accepts and xml file and extract the content, making it availble for 
the RelMDD system and is and contains all external functions for use by the user.


******************************************************************************/

// imports 
#include "structures.h"  //header file contains structures and extern functions 
#include <libxml/xmlmemory.h> //from Libxml2
#include <libxml/parser.h> //from Libxml2
#include <libxml/tree.h> //from Libxml2
#include <time.h>  
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
xmlDocPtr xmlDocument;



/**Function********************************************************************
 Function: RelMDDValidateXmlFile(const char *xmlPath)
 this function calls the validations function to valid an xml file. It returns 
 a non zero value is the file was validated successfully. otherwise returns zeor.
 

******************************************************************************/
int RelMDDValidateXmlFile(const char *xmlPath)
{
    LIBXML_TEST_VERSION
	int isvalid = validationXml("xmlschema.xsd", xmlPath);
    if (isvalid == 0) {
	printf("Validation successful so continue to load the file: ");
	return isvalid;
    } else {
	printf
	    ("Validation is not successful please check you xml document against the schema ");
	return isvalid;
    }
}/* RelMDDValidateXmlFile*/


/**Function********************************************************************
 Function: RelMDDParseXmlFile(const char *xmlFilename)
 This functions accepts a file and makes the content availble for all other functions. 
 it is used for initialization .
 

******************************************************************************/
int RelMDDParseXmlFile(const char *xmlFilename)
{
    xmlDocPtr doc;
    doc = xmlParseFile(xmlFilename);
    if (doc == NULL) {
	return (0);

    }
    if (doc == NULL) {
	printf("error: could not parse file file.xml\n");
	xmlFreeDoc(doc);
	return (0);
    }
    //xmlNode *root = NULL;
    //root = xmlDocGetRootElement(doc);
    xmlDocument = doc;
    return (1);

}/* RelMDDParseXmlFile*/

/**Function********************************************************************
 Function:  RelXmlGetUnion()
  Returns the contents of the union tag
 
 

******************************************************************************/
unionsPtr RelXmlGetUnion()
{

    xmlDocPtr doc;
    xmlNode *root = NULL;
    doc = xmlDocument;
    if (doc == NULL) {
	printf("error: could not parse file file.xml\n");
    }
    root = xmlDocGetRootElement(doc);
    unionsPtr ret = NULL;
    ret = (unionsPtr) malloc(sizeof(unions));
    if (ret == NULL) {
	fprintf(stderr, "out of memory\n");
	return (NULL);
    }
    memset(ret, 0, sizeof(unions));
    ret = (unionsPtr) relxmlReadUnion(root, doc);
   
    /* for(kk=0;kk<ret->len;kk++){
       printf("\nstarting union here so please be read thank you God\n ");

       printf("  %s || \t%s \n\t..%d..", *ret[kk].source, *ret[kk].target,  ret->len_Content[kk]);
       for(k=0;k<ret->len_Content[kk];k++)
       printf("  %f\n", ret[kk].content[k]);}

     */
    return ret;
}/* RelXmlGetUnion*/
/**Function********************************************************************
 Function: RelXmlGetIntersection()
  Returns the contents of the Intersection tag
 
 

******************************************************************************/
intersectionPtr RelXmlGetIntersection()
{
    xmlDocPtr doc;
    xmlNode *root = NULL;
    doc = xmlDocument;
    if (doc == NULL) {
	printf("error: could not parse file file.xml\n");
    }
    root = xmlDocGetRootElement(doc);
    intersectionPtr ret = NULL;
    ret = (intersectionPtr) malloc(sizeof(intersection));
    if (ret == NULL) {
	fprintf(stderr, "out of memory\n");
	return (NULL);
    }
    memset(ret, 0, sizeof(intersection));
    ret = (intersectionPtr) relxmlReadIntersection(root, doc);
    return ret;
}/* RelXmlGetIntersection*/
/**Function********************************************************************
 Function: RelXmlGetComplement()
  Returns the contents of the complement tag
 
 

******************************************************************************/
complementPtr RelXmlGetComplement()
{
    xmlDocPtr doc;
    xmlNode *root = NULL;
    doc = xmlDocument;
    if (doc == NULL) {
	printf("error: could not parse file file.xml\n");
    }
    root = xmlDocGetRootElement(doc);
    complementPtr ret = NULL;
    ret = (complementPtr) malloc(sizeof(complement));
    if (ret == NULL) {
	fprintf(stderr, "out of memory\n");
	return (NULL);
    }
    memset(ret, 0, sizeof(complement));
    ret = (complementPtr) relxmlReadComplement(root, doc);
    return ret;
}/* RelXmlGetComplement*/
/**Function********************************************************************
 Function: RelXmlGetComposition_Union()
  Returns the contents of the Composition_Union tag
 .
 

******************************************************************************/
compoUnionPtr RelXmlGetComposition_Union()
{
    xmlDocPtr doc;
    xmlNode *root = NULL;
    doc = xmlDocument;
    if (doc == NULL) {
	printf("error: could not parse file file.xml\n");
    }
    root = xmlDocGetRootElement(doc);
    compoUnionPtr ret = NULL;
    ret = (compoUnionPtr) malloc(sizeof(compoUnion));
    if (ret == NULL) {
	fprintf(stderr, "out of memory\n");
	return (NULL);
    }
    memset(ret, 0, sizeof(compoUnion));
    ret = (compoUnionPtr) relxmlReadCompostion_Union(root, doc);
    return ret;
}/*RelXmlGetComposition_Union*/
/**Function********************************************************************
 Function:  RelXmlGetComposition_Intersection()
  Returns the contents of the Composition_Intersection tag
 
 

******************************************************************************/
compoInterPtr RelXmlGetComposition_Intersection()
{
    xmlDocPtr doc;
    xmlNode *root = NULL;
    doc = xmlDocument;
    if (doc == NULL) {
	printf("error: could not parse file file.xml\n");
    }
    root = xmlDocGetRootElement(doc);
    compoInterPtr ret = NULL;
    ret = (compoInterPtr) malloc(sizeof(compoInter));
    if (ret == NULL) {
	fprintf(stderr, "out of memory\n");
	return (NULL);
    }
    memset(ret, 0, sizeof(compoInter));
    ret = (compoInterPtr) relxmlReadCompostion_Inter(root, doc);
    return ret;
}/*RelXmlGetComposition_Intersection*/
/**Function********************************************************************
 Function: RelXmlGetConverse()
  Returns the contents of the converse tag
 

******************************************************************************/
conversePtr RelXmlGetConverse()
{
    xmlDocPtr doc;
    xmlNode *root = NULL;
    doc = xmlDocument;
    if (doc == NULL) {
	printf("error: could not parse file file.xml\n");
    }
    root = xmlDocGetRootElement(doc);
    conversePtr ret = NULL;
    ret = (conversePtr) malloc(sizeof(converse));
    if (ret == NULL) {
	fprintf(stderr, "out of memory\n");
	return (NULL);
    }
    memset(ret, 0, sizeof(converse));
    ret = (conversePtr) relxmlReadTransposition(root, doc);
    return ret;
}/* RelXmlGetConverse*/

/**Function********************************************************************
 Function: RelXmlGetTop()
  Returns the contents of the top tag
 
 

******************************************************************************/
topPtr RelXmlGetTop()
{
    xmlDocPtr doc;
    xmlNode *root = NULL;
    doc = xmlDocument;
    if (doc == NULL) {
	printf("error: could not parse file file.xml\n");
    }
    root = xmlDocGetRootElement(doc);
    topPtr ret = NULL;
    ret = (topPtr) malloc(sizeof(top));
    if (ret == NULL) {
	fprintf(stderr, "out of memory\n");
	return (NULL);
    }
    memset(ret, 0, sizeof(top));
    ret = (topPtr) relxmlReadTop(root, doc);
    return ret;
}
/* RelXmlGetTop()*/

/**Function********************************************************************
 Function:RelXmlGetBottom()
  Returns the contents of the bottom tag
 
 

******************************************************************************/
bottomPtr RelXmlGetBottom()
{
    xmlDocPtr doc;
    xmlNode *root = NULL;
    doc = xmlDocument;
    if (doc == NULL) {
	printf("error: could not parse file file.xml\n");
    }
    root = xmlDocGetRootElement(doc);
    bottomPtr ret = NULL;
    ret = (bottomPtr) malloc(sizeof(bottom));
    if (ret == NULL) {
	fprintf(stderr, "out of memory\n");
	return (NULL);
    }
    memset(ret, 0, sizeof(top));
    ret = (bottomPtr) relxmlReadBottom(root, doc);
    return ret;
}
/* RelXmlGetBottom*/


/**Function********************************************************************
 Function: RelXmlGetIdentity
 Returns the contents of the identity tag
 

******************************************************************************/
identityPtr RelXmlGetIdentity()
{
    xmlDocPtr doc;
    xmlNode *root = NULL;
    doc = xmlDocument;
    if (doc == NULL) {
	printf("error: could not parse file file.xml\n");
    }
    root = xmlDocGetRootElement(doc);
    identityPtr ret = NULL;
    ret = (identityPtr) malloc(sizeof(identity));
    if (ret == NULL) {
	fprintf(stderr, "out of memory\n");
	return (NULL);
    }
    memset(ret, 0, sizeof(identity));
    ret = (identityPtr) relxmlReadIdentity(root, doc);
    return ret;
}/* RelXmlGetIdentity*/



/**Function********************************************************************
 Function: RelXmlGetRelation()
 returns the contents from the relation tags

******************************************************************************/
relationPtr RelXmlGetRelation()
{
    xmlDocPtr doc;
    xmlNode *root = NULL;
    doc = xmlDocument;
    if (doc == NULL) {
	printf("error: could not parse file file.xml\n");
    }
    root = xmlDocGetRootElement(doc);
    relationPtr ret = NULL;
    ret = (relationPtr) malloc(sizeof(relation));
    if (ret == NULL) {
	fprintf(stderr, "out of memory\n");
	return (NULL);
    }
    memset(ret, 0, sizeof(relation));
    ret = (relationPtr) relxmlReadrelations(root, doc);
    return ret;
}/*RelXmlGetRelation*/


/**Function********************************************************************
 Function: relXmlDisplayComment*s()
 //print/displays to the comments from the comments tag.
 

******************************************************************************/
 
void relXmlDisplayComments()
{
    xmlDocPtr doc;
    xmlNode *root = NULL;
    doc = xmlDocument;
    if (doc == NULL) {
	printf("error: could not parse file file.xml\n");
    }
    root = xmlDocGetRootElement(doc);
    root = root->xmlChildrenNode;
    while (root != NULL) {
	if ((!xmlStrcmp(root->name, (const xmlChar *) "relation"))) {
	    relXmlGetComments(doc, root);

	}

	root = root->next;
    }

}/* relXmlDisplayComments*/


/**Function********************************************************************
 Function: relXmlGetObjects()
 returns the list of objects 
 

******************************************************************************/
char **relXmlGetObjects()
{
    xmlDocPtr doc;
    xmlNode *root = NULL;
    doc = xmlDocument;
    if (doc == NULL) {
	printf("error: could not parse file file.xml\n");
    }
    root = xmlDocGetRootElement(doc);
    root = root->xmlChildrenNode;
    xmlChar *string;
    char **tokens = NULL;
    while (root != NULL) {
	if ((!xmlStrcmp(root->name, (const xmlChar *) "relation"))) {
	    string = (xmlChar *) relXmlGetListObject(doc, root);

	    printf("list of objects from the object in basis file %s\n",
		   string);
	    tokens = (char **) split((char *)string);
	}

	root = root->next;
    }

    return tokens;

}/*relXmlGetObjects*/




/*END Of FILE....................................*/
