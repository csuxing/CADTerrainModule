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
//----- CustomPolygonmesh.cpp : Implementation of CCustomPolygonmesh
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "CustomPolygonmesh.h"
#include"Manage_memory.h"
#include<memory>
#include <unordered_map>
#include "BoostGraphic.h"
//-----------------------------------------------------------------------------
Adesk::UInt32 CCustomPolygonmesh::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS (
	CCustomPolygonmesh, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, CUSTOMPOLYGONMESH,
ADSKTEXTUREAPP
|Product Desc:     A description for your object
|Company:          Your company name
|WEB Address:      Your company WEB site address
)

//-----------------------------------------------------------------------------
CCustomPolygonmesh::CCustomPolygonmesh () : AcDbEntity () {
	
	/*
	CReadSrcFile::ReadAscHeadfile("C:\\Users\\xyh19\\Desktop\\CAD二次开发\\Texture\\x64\\Debug\\金沙江10.asc", headfile);
	AscPoint = CReadSrcFile::CreateDynamicArray(AscPoint, headfile.ncols, headfile.nrows);
	CReadSrcFile::ReadASC("C:\\Users\\xyh19\\Desktop\\CAD二次开发\\Texture\\x64\\Debug\\金沙江10.asc", AscPoint, headfile);*/
}

CCustomPolygonmesh::~CCustomPolygonmesh () {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus CCustomPolygonmesh::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (CCustomPolygonmesh::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....
	/*for (int i=0;i<3;i++)
	{
		pFiler->writeItem(pts[i]);
	}
	pFiler->writeItem(flag);*/
	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus CCustomPolygonmesh::dwgInFields (AcDbDwgFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > CCustomPolygonmesh::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < CCustomPolygonmesh::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....
	/*for (int i=0;i<3;i++)
	{
		pFiler->readItem(&pts[i]);
	}
	pFiler->readItem(&flag);*/
	return (pFiler->filerStatus ()) ;
}

//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean CCustomPolygonmesh::subWorldDraw (AcGiWorldDraw *mode) {
	assertReadEnabled () ;
	//////////////////////////////////////////////////////////////////////////
	if (flag==0)
	{
		return (AcDbEntity::subWorldDraw(mode));
	}
	mode->geometry().shell(nbVertex, &pts[0], nbFacenum, &facelist[0]);

	//shared_ptr<int> ptr(new int[100],[](int *p) {delete[] p; });//使用lambda表达式
	return (AcDbEntity::subWorldDraw (mode)) ;
	
}


Adesk::UInt32 CCustomPolygonmesh::subSetAttributes (AcGiDrawableTraits *traits) {
	assertReadEnabled () ;
	return (AcDbEntity::subSetAttributes (traits)) ;
	
}

	//- Osnap points protocol
Acad::ErrorStatus CCustomPolygonmesh::subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d &pickPoint,
	const AcGePoint3d &lastPoint,
	const AcGeMatrix3d &viewXform,
	AcGePoint3dArray &snapPoints,
	AcDbIntArray &geomIds) const
{
	assertReadEnabled () ;
	return (AcDbEntity::subGetOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds)) ;
}

Acad::ErrorStatus CCustomPolygonmesh::subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d &pickPoint,
	const AcGePoint3d &lastPoint,
	const AcGeMatrix3d &viewXform,
	AcGePoint3dArray &snapPoints,
	AcDbIntArray &geomIds,
	const AcGeMatrix3d &insertionMat) const
{
	assertReadEnabled () ;
	return (AcDbEntity::subGetOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds, insertionMat)) ;
}

//- Grip points protocol
Acad::ErrorStatus CCustomPolygonmesh::subGetGripPoints (
	AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds
) const {
	assertReadEnabled () ;
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new getGripPoints() method below (which is the default implementation)

	return (AcDbEntity::subGetGripPoints (gripPoints, osnapModes, geomIds)) ;
}

Acad::ErrorStatus CCustomPolygonmesh::subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) {
	assertWriteEnabled () ;
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new moveGripPointsAt() method below (which is the default implementation)

	return (AcDbEntity::subMoveGripPointsAt (indices, offset)) ;
}

Acad::ErrorStatus CCustomPolygonmesh::subGetGripPoints (
	AcDbGripDataPtrArray &grips, const double curViewUnitSize, const int gripSize, 
	const AcGeVector3d &curViewDir, const int bitflags
) const {
	assertReadEnabled () ;

	//----- If you return eNotImplemented here, that will force AutoCAD to call
	//----- the older getGripPoints() implementation. The call below may return
	//----- eNotImplemented depending of your base class.
	return (AcDbEntity::subGetGripPoints (grips, curViewUnitSize, gripSize, curViewDir, bitflags)) ;
}

