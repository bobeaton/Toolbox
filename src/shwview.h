// shwview.h : interface of the CShwView class
//
/////////////////////////////////////////////////////////////////////////////

#include "recset.h"
#include "hints.h"
#include "shwvscrl.h"
#include "crecpos.h"
#include "browse.h"
#include "undo.h"
#include "shwdoc.h"
#define TEXTOUT_NORMAL      1
#define TEXTOUT_HIGHLIGHT   2
// void TextOut( CDC* pDC, CField* pfld, const char* psz, int iLen, int iSelect=0 );  // TextOut() with flag for drawing selected text // 1.4qtj Disable unused TextOut

//09-09-1997 Changed the parameterlist. If pmkr = NULL the color of the 
// plng() is used, otherwise the color of the marker
void TextOutClip( CDC* pDC, CLangEnc* plng, const char* psz, int iLen, 
                  CRect* prect, CMarker* pmkr = NULL, 
                  int iSelect = 0, UINT nOptions = ETO_OPAQUE | ETO_CLIPPED,  
                  int ixBegin = 0, int iyBegin = 0);  
// TextOut() with flag for drawing selected text

class Object_istream;  // obstream.h
class Object_ostream;  // obstream.h

class CShwStatusBar;  // status.h

class CMarkerUpdate;
class CFilterUpdate;
class CLangEncUpdate;
class CSortOrderUpdate;

class CFilterSetComboBox;
class CFindSettings;

class CRecPtr; // shwvdis.cpp
class CRecPtrList;

class CFieldConsistencyChecker;  // cck.h

class CShwView : public CShwScrollView
{
private:
    CIndexPtr m_pind;
    CIndexPtr m_pindUnfiltered;  // same as m_pind if not using a filter
    CKey m_keySearch;  // Current key to search for
    CKey m_keyInsert;  // Key to insert
//    CKey m_keyJump; // key to jump to
//    CKey m_keyJumpInsert; // key to jump insert

    CNumberedRecElPtr m_prelCur; // Current record
    CRecPos m_rpsCur; // Current caret position
    
    CRecPos m_rpsSelOrigin; // position in record where selection began

    CLPoint m_pntCaretLoc; // Location of caret cursor in window

    BOOL m_bIsJumpTarget; // if TRUE, this view is designated as a jump target
    BOOL m_bFocusTarget;  // 1.4qzjb Use focus target as target for parallel move

    BOOL m_bViewLocked;  // 1.2bn Lock project also locks active views so they can't be closed

	int m_iVerifyPassNumber; // Verify pass number 5.97x
	BOOL m_bVerifySkippedFail; // Verify skipped at least one failure 5.96zd

    int m_iFldStartCol; // Field start column on screen
    int m_iMaxLineWd;   // The maximum width(length) of a line in current record.  
    int m_iInterlinCharWd; // Char width to use for interlinear display, comes from document type
#define iMaxInterlinLineLen 10000
	int m_iPixelDisplayStarts[iMaxInterlinLineLen + 1]; // 6.0a Calculated positions for starts of displays of interlinear pieces
	CRecPos m_rpsPixelDisplayStarts; // 6.0a First line of currently calculated display pixel starts
	BOOL m_bRecalculatePixelDisplayStarts; // 6.0c Force recalculate except when it is known to be safe
    
    CRecPos m_rpsUpperLeft; // Position of first char visible in window
    long int m_lVPosUL; // vertical pixel offset of first visible line in window
    long int m_lTotalHt; // total height of record in pixels
	BOOL m_bScrollOverflow; // record is to long to scroll properly	  BW 2/3/98
	BOOL m_bScrollOverflowWarning; // user has been warned
    
    int m_iCaretHt; // height in pixels of caret when last displayed
    BOOL m_bCaretHidden; // current state of caret
    BOOL m_bHaveFocus; // if TRUE, this view has input focus

    int m_iXWidth; // current width of client area in pixels

	int m_iMaxLineLength;  // 07-31-1997 After a line exceeds MaxLineLength
	                       // charakters , it will be always wrapped
	BOOL m_bLineLengthMessageBoxShowed;// 07-31-1997
		                               // This Variable will become TRUE
		                               // if a Line exceeds more than 1000 characters
    
    BOOL m_bViewMarkers; // Show marker in display
    BOOL m_bViewMarkerHierarchy; // Indent marker to show hierarchy
    BOOL m_bViewFieldNames; // Show style name in display
                                                            
    BOOL m_bDontCallSetCaretPosAdj; // used to inhibit SetCaretPosAdj() call in DrawRec()

    BOOL m_bModifiedSinceValidated;
        // This view's current record has been modified since it was
        // last validated. Validate it _before_ calling SetCur.
    BOOL m_bCanUndoAll; // changes have been made to current record. Not cleared by validation
    char* m_pszUndoAllBuffer; // buffer containing contents of current record
    CRecord* m_precUndoAll; // used to know if we're really moving to a new record
    CUndoList m_undlst; // list of actions for individual undo. First item to be undone is at end of list

    enum { eText = 1, eLines = 2, eFields = 4, eAnyText = eText|eLines|eFields };
    int m_iSelection; // determines type of selection in process - TEXT within a field or whole LINES
    enum { eDivider = 8, eAnything = eDivider | eAnyText };
    int m_iTracking; // if mouse input is captured, this will be one of the above values
    
    int m_eInitialShowCmd; // The state of the window when Shoebox was closed.
    int m_z; // The z-order of the MDI child frame corresponding to this view.
    
    // Copies of the current record element's record field contents and
    // also the key field contents.
    // These are kept for display on the status bar, even if the actual
    // fields change or are deleted.
    Str8 m_sRecordFld;
    Str8 m_sKeyFld;
    
    int m_iWhatChanged; // keep track of what changed so we know what needs redrawn
    enum { eNothing=0, eSingleLine, eMultipleLines };
    
	Str8 m_sUnrecognizedSettingsInfo; // 1.0cp Don't lose settings info from later versions
	BOOL m_bUnicodeValidityCheckInProcess; // 1.4hr Remember if Unicode validity check is in process

