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
    float mutateStrength;

public:

    Ai(std::vector<int> structure, float strength);
    std::vector<float> think(std::vector<float>& inputs);
    void mutate();

};