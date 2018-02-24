#ifndef DATA_H_
#define DATA_H_

#include "map_pgm.h"
#include "util.h"
#include "constant.h"
#include <stdio.h>


struct data
{
    double vec[imgLength];
    int label;
    data (const Pgm_map & p)
    {
        label = p.label;
        int i = 0;
        for (int x = 0; x < p.getSizeX(); x++)
        {
            for (int y = 0; y< p.getSizeY(); y++)
            {
                //添加噪声
                vec[i++] = 1 - p.getGridMap2D()[x][y] + randomDouble() * 0.1;
                // std::cout << vec[i++] << " ";
            }
        }
        //memset(vec,0,784);

        //print map
        // for (int m = 0; m < imgLength; m ++)
        // {
        //     std::cout << vec[m] << " ";
        // }
        // std::cout << std::endl;
        // fgetc(stdin);
    }
    data (const Pgm_map & p,int flag)
    {
        label = p.label;
        int i = 0;
        for (int x = 0; x < p.getSizeX(); x++)
        {
            for (int y = 0; y< p.getSizeY(); y++)
            {
                //添加噪声
                vec[i++] = 1 - p.getGridMap2D()[x][y];
                // std::cout << vec[i++] << " ";
            }
        }
        //memset(vec,0,784);

        //print map
        // for (int m = 0; m < imgLength; m ++)
        // {
        //     std::cout << vec[m] << " ";
        // }
        // std::cout << std::endl;
        // fgetc(stdin);
    }
    data & operator = (const data &d)
    {
        label = d.label;
        memcpy(vec, d.vec, imgLength * sizeof(double));
        return *this;
    }
    data (const data& d)
    {
        label = d.label;
        memcpy(vec, d.vec, imgLength * sizeof(double));
    }
    void showData()
    {
        for(int i = 0; i < imgLength; i++)
        {
            std::cout << vec[i] << " ";
        }
        std::cout << std::endl;
    }
};


#endif