    // browse member variables
    int m_iBrowseHeaderHeight;  // 09-10-1997 Stores the hight of the Browsefieldheader
    BOOL m_bBrowsing; // Indicates view type - Browse or edit view
    CBrowseFieldList m_brflst; // list of fields and width of columns to display them in    
    CNumberedRecElPtr m_prelUL; // first visible record in browse view
    enum { eDividerBar = 1, eDividerPad = 3, eBrowseDividerWidth = eDividerBar+eDividerPad*2,
            eDividerTextPad = 2, eRecordDividerWidth = eDividerBar+eDividerPad*2+eDividerTextPad };

	// Initialization member variables used only to set current position on initialize
	int m_iInitLine; // Line number
	int m_iInitLineScroll; // Number of lines from current line to upper left
	int m_iInitChar; // Character number
    
    // NOTE NOTE NOTE: As new member variables are added to this class,
    // be sure to account for them in bDuplicateView if necessary.
    
    static CShwView* s_pViewToDuplicate;
    static Object_istream* s_pobsAutoload;
    static CRecLookEl* s_prelJumpTo;
    static const char* s_pszJumpInsert; // key to insert
    static CShwDoc* s_pdocJumpTo;
    static CIndex* s_pindJumpTo;

    // RNE 1996-11-01
    UINT m_hWavDevice;  // handle of .wav device.

    CPatMChar* m_ppat; // pattern used in find and replace
    CRecPtrList* m_prcplst; // list of record pointers used for replace only
    CRecPtr* m_prcpCur; // keeps track of current record if using m_prcplst

    CFieldConsistencyChecker* m_pcck;  // 1999-06-11 MRP

	CShwDoc* m_pdocDefaultForMove; // 1.5.3c Remember default for move

    // 2000/05/01 TLB & MRP - SetIndex calls SetCur directly to reset m_prelCur
        // If setting to use filtered index which is out-of-date, reapply the filter.
        // No longer return via pprelCur the first record element in pind
        // which represents the same record it had referenced. 
    // Change the index to pind and update the index reference counts.
    void  SetIndex(CIndex* pind, CIndex* pindUnfiltered /*,
            CNumberedRecElPtr* pprelCur */);

protected: // create from serialization only
    CShwView();
    DECLARE_DYNCREATE(CShwView)

// Attributes
public:
//	void CShwView::CloseWindow(); // 1.2bn Catch close window to prevent it if view is locked
//	void CShwView::OnClose(); // 1.2bn Catch close window to prevent it if view is locked

    CShwDoc* GetDocument() const // Document of view, Microsoft name, not hungarian
        { return (CShwDoc*)m_pDocument; }
    CShwDoc* pdoc() const // Document of view, Hungarian form like SHW uses
        { return (CShwDoc*)m_pDocument; }
    CDatabaseType* ptyp() const // Database type of document being viewed
        { return pdoc()->ptyp(); }

#ifdef prjWritefstream // 1.6.4aa 
    void WriteProperties(Object_ofstream& obs); // Save (Project) Settings
#else
    void WriteProperties(Object_ostream& obs); // Save (Project) Settings
#endif
    BOOL bReadProperties(Object_istream& obs); // Autoloading
    static void s_SetObjectStream(Object_istream& obs);
    static void s_ClearObjectStream();

    void ShowInitialStateAndZ();
    void ShowInitialTopState();
        
    int z() const { return m_z; }
    void SetZ(int z) { m_z = z; }
        // The bottom window's z is zero.
        // Greater z means above; lesser z means below.

    BOOL bExternalJump(const char* pszWordFocusRef, CNumberedRecElPtr prelFrom, BOOL bMultMatchMessage = TRUE, BOOL bJump = TRUE ); // 1.4qzjf
    	// 1998-09-24 MRP: If this window has the Focus Target property,
    	// it responds to focus synchronization requests from other programs.

    CMDIChildWnd* pwndChildFrame() const;
    static CShwView* s_pviewActiveChild(CMDIChildWnd* pwnd);

    void Update(CUpdate& up);
    void MarkerUpdated(CMarkerUpdate& mup);
    void FilterUpdated(CFilterUpdate& fup);
    void LangEncUpdated(CLangEncUpdate& lup);
    void SortOrderUpdated(CSortOrderUpdate& sup);
    
    void UpdateStatusBar(CShwStatusBar* pStatus);
    void UpdateFilterSetCombo(CFilterSetComboBox* pcboFilterSet); // set contents and current filter in filter combo on toolbar
    
    long lGetRecordCount() const { return m_pind->lNumRecEls(); } // return total number of records in record set
    long lGetCurrentRecNum() const { return m_prelCur.lNum() + 1; } // return current record number
    CNumberedRecElPtr prelCur() const { return m_prelCur; } // 1.1bu Current record
    
    CIndex* pind() const { return m_pind; }
    BOOL bBrowsing() const { return m_bBrowsing; }
    CBrowseFieldList* pbrflst() { return &m_brflst; } // 1.2bq Make not const so as to allow modification from other places
    
    BOOL bIsJumpTarget() const { return m_bIsJumpTarget; } // return TRUE if this view is a jump target
    BOOL bIsFocusTarget() const { return m_bFocusTarget; } // return TRUE if this view is a focus target
    BOOL bCanBeJumpTarget() const; // return TRUE if this view could be a jump target. i.e. not filtered, etc.
    void JumpInsert( const char* pszKey ); // insert new record and go to it

    BOOL bViewLocked() const { return m_bViewLocked; } // 1.2bn
	void SetViewLocked( BOOL b ) { m_bViewLocked = b; } // 1.2bn

    afx_msg void OnEditSubfieldMarker();  // 1996-10-14 MRP

// Operations
private:

