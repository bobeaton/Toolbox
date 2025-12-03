// number.cpp Implementation of numbering functions (1999-03-01)
//            found in Tools menu: Break/Number Text
//            and Renumber Text.

#include "stdafx.h"
#include "toolbox.h"
#include "shwdoc.h"
#include "number.h"
#include "progress.h"
#include "resource.h"
#include "ind.h"
#include "obstream.h"  // Object_ostream, Object_istream
#include "crlookel.h"
#include "crecord.h"
#include "cfield.h"
#include "mkr.h"
#include "shwview.h"

#include "number_d.h"
#include "typ_d.h"  // CDatabaseTypeSheet page numbers

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

CNumberProcess::CNumberProcess(CDatabaseType* ptypMyOwner, CMarker* pmkrRef, CMarker* pmkrTxt) :
	m_ptypMyOwner(ptypMyOwner),
	m_sBreakChars(".!?"),
	m_pmkrRef(pmkrRef),
    m_pmkrTxt(pmkrTxt),
    m_subsetTextBreakMarkers(ptypMyOwner->pmkrset(), FALSE)
{
	ASSERT ( m_ptypMyOwner );
	// Set defaults
	m_bWholeDocument = FALSE;
	m_bBreakText = FALSE;
	m_bGetNameFromMkr = TRUE;
	m_sName = "";
	m_pmkrUseContentsAsName = ptypMyOwner->pmkrRecord();
	m_iStartRef = 1;
	m_iNumberOfDigits = 3;
	// The following was originally going to be set by user using a check box in the Save First dialog box, but we decided to leave it as an undocumented feature for power users only. It can be set to true by manually editing database type file.
	m_bDontPromptForSave = FALSE;
}

CNumberProcess::~CNumberProcess()
{
}

static const char* psz_nbrproc = "numbering";
static const char* psz_mkrRef = "mkrRef";
static const char* psz_mkrTxt = "mkrTxt";
static const char* psz_subsetTBM = "subsetTextBreakMarkers";
static const char* psz_bDontPromptForSave = "DontPromptForSave";

#ifdef typWritefstream // 1.6.4ac
void CNumberProcess::WriteProperties(Object_ofstream& obs) const
#else
void CNumberProcess::WriteProperties(Object_ostream& obs) const
#endif
{
    obs.WriteBeginMarker(psz_nbrproc);
	if ( m_pmkrRef )
        obs.WriteString(psz_mkrRef, m_pmkrRef->sName());
    if ( m_pmkrTxt )
        obs.WriteString(psz_mkrTxt, m_pmkrTxt->sName());
	obs.WriteBool(psz_bDontPromptForSave, m_bDontPromptForSave);

    obs.WriteBeginMarker(psz_subsetTBM);

    m_subsetTextBreakMarkers.WriteProperties(obs);
    
    obs.WriteEndMarker(psz_subsetTBM);

	obs.WriteEndMarker(psz_nbrproc);
}

BOOL CNumberProcess::s_bReadProperties(Object_istream& obs, CDatabaseType* ptypMyOwner, CNumberProcess** ppnbrproc)
{
    if ( !obs.bReadBeginMarker(psz_nbrproc) )
        return FALSE;

    CNumberProcess* pnbrproc = new CNumberProcess( ptypMyOwner );
    pnbrproc->ReadProperties(obs);
    ASSERT( ppnbrproc );
	ASSERT( !*ppnbrproc );
    if ( *ppnbrproc )
        delete *ppnbrproc; // This should never happen!
	if ( pnbrproc->m_pmkrRef == NULL || pnbrproc->m_pmkrTxt == NULL )
		{
		delete pnbrproc; // Settings file didn't contain valid ref and text markers
		pnbrproc = NULL;
		}
    *ppnbrproc = pnbrproc;

    return TRUE;
}

void CNumberProcess::ReadProperties(Object_istream& obs)
{
    Str8 s;

    while ( !obs.bAtEnd() )
        {
	    if ( obs.bReadString( psz_mkrRef, s ) )
            m_pmkrRef = m_ptypMyOwner->pmkrset()->pmkrSearch_AddIfNew( (const char*)s );
        else if ( obs.bReadString( psz_mkrTxt, s ) )
            m_pmkrTxt = m_ptypMyOwner->pmkrset()->pmkrSearch_AddIfNew( (const char*)s );
		else if ( obs.bReadBool(psz_bDontPromptForSave, m_bDontPromptForSave) )
			;
        else if ( m_subsetTextBreakMarkers.bReadProperties(obs) )
			;
        else if ( obs.bEnd(psz_nbrproc) )
            break;
        }
}

