#pragma once


// CDlgTerrain 对话框
#include "resource.h"

class CDlgTerrain : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTerrain)

public:
	CDlgTerrain(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTerrain();

// 对话框数据
	enum { IDD = IDD_DIALOG_Terrain };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPic();
	afx_msg void OnBnClickedButtonHeight();
	CString m_edit_pic;
	CString m_edit_height;
	afx_msg void OnBnClickedButtonCreatepyramid();
	int m_edit_layer;
	afx_msg void OnBnClickedButtonCr2dterrain();
	afx_msg void OnBnClickedButtonCr3dterrain();
};