    // browse view implementation
    void InitBrowse(BOOL bCenterBrowseCur=TRUE); // find current record and center it in view
    void BrowseScrollToCur(); // make so current record is visible in browse view
    void BrowseSetScroll( BOOL bCalc ); // Set scroll sizes, calculate if bCalc is true
    int iBrowseWidth(); // Get total width of browse view by summing field widths and divider widths
    void VerifyBrowseFields(); // Make sure browse field widths match window size, etc.
    void ResizeBrowse(); // handle window size change for browse
    void ResetBrowseFields(); // Set browse fields to default state - fields listed in sort order
    void ResetBrowseFieldWidths(); // Set browse field widths to default - all equal based on windows size
    CBrowseField* pbrfPtInDivider( CPoint pnt ); // return browse field containing point or NULL
    BOOL bPtInDivider( CPoint pnt ); // Is point in a vertical divider?
    void StartMovingDivider( CPoint pnt ); // setup to allow user to move vertical divider bars
    void MoveDivider( CPoint pnt ); // try to move divider to pnt.x
    int iDownRec( CNumberedRecElPtr* pprel ); // move rel down a record returning height of line moved from
    int iUpRec( CNumberedRecElPtr* pprel ); // move rel up a record returning height of line moved to
    long lBrowseVScroll( long lPixels, BOOL bForce=FALSE ); // perform actual window scroll of at least iPixels
    void BrowseOnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll ); // handle scroll bar manipulation
    void DrawBrowse( CDC* pDC ); // Draw the record set
    void DrawBrowseLine( CDC* pDC, int iLine, CNumberedRecElPtr prel, BOOL bHilite, BOOL bDrawDividers=FALSE ); // Draw a single browse line
    void DrawBrowseField( CDC* pDC, const CRect& rect, int y,
            const CField* pfld, BOOL bHighlight, BOOL bDrawEnd );
    void DrawBrowseDividers( CDC* pDC, CRect* prectClip ); // Draw the vertical field dividers
    void RememberCurLine(); // Remember current line position, for redrawing after current line changes
    void DrawPrevAndCurLines(); // redraw line moved from and line moved to, to change hiliting
    BOOL bBrowseMoveDown(); // Move down one line in browse
    BOOL bBrowseMoveUp(); // Move up one line in browse
    void BrowseMoveCtrlHome(); // Ctrl Home, beginning of record set, core primitive
    BOOL bBrowseUp(); // Up arrow
    BOOL bBrowseDown(); // Down arrow
    BOOL bBrowsePgUp(); // page up
    BOOL bBrowsePgDn(); // page down
    void BrowseCtrlHome(); // Ctrl Home, beginning of record
    void BrowseCtrlEnd(); // Ctrl End, end of record set
    void BrowseMouseLeftClick( UINT nFlags, CPoint pnt ); // Mouse left button click
    void BrowseEditMouseDblClk( UINT nFlags, CPoint pnt ); // Mouse left button double click
    void CountRecords(); // Set total record count and current record number
    void BrowseOnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    void BrowseOnLButtonDown(UINT nFlags, CPoint point);
    void BrowseOnLButtonDblClk(UINT nFlags, CPoint point);
    void BrowseOnRButtonDown(UINT nFlags, CPoint point);   //09-12-1997
public: // 1.4tec Allow word list to force browse view
    void SwitchToBrowseView();
    void SwitchToRecordView();
    int iMargin() { return m_pind->pindsetMyOwner()->iMargin(); } // return margin setting for this doc // 1.5.8a Make public so XML export can call it
private:
    void BrowseOnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    
    CMarker* pmkrBrowsePtInMarker(CPoint point);    //09-12-1997
    // This function returns a pointer to the Marker which belongs to 
    // the row including the CPoint Object 
      
    // record view implementation
    int iMeasureRecord( BOOL bCalc ); // return width of record in pixels - length is set in m_lTotalHt
    void SetScroll( BOOL bCalc ); // Set scroll sizes, calculate if bCalc is true
    void GetFontParams(); // Set line height and average char width member variables
    long lLineCountAndMeasure();  // Set line count and return record length
    void SaveCaretPos(); // Save caret position so it can be restored
    void RestoreCaretPos(); // Restore caret position from save
    void CheckShiftKey( UINT nChar ); // handle starting selection, etc. based on SHIFT key state
    void Update(CHint* ph); 
    void UpdateRecordHint(CRecordHint* phRec);
    void UpdateRequiredReSorting();
    int  iLineHt( CField* pfld ) { return pfld->pmkr()->iLineHeight(); } // line height in pixels of specified field
    
    BOOL bHome( CRecPos& rps ); // go to beginning of line, doesn't set caret loc
    int iUpLine( CRecPos& rps ); // move rps up a line returning height of line moved to
    int iDnLine( CRecPos& rps ); // move rps down a line returning height of line moved from
    int iUpField( CRecPos& rps, BOOL bHide = FALSE ); // move rps up a field returning height of line moved to
    int iDnField( CRecPos& rps, BOOL bHide = FALSE ); // move rps down a field returning height of line moved from
    int iUp( CRecPos& rps ); // calls iUpLine() and/or iUpField()
    int iDown( CRecPos& rps ); // calls iDnLine() and/or iDnField()
    long lVScroll( long lPixels, BOOL bForce=FALSE ); // perform actual window scroll of at least iPixels
    BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll); // handle scroll bar manipulation
    CPoint GetScrollPosition() const; // override base class GetScrollPosition()
    void ResetUpperLeft(); // attempt to set upper left position to previous pixel depth in record
    BOOL bPositionValid( const CRecPos& rps ) const; // verify position exists in current record
    void SetCaretSizeCreate( BOOL bForce=FALSE ); // Create caret based on the current field font

    void BrowseJustificationDefault();
    //09-06-1997
    //Set the default conditions in the BrowesFieldList. These conditions are:
    //1) Left to right script: Left justified exept the PriKeyField if it is
    //   sortet from the end
    //2) Right to left script: Right justified exept the PriKeyField if it is
    //   sortet from the end.

    void RememberUnusualJustificationExceptPriKeyField(CBrowseFieldList* brflstBefore = NULL);
    //09-06-1997
    //The structur of the program requires a recalculation of the Browsefieldlist
    //after the 'Sorting by Fields" Dialog. The function 
    //BrowseJustificationDefault is used. But we don't wan't to lose
    //unusual justifications (after Concordance or Wordlist)
    //Solution: Store the BrowseFieldlist before entering the dialog
    //an give the temporary list as Paramter to this function. 
    //For an Left to Right Script the function does following. If the
    //Field is not the PriKey Field and if it is rightjustified 
    //(unusual) than it stores this feature back. The function does the
    //same for Right to Left Script.
    //I think it should be the only place to call this function
    //NEVER call it after the Browsefielddialog, becouse the
    //the Browsefields itself changes!

    void DrawBrowseHeader(CDC* pDC);
    //09-06-1997
    //Draws the Header of the Browsefield and stores the current value of
    //m_iBrowseHeaderHeigth

    void CalculateBrowseHeaderHeight();
    //09-15-1997. This function should only be called the first time, the
    //browse view appears.
    //This function calculates the Height of the header and sets the value
    //on m_iBrowseHeaderHeigth
    
    // Status bar record and primary key handling
    void SetStatusRecordAndPrimaryKey(const CNumberedRecElPtr& pnrl);

	// 07-31-1997 
	// If a Line contents more than 4000 charakters without wrapping, a bug could 
	// occure. LineLengthCheck() wrapps a Line if it contains more
	// then MaxLineLenth charakters
	void LineLengthCheck();