void CNumberProcess::MarkerUpdated(CMarkerUpdate& mup, CNumberProcess** ppnbrproc)
{
	CNumberProcess* pnbrproc = (CNumberProcess*)*ppnbrproc;
	ASSERT ( pnbrproc );
    ASSERT ( mup.pmkr()->pmkrsetMyOwner() == pnbrproc->m_ptypMyOwner->pmkrset() );
    if ( mup.bToBeDeleted() )
		{
		if ( mup.pmkr() == pnbrproc->m_pmkrRef || mup.pmkr() == pnbrproc->m_pmkrTxt )
			{
			delete *ppnbrproc;
			*ppnbrproc = pnbrproc = NULL;
			return;
			}
		if ( mup.pmkr() == pnbrproc->m_pmkrUseContentsAsName )
			pnbrproc->m_pmkrUseContentsAsName = pnbrproc->m_ptypMyOwner->pmkrRecord();
		}
	if ( mup.bLangEncModified() )
		{
		if ( mup.pmkr() == pnbrproc->m_pmkrTxt )
			{
			pnbrproc->m_subsetTextBreakMarkers.IncludeAllElements(FALSE);
			}
        else
            {
            pnbrproc->m_subsetTextBreakMarkers.Include( mup.pmkr(), FALSE );
            }
		}
	if ( mup.bModified() && mup.pmkrMerge() != NULL )
		{
		if ( mup.pmkr() == pnbrproc->m_pmkrRef )
			{
			if ( mup.pmkrMerge() == pnbrproc->m_pmkrTxt ||
				 mup.pmkrMerge() == pnbrproc->m_ptypMyOwner->pmkrRecord() ||
				 mup.pmkrMerge() == pnbrproc->m_ptypMyOwner->pmkrDateStamp() )
				{
				delete *ppnbrproc;
				*ppnbrproc = pnbrproc = NULL;
				return;
				}
			}
		else if ( mup.pmkr() == pnbrproc->m_pmkrTxt )
			{
			if ( mup.pmkrMerge() == pnbrproc->m_pmkrRef ||
				 mup.pmkrMerge() == pnbrproc->m_ptypMyOwner->pmkrRecord() ||
				 mup.pmkrMerge() == pnbrproc->m_ptypMyOwner->pmkrDateStamp() )
				{
				delete *ppnbrproc;
				*ppnbrproc = pnbrproc = NULL;
				return;
				}
			}
		if ( mup.pmkr() == pnbrproc->m_pmkrUseContentsAsName )
			{
			if ( mup.pmkrMerge() == pnbrproc->m_ptypMyOwner->pmkrDateStamp() )
				pnbrproc->m_pmkrUseContentsAsName = pnbrproc->m_ptypMyOwner->pmkrRecord();
			else
				pnbrproc->m_pmkrUseContentsAsName = mup.pmkrMerge();
			}
		}
}

void CNumberProcess::s_CreateInstance(CDatabaseType* ptypMyOwner, CNumberProcess** ppnbrproc, CMarker* pmkrRef, CMarker* pmkrTxt)
{
	ASSERT ( pmkrRef );
	ASSERT ( pmkrTxt );
	CNumberProcess* pnbrproc = new CNumberProcess( ptypMyOwner, pmkrRef, pmkrTxt );
    ASSERT( ppnbrproc );
	ASSERT( !*ppnbrproc );
    if ( *ppnbrproc )
        delete *ppnbrproc; // This should never happen!
    *ppnbrproc = pnbrproc;
}


void CNumberProcess::s_CreateInstance(CDatabaseType* ptyp, CNumberProcess** ppnbrproc)
{
	CSetupNbrDlg dlg( ptyp->sName() );
    dlg.DoModal();
    ptyp->bViewProperties(CDatabaseTypeSheet::eNumberingPage);
}

