#pragma once
#include<string>
#include<vector>
#include "gdalwarper.h"
struct HeadFile
{
	int ncols, nrows;
	double xllcorner, yllcorner;
	double cellsize;
	float nodata_value;
};
class CReadSrcFile
{
public:
	
	friend class CCustomPolygonmesh;//声明友元类 使用读文件静态方法
	static bool ReadASC(std::string filepath,double **asc,HeadFile &headfile);
	static bool ReadAscHeadfile(std::string filepath, HeadFile &headfile);//读取asc文件的头文件，确定行列数,CAD实体
	static double ** CreateDynamicArray(double **array,int M,int N);
	static bool DeleteDynamicArray(double **array, int M, int N);

	static double * CreateDynamicArray1D(double *asc, int Pointnum);//自定义实体数据
	static bool DeleteDynamicArray1D(double *asc,int Pointnum);
	
	//////////////////////////////////////////////////////////////////////////对asc文件进行分块划分
	static bool ASCtoDEM(std::string input,std::string output);//将文本格式转为二进制存储
	static void readtifftodem(std::string inputfile, std::string output);//将tiff转化为dem
	static void DEMtoASC(std::string input, std::string output);
	static bool ReadASCIIHeadfile(std::string filepath, HeadFile &headfile);
	static bool ReadASCII(std::vector<float> &vec,std::string filepath);//顺序读取
	static bool ReadASCIIindex(std::string filepath,std::vector<long> &index);
	static bool ASCresample(std::string input,std::string output,double scale,int position);//暂时废弃
	static void TerrainBlock(int depth, std::string input, std::string output,HeadFile headfile);//将大地形文件划分为小地形文件 output为输出路径,以文件进行划分
	static void TerrainBlock(int depth, std::string input, std::string output, HeadFile headfile, HeadFile originHeadfile);//以原始文件为框架进行划分，使每一个小块都能覆盖相应框架部分
	static void TerrainChangeCellsize(std::string input,std::string output,double scale);//改变地形块的格网间距，scale为采样间距的倍数 *2 cellsize变为原来的二倍
	static float getheight(std::string input,double x,double y,HeadFile headfile);//获取某文件内的高程
	static void ProduceVirtualDem(std::string input,std::string &output);//produce virtual dem
	//////////////////////////////////////////////////////////////////////////生成栅格金字塔
	static void CreatePyramid(int depth, std::string inputfilepathname,std::string dir);
	//////////////////////////////////////////////////////////////////////////图片处理部分
	static int ResampleGDAL(const char* pszSrcFile, const char* pszOutFile, float fResX = 1.0, float fResY = 1.0, GDALResampleAlg eResample = GRA_Bilinear);//gdalwarp 
	static int ReSampleBlock(std::string program, std::string inputfile, int xsize, int ysize, int layerclass, bool compress);//分块重采样
	static int ImageDividedBlock(std::string inputfile, int layerClass, bool compress);//影像分块 

	static void createImagePyramid(std::string inputfile, int layer, bool compress = true);//图像金字塔

	//获取影像、高程文件的路径 返回值 true 读取成功
	static bool ReadPicAndHeiPath(std::string &filepath_pic, std::string &filepath_height ,int &layer);

};

