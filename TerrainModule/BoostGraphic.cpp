#include "BoostGraphic.h"
#include <iostream>
#include<fstream>
#include <io.h>
#include<direct.h>
using namespace std;
int CustomBoost::BoostGraphic::LeftOfLine(const BO_point2d & pt, const BO_point2d & line_start,
	const BO_point2d & line_end)
{
	double tmpx = (line_start.x()- line_end.x()) / (line_start.y() - line_end.y()) * (pt.y() - line_end.y()) + line_end.x();
	if (tmpx > pt.x())//当tmpx>p.x的时候，说明点在线的左边，小于在右边，等于则在线上。
		return 1;
	if (fabs(tmpx - pt.x()) < 0.001)
		return 0;
	return -1;
}

void CustomBoost::BoostGraphic::CalIntersectionPoint(BO_point2d rect[], BO_point2d poly[], 
	int n, std::vector<BO_point2d>& result)
{
	polygon re, p1y;
	//construct rectangle
	for (int i = 0; i < 5; i++)
	{
		bg::append(re.outer(), rect[i]);
	}
	//construct ploygon
	for (int i = 0; i < n; i++)
	{
		bg::append(p1y.outer(), poly[i]);
	}

	std::deque<polygon> output;

	bg::intersection(re, p1y ,output);
	result = output.front().outer();
}

void CustomBoost::BoostGraphic::CalIntersectionPoint(polygon &re, polygon & poly_hole, 
	std::deque<polygon> &output)
{
	bool inter = false;
	inter=bg::intersects(re, poly_hole);
	if (inter)
		bg::intersection(re, poly_hole, output);
	
}

int CustomBoost::BoostGraphic::compare_qsort(void const * a, void const * b)
{
	double rex = (*(BO_point2d *)a).x() - (*(BO_point2d *)b).x();
	if (fabs(rex) < 0.001)//x相等 比较y
	{
		double re = (*(BO_point2d *)a).y() - (*(BO_point2d *)b).y();
		if (fabs(re) < 0.001) return 0;
		else if (re > 0) return 1;
		else return -1;
	}
	else 
	{
		if (rex > 0) return 1;
		else return -1;
	}
}

void CustomBoost::BoostGraphic::Pointsort(std::vector<BO_point2d> &cell,SORTRULE rule)
{
	switch (rule)
	{
	case SX:
		qsort(&cell[0], cell.size(), sizeof(BO_point2d), [](void const *a, void const *b)->int { double re = ((*(BO_point2d *)a).x() - (*(BO_point2d *)b).x()); if (fabs(re) < 0.001)return 0; else if (re > 0)return 1; else return -1; });
		break;
	case SY:
		qsort(&cell[0], cell.size(), sizeof(BO_point2d), [](void const *a, void const *b)->int { double re = (*(BO_point2d *)a).y() - (*(BO_point2d *)b).y(); if (fabs(re) < 0.001)return 0; else if (re > 0)return 1; else return -1; });
		break;
	case SXY:
		qsort(&cell[0], cell.size(), sizeof(BO_point2d), compare_qsort);
		break;
	default:
		abort();
		break;
	}
	
}

MyPoint CustomBoost::BoostGraphic::findPositionOntheTerrain(BO_point2d srcPoint, double xllcorner,
	double yllcorner, double cellsize)
{
	double detx(0), dety(0);
	detx = srcPoint.x() - xllcorner;
	dety = srcPoint.y() - yllcorner;

	int rows(0), cols(0);
	rows = (int)dety / cellsize;//行数从0开始计数
	cols = (int)detx / cellsize;//列数从0开始计数

	return MyPoint(rows,cols);
}

bool CustomBoost::BoostGraphic::readTerrainElevation(std::string filepath,
	std::vector<float> &elevation)
{
	std::ifstream inFile(filepath.c_str(), ios::in | ios::binary);
	if (!inFile)
	{
		return false;
	}
	unsigned ncols, nrows;//4*2
	double x, y, cellsize;//8*3
	float no_data;//4
	inFile.read((char*)&ncols, sizeof(unsigned));
	inFile.read((char*)&nrows, sizeof(unsigned));
	inFile.read((char*)&x, sizeof(double));
	inFile.read((char*)&y, sizeof(double));
	inFile.read((char*)&cellsize, sizeof(double));
	inFile.read((char*)&no_data, sizeof(float));
	elevation.resize(ncols*nrows);
	inFile.read((char *)&elevation[0], sizeof(float)*nrows*ncols);
	inFile.close();
	return true;
}

