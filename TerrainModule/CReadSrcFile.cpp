#include "stdafx.h"
#include "CReadSrcFile.h"

#include "gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")
#include "ogrsf_frmts.h"
//#include"StdAfx.h"
#include<string>
#include<fstream>
#include<iostream>
#include <iomanip>
#include<direct.h>
#include<io.h>
#include<stdio.h>
#include<math.h>
#include"StringConvert.h"
#define RAM_RIZE 1600
using namespace std;

bool CReadSrcFile::ReadASC(std::string filepath, double **asc,HeadFile &headfile)
{
	
	std::string s;
	ifstream infile(filepath);
	if (!infile)
	{
		return false;
	}
	infile >> s >> headfile.ncols;
	infile >> s >> headfile.nrows;
	infile >> s >> headfile.xllcorner;
	infile >> s >> headfile.yllcorner;
	infile >> s >> headfile.cellsize;
	infile >> s >> s;//读取文件中的每一个字符，存储有用数据
	//cout.setf(ios::fixed);
	//cout.precision(5);
	//cout << ncols << "\n" << nrows << "\n" << xllcorner << "\n" << yllcorner << "\n" << cellsize << s << endl;
	for (int i = 0; i < headfile.nrows; i++)
		for (int j = 0; j < headfile.ncols; j++)
			infile >> asc[j][i];
	infile.close();
	return true;
}

bool CReadSrcFile::ReadAscHeadfile(string filepath, HeadFile & headfile)
{
	std::string s;
	ifstream infile(filepath);
	if (!infile)
	{
		return false;
	}
	infile >> s >> headfile.ncols;
	infile >> s >> headfile.nrows;
	infile >> s >> headfile.xllcorner;
	infile >> s >> headfile.yllcorner;
	infile >> s >> headfile.cellsize;
	infile.close();
	return true;
}

double ** CReadSrcFile::CreateDynamicArray(double ** array, int M, int N)
{
	array = new double *[M];
	for (int i=0;i<M;i++)
	{
		array[i] = new double[N];
	}
	return array;
}

bool CReadSrcFile::DeleteDynamicArray(double ** array, int M, int N)
{
	for (int i=0;i<M;i++)
	{
		delete[] array[i];
	}
	delete []array;
	return true;
}

double * CReadSrcFile::CreateDynamicArray1D(double * asc, int Pointnum)
{
	asc = new double[Pointnum];
	return asc;
}

bool CReadSrcFile::DeleteDynamicArray1D(double * asc, int Pointnum)
{
	delete asc;
	return true;
}
bool CReadSrcFile::ASCtoDEM(string input, string output)
{
	FILE *pFRead = fopen(input.c_str(),"r");
	FILE *pFWrite =fopen(output.c_str(),"wb");
	if (pFRead && pFWrite)
	{
		char str[50];
		int ncols, nrows;
		double wide;
		double XOri, YOri;
		double  nodata_value;
		if (pFRead)
		{
			fscanf(pFRead, "%s\t%d", str, &ncols);
			fscanf(pFRead, "%s\t%d", str, &nrows);
			fscanf(pFRead, "%s\t%lf", str, &XOri);
			fscanf(pFRead, "%s\t%lf", str, &YOri);
			fscanf(pFRead, "%s\t%lf", str, &wide);
			fscanf(pFRead, "%s\t%f", str, &nodata_value);

			fwrite(&ncols, sizeof(unsigned), 1, pFWrite);
			fwrite(&nrows, sizeof(unsigned), 1, pFWrite);
			fwrite(&XOri, sizeof(double), 1, pFWrite);
			fwrite(&YOri, sizeof(double), 1, pFWrite);
			fwrite(&wide, sizeof(double), 1, pFWrite);
			fwrite(&nodata_value, sizeof(float), 1, pFWrite);
		}
		else
		{
			printf("读取数字高程文件: %s 失败！", input.c_str());
			return false;
		}

		float z = nodata_value;
		for (int r = nrows - 1; r > -1; r--)
		{
			for (int c = 0; c < ncols; c++)
			{
				fscanf(pFRead, "%f", &z);
				fwrite(&z, sizeof(float), 1, pFWrite);
			}
		}
		fclose(pFRead);
		fclose(pFWrite);
	}

	return true;
}

void CReadSrcFile::readtifftodem(std::string inputfilepath, std::string output)
{
	//myConvert::CConverter::TiffToDem(inputfile, output);
	int num_iamge_size = 0;

	float *pafScanblock1;  //开辟缓存区
	GDALDataset *poDataset;   //GDAL数据集
	GDALAllRegister();  //注册所有的驱动
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	poDataset = (GDALDataset *)GDALOpen(inputfilepath.c_str(), GA_ReadOnly);
	if (poDataset == NULL)
	{
		cout << "fail in open files!!!" << endl;
		return;
	}

	//获取图像波段
	GDALRasterBand *poBand1;
	int a=poDataset->GetRasterCount();
	poBand1 = poDataset->GetRasterBand(1);
	GDALDataType Gtype=poDataset->GetRasterBand(1)->GetRasterDataType();

	//获取图像的尺寸
	int nImgSizeX = poDataset->GetRasterXSize();
	int nImgSizeY = poDataset->GetRasterYSize();

	//获取坐标变换系数
	double trans[6];
	CPLErr aaa = poDataset->GetGeoTransform(trans);

	//定义一个写指针 将文件写成dem格式
	/*if (0 != access(output.c_str(), 0))
	{
		// if this folder not exist, create a new one.
		mkdir(output.c_str());   // 返回 0 表示创建成功，-1 表示失败
	}*/
	FILE *pwrite;
	float defaultvalue = -9999;

	pwrite = fopen(output.c_str(), "wb");
	if (!pwrite)
	{
		cout << "can not open file";
	}
	fwrite(&nImgSizeX, sizeof(unsigned), 1, pwrite);
	fwrite(&nImgSizeY, sizeof(unsigned), 1, pwrite);
	fwrite(&trans[0], sizeof(double), 1, pwrite);
	fwrite(&trans[3], sizeof(double), 1, pwrite);
	fwrite(&trans[1], sizeof(double), 1, pwrite);
	fwrite(&defaultvalue, sizeof(float), 1, pwrite);
	//读取图像高程数据
	double Xgeo, Ygeo;
	pafScanblock1 = (float *)CPLMalloc(sizeof(float)*(nImgSizeX)*(nImgSizeY));
	poBand1->RasterIO(GF_Read, 0, 0, nImgSizeX, nImgSizeY, pafScanblock1, nImgSizeX, nImgSizeY, GDT_Float32, 0, 0);
	int total_num = nImgSizeX * nImgSizeY;
	for (int i = 0; i < total_num; i++)
	{
		float elevation = *pafScanblock1;
		fwrite(&elevation, sizeof(float), 1, pwrite);
		pafScanblock1++;
	}
	fclose(pwrite);
	delete poDataset;
	return;
}

