// chartView.cpp : implementation of the CchartView class
//

#include "stdafx.h"
#include "chart.h"

#include "chartDoc.h"
#include "chartView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CchartView

IMPLEMENT_DYNCREATE(CchartView, CView)

BEGIN_MESSAGE_MAP(CchartView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CchartView construction/destruction

CchartView::CchartView()
{
	// TODO: add construction code here

}

CchartView::~CchartView()
{
}

BOOL CchartView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CchartView drawing

void CchartView::OnDraw(CDC* /*pDC*/)
{
	CchartDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CchartView printing

BOOL CchartView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CchartView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CchartView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CchartView diagnostics

#ifdef _DEBUG
void CchartView::AssertValid() const
{
	CView::AssertValid();
}

void CchartView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CchartDoc* CchartView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CchartDoc)));
	return (CchartDoc*)m_pDocument;
}
#endif //_DEBUG


// CchartView message handlers
