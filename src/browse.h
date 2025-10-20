// browse.h
// Brian Yoder  10-2-95

#ifndef Browse_h
#define Browse_h

#include "cdbllist.h" // For derivation
#include "cdblel.h" // For derivation
#include "mkr.h"

class Object_istream;  // obstream.h
class Object_ostream;  // obstream.h

class CBrowseField : public CDblListEl  // Hungarian: brf
{
private:
    CMarkerPtr m_pmkr; // Display first field containing this marker in record being browsed
    int m_iWidth; // width of field in pixels
    
      //09-05-1997
    BOOL m_bRightJustified;  // justification of the field
  
    friend class CBrowseFieldList;
        
#ifdef typWritefstream // 1.6.4ac
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    static BOOL s_bReadProperties(Object_istream& obs, CMarkerSet* pmkrset, CBrowseField** ppbrf);

public:
    // 09-04-1997 Added parameter 3 to paramterlist
    CBrowseField( const CMarker* pmkr, int iWidth = eMinFieldWidth ,
                  BOOL bRightJustified = FALSE);

    enum { eMinFieldWidth = 10 };

    CMarker* pmkr() { return m_pmkr; }
    int iWidth() const { return m_iWidth; }
    
    void SetWidth( int iWidth ) { m_iWidth = iWidth; }

    //09-04-1997
    BOOL bIsRightJustified()
        { return m_bRightJustified; } 
    void SetRightJustified(BOOL bRightJustified)
        { m_bRightJustified = bRightJustified; }
  
    ~CBrowseField();
};  // class CBrowseField


// --------------------------------------------------------------------------

class CBrowseFieldList : public CDblList  // Hungarian: brflst
{
private:
    int m_iLineHt;
    
      //09-23-1997
    Str8 m_sFieldEmpty;
    Str8 m_sNoField;

public:
    CBrowseFieldList();
    ~CBrowseFieldList() { DeleteAll(); }

      //09-23-1997
    const Str8& GetFieldEmptyEntry() { return m_sFieldEmpty; }
    void SetFieldEmptyEntry(const char* pszEntry) { m_sFieldEmpty = pszEntry; } // 1.2kn Change to char* arg for portability to Str8
    const Str8& GetNoFieldEntry() { return m_sNoField; }
    void SetNoFieldEntry(const char* pszEntry) {m_sNoField = pszEntry; } // 1.2kn Change to char* arg for portability to Str8

#ifdef typWritefstream // 1.6.4ac
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    BOOL bReadProperties(Object_istream& obs, CMarkerSet* pmkrset);

    CBrowseField* pbrfFirst() const { return (CBrowseField*)pelFirst(); }
    CBrowseField* pbrfNext( const CBrowseField* pbrf) const { return (CBrowseField*)pelNext(pbrf); }
    CBrowseField* pbrfPrev( const CBrowseField* pbrf) const { return (CBrowseField*)pelPrev(pbrf); }
    CBrowseField* pbrfLast() const  { return (CBrowseField*)pelLast(); }    
    
    CBrowseFieldList& operator = (const CBrowseFieldList& brlst);
   
    int iLineHt() { return m_iLineHt; }
    void RecalcLineHt(); // make sure m_iLineHt correct

    void Add( const CMarker* pmkr );
    void Add( CBrowseField* pbrf );
    
    void DeleteAll() { CDblList::DeleteAll(), m_iLineHt = 0; } // Delete all elements       
    
    //------------------------------------------    
    // Interface elements.
    
    BOOL bModalView_Elements( CMarkerSet* pmkrset );
};  // class CBrowseFieldList



#endif  // Browse_h