void CReadSrcFile::DEMtoASC(std::string input, std::string output)
{
	FILE *pFRead = fopen(input.c_str(), "rb");
	FILE *pFWrite = fopen(output.c_str(), "w");
	if (pFRead && pFWrite)
	{
		char str[50];
		int ncols, nrows;
		double wide;
		double XOri, YOri;
		float  nodata_value;
		if (pFRead)
		{
			fread(&ncols, sizeof(unsigned), 1, pFRead);
			fread(&nrows, sizeof(unsigned), 1, pFRead);
			fread(&XOri, sizeof(double), 1, pFRead);
			fread(&YOri, sizeof(double), 1, pFRead);
			fread(&wide, sizeof(double), 1, pFRead);
			fread(&nodata_value, sizeof(float), 1, pFRead);
			
			fprintf(pFWrite, "%u\t", ncols);
			fprintf(pFWrite, "%u\t", nrows);
			fprintf(pFWrite, "%f\t", XOri);
			fprintf(pFWrite, "%f\t",  YOri);
			fprintf(pFWrite, "%f\t",  wide);
			fprintf(pFWrite, "%f\t",  nodata_value);
			fprintf(pFWrite, "\n");
		}
		else
		{
			printf("读取数字高程文件: %s 失败！", input.c_str());
		}
		long basic = 2 * sizeof(unsigned) + 3 * sizeof(double) + sizeof(float);
		long offset=basic;
		
		for (int r = nrows - 1; r > -1; r--)
		{
			for (int c = 0; c < ncols; c++)
			{
				float z;
				fseek(pFRead, offset, SEEK_SET);
				fread(&z,sizeof(float),1,pFRead);
				fprintf(pFWrite,"%lf\t",z);
				offset = offset + sizeof(float);
			}
			fprintf(pFWrite, "\n");
		}
		fclose(pFRead);
		fclose(pFWrite);
	}

}

bool CReadSrcFile::ReadASCIIHeadfile(std::string filepath, HeadFile & headfile)
{
	FILE *m_pFDEM=NULL;
	m_pFDEM = fopen(filepath.c_str(), "rb");//读文件指针
	if (m_pFDEM)
	{
		fread(&headfile.ncols, sizeof(unsigned), 1, m_pFDEM);
		fread(&headfile.nrows, sizeof(unsigned), 1, m_pFDEM);
		fread(&headfile.xllcorner, sizeof(double), 1, m_pFDEM);
		fread(&headfile.yllcorner, sizeof(double), 1, m_pFDEM);
		fread(&headfile.cellsize, sizeof(double), 1, m_pFDEM);
		fread(&headfile.nodata_value, sizeof(float), 1, m_pFDEM);
		fclose(m_pFDEM);
	}
	else
	{
		AfxMessageBox(L"error file");
	}
	return true;
}

bool CReadSrcFile::ReadASCII(std::vector<float>& vec, std::string filepath)
{
	FILE *pdem=NULL;
	long basic_size= 2 * sizeof(unsigned) + 3 * sizeof(double) + sizeof(float);
	pdem = fopen(filepath.c_str(), "rb");
	if (pdem)
	{
		do
		{
			float z;

			fseek(pdem, basic_size, SEEK_SET);
			fread(&z, sizeof(float), 1, pdem);

			vec.push_back(z);
			basic_size = basic_size + sizeof(float);
		} while (!feof(pdem));

		fclose(pdem);
	}
	else
	{
		AfxMessageBox(L"error file");
	}	
	vec.pop_back();
	return true;
}

bool CReadSrcFile::ReadASCIIindex(std::string filepath, std::vector<long>& index)
{
	ifstream infile(filepath.c_str(), ios::in | ios::binary);
	if (!infile)
	{
		return false;
	}
	unsigned Pointnum=0;
	infile.read((char *)&Pointnum, sizeof(unsigned));
	index.resize(Pointnum);
	infile.read((char *)&index[0], sizeof(long)*Pointnum);
	infile.close();
	return true;
}



bool CReadSrcFile::ASCresample(std::string input, std::string output, double scale,int position)
{
	HeadFile headfile;
	CReadSrcFile::ReadAscHeadfile(input, headfile);
	double cellsize = scale * headfile.cellsize;
	int cols = headfile.ncols*headfile.cellsize / cellsize;
	double detcols = headfile.ncols*headfile.cellsize - cols * cellsize;//列的余量
	if (detcols>0.001)
	{
		cols=cols+1;
	}
	int rows = headfile.nrows*headfile.cellsize / cellsize;
	double detrows = headfile.nrows*headfile.cellsize - rows * cellsize;//行的余量
	if (rows>0.001)
	{
		rows = rows + 1;
	}
	ofstream ofile("D:\\output.asc");

	ofile.setf(ios::fixed);
	ofile << setprecision(4);
	ofile << "ncols" << "\t" << cols<<endl;
	ofile << "nrows" << "\t" << rows<<endl;
	ofile << "xllcorner" << "\t" << headfile.xllcorner<<endl;
	ofile << "yllcorner" << "\t" << headfile.yllcorner << endl;
	ofile << "cellsize" << "\t" << cellsize<<endl;
	ofile << "nodata_value" << "\t" << "-9999.0" << endl;

	for (int r=0;r<rows;r++)
		for (int c=0;c<cols;c++)
		{

		}
	ofile.close();

	return false;
}

void CReadSrcFile::TerrainBlock(int depth, std::string input, std::string output,HeadFile headfile)
{
	double subrows, subcols;
	int numsides = pow(2, depth-1);
	subrows = (double)(headfile.nrows - 1) / numsides;//记录每个小块的格子个数
	subcols = (double)(headfile.ncols - 1) / numsides;
	
	FILE *pread;
	pread = fopen(input.c_str(), "rb");
	for (int i=0;i<numsides;i++)
	   for (int j=0;j<numsides;j++)
	   {
		   long offset;
		   
		   int startrows, startcols;
		   double endrows, endcols;
		   string outfilename;
		   outfilename = output + "\\L"+to_string(depth-1)+"_"+to_string(i)+"_"+to_string(j);
		   startrows = (int)(subrows * i);
		   startcols = (int)(subcols * j);
		   endrows = subrows * (i + 1);
		   endcols = subcols * (j + 1);
		   if ((endrows-(int)endrows)>0.001)
		   {
			   endrows = (int)endrows + 1;
		   }
		   if ((endcols - (int)endcols) > 0.001)
		   {
			   endcols = (int)endcols + 1;
		   }
		   FILE  *pwrite;
		   pwrite = fopen(outfilename.c_str(), "wb");
		   unsigned c = endcols - startcols+1;
		   unsigned r = endrows - startrows+1;
		   double x = headfile.xllcorner + j*headfile.cellsize*subcols;
		   double y = headfile.yllcorner + (numsides -1- i)*headfile.cellsize*subrows;
		   fwrite(&c, sizeof(unsigned), 1, pwrite);
		   fwrite(&r, sizeof(unsigned), 1, pwrite);
		   fwrite(&x, sizeof(double), 1, pwrite);
		   fwrite(&y, sizeof(double), 1, pwrite);
		   fwrite(&headfile.cellsize, sizeof(double), 1, pwrite);
		   fwrite(&headfile.nodata_value, sizeof(float), 1, pwrite);
		   long basic_size = 2 * sizeof(unsigned) + 3 * sizeof(double) + sizeof(float) + startrows* headfile.ncols * sizeof(float) + startcols * sizeof(float);//每一个小块基础偏移量
		   for (int oi=0;oi<r;oi++)
			   for (int oj=0;oj<c;oj++)
			   {
				   float z;
				   offset = basic_size +oi*headfile.ncols* sizeof(float) +oj * sizeof(float);
				   fseek(pread, offset, SEEK_SET);
				   fread(&z, sizeof(float), 1, pread);
				   fwrite(&z, sizeof(float), 1, pwrite);
			   }
		   fclose(pwrite);
		  
	   }
	fclose(pread);
	
}

