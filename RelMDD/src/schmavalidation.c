/**CFile***********************************************************************//*
Most of the functions in this particular file are copy right of ( fucntions are copy right of ....
Copyright (C) 1998-2003 Daniel Veillard.All Rights Reserved.) or whoever has the right. I bear no ownership of most of this functiions.


................................................................................

This file contains the functions for validation the xml file against a schema file.

******************************************************************************/
#include "structures.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/valid.h>
#include <libxml/xmlschemas.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/tree.h>
#include <stdarg.h>
int vasprintf(char **  ptr, const char *  format, va_list arg);
u_int32_t getXMlFileSize(const char *file_name);
void	xmlSchemaCleanupTypes(void);


void handleValidationError(void *ctx, const char *format, ...);

u_int32_t getXMlFileSize(const char *file_name) {
    struct stat buf;
    if ( stat(file_name, &buf) != 0 ) return(0);
    return (unsigned int)buf.st_size;
}

void handleValidationError(void *ctx, const char *format, ...) {
    char *errMsg;
    va_list args;
    va_start(args, format);
    vasprintf(&errMsg, format, args);
    va_end(args);
    fprintf(stderr, "Validation Error: %s", errMsg);
    free(errMsg);
}

 int validationXml (const char *xsdPath , const char *xmlPath) {
    printf("''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''");
    printf("\n");

    printf("XSD File: %s\n", xsdPath);
    printf("XML File: %s\n", xmlPath);

    int xmlLength = getXMlFileSize(xmlPath);
    char *xmlSource = (char *)malloc(sizeof(char) * xmlLength);

    FILE *p = fopen(xmlPath, "r");
    char c;
    unsigned int i = 0;
    while ((c = fgetc(p)) != EOF) {
        xmlSource[i++] = c;
    }
    printf("\n");

   // printf("XML Source:\n\n%s\n", xmlSource); \\ you can use this to print out the content of the xml file
    fclose(p);

    printf("\n");

    int result = 42;
    xmlSchemaParserCtxtPtr parserCtxt = NULL;
    xmlSchemaPtr schema = NULL;
    xmlSchemaValidCtxtPtr validCtxt = NULL;

    xmlDocPtr xmlDocumentPointer = xmlParseMemory(xmlSource, xmlLength);
    parserCtxt = xmlSchemaNewParserCtxt(xsdPath);

    if (parserCtxt == NULL) {
        fprintf(stderr, "Could not create XSD schema parsing context.\n");
        goto leave;
    }

    schema = xmlSchemaParse(parserCtxt);
    //xmlSchemaDump(stdout, schema);
    if (schema == NULL) {
        fprintf(stderr, "Could not parse XSD schema.\n");
        goto leave;
    }

    validCtxt = xmlSchemaNewValidCtxt(schema);

    if (!validCtxt) {
        fprintf(stderr, "Could not create XSD schema validation context.\n");
        goto leave;
    }

    xmlSetStructuredErrorFunc(NULL, NULL);
    xmlSetGenericErrorFunc(NULL, handleValidationError);
    xmlThrDefSetStructuredErrorFunc(NULL, NULL);
    xmlThrDefSetGenericErrorFunc(NULL, handleValidationError);

    result = xmlSchemaValidateDoc(validCtxt, xmlDocumentPointer);

leave:

    if (parserCtxt) {
        xmlSchemaFreeParserCtxt(parserCtxt);
    }

    if (schema) {
        xmlSchemaFree(schema);
    }

    if (validCtxt) {
        xmlSchemaFreeValidCtxt(validCtxt);
    }
    xmlSchemaCleanupTypes();
    xmlCleanupParser();
    xmlMemoryDump();
    printf("\n");
    printf("Validation successful: %s (result: %d)\n", (result == 0) ? "YES" : "NO", result);

    return result;
}


/*int main (int argc, const char * argv[]) {
    const char *xsdPath = argv[1];
    const char *xmlPath = argv[2];
    int isvalid = validationXml(xsdPath, xmlPath);
    if(isvalid==0)   printf("Validation successful so continue to load the file xoxoxoxooxo : ");
    else {printf("Validation is not successful please check you xml document  ");}
}*/