// Implementation
public:
    virtual ~CShwView();
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    
    void DebugRod();    // Needed for calling protected members of CShwView
                        // such as OnFilePrintPreview, OnFilePrint

#ifndef _MAC
    // Play a .wav file
    BOOL bPlaySound( const char* lpszFileName, Str8 sFrom, Str8 sTo ); // 1.4sg
#endif // not _MAC

    enum EDirection
        {
        dirPrev = -1,
        dirSameRecord = 0,
        dirNext = 1
        };
    //BOOL bValidateCur();
    // 2000/05/02 TLB & MRP -- Caller never sets current. Caller can only tell this function to go to the
    // previous or next element.
    BOOL bValidateCur(EDirection dir = dirSameRecord, BOOL bEditing = TRUE );
        // Call this function *before* changing the current record.
        // If there is a current record and if it has been modified,
        // validity checks are done (e.g. range sets, CARLA field syntax).
        // If it is not valid, this function gives feedback and returns FALSE;
        // the caller should allow the user to continue to edit this record.
        // If it is, the key is recomputed and, if it has changed (in this
        // or any other index) the record element is positioned correctly.
        // NOTE! As of 3/27/96 bValidateCur() causes all views of the current
        // document to be validated.  bValidate() (below) is used to validate
        // a single view only.
    //BOOL bValidate(); // lower level validation, only validates single view
    BOOL bValidate(BOOL bEditing = TRUE,  // 1999-06-28 MRP
            EDirection dir = dirSameRecord /*, CNumberedRecElPtr* pprel,
            BOOL bCallerWillSetCur = FALSE */);

    // Validate record contents with range sets
    // If a piece of text is not in the range set,
    //  and the user doesn't want to add it to the
    //  range set, select the offending word.
    // TLB - 07/29/1999 : Moved (and renamed) function to CRecord
    //BOOL bValidateRangeSet( CRecord* prec );

    void UpdateIndexes(); // If current record modified, update indexes
//	void UpdateInterlinear(); // Update interlinear files for this change, if necessary

    // Public member functions that are used for checking consistency.
    void SetCurAndCaret(const CNumberedRecElPtr& pnrl,
            CField* pfld, int iChar, int lenSelection = 0);
    void ReplaceSelectedInconsistency(const Str8& sReplacement);
    CNumberedRecElPtr pnrlNextToCheck(CNumberedRecElPtr pnrl);
    void ConsistencyCheckingCancelled(CNumberedRecElPtr pnrl);
    
    BOOL bEditRight(); // Right arrow
    BOOL bEditLeft(); // Left arrow
    BOOL bEditUp(); // Up arrow
    BOOL bEditDown(); // Down arrow
    BOOL bEditHome(); // Home, beginning of line
    void EditEnd(); // End, end of line
    BOOL bEditPgUp(); // PgUp, page up
    BOOL bEditPgDn(); // PgDn, page down
    void EditInsert(); // Insert, toggle overlay mode
    BOOL bEditDel(BOOL bAddToUndoBuffer=TRUE); // Del, delete char
    BOOL bEditBackspace(); // Backspace, delete prev char
    void EditCharInsert( LPCSTR szChar ); // Insert or overlay *string*
    BOOL bEditCtrlRight(); // Ctrl Right arrow, word right
    BOOL bEditCtrlLeft(); // Ctrl Left arrow, word left
    BOOL bEditCtrlUp(); // Ctrl Up arrow  go up a paragraph
    BOOL bEditCtrlDown(); // Ctrl Down arrow  go down a paragraph
    void EditCtrlHome(); // Ctrl Home, beginning of record
    void EditCtrlEnd(); // Ctrl End, end of record
    void EditCtrlPgUp(); // Ctrl PgUp, top of window
    void EditCtrlPgDn(); // Ctrl PgDn, bottom of window
    void EditMouseLeftClick( CPoint pnt); // Mouse left button click
    void EditMouseDblClk( CPoint pnt ); // Mouse left button double click
    void SelectWd( CRecPos rps ); // Select word containing rps
    void SelectWd() { SelectWd( m_rpsCur ); } // Select current word
    void SelectAmbig( CRecPos rps ); // Select ambiguity containing rps, scroll it up from bottom
    void EditMarker( CField* pfld ); // Edit the marker of a field
    void NewMarker( CField** ppfld ); // Insert a new field
    void EditSetCaret( CField* pfld, int iChar, int iSelLen = 0 ); // Set the caret to a particular position
    void EditInsertWord( const char* psz, BOOL bPad = FALSE ); // Insert word at current position, adding space before and after if necessary
	BOOL bMoveDownRecord(); // 1.4hm Add new function to move through records
	void UnHideInterlinearFields(); // 1.0bw Unhide interlinear fields before interlinearizing
    BOOL bInterlinearize( BOOL bAdapt, BOOL bCont = FALSE ); // Interlinearize text
	BOOL bSpellCheckRecord( CRecPos& rpsCur, CRecPos rpsEnd ); // Spell check current record, starting at rpsCur
	int iVerifyPassNumber() { return m_iVerifyPassNumber; } // Verify pass number
	void SetVerifySkippedFail( BOOL b ) { m_bVerifySkippedFail = b; } // Verify skipped at least one failure 5.96zd

    void InvalidateLineContaining( CRecPos rpsTarget, BOOL bErase ); // Cause line containing rps to be redrawn
    int iCharToPixel( CDC* pDC, CField* pfld, const char* pszLine, int iLineLen, int iCharPos ); // get pixel length of a chunk of text
    int iPixelToChar( CDC* pDC, CField* pfld, const char* pszLine, int iLineLen, int iPixel ); // get character offset closest to iPixel
    void CalcCaretLocX(); // calc x pos of caret loc using m_rpsCur
    int iCharsFit( CRecPos& rps, int iPixels, BOOL bBrowse = FALSE ); // return rps and pixel offset closest to iPixels. rps assumed to be at beginning of line
    CLPoint pntPosToPnt( CRecPos rps, int iRestore = 0 ); // calculate a screen point from a field and character index
    CLPoint pntPntToPos( CLPoint pnt, int iRestore = 0, CRecPos* prps=NULL ); // calculate a field and character position from a screen point
	int iInterlinPixelPos( CRecPos rps, int iChar ); // 6.0a return interlinear display position in pixels of a char number position
	void RememberInterlinearWidths( BOOL b ); // 6.0j Turn on and off the remembering of interlinear widths for speed

    enum ECurTextType { cttFieldContents = 1, cttItem = 2, cttWord = 4 };
    Str8 sGetCurText(ECurTextType ctt = cttItem); // return selected text (if any); otherwise current text
    // 03-13-2000 TLB : Split EditJumpTo into two separate functions to enable iplementation of right-click jumping from browse view
    void EditJumpTo( BOOL bIsJumpInsert, BOOL bDontShowDlg = FALSE, BOOL bJumpEvenIfEmptyString = TRUE ); // jump and jump insert implementation
    void EditJumpTo( BOOL bIsJumpInsert, BOOL bShowDlg, Str8 sWord, CMarker* pmkr ); // jump to record matching passed in text
    void JumpToKey( BOOL bInsert, const Str8& sWord, CIndex* pind, CShwDoc* pdoc, CNumberedRecElPtr prel); // Actually does the jump/jump insert
    BOOL bEditFind( BOOL bForward ); // find implementation
    BOOL bFindInField( CFindSettings* pfndset, CPatMChar* ppat, CRecPos& rps, int* piLen, BOOL bForward ); // low level find function
    BOOL bFindInRec( CFindSettings* pfndset, CPatMChar* ppat, CRecPos& rps, int* piLen, BOOL bForward ); // low level find function
    BOOL bFind( CPatMChar* ppat, BOOL bForward ); // find match
    BOOL bReplace( BOOL bDoReplace, CPoint* ppntCaret ); // called from replace dialog to do actual replace and next find
    CRecPtrList* prcpBuildList( BOOL bIsReplaceAll ); // build list of records to do replace operation on
    CNumberedRecElPtr prelNext( CNumberedRecElPtr prel ); // get prel of next record to do find or replace operation on

