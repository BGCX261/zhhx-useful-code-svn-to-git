// chartView.h : interface of the CchartView class
//


#pragma once


class CchartView : public CView
{
protected: // create from serialization only
	CchartView();
	DECLARE_DYNCREATE(CchartView)

// Attributes
public:
	CchartDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CchartView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in chartView.cpp
inline CchartDoc* CchartView::GetDocument() const
   { return reinterpret_cast<CchartDoc*>(m_pDocument); }
#endif

