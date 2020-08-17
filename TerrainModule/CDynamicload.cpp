#include "stdafx.h"
#include "CDynamicload.h"
#include"CustomPolygonmesh.h"
#include"PostToModelSpace.h"
#include<map>
#include"CTerrainQuadTree.h"
#include"StructQuadTreeNode.h"

//全局变量 存储块 遍历
extern map<string, AcDbObjectId> MapCurrent;
extern CTerrainQuadTree *QUADTREE;
extern map<string, AcDbObjectId> MapMaterial;
extern std::string EX_CDynamic2CDlgTerrain;

map<string, AcDbObjectId> MapMaterial;

void CDynamicload::GetCurrentWindowsRange(acedDwgPoint &pt1, acedDwgPoint &pt2)
{
	CRect rc;
	CView* pCurView = acedGetAcadDwgView();
	pCurView->GetClientRect(&rc);
	CPoint ptTopLeft = rc.TopLeft();
	CPoint ptBottomRight = rc.BottomRight();

	CPoint ptBottomLeft,ptTopRight;
	ptBottomLeft.x = ptTopLeft.x;
	ptBottomLeft.y = ptBottomRight.y;
	ptTopRight.x = ptBottomRight.x;
	ptTopRight.y = ptTopLeft.y;
	//acedDwgPoint pt1, pt2;
	acedCoordFromPixelToWorld(ptBottomLeft, pt1);
	acedCoordFromPixelToWorld(ptTopRight, pt2);
}

