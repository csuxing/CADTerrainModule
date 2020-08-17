#include "stdafx.h"
#include "PostToModelSpace.h"


CPostToModelSpace::CPostToModelSpace()
{
}


CPostToModelSpace::~CPostToModelSpace()
{
}

AcDbObjectId CPostToModelSpace::PostToModelSpace(AcDbEntity * pEnt)
{
	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
	AcDbBlockTable *pBlockTable;
	Acad::ErrorStatus es;
	es=acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);
	if (es==Acad::eOk)
	{
		AcDbBlockTableRecord *pBlockTableRecord;
		pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,AcDb::kForWrite);
		AcDbObjectId id;
		es=pBlockTableRecord->appendAcDbEntity(id,pEnt);
		pBlockTable->close();
		pBlockTableRecord->close();
		pEnt->close();
		acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
		return id;
	}
	return NULL;
	/*acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
	//	Acad::ErrorStatus     es = Acad::eOk;
	AcDbObjectId objId;
	// CAD R2000
	AcDbObjectId btrId = acdbHostApplicationServices()->workingDatabase()->currentSpaceId();//获取当前绘图空间
	AcDbBlockTableRecord* pSpaceRecord;
	if (Acad::eOk != acdbOpenObject(pSpaceRecord, btrId, AcDb::kForWrite))
	{
		acDocManager->unlockDocument(acDocManager->curDocument());
		return NULL;
	}

	pSpaceRecord->appendAcDbEntity(objId,pEnt);
	pSpaceRecord->close();
	pEnt->draw();
	pEnt->close();
	acDocManager->unlockDocument(acDocManager->curDocument());
	return objId;*/
}
