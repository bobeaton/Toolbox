// cbo.h  Interface for a combo box representing a set (1995-06-19)

// A class derived from CSetComboBox provides the functionality of a
// Windows combo box control on a dialog box. A combo box shows a view
// of one particular element from a set and provides a means by which
// the user can change the selection. The selected element represents
// an attribute of the data entity represented by the entire dialog box.

// How to derive your combo box class appears at the end of this file!

// Change history:
// 1995-06-19 0.15  MRP: Initial design sketch


#ifndef CBO_H
#define CBO_H

#include "set.h"  // classes CSet and CSetEl
#include "resource.h"

// #define IDSW_NONEITEM 100 // 1.4ef Use switch instead of string ID

// **************************************************************************

class CSetComboBox : public CComboBox  // Hungarian: cbo
{
private:
    CSet* m_pset;  // the set from which an element is selected
    BOOL m_bUseNoneItem; // if TRUE, a [none] will be first item in combo
    CSetEl* m_pselDontUse; // element to not include in combo list
    unsigned int m_iNoneID; // id of string resource for [none] item
    Str8 m_sNoneItem;
    Str8 m_sMatched;  // User's keystrokes for selecting an item
    BOOL m_bMyOwnNotification;  // Distinguish initiative for selchanges
    
protected:
    CSetComboBox(CSet* pset, BOOL bUseNoneItem=FALSE, CSetEl* pselDontUse=NULL, const char* pszNoneItem=NULL); // 1.4gg Eliminate unused NoneID // 1.4gh Add string NoneItem
        // If bUseNoneItem is TRUE a [none] will appear as the first item
        // in the combo box. A set element can be prevented from being in
        // the list by passing it as pselDontUse.
    
    void UpdateElements(CSetEl* pselToSelect);
        // Add the names of the set's elements as items in the combo box,
        // first removing any items that had already been in it.
        // Select the item corresponding to the specified element;
        // specify NULL if the set is empty, or if no item is to be selected.
        // NOTE: Unlike a list box, a non-empty combo box is permitted
        // not to have any item selected. It seems that for a non-empty set,
        // once the program has selected an item, there will be a selection--
        // the user can change it, but not clear it.
        // NOTE: Call this in the derived dialog box's OnInitDialog(),
        // and anytime the set has had elements added, modified, or deleted
        // via manipulation of some other control on the dialog box.
    void ChangeSet(CSet* pset, CSetEl* pselToSelect, CSetEl* pselDontUse=NULL);
        // Change the set being viewed through the box, and update it.
        // NOTE: Derived classes should override this only if it is needed,
        // i.e. if the contents of this control can be caused to change
        // when the user manipulates another control on the dialog box.
    
    void SelectElement(CSetEl* pselToSelect);
        // Select the item corresponding to the specified element.
        // NOTE: Derived classes should override this only if it is needed;
        // ordinarily the dialog box code needs only call UpdateElements().
    CSetEl* pselSelected();
        // Return the element corresponding to the item currently selected.
        // Return NULL, if the set is empty, or if no item has been selected.
        
    virtual BOOL bIncludeElement(const CSetEl* psel) const;
        // Derived class can override to choose individual elements to show.

    ItemIndex SetCurSelAndNotifyParent(ItemIndex itmToSelect);
        // Changes to the current selection caused by user keystrokes
        // must be sent to the dialog box parent in case it has an
        // OnSelchange event function.

    afx_msg void OnChar(UINT, UINT, UINT);
    afx_msg void OnKillFocus(CWnd*);
    DECLARE_MESSAGE_MAP()
        
    virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam,
        LRESULT* pLResult);

    const Str8& sNoneItem() const { return m_sNoneItem; }

#ifdef OWNER_DRAW
    virtual BOOL bIsBold(CSetEl* psel);
    virtual void DrawElement(CDC& cDC, const RECT& rcItem,
            const CSetEl *psel);
        // Implement this function in your derived class to draw the item
        // in the form you desire to represent the list element pel.
        // Call DrawSubItem with the cDC and rcItem arguments provided,
        // as well as the appropriate member data from the element object.
        
    // Helper function to be called from DrawElement
    void DrawSubItem(CDC& cDC, const RECT& rcItem, int xCur, int xNext,
            const char* pszSubItem);
        // Draw a subitem in the list box starting at offset xCur,
        // (clipped just before the offset xNext of the next subitem)
        // consisting of the text pszSubItem.
        // (For the first subitem xCur = 0; and for the last xNext = 0.)

private:
    virtual void DrawItem(LPDRAWITEMSTRUCT);
        // Called by the framework whenever a visual aspect of an owner-draw
        // list box changes. Calls the derived class's DrawElement function.
#endif  // OWNER_DRAW
};  // class CSetComboBox


// --------------------------------------------------------------------------

// COOKBOOK RECIPE FOR DERIVING YOUR COMBO BOX CLASS IN ONLY FIVE MINUTES...

// 1. Use Class Wizard to add a member variable (Property: Control) to your
//    derived dialog box class corresponding to the combo box resource ID.
// 2. Paste the following code fragment before the derived dialog box class
//    which contains the combo box control in its .h file, and Edit Replace
//    the proper element class and Hungarian type.
// 3. Change the type of the m_cboYourSet variable from CComboBox to
//    the class which you have just derived. (Move the DDX member data
//    block from the public to the _private_ section of the class.)
// 4. a. Add the member data constructing initializer m_cboYourSet(pyelset)
//       to the constructor of the derived dialog box class.
//    b. Call UpdateElements() in the dialog box's OnInitDialog() and
//       also anytime that the set's contents have been changed.
//    c. Assign the value of pyelSelected() to a dialog box member variable
//       _before_ calling CDialog::OnOK() in your OnOK() or OnClose() and
//       also anytime that another control needs to know the current value.

// See CMarkerEditDlg (dlgmkred.h/.cpp) and CFilterDlg (dlgfilmn.h/.cpp).

// E.g. a combo box representing a marker set: Your -> Marker, yel -> mkr.
/*
#include "cbo.h"

class CYourSetComboBox : public CSetComboBox  // Hungarian: cbo
{
public:
    CYourSetComboBox(CYourSet* yelset) : CSetComboBox(yelset) {}
    
    void UpdateElements(CYour* yelToSelect)
        { CSetComboBox::UpdateElements(yelToSelect); }
    CYour* yelSelected()
        { return (CYour*)CSetComboBox::pselSelected(); }
};  // class CYourSetComboBox
*/

// **************************************************************************

#endif  // CBO_H
