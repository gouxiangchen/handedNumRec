#ifndef NEURON_H_
#define NEURON_H_

#include <vector>
#include "constant.h"
#include "data.h"


class neuronLayer;

class neuronCell
{
public:
    neuronCell(int _preCellsNum);
    double calculateOutput(const neuronLayer& nl);
    double error;
    double output;
    double getErrorFromNext(const neuronLayer& nl, int index);
    void updateWeight(const neuronLayer &nl);
    std::vector<double> weight;
private:
    
    
    int preCellsNum;
    double activateFunc(double activation, double response);
    double backActive(double x);
};

class neuronLayer
{
public:
    neuronLayer(int _cellsNum, int _preCellsNUm, int _outputNum);   //hidden layer
    neuronLayer(int _outputNum);  //input layer
    neuronLayer(int _preCellsNum,int flag);  //output layer
    // void train();
    
    std::vector<double> output;
    std::vector<neuronCell> cells;
    int cellsNum;
    int preCellsNum;
private:
    
    
    int afterNum;
    // double * preOutput;
    
    // std::vector<double> weight;
    
};

class neuronNet
{
public:
    neuronNet(int _hidenLayerNum);
    void train();
    void init();
    void saveWeight();
    void readWeight();
    int recInputNum(std::string imgFileName);
private:
    std::vector<neuronLayer> layers;
    std::vector<data> vd;
    void getFiles(const char* path, std::vector<std::string>& files);
    double target[10];
};



#endif