BOOL CNumberProcess::bNumber(CShwView* pview, CRecLookEl* prelCur, BOOL bDoBreak)
{
    ASSERT ( pview );
	ASSERT( prelCur );
	ASSERT( m_pmkrRef );
	BOOL bChanged;

	m_bBreakText = bDoBreak;
	CShwDoc* pdoc = pview->GetDocument();
	ASSERT( pdoc );

	// Put up dialog box to get options
	if ( bDoBreak )
		{
        // Default marker to use for getting text name
		if ( !m_pmkrUseContentsAsName )
			m_pmkrUseContentsAsName = m_ptypMyOwner->pmkrRecord();
		CNumberDlg dlg( this, prelCur->prec()->pfldFirst() );
        if ( dlg.DoModal() != IDOK )
            return FALSE;
        }
	else
		{
        CReNumberDlg dlg( this, prelCur->prec()->pfldFirst() );
        if ( dlg.DoModal() != IDOK )
            return FALSE;
		m_bGetNameFromMkr = TRUE;
		m_pmkrUseContentsAsName = m_pmkrRef;
        }

	m_iRefNum = m_iStartRef;

	if ( m_bWholeDocument )
		{
        if ( !pdoc->bValidateViews() ) // Validate all views
            return FALSE;
		bChanged = bReNumberDocument( pdoc );
		if ( bChanged )
			pdoc->ResetUndoForAllViews(); // clear undo buffers for all views of this document
		}
	else
		{
		bChanged = bReNumberRecord( pdoc, prelCur );
		if ( bChanged )
			pdoc->ResetUndoForAllViews( prelCur->prec() ); // clear undo buffers for all views of this document currently editing this record
		}

	return bChanged;
}

BOOL CNumberProcess::bReNumberDocument( CShwDoc* pdoc )
{
    ASSERT( pdoc );
	ASSERT( m_pmkrRef );
    ASSERT( m_pmkrTxt );
	ASSERT( !m_bGetNameFromMkr || m_pmkrUseContentsAsName );

	BOOL bChanged = FALSE;
    CIndex* pind = pdoc->pindset()->pindRecordOwner(); // use index that owns the document's records to march thru database
    CProgressIndicator prg(pind->lGetCount(), NULL,  sUTF8( pdoc->GetPathName() ), TRUE); // 1.4ad Eliminate resource messages sent to progress bar // 1.4qup Upgrade GetPathName for Unicode build
    long int lRec=0;
    for ( CNumberedRecElPtr prel = pind->pnrlFirst(); prel; prel = pind->pnrlNext( prel ), lRec++ )
        {
        if ( !prg.bUpdateProgress(lRec) ) // 1.2gr
            break;
		bChanged |= bReNumberRecord( pdoc, prel ); // Renumber this record
        }
	return bChanged;
}

BOOL CNumberProcess::bReNumberRecord( CShwDoc* pdoc, CRecLookEl* prel )
{
	ASSERT( pdoc );
    ASSERT( prel );
	ASSERT( m_pmkrRef );
    ASSERT( m_pmkrTxt );
	ASSERT( !m_bGetNameFromMkr || m_pmkrUseContentsAsName );

	BOOL bChanged = bReNumberRecord( prel, pdoc->pindset() ); // (re-)number current record

    if ( bChanged )
		{
        pdoc->RecordModified( prel ); // let everyone know we modified this record
		}
	return bChanged;
}