void CReadSrcFile::TerrainBlock(int depth, std::string input, std::string output, HeadFile headfile, HeadFile originHeadfile)
{
	float width, height;
	int numsides = pow(2, depth - 1);//num block
	width = originHeadfile.cellsize*(originHeadfile.ncols - 1)/numsides;
	height = originHeadfile.cellsize*(originHeadfile.nrows - 1)/numsides;
	

	FILE *pread;
	pread = fopen(input.c_str(), "rb");

	for (int i=0;i<numsides;i++)
	{
		for (int j=0;j<numsides;j++)
		{
			float StartBorderPointX = width * j;
			float StartBorderPointY = height * i;

			float EndBorderPointX = width * (j + 1);
			float EndBorderPointY = height *(i + 1);

			float startcol = StartBorderPointX / headfile.cellsize;
			float startrow = StartBorderPointY / headfile.cellsize;

			float endcol = EndBorderPointX / headfile.cellsize;
			float endrow = EndBorderPointY / headfile.cellsize;

			if (startrow-(int)startrow>0.01)
			{
				startrow = (int)startrow;
			}
			if (startcol-(int)startcol>0.01)
			{
				startcol = (int)startcol;
			}
			if (endrow-(int)endrow>0.01||endrow==headfile.nrows)
			{
				if (endrow>headfile.nrows-1)
				{
					endrow = headfile.nrows-1;
				}
				else
				{
					endrow = (int)endrow + 1;
				}
			}
			if (endcol-(int)endcol>0.01||endcol==headfile.ncols)
			{
				if (endcol>headfile.ncols-1)
				{
					endcol = headfile.ncols-1;
				}
				else
				{
					endcol = (int)endcol + 1;
				}
			}
			string outfilename;
			outfilename = output + "\\L" + to_string(depth - 1) + "_" + to_string(i) + "_" + to_string(j);

			FILE *pwrite;
			pwrite = fopen(outfilename.c_str(), "wb");
			unsigned c = endcol - startcol + 1;
			unsigned r = endrow - startrow + 1;

			double x = headfile.xllcorner + startcol*headfile.cellsize;
			double y = headfile.yllcorner + (headfile.nrows- 1 - endrow)*headfile.cellsize;
			fwrite(&c, sizeof(unsigned), 1, pwrite);
			fwrite(&r, sizeof(unsigned), 1, pwrite);
			fwrite(&x, sizeof(double), 1, pwrite);
			fwrite(&y, sizeof(double), 1, pwrite);
			fwrite(&headfile.cellsize, sizeof(double), 1, pwrite);
			fwrite(&headfile.nodata_value, sizeof(float), 1, pwrite);
			long basic_size = 2 * sizeof(unsigned) + 3 * sizeof(double) + sizeof(float) + startrow * headfile.ncols * sizeof(float) + startcol * sizeof(float);//每一个小块基础偏移量
			long offset;
			for (int oi = 0; oi < r; oi++)
				for (int oj = 0; oj < c; oj++)
				{
					float z;
					offset = basic_size + oi * headfile.ncols * sizeof(float) + oj * sizeof(float);
					fseek(pread, offset, SEEK_SET);
					fread(&z, sizeof(float), 1, pread);
					fwrite(&z, sizeof(float), 1, pwrite);
				}
			fclose(pwrite);
		}
	}
	fclose(pread);
}

void CReadSrcFile::TerrainChangeCellsize(std::string input, std::string output, double scale)
{
	HeadFile headfile;
	
	CReadSrcFile::ReadASCIIHeadfile(input.c_str(), headfile);
	double newcellsize = scale * headfile.cellsize;
	double rows = headfile.cellsize*(headfile.nrows-1) / newcellsize;//重采样后的行数
	double cols = headfile.cellsize*(headfile.ncols-1) / newcellsize;//列数
	if ((rows-(int)rows)>0.001)
	{
		rows = (int)rows + 1;//行数
	}
	if ((cols-(int)cols)>0.001)
	{
		cols = (int)cols + 1;//列数
	}
	FILE *pwrite;
	
	pwrite=fopen(output.c_str(), "wb");
	int wcols = cols;
	int wrows = rows;
	double ycol;
	ycol = headfile.yllcorner + headfile.cellsize*(headfile.nrows - 1) - (wrows - 1)*newcellsize;
	fwrite(&wcols, sizeof(unsigned), 1, pwrite);
	fwrite(&wrows, sizeof(unsigned), 1, pwrite);
	fwrite(&headfile.xllcorner, sizeof(double), 1, pwrite);
	fwrite(&ycol, sizeof(double), 1, pwrite);
	fwrite(&newcellsize, sizeof(double), 1, pwrite);
	fwrite(&headfile.nodata_value, sizeof(float), 1, pwrite);
	if (!pwrite)
	{
		return ;
	}
	for (int i=0;i<rows;i++)
	  for (int j=0;j<cols;j++)
	  {
		  float z;
		  z = CReadSrcFile::getheight(input, newcellsize*j, newcellsize*i, headfile);
		  fwrite(&z, sizeof(float), 1, pwrite);
	  }
	fclose(pwrite);
}