void CDynamicload::GetCurrentWindowsRange_ex(acedDwgPoint & pt1, acedDwgPoint & pt2)
{
	AcDbViewTableRecord view;
	struct resbuf rb1,rb2,rb3;
	//获取视图中心点坐标
	acedGetVar(L"VIEWCTR", &rb1);

	//获取视图高度
	acedGetVar(L"VIEWSIZE",&rb2);

	acedGetVar(L"SCREENSIZE", &rb3);

	pt1[0] = rb1.resval.rpoint[0] - rb2.resval.rreal/2*rb3.resval.rpoint[0] / rb3.resval.rpoint[1];
	pt2[0] = rb1.resval.rpoint[0] + rb2.resval.rreal/2*rb3.resval.rpoint[0] / rb3.resval.rpoint[1];
	pt1[1] = rb1.resval.rpoint[1] - rb2.resval.rreal/2;
	pt2[1] = rb1.resval.rpoint[1] + rb2.resval.rreal/2;

}
//AcDbObjectId id;
bool CDynamicload::GetScreenScaleHook(MSG * msg, AcGePoint2d & leftup=AcGePoint2d(0,0), AcGePoint2d & rightdown=AcGePoint2d(0,0))
{
	/*if (msg->message== WM_MBUTTONUP)
	{
		
			/ *acedGetAcadDwgView()->SetFocus();
			acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);* /
		static int i;
		AcDbEntity *p = NULL;
		acDocManager->lockDocument(curDoc(), AcAp::kWrite, NULL, NULL, true);
		
		acdbOpenAcDbEntity(p, id, AcDb::kForWrite);
		CCustomPolygonmesh *p1;
		p1 = CCustomPolygonmesh::cast(p);
		p1->assertWriteEnabled();
		//p1->setflag(i);
		p1->close();
		i++;
			//acdbOpenObject(p, id, AcDb::kForWrite);
		acDocManager->unlockDocument(curDoc());
		/ *acDocManager->unlockDocument(acDocManager->curDocument());* /
		SetFocus(0);
		
		AfxMessageBox(L"get message");
	}*/
	//构建场景树
	
	if (msg->wParam==VK_CONTROL)
	{
		//清空数组
		QUADTREE->nextframe.clear();
		//获取当前视口的范围
		acedDwgPoint PtLeftDown, PtRightUp;
		GetCurrentWindowsRange(PtLeftDown, PtRightUp);
		//设置场景树的范围
		QUADTREE->minx = PtLeftDown[0];
		QUADTREE->miny = PtLeftDown[1];
		QUADTREE->maxx = PtRightUp[0];
		QUADTREE->maxy = PtRightUp[1];
		QUADTREE->TraverseQuadTree(QUADTREE->Root);

		vector<string> NewAdd;//新增加的块
		vector<string> NewErase;//要重写的块
		vector<string> ReUse;//重复使用的块


		CDynamicload::UpdataMap(QUADTREE->nextframe, MapCurrent, NewAdd, NewErase, ReUse);
		CDynamicload::ModifyCustomPolygonMeshData(NewAdd, NewErase,ReUse, MapCurrent, "C:\\Users\\xyh19\\Desktop\\example\\NewData\\dem\\");
		
		/*int static count = 0;
		
		string path = "C:\\Users\\xyh19\\Desktop\\CAD二次开发\\triDTM\\Release\\new\\";
		vector<string> ascPathsCurrent;//当前帧绘制内容
		vector<string> ascPathNextFrame;//下一帧绘制内容
		vector<string> ascPathsLastFrame;//上一帧绘制内容
		vector<AcDbObjectId> ids;//存放实体的id
		map<string, AcDbObjectId> MapBlockToID;//块名与实体块的ID实现one to one
		//利用函数获取当前的视口范围

		//返回当前应该在屏幕上绘制的小块

		//获取小块文件名称，加入current中
		for (int i=0;i<8;i++)
		{
			for (int j=0;j<8;j++)
			{
				string tmppath= path + "L3_" + to_string(i) + "_" + to_string(j);
				ascPathsCurrent.push_back(tmppath);
			}
		}
		    //第二次进入改变文件
		if (count!=0)
		{
			ascPathsCurrent.clear();
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					string tmppath = path + "L2_" + to_string(i) + "_" + to_string(j);
					ascPathsCurrent.push_back(tmppath);
				}
			}
		}
		//根据nextFrame中的块与map中的数据进行比较
		//以写的方式打开，修改自定义实体数据
		acDocManager->lockDocument(curDoc(), AcAp::kWrite, NULL, NULL, true);

		//////////////////////////////////////////////////////////////////////////
		
		

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				acDocManager->lockDocument(curDoc(), AcAp::kWrite, NULL, NULL, true);//此处更新需要加锁
				
				Acad::ErrorStatus e;
				string filepath = path + "L3_" + to_string(i) + "_" + to_string(j);
				CCustomPolygonmesh *pm = new CCustomPolygonmesh();
				CReadSrcFile::ReadASCIIHeadfile(filepath, pm->headfile);
				pm->vecpts.clear();
				CReadSrcFile::ReadASCII(pm->vecpts, filepath);
				AcGeMatrix3d m;
				AcGeVector3d a(pm->headfile.xllcorner, pm->headfile.yllcorner, 0);
				m.setToTranslation(a);
				AcDbObjectId materialid;
				string image;
				image = "image_" + to_string(i) + "_" + to_string(j);
				materialid = pm->createMaterial(image.c_str(), L"C:\\Users\\xyh19\\Desktop\\CAD二次开发\\Texture\\x64\\Debug\\金沙江.jpg", (double)1 / (pm->headfile.ncols*pm->headfile.cellsize), (double)1 / (pm->headfile.nrows*pm->headfile.cellsize));
				pm->setMaterial(materialid);
				//pm->transformBy(m);
				AcDbObjectId id = CPostToModelSpace::PostToModelSpace(pm);

				acDocManager->unlockDocument(curDoc());
			}
		}*/
	}
	return TRUE;
}

