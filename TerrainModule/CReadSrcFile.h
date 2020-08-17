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
	
	friend class CCustomPolygonmesh;//������Ԫ�� ʹ�ö��ļ���̬����
	static bool ReadASC(std::string filepath,double **asc,HeadFile &headfile);
	static bool ReadAscHeadfile(std::string filepath, HeadFile &headfile);//��ȡasc�ļ���ͷ�ļ���ȷ��������,CADʵ��
	static double ** CreateDynamicArray(double **array,int M,int N);
	static bool DeleteDynamicArray(double **array, int M, int N);

	static double * CreateDynamicArray1D(double *asc, int Pointnum);//�Զ���ʵ������
	static bool DeleteDynamicArray1D(double *asc,int Pointnum);
	
	//////////////////////////////////////////////////////////////////////////��asc�ļ����зֿ黮��
	static bool ASCtoDEM(std::string input,std::string output);//���ı���ʽתΪ�����ƴ洢
	static void readtifftodem(std::string inputfile, std::string output);//��tiffת��Ϊdem
	static void DEMtoASC(std::string input, std::string output);
	static bool ReadASCIIHeadfile(std::string filepath, HeadFile &headfile);
	static bool ReadASCII(std::vector<float> &vec,std::string filepath);//˳���ȡ
	static bool ReadASCIIindex(std::string filepath,std::vector<long> &index);
	static bool ASCresample(std::string input,std::string output,double scale,int position);//��ʱ����
	static void TerrainBlock(int depth, std::string input, std::string output,HeadFile headfile);//��������ļ�����ΪС�����ļ� outputΪ���·��,���ļ����л���
	static void TerrainBlock(int depth, std::string input, std::string output, HeadFile headfile, HeadFile originHeadfile);//��ԭʼ�ļ�Ϊ��ܽ��л��֣�ʹÿһ��С�鶼�ܸ�����Ӧ��ܲ���
	static void TerrainChangeCellsize(std::string input,std::string output,double scale);//�ı���ο�ĸ�����࣬scaleΪ�������ı��� *2 cellsize��Ϊԭ���Ķ���
	static float getheight(std::string input,double x,double y,HeadFile headfile);//��ȡĳ�ļ��ڵĸ߳�
	static void ProduceVirtualDem(std::string input,std::string &output);//produce virtual dem
	//////////////////////////////////////////////////////////////////////////����դ�������
	static void CreatePyramid(int depth, std::string inputfilepathname,std::string dir);
	//////////////////////////////////////////////////////////////////////////ͼƬ������
	static int ResampleGDAL(const char* pszSrcFile, const char* pszOutFile, float fResX = 1.0, float fResY = 1.0, GDALResampleAlg eResample = GRA_Bilinear);//gdalwarp 
	static int ReSampleBlock(std::string program, std::string inputfile, int xsize, int ysize, int layerclass, bool compress);//�ֿ��ز���
	static int ImageDividedBlock(std::string inputfile, int layerClass, bool compress);//Ӱ��ֿ� 

	static void createImagePyramid(std::string inputfile, int layer, bool compress = true);//ͼ�������

	//��ȡӰ�񡢸߳��ļ���·�� ����ֵ true ��ȡ�ɹ�
	static bool ReadPicAndHeiPath(std::string &filepath_pic, std::string &filepath_height ,int &layer);

};

