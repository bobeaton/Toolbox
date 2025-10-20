// update.cpp

#include "stdafx.h"
#include "toolbox.h"
#include "update.h"

#include "mkr.h"
#include "fil.h"

#include "lng.h"  // CLangEnc
#include "typ.h"  // CDatabaseType
#include "shwdoc.h"  // CShwDoc
#include "shwview.h"  // CShwView

// --------------------------------------------------------------------------

void CMarkerUpdate::UpdateLangEnc(CLangEnc*)
{
}

void CMarkerUpdate::UpdateDatabaseType(CDatabaseType* ptyp)
{
	ptyp->MarkerUpdated(*this);
}

void CMarkerUpdate::UpdateShwDoc(CShwDoc* pdoc)
{
	pdoc->MarkerUpdated(*this);
}

void CMarkerUpdate::UpdateShwView(CShwView* pview)
{
	pview->MarkerUpdated(*this);
}


// --------------------------------------------------------------------------

void CFilterUpdate::UpdateLangEnc(CLangEnc*)
{
}

void CFilterUpdate::UpdateDatabaseType(CDatabaseType* ptyp)
{
	ptyp->FilterUpdated(*this);
}

void CFilterUpdate::UpdateShwDoc(CShwDoc* pdoc)
{
	pdoc->FilterUpdated(*this);
}

void CFilterUpdate::UpdateShwView(CShwView* pview)
{
	pview->FilterUpdated(*this);
}


// --------------------------------------------------------------------------

void CLangEncUpdate::UpdateLangEnc(CLangEnc* plng)
{
	plng->LangEncUpdated(*this);
}

void CLangEncUpdate::UpdateDatabaseType(CDatabaseType* ptyp)
{
	ptyp->LangEncUpdated(*this);
}

void CLangEncUpdate::UpdateShwDoc(CShwDoc* pdoc)
{
	pdoc->LangEncUpdated(*this);
}

void CLangEncUpdate::UpdateShwView(CShwView* pview)
{
	pview->LangEncUpdated(*this);
}


// --------------------------------------------------------------------------

void CSortOrderUpdate::UpdateLangEnc(CLangEnc*)
{
}

void CSortOrderUpdate::UpdateDatabaseType(CDatabaseType* ptyp)
{
	ptyp->SortOrderUpdated(*this);
}

void CSortOrderUpdate::UpdateShwDoc(CShwDoc* pdoc)
{
	pdoc->SortOrderUpdated(*this);
}

void CSortOrderUpdate::UpdateShwView(CShwView* pview)
{
	pview->SortOrderUpdated(*this);
}

// --------------------------------------------------------------------------

void CIndexUpdate::UpdateLangEnc(CLangEnc*)
{
}

void CIndexUpdate::UpdateDatabaseType(CDatabaseType* ptyp)
{
	ptyp->IndexUpdated(*this);
}

void CIndexUpdate::UpdateShwDoc(CShwDoc* pdoc)
{
}

void CIndexUpdate::UpdateShwView(CShwView* pview)
{
}