private: // Primitives for editing operations
    void MoveCtrlHome(); // Ctrl Home, beginning of record
    BOOL bMoveRight(); // Move right one char
    BOOL bMoveCtrlRight(); // Ctrl Right arrow move, word right but stay in line
    BOOL bMoveDown( BOOL bHide = FALSE ); // Move down one line, including moving into next field if necessary
    BOOL bMoveDownLine(); // Move down one line in field
    BOOL bMoveDownField( BOOL bHide = FALSE ); // Move down one field in record
    BOOL bMoveUp( BOOL bHide = FALSE ); // Move up one line, including moving into previous field if necessary
    BOOL bMoveUpLine(); // Move up one line in field
    BOOL bMoveUpField( BOOL bHide = FALSE ); // Move up one field in record
    BOOL bMoveLeft(); // Move left one char
    void StringInsert( const char* szChar );  // Simple insert a string at current pos primitive
    void CharInsert( UINT nChar ); // Simple insert char primitive
    void CharDelete(); // Simple delete current char primitive

    void InitializeRec(); // Implementation for OnInitialUpdate()
    void Duplicate(); // Window Duplicate; called by above

    void DrawRec( CDC* pDC ); // Draw the record
    void FldContOut( CDC* pDC, CField* pfld, const char* pszStart=NULL ); // Output field contents, wrapping lines as necessary
    void DrawSubstring( CDC* pDC, const CRect& rect,
            const CRecPos& rpsBegin, const CRecPos& rpsEnd,
            BOOL bDrawSpace, BOOL bHighlight );
    
      //09-02-1997
      //This function writes the string between rpsBegin and rpsEnd
      //starting on the right Border on the rect. If the string is bigger
      //then the rect, the function cuts the string on the left Border
      //of the rect. In a way you can say it is a right justification of
      //a string
      //The function returns TRUE if the current string is completly drawn
      //on the screen. If the string is cut, the function returns FALSE.
      //if bDrawSpace is TRUE and there is enough space in the rect a space
      //will be added on the right end of the string
      //NOTE:After leaving this function the pDC->GetCurrentPosition().x
      //returns the left border of the already drawn String. The y value
      //represents the baseline.
      //NOTE: The function works properly only if function call 
      //pDC->GetCurrentPosition().x returns the right border for the string. 
    BOOL bDrawStringEnd(CDC* pDC, const CRect& rect, CRecPos& rpsBegin, 
                        CRecPos& rpsEnd, BOOL bDrawSpace, BOOL bHighlight);
public: // 1.5.0jg Allow call from MergeRec
	BOOL bHiddenField( CField* pfld ); // Return true if field is hidden, if bShowCursorField is true, don't hide field cursor is in
public: // 1.0bh Allow call from debug routine
    void SetCaretPosAdj(); // SetCaretPos adjusted for scrolling
private:
    void SetCaretLoc(); // set caret position to m_pntCaretLoc accounting for possible out of range location
    void ShowCaret(); // override to add more intelligent caret handling
    void HideCaret();

    void SetCur(CNumberedRecElPtr prelCur, BOOL bCenterBrowseCur=TRUE, BOOL bDuplicating=FALSE);
        // Move to a new current record [element] and display it in the view
        
    void UpdateBasedOnChange();
    BOOL bModifiedSinceValidated() const { return m_bModifiedSinceValidated; }
    void SetModifiedFlag();
        // This view's current record has just been modified.
        // Calling this function also sets the document's modified flag.
    BOOL bPtInMarker( CPoint pnt ); // Is point in marker area?
    void StartTracking( int type ); // capture mouse input
    void StopTracking(); // release mouse input
    BOOL bTracking( int type ) // find out what action the mouse drag is performing
        { return( (type & m_iTracking ) != 0 ); }

    const CRecPos& rpsSelBeg(); // position of beginning of selection
    const CRecPos& rpsSelEnd(); // position of end of selection
    int iGetSelBeg( CField* pfld ); // Get starting position of selection for pfld.
                                    // Please note that this will return 0 if selection starts at the
                                    // beginning of the field or if there is no selection at all.
                                    // iGetSelEnd() should be called to determine if there is any
                                    // selected text in a field.
    int iGetSelEnd( CField* pfld ); // Get end position of selection in pfld (0 if no selection in pfld)
    void ShiftSelectText();         // Start or extend selection as eText selection.
    void StartSelection( int type ); // Mark current caret position as beginning of text or field selection
