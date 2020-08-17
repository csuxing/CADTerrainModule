// DlgTerrain.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgTerrain.h"
#include "afxdialogex.h"

#include "CReadSrcFile.h"
#include<string>
#include "StringConvert.h"
#include "CDynamicload.h"
#include<fstream>
#include<io.h>
#include "CTerrainQuadTree.h"
#include"CustomPolygonmesh.h"
#include"PostToModelSpace.h"

extern map<string, AcDbObjectId> MapMaterial;
extern map<string, AcDbObjectId> MapCurrent;
extern CTerrainQuadTree *QUADTREE;
extern std::string EX_CDynamic2CDlgTerrain;

std::string EX_CDynamic2CDlgTerrain;
CTerrainQuadTree *QUADTREE = new CTerrainQuadTree;//定义
map<string, AcDbObjectId> MapCurrent;

// CDlgTerrain 对话框

IMPLEMENT_DYNAMIC(CDlgTerrain, CDialogEx)

CDlgTerrain::CDlgTerrain(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTerrain::IDD, pParent)
	, m_edit_pic(_T(""))
	, m_edit_height(_T(""))
	, m_edit_layer(1)
{

}

CDlgTerrain::~CDlgTerrain()
{
}

void CDlgTerrain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT_PIC,m_edit_pic);
	DDX_Text(pDX,IDC_EDIT_HEIGHT,m_edit_height);
	DDX_Text(pDX, IDC_EDIT_layer, m_edit_layer);
	DDV_MinMaxInt(pDX, m_edit_layer, 1, 10);
	
}


BEGIN_MESSAGE_MAP(CDlgTerrain, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_PIC, &CDlgTerrain::OnBnClickedButtonPic)
	ON_BN_CLICKED(IDC_BUTTON_HEIGHT, &CDlgTerrain::OnBnClickedButtonHeight)
	ON_BN_CLICKED(IDC_BUTTON_CreatePyramid, &CDlgTerrain::OnBnClickedButtonCreatepyramid)
	ON_BN_CLICKED(IDC_BUTTON_CR2DTerrain, &CDlgTerrain::OnBnClickedButtonCr2dterrain)
	ON_BN_CLICKED(IDC_BUTTON_CR3DTerrain, &CDlgTerrain::OnBnClickedButtonCr3dterrain)
END_MESSAGE_MAP()


// CDlgTerrain 消息处理程序


void CDlgTerrain::OnBnClickedButtonPic()
{
	// TODO: 在此添加控件通知处理程序代码
	m_edit_layer=0;
	LPCTSTR strFilter = L"TIFF图像(*.tif)|*.tif;*.tiff|JPG文件(*.jpg)|*.jpg||";
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFilter,NULL,0);
	if (dlg.DoModal())
	{
		m_edit_pic=dlg.GetPathName();
	}
	//获取文件大小，给出推荐的层数
	
	std::string filepath;
	filepath=ConvertString::wchar2char(m_edit_pic);
	int size;

	/*HANDLE handle = CreateFileA( filepath.c_str(), FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 );
	if ( handle != INVALID_HANDLE_VALUE )
	{
		size = GetFileSize( handle, NULL );
		CloseHandle( handle );
	}*/

	struct _stat info;
	_stat(filepath.c_str(), &info);
	size = info.st_size;

	while (size>3*1024*1024)
	{
		m_edit_layer++;
		size=size/4;
	}

	if (m_edit_layer==0)
	{
		m_edit_layer=1;
	}
	UpdateData(FALSE);
}


void CDlgTerrain::OnBnClickedButtonHeight()
{
	// TODO: 在此添加控件通知处理程序代码
	LPCTSTR strFilter = L"TIFF图像(*.tif)|*.tif;*.tiff|ASC文件(*.asc)|*.asc||";
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFilter,NULL,0);
	if (dlg.DoModal())
	{
		m_edit_height=dlg.GetPathName();
	}
	UpdateData(FALSE);
}


