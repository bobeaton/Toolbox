// doclist.h

#ifndef Doclist_H
#define Doclist_H

/////////////////////////////////////////////////////////////////////////////
// CDocList

// This class assumes use of a single template in the app

// see SHW.CPP for some examples of how this class is used

class CShwDoc;
class CDatabaseType;

class CDocList // Hungarian: doclst
{
	POSITION m_posTemplate;
	CDocTemplate* m_pTemplate;
	POSITION m_posDoc;

public:
	CDocList(); // Constructor
	
	CShwDoc* pdocFirst(); // return first open document
	
	virtual CShwDoc* pdocNext(); // return next open document
};  // class CDocList

class CTypedDocList : public CDocList // Hungarian: doclst
{
private:
	CDatabaseType* m_ptyp;
    CShwDoc* m_pdocExclude;

public:
	CTypedDocList(CDatabaseType* ptyp); // Constructor
	
	CShwDoc* pdocFirst(CShwDoc* pdocExclude); // return first open document of this type

   	virtual CShwDoc* pdocNext(); // return next open document of this type
};  // class CTypedDocList

#endif // Doclist_H