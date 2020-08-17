#pragma once
#include"StructQuadTreeNode.h"
#include<vector>
#include<iostream>
class CTerrainQuadTree
{
	//����һ���Ĳ������ڵ���Ϣ���������Ϊ������꣬��ı��
public:
	CTerrainQuadTree() { Root = new QuadNode;};
	void CreateQuadTree(int Idepth,QuadNode **node,string block,BlockInformation blockinfo,int position = 0 );//����ȣ���㣬λ��position,1,2,3,4����ͬ��λ�� 0�������� blockinfo ��ʼ������Ϣ
	void TraverseQuadTree(QuadNode *node);
	void TraverseQuadTree(QuadNode *node, int layer);
	static void GetRowsandColsInString(string blockname,int &rows,int &cols,int &layer);
	static BlockInformation & DivideBlock(BlockInformation info,int position);
	void setdepth(int Idepth) { depth = Idepth; };
	void InitBasicBlockInfo(string input);

	void DeleteTerrainTree(QuadNode *node);
/*private:*/
	QuadNode *Root;
	int depth;

	vector<NextFrame> nextFrame;//��װ�ļ����Ϳ����Ϣ

	vector<string> nextframe;//�����һ֡���ļ���
	vector<BlockInformation> nextTexInf;//�����һ֡����ʵλ��

	double minx, miny, maxx, maxy;//ѡȡ�������������Ļ��Χ
	string programName;
	

	BlockInformation BasicInfo;
};