BOOL CNumberProcess::bReNumberRecord( CRecLookEl* prel, CIndexSet* pindset )
{
    CRecord* prec = prel->prec();
	ASSERT( prec );
	BOOL bChanged = FALSE;
	BOOL bNewRecord = TRUE;
	BOOL bBogusRef;
    BOOL bInsertRef = TRUE; // This will be set to FALSE if a valid ref is encountered
	CField* pfld;
	for (pfld = prec->pfldFirst(); pfld; pfld = prec->pfldNext(pfld) )
		{
		if ( pfld->pmkr() == m_pmkrUseContentsAsName && m_bGetNameFromMkr )
			{
			GetNameFromMkr( pfld, bNewRecord, ( m_pmkrUseContentsAsName == m_pmkrRef ) );
			bNewRecord = FALSE;
			}
		if ( pfld->pmkr() == m_pmkrRef )
			{
			bBogusRef = FALSE;
			CField* pfldNext = prec->pfldNext(pfld);
			if ( pfldNext == NULL )
				bBogusRef = TRUE;
			else
				{
				if ( pfldNext->pmkr() == m_pmkrRef ||  // A ref followed immediately by a ref is bogus
                     ( m_bBreakText && pfldNext->pmkr() != m_pmkrTxt && // When doing text breaking, if next marker is to be broken, and
                       m_subsetTextBreakMarkers.bIncludes(pfldNext->pmkr()) )) //  it isn't the text marker, delete leading ref marker
					bBogusRef = TRUE;
				else
                    {
					bChanged |= bUpdateRef( pfld );
                    bInsertRef = FALSE;
                    }
				}
			if ( bBogusRef )
				{
				CField* pfldBogusRef = pfld;
				pfld = prec->pfldPrev( pfldBogusRef );
                bChanged = TRUE; // 01/06/2000 - TLB: Added this to reflect the fact
                                 // that deleting a ref field is a change to the record.
                // 1999-12-16 - TLB: This code was added thinking it might solve some problems
                // with insertion point and browse view getting messed up when fields are deleted.
                // It doesn't seem to help, but it is still presumably better than the previous
                // way of deletingn the bogus ref field: prec->Delete( pfldBogusRef );
                pindset->DeleteField( prel, pfldBogusRef );
				}
			}
		else if ( m_bBreakText )
			{
			if ( m_subsetTextBreakMarkers.bIncludes( pfld->pmkr() ) )
                {
				bChanged |= bBreakFieldText( prec, &pfld, bInsertRef );
                bInsertRef = TRUE; // This will be set to FALSE if a valid ref is encountered
                }
			}
		}
	return bChanged;
}

// Set bInsertRef to TRUE if a ref field is needed
BOOL CNumberProcess::bBreakFieldText( CRecord* prec, CField** ppfld, BOOL bInsertRef )
{
	ASSERT( prec );
	BOOL bChanged = FALSE;
	Str8 sSentence, sTxtFldContents;
	CField* pfld = *ppfld;
	CMarker* pmkrNext = NULL;
    CField* pfldNext = prec->pfldNext(pfld);
	if ( pfldNext )
		{
		pmkrNext = pfldNext->pmkr();
		// If next field exists and is an interlinear field other than the text field, we don't break it.
		if ( pmkrNext->bInterlinear() && pmkrNext != m_pmkrTxt )
			return FALSE;
		}

	sTxtFldContents = pfld->sContents();
	if ( pfld->pmkr() != m_pmkrTxt && !sTxtFldContents.IsEmpty() )
		{
		// This code accomplishes the following:
		//     BEFORE:                  |    AFTER:
		// pfld-> \p Text to be broken. |        \p
		//                              | pfld-> \t Text to be broken.
		CField* pfldNew = new CField( m_pmkrTxt, sTxtFldContents );
		ASSERT( pfldNew );
		prec->InsertAfter( pfld, pfldNew );
		pfld->SetContent( "" );
		pfld = pfldNew;
		bChanged = TRUE;
		}
	while ( !sTxtFldContents.IsEmpty() )
		{
		if ( bInsertRef )
			{
			Str8 sNewContents = m_sName;
			FormatRefNum( sNewContents );
			CField* pfldNew = new CField( m_pmkrRef, sNewContents );
			ASSERT( pfldNew );
			prec->InsertBefore( pfld, pfldNew );
			bChanged = TRUE;
			}
		BreakText( sTxtFldContents, sSentence );
		if ( sTxtFldContents.IsEmpty() )
			{
			if ( pfld->sContents() != sSentence )
				bChanged = TRUE;
				pfld->SetContent( sSentence );
			}
		else
			{
			CField* pfldNew = new CField( m_pmkrTxt, sSentence );
			ASSERT( pfldNew );
			prec->InsertBefore( pfld, pfldNew );
			// No need to set bChanged to TRUE here because it will be set in next loop through when Ref marker is inserted
			bInsertRef = TRUE;
			}
		}
    *ppfld = pfld;
	return bChanged;
}

