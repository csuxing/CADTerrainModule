#include "stdafx.h"
#include "CreateGeomtry.h"
#include "PostToModelSpace.h"

CreateGeomtry::CreateGeomtry(void)
{
}


CreateGeomtry::~CreateGeomtry(void)
{
}

AcDbObjectId CreateGeomtry::Fun_createline()
{
	//创建直线所需要的点
	AcGePoint3d pt_start(0, 0, 0);
	AcGePoint3d pt_end(10, 10, 0);
	AcDbLine *pline = new	AcDbLine(pt_start, pt_end);
	//添加获得指向块表的指针代码
	AcDbBlockTable *PBlockTable;
	//获得一个指向当前活动块表的指针
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(PBlockTable, AcDb::kForRead);
	//添加获得指向指定块表的指针的下相关代码
	AcDbBlockTableRecord *PBlockTableRecord;
	//获得指向特定的块表记录的指针（模型空间）
	PBlockTable->getAt(ACDB_MODEL_SPACE, PBlockTableRecord, AcDb::kForWrite);
	//向块表中添加记录
	AcDbObjectId lineld;
	PBlockTableRecord->appendAcDbEntity(lineld, pline);
	//关闭添加图形数据库的各种代码
	PBlockTable->close();
	PBlockTableRecord->close();
	pline->close();
	return lineld;
}

AcDbObjectId CreateGeomtry::Fun_createPline(AcGePoint2dArray points, double width)
{
	int NumberVertices = points.length();
	AcDbPolyline *Polyline = new AcDbPolyline(NumberVertices);
	for (int i=0;i<NumberVertices;i++)
	{
		Polyline->addVertexAt(i, points.at(i),0,width,width);
	}
	AcDbObjectId polyid;
	polyid=CPostToModelSpace::PostToModelSpace(Polyline);
	return polyid;
	//return AcDbObjectId();
}

AcDbObjectId CreateGeomtry::Fun_create3D_Box()
{

	//AcDb3dSolid 
	return AcDbObjectId();
}

AcDbObjectIdArray CreateGeomtry::FunCreateRegion(const AcDbObjectIdArray & curveIds)
{
	AcDbObjectIdArray regionIds;
	AcDbVoidPtrArray curves;
	AcDbVoidPtrArray regions;
	AcDbEntity *pEnt;
	AcDbRegion *pRegion;
	for (int i=0;i<curveIds.length();i++)
	{
		acdbOpenAcDbEntity(pEnt, curveIds.at(i), AcDb::kForRead);
		if (pEnt->isKindOf(AcDbCurve::desc()))
		{
			curves.append(static_cast<void*>(pEnt));
		}
	}
	Acad::ErrorStatus es = AcDbRegion::createFromCurves(curves, regions);
	//return AcDbObjectIdArray();
	if (es==Acad::eOk)
	{
		for (int i=0;i<regions.length();i++)
		{
			pRegion = static_cast<AcDbRegion*>(regions[i]);
			pRegion->setDatabaseDefaults();
			AcDbObjectId regionId;
			regionId = CPostToModelSpace::PostToModelSpace(pRegion);
			regionIds.append(regionId);
		}
	}
	else
	{
		for (int i=0;i<regionIds.length();i++)
		{
			delete(AcRxObject*)regions[i];
		}
	}
	for (int i=0;i<curves.length();i++)
	{
		pEnt = static_cast<AcDbEntity*>(curves[i]);
		pEnt->close();
	}
	return regionIds;
}
int Rand(int min,int max)
{
	//srand((unsigned int)time(NULL));
	return rand() % (max - min+1)+min;
}
void CreateGeomtry::set_3D_material()
{
	//获取软件路径
	CString Ori_FileName = acedGetAppName();
	int p = Ori_FileName.ReverseFind(L'\\');
	Ori_FileName = Ori_FileName.Left(p);
	CString  FileName;

	//设置路基材质
	FileName = Ori_FileName + L"\\image.jpg";
	AcDbObjectId m_MaterialID_Roadbed;
	m_MaterialID_Roadbed = CreateMaterial(L"路基面", FileName, 0.01, 0.01);
	AcGePoint3d pt0(0, 0, 0), pt1(0,1,0), pt2(1,1,0), pt3(1,0,0);
	AcDbFace *face = new AcDbFace(pt0,pt1,pt2);
	face->setMaterial(m_MaterialID_Roadbed);
	//AcDbVoidPtrArray pta;
	//face->explode(pta);
	CPostToModelSpace::PostToModelSpace(face);
	
	AcGePoint3dArray pts;
	int a = 0;
	for(int i=0;i<50;i++)
		for (int j=0;j<50;j++)
		{
			a=Rand(0,5);
			AcGePoint3d ptmp(j,i,a);
			pts.append(ptmp);	
		}
	AcDbPolygonMesh *mesh = new AcDbPolygonMesh(AcDb::kQuadSurfaceMesh, 50, 50, pts,Adesk::kFalse,Adesk::kFalse);
	mesh->setMaterial(m_MaterialID_Roadbed);
	/*
	AcDbVoidPtrArray op;
	Acad::ErrorStatus e;
	e=mesh->explode(op);//将地形炸开

	if (e!=Acad::eOk)
	{
		AfxMessageBox(L"can not explode");
	}
	//CPostToModelSpace::PostToModelSpace();
	for (int i=0;i<op.length();i++)
	{
		AcDbEntity *pent=(AcDbEntity *)op[i];
		if (i % 3 == 0)
		{
			pent->setMaterial(m_MaterialID_Roadbed);
		}
		CPostToModelSpace::PostToModelSpace(pent);

		
	}*/
	CPostToModelSpace::PostToModelSpace(mesh);
	AfxMessageBox(L"OK");
}

AcDbObjectId CreateGeomtry::CreateMaterial(CString Material_Name, CString File_Name, double uScale, double vScale)
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
		//AcGeVector3d av(1,0,0);
		//mx.setToTranslation(av);

		AcGiMapper mapper;//映射函数 贴图位置、大小、旋转方式
		mapper.setProjection(AcGiMapper::Projection::kBox);//设置投影 平板模式
		mapper.setUTiling(AcGiMapper::Tiling::kTile);//水平平铺
		mapper.setVTiling(AcGiMapper::Tiling::kTile);//竖直平铺
		//mapper.setAutoTransform(AcGiMapper::AutoTransform::kNone);//自动旋转
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
		//
		pMaterialObj->setTwoSided(Adesk::kTrue);
		//pMaterialObj->
		es = pMaterialDict->setAt(Name_ACHAR, pMaterialObj, materialId);

		AcGiMaterialColor diffuseColor;//定义材料组件的颜色
		pMaterialObj->setDiffuse(diffuseColor, map);
		pMaterialObj->setMode(AcGiMaterialTraits::kRealistic);

		pMaterialObj->close();
		pMaterialDict->close();
	}
	return materialId;
}