float CReadSrcFile::getheight(std::string input, double x, double y, HeadFile headfile)
{
	FILE *pread;
	pread = fopen(input.c_str(), "rb");
	if (!pread)
	{
		return 0.0f;
	}
	int cols = x / headfile.cellsize;
	int rows = y / headfile.cellsize;
	double detx = x - cols * headfile.cellsize;
	double dety = headfile.cellsize-y + rows * headfile.cellsize;
	float lu, ru, ld, rd;
	long basic_size = 2 * sizeof(unsigned) + 3 * sizeof(double) + sizeof(float);
	long b;
	b = basic_size + rows * (headfile.ncols) * sizeof(float) + (cols) * sizeof(float);
	fseek(pread, basic_size + rows*(headfile.ncols)*sizeof(float) + (cols)*sizeof(float), SEEK_SET);
	fread(&lu, sizeof(float), 1, pread);
	b = basic_size + rows * (headfile.ncols) * sizeof(float) + (cols + 1) * sizeof(float);
	fseek(pread, basic_size + rows*headfile.ncols*sizeof(float) + (cols+1)*sizeof(float), SEEK_SET);
	fread(&ru, sizeof(float), 1, pread);
	b = basic_size + (rows + 1)*(headfile.ncols) * sizeof(float) + (cols) * sizeof(float);
	fseek(pread, basic_size + (rows+1)*headfile.ncols*sizeof(float)+(cols) * sizeof(float), SEEK_SET);
	fread(&ld, sizeof(float), 1, pread);
	b = basic_size + (rows + 1)*(headfile.ncols) * sizeof(float) + (cols + 1) * sizeof(float);
	fseek(pread, basic_size + (rows+1)*headfile.ncols*sizeof(float)+(cols+1)*sizeof(float), SEEK_SET);
	fread(&rd, sizeof(float), 1, pread);

	float height=(1-detx/headfile.cellsize)*(1-dety/headfile.cellsize)*ld+(detx/headfile.cellsize)*(1-dety/headfile.cellsize)*rd+
		          (detx/headfile.cellsize)*(dety/headfile.cellsize)*ru+(1-detx/headfile.cellsize)*(dety/headfile.cellsize)*lu;
	fclose(pread);
	return height;
}

void CReadSrcFile::ProduceVirtualDem(std::string input,std::string &output)
{
	float *pafScanblock1;  //开辟缓存区
	GDALDataset *poDataset;   //GDAL数据集
	GDALAllRegister();  //注册所有的驱动
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	poDataset = (GDALDataset *)GDALOpen(input.c_str(), GA_ReadOnly);
	if (poDataset == NULL)
	{
		cout << "fail in open files!!!" << endl;
		return;
	}

	//获取图像波段
	GDALRasterBand *poBand1;
	poBand1 = poDataset->GetRasterBand(1);

	//获取图像的尺寸
	int nImgSizeX = poDataset->GetRasterXSize();
	int nImgSizeY = poDataset->GetRasterYSize();

	//获取坐标变换系数
	double trans[6];//0 top left x;3 top left y
	CPLErr aaa = poDataset->GetGeoTransform(trans);
	
	GDALClose(poDataset);

	int position=input.find_last_of("\\");
	int position_point = input.find_last_of(".");
	string program_name=input.substr(position+1,position_point-position-1);
	input=input.substr(0,position+1)+program_name+"_VirtualDEM.bin";

	output=input;
	//check out file is exist
	if (0!=_access(input.c_str(),0))
	{
		FILE *pFWrite = fopen(input.c_str(), "wb");
		if (!pFWrite)
		{
			return;
		}
		int ncols = nImgSizeX/40;
		int nrows = nImgSizeY/40;
		double XOri = trans[0];
		double YOri = trans[3];
		double wide = trans[1]*40;
		float nodata_value = -1;

		fwrite(&ncols, sizeof(unsigned), 1, pFWrite);
		fwrite(&nrows, sizeof(unsigned), 1, pFWrite);
		fwrite(&XOri, sizeof(double), 1, pFWrite);
		fwrite(&YOri, sizeof(double), 1, pFWrite);
		fwrite(&wide, sizeof(double), 1, pFWrite);
		fwrite(&nodata_value, sizeof(float), 1, pFWrite);

		float *pFloatArray=new float[ncols*nrows];
		/*memset(pFloatArray,-1,sizeof(pFloatArray));
		fwrite(pFloatArray,sizeof(float),ncols*nrows,pFWrite);*/
		
		for (int i = 0; i < ncols*nrows; i++)
		{
			float elevation = -1.0;
			fwrite(&elevation, sizeof(float), 1, pFWrite);
		}
		fclose(pFWrite);
		delete []pFloatArray;
	}
}
void CReadSrcFile::CreatePyramid(int depth,string inputfilepathname,std::string dir)
{
	int position_ = inputfilepathname.find_last_of("\\");
	string outputfilepath = inputfilepathname.substr(0, position_ + 1)+dir;

	if (0 != access(outputfilepath.c_str(), 0))
	{
		// if this folder not exist, create a new one.
		mkdir(outputfilepath.c_str());   // 返回 0 表示创建成功，-1 表示失败
	}

	int position_point = inputfilepathname.find_last_of(".");
	string extension_name = inputfilepathname.substr(position_point+1);
	string programname = inputfilepathname.substr(position_+1, position_point - position_-1);
	string outputfilename = outputfilepath+"\\"+programname;
	//ASCtoDem
	if (extension_name=="asc")
	{
		CReadSrcFile::ASCtoDEM(inputfilepathname, outputfilename);
	}
	else if(extension_name=="tif")
	{
		CReadSrcFile::readtifftodem(inputfilepathname, outputfilename);
	}
	else if(extension_name=="bin")
	{
		CopyFileA(inputfilepathname.c_str(),outputfilename.c_str(),FALSE);
	}
	else
	{
		AfxMessageBox(L"不支持的高程文件格式");
	}
	//tifftodem

	//修改栅格间距
	for (int i = 1; i < depth; i++)
	{
		string out;
		out = outputfilename + to_string(depth - 1 - i);
		CReadSrcFile::TerrainChangeCellsize(outputfilename, out, 2 * i);
	}
	string newname = outputfilename + to_string(depth - 1);
	rename(outputfilename.c_str(), newname.c_str());
	HeadFile oriheadfile;
	ReadASCIIHeadfile(newname, oriheadfile);
	//文件分块
	for (int j = 0; j < depth; j++)
	{
		string inname = outputfilename + to_string(j);
		HeadFile headfile;
		ReadASCIIHeadfile(inname, headfile);
		CReadSrcFile::TerrainBlock(j+1, inname, outputfilepath, headfile,oriheadfile);
	}
}
/***
* 遥感影像重采样   (要求影像必须有投影，否则走不通)
* @param pszSrcFile        输入文件的路径
* @param pszOutFile        写入的结果图像的路径
* @param eResample         采样模式，有五种，具体参见GDALResampleAlg定义，默认为双线性内插
* @param fResX             X转换采样比，默认大小为1.0，大于1图像变大，小于1表示图像缩小。数值上等于采样后图像的宽度和采样前图像宽度的比
* @param fResY             Y转换采样比，默认大小为1.0，大于1图像变大，小于1表示图像缩小。数值上等于采样后图像的高度和采样前图像高度的比
* @retrieve     0   成功
* @retrieve     -1  打开源文件失败
* @retrieve     -2  创建新文件失败
* @retrieve     -3  处理过程中出错
*/
int CReadSrcFile::ResampleGDAL(const char * pszSrcFile, const char * pszOutFile, float fResX, float fResY, GDALResampleAlg eResample)
{
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	GDALDataset *pDSrc = (GDALDataset *)GDALOpen(pszSrcFile, GA_ReadOnly);
	if (pDSrc == NULL)
	{
		return -1;
	}

	GDALDriver *pDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
	//GDALDriver *pDriver = GetGDALDriverManager()->GetDriverByName("JPEG");
	if (pDriver == NULL)
	{
		GDALClose((GDALDatasetH)pDSrc);
		return -2;
	}
	int width = pDSrc->GetRasterXSize();
	int height = pDSrc->GetRasterYSize();
	int nBandCount = pDSrc->GetRasterCount();
	GDALDataType dataType = pDSrc->GetRasterBand(1)->GetRasterDataType();

	char *pszSrcWKT = NULL;
	pszSrcWKT = const_cast<char *>(pDSrc->GetProjectionRef());

	//如果没有投影，人为设置一个    
	if (strlen(pszSrcWKT) <= 0)
	{
		OGRSpatialReference oSRS;
		oSRS.SetUTM(50, true);   //北半球  东经120度  
		oSRS.SetWellKnownGeogCS("WGS84");
		oSRS.exportToWkt(&pszSrcWKT);
	}

	void *hTransformArg;
	hTransformArg = GDALCreateGenImgProjTransformer((GDALDatasetH)pDSrc, pszSrcWKT, NULL, pszSrcWKT, FALSE, 0.0, 1);

	//(没有投影的影像到这里就走不通了)  
	if (hTransformArg == NULL)
	{
		GDALClose((GDALDatasetH)pDSrc);
		return -3;
	}

	double dGeoTrans[6] = { 0 };
	int nNewWidth = 0, nNewHeight = 0;
	if (GDALSuggestedWarpOutput((GDALDatasetH)pDSrc, GDALGenImgProjTransform, hTransformArg, dGeoTrans, &nNewWidth, &nNewHeight) != CE_None)
	{
		GDALClose((GDALDatasetH)pDSrc);
		return -3;
	}

	GDALDestroyGenImgProjTransformer(hTransformArg);

	//adfGeoTransform[0] /* top left x */  
	//adfGeoTransform[1] /* w-e pixel resolution */  
	//adfGeoTransform[2] /* rotation, 0 if image is "north up" */  
	//adfGeoTransform[3] /* top left y */  
	//adfGeoTransform[4] /* rotation, 0 if image is "north up" */  
	//adfGeoTransform[5] /* n-s pixel resolution */  

	dGeoTrans[1] = dGeoTrans[1] / fResX;
	dGeoTrans[5] = dGeoTrans[5] / fResY;
	nNewWidth = static_cast<int>(nNewWidth*fResX + 0.5);
	nNewHeight = static_cast<int>(nNewHeight*fResY + 0.5);

	//创建结果数据集  输出数据集
	GDALDataset *pDDst = pDriver->Create(pszOutFile, nNewWidth, nNewHeight, nBandCount, dataType, NULL);
	if (pDDst == NULL)
	{
		GDALClose((GDALDatasetH)pDSrc);
		return -2;
	}

	pDDst->SetProjection(pszSrcWKT);
	pDDst->SetGeoTransform(dGeoTrans);

	GDALWarpOptions *psWo = GDALCreateWarpOptions();//结构体对象

	//psWo->papszWarpOptions = CSLDuplicate(NULL);    
	psWo->eWorkingDataType = dataType;
	psWo->eResampleAlg = eResample;

	psWo->hSrcDS = (GDALDatasetH)pDSrc;
	psWo->hDstDS = (GDALDatasetH)pDDst;

	psWo->pfnTransformer = GDALGenImgProjTransform;
	psWo->pTransformerArg = GDALCreateGenImgProjTransformer((GDALDatasetH)pDSrc, pszSrcWKT, (GDALDatasetH)pDDst, pszSrcWKT, FALSE, 0.0, 1);;

	psWo->nBandCount = nBandCount;
	psWo->panSrcBands = (int *)CPLMalloc(nBandCount * sizeof(int));
	psWo->panDstBands = (int *)CPLMalloc(nBandCount * sizeof(int));
	for (int i = 0; i < nBandCount; i++)
	{
		psWo->panSrcBands[i] = i + 1;
		psWo->panDstBands[i] = i + 1;
	}

	GDALWarpOperation oWo;
	if (oWo.Initialize(psWo) != CE_None)
	{
		GDALClose((GDALDatasetH)pDSrc);
		GDALClose((GDALDatasetH)pDDst);
		return -3;
	}

	oWo.ChunkAndWarpImage(0, 0, nNewWidth, nNewHeight);
	GDALFlushCache(pDDst);

	GDALDestroyGenImgProjTransformer(psWo->pTransformerArg);
	GDALDestroyWarpOptions(psWo);
	GDALClose((GDALDatasetH)pDSrc);
	GDALClose((GDALDatasetH)pDDst);

	return 0;
}

