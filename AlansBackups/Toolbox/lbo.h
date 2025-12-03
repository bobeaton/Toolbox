// lbo.h  Interface for a list box control representing a list (1995-06-19)

// Derive a class from CDblListListBox to encapsulate a view of a list or
// set as a control on a dialog box. Except when the list box is empty,
// it has a currently selected item corresponding to an element.

// How to derive your list box class appears at the end of this file!

// Change history:
// 1995-06-19 0.15  MRP: Initial design sketch


#ifndef LBO_H
#define LBO_H

#include "set.h"  // CDblList, CDblListEl, CSet, CSetEl
class CLangEnc;  // lng.h
class CMarker;  // mkr.h


// **************************************************************************

class CDblListListBox : public CListBox  // Hungarian: lbo
{
protected:
    CDblList* m_plst;  // the list (or set) of elements to be viewed
    int m_iItemHeight;
    
    CDblListListBox(CDblList* plst);

    CDblListEl* pelSelected();
        // Return the element corresponding to the item currently selected;
        // otherwise NULL, if the list is empty.

    void SetItemHeight(int iItemHeight);
        // Set the height of items so that fonts other than the default
        // for list boxes can be used, especially for language text.
    virtual void DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel) = 0;
        // Implement this function in your derived class to draw the item
        // in the form you desire to represent the list element pel.
        // Call DrawSubItem with the cDC and rcItem arguments provided,
        // as well as the appropriate member data from the element object.

public:    
    void UpdateElements(const CDblListEl* pelToSelect);
        // Add the list's elements as items in the list box,
        // first removing any items that had already been in it.
        // Select the item corresponding to the specified element;
        // specify NULL (only) if the list is empty.
        // NOTE: Unlike a combo box, a non-empty list box is required
        // always to have one item selected.
        // NOTE: Call this in the derived dialog box's OnInitDialog,
        // and anytime the list has had elements added, modified, or deleted
        // via manipulation of some other control on the dialog box. 

    // Helper functions to be called from DrawElement and InitListBox:

    void DrawSubItem(CDC& cDC, const RECT& rcItem, int xCur, int xNext,
            const char* pszSubItem);
        // Draw a subitem in the list box starting at offset xCur,
        // (clipped just before the offset xNext of the next subitem)
        // consisting of the text pszSubItem.
        // (For the first subitem xCur = 0; and for the last xNext = 0.)
        // If all subitems consist of ordinary non-language text
        // (e.g. object names), use this function to draw them all.

    void DrawSubItemJustify(CDC& cDC, const RECT& rcItem, int xLeft, int xRight,
            const char* pszSubItem, CLangEnc* plng = NULL);
        // Draw a subitem using language encoding plng's font, if specified;
        // otherwise, use the device context cDC's current font.
        // (The cDC's current font is restored before returning.)
        // This function makes all subitems align on the baseline
        // even though they make have be shown in different fonts.
        // If any subitems consist of language text,
        // use DrawSubItemJustify functions to draw them all.
    void DrawSubItemJustify(CDC& cDC, const RECT& rcItem, int* pxLeft, int xRight,
            const char* pszSubItem, CLangEnc* plng = NULL);
        // Draw a subitem, then increment *pxLeft by its text extent.
        // Use this function to draw filters piece-by-piece.

    // 1999-06-23 MRP: Use when marker font differs from language font.
    void DrawSubItemJustify(CDC& cDC, const RECT& rcItem, int xLeft, int xRight,
            const char* pszSubItem, CMarker* pmkr);
    void DrawSubItemJustify(CDC& cDC, const RECT& rcItem, int* pxLeft, int xRight,
            const char* pszSubItem, CMarker* pmkr);

protected:
    // 1999-10-01 TLB: Use when font for new marker differs from language font.
    void DrawSubItemJustify(CDC& cDC, const RECT& rcItem, int xLeft, int xRight,
            const char* pszSubItem, CFont* pfnt, CLangEnc* plng);

    int xSubItem_Show(ID idSubItemLabel = 0);
        // Return the horizontal offset from the left edge of the list box
        // client drawing rectangle to the left edge of the label control
        // whose Windows resource ID is idSubItemLabel. This is the offset
        // needed to draw any subitem after the first so that it lines up
        // in a column underneath the label. Also make the label visible.
		//AB 3-5-02 Make arg optional for ambig box, and in prep for edit

