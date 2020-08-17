#pragma once
//extern AcDbObjectId id;
#include<vector>
#include"StructQuadTreeNode.h"
using namespace std;
class CDynamicload
{
public:
	
	static void GetCurrentWindowsRange_ex(acedDwgPoint &pt1, acedDwgPoint &pt2);
	
	static void UpdataMap(vector<string> &nextframe,map<string,AcDbObjectId> &Map,
		                  vector<string> &NewAdd, vector<string> &NewErase, vector<string> &ReUse);
	//filepath ��λ����Ƭ�߳������ļ���+��\\��
	static void ModifyCustomPolygonMeshData(vector<string> &NewAdd, vector<string> &NewErase,vector<string> &reUse,
		                                    map<string, AcDbObjectId> &Map,string filepath);//filepathΪ��ȡ�ļ���·����+��\\��
	static void CreateMaterialGroup(std::string input, int layer);
	
	static AcDbObjectId creatematerial(string filepath, string groupname, 
		double uScale, double vScale, AcGeVector3d transport = AcGeVector3d(0, 0, 0));
	
	static void arxzoom(AcGePoint3d cenPt, double width, double height, BOOL zflag);
	
	//deprecated
	static void GetCurrentWindowsRange(acedDwgPoint &pt1, acedDwgPoint &pt2);//��ȡ��ǰ�ӿڵ����� ��������
	static bool GetScreenScaleHook(MSG *msg, AcGePoint2d &leftup, AcGePoint2d &rightdown);
	static void ifScreenChange();
	
};