void CDynamicload::UpdataMap(vector<string>& nextframe, map<string, AcDbObjectId>& Map, vector<string> &NewAdd, vector<string> &NewErase, vector<string> &ReUse)
{
	
	//check out new block
	/*vector<string> NewAdd;//新增加的块
	vector<string> NewErase;//要重写的块
	vector<string> ReUse;//重复使用的块*/
	for (int i=0;i<nextframe.size();i++)
	{
		map<string, AcDbObjectId>::iterator iter;
		iter = Map.find(nextframe[i]);
		if (iter==Map.end())
		{
			//没有找到相应的key
			NewAdd.push_back(nextframe[i]);
		}
		else
		{
			//重复使用的块
			ReUse.push_back(nextframe[i]);
		}
	}
	map<string, AcDbObjectId>::iterator itera;
	for (itera=Map.begin();itera!=Map.end();itera++)
	{
		for (int i=0;i<nextframe.size();i++)
		{
			if (itera->first == nextframe[i])
				break;
			if (i == nextframe.size() - 1)
				NewErase.push_back(itera->first);
		}
	}
	
}

void CDynamicload::ModifyCustomPolygonMeshData(vector<string>& NewAdd, vector<string>& NewErase,vector<string>&ReUse, map<string, AcDbObjectId>& Map,string filepath)
{

	acDocManager->lockDocument(curDoc(), AcAp::kWrite, NULL, NULL, true);
	int newCustomNum = NewAdd.size() - NewErase.size();
	map<string, AcDbObjectId>::iterator iter;
	//只需要改写
	if (newCustomNum<=0)
	{
		for (int i = 0; i < NewErase.size(); i++)
		{
			//数据改写部分
			if(i<NewAdd.size())
			{
				iter = Map.find(NewErase[i]);
				if (iter == Map.end())
				{
					AfxMessageBox(L"error");
					abort();
				}
				AcDbEntity *pent = NULL;
				if (Acad::eOk == acdbOpenObject(pent, iter->second, AcDb::kForWrite))
				{
					CCustomPolygonmesh *pc;
					pc = CCustomPolygonmesh::cast(pent);
					pc->assertWriteEnabled();
					pc->InitCustomShell(filepath, NewAdd[i]);
					/*CReadSrcFile::ReadASCIIHeadfile(filepath + NewAdd[i], pc->headfile);
					pc->vecpts.clear();
					CReadSrcFile::ReadASCII(pc->vecpts, filepath + NewAdd[i]);*/
					pc->setflag(1);
					//////////////////////////////////////////////////////////////////////////获取块的实际位置
					/*string MaterialName;
					MaterialName = "C:\\Users\\xyh19\\Desktop\\example\\NewData\\dem\\" + QUADTREE->programName + NewAdd[i] + ".jpg";
					int row, col,layer;
					CTerrainQuadTree::GetRowsandColsInString(NewAdd[i], row, col, layer);
					float Ixcol, Iycol,Iwidth,Iheight;
					Iwidth = QUADTREE->BasicInfo.width / pow(2, layer);
					Iheight = QUADTREE->BasicInfo.height / pow(2, layer);
					Ixcol = QUADTREE->BasicInfo.minx + Iwidth*col;
					Iycol= QUADTREE->BasicInfo.miny + Iheight*(pow(2,layer)-1-row);
					float dx = Ixcol -pc->headfile.xllcorner ;
					float dy = Iycol -pc->headfile.yllcorner ;
					AcDbObjectId mid=pc->createMaterial(NewAdd[i].c_str(), MaterialName.c_str(), 1/((pc->headfile.ncols-1)*(pc->headfile.cellsize)),1/((pc->headfile.nrows-1)*pc->headfile.cellsize)/ *1 / Iwidth, 1 / Iheight,AcGeVector3d(dx/Iwidth,dy/Iheight,0)* /);
					pc->setMaterial(mid);*/
					auto it = MapMaterial.find(NewAdd[i]);
					if (it == MapMaterial.end() || it->second==NULL)
					{
						AfxMessageBox(L"error");
					}
					//pc->setMaterial(it->second);
					if (pc->setMaterial(it->second)!=Acad::eOk)
					{
						AfxMessageBox(L"error");
					}
					//ErrorStatus er = pc->setMaterial(it->second);
                    //////////////////////////////////////////////////////////////////////////
					pc->close();
					AcDbObjectId storeID;
					storeID = iter->second;
					Map.erase(iter);
					Map.insert(pair<string, AcDbObjectId>(NewAdd[i], storeID));
				}
			}
			//多余的块做标记，在subworlddraw中不绘制
			else
			{
				iter = Map.find(NewErase[i]);
				if (iter == Map.end())
				{
					AfxMessageBox(L"CDynamiclod Modify error");
				}
				AcDbEntity *pent = NULL;
				if (Acad::eOk == acdbOpenObject(pent, iter->second, AcDb::kForWrite))
				{
					CCustomPolygonmesh *pc;
					pc = CCustomPolygonmesh::cast(pent);
					pc->assertWriteEnabled();
					pc->setflag(0);
					pc->close();
				}
			}
			

		}
	}
	//需要增加新实体
	else
	{
		//改写部分
		for (int i=0;i<NewErase.size();i++)
		{
			iter = Map.find(NewErase[i]);
			if (iter == Map.end())
			{
				AfxMessageBox(L"error");
			}
			AcDbEntity *pent = NULL;
			if (Acad::eOk == acdbOpenObject(pent, iter->second, AcDb::kForWrite))
			{
				CCustomPolygonmesh *pc;
				pc = CCustomPolygonmesh::cast(pent);
				pc->assertWriteEnabled();
				pc->InitCustomShell(filepath, NewAdd[i]);
				/*CReadSrcFile::ReadASCIIHeadfile(filepath + NewAdd[i], pc->headfile);
				pc->vecpts.clear();
				CReadSrcFile::ReadASCII(pc->vecpts, filepath + NewAdd[i]);*/
				pc->setflag(1);
				//////////////////////////////////////////////////////////////////////////获取块的实际位置
				/*string MaterialName;
				MaterialName = "C:\\Users\\xyh19\\Desktop\\example\\NewData\\dem\\" + QUADTREE->programName + NewAdd[i] + ".jpg";
				int row, col, layer;
				CTerrainQuadTree::GetRowsandColsInString(NewAdd[i], row, col, layer);
				float Ixcol, Iycol, Iwidth, Iheight;
				Iwidth = QUADTREE->BasicInfo.width / pow(2, layer);
				Iheight = QUADTREE->BasicInfo.height / pow(2, layer);
				Ixcol = QUADTREE->BasicInfo.minx + Iwidth * col;
				Iycol = QUADTREE->BasicInfo.miny + Iheight * (pow(2, layer) - 1 - row);
				float dx = Ixcol - pc->headfile.xllcorner;
				float dy = Iycol - pc->headfile.yllcorner;
				AcDbObjectId mid = pc->createMaterial(NewAdd[i].c_str(), MaterialName.c_str(), 1/((pc->headfile.ncols - 1)*(pc->headfile.cellsize)), 1/((pc->headfile.nrows - 1)*pc->headfile.cellsize));
				pc->setMaterial(mid);*/
				auto it = MapMaterial.find(NewAdd[i]);
				if (it == MapMaterial.end() || it->second == NULL)
				{
					AfxMessageBox(L"CDynamiclod Modify error");
				}
				if (pc->setMaterial(it->second) != Acad::eOk)
				{
					AfxMessageBox(L"CDynamiclod Modify error");
				}
				//pc->setMaterial(it->second);
				//////////////////////////////////////////////////////////////////////////
				pc->close();
				AcDbObjectId storeID;
				storeID = iter->second;
				Map.erase(iter);
				Map.insert(pair<string, AcDbObjectId>(NewAdd[i], storeID));
			}
		}
		//新增部分
		for (int i=0;i<newCustomNum;i++)
		{
			CCustomPolygonmesh *pn=new CCustomPolygonmesh;
			pn->assertWriteEnabled();
			pn->InitCustomShell(filepath, NewAdd[NewErase.size() + i]);
			pn->setflag(1);

			/*CReadSrcFile::ReadASCIIHeadfile(filepath + NewAdd[NewErase.size()+i], pn->headfile);
			CReadSrcFile::ReadASCII(pn->vecpts, filepath + NewAdd[NewErase.size()+i]);*/
			//////////////////////////////////////////////////////////////////////////获取块的实际位置
			/*string MaterialName;
			MaterialName = "C:\\Users\\xyh19\\Desktop\\example\\NewData\\dem\\" + QUADTREE->programName + NewAdd[NewErase.size() + i] + ".jpg";
			int row, col, layer;
			CTerrainQuadTree::GetRowsandColsInString(NewAdd[NewErase.size() + i], row, col, layer);
			float Ixcol, Iycol, Iwidth, Iheight;
			Iwidth = QUADTREE->BasicInfo.width / pow(2, layer);
			Iheight = QUADTREE->BasicInfo.height / pow(2, layer);
			Ixcol = QUADTREE->BasicInfo.minx + Iwidth * col;
			Iycol = QUADTREE->BasicInfo.miny + Iheight * (pow(2, layer) - 1 - row);
			float dx = Ixcol - pn->headfile.xllcorner;
			float dy = Iycol - pn->headfile.yllcorner;
			AcDbObjectId mid = pn->createMaterial(NewAdd[NewErase.size() + i].c_str(), MaterialName.c_str(), 1/((pn->headfile.ncols - 1)*(pn->headfile.cellsize)), 1/((pn->headfile.nrows - 1)*pn->headfile.cellsize));
			pn->setMaterial(mid);*/
			auto it = MapMaterial.find(NewAdd[NewErase.size() + i]);
			if (it == MapMaterial.end() || it->second == NULL)
			{
				AfxMessageBox(L"CDynamiclod Modify error");
			}
			//pn->setMaterial(it->second);
			if (pn->setMaterial(it->second) != Acad::eOk)
			{
				AfxMessageBox(L"CDynamiclod Modify error");
			}
			//////////////////////////////////////////////////////////////////////////
			AcDbObjectId id=CPostToModelSpace::PostToModelSpace(pn);
			Map.insert(pair<string, AcDbObjectId>(NewAdd[NewErase.size() + i], id));
		}
	}
	//重复使用的块 做标记
	for (int i=0;i<ReUse.size();i++)
	{
		iter=MapCurrent.find(ReUse[i]);
		AcDbEntity *pent = NULL;
		if (Acad::eOk == acdbOpenObject(pent, iter->second, AcDb::kForWrite))
		{
			CCustomPolygonmesh *pc;
			pc = CCustomPolygonmesh::cast(pent);
			pc->assertWriteEnabled();
			auto  it = MapMaterial.find(ReUse[i]);
			if (it == MapMaterial.end() || it->second == NULL)
			{
				AfxMessageBox(L"error");
			}
			//pc->setMaterial(it->second);
			if (pc->setMaterial(it->second) != Acad::eOk)
			{
				AfxMessageBox(L"CDynamiclod Modify error");
			}
			pc->setflag(1);
			pc->close();
		}
	}
	acDocManager->unlockDocument(curDoc());
}

