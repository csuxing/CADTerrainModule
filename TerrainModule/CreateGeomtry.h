#pragma once
class CreateGeomtry
{
public:
	
	CreateGeomtry(void);
	~CreateGeomtry(void);
	static AcDbObjectId Fun_createline();//����ֱ��
	static AcDbObjectId Fun_createPline(AcGePoint2dArray points,double width=0);
	static AcDbObjectId Fun_create3D_Box();
	static AcDbObjectIdArray FunCreateRegion(const AcDbObjectIdArray &curveIds);
	void set_3D_material();//���ò���
	AcDbObjectId CreateMaterial(CString Material_Name, CString File_Name, double uScale, double vScale);//�ڲ�������������ɲ���
};