bool CustomBoost::BoostGraphic::readTerrainHeadfile(std::string filepath, HeadFile &mheadfile)
{
	std::ifstream inFile(filepath.c_str(), ios::in | ios::binary);
	if (!inFile)
	{
		return false;
	}
	inFile.read((char*)&mheadfile.ncols, sizeof(unsigned));
	inFile.read((char*)&mheadfile.nrows, sizeof(unsigned));
	inFile.read((char*)&mheadfile.xllcorner, sizeof(double));
	inFile.read((char*)&mheadfile.yllcorner, sizeof(double));
	inFile.read((char*)&mheadfile.cellsize, sizeof(double));
	inFile.read((char*)&mheadfile.nodata_value, sizeof(float));
	inFile.close();
	return true;
}


double CustomBoost::BoostGraphic::Interploation(double x, double y, HeadFile & headfile, 
	std::vector<float>& VeEle)
{
	//左上角为坐标原点进行计算
	x = x - headfile.xllcorner;
	y = headfile.cellsize*(headfile.nrows-1)-(y - headfile.yllcorner);


	int cols = (int)(x / headfile.cellsize);
	int rows = (int)(y / headfile.cellsize);
	double detx = x - cols * headfile.cellsize;
	double dety = headfile.cellsize - y + rows * headfile.cellsize;

	double ld, lu, rd, ru;
	lu = VeEle[rows*headfile.ncols+cols];
	ru = VeEle[rows*headfile.ncols + cols + 1];
	ld = VeEle[(rows + 1)*headfile.ncols + cols];
	rd = VeEle[(rows)*headfile.ncols + cols + 1];

	double height = (1 - detx / headfile.cellsize)*(1 - dety / headfile.cellsize)*ld + (detx / headfile.cellsize)*(1 - dety / headfile.cellsize)*rd +
		(detx / headfile.cellsize)*(dety / headfile.cellsize)*ru + (1 - detx / headfile.cellsize)*(dety / headfile.cellsize)*lu;
	return height;
}

void CustomBoost::BoostGraphic::DigHole(std::string filepath, std::string holefile, int layer)
{
	std::vector<std::vector<BO_point2d>> Vechole;//用于存储读取到的洞文件
	readHoleFile(holefile, Vechole);


	std::vector<polygon> poly_hole;//存储洞多边形

	for (int i=0;i<Vechole.size();i++)
	{
		polygon poly;
		for (int j=0;j<Vechole[i].size();j++)
		{
			bg::append(poly.outer(), Vechole[i][j]);
		}
		bg::append(poly.outer(), Vechole[i][0]);
		poly_hole.push_back(poly);
	}
	//修改文件路径 定位到地形文件
	int position = filepath.find_last_of("\\");
	std::string dempath = filepath.substr(0,position+1) + "dem\\";
	std::string outpath = filepath.substr(0, position + 1) + "modify";

	int position_point = filepath.find_last_of(".");
	std::string filename = filepath.substr(position + 1, position_point - position - 1);

	
	/*std::vector<float> VecElv;//一次读取高程值 函数生存期间使用
	readTerrainElevation(heightfile, VecElv);*/

	if (_access(outpath.c_str(),0)!=0)
	{
		_mkdir(outpath.c_str());
	}

	for (int i = 0; i < layer; i++)
		for (int j = 0; j < pow(2, i); j++)
			for (int k = 0; k < pow(2, i); k++)
			{
				std::string Lpath = dempath + "L" + std::to_string(i) + "_" + std::to_string(j) + "_" + to_string(k);
				HeadFile mheadfile;
				readTerrainHeadfile(Lpath, mheadfile);

				BO_point2d rect[4];
				rect[0].x(mheadfile.xllcorner );
				rect[0].y(mheadfile.yllcorner );
				rect[1].x(rect[0].x());
				rect[1].y(rect[0].y() + mheadfile.cellsize*(mheadfile.nrows-1));
				rect[2].x(rect[1].x() + mheadfile.cellsize*(mheadfile.ncols-1));
				rect[2].y(rect[1].y());
				rect[3].x(rect[2].x());
				rect[3].y(rect[0].y());
				
				polygon poly_rect;
				for (int n = 0; n < 4; n++)
				{
					bg::append(poly_rect.outer(), rect[n]);
				}
				bg::append(poly_rect.outer(), rect[0]);

				for (int m = 0; m < poly_hole.size(); m++)
				{
					bool intersection = bg::intersects(poly_rect, poly_hole[m]);
					if (intersection)
					{
						std::string heightfile = dempath + "L" + std::to_string(i) + "_" + std::to_string(j) + "_" + to_string(k);
						std::string modify_path=outpath+"\\L"+ std::to_string(i) + "_" + std::to_string(j) + "_" + to_string(k);
						TerrainBlockToAllHole(modify_path,mheadfile, Vechole,heightfile);
						break;
					}
				}
			}

}

