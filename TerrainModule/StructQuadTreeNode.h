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
	//������
	int layer;//���
	string BlockName;//����
	BlockInformation BlockInfo;//�����Ϣ
	//ָ����
	QuadNode * AreaLeftUp;
	QuadNode * AreaRightUp;
	QuadNode * AreaLeftDown;
	QuadNode * AreaRightDown;
	QuadNode() { layer = 0; AreaLeftUp = NULL; AreaRightUp = NULL; AreaLeftDown = NULL; AreaRightDown = NULL; };
};