Acad::ErrorStatus CCustomPolygonmesh::subMoveGripPointsAt (
	const AcDbVoidPtrArray &gripAppData, const AcGeVector3d &offset,
	const int bitflags
) {
	assertWriteEnabled () ;

	//----- If you return eNotImplemented here, that will force AutoCAD to call
	//----- the older getGripPoints() implementation. The call below may return
	//----- eNotImplemented depending of your base class.
	return (AcDbEntity::subMoveGripPointsAt (gripAppData, offset, bitflags)) ;
}

void CCustomPolygonmesh::setflag(int x)
{
	assertWriteEnabled();
	flag = x;
}

AcDbObjectId CCustomPolygonmesh::createMaterial(CString Material_Name, CString File_Name, double uScale, double vScale,AcGeVector3d transport)
{
	AcDbDictionary* pMaterialDict;
	Acad::ErrorStatus es;
	es = acdbHostApplicationServices()->workingDatabase()->getMaterialDictionary(pMaterialDict, AcDb::kForWrite);
	ACHAR Name_ACHAR[256], File_Name_ACHAR[256];
	_tcscpy(Name_ACHAR, Material_Name);
	_tcscpy(File_Name_ACHAR, File_Name);
	AcDbObjectId materialId = NULL;
	if (es == Acad::eOk)//如果材质浏览器可以打开
	{
		AcGiImageFileTexture tex;
		tex.setSourceFileName(File_Name_ACHAR);//设置贴图路径
		
		//double uScale = 1.0;
		//double vScale = 1.0;
		double uOffset = 0;
		double vOffset = 0;

		AcGeMatrix3d mx;//设置贴图位置


		mx(0, 0) = uScale;
		mx(0, 1) = 0;
		mx(0, 2) = 0;
		mx(0, 3) = uScale * uOffset;

		mx(1, 0) = 0;
		mx(1, 1) = vScale;
		mx(1, 2) = 0;
		mx(1, 3) = vScale * vOffset;

		mx(2, 0) = 0;
		mx(2, 1) = 0;
		mx(2, 2) = 1;
		mx(2, 3) = 0;

		mx(3, 0) = 0;
		mx(3, 1) = 0;
		mx(3, 2) = 0;
		mx(3, 3) = 1;
		//mx.setToRotation(270, AcGeVector3d::kZAxis);
		AcGeVector3d av1, av2, av3;
		AcGePoint3d ap1;
		mx.getCoordSystem(ap1, av1, av2, av3);//获取当前的坐标原点，x,y,z轴
		
		//mx.setToTranslation(transport);
		//mx.setTranslation(transport);

		mx(0, 3) = transport.x;
		mx(1, 3) = transport.y;
		mx(2, 3) = transport.z;

		AcGiMapper mapper;//映射函数 贴图位置、大小、旋转方式
		mapper.setProjection(AcGiMapper::Projection::kBox);//设置投影 平板模式
		mapper.setUTiling(AcGiMapper::Tiling::kTile);//水平平铺
		//mapper.setUTiling(AcGiMapper::kInheritTiling);
		
		mapper.setVTiling(AcGiMapper::Tiling::kTile);//竖直平铺
		//mapper.setVTiling(AcGiMapper::kInheritTiling);
		mapper.setAutoTransform(AcGiMapper::AutoTransform::kInheritAutoTransform);//自动旋转
		mapper.setTransform(mx);
		

		AcGiMaterialMap map;//材质组件映射
		map.setSourceFileName(File_Name_ACHAR);//此方法被弃用了，在tex中设置
		map.setTexture(&tex);
		map.setBlendFactor(1.0);//使贴图完全不透明
		map.setMapper(mapper);//对贴图进行设置、包括大小、旋转方式
		
		AcDbMaterial* pMaterialObj = new AcDbMaterial();//设置材质名称
		pMaterialObj->setName(Name_ACHAR);
		pMaterialObj->setReflectanceScale(0.0);
		pMaterialObj->setShininess(0.0);
		
		pMaterialObj->setTwoSided(Adesk::kFalse);

		es = pMaterialDict->setAt(Name_ACHAR, pMaterialObj, materialId);

		AcGiMaterialColor diffuseColor;//定义材料组件的颜色
		pMaterialObj->setDiffuse(diffuseColor, map);
		pMaterialObj->setMode(AcGiMaterialTraits::kRealistic);

		pMaterialObj->close();
		pMaterialDict->close();
	}
	return materialId;
}

void CCustomPolygonmesh::setfilepath(std::string filepath)
{
	inputfilepath = filepath;
}