void CustomBoost::BoostGraphic::readHoleFile(std::string holefile, std::vector<std::vector<BO_point2d>>& hole)
{
	//假设 洞边界数据成对出现 txt文本格式 数据类型为 double
	ifstream inFile(holefile.c_str(), ios::in);
	if (!inFile)
	{
		return;
	}
	unsigned totalcountNum;
	double x,y,nodata;
	inFile >> totalcountNum;
	for (int i=0;i<totalcountNum;i++)
	{
		unsigned singCountNum;
		inFile >> singCountNum;
		std::vector<BO_point2d> temHole(singCountNum);
		for (int j=0;j<singCountNum;j++)
		{
			inFile >> x;
			inFile >> y;
			inFile >> nodata;
			temHole[j].x(x);
			temHole[j].y(y);
		}

		hole.push_back(temHole);
	}
	

	inFile.close();
}

bool CustomBoost::BoostGraphic::readHoleFileForPaint(std::string holefile, std::vector<std::vector<AcGePoint3d>>& hole_pts)
{
	if (0!=_access(holefile.c_str(),0))
	{
		return false;//文件不存在 说明没有洞 直接绘图
	}
	ifstream inFile(holefile.c_str(), ios::in | ios::binary);
	if (!inFile)
	{
		return false;
	}
	int hole_num;
	inFile.read((char *)&hole_num, sizeof(int));

	for (int i=0;i<hole_num;i++)
	{
		int num;
		inFile.read((char *)&num, sizeof(int));
		if (num < 0)
		{
			std::vector<AcGePoint3d> singlehole;
			hole_pts.push_back(singlehole);
			continue;
		}
		std::vector<AcGePoint3d> singlehole(num);
		inFile.read((char *)&singlehole[0], sizeof(AcGePoint3d)*num);
		hole_pts.push_back(singlehole);
	}

	inFile.close();
}

void CustomBoost::BoostGraphic::ConstructPolygon(polygon & poly, int SideSize, BO_point2d verts[])
{
	for (int i=0;i<SideSize;i++)
	{
		bg::append(poly.outer(), verts[i]);
	}
	bg::append(poly.outer(), verts[0]);
}

bool CustomBoost::BoostGraphic::ConvetPolygonOrNot(const polygon & poly)
{
	polygon hull;
	bg::convex_hull(poly,hull);
	bool ornot;
	ornot = bg::equals(poly, hull);
	return ornot;
}