void CDynamicload::arxzoom(AcGePoint3d cenPt, double width, double height, BOOL zflag)
{
	AcDbViewTableRecord view;
	struct resbuf rb;
	struct resbuf wcs, dcs, ccs;         // ads_trans coord system flags
	ads_point vpDir;
	ads_point wmin, wmax;             // min and max corners of the zoom window
	ads_point wdcsmax, wdcsmin;   // windows corners in device coords
	AcGeVector3d viewDir;

	ads_real    lenslength,
		viewtwist,
		frontz,
		backz;

	ads_point target;

	int viewmode,
		tilemode,
		cvport;

	// Get window to zoom to:
	wmax[X] = cenPt.x + width / 2.0;
	wmin[X] = cenPt.x - width / 2.0;
	wmax[Y] = cenPt.y + height / 2.0;
	wmin[Y] = cenPt.y - height / 2.0;


	wmax[Z] = 0.0;
	wmin[Z] = 0.0;

	wcs.restype = RTSHORT;         // WORLD coord system flag
	wcs.resval.rint = 0;
	ccs.restype = RTSHORT;         // CURRENT coord system flag
	ccs.resval.rint = 1;
	dcs.restype = RTSHORT;         // DEVICE coord system flag
	dcs.resval.rint = 2;

	// Get the 'VPOINT' direction vector
	ads_getvar(L"VIEWDIR", &rb);
	ads_trans(rb.resval.rpoint, &ccs, &wcs, 0, vpDir);
	viewDir.set(vpDir[X], vpDir[Y], vpDir[Z]);


	if (!zflag)
	{
		double len = (width > height) ? height : width;
		wmax[X] += 0.025*len;
		wmax[Y] += 0.025*len;
		wmin[X] -= 0.025*len;
		wmin[Y] -= 0.025*len;
	}
	// convert upper right window corner to DCS
	ads_trans(wmax, &ccs, &dcs, 0, wdcsmax);

	// convert lower left window corner to DCS
	ads_trans(wmin, &ccs, &dcs, 0, wdcsmin);
	// calculate and set view center point
	AcGePoint2d cenPt1(cenPt.x, cenPt.y);
	view.setCenterPoint(cenPt1);

	// set view height and width and direction
	view.setHeight(fabs(wdcsmax[Y] - wdcsmin[Y]));
	view.setWidth(fabs(wdcsmax[X] - wdcsmin[X]));
	view.setViewDirection(viewDir);

	// get and set other properties
	ads_getvar(L"LENSLENGTH", &rb);
	lenslength = rb.resval.rreal;
	view.setLensLength(lenslength);

	ads_getvar(L"VIEWTWIST", &rb);
	viewtwist = rb.resval.rreal;
	view.setViewTwist(viewtwist);

	ads_getvar(L"FRONTZ", &rb);
	frontz = rb.resval.rreal;

	ads_getvar(L"BACKZ", &rb);
	backz = rb.resval.rreal;

	ads_getvar(L"VIEWMODE", &rb);
	viewmode = rb.resval.rint;

	view.setPerspectiveEnabled(viewmode & 1);
	view.setFrontClipEnabled(viewmode & 2);
	view.setBackClipEnabled(viewmode & 4);
	view.setFrontClipAtEye(!(viewmode & 16));

	ads_getvar(L"TILEMODE", &rb);
	tilemode = rb.resval.rint;
	ads_getvar(L"CVPORT", &rb);
	cvport = rb.resval.rint;

	// Paperspace flag
	Adesk::Boolean paperspace =
		((tilemode == 0) && (cvport == 1)) ? Adesk::kTrue : Adesk::kFalse;
	view.setIsPaperspaceView(paperspace);

	if (Adesk::kFalse == paperspace)
	{
		view.setFrontClipDistance(frontz);
		view.setBackClipDistance(backz);
	}
	else
	{
		view.setFrontClipDistance(0.0);
		view.setBackClipDistance(0.0);
	}

	ads_getvar(L"TARGET", &rb);
	ads_trans(rb.resval.rpoint, &ccs, &wcs, 0, target);
	view.setTarget(AcGePoint3d(target[X], target[Y], target[Z]));

	// update view
#if (ADS>14)
	acedSetCurrentView(&view, NULL);
#else
	//acdbSetCurrentView(&view, NULL);
	acedSetCurrentView(&view, NULL);
#endif
}