int CReadSrcFile::ReSampleBlock(std::string programname, std::string inputfile, int xsize, int ysize, int layerclass, bool compress)
{
	int position_ = inputfile.find_last_of("\\");
	int position_point = inputfile.find_last_of(".");
	//string programname = inputfile.substr(position_ + 1, position_point - position_ - 1);
	string outputfile = inputfile.substr(0, position_) + "\\" + programname + to_string(layerclass - 1) + ".tif";
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	GDALDataset  *poDataset2;
	GDALDriver *poDriver2;
	//申明数据格式
	const char *pszFormat2 = "Gtiff";
	//驱动获取数据格式
	poDriver2 = GetGDALDriverManager()->GetDriverByName(pszFormat2);
	//读入数据
	GDALDataset *finput = (GDALDataset *)GDALOpen(inputfile.c_str(), GA_ReadOnly);
	if (finput == NULL)
	{
		printf("Open Img Failed:\n%s\n", inputfile);
	}
	//获取影像数据
	int ncols = finput->GetRasterXSize();
	int nrows = finput->GetRasterYSize();
	int nbands = finput->GetRasterCount();//波段数
	GDALDataType gBand = finput->GetRasterBand(1)->GetRasterDataType();
	int nbits = GDALGetDataTypeSize(gBand);//获取数据大小
	//对文件进行分块读取，块的划分原则
	int numrows, numcols;
	long long count = (long long)ncols * nrows*nbands;
	long long maxcount = (long long)RAM_RIZE * 1024 * 1024;
	bool need_block;
	if (count <= maxcount)//小文件
	{
		need_block = FALSE;
		numcols = ncols;
		numrows = nrows;
	}
	else//大文件
	{
		need_block = TRUE;
		int cols_d_rows = ncols / nrows;//列行比
		numrows = sqrt((RAM_RIZE * 1024 * 1024) / (cols_d_rows*nbands));
		numrows = (numrows / ysize)*ysize;
		numcols = cols_d_rows * numrows;
	}


	double GeoTransform[6];                     	//获取坐标信息
	finput->GetGeoTransform(GeoTransform);
	const char *sProRef = finput->GetProjectionRef();	//获取投影信息

	int isize = GDALGetDataTypeSize(gBand) / 8;
	double nodata;
	nodata = finput->GetRasterBand(1)->GetNoDataValue();

	int *pBand = new int[nbands];
	for (int gi = 0; gi < nbands; gi++)
	{
		pBand[gi] = gi + 1;
	}
	char **psz = NULL;
	if (compress == true)
	{

		psz = CSLSetNameValue(psz, "TILED", "YES");
		psz = CSLSetNameValue(psz, "COMPRESS", "LZW");
	}
	int newcols = ncols / xsize;
	int newrows = nrows / ysize;
	poDataset2 = poDriver2->Create(outputfile.c_str(), newcols, newrows, nbands, GDT_Byte, psz);//将输入文件 重采样 输出到文件


	int numx = ncols / numcols;
	bool flagx = FALSE;
	bool flagy = FALSE;
	int numy = nrows / numrows;
	if ((float)ncols / numcols - numx >= 0)
	{
		flagx = TRUE;
		numx = numx + 1;
	}
	if ((float)nrows / numrows - numy >= 0)
	{
		flagy = TRUE;
		numy = numy + 1;
	}
	//申请一块内存 用于读取数据
	if (need_block == TRUE)//需要分块
	{
		int sx = 0, sy = 0;
		for (int i = 0; i < numy; i++)
		{
			for (int j = 0; j < numx; j++)
			{
				if (i < numy - 1 && j < numx - 1)
				{
					clock_t start = clock();
					sy = i * numrows;
					sx = j * numcols;
					GByte *pImg = new GByte[(int)(numcols / xsize) * (int)(numrows / ysize) * nbands];
					memset(pImg, 0, (int)(numcols / xsize) * (int)(numrows / ysize) * nbands * sizeof(GByte));
					finput->RasterIO(GF_Read, sx, sy, numcols, numrows, pImg, numcols / xsize, numrows / ysize, gBand, nbands, pBand, isize*nbands, isize*nbands*numcols / xsize, isize);
					poDataset2->RasterIO(GF_Write, numcols / xsize * j, numrows / ysize * i, numcols / xsize, numrows / ysize, pImg, numcols / xsize, numrows / ysize, gBand, nbands, pBand, isize*nbands, isize*nbands*numcols / xsize, isize);
					delete[]pImg;
					pImg = NULL;
					clock_t end = clock();
					//cout << "time=" << ((double)end - start) / CLOCKS_PER_SEC << "\n";
				}

			}
		}
		if (flagx == TRUE)
		{
			for (int i = 0; i < numy - 1; i++)
			{
				sy = i * numrows;
				sx = (numx - 1) * numcols;
				GByte *pImg = new GByte[(ncols - (numx - 1)*numcols) * numrows * nbands];
				memset(pImg, 0, (ncols - (numx - 1)*numcols) * numrows * nbands * sizeof(GByte));
				finput->RasterIO(GF_Read, sx, sy, (ncols - (numx - 1)*numcols), numrows, pImg, (ncols - (numx - 1)*numcols) / xsize, numrows / ysize, gBand, nbands, pBand, isize*nbands, isize*nbands*(ncols - (numx - 1)*numcols) / xsize, isize);
				poDataset2->RasterIO(GF_Write, numcols*(numx - 1) / xsize, numrows / ysize * i, (ncols - (numx - 1)*numcols) / xsize, numrows / ysize, pImg, (ncols - (numx - 1)*numcols) / xsize, numrows / ysize, gBand, nbands, pBand, isize*nbands, isize*nbands*(ncols - (numx - 1)*numcols) / xsize, isize);
				delete[]pImg;
				pImg = NULL;
			}
		}
		if (flagy == TRUE)
		{
			for (int i = 0; i < numx - 1; i++)
			{
				sy = (numy - 1) * numrows;
				sx = i * numcols;
				GByte *pImg = new GByte[numcols * (nrows - numrows * (numy - 1)) * nbands];
				memset(pImg, 0, numcols * (nrows - numrows * (numy - 1)) * nbands * sizeof(GByte));
				finput->RasterIO(GF_Read, sx, sy, numcols, (nrows - numrows * (numy - 1)), pImg, numcols / xsize, (nrows - numrows * (numy - 1)) / ysize, gBand, nbands, pBand, isize*nbands, isize*nbands*numcols / xsize, isize);
				poDataset2->RasterIO(GF_Write, numcols*i / xsize, numrows*(numy - 1) / ysize, numcols / xsize, (nrows - (numy - 1)*numrows) / ysize, pImg, numcols / xsize, (nrows - (numy - 1)*numrows) / ysize, gBand, nbands, pBand, isize*nbands, isize*nbands*numcols / xsize, isize);
				delete[]pImg;
				pImg = NULL;
			}
		}
		if (flagx == TRUE && flagy == TRUE)
		{
			sy = (numy - 1)*numrows;
			sx = (numx - 1)*numcols;
			GByte *pImg = new GByte[(ncols - (numx - 1)*numcols) * (nrows - numrows * (numy - 1)) * nbands];
			memset(pImg, 0, (ncols - (numx - 1)*numcols) * (nrows - numrows * (numy - 1)) * nbands * sizeof(GByte));
			finput->RasterIO(GF_Read, sx, sy, (ncols - (numx - 1)*numcols), (nrows - numrows * (numy - 1)), pImg, (ncols - (numx - 1)*numcols) / xsize, (nrows - numrows * (numy - 1)) / ysize, gBand, nbands, pBand, isize*nbands, isize*nbands*(ncols - (numx - 1)*numcols) / xsize, isize);
			poDataset2->RasterIO(GF_Write, numcols*(numx - 1) / xsize, numrows*(numy - 1) / ysize, (ncols - (numx - 1)*numcols) / xsize, (nrows - (numy - 1)*numrows) / ysize, pImg, (ncols - (numx - 1)*numcols) / xsize, (nrows - (numy - 1)*numrows) / ysize, gBand, nbands, pBand, isize*nbands, isize*nbands*(ncols - (numx - 1)*numcols) / xsize, isize);
			delete[]pImg;
			pImg = NULL;
		}
	}
	else//一次读入内存
	{
		GByte *pImg = new GByte[numcols * numrows * nbands];
		memset(pImg, 0, numcols * numrows * nbands * sizeof(GByte));
		finput->RasterIO(GF_Read, 0, 0, numcols, numrows, pImg, numcols / xsize, numrows / ysize, gBand, nbands, pBand, isize*nbands, isize*nbands*numcols / xsize, isize);
		poDataset2->RasterIO(GF_Write, 0, 0, numcols / xsize, numrows / ysize, pImg, numcols / xsize, numrows / ysize, gBand, nbands, pBand, isize*nbands, isize*nbands*numcols / xsize, isize);
		delete[]pImg;
		pImg = NULL;
	}


	GeoTransform[1] = GeoTransform[1] * xsize;
	GeoTransform[5] = GeoTransform[5] * ysize;
	poDataset2->GetRasterBand(1)->SetNoDataValue(nodata);
	//设置图像坐标系
	poDataset2->SetGeoTransform(GeoTransform);
	//设置投影方式
	poDataset2->SetProjection(sProRef);
	//cout << "分块成功" << endl;
	delete finput;
	delete poDataset2;
	return 0;
}

