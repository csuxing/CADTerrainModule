#include "CTerrainQuadTree.h"
#include "CReadSrcFile.h"
void CTerrainQuadTree::CreateQuadTree(int Idepth, QuadNode ** node, string Block,BlockInformation blockinfo,int position)
{
	//创建一个结点
	if (Idepth!=0)
	{
		QuadNode *pNode = new QuadNode;
		
		int layer, rows, cols;
		int oLayer, orows, ocols;
		GetRowsandColsInString(Block, rows, cols, layer);
		switch (position)
		{
		case 1:
			oLayer = layer + 1;
			orows = 2 * rows;
			ocols = 2 * cols;
			pNode->BlockName = "L" + to_string(oLayer) + "_" + to_string(orows) + "_" + to_string(ocols);
			pNode->layer = oLayer;
			pNode->BlockInfo = DivideBlock(blockinfo, 1);
			break;
		case 2:
			oLayer = layer + 1;
			orows = 2 * rows;
			ocols = 2 * cols+1;
			pNode->BlockName = "L" + to_string(oLayer) + "_" + to_string(orows) + "_" + to_string(ocols);
			pNode->layer = oLayer;
			pNode->BlockInfo = DivideBlock(blockinfo, 2);
			break;
		case 3:
			oLayer = layer + 1;
			orows = 2 * rows+1;
			ocols = 2 * cols;
			pNode->BlockName = "L" + to_string(oLayer) + "_" + to_string(orows) + "_" + to_string(ocols);
			pNode->layer = oLayer;
			pNode->BlockInfo = DivideBlock(blockinfo, 3);
			break;
		case 4:
			oLayer = layer + 1;
			orows = 2 * rows+1;
			ocols = 2 * cols+1;
			pNode->BlockName = "L" + to_string(oLayer) + "_" + to_string(orows) + "_" + to_string(ocols);
			pNode->layer = oLayer;
			pNode->BlockInfo = DivideBlock(blockinfo, 4);
			break;
		default:
			pNode->BlockName = "L0_0_0";
			pNode->layer = 0;
			
			pNode->BlockInfo = blockinfo;
			break;
		}
		*node = pNode;
		CreateQuadTree(Idepth - 1, &(*node)->AreaLeftUp, pNode->BlockName,pNode->BlockInfo,1);
		CreateQuadTree(Idepth - 1, &(*node)->AreaRightUp,pNode->BlockName,pNode->BlockInfo,2);
		CreateQuadTree(Idepth - 1, &(*node)->AreaLeftDown,pNode->BlockName,pNode->BlockInfo,3);
		CreateQuadTree(Idepth - 1, &(*node)->AreaRightDown,pNode->BlockName,pNode->BlockInfo,4);
	}
	
	
	
}

void CTerrainQuadTree::TraverseQuadTree(QuadNode *node)
{
	if (node!=NULL)
	{
		//cout << node->BlockName<<endl;
		if (node->BlockInfo.minx >= minx &&
			node->BlockInfo.miny >= miny &&
			node->BlockInfo.maxx <= maxx &&
			node->BlockInfo.maxy <= maxy
			/*(node->BlockInfo.minx+node->BlockInfo.maxx)/2>minx&&
			(node->BlockInfo.minx + node->BlockInfo.maxx)/2<maxx&&
			(node->BlockInfo.miny+node->BlockInfo.maxy)/2>miny&&
			(node->BlockInfo.miny+node->BlockInfo.maxy)/2<maxy*/
			)
		{
			/*NextFrame nf;
			nf.BlockName = node->BlockName;
			nf.BlockInfo = node->BlockInfo;
			this->nextFrame.push_back(nf);*/
			this->nextframe.push_back(node->BlockName);
			//this->nextTexInf.push_back(node->BlockInfo);
		}
		else
		{
			TraverseQuadTree(node->AreaLeftUp);
			TraverseQuadTree(node->AreaRightUp);
			TraverseQuadTree(node->AreaLeftDown);
			TraverseQuadTree(node->AreaRightDown);
		}
	}
	
}

