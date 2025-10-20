// update.h  Notify objects when one they own or refer to is updated

#ifndef UPDATE_H
#define UPDATE_H

class CDatabaseType;
class CLangEnc;
class CShwView;
class CShwDoc;

// Dispatch functions for updates. Written by Mark P. Documentation by Alan B 2-2-96
// See more documentation at bottom of file.
class CUpdate  // Hungarian: up
{
public:
	virtual void UpdateDatabaseType(CDatabaseType* ptyp) = 0;
	virtual void UpdateLangEnc(CLangEnc* plng) = 0;
	virtual void UpdateShwView(CShwView* pview) = 0;
	virtual void UpdateShwDoc(CShwDoc* pdoc) = 0;
};  // class CUpdate

#ifdef DOCUMENTATION
/* These rely on polymorphism to call the appropriate functions.
CUpdate is a virtual base class for all update classes so that pointers
 	to a general update can call the appropriate function.
For an example of use, see CMarkerUpdate in mkr.h. It overrides these
 	functions, with implementations in update.cpp.
*/
// For example, CMarkerSet::bAddMarker (in mkr.cpp) calls Shw_Update to notify all other objects that a marker has been added
	CMarkerUpdate mup(pmkr); // Construct an update notification of a marker add
	extern void Shw_Update(CUpdate& up); // Declare so shw.h doesn't have to be included
	Shw_Update(mup);  // Notify all other objects that a marker has been added
// Shw_Update passes the update down to the app, by calling CShwApp::Update(CUpdate& up) (in shw.cpp)
	Shw_app().Update(up);
// CShwApp::Update notifies the app's set of all database types about the update, 
// by calling CDatabaseTypeSet::Update(CUpdate& up) (in typ.cpp) on the app's set of all database types
	m_typset.Update(up);
// CDatabaseTypeSet::Update uses a loop to inform all database types of the update.
// It calls CDatabaseTypeProxy::Update(CUpdate& up) (in typ.cpp)
	CDatabaseTypeProxy* ptrx = ptrxFirst();
	for ( ; ptrx; ptrx = ptrxNext(ptrx) )
		ptrx->Update(up)
// CDatabaseTypeProxy::Update passes the update to the database type,
// by calling CDatabaseType::Update(CUpdate& up) (in typ.cpp)
	if ( m_ptyp )
		m_ptyp->Update(up);
// CDatabaseType::Update uses polymorphism to tell the marker update a database type is being notified,
// by calling CMarkerUpdate::UpdateDatabaseType(CDatabaseType* ptyp) (in update.cpp)	
	up.UpdateDatabaseType(this);
// CMarkerUpdate::UpdateDatabaseType tells the database type a marker is being updated,
// by calling CDatabaseType::MarkerUpdated(CMarkerUpdate& mup) (in typ.cpp)	
	ptyp->MarkerUpdated(*this);
// CDatabaseType::MarkerUpdated sets m_bModified if the marker is in the marker set of the database type.
	m_bModified = m_bModified || mup.pmkr()->pmkrsetMyOwner() == pmkrset();
// m_bModified is used to prevent writing the database type settings file if not modified. It is used only in CDatabaseType::WritePropertiesIfModified()
	if ( !m_bModified )
		return;
#endif // Documentation

#endif  // not UPDATE_H