void CustomBoost::BoostGraphic::Triangulation(std::vector<BO_point2d>& points,std::vector<int> &orignal,std::vector<std::vector<int>>& output)
{
	polygon poly;
	for (int i = 0; i < points.size(); i++)
	{
		bg::append(poly.outer(), points[i]);
	}
	bg::correct(poly);
	if (points.size() == 3 || ConvetPolygonOrNot(poly))
	{
		for (int j = 0; j < points.size() - 2; j++)
		{
			std::vector<int> tmp;
			tmp.push_back(orignal[0]);
			tmp.push_back(orignal[j + 1]);
			tmp.push_back(orignal[j + 2]);
			output.push_back(tmp);
		}
		return;
	}
	else
	{
		polygon tmppoly;
		polygon tri;
		std::vector<int> TmpVecInt;
		std::vector<BO_point2d> TmpVecBo;
		int Iori;
		int i = 0;
		do
		{
			//寻找凸顶点 若找到 则终止循环
			

			

			for (; i < points.size(); i++)
			{
				//构建多边形
				tmppoly.clear();
				for (int k = 0; k < points.size(); k++)
				{
					if (k != i)
					{
						bg::append(tmppoly.outer(), points[k]);
					}
				}
				bg::correct(tmppoly);
				if (!bg::within(points[i], tmppoly))
				{
					Iori = i;
					i++;
					//构建三角形
					TmpVecBo.clear();
					if (Iori == 0)
					{
						TmpVecBo.push_back(points.front());
						TmpVecBo.push_back(points[Iori + 1]);
						TmpVecBo.push_back(points.back());
					}
					else if (Iori == points.size() - 1)
					{
						TmpVecBo.push_back(points.back());
						TmpVecBo.push_back(points[Iori - 1]);
						TmpVecBo.push_back(points.front());
					}
					else
					{
						TmpVecBo.push_back(points[Iori - 1]);
						TmpVecBo.push_back(points[Iori]);
						TmpVecBo.push_back(points[Iori + 1]);
					}
					tri.clear();
					for (int t = 0; t < TmpVecBo.size(); t++)
					{
						bg::append(tri.outer(), TmpVecBo[t]);
					}
					bg::correct(tri);
					break;
				}
			}
		} while (bg::intersects(tmppoly) || bg::overlaps(tri, tmppoly));
		//输出三角形顶点位置
		if (Iori == 0)
		{
			TmpVecInt.push_back(orignal.front());
			TmpVecInt.push_back(orignal[Iori + 1]);
			TmpVecInt.push_back(orignal.back());
		}
		else if (Iori == points.size() - 1)
		{
			TmpVecInt.push_back(orignal.back());
			TmpVecInt.push_back(orignal[Iori - 1]);
			TmpVecInt.push_back(orignal.front());
		}
		else
		{
			TmpVecInt.push_back(orignal[Iori - 1]);
			TmpVecInt.push_back(orignal[Iori]);
			TmpVecInt.push_back(orignal[Iori + 1]);
		}
		//维护原始位置数组
		orignal.erase(orignal.begin() + Iori);
		points.erase(points.begin() + Iori);
		output.push_back(TmpVecInt);
		Triangulation(points, orignal, output);
	}
}



void CustomBoost::BoostGraphic::TerrainBlockToSingleHole(HeadFile & headfile, std::vector<BO_point2d> hole, 
	std::vector<std::vector<BO_point2d>> &hole_vec,std::vector<long> &IndexGrid)
{
	//it must be intersection, we want to find intersecion cells
	std::vector<BO_point2d> vec;
	vec = hole;
	double min_x(0), min_y(0), max_x(0), max_y(0);
	Pointsort(vec, SORTRULE::SX);
	min_x =vec.front().x();
	max_x =vec.back().x();
	Pointsort(vec, SORTRULE::SY);
	min_y = vec.front().y();
	max_y= vec.back().y();
	
	MyPoint start, end;

	start = findPositionOntheTerrain(BO_point2d(min_x, min_y),
		headfile.xllcorner, headfile.yllcorner, headfile.cellsize);
	start.x = std::max(0, start.x);
	start.y = std::max(0, start.y);
	end = findPositionOntheTerrain(BO_point2d(max_x, max_y),
		headfile.xllcorner, headfile.yllcorner, headfile.cellsize);
	end.x = std::min(headfile.nrows - 2, end.x);
	end.y = std::min(headfile.ncols - 2, end.y);

	polygon poly_hole;
	for (int i=0;i<hole.size();i++)
	{
		bg::append(poly_hole.outer(), hole[i]);
	}
	bg::append(poly_hole.outer(), hole[0]);//polygon is closed

	BO_point2d rect[4];//点采用顺时针方向
	
	for (int i=start.x;i<=end.x;i++)
	{
		for (int j=start.y;j<=end.y;j++)
		{
			rect[0].x (headfile.xllcorner + j * headfile.cellsize);
			rect[0].y (headfile.yllcorner + i * headfile.cellsize);
			rect[1].x (rect[0].x());
			rect[1].y (rect[0].y() + headfile.cellsize);
			rect[2].x (rect[1].x() + headfile.cellsize);
			rect[2].y (rect[1].y());
			rect[3].x (rect[2].x());
			rect[3].y (rect[2].y() - headfile.cellsize);

			polygon re;
			for (int i = 0; i < 4; i++)
			{
				bg::append(re.outer(), rect[i]);
			}
			bg::append(re.outer(), rect[0]);

			std::deque<polygon> output;
			CalIntersectionPoint(re, poly_hole, output);//get the boundray point array 

			if (output.empty())
			{
				continue;
			}
			//以左上角起始编号
			//long index = (headfile.nrows - i - 2)*(headfile.ncols - 1) + j;
			//以左下角起始编号
			long index = i*(headfile.ncols - 1) + j;
			IndexGrid.push_back(index);

			std::list<polygon> result;
			bg::difference(re, output.front(), result);
			if (result.empty())
			{
				std::vector<BO_point2d> tmp;
				hole_vec.push_back(tmp);
			}
			else
			{
				hole_vec.push_back(result.front().outer());
			}
		}
	}
}