void CDlgTerrain::OnBnClickedButtonCreatepyramid()
{
	// TODO: 在此添加控件通知处理程序代码
	/*std::vector<double> vec;
	CReadSrcFile::ProduceVirtualDem("C:\\Users\\xyh19\\Desktop\\CAD二次开发\\OSGTerrain\\height.tif");
	CReadSrcFile::ReadASCII(vec,"C:\\Users\\xyh19\\Desktop\\CAD二次开发\\OSGTerrain\\height_VirtualDEM");*/
	
	UpdateData(TRUE);

	HMODULE handle=GetModuleHandle(L"AdskTerrainModule.arx");
	wchar_t strModule[256];
	GetModuleFileName(handle,strModule,256);
	std::string path=ConvertString::wchar2char(strModule);
	int position_=path.find_last_of("\\");
	path=path.substr(0,position_);
	path=path+"\\filepathname.txt";

	ofstream fout(path);
	std::string path_pic=ConvertString::wchar2char(m_edit_pic);
	std::string path_height=ConvertString::wchar2char(m_edit_height);
	if (fout!=NULL)
	{
		fout<<path_pic<<"\n";
		fout<<path_height<<"\n";
		fout<<m_edit_layer<<"\n";
		fout.close();
	}

	if(m_edit_pic!=L"")
	{
		std::string path_pic=ConvertString::wchar2char(m_edit_pic);
		CReadSrcFile::createImagePyramid(path_pic,m_edit_layer);
	}

	if(m_edit_height!=L"")
	{
		std::string path_height=ConvertString::wchar2char(m_edit_height);
		CReadSrcFile::CreatePyramid(m_edit_layer,path_height,"dem");
	}

	//create material lib
}



void CDlgTerrain::OnBnClickedButtonCr2dterrain()
{
	// TODO: 在此添加控件通知处理程序代码

	std::string filepath_pic;
	std::string filepath_height;
	std::string ori_filepath;
	int layer;
	CReadSrcFile::ReadPicAndHeiPath(filepath_pic, filepath_height, layer);

	ori_filepath=filepath_pic;

	int file_position=filepath_pic.find_last_of("\\");
	filepath_pic=filepath_pic.substr(0,file_position);
	filepath_pic=filepath_pic+"\\";

	acDocManager->lockDocument(curDoc(), AcAp::kWrite, NULL, NULL, true);
	if (MapMaterial.empty())
	{
		CDynamicload::CreateMaterialGroup(filepath_pic,layer);
	}
	

	std::string output,outputdir,filename;
	CReadSrcFile::ProduceVirtualDem(ori_filepath,output);//create virtual dem

	int o_position=output.find_last_of("\\");
	int o_position_point=output.find_last_of(".");
	std::string programName=output.substr(o_position+1,o_position_point-o_position-1);
	outputdir=output.substr(0,o_position+1)+programName+"_dem";

	EX_CDynamic2CDlgTerrain = outputdir+"\\";
	filename=outputdir+"\\"+programName;

	if(0!=_access(outputdir.c_str(),0))
	{
		_mkdir(outputdir.c_str());
		CReadSrcFile::CreatePyramid(layer,output,programName+"_dem");
	}
	
	HeadFile headfile;
	CReadSrcFile::ReadASCIIHeadfile(filename+"0", headfile);

	CCustomPolygonmesh *p = new CCustomPolygonmesh;

	p->InitCustomShell("D:\\备份\\TerrainModule\\Data\\dem\\", "高程0");

	p->setMaterial(MapMaterial["L0_0_0"]);
	AcDbObjectId id=CPostToModelSpace::PostToModelSpace(p);

	MapCurrent.insert(pair<string, AcDbObjectId>("L0_0_0", id));

	BlockInformation block;
	block.minx = headfile.xllcorner;
	block.miny = headfile.yllcorner;
	block.height = headfile.cellsize*(headfile.nrows - 1);
	block.width = headfile.cellsize*(headfile.ncols - 1);
	block.maxx = block.minx + block.width;
	block.maxy = block.miny + block.height;
	QUADTREE->setdepth(layer);
	QUADTREE->CreateQuadTree(layer, &QUADTREE->Root, "L0_0_0", block);
	//QUADTREE->DeleteTerrainTree(QUADTREE->Root);
	QUADTREE->InitBasicBlockInfo(filename+to_string(layer-1));//初始化

	AcGePoint2d centerPoint;
	centerPoint.set((block.minx + block.maxx) / 2, (block.miny + block.maxy)/2);
	AcDbViewTableRecord *view = new AcDbViewTableRecord();
	view->setCenterPoint(centerPoint);
	view->setHeight(block.height);
	view->setWidth(block.width);
	acedSetCurrentView(view, NULL);
	view->close();
	delete view;
	view = NULL;
	
	AfxMessageBox(L"创建成功");

	acDocManager->unlockDocument(curDoc());
}