public: // Needed for interlinear
    void ClearSelection( int iDirection = 0 ); // Deselect any current selection.
        // if iDirection is negative, put caret at beginning of selection
        // if iDirection is zero, leave caret alone
        // if iDirection is positive, put caret at end of selection
private:    
    BOOL bSelecting( int type ) // Is current selection specified type?
        { return ( ( type & m_iSelection ) != 0 ); }
    void DeleteSelection( BOOL bAddToUndoBuffer=TRUE ); // just what the name implies!
    void CopyTemplate(); // make template of current record contents
    BOOL bCopy( CRecPos rpsBeg, CRecPos rpsEnd, Str8& sSel, // return record contents between rpsBeg and rpsEnd
                BOOL bCopyFirstMkr, BOOL bMkrsOnly=FALSE ); 
    BOOL bEditCopy(); // copy selection to clipboard
    BOOL bEditPaste(); // insert contents of clipboard into record
    BOOL bEditUndoAll(); // undo all changes since last SetCur
    void CopyToUndoAllBuffer(); // copy contents of current record to undo buffer

    BOOL bEditUndo(); // undo last action

    // 03/01/2000 TLB : Helper function for OnSetFocus (trying to fix renumbering bug #18)
    void ResynchCaretAndCursor( BOOL bSetKbd = TRUE ); // 1.5.1kh 

public:
    void ResetUndo() // clear undo list
        { m_undlst.DeleteAll(); }
    void ResetUndoAll() // reset can undo all flag and reload undo all buffer
        { m_precUndoAll = NULL; CopyToUndoAllBuffer(); }
    void ResetUndo( CRecord* prec ); // clear Undo list and Undo All buffer if this record is currently being edited
private:
    // word wrap implementation (shwvwrap.cpp)
    BOOL bAtParagraphEnd( const char* psz ); // returns TRUE if at paragraph break
    BOOL bAtParagraphStart( CRecPos* prps ); // returns TRUE if at start of paragraph (or field)
    BOOL bInParagraph( CRecPos* prps ); // returns TRUE if caret position is in or touching paragraph
    BOOL bParagraphStart( CRecPos* prps ); // sets prps to start of paragraph
    BOOL bUpParagraph(); // move to previous paragraph (or beginning of current paragraph)
    BOOL bDnParagraph(); // move to beginning of next paragraph
    BOOL bBreakLine( CLangEnc* plng, CDC* pDC, const char* pszLine, char** ppszLineEnd, CSize sizLine ); // wrap an individual line
    BOOL bGrowLine( CLangEnc* plng, CDC* pDC, const char* pszLine, char** ppszLineEnd, CSize sizLine ); // add words from the next line to current line
    BOOL bWrapParagraph( CRecPos rps ); // reshape current paragraph
    void LineWrap(); // 2000-09-07 TLB: Do auto-wrap or emergency line breaking to 1000-chars max
    BOOL bDoAutoWrap(); // do auto wrap
    void EditReshape(); // reshape paragraph containing m_rpsCur or selected text if any
    void EditReshapeDocument(); // reshape all fields in entire document
    BOOL bCombineBundles( CRecPos& rpsCur, BOOL bMult = TRUE ); // pull interlinear bundles together in preparation for wrapping, return TRUE if changes occurred, default to multiple
    CRecPos rpsWrapInterlinear( CRecPos rps, BOOL bJoin = FALSE ); // Break interlinear as necessary
private:
    // Implementation of Tools menu items (number.cpp)
    void ReNumber(); // prompt user for details of how to (re-)number and then do it
    void CheckConsistency();  // 1999-06-11 MRP
    void RecheckConsistency();  // 1999-06-12 MRP
private:
    // Implementation of Edit menu items (shwvdis.cpp)
    void EditInsertMarker();
    void EditInsertFromRangeSet();
    
    // Implementation of Database menu items (shwvdat.cpp)
	void FixCursorPosForHidden( BOOL bScrollUp = TRUE ); // 1.0br Fix cursor position for hidden fields after Database Next, Prev, etc. // 1.5.0fn Scroll up after jump or turn off browse
    void DatabaseNextRecord();
    void DatabasePreviousRecord();
    void DatabaseFirstRecord();
    void DatabaseLastRecord();
    BOOL bDatabaseSearch();
    void DatabaseInsertRecord();
    void DatabaseDeleteRecord();
	void DatabaseCopyRecord(BOOL bDeleteOld);
	void DatabaseMergeDb();
    void DatabaseFiltering();
    void DatabaseSorting();
    void DatabaseMarkers();

	// Non-menu-related functions for manipulating the database
	void DeleteCurRecord();

    //08-25-1997
    void BuildRangeSet();
    
    // Computing the left column space for markers and/or field names.
    void AdjustLeftColumn();

public:
	void TextParallelJump(); // 1.1me ParallelJump from text file, used on mouse click and cursor movements
	void ParallelJump(); // 6.0y Book reference jump

//	void PlaceCursorAtTopOfWindow(); // 6.0zh Place cursor at top of window after jump if in the middle of a long record // 1.5.0fp 

    void UseFilter(CFilter* pfil); // set filter - filter already selected from toolbar filter combo

    void ViewDatabaseProperties(int iPage=0); // brings up Database Type properties tabbed dialog

	void SetHideFields( BOOL bHide ); // 1.0bg Set hide fields on or off
	BOOL bPlayableFile( CField* pfld, Str8& sPath, Str8& sType, Str8& sFrom, Str8& sTo  ); // 1.2rc
	BOOL bFindPlayableFile( Str8& sPath, Str8& sType, Str8& sFrom, Str8& sTo ); // 1.2mn For Play Sound or Play File, find file name in record

	CShwView* pviewSideBrowse(); // 1.5.1ea Find side browse of this window, if it has one

private:
    // called to view record inserted   
    void ViewInsertedRecord(CNumberedRecElPtr prel);

