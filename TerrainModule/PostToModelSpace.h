#pragma once
class CPostToModelSpace
{
public:
	CPostToModelSpace();
	~CPostToModelSpace();
	static AcDbObjectId PostToModelSpace(AcDbEntity *pEnt);
};

