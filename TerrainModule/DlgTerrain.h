#pragma once


// CDlgTerrain �Ի���
#include "resource.h"

class CDlgTerrain : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTerrain)

public:
	CDlgTerrain(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTerrain();

// �Ի�������
	enum { IDD = IDD_DIALOG_Terrain };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
