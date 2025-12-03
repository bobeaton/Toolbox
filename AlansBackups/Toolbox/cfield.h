// cfield.h Interface for Standard format field (1995-02-28)

#ifndef CFIELD_H
#define CFIELD_H

#include "mkr.h"
#include "cdbllist.h"
#include "tools.h"  // 1997-06-19 MRP: Remove this next time we clean up code
class CCheckingState; // cck.h
class CRecord; // crecord.h
class CRecPos; // crecpos.h

class CField : public CMString, public CDblListEl
{
public:
    CField(CMarker* pmkr, const char* pszContents = "") :
        CMString(pmkr, pszContents) {}
    CField( CField& pfld ) :
        CMString((CMString&)pfld) {}    
    virtual ~CField();
    
    const Str8& sContents() const { return *this; }
//    void TrimContents(); // 1.4qzfj

    // If data is defined to be a single "word," return FALSE and set 
    // message if any embedded whitespace is found. If data field
    // can't be empty and it is, return FALSE and set message.
    // If there is an inconsistency, it also sets the rpsStart and rpsEnd positions
    // to indicate the portion of the data that has the error. Note that if data
    // has multiple words and shouldn't, the inconsistent data spans from the first
    // non-white character to the last non-white character. For missing data, the
    // start and end positions are both set to 0, and any trailing whitespace is
    // left untouched. The values of rpsStart, rpsEnd, and nID are undefined if this
    // function returns TRUE.
    BOOL bValidItemCount(CRecord* prec, CRecPos& rpsStart, CRecPos& rpsEnd, Str8& sMessage, BOOL& bExplicitNewlines); // 1.4heq

    // Set rpsStart to the first non-whitespace character. Set rpsEnd to the position just
    // beyond the last non-whitespace character in the first item, which may be
    // either a word or the entire field contents (based on pmkr->bMultipleItemData). If
    // no data item exists (i.e., field is empty or has only whitespace), the values of
    // rpsStart and rpsEnd are undefined and the function returns FALSE.
    BOOL bParseFirstItem(CRecord* prec, CRecPos& rpsStart, CRecPos& rpsEnd, BOOL bMultipleItemData);
    
    // Set rpsStart to the first non-whitespace character on or after the passed-in rpsEnd value.
    // Set rpsEnd to the position just beyond the last non-whitespace character in the next
    // item. If no more data items exist, the values of rpsStart and rpsEnd are
    // undefined, and this function returns FALSE.
    BOOL bParseNextItem(CRecPos& rpsStart, CRecPos& rpsEnd, BOOL bMultipleItemData);
    
    // Set rpsStart to the first non-whitespace character on or after the passed-in rpsStart
    // value. Set rpsEnd to the position just beyond the last non-whitespace character in the
    // next item. If no more data items exist, the values of rpsStart and rpsEnd are undefined,
    // and this function returns FALSE.
    BOOL bParseCurItem(CRecPos& rpsStart, CRecPos& rpsEnd, BOOL bMultipleItemData);

    // Return a Str8 object representing the item delimited by rpsStart and rpsEnd.
    // Return value is undefined (function will assert) if rpsEnd <= rpsStart.
    Str8 sItem(const CRecPos& rpsStart, const CRecPos& rpsEnd, BOOL bHandleMultiLineData = TRUE);

private:
    // If bCurrent, re-parse the current item and return TRUE. Otherwise, call and
    // return results of bParseNextItem.
    BOOL bParseItem(BOOL bCurrent, CRecPos& rpsStart, CRecPos& rpsEnd, BOOL bMultipleItemData);

#ifdef _DEBUG
public:
    void AssertValid() const {}
#endif  // _DEBUG
};  // class CField

#endif  // CFIELD_H