void CDynamicload::ifScreenChange()
{
	double ptleftdownX=0;
	static bool Flag=false;
	while (1)
	{
		QUADTREE->nextframe.clear();
		//获取当前视口的范围
		acedDwgPoint PtLeftDown, PtRightUp;
		GetCurrentWindowsRange_ex(PtLeftDown, PtRightUp);
		if (Flag==false&&fabs(ptleftdownX-PtLeftDown[0])>50)
		{
			Flag = true;
			ptleftdownX = PtLeftDown[0];//充当信号的作用
		}
		//设置场景树的范围
		if (Flag==true)
		{
			QUADTREE->minx = PtLeftDown[0];
			QUADTREE->miny = PtLeftDown[1];
			QUADTREE->maxx = PtRightUp[0];
			QUADTREE->maxy = PtRightUp[1];


			QUADTREE->TraverseQuadTree(QUADTREE->Root);

			vector<string> NewAdd;//新增加的块
			vector<string> NewErase;//要重写的块
			vector<string> ReUse;//重复使用的块

			CDynamicload::UpdataMap(QUADTREE->nextframe, MapCurrent, NewAdd, NewErase, ReUse);

			CDynamicload::ModifyCustomPolygonMeshData(NewAdd, NewErase,ReUse, MapCurrent, EX_CDynamic2CDlgTerrain);
			Flag = false;
		}	
		//Sleep(1000);
	}
}

