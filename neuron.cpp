#include "neuron.h"
#include <math.h>
#include "util.h"
#include <sys/stat.h>
#include <dirent.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

//neuronCell define
neuronCell::neuronCell(int _preCellsNum)
{
    preCellsNum = _preCellsNum;
    error = 9999.;
    weight.reserve(preCellsNum + 2);
    output = 0;
    for(int i = 0; i < preCellsNum; i++)        //initial random weight
    {
        weight.push_back(randomDouble());
    }
    weight.push_back(randomDouble());   //output weight


    // if(weight.size() > 2)
    // {
    //     for(int i = 0; i < preCellsNum; i++)        //initial random weight
    //     {
    //         std::cout << weight.at(i) << " ";
    //     }
    //     std::cout << weight.back() << std::endl;
    //     fgetc(stdin);
    // }

    

}

double neuronCell::calculateOutput(const neuronLayer & nl)
{
    if(nl.cellsNum != preCellsNum)
    {
        return 0.;
    }
    else 
    {
        int i;
        for(i = 0; i < preCellsNum; i++)
        {
            // std::cout << nl.output.at(i) << " * " << weight.at(i) << " " << output << " "; 
            output += nl.cells.at(i).output * weight.at(i); 
            // fgetc(stdin);
        }
        // std::cout << output << std::endl;
        // output += weight.at(i);    //output bias
        // std::cout << output << std::endl;

        output = activateFunc(output,0.7);
        // std::cout << output << std::endl;
        // fgetc(stdin);
        return output;

    }
}

double neuronCell::activateFunc(double activation, double response)    //激活函数 response 用于缩放参数
{
    // std::cout << "activation:" << activation << std::endl;
    // fgetc(stdin);
    return (1.0 / (1.0 + exp(-activation * response)));
}

double neuronCell::getErrorFromNext(const neuronLayer &nl, int index)
{
    error = 0;
    
    for(int i = 0 ; i < nl.cellsNum; i++)
    {
        const neuronCell & nc = nl.cells.at(i);
        error += nc.error * nc.weight.at(index);
    }
    return error;
}

double neuronCell::backActive(double x)
{
    return x * (1 - x);
}

void neuronCell::updateWeight(const neuronLayer &nl)
{
    int i;
    for(i = 0 ; i < preCellsNum; i++)
    {
        // std::cout << "error : " << error << " output : " << output << " preoutput : " << nl.cells.at(i).output <<std::endl;
        double dw = error * backActive(output) * learnRate * nl.cells.at(i).output;
        // std::cout << "dw is " << dw << std::endl;
        weight.at(i) += dw;
    }
    weight.at(i) += error * backActive(output) * learnRate;
    // fgetc(stdin);
}

//neuronLayer define

neuronLayer::neuronLayer(int _cellsNum, int _preCellsNum, int _afterNum)    //hidden layer
        :cellsNum(_cellsNum),preCellsNum(_preCellsNum),afterNum(_afterNum)
{
    cells.reserve(cellsNum + 2);   //当前层的参数取决于前一层输出个数
    output.reserve(cellsNum + 2);      //当前层的输出参数取决于当前层的神经细胞个数
    for (int i = 0; i < cellsNum; i++)
    {
        cells.push_back(neuronCell(preCellsNum));
        output.push_back(1.);
    }

}





//neuronNet define

neuronNet::neuronNet(int _hiddenLayerNum)
{
    layers.push_back(neuronLayer(imgLength,0,hiddenLayerCells));        //input layer
    std::cout << "==== input layer push done! ====" << std::endl;
    int preNum = imgLength;
    for(int i = 0; i < 1; /*_hiddenLayerNum;*/ i++)     //暂时只搞一层
    {
        layers.push_back(neuronLayer(hiddenLayerCells,preNum,10));
    }
    std::cout << "==== hidden layer push done! ====" << std::endl;
    layers.push_back(neuronLayer(10,hiddenLayerCells,0));           //output layer
    std::cout << "==== output layer push done! ====" << std::endl;
    vd.reserve(40000);
    
}

