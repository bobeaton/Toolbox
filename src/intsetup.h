// intsetup.h Interlinear Setups // AB 2-28-96 Currently unused, still under consideration

#include "set.h"  // classes CSet and CSetEl
#include "mkr.h"
#include "interlin.h"

class CInterlinearSetup : public CSetEl // intsup Interlinear Setup class
{
private:
    CInterlinearProcList m_intprclst;
public:
    CInterlinearSetup( const char* pszName ) : CSetEl( pszName ) {} // constructor

    static BOOL s_bReadProperties( Object_istream& obs, CInterlinearSetup** ppintsup );
#ifdef typWritefstream // 1.6.4ac
    void WriteProperties( Object_ofstream& obs ); 
#else
    void WriteProperties( Object_ostream& obs ); 
#endif

    virtual CSet* psetMyOwner();

    BOOL bCopy(CSetEl** ppselNew) { return FALSE; }
    BOOL bModify() { return FALSE; }
    BOOL bDeletable() { return FALSE; }
};

class CInterlinearSetupSet : public CSet // intsupset Interlinear Setup Set class
{
public:
    CInterlinearSetupSet() {}
    ~CInterlinearSetupSet();

#ifdef typWritefstream // 1.6.4ac
    void WriteProperties( Object_ofstream& obs ) const;
#else
    void WriteProperties( Object_ostream& obs ) const;
#endif
    BOOL bReadProperties( Object_istream& obs );

    CInterlinearSetup* pintsupSearch( const char* psz ) const // Perform a search
        { return (CInterlinearSetup*) pselSearch( psz ); }
    CInterlinearSetup* pintsupFirst() const // First element
        { return (CInterlinearSetup*) CDblList::pelFirst(); }
    CInterlinearSetup* pintsupLast() const // Last element
        { return (CInterlinearSetup*) CDblList::pelLast(); }
    CInterlinearSetup* pintsupNext( CInterlinearSetup* pintsupCur ) const // Next element after current
        { return (CInterlinearSetup*) CDblList::pelNext( (CDblListEl*) pintsupCur ); }
    CInterlinearSetup* pintsupPrev( CInterlinearSetup* pintsupCur ) const // Previous element after current
        { return (CInterlinearSetup*) CDblList::pelPrev( (CDblListEl*) pintsupCur ); }
    void Delete( CInterlinearSetup** ppintsup ) // Remove element, update pel
	{ CSet::Delete( (CSetEl**)ppintsup ); }  
    BOOL bAdd( CSetEl** ppselNew ) { return FALSE; }
};