#ifdef _DEBUG
    virtual void AssertValid() const;
#endif

protected:
    virtual void OnInitialUpdate(); // called first time after construct
    virtual void OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint );
        // lHint gives the type of operation: eg. delete, insert, change, based on constants in CHint class
        // pHint contains a pointer to the record modified
    
    // Put a message in the status line.
    void MessageStatusLine(const char* pszMessage);
	void ReadOnlyMessage(); // 1.1ch Give message in status bar when user tries to modify read-only file
    
    // Show or hide cursor on activate or deactivate
//  afx_msg void OnActivate( UINT nState, CWnd* pwnd, BOOL bMinimized );
    
    // Printing support
    virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL );
    virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo );
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

    void PageSetup();
    void DoPrint(CDC* pDC, CPrintInfo* pInfo, BOOL bDontPrint = FALSE);

    void PrintRecord(CDC* pDC, CRect &rectMarkerPrintArea,
                     CRect &rectFieldsPrintArea, CPrintInfo* pInfo,
                     BOOL bDontPrint=FALSE);
    // Prints a single record.

    void PrintRec(  CDC* pDC, CRect &rectMarkerPrintArea,
                    CRect &rectFieldsPrintArea,
                    CNumberedRecElPtr& prel, CRecPos& prps,
                    BOOL *pbEndOfRecord, BOOL *pbPageBreak,
                    BOOL bDontPrint=FALSE);
    // Print one page of the current record.
    // The vertical position on the page must be set before calling.

    void PrintFldInterlinear(/*CPrintProperties* pprtp, */ CDC* pDC, CRecPos& prps, CRect &rectFieldsPrintArea, BOOL bDontPrint);
    // Print an interlinear field.

    void PrintBrowse( CDC* pDC, CPrintInfo* pInfo, CRect& rectBrowseArea,
                        BOOL bDontPrint=FALSE );

    void PrintHeader(CDC* pDC, CRect& rect, CPrintInfo* pInfo);
    void PrintFooter(CDC* pDC, CRect& rect, CPrintInfo* pInfo);

    void Print_MoveToPage(CPrintInfo* pInfo, CDC* pDC);

    void Print_SkipTo(CPrintInfo* pInfo, CDC* pDC, UINT* pnWhereStopped);


    void CalculatePrintAreas(   CDC* pDC, CPrintInfo* pInfo, CRect &rectPage, CRect &rectMarkerPrintArea,
                                CRect &rectFieldsPrintArea, CRect& rectBrowseArea, 
                                CRect &rectHeader, CRect& rectFooter);
    // rectPage is the whole printable area on the page (takes margins into account)
    // rectHeader, rectFooter are for header, footer info.

      //09-10-1997
    void ExtTextOut_AsOneLinePrnt(CDC* pDC, int xLeft, int y, 
                                  UINT nOptions, LPCRECT prect, LPCSTR psz,
                                  UINT len, int iSelected, CMarker* pmkr);
    void ExtTextOut_AsOneLinePrnt(CDC* pDC, int* pxLeft, int y, UINT nOptions, 
                                  LPCRECT prect, LPCSTR psz, UINT len, int iSelected, CMarker* pmkr);
    // Look at Tool.h for desciption. The diference is, that this function
    // uses for the final output the TextClipOut funktion.

	// Generated message map functions
public: 
    afx_msg void OnFileExport(); // 1.4qmh Make public so doc can call it