void CCustomPolygonmesh::InitDataForPaint(HeadFile headfile, std::vector<float>& vecpts, 
	std::vector<vector<AcGePoint3d>> &hole_pts,std::vector<long> &index, unsigned long holepoint_num)
{
	facelist.clear();
	pts.clear();
	
	unsigned long tmpPointNum= headfile.ncols*headfile.nrows;
	unordered_map<long, SPosition_hole> mapindex;
	
	for (int i=0;i<hole_pts.size();i++)
	{
		mapindex.insert(pair<long,SPosition_hole>(index[i],SPosition_hole(hole_pts[i].size(),tmpPointNum)));
		tmpPointNum = tmpPointNum + hole_pts[i].size();
	}

	nbVertex = vecpts.size() + holepoint_num;
	pts.resize(nbVertex);

	
	unsigned long eleNum = headfile.ncols*headfile.nrows;//原始点总数
	int count = 0;
	for (int i = 0; i < headfile.nrows; i++)
		for (int j = 0; j < headfile.ncols; j++)
		{
			/*pts[count].set(headfile.xllcorner+j*headfile.cellsize, headfile.yllcorner+(headfile.nrows-i-1)*headfile.cellsize, vecpts[count]);
			count++;*/
			pts[count].set(headfile.xllcorner+j*headfile.cellsize, headfile.yllcorner+i*headfile.cellsize, vecpts[j+(headfile.nrows-i-1)*headfile.ncols]);
			count++;
		}
	nbFacenum = ((headfile.nrows - 1)*(headfile.ncols - 1)-index.size())* 5;
	for (int i = 0; i < (int)hole_pts.size(); i++)
	{
		if (hole_pts[i].size()>0)
		{
			//nbFacenum = nbFacenum + hole_pts[i].size() + 1;四边形面片
			nbFacenum = nbFacenum + (hole_pts[i].size() - 2) * 4;
		}
		for (int j = 0; j < hole_pts[i].size(); j++)
		{
			pts[count] = hole_pts[i][j];
			count++;
		}
	}
	facelist.resize(nbFacenum);

	long fcount = 0;
	unsigned gridCount = -1;
	for (int i = 0; i < headfile.nrows - 1; i++)
		for (int j = 0; j < headfile.ncols - 1; j++)
		{
			//没有洞
			gridCount++;
			auto it = mapindex.find(gridCount);
			if (it==mapindex.end())
			{
				facelist[fcount] = 4;
				fcount++;
				facelist[fcount] = i * headfile.ncols + j;
				fcount++;
				facelist[fcount] = facelist[fcount - 1] + 1;
				fcount++;
				facelist[fcount] = (i + 1)*headfile.ncols + j + 1;
				fcount++;
				facelist[fcount] = facelist[fcount - 1] - 1;
				fcount++;
			}
			else
			{
				if ((it->second).size==0)
				{
					continue;
				}
				std::vector<BO_point2d> input;
				std::vector<int> Original;
				std::vector<std::vector<int>> output;
				for (int i=0;i<(it->second).size;i++)
				{
					input.push_back(BO_point2d(pts[(it->second).startPositionInVertex+i].x, 
						pts[(it->second).startPositionInVertex+i].y));
					Original.push_back(i);
				}
				CustomBoost::BoostGraphic::Triangulation(input,Original,output);

				for (int i=0;i<output.size();i++)
				{
					facelist[fcount++] = 3;
					//very very very important 
					facelist[fcount++] = (it->second).startPositionInVertex + output[i][0];
					facelist[fcount++] = (it->second).startPositionInVertex + output[i][2];
					facelist[fcount++] = (it->second).startPositionInVertex + output[i][1];
				}
			}
		}
}

void CCustomPolygonmesh::InitCustomShell(std::string filepath, std::string blockName)
{
	string::size_type position;
	position = filepath.find("VirtualDEM");
	//没有找到此 字符串 数据源为3D数据
	if (position==filepath.npos)
	{
		std::vector<float> tmpVecHeight;
		std::vector<long> tmpIndex;
		std::vector<vector<AcGePoint3d>> tmpHole_pts;
		unsigned long tmpHolenum=0;

		int p_dem = filepath.find("dem");
		std::string modifyPath = filepath.substr(0, p_dem) + "modify\\";
		
		CustomBoost::BoostGraphic::readHoleFileForPaint(modifyPath+blockName, tmpHole_pts);
		for (int i=0;i<tmpHole_pts.size();i++)
		{
			tmpHolenum = tmpHolenum + tmpHole_pts[i].size();
		}
		CReadSrcFile::ReadASCIIHeadfile(filepath+blockName, headfile);
		CReadSrcFile::ReadASCII(tmpVecHeight, filepath+blockName);
		CReadSrcFile::ReadASCIIindex(modifyPath+blockName+"_index", tmpIndex);
		InitDataForPaint(headfile, tmpVecHeight, tmpHole_pts, tmpIndex, tmpHolenum);
	}
	//数据源为2D数据
	else
	{
		std::vector<float> tmpVecHeight;
		std::vector<long> tmpIndex;
		std::vector<vector<AcGePoint3d>> tmpHole_pts;
		unsigned long tmpHolenum=0;
		CReadSrcFile::ReadASCIIHeadfile(filepath + blockName, headfile);
		CReadSrcFile::ReadASCII(tmpVecHeight, filepath + blockName);
		InitDataForPaint(headfile, tmpVecHeight, tmpHole_pts, tmpIndex, tmpHolenum);
	}
}

