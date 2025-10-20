// dbtypmac.h Database type and marker Alan Buseman 21 Oct 06

#ifndef CDBTypMac_H
#define CDBTypMac_H

#include "dbmac.h"

class CMkr : public CDblLstEl
	{
public:
public:
	CMkr() {}
	~CMkr() {}
	CMkr* pmkrNext() { return (CMkr*)pelNext; }
	CMkr* pmkrPrev() { return (CMkr*)pelPrev; }
	};

class CMkrLst : public CDblLst
	{
public:
public:
	CMkrLst() {}
	~CMkrLst() {}
	CMkr* pmkrFirst() { return (CMkr*)pelFirst; }
	CMkr* pmkrLast() { return (CMkr*)pelLast; }
	};

class CDBTyp
	{
public:
	CMkrLst* pmkrlst;
public:
	CDBTyp() {}
	~CDBTyp() {}
	};

#endif // CDBTypMac_H
