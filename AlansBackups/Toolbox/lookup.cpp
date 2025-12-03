//File: lookup.cpp
// Implementation for the CLookupMatchList and CLookupMatch classes.
// See lookup.h for more information.

#include "stdafx.h"
#include "toolbox.h"
#include "lookup.h"
#include "progress.h"

extern const char* pNL; // system dependent newline strings

CLookupMatch::CLookupMatch (const Str8& sNewRef, const Str8& sNewPreBefore, 
	const Str8& sNewBefore, const Str8& sNewTarget, 
	const Str8& sNewPreAfter, const Str8& sNewAfter) : m_sRef(sNewRef),
	m_sPreBefore(sNewPreBefore), m_sBefore(sNewBefore), m_sTarget(sNewTarget), 
	m_sPreAfter(sNewPreAfter), m_sAfter(sNewAfter)
{
}

CLookupMatchList::CLookupMatchList() : CObList()
{
}

CLookupMatchList::~CLookupMatchList()
{
	DeleteAll();
}
 
// An extention of the RemoveAll function that deletes list elements as well
// as removing them from the list.
void CLookupMatchList::DeleteAll()
{
	POSITION pos;
	CLookupMatch *plm;
	pos = GetHeadPosition();
	while (pos != NULL) {
		plm = (CLookupMatch*) GetNext(pos);
		delete plm;
	}
	RemoveAll();
}

// Adds the lookup match given by these four strings to the end of this.
void CLookupMatchList::AddLookupMatch (const Str8& sNewRef, 
	const Str8& sNewPreBefore, const Str8& sNewBefore, 
	const Str8& sNewTarget, const Str8& sNewPreAfter, 
	const Str8& sNewAfter)
{
	AddTail (new CLookupMatch (sNewRef, sNewPreBefore, sNewBefore, sNewTarget,
		sNewPreAfter, sNewAfter));
}

/*
code for ideas...
void FindNextWord (int& iStartAt, Str8& sLine, Str8& sWord)
{
	int iWordEnd;
    int iLineLength = sLine.GetLength();
	while ((iStartAt < iLineLength) && _istspace(sLine[iStartAt]))
		iStartAt++;
	
    if (iStartAt == iLineLength)
		sWord.Empty();
	else 
        {
		for (iWordEnd = iStartAt; (iWordEnd < iLineLength) && !_istspace(sLine[iWordEnd]); iWordEnd++)
				;

		sWord = sLine.Mid(iStartAt, iWordEnd - iStartAt);
		iStartAt = iWordEnd;
	    }  //   else - if
}
*/

// Helper function to derive a reference string with zero-filled numbers
Str8 sZeroFillRef (Str8 sRef)
{
//	in a valid ref assume:
//	single space separates book & chapter; 
//	no space means book, chapter, or both are missing
//	colon separates chapter & verse; no colon means no verse
//	only attempt to zero fill a valid number
//	if any invalidity, just leave that part as is

//	iSpace = sRef.Find (' ');
//	iColon = sRef.Find (':');
//	if (iSpace>-1 && iColon>-1)	// three items
//		if atoi(Left
//	else if (iSpace>-1 || iColon>0)	// two items
//	else if (iColon==0)	 // one item, after colon
//	else		//one item	*/

	Str8 sTemp = sRef;
	Str8 sItem, sOut;
	int  i, n, nItemLen;
	BOOL bColon = FALSE;
	for (i=1; i<=3; i++)		//for max 3 items
		{
//		sItem = sTemp.SpanExcluding (" :");	// get the next item, space and colon are separators // 1.4heh Replace SpanExcluding in ZeroFillRef
		int iSp = sTemp.Find( " " ); // 1.4heh Find space
		if ( iSp == -1 ) // If no space, set to full length
			iSp = sTemp.GetLength();
		int iColon = sTemp.Find( ":" ); // 1.4heh Find colon
		if ( iColon == -1 ) // If no colon, set to full length
			iColon = sTemp.GetLength();
		iSp = __min( iSp, iColon ); // 1.4heh Set to earliest space or colon, full length if neither found
		sItem = sTemp.Left( iSp ); // 1.4heh Set item to part up to the space or colon

		nItemLen = sItem.GetLength();
		n=atoi(sItem);
		if (n!=0 || sItem=="000")	// if a valid number (under 32767)
			if (!bColon)
				sItem.Format("%05d", n);	// make 5 digits (pri, sec refs)
			else
				sItem.Format("%03d", n);	// make 3 digits (255 verse max)
		sOut += sItem;			// append sItem to the output

		n = sTemp.GetLength() - nItemLen;	// count of remaining
		if (n>1)
			{
			sOut += sTemp[nItemLen];		// append separator
			if (sTemp[nItemLen]  == ':')
				bColon = TRUE;
			sTemp = sTemp.Right(n - 1);		// keep the remainder of the input
			}
		else
			break;	// nothing more to do
		}
	return sOut;
}


// Writes the contents of this match list to a SF output file. 
//  fOutput is assumed to already be open in binary (NOT text) mode.  
// The four marker strings are used to make the SFMs.
void CLookupMatchList::WriteToFile (CStdioFileEx& fOutput, const char* pszRefMkr,
			const char* pszBefMkr, const char* pszTargetMkr, 
			const char* pszAftMkr, BOOL bMakeConcRefz/*FALSE*/)
{
	Str8 sBuffer;
	const Str8 sRefMarker =   (Str8)'\\' + pszRefMkr   + ' ';
	const Str8 sRefzMarker =  (Str8)'\\' + pszRefMkr   + "z ";
	const Str8 sBefMarker =   (Str8)'\\' + pszBefMkr   + ' ';
	const Str8 sPreBefMarker =(Str8)'\\' + pszBefMkr   + "x ";
	const Str8 sTargetMarker =(Str8)'\\' + pszTargetMkr+ ' ';
	const Str8 sAftMarker =   (Str8)'\\' + pszAftMkr   + ' ';

	POSITION pos;
	CLookupMatch *plm;
	pos = GetHeadPosition();
    CProgressIndicator prg( GetCount(), NULL,  sUTF8( fOutput.GetFilePath() ) ); // 1.4quq Upgrade GetFilePath for Unicode build
    int iCounter = 0;   
	while (pos != NULL) 
		{
        prg.bUpdateProgress(++iCounter);    
		plm = (CLookupMatch*) GetNext(pos);
		//CHECK on quickest way to write a bunch of strings
		sBuffer = sRefMarker;
		sBuffer += plm->GetRef();
		sBuffer += pNL;
		if (bMakeConcRefz)
			{	//make an extra concref field with zero-filled numbers, for sorting
			sBuffer += sRefzMarker;
			sBuffer += sZeroFillRef(plm->GetRef());
			sBuffer += pNL;
			} 
		if (plm->GetPreBefore().GetLength() &&	//  if prebefore
			plm->GetPreBefore()[0] != '\\')		//  does not begin with a marker
			sBuffer += sPreBefMarker;			//		add our own marker
		sBuffer += plm->GetPreBefore();	// prebefore data includes various markers
		sBuffer += pNL;
		sBuffer += sBefMarker;
		sBuffer += plm->GetBefore();
		sBuffer += pNL;
		sBuffer += sTargetMarker;
		sBuffer += plm->GetTarget();
		sBuffer += pNL;
		sBuffer += plm->GetPreAfter(); // if any, includes markers & nl
//		sBuffer += pNL;
		sBuffer += sAftMarker;
		sBuffer += plm->GetAfter();	// includes following fields
		sBuffer += pNL;
		fOutput.Write(sBuffer, sBuffer.GetLength());
	}
}
