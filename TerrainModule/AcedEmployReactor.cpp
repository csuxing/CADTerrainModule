#include "stdafx.h"
#include "AcedEmployReactor.h"
#include "CDynamicload.h"
#include"CTerrainQuadTree.h"
#include<acedCmdNF.h>

extern map<string, AcDbObjectId> MapCurrent;
extern CTerrainQuadTree *QUADTREE;
extern map<string, AcDbObjectId> MapMaterial;
extern std::string EX_CDynamic2CDlgTerrain;

void AcedEmployReactor::commandWillStart(const ACHAR * cmdStr)
{
	//acutPrintf(_T("commandStart"));
}

void AcedEmployReactor::commandEnded(const ACHAR * cmdStr)
{
	//acutPrintf(_T("commandEnd"));
	//acedCommandS(RTSTR, "REGEN", RTNONE);
	
}

void AcedEmployReactor::viewChanged()
{
	//acutPrintf(_T("viewchanged"));
	static double ptleftdownX = 0;
	static bool Flag = false;

		QUADTREE->nextframe.clear();
		//��ȡ��ǰ�ӿڵķ�Χ
		acedDwgPoint PtLeftDown, PtRightUp;
		CDynamicload::GetCurrentWindowsRange_ex(PtLeftDown, PtRightUp);
		if (Flag == false && fabs(ptleftdownX - PtLeftDown[0]) > 50)
		{
			Flag = true;
			ptleftdownX = PtLeftDown[0];//�䵱�źŵ�����
		}
		//���ó������ķ�Χ
		if (Flag == true)
		{
			QUADTREE->minx = PtLeftDown[0];
			QUADTREE->miny = PtLeftDown[1];
			QUADTREE->maxx = PtRightUp[0];
			QUADTREE->maxy = PtRightUp[1];

			double scalewidth = PtRightUp[0] - PtLeftDown[0];

			float flayer = scalewidth / QUADTREE->Root->BlockInfo.width;

			int layer = (1-flayer) * QUADTREE->depth;
			if (layer<0)
			{
				layer=0;
			}
			QUADTREE->TraverseQuadTree(QUADTREE->Root,layer);

			vector<string> NewAdd;//�����ӵĿ�
			vector<string> NewErase;//Ҫ��д�Ŀ�
			vector<string> ReUse;//�ظ�ʹ�õĿ�

			CDynamicload::UpdataMap(QUADTREE->nextframe, MapCurrent, NewAdd, NewErase, ReUse);

			CDynamicload::ModifyCustomPolygonMeshData(NewAdd, NewErase, ReUse, MapCurrent, EX_CDynamic2CDlgTerrain);
			Flag = false;
		}
}