void neuronNet::getFiles(const char* path, std::vector<std::string>& files){

    using namespace std;
    const string path0 = path;
    DIR* pDir;
    struct dirent* ptr;

    struct stat s;
    lstat(path, &s);

    if(!S_ISDIR(s.st_mode)){
        cout << "not a valid directory: " << path << endl;
        return;
    }

    if(!(pDir = opendir(path))){
        cout << "opendir error: " << path << endl;
        return;
    }
    int i = 0;
    string subFile;
    while((ptr = readdir(pDir)) != 0){
        subFile = ptr -> d_name;
        if(subFile == "." || subFile == "..")
            continue;
        subFile = subFile;
        //cout << ++i << ": " << subFile << endl;
        files.push_back(subFile);
    }
    closedir(pDir);

}

void neuronNet::init()
{
    using namespace std;
    vector<string> trainFiles;
    trainFiles.reserve(40000);
    getFiles("/home/chen/knn-recgonizer/mnist_data",trainFiles);
    
    for(auto &s : trainFiles)
    {
        string filepath = "/home/chen/knn-recgonizer/mnist_data/"; 
        filepath.append(s);   
        Pgm_map p(filepath.c_str());
        p.transformMapToGrid();
        data d(p);
        
        // cout << d.label << endl;
        
        vd.push_back(d);

        // cout << vd.back().label << endl;
        // fgetc(stdin);

        
    }
    std::cout << "==== img data load done! ==== " << vd.size() << std::endl;

    
    
}

void neuronNet::train()
{
    using namespace std;
    int numOfTrain = 0;
    int numRight = 0;
    double sumError = 0;

    // output init weight
    // for ( int i = 0; i < layers.back().cells.size(); i++)
    // {
    //     for(int j = 0; j < layers.back().cells.at(i).weight.size(); j ++)
    //     {
    //         cout << layers.back().cells.at(i).weight.at(j) << " ";
    //     }
    //     cout << endl;
    // }
    // fgetc(stdin);
    
    for (auto & d : vd)
    {
        neuronLayer & input = layers.front();   //输入层随读入的图片变化而变化
        // std::cout << "input layer " << input.cellsNum << std::endl;
        for (int i = 0; i < imgLength; i++ )
        {
            input.cells.at(i).output = d.vec[i];
            input.output.at(i) = d.vec[i];

            // std::cout << "==== input layer output init " << i << " push done! ====" << std::endl;
            // cout << input.output.at(i) << " ";
        }
        // cout << endl;   //输入层打印结果
        // fgetc(stdin);
        neuronLayer & output = layers.back();   
        
        std::memset(target, 0, 10 * sizeof(double) );   //输出层随读入的标签变化而变化
        target[d.label] = 1;
        // cout << "recent label " << d.label << endl;
        //预期结果打印
        // for(int i = 0; i < 10; i++)
        // {
        //     cout << target[i] << " ";
        // }
        // cout << endl;
        // fgetc(stdin);


        neuronLayer & hiddenLayer = layers.at(1);
        
        for (int i = 0; i < hiddenLayerCells; i++ )
        {
            double d = hiddenLayer.cells.at(i).calculateOutput(input);
            // cout << d << " ";
            
        }
        // cout << endl;
        for (int i = 0; i < 10; i++ )
        {
            double d = output.cells.at(i).calculateOutput(hiddenLayer);
            // cout << d << " ";
        }
        // cout << endl;
        // fgetc(stdin);

        //计算output层的误差
        for(int i = 0; i < 10; i++)
        {
            output.cells.at(i).error = target[i] - output.cells.at(i).output;

            // cout << output.cells.at(i).error << " ";
        }
        // cout << endl;
        // fgetc(stdin);


        sumError = 0;
        // cout << output.cells.size() << endl;
        for (auto &d : output.cells)
        {
            // cout << d.error << " ";
            sumError += d.error * d.error;
        }
        // cout << endl;
        // cout << "recent error : " << sumError <<endl;
        

        // fgetc(stdin);

        // if(sumError < expectedError)
        // {
            
        //     break;
        // }
        //隐含层误差
        for (int i = 0; i < hiddenLayerCells; i ++)
        {
            double e = hiddenLayer.cells.at(i).getErrorFromNext(output,i);
            // cout << e << " ";
        }
        // cout << endl;
        //输入层误差
        for(int i = 0; i < imgLength; i++)
        {
            double e = input.cells.at(i).getErrorFromNext(hiddenLayer,i);
            // cout << e << " ";
        }
        // cout << endl;
        // fgetc(stdin);
        //更新权重weight
        for(int i = 0 ; i < 10; i++)
        {
            output.cells.at(i).updateWeight(hiddenLayer);   
        }
        // cout << "hidden prenum : " << hiddenLayer.preCellsNum << endl;
        for(int i = 0; i < hiddenLayerCells; i++)
        {
            hiddenLayer.cells.at(i).updateWeight(input);
        }
        numOfTrain ++;
    }
        




    std::cout << "sumError is " << sumError << std::endl;
    // std::cout << "total used img " << numOfTrain << std::endl;
}