int CReadSrcFile::ImageDividedBlock(std::string inputfile, int layerClass, bool compress)
{
	//计算每层划分的块数
	int countNum = pow(2, layerClass);
	//对文件名进行处理、每块文件进行编号
	int position_ = inputfile.find_last_of("\\");

	string outputfilepath = inputfile.substr(0, position_ + 1) + "img";
	if (0 != _access(outputfilepath.c_str(), 0))
	{
		_mkdir(outputfilepath.c_str());
	}

	int position_point = inputfile.find_first_of(".");
	string filename = inputfile.substr(position_ + 1, position_point - position_ - 1);
	//确定分块大小
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	GDALDataset *pinput;
	GDALDriver *pdrvier;
	const char *pszFormat2 = "Gtiff";
	pdrvier = GetGDALDriverManager()->GetDriverByName(pszFormat2);
	pinput = (GDALDataset*)GDALOpen(inputfile.c_str(), GA_ReadOnly);

	if (pinput == NULL)
	{
		printf("Open Img Failed:\n%s\n", inputfile);
	}
	char **psz = NULL;
	if (compress == true)
	{
		psz = CSLSetNameValue(psz, "TILED", "YES");
		psz = CSLSetNameValue(psz, "COMPRESS", "LZW");
	}

	int nCols = pinput->GetRasterXSize();         //获取影像信息
	int nRows = pinput->GetRasterYSize();
	int nBands = pinput->GetRasterCount();
	GDALDataType gBand = pinput->GetRasterBand(1)->GetRasterDataType();
	int nBits = GDALGetDataTypeSize(gBand);

	double GeoTransform[6];                     	//获取坐标信息
	pinput->GetGeoTransform(GeoTransform);
	const char *sProRef = pinput->GetProjectionRef();	//获取投影信息

	int *pBand = new int[nBands];
	for (int gi = 0; gi < nBands; gi++)
	{
		pBand[gi] = gi + 1;
	}
	int isize = GDALGetDataTypeSize(GDT_Byte) / 8;

	double nodata;

	nodata = pinput->GetRasterBand(1)->GetNoDataValue();



	//一次IO可以切分几块 每块的行列数
	int numcols = nCols / countNum;
	int numrows = nRows / countNum;

	if ((float)nCols / countNum - numcols > 0.001)
	{
		numcols = (int)numcols + 1;
	}
	if ((float)nRows / countNum - numrows > 0.001)
	{
		numrows = (int)numrows + 1;
	}
	int read_Ram_num = (long long)RAM_RIZE * 1024 * 1024 / (numrows*numcols*nBands*nBits / 8);
	read_Ram_num = read_Ram_num / countNum;//一次可以读取的行数
	if (read_Ram_num == 0)
	{
		return 1;//内存过小，逐一块读取
	}
	int Block_num = 0;//用于给块编号
	int total_read_num = countNum / read_Ram_num;//读取的次数
	int residue_block_num = countNum % read_Ram_num;//剩余的块数
	for (int i = 0; i < total_read_num; i++)
	{

		GByte *pImg = new GByte[read_Ram_num * numrows *nCols*nBands];       //存储开操作时输入影像
		memset(pImg, 0, read_Ram_num * numrows *nCols*nBands);
		pinput->RasterIO(GF_Read, 0, i*read_Ram_num*numrows, nCols, read_Ram_num*numrows, pImg, nCols, read_Ram_num*numrows, GDT_Byte, nBands, pBand, isize*nBands, isize*nBands*nCols, isize);
		//resavedata(pImg, nCols, read_Ram_num*numrows, "C:\\Users\\xyh19\\Desktop\\example\\TiffBlock\\x64\\Debug\\test.tif");
		for (int j = 0; j < countNum*read_Ram_num; j++)
		{
			int rownum, colnum;
			rownum = Block_num / countNum;//确定写出的小块的位置
			colnum = Block_num % countNum;
			GByte *poimg = new GByte[numrows*numcols*nBands];

			memset(poimg, 0, numrows*numcols*nBands);
			for (int c = 0; c < numrows; c++)
			{
				int n = (j / countNum)*numrows*nCols*nBands + (j%countNum)*numcols*nBands;
				GByte *psrc = pImg + n + c * nCols*nBands;

				memcpy(poimg + c * numcols*nBands, psrc, numcols*nBands);
			}

			//printf("%d,%d,%d,%d,%d\n", poimg[0], poimg[2495 * 3], poimg[2495 * 2 * 3], poimg[2495 * 3 * 3], poimg[2495 * 3 * 3+1]);
			//printf("%d,%d,%d,%d,%d\n", pImg[0], pImg[4989 * 3], pImg[4989*3*2], pImg[4989 * 3 * 3], pImg[4989 * 3 * 3+1]);

		//resavedata(poimg, numcols, numrows, "C:\\Users\\xyh19\\Desktop\\example\\TiffBlock\\x64\\Debug\\test.tif");//用于调试
			string blockfilename = outputfilepath + "\\" + "L" + to_string(layerClass) + "_" + to_string(rownum) + "_" + to_string(colnum) + ".tif";
			GDALDataset *poutput;

			poutput = pdrvier->Create(blockfilename.c_str(), numcols, numrows, nBands, GDT_Byte, psz);
			//申明写出图像路径

			poutput->RasterIO(GF_Write, 0, 0, numcols, numrows, poimg, numcols, numrows, GDT_Byte, nBands, pBand, isize*nBands, isize*nBands*numcols, isize);
			Block_num++;
			poutput->GetRasterBand(1)->SetNoDataValue(nodata);
			//设置图像坐标系
			double tmpGeoTransform[6]={ 0 };
			tmpGeoTransform[0] = GeoTransform[0] + colnum * numcols*tmpGeoTransform[1];
			tmpGeoTransform[1] = GeoTransform[1];
			tmpGeoTransform[2] = GeoTransform[2];
			tmpGeoTransform[3] = GeoTransform[3] + rownum * numrows*tmpGeoTransform[5];
			tmpGeoTransform[4] = GeoTransform[4];
			tmpGeoTransform[5] = GeoTransform[5];
			poutput->SetGeoTransform(tmpGeoTransform);
			//设置投影方式
			poutput->SetProjection(sProRef);
			GDALClose(poutput);
			delete[]poimg;
			poimg = NULL;
		}
		delete[]pImg;
		pImg = NULL;
	}
	if (total_read_num > 0 && residue_block_num > 0)
	{
		GByte *pImg = new GByte[residue_block_num*numrows*nCols*nBands];
		memset(pImg, 0, residue_block_num*numrows*nCols*nBands);
		pinput->RasterIO(GF_Read, 0, nRows - residue_block_num * numrows, nCols, residue_block_num*numrows, pImg, nCols, residue_block_num*numrows, GDT_Byte, nBands, pBand, isize*nBands, isize*nBands*nCols, isize);
		//resavedata(pImg, nCols, residue_block_num*numrows, "C:\\Users\\xyh19\\Desktop\\example\\TiffBlock\\x64\\Debug\\test.tif");
		for (int j = 0; j < residue_block_num*countNum; j++)
		{
			int rownum, colnum;
			rownum = Block_num / countNum;//确定写出的小块的位置
			colnum = Block_num % countNum;
			GByte *poimg = new GByte[numrows*numcols*nBands];//申请空间，用于写出小块
			memset(poimg, 0, numcols*numrows*nBands);
			for (int c = 0; c < numrows; c++)
			{
				GByte *psrc = pImg + (j / countNum)*nCols*numrows*nBands + (j%countNum)*numcols*nBands + c * nCols*nBands;
				memcpy(poimg + c * numcols*nBands, psrc, numcols*nBands);
			}

			string blockfilename = outputfilepath + "\\" + "L" + to_string(layerClass) + "_" + to_string(rownum) + "_" + to_string(colnum) + ".tif";
			GDALDataset *poutput;

			poutput = pdrvier->Create(blockfilename.c_str(), numcols, numrows, nBands, GDT_Byte, psz);
			//申明写出图像路径

			poutput->RasterIO(GF_Write, 0, 0, numcols, numrows, poimg, numcols, numrows, GDT_Byte, nBands, pBand, isize*nBands, isize*nBands*numcols, isize);
			Block_num++;
			poutput->GetRasterBand(1)->SetNoDataValue(nodata);
			//设置图像坐标系
			double tmpGeoTransform[6]={ 0 };
			tmpGeoTransform[0] = GeoTransform[0] + colnum * numcols*tmpGeoTransform[1];
			tmpGeoTransform[1] = GeoTransform[1];
			tmpGeoTransform[2] = GeoTransform[2];
			tmpGeoTransform[3] = GeoTransform[3] + rownum * numrows*tmpGeoTransform[5];
			tmpGeoTransform[4] = GeoTransform[4];
			tmpGeoTransform[5] = GeoTransform[5];
			poutput->SetGeoTransform(tmpGeoTransform);
			//设置投影方式
			poutput->SetProjection(sProRef);
			//cout << "分块成功" << endl;
			GDALClose(poutput);
			//delete poutput;
			delete[]poimg;
			poimg = NULL;
		}
	}
	else//一次读取
	{
		GByte *pImg = new GByte[nRows*nCols*nBands];
		memset(pImg, 0, nRows*nCols*nBands);
		pinput->RasterIO(GF_Read, 0, 0, nCols, nRows, pImg, nCols, nRows, GDT_Byte, nBands, pBand, isize*nBands, isize*nBands*nCols, isize);

		float fncols = (float)nCols / countNum;
		float fnrows = (float)nRows / countNum;

		for (int i = 0; i < countNum*countNum; i++)
		{
			int rownum, colnum;
			rownum = Block_num / countNum;//确定写出的小块的位置
			colnum = Block_num % countNum;
			int startcol = fncols * colnum;
			int startrow = fnrows * rownum;
			int endcol = fncols * (colnum + 1);
			int endrow = fnrows * (rownum + 1);
			GByte *poimg = new GByte[(endrow - startrow)*(endcol - startcol)*nBands];//申请空间，用于写出小块
			memset(poimg, 0, (endrow - startrow)*(endcol - startcol)*nBands);
			for (int c = 0; c < endrow - startrow; c++)
			{
				GByte *psrc = pImg + startrow * nCols*nBands + colnum * (endcol - startcol)*nBands + c * nCols*nBands;
				memcpy(poimg + c * (endcol - startcol)*nBands, psrc, (endcol - startcol)*nBands);
			}
			string blockfilename = outputfilepath + "\\" + "L" + to_string(layerClass) + "_" + to_string(rownum) + "_" + to_string(colnum) + ".tif";
			GDALDataset *poutput;
			poutput = pdrvier->Create(blockfilename.c_str(), endcol - startcol, endrow - startrow, nBands, GDT_Byte, psz);
			poutput->RasterIO(GF_Write, 0, 0, endcol - startcol, endrow - startrow, poimg, endcol - startcol, endrow - startrow, GDT_Byte, nBands, pBand, isize*nBands, isize*nBands*(endcol - startcol), isize);
			Block_num++;
			poutput->GetRasterBand(1)->SetNoDataValue(nodata);
			//设置图像坐标系
			double tmpGeoTransform[6]={ 0 };
			tmpGeoTransform[0] = GeoTransform[0] + colnum * numcols*tmpGeoTransform[1];
			tmpGeoTransform[1] = GeoTransform[1];
			tmpGeoTransform[2] = GeoTransform[2];
			tmpGeoTransform[3] = GeoTransform[3] + rownum * numrows*tmpGeoTransform[5];
			tmpGeoTransform[4] = GeoTransform[4];
			tmpGeoTransform[5] = GeoTransform[5];
			poutput->SetGeoTransform(tmpGeoTransform);
			//设置投影方式
			poutput->SetProjection(sProRef);
			GDALClose(poutput);
			delete[]poimg;
			poimg = NULL;
		}
	}
	GDALClose(pinput);
	return 0;
}

