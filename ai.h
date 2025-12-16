#include <vector>
#include <random>
#include <iostream>

float sigmoid(float x);

struct neuron{
    float bias;
    std::vector<float> weights;
};

class Ai{

    std::vector<std::vector<neuron>> network;

public:

    Ai(std::vector<int> structure);
    std::vector<float> think(std::vector<float>& inputs);
    void mutate(float mutateStrength);
    

};