void CDlgTerrain::OnBnClickedButtonCr3dterrain()
{
	// TODO: 在此添加控件通知处理程序代码
	std::string filepath_pic;
	std::string filepath_height;
	std::string ori_filepath;
	int layer;
	CReadSrcFile::ReadPicAndHeiPath(filepath_pic, filepath_height, layer);

	ori_filepath = filepath_pic;

	int file_position = filepath_height.find_last_of("\\");
	int file_position_point = filepath_height.find_last_of(".");
	std::string programName = filepath_height.substr(file_position + 1, file_position_point - file_position - 1);

	filepath_height = filepath_height.substr(0, file_position);
	filepath_height = filepath_height + "\\";

	acDocManager->lockDocument(curDoc(), AcAp::kWrite, NULL, NULL, true);
	if (MapMaterial.empty())
	{
		file_position = filepath_pic.find_last_of("\\");
		filepath_pic = filepath_pic.substr(0, file_position + 1);
		CDynamicload::CreateMaterialGroup(filepath_pic, layer);
	}

	EX_CDynamic2CDlgTerrain = filepath_height + "dem\\";

	filepath_height = filepath_height + "dem\\"+programName;


	HeadFile headfile;
	CReadSrcFile::ReadASCIIHeadfile(filepath_height + "0", headfile);
	CCustomPolygonmesh *p = new CCustomPolygonmesh;
	p->InitCustomShell(EX_CDynamic2CDlgTerrain, "L0_0_0");

	p->setMaterial(MapMaterial["L0_0_0"]);
	AcDbObjectId id = CPostToModelSpace::PostToModelSpace(p);

	MapCurrent.insert(pair<string, AcDbObjectId>("L0_0_0", id));


	BlockInformation block;
	block.minx = headfile.xllcorner;
	block.miny = headfile.yllcorner;
	block.height = headfile.cellsize*(headfile.nrows - 1);
	block.width = headfile.cellsize*(headfile.ncols - 1);
	block.maxx = block.minx + block.width;
	block.maxy = block.miny + block.height;
	QUADTREE->setdepth(layer);
	QUADTREE->CreateQuadTree(layer, &QUADTREE->Root, "L0_0_0", block);
	QUADTREE->InitBasicBlockInfo(filepath_height + to_string(layer - 1));//初始化

	AcGePoint2d centerPoint;
	centerPoint.set((block.minx + block.maxx) / 2, (block.miny + block.maxy) / 2);
	AcDbViewTableRecord *view = new AcDbViewTableRecord();
	view->setCenterPoint(centerPoint);
	view->setHeight(block.height);
	view->setWidth(block.width);
	acedSetCurrentView(view, NULL);
	view->close();
	delete view;
	view = NULL;

	AfxMessageBox(L"创建成功");

	acDocManager->unlockDocument(curDoc());
}
