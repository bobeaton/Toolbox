// shwdtplt.h   CShwMultiDocTemplate class
// Rod Early
// 1995-05-01

/**********************
I overrode CMultiDocTemplate so as to prevent OpenDocumentFile from setting the
document title automatically.
***/

class  CShwMultiDocTemplate : public CMultiDocTemplate
{

public:
	// Constructor
	CShwMultiDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
	CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass) :
		CMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
		{ ; }
	// Override to prevent it from automatically setting the title.
	virtual CDocument* OpenDocumentFile(LPCTSTR pswzPathName, BOOL bMakeVisible ); // 1.4qzb Make first arg TSTR so Unicode will call this

};