void CReadSrcFile::createImagePyramid(std::string inputfile, int layer, bool compress)
{
	int position_ = inputfile.find_last_of("\\");
	int position_point = inputfile.find_last_of(".");

	string programname = inputfile.substr(position_ + 1, position_point - position_ - 1);
	string path = inputfile.substr(0, position_);
	string filename;
	filename = inputfile;
	for (int i = 1; i < layer; i++)
	{
		//clock_t start = clock();
		ReSampleBlock(programname, filename, pow(2, 1), pow(2, 1), layer - i, compress);
		filename = path + "\\" + programname + to_string(layer - i - 1) + ".tif";
		//clock_t end = clock();
		//printf("第%d次重采样时间：%f\n", i, ((float)end - start) / CLOCKS_PER_SEC);
	}
	//string newname = path + "\\" + programname + to_string(layer - 1)+".tif";
	//rename(inputfile.c_str(),newname.c_str());

	//分块
	for (int i = 1; i < layer - 1; i++)
	{
		string Imfilepath = path + "\\" + programname + to_string(i) + ".tif";
		ImageDividedBlock(Imfilepath, i, compress);
	}

	ImageDividedBlock(inputfile, layer - 1, compress);

	string oldfile = path + "\\" + programname + to_string(0) + ".tif";
	string newfile = path + "\\" + "img\\" + "L0_0_0.tif";

	CopyFileA(oldfile.c_str(),newfile.c_str(),FALSE);
	
	
}

bool CReadSrcFile::ReadPicAndHeiPath(std::string & filepath_pic, std::string & filepath_height, int & layer)
{
	HMODULE handle = GetModuleHandle(L"AdskTerrainModule.arx");
	wchar_t strModule[256];
	GetModuleFileName(handle, strModule, 256);
	std::string path = ConvertString::wchar2char(strModule);
	int position_ = path.find_last_of("\\");
	path = path.substr(0, position_);
	path = path + "\\filepathname.txt";

	ifstream in(path);
	if (in != NULL)
	{
		in >> filepath_pic;
		in >> filepath_height;
		in >> layer;
		in.close();
		return true;
	}
	return false;
}


