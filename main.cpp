#include "neuron.h"

int main(int argc, char * argv[])
{
    neuronNet net(1);
    // net.init();
    // net.train();
    // net.saveWeight();
    net.readWeight();
    std::cout << argv[1] << std::endl;
    net.recInputNum(argv[1]);
    std::cout << "all is done!" << std::endl;
    return 0;
}


