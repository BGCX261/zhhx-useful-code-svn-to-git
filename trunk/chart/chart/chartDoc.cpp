// chartDoc.cpp : implementation of the CchartDoc class
//

#include "stdafx.h"
#include "chart.h"

#include "chartDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CchartDoc

IMPLEMENT_DYNCREATE(CchartDoc, CDocument)

BEGIN_MESSAGE_MAP(CchartDoc, CDocument)
END_MESSAGE_MAP()


// CchartDoc construction/destruction

CchartDoc::CchartDoc()
{
	// TODO: add one-time construction code here

}

CchartDoc::~CchartDoc()
{
}

BOOL CchartDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CchartDoc serialization

void CchartDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CchartDoc diagnostics

#ifdef _DEBUG
void CchartDoc::AssertValid() const
{
	CDocument::AssertValid();
}

int testfunc(const int* param[])
{
	const int** p1 = param;

	return 0;
}

void CchartDoc::Dump(CDumpContext& dc) const
{
	int a = 10;
	int b = 20;
	int c = 30;
	int* ab[3] = {0};
	ab[0] = &a;
	ab[1] = &b;
	ab[2] = &c;

	int d = 40;
	int * const cd1[3] = {&a, &b, &c};
	//cd1[0] = &a;

	//cd1[1] = &c;
	*(cd1[1]) = 100;

	testfunc(const_cast<const int**>(ab));

	CDocument::Dump(dc);
}
#endif //_DEBUG


// CchartDoc commands


