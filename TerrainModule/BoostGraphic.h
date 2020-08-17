#pragma once
#include<boost/geometry.hpp>
#include<vector>
#include<string>

#include "CReadSrcFile.h"
#include <gepent3d.h>//使用acgepoint3d


namespace bg = boost::geometry;
typedef bg::model::d2::point_xy<double> BO_point2d;
typedef bg::model::point<double, 3, bg::cs::cartesian> BO_point3d;
typedef bg::model::polygon<BO_point2d> polygon;

enum SORTRULE
{
	SX, SY,SXY
};
/*
struct HeadFile
{
	int ncols, nrows;
	double xllcorner, yllcorner;
	double cellsize;
	float nodata_value;
};*/

struct MyPoint
{
	int x, y;
	MyPoint() {};
	MyPoint(int a,int b):x(a),y(b) {};
};
namespace CustomBoost
{

	class BoostGraphic
	{
	public:
		//return 0-on the line *** 1-top *** -1-under
		static int LeftOfLine(const BO_point2d& pt, 
			const BO_point2d& line_start, const BO_point2d& line_end);
		//on the 2d plane,Computing intersection ***2 geometry ,return a point array
		static void CalIntersectionPoint(BO_point2d rect[], BO_point2d poly[], 
			int n, std::vector<BO_point2d> &result);
		static void CalIntersectionPoint(polygon &re, polygon &poly_hole,
			std::deque<polygon> &output);
		//static function sort the point ,from left/down to right/up
		static int compare_qsort(void const *a, void const *b);

		static void Pointsort(std::vector<BO_point2d> &cell,SORTRULE rule);
		//calculate single hole with a terrain block
		static void TerrainBlockToSingleHole(HeadFile &headfile,std::vector<BO_point2d> hole,
			std::vector<std::vector<BO_point2d>> &hole_vec, std::vector<long> &IndexGrid);
		//calculate all hole with a terrain block
		static void TerrainBlockToAllHole(std::string outpath,HeadFile &headfile, std::vector<std::vector<BO_point2d>> &hole,
			std::string heightfile);//此处可以将数组变为多边形
		//give one point  ,find its position on the terrain
		static MyPoint findPositionOntheTerrain(BO_point2d srcPoint, double xllcorner,
			double yllcorner, double cellsize);
		//read elevation data 
		static bool readTerrainElevation(std::string filepath,std::vector<float> &elevation);
		static bool readTerrainHeadfile(std::string filepath, HeadFile &mheadfile);
		//VcEle all the elevation 高程插值函数 双线性插值法
		static double Interploation(double x,double y,HeadFile &headfile,std::vector<float> &VeEle);
		//挖洞主控函数
		static void DigHole(std::string filepath,std::string holefile,int layer);
		//读取洞文件 原始洞文件
		static void readHoleFile(std::string holefile, std::vector<std::vector<BO_point2d>> &hole);
		//读取洞文件 修改后用于画图的数据
		static bool readHoleFileForPaint(std::string holefile,std::vector<std::vector<AcGePoint3d>> &hole_pts);
		//构建polygon 通过数组构建
		static void ConstructPolygon(polygon &poly,int SideSize,BO_point2d verts[]);
		//判断是否为凸多边形
		static bool ConvetPolygonOrNot(const polygon &poly);
		//三角剖分 triangulation
		static void Triangulation(std::vector<BO_point2d> &points,std::vector<int> &orignal,std::vector<std::vector<int>> &output);
	};
	
}