protected:
    //{{AFX_MSG(CShwView)
    afx_msg void OnDatabaseNextRecord();
    afx_msg void OnDatabasePreviousRecord();
    afx_msg void OnDatabaseFirstRecord();
    afx_msg void OnDatabaseLastRecord();
    afx_msg void OnDatabaseSearch();
    afx_msg void OnEditInsertField();
	afx_msg void OnEditReturnFromJump();
    afx_msg void OnDatabaseInsertRecord();
    afx_msg void OnDatabaseDeleteRecord();
	afx_msg void OnDatabaseCopyRecord();
	afx_msg void OnDatabaseMoveRecord();
	afx_msg void OnDatabaseMergeDb();
    afx_msg void OnDatabaseFiltering();
    afx_msg void OnUpdateDatabaseFiltering(CCmdUI* pCmdUI);
    afx_msg void OnDatabaseSorting();
    afx_msg void OnUpdateDatabaseProperties(CCmdUI* pCmdUI);
    afx_msg void OnDatabaseMarkers();
    afx_msg void OnUpdateDatabaseNextRecord(CCmdUI* pCmdUI);
    afx_msg void OnUpdateDatabasePreviousRecord(CCmdUI* pCmdUI);
    afx_msg void OnUpdateDatabaseFirstRecord(CCmdUI* pCmdUI);
    afx_msg void OnUpdateDatabaseLastRecord(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditInsertField(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditReturnFromJump(CCmdUI* pCmdUI);
    afx_msg void OnUpdateDatabaseInsertRecord(CCmdUI* pCmdUI);
    afx_msg void OnUpdateDatabaseDeleteRecord(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDatabaseCopyRecord(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDatabaseMoveRecord(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDatabaseMergeDb(CCmdUI* pCmdUI);
    afx_msg void OnWindowDuplicate();
    afx_msg void OnUpdateFileExport(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI); // 1.4wb 
    afx_msg void OnUpdateFileSaveAll(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFileClose(CCmdUI* pCmdUI);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnViewMarkers();
    afx_msg void OnViewFieldNames();
    afx_msg void OnUpdateViewFieldNames(CCmdUI* pCmdUI);
    afx_msg void OnUpdateViewMarkers(CCmdUI* pCmdUI);
    afx_msg void OnEditMarker();
    afx_msg void OnUpdateEditMarker(CCmdUI* pCmdUI);
    afx_msg void OnEditInsertFromRangeSet();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnEditCopy();
    afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
    afx_msg void OnEditCut();
    afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
    afx_msg void OnEditPaste();
    afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
    afx_msg void OnEditUndoAll();
    afx_msg void OnUpdateEditUndoAll(CCmdUI* pCmdUI);
    afx_msg void OnEditUndo();
    afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnToolsCheckUnicodeValidity();
    afx_msg void OnToolsCompareFiles();
    afx_msg void OnEditInterlinearize();
    afx_msg void OnEditInterlinShowFail();
    afx_msg void OnEditAdaptCont();
    afx_msg void OnEditAdapt();
    afx_msg void OnEditSpellCheck();
    afx_msg void OnToolsNumbering();
    afx_msg void OnToolsReNumber();
    afx_msg void OnUpdateToolsNumbering(CCmdUI* pCmdUI);
    afx_msg void OnUpdateToolsReNumber(CCmdUI* pCmdUI);
    afx_msg void OnToolsCheckConsistency();
    afx_msg void OnToolsNewConsistencyCheck();
    afx_msg void OnUpdateToolsNewConsistencyCheck(CCmdUI* pCmdUI);
    afx_msg void OnToolsContinueConsistencyCheck();
    afx_msg void OnUpdateToolsContinueConsistencyCheck(CCmdUI* pCmdUI);
    afx_msg void OnSize(UINT nType, int cx, int cy);
//    afx_msg void OnMove( int x, int y);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnViewBrowse();
    afx_msg void OnViewSideBrowse();
    afx_msg void OnUpdateViewBrowse(CCmdUI* pCmdUI);
    afx_msg void OnViewBrowsefields();
    afx_msg void OnViewHideFields();
    afx_msg void OnViewFieldsToHide();
    afx_msg void OnViewReapplyFilter();
    afx_msg void OnUpdateViewReapplyFilter(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditInsertFromRangeSet(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditInterlinearize(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditInterlinShowFail(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditAdapt(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditAdaptCont(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditInterlinearizeWord(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditSpellCheck(CCmdUI* pCmdUI);
    afx_msg void OnViewMarkerHierarchy();
    afx_msg void OnViewJumpTarget();
    afx_msg void OnUpdateViewJumpTarget(CCmdUI* pCmdUI);
    afx_msg void OnViewFocusTarget();
    afx_msg void OnUpdateViewFocusTarget(CCmdUI* pCmdUI);
    afx_msg void OnUpdateViewHideFields(CCmdUI* pCmdUI);
    afx_msg void OnUpdateViewMarkerHierarchy(CCmdUI* pCmdUI);
    afx_msg void OnEditUseAsTemplate();
    afx_msg void OnUpdateEditUseAsTemplate(CCmdUI* pCmdUI);
    afx_msg void OnDatabaseProperties();
    afx_msg void OnEditJumpto();
    afx_msg void OnUpdateEditJumpto(CCmdUI* pCmdUI);
    afx_msg void OnEditJumpinsert();
    afx_msg void OnUpdateEditJumpinsert(CCmdUI* pCmdUI);
    afx_msg LRESULT OnCommandHelp(WPARAM wParam, LPARAM lParam);
    afx_msg void OnFileLanguageEncodings();
    afx_msg void OnUpdateFileLanguageEncodings(CCmdUI* pCmdUI);
    afx_msg void OnFileDatabaseTypes();
    afx_msg void OnUpdateFileDatabaseTypes(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFileTextCorpus(CCmdUI* pCmdUI);
    afx_msg void OnViewAutowrap();
    afx_msg void OnViewSetMargin();
    afx_msg void OnUpdateViewSetMargin(CCmdUI* pCmdUI);
    afx_msg void OnUpdateViewAutowrap(CCmdUI* pCmdUI);
    afx_msg void OnEditReshape();
    afx_msg void OnEditReshapeDb();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnUpdateEditReshape(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditReshapeDb(CCmdUI* pCmdUI);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnEditFind();
    afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
    afx_msg void OnEditReplace();
    afx_msg void OnUpdateEditReplace(CCmdUI* pCmdUI);
//    afx_msg void OnDebugMark();
    afx_msg void OnExerciseToggle();
    afx_msg void OnExerciseUpdate();
    afx_msg void OnDatabaseTypeUpdate();
    afx_msg void OnDebugAlan();
	afx_msg void AlanDebug2();
	afx_msg void AlanDebug3();
	afx_msg void ToggleInternalKeyboard();
// afx_msg void OnToolsMacros(); // 1.5.0ah 
	afx_msg void ActivateFilterCombo();
    afx_msg void OnDebugKaren();
    afx_msg void OnViewBothmarkersandnames();
    afx_msg void OnUpdateViewBothmarkersandnames(CCmdUI* pCmdUI);
    afx_msg void OnFilePageSetup();
    afx_msg void OnEditCtrlSpace();
    afx_msg void OnEditSelectAll();
    afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
    afx_msg void OnEditDelwordLeft();
    afx_msg void OnEditDelwordRight();
    afx_msg void OnEditSelectLine();
    afx_msg void OnEditSelectField();
    afx_msg void OnEditPlaysound();
    afx_msg void OnUpdateEditPlaysound(CCmdUI* pCmdUI);
    afx_msg void OnEditPlayFile();
    afx_msg void OnUpdateEditPlayFile(CCmdUI* pCmdUI);
    //}}AFX_MSG
    afx_msg void OnWindowClose();
    afx_msg LRESULT OnMCNotify(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnUnicodeChar(WPARAM wParam, LPARAM lParam); // AB Unicode support
#ifdef TLB_MRP_2000_09_19_EDIT_SOUND
    afx_msg void OnEdit_Editsound();
    afx_msg void OnUpdateEdit_Editsound(CCmdUI* pCmdUI);
#endif
    DECLARE_MESSAGE_MAP()
    // New mechanism for cut/copy/paste handling: we delay rendering the copied data until 
    //  the clipboard sends us...
    void OnRenderFormat(UINT nFormat); // 1.1bp Fix bug of copy failing in various situations
    // Make the data members static, so they apply to all views and not just this one.
    static Str8     m_strCopied; // 1.2km Remove paste stack
#ifndef _MAC
//    MyML2Ptr                m_pML2; // 5.97n turn off // can't be static, because it can't be instantiated until
	// after CoInitialize is called in InitInstance()
#endif
    void OnCharEx(Str8& strChar, UINT nRepCnt, UINT nFlags);
//    afx_msg LRESULT OnInputLangChange(WPARAM wParam, LPARAM lParam); // 1.4qzhm
#ifndef _UNICODE // 1.4zam 
    BOOL m_bConvertNext;
    Str8 m_chLeadByte;
    UINT m_cpIM;
#endif  // 1.4zam 
};
///////////////////////////////////////////////////////////////////////////// 

