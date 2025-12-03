// intsetup.cpp Interlinear setup

#include "stdafx.h"
#include "toolbox.h"
#include "intsetup.h"
#include "obstream.h"  // Object_istream, Object_ostream

static const char* psz_InterlinearSetup = "InterlinearSetup";
static const char* psz_InterlinearSetupSet = "InterlinearSetupSet";

//------------------------------------------------------------------------
#ifdef typWritefstream // 1.6.4ac
void CInterlinearSetup::WriteProperties( Object_ofstream& obs )
#else
void CInterlinearSetup::WriteProperties( Object_ostream& obs )
#endif
{
    obs.WriteNewline();
    obs.WriteBeginMarker( psz_InterlinearSetup, pszKey() );

    m_intprclst.WriteProperties( obs );
        
    obs.WriteEndMarker( psz_InterlinearSetup );
}   

BOOL CInterlinearSetup::s_bReadProperties( Object_istream& obs,
        CInterlinearSetup** ppintsup )
{
    Str8 sName;
    if ( !obs.bReadBeginMarker( psz_InterlinearSetup, sName ) ) // If not a setup, fail
        return FALSE;
    *ppintsup = new CInterlinearSetup( sName );
    
    while ( !obs.bAtEnd() )
        {
        if ( (*ppintsup)->m_intprclst.bReadProperties( obs ) )
            ;
        else if ( obs.bEnd( psz_InterlinearSetup ) ) // If end or begin something else, break
            break;
        }
    return TRUE;
}


CSet* CInterlinearSetup::psetMyOwner()
{
    ASSERT( FALSE );  // 1996-03-11 MRP: TO DO!!!
    return NULL;
}

//------------------------------------------------------------------------
CInterlinearSetupSet::~CInterlinearSetupSet()
{
    CInterlinearSetup* pintsup = pintsupFirst();
    while (pintsup)
        {
        Delete(&pintsup);
        pintsup = pintsupFirst();
        }
}

#ifdef typWritefstream // 1.6.4ac
void CInterlinearSetupSet::WriteProperties( Object_ofstream& obs ) const
#else
void CInterlinearSetupSet::WriteProperties( Object_ostream& obs ) const
#endif
{
    CInterlinearSetup* pintsup = pintsupFirst();
    if ( !pintsup ) // If no setups, don't write the markers
        return;
    obs.WriteNewline();
    obs.WriteBeginMarker( psz_InterlinearSetupSet );
    for ( ; pintsup; pintsup = pintsupNext( pintsup ) )
        pintsup->WriteProperties( obs );
    obs.WriteNewline();
    obs.WriteEndMarker( psz_InterlinearSetupSet );
}          

BOOL CInterlinearSetupSet::bReadProperties( Object_istream& obs )
{
    if ( !obs.bReadBeginMarker( psz_InterlinearSetupSet ) )
        return FALSE;
    while ( !obs.bAtEnd() )
        {
        CInterlinearSetup* pintsup = NULL;
        if ( CInterlinearSetup::s_bReadProperties( obs, &pintsup ) ) // If we read a process, add it to list
            {
            if ( pintsup ) // If valid setup, add it to the list
                Add( pintsup );
            }   
        else if ( obs.bEnd( psz_InterlinearSetupSet ) ) // If end of list, stop
            break;
        }
   return TRUE;
}