void CDynamicload::CreateMaterialGroup(std::string input,int layer)
{
	for (int i=0;i<layer;i++)
	{
		for (int j=0;j<pow(2,i);j++)
		{
			for (int k=0;k<pow(2,i);k++)
			{
				HeadFile headfile;
				string str ="L" + to_string(i) + "_" + to_string(j) + "_" + to_string(k);
				string filepath = input+"img\\"+str+".tif";
				CReadSrcFile::ReadASCIIHeadfile(input+"dem\\"+str, headfile);
				AcDbObjectId id=creatematerial(filepath, str, 1 / ((headfile.ncols - 1)*headfile.cellsize), 1 / ((headfile.nrows - 1)*headfile.cellsize));
				MapMaterial.insert(pair<string, AcDbObjectId>(str,id));
			}
		}
	}
}

AcDbObjectId CDynamicload::creatematerial(string filepath,string groupname, double uScale, double vScale, AcGeVector3d transport)
{
	AcDbDictionary* pMaterialDict;
	Acad::ErrorStatus es;
	es = acdbHostApplicationServices()->workingDatabase()->getMaterialDictionary(pMaterialDict, AcDb::kForWrite);
	AcDbObjectId materialId = NULL;
	ACHAR Name_ACHAR[256], File_Name_ACHAR[256];
	_tcscpy(Name_ACHAR, (CString)groupname.c_str());
	_tcscpy(File_Name_ACHAR, (CString)filepath.c_str());
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
		
		AcGeVector3d av1, av2, av3;
		AcGePoint3d ap1;
		mx.getCoordSystem(ap1, av1, av2, av3);//获取当前的坐标原点，x,y,z轴


		mx(0, 3) = transport.x;
		mx(1, 3) = transport.y;
		mx(2, 3) = transport.z;

		AcGiMapper mapper;//映射函数 贴图位置、大小、旋转方式
		mapper.setProjection(AcGiMapper::Projection::kBox);//设置投影 平板模式
		//mapper.setUTiling(AcGiMapper::Tiling::kTile);//水平平铺
		mapper.setUTiling(AcGiMapper::kInheritTiling);

		//mapper.setVTiling(AcGiMapper::Tiling::kTile);//竖直平铺
		mapper.setVTiling(AcGiMapper::kInheritTiling);
		//mapper.setAutoTransform(AcGiMapper::AutoTransform::kNone);//自动旋转
		mapper.setTransform(mx);


		AcGiMaterialMap map;//材质组件映射
		//map.setSourceFileName(File_Name_ACHAR);//此方法被弃用了，在tex中设置
		map.setTexture(&tex);
		map.setBlendFactor(1.0);//使贴图完全不透明
		map.setMapper(mapper);//对贴图进行设置、包括大小、旋转方式

		AcDbMaterial* pMaterialObj = new AcDbMaterial();//设置材质名称
		pMaterialObj->setName(Name_ACHAR);
		pMaterialObj->setReflectanceScale(0.0);
		pMaterialObj->setShininess(0.0);

		pMaterialObj->setTwoSided(Adesk::kTrue);

		es = pMaterialDict->setAt(Name_ACHAR, pMaterialObj, materialId);

		AcGiMaterialColor diffuseColor;//定义材料组件的颜色
		pMaterialObj->setDiffuse(diffuseColor, map);
		pMaterialObj->setMode(AcGiMaterialTraits::kRealistic);

		pMaterialObj->close();
		pMaterialDict->close();
	}
	return materialId;

}