void neuronNet::saveWeight()
{
    using namespace std;
    ofstream fout;
    fout.open("model.txt");
    for (int i = 0; i < hiddenLayerCells; i ++)
    {
        for(int j = 0; j < imgLength; j++)
        {
            fout << layers.at(1).cells.at(i).weight.at(j);
            fout << " ";
        }
        fout << "\n";
        
    }
    // std::cout << layers.back().cells.size() << std::endl;
    // std::cout << layers.back().cells.at(1).weight.size() << std::endl;
    for (int i = 0; i < 10; i ++ )
    {
        for(int j = 0; j < hiddenLayerCells; j ++)
        {
            fout << layers.back().cells.at(i).weight.at(j);
            fout << " ";
        }
        fout << "\n";
    }
    
    fout.close();
    std::cout << "write file finish!" << std::endl;
}

void neuronNet::readWeight()
{
    using namespace std;
    ifstream fin;
    fin.open("model.txt");
    for (int i = 0; i < hiddenLayerCells; i ++)
    {
        for(int j = 0; j < imgLength; j++)
        {
            fin >> layers.at(1).cells.at(i).weight.at(j);
            
        }
        
        
    }
    for (int i = 0; i < 10; i ++ )
    {
        for(int j = 0; j < hiddenLayerCells; j ++)
        {
            fin >> layers.back().cells.at(i).weight.at(j);
            
        }
    }
    fin.close();
}

int neuronNet::recInputNum(std::string imgFileName)
{
    Pgm_map p(imgFileName.c_str());
    p.transformMapToGrid();
    data d(p,1);
    d.showData();
    neuronLayer & input = layers.front();   //输入层随读入的图片变化而变化
    // std::cout << "input layer " << input.cellsNum << std::endl;
    for (int i = 0; i < imgLength; i++ )
    {
        input.cells.at(i).output = d.vec[i];
        input.output.at(i) = d.vec[i];
    }

    neuronLayer & hiddenLayer = layers.at(1);
    
    // for(int i = 0; i < imgLength; i++)
    // {
    //     std::cout << hiddenLayer.cells.front().weight.at(i) << " ";
    // }
    // std::cout << std::endl;
    std::cout << hiddenLayer.cells.at(1).output << " hidden " << std::endl;
    for (int i = 0; i < hiddenLayerCells; i++ )
    {
        hiddenLayer.cells.at(i).calculateOutput(input);
        
    }
    // std::cout << "hiddenLayer output : ";
    // for(int i = 0; i < hiddenLayer.output.size(); i++)
    // {
    //     std::cout << hiddenLayer.output.at(i) << " " ; 
    // }
    // std::cout << std::endl;
    // fgetc(stdin);

    neuronLayer & output = layers.back();   
    for (int i = 0; i < 10; i++ )
    {
        output.cells.at(i).calculateOutput(hiddenLayer);
        std::cout << output.cells.at(i).output << " ";
    }

    double maxResult = 0;
    int index = 0;
    for(int i = 0; i < 10; i ++)
    {
        double d = output.cells.at(i).output;
        if(d > maxResult )
        {
            maxResult = d;
            index = i;
        }
    }
    std::cout << std::endl;
    std::cout << "rec result is : " << index << std::endl;

    std::cout << std::endl;

}