void CTerrainQuadTree::TraverseQuadTree(QuadNode * node, int layer)
{
	if (node!=NULL)
	{

		if (node->layer < layer)
		{
			TraverseQuadTree(node->AreaLeftUp, layer);
			TraverseQuadTree(node->AreaRightUp, layer);
			TraverseQuadTree(node->AreaLeftDown, layer);
			TraverseQuadTree(node->AreaRightDown, layer);
		}

		if (node->layer==layer)
		{
			if ((node->BlockInfo.minx + node->BlockInfo.maxx) / 2 > minx &&
				(node->BlockInfo.minx + node->BlockInfo.maxx) / 2 < maxx &&
				(node->BlockInfo.miny + node->BlockInfo.maxy) / 2 > miny &&
				(node->BlockInfo.miny + node->BlockInfo.maxy) / 2 < maxy)
			{
				this->nextframe.push_back(node->BlockName);
			}
		}
		
		
	}
}

void CTerrainQuadTree::GetRowsandColsInString(string blockname, int & rows, int & cols,int &layer)
{
	int L = blockname.find("L");
	int first_ = blockname.find_first_of("_");
	int last_ = blockname.find_last_of("_");
	string Layer = blockname.substr(L + 1, first_ - L-1);
	string srows = blockname.substr(first_+ 1, last_ - first_-1);
	string scols = blockname.substr(last_+1);
	layer = atoi(Layer.c_str());
	rows = atoi(srows.c_str());
	cols = atoi(scols.c_str());
}

BlockInformation & CTerrainQuadTree::DivideBlock(BlockInformation info,int position)
{
	BlockInformation newinfo;
	newinfo.height = info.height / 2;
	newinfo.width = info.width / 2;
	switch (position)
	{
	case 1:
		newinfo.minx = info.minx;
		newinfo.miny = info.miny + newinfo.height;
		newinfo.maxx = newinfo.minx + newinfo.width;
		newinfo.maxy = newinfo.miny + newinfo.height;
		break;
	case 2:
		newinfo.minx = info.minx + newinfo.width;
		newinfo.miny = info.miny + newinfo.height;
		newinfo.maxx = newinfo.minx + newinfo.width;
		newinfo.maxy = newinfo.miny + newinfo.height;
		break;
	case 3:
		newinfo.minx = info.minx ;
		newinfo.miny = info.miny ;
		newinfo.maxx = newinfo.minx + newinfo.width;
		newinfo.maxy = newinfo.miny + newinfo.height;
		break;
	case 4:
		newinfo.minx = info.minx + newinfo.width ;
		newinfo.miny = info.miny ;
		newinfo.maxx = newinfo.minx + newinfo.width;
		newinfo.maxy = newinfo.miny + newinfo.height;
		break;
	default:
		break;
	}
	return newinfo;
}

void CTerrainQuadTree::InitBasicBlockInfo(string input)
{
	HeadFile headfile;
	CReadSrcFile::ReadASCIIHeadfile(input, headfile);
	BasicInfo.minx = headfile.xllcorner;
	BasicInfo.miny = headfile.yllcorner;
	BasicInfo.width = headfile.cellsize*(headfile.ncols - 1);
	BasicInfo.height = headfile.cellsize*(headfile.nrows - 1);
}

void CTerrainQuadTree::DeleteTerrainTree(QuadNode * node)
{
	if (node!=NULL)
	{
		QuadNode *n1, *n2, *n3, *n4;
		n1 = node->AreaLeftDown;
		n2 = node->AreaRightDown;
		n3 = node->AreaLeftUp;
		n4 = node->AreaRightUp;
		delete node;
		node = NULL;
		DeleteTerrainTree(n1);
		DeleteTerrainTree(n2);
		DeleteTerrainTree(n3);
		DeleteTerrainTree(n4);
	}
}