void CNumberProcess::GetNameFromMkr( CField* pfld, BOOL bNewRecord, BOOL bDiscardNumericName )
{
	Str8 sNewContents = pfld->sContents();
	// This code is to make "PIGS 003" into "PIGS", but leave "PIGS3" as is.
	BOOL bFoundSpace = FALSE;
	int iGoodThru = 0;
	for (int i = 0; i < sNewContents.GetLength(); i++)
		{
		switch( sNewContents[i] )
			{
			case ' ':
			case '.':
			case '\n': // 1.4zbe Fix bug of text numbering including nl in ref
				bFoundSpace = TRUE;
				bDiscardNumericName = FALSE;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if ( !bFoundSpace )
					iGoodThru = i + 1;
				break;
			default:
				iGoodThru = i + 1;
				bFoundSpace = bDiscardNumericName = FALSE;
			}
		}
	
	if ( bDiscardNumericName )
		iGoodThru = 0;

	if ( iGoodThru == 0 && !bNewRecord )
		{
		sNewContents = m_sName;
		}
	else if ( m_sName != sNewContents.Left(iGoodThru) )
		{
		m_sName = sNewContents.Left(iGoodThru);
		m_iNumberOfDigits = __max( m_iNumberOfDigits, sNewContents.GetLength() - iGoodThru - 1 ); // 5.97s Make it handle 4 digit numbers if requested
		m_iRefNum = m_iStartRef;
		m_iStartRef = 1;
		}
}

void CNumberProcess::BreakText( Str8& sTxt, Str8& sSentence )
{
	BOOL bFoundBreakChar = FALSE;
	BOOL bStrippingWhitespace = TRUE;

	sSentence.Empty();

	for (int i = 0; i < sTxt.GetLength(); i++)
		{
		if  ( m_pmkrTxt->plng()->bIsWhite( sTxt[i] ) )
			{
			if ( bFoundBreakChar )
				{
				sTxt = sTxt.Mid(i+1);
				sTxt.TrimLeft();
				return;
				}
			if ( !bStrippingWhitespace )
				{
				sSentence += ' ';
				bStrippingWhitespace = TRUE;
				}
			}
		else 
			{
			bStrippingWhitespace = FALSE;
			if ( bBreakChar( sTxt[i] ) )
                {
			    sSentence += sTxt[i];
				bFoundBreakChar = TRUE;
                }
            // If we've already found a break char, but we haven't yet found whitespace, then if we
            // find another word-formation character first, all bets are off and we shouldn't break
            // the text. This covers the following types of scenarios:
            //   ?You did what? I went to the zoo.
            //   (!"Where"?) Over there.
            // It does not cover the following:
            //   "Where is she?", he asked.
			else 
                {
                char const *psz = (const char*)sTxt + i;
                if ( m_pmkrTxt->plng()->bIsWdCharMove( &psz ) ) // could be a multigraph
                    {
                    int nCharLen = psz - ((const char*)sTxt + i);
                    sSentence += sTxt.Mid(i, nCharLen);
                    i += nCharLen - 1; // subtract one because for-loop will increment it.
                    bFoundBreakChar = FALSE;
                    }
                else // undefined character (probably punctuation). Assume single-width character.
                    sSentence += sTxt[i];
				}
			}
		}
	sTxt.Empty();
}

BOOL CNumberProcess::bUpdateRef( CField* pfld )
{
	ASSERT( pfld );
	ASSERT( m_iRefNum > 0 );
	BOOL bChanged = FALSE;
	Str8 sNewContents = m_sName;
	FormatRefNum( sNewContents );
	if ( pfld->sContents() != sNewContents )
		{
		pfld->SetContent( sNewContents );
		bChanged = TRUE;
		}
	return bChanged;
}

void CNumberProcess::FormatRefNum( Str8& sContents )
{
	Str8 sRefNum;	// 1.3aj No need to check for max reference number because int is 32 bit
	sRefNum.Format( ".%.*u", m_iNumberOfDigits, m_iRefNum++ ); // 5.9g Change to period instead of space for better jump ref
	sContents += sRefNum;
	sContents.TrimLeft();
}

BOOL CNumberProcess::bBreakChar( char ch)
{
	for (int i = 0; i < m_sBreakChars.GetLength(); i++)
		if ( ch == m_sBreakChars[i] )
			return TRUE;
	return FALSE;
}