private:
    virtual void DrawItem(LPDRAWITEMSTRUCT);
        // Called by the framework whenever a visual aspect of an owner-draw
        // list box changes. Calls the derived class's DrawElement function.
    void DrawSubItemJustify(CDC& cDC, const RECT& rcItem, int* pxLeft, int xRight,
            const char* pszSubItem, CFont* pfnt,  // 1999-06-23 MRP
            CLangEnc* plng, BOOL bAlignIfRightToLeft);
};  // class CDblListListBox


// --------------------------------------------------------------------------

// COOKBOOK RECIPE FOR DERIVING YOUR LIST BOX CLASS IN HALF AN HOUR...

// 1. Use Class Wizard to add to your derived dialog box class a member
//    variable (with the control property), e.g. m_lboYourList, which
//    corresponds to the list box control (via its resource ID).
// 2. Paste the first code fragment before the dialog box class interface
//    in its .h file. Edit Replace the proper classes and Hungarian types.
// 3. Change the type of the m_lboYourList variable from CListBox to
//    the class which you have just pasted. (Move the DDX member data block
//    from the public to the _private_ section, if it isn't already there.)
// 4. a. Add the member data constructor m_lboYourList(pyellst) to the 
//       initializations list of the derived dialog box constructor.
//    b. Call UpdateElements in the dialog box's OnInitDialog, and also
//       anytime that the list's contents have been changed.
//    c. Assign the value of pyelSelected to a dialog box member variable
//       _before_ calling CDialog::OnOK in your OnOK or OnClose, and also
//       anytime that another control needs to know the current element.
//    d. If the list box item is to consist of more that one subitem
//       (e.g. a filter's name and condition), include in your list box class
//       as member data the horizontal offsets to subitems after the first.
//       Initialize those members in an OnInitListBox function to be called
//       (before UpdateElements) in the dialog box's OnInitDialog function.
//    e. Implement DrawElement in your list box class to draw the item
//       (or subitems) representing a list element in the form you desire.
//       Paste the second code fragment in the dialog box implementation
//       .cpp file and revise according to the element object's member data. 

// See CFilterSetListBox, which is used by CFilterSetDlg (filset_d.h/.cpp).


// E.g. To derive a list box representing a filter set, you would replace:
//      YourList -> FilterSet, Your -> Filter, yellst -> filset, yel -> fil.


// obj_d.h  Interface for your dialog box, which contains the list box.

/*
#include "lbo.h"

class CYourListListBox : public CDblListListBox  // Hungarian: lbo
{
public:
    CYourListListBox(CYourList* pyellst) : CDblListListBox(pyellst) {}
    
    //void InitLabels();  // needed only if you draw multiple subitems
    
    void UpdateElements(const CYour* pyelToSelect)
        { CDblListListBox::UpdateElements(pyelToSelect); }
    CYour* pyelSelected()
        { return (CYour*)CDblListListBox::pelSelected(); }

private:
    //int m_xSubItem2;  // , m_xSubItem3, etc.  // offsets to subitems

    void DrawElement(CDC& cDC, const RECT& rcItem, const CDblListEl *pel);
};  // class CYourListListBox
*/  


// obj_d.cpp  Implementation of your dialog box, which contains the list box.

/*
void CYourListListBox::InitLabels()
{
    m_xSubItem2 = xSubItem(IDC_OBJ_SUBITEM2);
    m_xSubItem3 = xSubItem(IDC_OBJ_SUBITEM3);
    // etc.
}

void CYourListListBox::DrawElement(CDC& cDC, const RECT& rcItem,
        const CDblListEl *pel)
{
    CYour* pyel = (CYour*)pel;
    
    // If you are drawing a simple item:
    DrawSubItem(cDC, rcItem, 0, 0, pyel->A());
    
    // If you are drawing multiple subitems:
    DrawSubItem(cDC, rcItem, 0, m_xSubItem2, pyel->A());
    DrawSubItem(cDC, rcItem, m_xSubItem2, m_xSubItem3, pyel->B());
    // ...
    DrawSubItem(cDC, rcItem, m_xSubItem7, 0, pyel->G());
}
*/


