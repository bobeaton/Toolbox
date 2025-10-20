// dbmac.h Database, record, and field Alan Buseman 21 Oct 06

#ifndef CDBMac_H
#define CDBMac_H

#include "Str8.h"
#include "cdbllstmac.h"
#include "dbtypmac.h"

class CFld : public CDblLstEl, public Str8
	{
public:
	CMkr* pmkr;
public:
	CFld( CMkr* pmkrA ) { pmkr = pmkrA; }
	~CFld() {}
	CFld* pfldNext() { return (CFld*)pelNext; }
	CFld* pfldPrev() { return (CFld*)pelPrev; }
	};

class CRec : public CDblLst, public CDblLstEl
	{
public:
	CDBTyp* ptyp;
public:
	CRec( CDBTyp* ptypA ) { ptyp = ptypA; }
	~CRec() {}
	CFld* pfldFirst() { return (CFld*)pelFirst; }
	CFld* pfldLast() { return (CFld*)pelLast; }
	CRec* precNext() { return (CRec*)pelNext; }
	CRec* precPrev() { return (CRec*)pelPrev; }
	};

class CDB : public CDblLst
	{
public:
	CDBTyp* ptyp;
public:
	CDB( CDBTyp* ptypA ) { ptyp = ptypA; }
	~CDB() {}
	CRec* precFirst() { return (CRec*)pelFirst; }
	CRec* precLast() { return (CRec*)pelLast; }
	};

#endif // CDBMac_H
