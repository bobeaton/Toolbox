// shwdtplt.cpp

#include "stdafx.h"
#include "toolbox.h"
#include "shwdtplt.h"
#include "shwdoc.h"
#include "shwview.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


CDocument* CShwMultiDocTemplate::OpenDocumentFile(LPCTSTR pswzPathName, BOOL bMakeVisible)
{
    CShwDoc* pDocument = (CShwDoc*)CreateNewDocument(); // 1.4qzd Upgrade pDocument for Unicode build
    if (pDocument == NULL)
    {
        AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
        return NULL;
    }
    ASSERT_VALID(pDocument);

    BOOL bAutoDelete = pDocument->m_bAutoDelete;
    pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong
    CFrameWnd* pFrame = CreateNewFrame(pDocument, NULL);
    pDocument->m_bAutoDelete = bAutoDelete;
    if (pFrame == NULL)
    {
        AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
        delete pDocument;       // explicit delete on error
        return NULL;
    }
    ASSERT_VALID(pFrame);

    if (pswzPathName == NULL)
    {
        // create a new document - with default document name
        UINT nUntitled = m_nUntitledCount + 1;

        Str8 sDocName; // 1.4hed Use CStrg to pass in to MFC function
		CString swDocName; // 1.4qzc Correct Cstrg that called wrong function
        if (GetDocString(swDocName, CDocTemplate::docName) && !swDocName.IsEmpty())
        {
			sDocName =  sUTF8( swDocName ); // 1.4qzc
            char szNum[8];
//            wsprintf(szNum, "%d", nUntitled); // 1.4que Upgrade wsprintf for Unicode build
            _itoa( nUntitled, szNum, 10 ); // 1.4que Upgrade wsprintf for Unicode build
            sDocName += szNum;
        }
        else
        {
            // use generic 'untitled' - ignore untitled count
			sDocName = _("untitled");
        }
        pDocument->SetTitle( sDocName ); // 1.4qzc // 1.4qzd

        if (bMakeVisible && !pDocument->OnNewDocument())
        {
            // user has be alerted to what failed in OnNewDocument
            pFrame->DestroyWindow();
            return NULL;
        }

        // it worked, now bump untitled count
        m_nUntitledCount++;
    }
    else
    {
        // open an existing document
        BeginWaitCursor();
		CString swPathName = pswzPathName; // 1.4quf // 1.4qzb
		Str8 sPathName =  sUTF8( swPathName ); // 1.4quf // 1.4qzb // 1.4qzd
        if (!pDocument->OnOpenDocument( sPathName )) // 1.4quf Upgade OnOpenDocuument for Unicode build // 1.4qzb In Unicode, this is passed in as TSTR // 1.4qzd
	        {
            // user has be alerted to what failed in OnOpenDocument
            // Cancel autoload stream. Moved to shwdoc.cpp 1996-09-05 MRP
            pFrame->DestroyWindow();
            EndWaitCursor();
            return NULL;
	        }
        EndWaitCursor();
    }

    InitialUpdateFrame(pFrame, pDocument, bMakeVisible);
    return pDocument;
}
