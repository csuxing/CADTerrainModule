#pragma once
#include"StructQuadTreeNode.h"
#include<vector>
#include<iostream>
class CTerrainQuadTree
{
	//创建一颗四叉树，节点信息保存的数据为块的坐标，块的编号
public:
	CTerrainQuadTree() { Root = new QuadNode;};
	void CreateQuadTree(int Idepth,QuadNode **node,string block,BlockInformation blockinfo,int position = 0 );//树深度，结点，位置position,1,2,3,4代表不同的位置 0代表根结点 blockinfo 初始地理信息
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

	vector<NextFrame> nextFrame;//包装文件名和块的信息

	vector<string> nextframe;//存放下一帧的文件名
	vector<BlockInformation> nextTexInf;//存放下一帧的真实位置

	double minx, miny, maxx, maxy;//选取区域的条件，屏幕范围
	string programName;
	

	BlockInformation BasicInfo;
};