void CustomBoost::BoostGraphic::TerrainBlockToAllHole(std::string outpath,HeadFile &headfile, std::vector<std::vector<BO_point2d>>& hole,
	std::string heightfile)
{
	
	std::vector<float> VecElv;
	readTerrainElevation(heightfile, VecElv);

	std::vector<std::vector<BO_point2d>> hole_vec;//写出裁剪后的文件
	std::vector<long> IndexGrid;//索引数组

	polygon rectangle;
	std::vector<BO_point2d> veRect;
	veRect.push_back(BO_point2d(headfile.xllcorner, headfile.yllcorner));
	veRect.push_back(BO_point2d(headfile.xllcorner, headfile.yllcorner+ (headfile.nrows - 1)*headfile.cellsize));
	veRect.push_back(BO_point2d(headfile.xllcorner+(headfile.ncols - 1)*headfile.cellsize, headfile.yllcorner+ (headfile.nrows - 1)*headfile.cellsize));
	veRect.push_back(BO_point2d(headfile.xllcorner + (headfile.ncols - 1)*headfile.cellsize, headfile.yllcorner));
	for (int i=0;i<4;i++)
	{
		bg::append(rectangle.outer(), veRect[i]);
	}
	bg::append(rectangle.outer(), veRect[0]);
	//判断地形与单洞是否相交 相交进行下一步操作
	for (int i=0;i<hole.size();i++)
	{
		polygon poly_hole;
		for (int j = 0; j < hole[i].size(); j++)
		{
			bg::append(poly_hole.outer(), hole[i][j]);
		}
		bg::append(poly_hole.outer(), hole[i][0]);

		bool Intersection=bg::intersects(rectangle, poly_hole);
		if (Intersection)
		{
			TerrainBlockToSingleHole(headfile, hole[i], hole_vec,IndexGrid);
		}
	}
	//read a terrain's elevation
	//interpolation 插值
	ofstream outfile(outpath.c_str(),ios::out|ios::binary);
	if (!outfile)
	{
		return ;
		abort();
	}
	unsigned holenum=hole_vec.size();
	outfile.write((char*)&holenum, sizeof(int));

	for (int i=0;i<hole_vec.size();i++)
	{
		int singHolePointNum = hole_vec[i].size() - 1;
		outfile.write((char *)&singHolePointNum, sizeof(int));

		for (int j=0;j<singHolePointNum;j++)
		{
			double x = hole_vec[i][j].x();
			double y = hole_vec[i][j].y();
			double z = Interploation(x, y, headfile, VecElv);
			outfile.write((char *)&x, sizeof(double));
			outfile.write((char *)&y, sizeof(double));
			outfile.write((char *)&z, sizeof(double));
		}
	}
	outfile.close();
	//write the hole file ,x y z 顺时针
	//unsigned 4byte long 8byte
	std::string indexOutpath = outpath + "_index";
	ofstream IndexOut(indexOutpath.c_str(),ios::out|ios::binary);
	if (!IndexOut)
	{
		return;
	}
	unsigned total_num = IndexGrid.size();
	IndexOut.write((char *)&total_num, sizeof(unsigned));
	IndexOut.write((char *)&IndexGrid[0], sizeof(long)*total_num);
	IndexOut.close();
}