// ==========================================================================

class CSetListBox : public CDblListListBox  // Hungarian: lbo
{
public:
    CSetListBox(CSet* pset, CSetEl** ppsel);
    CSetListBox(CSet* pset);
    
    void InitListBox();
    void Close();
    
    BOOL bAdd();
    BOOL bCopy();
    BOOL bModify();
    BOOL bDelete();
    
    void UpdateElements(const CSetEl* pselToSelect);
    CSetEl* pselSelected()
        { return (CSetEl*)CDblListListBox::pelSelected(); }
    void ChangeSet(CSet* pset, CSetEl* pselToSelect);
        // Change the set being viewed through the box, and update it.
        // NOTE: Derived classes should override this only if it is needed,
        // i.e. if the contents of this control can be caused to change
        // when the user manipulates another control on the dialog box.

protected:
    int m_xName;
    virtual void InitLabels();
    virtual int iItemHeight() { return 0; }
    virtual void DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel);
    virtual BOOL bDelete(CSetEl* psel);
    virtual BOOL bIsBold(CSetEl* psel);

private:
    CSet* m_pset;
    CSetEl** m_ppsel;
    CSetEl* m_psel;
    Str8 m_sMatched;  // User's keystrokes for selecting an item
    BOOL m_bMyOwnNotification;  // Distinguish initiative for selchanges

    void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct); // overrides CDblListListBox::DrawItem()

protected:
    ItemIndex SetCurSelAndNotifyParent(ItemIndex itmToSelect);
        // Changes to the current selection caused by user keystrokes
        // must be sent to the dialog box parent in case it has an
        // OnSelchange event function.

    afx_msg void OnChar(UINT, UINT, UINT);
    afx_msg void OnKillFocus(CWnd*);
    DECLARE_MESSAGE_MAP()
    
    virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam,
        LRESULT* pLResult);
};  // class CSetListBox


// --------------------------------------------------------------------------

// COOKBOOK RECIPE FOR DERIVING YOUR SET DIALOG BOX IN HALF AN HOUR...

// See CFilterSet::bModalView (fil.cpp).


// E.g. To derive a dialog box representing a filter set, you would replace:
//      YourSet -> FilterSet, Your -> Filter, yelset -> filset, yel -> fil.


// obj.cpp  Implementation of your object set

/*
#include "set_d.h"

// ...

class CYourSetListBox : public CSetListBox
{
public:
    CYourSetListBox(CYourSet* pyelset, CYour** ppyel) :
        CSetListBox(pyelset, (CSetEl**)ppyel) {}

protected:
    //int m_xSubItem2;  // , m_xSubItem3, etc.  // offsets to subitems
    virtual void InitLabels();
    virtual void DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel);
};  // class CYourSetListBox

void CYourSetListBox::InitLabels()
{
    CSetListBox::InitLabels();  // if you use the default "Name" label
    m_xSubItem2 = xSubItem(IDC_YELSET_SUBITEM2);
    m_xSubItem3 = xSubItem(IDC_YELSET_SUBITEM3);
    // etc.
}

void CYourSetListBox::DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel)
{
    CYour* pyel = (CYour*)pel;
    
    // If you are drawing a simple item:
    DrawSubItem(cDC, rcItem, 0, 0, pyel->A());
    
    // If you are drawing multiple subitems:
    DrawSubItem(cDC, rcItem, 0, m_xSubItem2, pyel->A());
    DrawSubItem(cDC, rcItem, m_xSubItem2, m_xSubItem3, pyel->B());
    // ...
    DrawSubItem(cDC, rcItem, m_xSubItem7, 0, pyel->G());
}


BOOL CYourSet::bModalView_Elements(CYour** ppyelSelected)
{
    CYourSetListBox lbo(this, ppyelSelected);
    return CSetDlg::s_bModalView(IDSFILSET_TITLE, lbo);
}
*/

// **************************************************************************

#endif  // LBO_H
