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
	//����ֱ������Ҫ�ĵ�
	AcGePoint3d pt_start(0, 0, 0);
	AcGePoint3d pt_end(10, 10, 0);
	AcDbLine *pline = new	AcDbLine(pt_start, pt_end);
	//��ӻ��ָ�����ָ�����
	AcDbBlockTable *PBlockTable;
	//���һ��ָ��ǰ�����ָ��
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(PBlockTable, AcDb::kForRead);
	//��ӻ��ָ��ָ������ָ�������ش���
	AcDbBlockTableRecord *PBlockTableRecord;
	//���ָ���ض��Ŀ���¼��ָ�루ģ�Ϳռ䣩
	PBlockTable->getAt(ACDB_MODEL_SPACE, PBlockTableRecord, AcDb::kForWrite);
	//��������Ӽ�¼
	AcDbObjectId lineld;
	PBlockTableRecord->appendAcDbEntity(lineld, pline);
	//�ر����ͼ�����ݿ�ĸ��ִ���
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
	//��ȡ���·��
	CString Ori_FileName = acedGetAppName();
	int p = Ori_FileName.ReverseFind(L'\\');
	Ori_FileName = Ori_FileName.Left(p);
	CString  FileName;

	//����·������
	FileName = Ori_FileName + L"\\image.jpg";
	AcDbObjectId m_MaterialID_Roadbed;
	m_MaterialID_Roadbed = CreateMaterial(L"·����", FileName, 0.01, 0.01);
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
	e=mesh->explode(op);//������ը��

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
	if (es == Acad::eOk)//���������������Դ�
	{
		AcGiImageFileTexture tex;
		tex.setSourceFileName(File_Name_ACHAR);//������ͼ·��

		//double uScale = 1.0;
		//double vScale = 1.0;
		double uOffset = 0;
		double vOffset = 0;

		AcGeMatrix3d mx;//������ͼλ��
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
		mx.getCoordSystem(ap1, av1, av2, av3);//��ȡ��ǰ������ԭ�㣬x,y,z��
		//AcGeVector3d av(1,0,0);
		//mx.setToTranslation(av);

		AcGiMapper mapper;//ӳ�亯�� ��ͼλ�á���С����ת��ʽ
		mapper.setProjection(AcGiMapper::Projection::kBox);//����ͶӰ ƽ��ģʽ
		mapper.setUTiling(AcGiMapper::Tiling::kTile);//ˮƽƽ��
		mapper.setVTiling(AcGiMapper::Tiling::kTile);//��ֱƽ��
		//mapper.setAutoTransform(AcGiMapper::AutoTransform::kNone);//�Զ���ת
		mapper.setTransform(mx);
		

		AcGiMaterialMap map;//�������ӳ��
		map.setSourceFileName(File_Name_ACHAR);//�˷����������ˣ���tex������
		map.setTexture(&tex);
		map.setBlendFactor(1.0);//ʹ��ͼ��ȫ��͸��
		map.setMapper(mapper);//����ͼ�������á�������С����ת��ʽ

		AcDbMaterial* pMaterialObj = new AcDbMaterial();//���ò�������
		pMaterialObj->setName(Name_ACHAR);
		pMaterialObj->setReflectanceScale(0.0);
		pMaterialObj->setShininess(0.0);
		//
		pMaterialObj->setTwoSided(Adesk::kTrue);
		//pMaterialObj->
		es = pMaterialDict->setAt(Name_ACHAR, pMaterialObj, materialId);

		AcGiMaterialColor diffuseColor;//��������������ɫ
		pMaterialObj->setDiffuse(diffuseColor, map);
		pMaterialObj->setMode(AcGiMaterialTraits::kRealistic);

		pMaterialObj->close();
		pMaterialDict->close();
	}
	return materialId;
}
