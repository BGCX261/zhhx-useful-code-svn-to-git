// xml.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "xml.h"
#include <stdio.h>
#include <string.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



void testXmlwriterMemory(const char *file);

// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		testXmlwriterMemory("xml.xml");
	}

	return nRetCode;
}


void
testXmlwriterMemory(const char *file)
{
	int rc;
	xmlTextWriterPtr writer;
	xmlBufferPtr buf;
	xmlChar *tmp;
	FILE *fp;

	/* Create a new XML buffer, to which the XML document will be
	* written */
	buf = xmlBufferCreate();
	if (buf == NULL) {
		printf("testXmlwriterMemory: Error creating the xml buffer\n");
		return;
	}

	/* Create a new XmlWriter for memory, with no compression.
	* Remark: there is no compression for this kind of xmlTextWriter */
	writer = xmlNewTextWriterMemory(buf, 0);
	if (writer == NULL) {
		printf("testXmlwriterMemory: Error creating the xml writer\n");
		return;
	}

	/* Start the document with the xml default for the version,
	* encoding ISO 8858-1 and the default for the standalone
	* declaration. */
	rc = xmlTextWriterStartDocument(writer, NULL, "utf-8", NULL);
	if (rc < 0) {
		printf
			("testXmlwriterMemory: Error at xmlTextWriterStartDocument\n");
		return;
	}

	/* Start an element named "EXAMPLE". Since thist is the first
	* element, this will be the root element of the document. */
	rc = xmlTextWriterStartElement(writer, BAD_CAST "EXAMPLE");
	if (rc < 0) {
		printf
			("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		return;
	}	

	/* Start an element named "ORDER" as child of EXAMPLE. */
	rc = xmlTextWriterStartElement(writer, BAD_CAST "ORDER");
	if (rc < 0) {
		printf
			("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		return;
	}

	/* Add an attribute with name "version" and value "1.0" to ORDER. */
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "version",
		BAD_CAST "1.0");
	if (rc < 0) {
		printf
			("testXmlwriterMemory: Error at xmlTextWriterWriteAttribute\n");
		return;
	}

	/* Add an attribute with name "xml:lang" and value "de" to ORDER. */
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xml:lang",
		BAD_CAST "de");
	if (rc < 0) {
		printf
			("testXmlwriterMemory: Error at xmlTextWriterWriteAttribute\n");
		return;
	}	

	/* Start an element named "HEADER" as child of ORDER. */
	rc = xmlTextWriterStartElement(writer, BAD_CAST "HEADER");
	if (rc < 0) {
		printf
			("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		return;
	}

	/* Write an element named "X_ORDER_ID" as child of HEADER. */
	rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "X_ORDER_ID",
		"%010d", 53535L);
	if (rc < 0) {
		printf
			("testXmlwriterMemory: Error at xmlTextWriterWriteFormatElement\n");
		return;
	}

	/* Write an element named "CUSTOMER_ID" as child of HEADER. */
	rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "CUSTOMER_ID",
		"%d", 1010);
	if (rc < 0) {
		printf
			("testXmlwriterMemory: Error at xmlTextWriterWriteFormatElement\n");
		return;
	}
	
	/* Close the element named HEADER. */
	rc = xmlTextWriterEndElement(writer);
	if (rc < 0) {
		printf("testXmlwriterMemory: Error at xmlTextWriterEndElement\n");
		return;
	}

	/* Start an element named "ENTRIES" as child of ORDER. */
	rc = xmlTextWriterStartElement(writer, BAD_CAST "ENTRIES");
	if (rc < 0) {
		printf
			("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		return;
	}

	/* Start an element named "ENTRY" as child of ENTRIES. */
	rc = xmlTextWriterStartElement(writer, BAD_CAST "ENTRY");
	if (rc < 0) {
		printf
			("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		return;
	}

	/* Write an element named "ARTICLE" as child of ENTRY. */
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "ARTICLE",
		BAD_CAST "<Test>");
	if (rc < 0) {
		printf
			("testXmlwriterMemory: Error at xmlTextWriterWriteElement\n");
		return;
	}

	/* Write an element named "ENTRY_NO" as child of ENTRY. */
	rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "ENTRY_NO", "%d",
		10);
	if (rc < 0) {
		printf
			("testXmlwriterMemory: Error at xmlTextWriterWriteFormatElement\n");
		return;
	}

	/* Close the element named ENTRY. */
	rc = xmlTextWriterEndElement(writer);
	if (rc < 0) {
		printf("testXmlwriterMemory: Error at xmlTextWriterEndElement\n");
		return;
	}

	/* Start an element named "ENTRY" as child of ENTRIES. */
	rc = xmlTextWriterStartElement(writer, BAD_CAST "ENTRY");
	if (rc < 0) {
		printf
			("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		return;
	}

	/* Write an element named "ARTICLE" as child of ENTRY. */
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "ARTICLE",
		BAD_CAST "<Test 2>");
	if (rc < 0) {
		printf
			("testXmlwriterMemory: Error at xmlTextWriterWriteElement\n");
		return;
	}

	/* Write an element named "ENTRY_NO" as child of ENTRY. */
	rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "ENTRY_NO", "%d",
		20);
	if (rc < 0) {
		printf
			("testXmlwriterMemory: Error at xmlTextWriterWriteFormatElement\n");
		return;
	}

	/* Close the element named ENTRY. */
	rc = xmlTextWriterEndElement(writer);
	if (rc < 0) {
		printf("testXmlwriterMemory: Error at xmlTextWriterEndElement\n");
		return;
	}

	/* Close the element named ENTRIES. */
	rc = xmlTextWriterEndElement(writer);
	if (rc < 0) {
		printf("testXmlwriterMemory: Error at xmlTextWriterEndElement\n");
		return;
	}

	/* Start an element named "FOOTER" as child of ORDER. */
	rc = xmlTextWriterStartElement(writer, BAD_CAST "FOOTER");
	if (rc < 0) {
		printf
			("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		return;
	}

	/* Write an element named "TEXT" as child of FOOTER. */
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "TEXT",
		BAD_CAST "This is a text.");
	if (rc < 0) {
		printf
			("testXmlwriterMemory: Error at xmlTextWriterWriteElement\n");
		return;
	}

	/* Close the element named FOOTER. */
	rc = xmlTextWriterEndElement(writer);
	if (rc < 0) {
		printf("testXmlwriterMemory: Error at xmlTextWriterEndElement\n");
		return;
	}

	/* Here we could close the elements ORDER and EXAMPLE using the
	* function xmlTextWriterEndElement, but since we do not want to
	* write any other elements, we simply call xmlTextWriterEndDocument,
	* which will do all the work. */
	rc = xmlTextWriterEndDocument(writer);
	if (rc < 0) {
		printf("testXmlwriterMemory: Error at xmlTextWriterEndDocument\n");
		return;
	}

	xmlFreeTextWriter(writer);

	fp = fopen(file, "w");
	if (fp == NULL) {
		printf("testXmlwriterMemory: Error at fopen\n");
		return;
	}

	fprintf(fp, "%s", (const char *) buf->content);

	fclose(fp);

	xmlBufferFree(buf);
}

