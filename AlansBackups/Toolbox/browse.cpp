#include "stdafx.h"
#include "toolbox.h"

#include "browse.h"
#include "browse_d.h"
#include "tools.h"
#include "obstream.h"  // Object_istream, Object_ostream

// 09-04-1997 Added parameter 3 to paramterlist
CBrowseField::CBrowseField( const CMarker* pmkr, int iWidth , BOOL bRightJustified )
{
    m_pmkr = (CMarker*)pmkr; // cast away constness
    m_iWidth = iWidth;
    m_bRightJustified = bRightJustified;
}

CBrowseField::~CBrowseField()
{
}

static const char* psz_width = "width";
static const char* psz_brf = "brf";
static const char* psz_justified = "rightjustify";   //09-04-1997
static const char* psz_FieldEmpty = "FieldEmptyEntry";  //09-23-1997
static const char* psz_NoField = "NoFieldEntry";  //09-23-1997
static const char* psz_FieldEmptyWhite = "FieldEmptyEntry_White";  //09-23-1997
static const char* psz_NoFieldWhite = "NoFieldEntry_White";  //09-23-1997

#ifdef typWritefstream // 1.6.4ac
void CBrowseField::WriteProperties(Object_ofstream& obs) const 
#else
void CBrowseField::WriteProperties(Object_ostream& obs) const 
#endif
{
    obs.WriteBeginMarker(psz_brf);
    m_pmkr->WriteRef(obs, "");
    obs.WriteInteger(psz_width, iWidth());
    obs.WriteBool(psz_justified,m_bRightJustified);   //09-04-1997
    obs.WriteEndMarker(psz_brf);
}

BOOL CBrowseField::s_bReadProperties(Object_istream& obs, CMarkerSet* pmkrset,
                                        CBrowseField** ppbrf)
{
    if ( !obs.bReadBeginMarker(psz_brf) )
        return FALSE;
        
    CMarker* pmkr = NULL;
    int iWidth = eMinFieldWidth;
    BOOL bRightJustified = FALSE;
    
    while ( !obs.bAtEnd() )
        {
        if ( pmkrset->bReadMarkerRef(obs, "", pmkr) )
            ;
        else if ( obs.bReadInteger(psz_width, iWidth) )
            ;
        else if ( obs.bReadBool(psz_justified, bRightJustified) ) //09-04-1997
            ;
        else if ( obs.bEnd(psz_brf) )
            break;
        }

      // 09-04-1997 Added parameter 3 to paramterlist
    *ppbrf = pmkr ? new CBrowseField( pmkr, iWidth, bRightJustified ) : NULL;
    return TRUE;            
}

#ifdef _DEBUG
#endif // _DEBUG    


// --------------------------------------------------------------------------

static const char* psz_brflst = "brflst";

CBrowseFieldList::CBrowseFieldList()
{
    m_iLineHt = 0;

      //09-23-1997
      //If there is a entry in the project file, the default will be overwritten
    m_sFieldEmpty = _("*empty*");
    m_sNoField = _("*no field*");
}

#ifdef typWritefstream // 1.6.4ac
void CBrowseFieldList::WriteProperties(Object_ofstream& obs) const
#else
void CBrowseFieldList::WriteProperties(Object_ostream& obs) const
#endif
{
    obs.WriteBeginMarker(psz_brflst);
    
      //if the Entry is "" nothing will be stored in the project file.
      //For this case we need a flag.
    if ( m_sFieldEmpty.GetLength() )
        obs.WriteString(psz_FieldEmpty, m_sFieldEmpty);   //09-23-1997
    else
        obs.WriteBool(psz_FieldEmptyWhite, TRUE);
    if (m_sNoField.GetLength())
        obs.WriteString(psz_NoField, m_sNoField);         //09-23-1997
    else
        obs.WriteBool(psz_NoFieldWhite , TRUE);
    
    for ( CBrowseField* pbrf = pbrfFirst(); pbrf; pbrf = pbrfNext(pbrf) )
        pbrf->WriteProperties(obs);
        
    obs.WriteEndMarker(psz_brflst);
}

BOOL CBrowseFieldList::bReadProperties(Object_istream& obs, CMarkerSet* pmkrset)
{
    if ( !obs.bReadBeginMarker(psz_brflst) )
        return FALSE;

      //09-23-1997  
      //Need this Bool for a parameterlist
    BOOL bTemp;
    
    while ( !obs.bAtEnd() )
        {
        CBrowseField* pbrf = NULL;
        if ( CBrowseField::s_bReadProperties(obs, pmkrset, &pbrf) )
            {
            if ( pbrf )
                Add(pbrf);
            }
        else if (obs.bReadString (psz_FieldEmpty, m_sFieldEmpty) )  //09-23-1997
            ;
        else if (obs.bReadBool(psz_FieldEmptyWhite, bTemp) )
            m_sFieldEmpty = "";
        else if (obs.bReadString (psz_NoField, m_sNoField) )   //09-23-1997
            ;
        else if (obs.bReadBool(psz_NoFieldWhite, bTemp) )
            m_sNoField = "";
        else if ( obs.bEnd(psz_brflst) )
            break;
        }
        
    return TRUE;
}

void CBrowseFieldList::RecalcLineHt() // make sure m_iLineHt correct
{
    m_iLineHt = 0;
    for ( CBrowseField* pbrf = pbrfFirst(); pbrf; pbrf = pbrfNext(pbrf) )
        m_iLineHt = max( m_iLineHt, pbrf->pmkr()->iLineHeight() );
}

void CBrowseFieldList::Add( const CMarker* pmkr )
{
    
    CBrowseField* pbrf = new CBrowseField( pmkr );
    Add(pbrf);
}

void CBrowseFieldList::Add( CBrowseField* pbrf )
{
    CDblList::Add(pbrf);
    m_iLineHt = max( m_iLineHt, pbrf->pmkr()->iLineHeight() ); // adjust line height if necessary
}

BOOL CBrowseFieldList::bModalView_Elements( CMarkerSet* pmkrset )
{
    CBrowseFieldsDlg dlg( pmkrset, this );
    return( dlg.DoModal() == IDOK );
}

CBrowseFieldList& CBrowseFieldList::operator=(const CBrowseFieldList& brflst)
{
    if (this == &brflst) 
        return *this;

    DeleteAll();

    m_sFieldEmpty = brflst.m_sFieldEmpty;
    m_sNoField = brflst.m_sNoField;
    m_iLineHt = brflst.m_iLineHt;

      // Copy the list
    for ( CBrowseField* pbrf = brflst.pbrfFirst(); pbrf; pbrf = brflst.pbrfNext( pbrf ) )
        Add( new CBrowseField( pbrf->pmkr(), pbrf->iWidth(), pbrf->bIsRightJustified() ) );

    return *this;
}
