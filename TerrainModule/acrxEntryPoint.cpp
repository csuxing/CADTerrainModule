// (C) Copyright 2002-2012 by Autodesk, Inc. 
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
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#define ACED_EDITOR_OBJ "AcEditor"
#define acedEditor AcEditor::cast(acrxSysRegistry()->at(ACED_EDITOR_OBJ))


#include "StdAfx.h"
#include "resource.h"

#include "CreateGeomtry.h"
#include "memory.h"
#include "CReadSrcFile.h"
#include"PostToModelSpace.h"
#include"CDynamicload.h"
#include"CustomPolygonmesh.h"
#include<fstream>
#include<iomanip>
#include"dbmain.h"
#include"CTerrainQuadTree.h"

#include <acadi.h>
#include<AcCamera.h>//������ͷ�ļ�
#include<acedCmdNF.h>

#include<thread>

#include "AcedEmployReactor.h"

#include "DlgTerrain.h"
#include "BoostGraphic.h"//�ڶ�

//-----------------------------------------------------------------------------
#define szRDS _RXST("Adsk")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CTerrainModuleApp : public AcRxArxApp {

public:
	CTerrainModuleApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		CCustomPolygonmesh::rxInit();
		// TODO: Add your initialization code here

		AcedEmployReactor *preactor = new AcedEmployReactor;
		acedEditor->addReactor(preactor);

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
	
	// The ACED_ARXCOMMAND_ENTRY_AUTO macro can be applied to any static member 
	// function of the CTerrainModuleApp class.
	// The function should take no arguments and return nothing.
	//
	// NOTE: ACED_ARXCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid and
	// have arguments to define context and command mechanism.
	
	// ACED_ARXCOMMAND_ENTRY_AUTO(classname, group, globCmd, locCmd, cmdFlags, UIContext)
	// ACED_ARXCOMMAND_ENTRYBYID_AUTO(classname, group, globCmd, locCmdId, cmdFlags, UIContext)
	// only differs that it creates a localized name using a string in the resource file
	//   locCmdId - resource ID for localized command

	// Modal Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CTerrainModuleApp, AdskMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void AdskMyGroupMyCommand () {
		// Put your command code here
		CDlgTerrain *pt=new CDlgTerrain;
		pt->Create(IDD_DIALOG_Terrain,NULL);
		pt->ShowWindow(SW_SHOW);
	}

	static void AdskMyGroupCTerrain() {
		// Put your command code here
		//CustomBoost::BoostGraphic::DigHole("D:\\����\\TerrainModule\\Data\\�߳�.tif", "D:\\����\\TerrainModule\\Data\\hole2.txt", 3);
		CDlgTerrain *pt = new CDlgTerrain;
		pt->Create(IDD_DIALOG_Terrain, NULL);
		pt->ShowWindow(SW_SHOW);
		
		/*CCustomPolygonmesh *p = new CCustomPolygonmesh;
		std::vector<vector<AcGePoint3d>> hole_pts;
		HeadFile headfile;
		std::vector<float> vecpts;
		std::vector<long> index;
		unsigned long holepoint_num = 0;
		CustomBoost::BoostGraphic::readHoleFileForPaint("D:\\����\\TerrainModule\\Data\\modify\\L0_0_0", hole_pts);
		CReadSrcFile::ReadASCIIHeadfile("D:\\����\\TerrainModule\\Data\\dem\\L0_0_0", headfile);
		CReadSrcFile::ReadASCII(vecpts, "D:\\����\\TerrainModule\\Data\\dem\\L0_0_0");
		CReadSrcFile::ReadASCIIindex("D:\\����\\TerrainModule\\Data\\modify\\L0_0_0_index", index);
		
		for (int i = 0; i < hole_pts.size(); i++)
		{
			holepoint_num = holepoint_num + hole_pts[i].size();
		}
		p->InitDataForPaint(headfile,vecpts,hole_pts,index,holepoint_num);

		p->setflag(1);
		AcDbObjectId id=p->createMaterial("L0", "C:/Users/xyh19/Desktop/CAD���ο���/Texture/NewData/image.jpg", 1/((headfile.ncols-1)*headfile.cellsize), 1 / ((headfile.nrows - 1)*headfile.cellsize), AcGeVector3d(1, 1, 0));
		p->setMaterial(id);
		CPostToModelSpace::PostToModelSpace(p);
		AfxMessageBox(L"success");*/
	}
	// Modal Command with pickfirst selection
	// ACED_ARXCOMMAND_ENTRY_AUTO(CTerrainModuleApp, AdskMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET)
	static void AdskMyGroupMyPickFirst () {
		ads_name result ;
		int iRet =acedSSGet (ACRX_T("_I"), NULL, NULL, NULL, result) ;
		if ( iRet == RTNORM )
		{
			// There are selected entities
			// Put your command using pickfirst set code here
		}
		else
		{
			// There are no selected entities
			// Put your command code here
		}
	}

	// Application Session Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CTerrainModuleApp, AdskMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION)
	static void AdskMyGroupMySessionCmd () {
		// Put your command code here
	}

	// The ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO macros can be applied to any static member 
	// function of the CTerrainModuleApp class.
	// The function may or may not take arguments and have to return RTNORM, RTERROR, RTCAN, RTFAIL, RTREJ to AutoCAD, but use
	// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal to return
	// a value to the Lisp interpreter.
	//
	// NOTE: ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid.
	
	//- ACED_ADSFUNCTION_ENTRY_AUTO(classname, name, regFunc) - this example
	//- ACED_ADSSYMBOL_ENTRYBYID_AUTO(classname, name, nameId, regFunc) - only differs that it creates a localized name using a string in the resource file
	//- ACED_ADSCOMMAND_ENTRY_AUTO(classname, name, regFunc) - a Lisp command (prefix C:)
	//- ACED_ADSCOMMAND_ENTRYBYID_AUTO(classname, name, nameId, regFunc) - only differs that it creates a localized name using a string in the resource file

	// Lisp Function is similar to ARX Command but it creates a lisp 
	// callable function. Many return types are supported not just string
	// or integer.
	// ACED_ADSFUNCTION_ENTRY_AUTO(CTerrainModuleApp, MyLispFunction, false)
	static int ads_MyLispFunction () {
		//struct resbuf *args =acedGetArgs () ;
		
		// Put your command code here

		//acutRelRb (args) ;
		
		// Return a value to the AutoCAD Lisp Interpreter
		// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal

		return (RTNORM) ;
	}
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CTerrainModuleApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CTerrainModuleApp, AdskMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CTerrainModuleApp, AdskMyGroup, CTerrain, CTerrain, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CTerrainModuleApp, AdskMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CTerrainModuleApp, AdskMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION, NULL)
ACED_ADSSYMBOL_ENTRY_AUTO(CTerrainModuleApp, MyLispFunction, false)

