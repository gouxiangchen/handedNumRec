#include "map_pgm.h"

Pgm_map::Pgm_map(const char * filename):isopen(false)
{
	using namespace cv;
	using namespace std;
	basemap= imread(filename,0);
	// cout << filename << endl;
	// cout << filename[11];
	


	label = filename[37] - 48;

	// cout << filename << endl;
	// cout << (int)filename[37] << endl;
	// cout << label << endl;
	// fgetc(stdin);

	if (!( basemap.cols >0 && basemap.rows > 0 ))
	{
		sizeX=0;
		sizeY=0;
	}
	else
	{
		isopen= true;
		sizeX=basemap.cols;
		sizeY=basemap.rows;
	}
}

bool Pgm_map::transformMapToGrid()
{
	using namespace cv;
	if (!isopen)
	{
		return false;
	}
	else
	{
		gridmap2D= new int * [ sizeX ];
		for (int x=0;x<sizeX;x++)
		{
			gridmap2D[x]=new int [sizeY];
		}
		int index=0;
		for (int y=0;y<sizeY;y++)
		{
			for(int x=0;x<sizeX;x++)
			{
				if ((  basemap.at<unsigned char>(y,x)) > 240)
					gridmap2D[x][y]=0;
				else
					gridmap2D[x][y]=1;
			}
		}
		return true;
	}
}

int ** Pgm_map::getGridMap2D() const 
{
	return gridmap2D;
}

const int Pgm_map::getSizeX() const
{
	return sizeX;
}

const int Pgm_map::getSizeY() const
{
	return sizeY;
}

Pgm_map::~Pgm_map()
{
	if (isopen)
	{
		for (int x=0;x<sizeX;x++)
		{
			delete [] gridmap2D[x];
		}
		delete [] gridmap2D;
	}

}

cv::Mat Pgm_map::getImg()
{
	return basemap;
}
