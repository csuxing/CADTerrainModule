#pragma once
#include<string>
using namespace std;
struct BlockInformation
{
	double minx, miny, maxx, maxy;
	double width, height;
};
struct NextFrame
{
	string BlockName;
	BlockInformation BlockInfo;
};
struct QuadNode
{
	//数据域
	int layer;//层号
	string BlockName;//块名
	BlockInformation BlockInfo;//块的信息
	//指针域
	QuadNode * AreaLeftUp;
	QuadNode * AreaRightUp;
	QuadNode * AreaLeftDown;
	QuadNode * AreaRightDown;
	QuadNode() { layer = 0; AreaLeftUp = NULL; AreaRightUp = NULL; AreaLeftDown = NULL; AreaRightDown = NULL; };
};