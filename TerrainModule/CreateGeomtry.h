#pragma once
class CreateGeomtry
{
public:
	
	CreateGeomtry(void);
	~CreateGeomtry(void);
	static AcDbObjectId Fun_createline();//创建直线
	static AcDbObjectId Fun_createPline(AcGePoint2dArray points,double width=0);
	static AcDbObjectId Fun_create3D_Box();
	static AcDbObjectIdArray FunCreateRegion(const AcDbObjectIdArray &curveIds);
	void set_3D_material();//设置材质
	AcDbObjectId CreateMaterial(CString Material_Name, CString File_Name, double uScale, double vScale);//在材质浏览器中生成材质
};

