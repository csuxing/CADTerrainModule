// (C) Copyright 2002-2007 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- CustomPolygonmesh.h : Declaration of the CCustomPolygonmesh
//-----------------------------------------------------------------------------
#pragma once
#include"CReadSrcFile.h"
#include<vector>

using namespace std;
#ifdef TEXTURE_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else
//----- Note: we don't use __declspec(dllimport) here, because of the
//----- "local vtable" problem with msvc. If you use __declspec(dllimport),
//----- then, when a client dll does a new on the class, the object's
//----- vtable pointer points to a vtable allocated in that client
//----- dll. If the client dll then passes the object to another dll,
//----- and the client dll is then unloaded, the vtable becomes invalid
//----- and any virtual calls on the object will access invalid memory.
//-----
//----- By not using __declspec(dllimport), we guarantee that the
//----- vtable is allocated in the server dll during the ctor and the
//----- client dll does not overwrite the vtable pointer after calling
//----- the ctor. And, since we expect the server dll to remain in
//----- memory indefinitely, there is no problem with vtables unexpectedly
//----- going away.
#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "dbmain.h"

//-----------------------------------------------------------------------------
struct SPosition_hole
{
	unsigned size;
	unsigned startPositionInVertex;
	SPosition_hole() {}
	SPosition_hole(unsigned pointnum, unsigned startposition) { size = pointnum; startPositionInVertex = startposition; }
};
class DLLIMPEXP CCustomPolygonmesh : public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(CCustomPolygonmesh) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	CCustomPolygonmesh () ;
	virtual ~CCustomPolygonmesh () ;

	//----- AcDbObject protocols
	//- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

	//----- AcDbEntity protocols
	//- Graphics protocol
protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;
	virtual Adesk::UInt32 subSetAttributes (AcGiDrawableTraits *traits) ;

	//- Osnap points protocol
public:
	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		Adesk::GsMarker gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		Adesk::GsMarker gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds,
		const AcGeMatrix3d &insertionMat) const ;

	//- Grip points protocol
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;
	virtual Acad::ErrorStatus subGetGripPoints (
		AcDbGripDataPtrArray &grips, const double curViewUnitSize, const int gripSize, 
		const AcGeVector3d &curViewDir, const int bitflags) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbVoidPtrArray &gripAppData, const AcGeVector3d &offset, const int bitflags) ;
public:
	//vector<vector<AcGePoint3d>> hole_pts;//存储地形中的洞 无用
	//unsigned long holepoint_num;//无用
	//vector<float> vecpts;//存储数据 无用

	HeadFile headfile;//存储头文件
	AcDbObjectId createMaterial(CString Material_Name, CString File_Name, double uScale, double vScale,AcGeVector3d transport=AcGeVector3d(0,0,0));
private:
	unsigned long nbVertex;//点数组的大小
	unsigned long nbFacenum;//面索引数组的大小
	vector<long> facelist;
	vector<AcGePoint3d> pts;
	int flag;
	std::string inputfilepath;//读取asc文件的路径
public:
	void setflag(int x);//是否绘制
	void setfilepath(std::string filepath);
	void InitDataForPaint(HeadFile headfile,std::vector<float> &vecpts,
		std::vector<vector<AcGePoint3d>> &hole_pts, std::vector<long> &index,unsigned long holepoint_num);
	void InitCustomShell(std::string filepath,std::string blockName);
	
} ;

#ifdef TEXTURE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(CCustomPolygonmesh)
#endif
