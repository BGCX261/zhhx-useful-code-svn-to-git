// chartDoc.h : interface of the CchartDoc class
//


#pragma once


class CchartDoc : public CDocument
{
protected: // create from serialization only
	CchartDoc();
	DECLARE_DYNCREATE(CchartDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CchartDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


