#include "ai.h"
#include <cmath>

float sigmoid(float x) {
    return 1.0f / (1.0f + std::exp(-x));
}

double randomize_num() { //returns a random number between -1 and 1
    static std::random_device rd;
    static std::mt19937 gen(rd());

    static std::uniform_real_distribution<> distrib(-1.0, 1.0);

    return distrib(gen);
}

Ai::Ai(std::vector<int> structure){

    for(int struct_index=1; struct_index !=structure.size(); ++struct_index){ // for every layer
        network.push_back(std::vector<neuron>());
        for(int neuron_index=0; neuron_index!=structure[struct_index]; ++neuron_index){ // for every neuron
            neuron tempNode;
            for(int weight_index=0; weight_index!=structure[struct_index-1]; ++weight_index){ // for every weight of node
                tempNode.weights.push_back(randomize_num());
            }
            tempNode.bias = randomize_num();
            network[struct_index-1].push_back(tempNode);
        }

    }
}  

std::vector<float> Ai::think(std::vector<float>& inputs){ 
    std::vector<std::vector<float>> layers;
    layers.push_back(inputs);

    // starts at hidden layer number 1
    for(auto layer_index = 0; layer_index != network.size(); ++layer_index){
        layers.push_back(std::vector<float>());
        for(auto node_index = 0; node_index != network[layer_index].size(); ++node_index){
            float sum = 0;
            for(auto weights_index=0; weights_index != network[layer_index][node_index].weights.size(); ++weights_index){
                sum += layers[layer_index][weights_index] * network[layer_index][node_index].weights[weights_index];
            }
            layers[layer_index+1].push_back(sigmoid(sum+network[layer_index][node_index].bias));
        }
    }
    return layers.back();
}

void Ai::mutate(float mutateStrength){
    for(auto layer_index = 1; layer_index != network.size(); ++layer_index){
        for(auto node_index = 0; node_index != network[layer_index].size(); ++node_index){
            for(auto weights_index=0; weights_index != network[layer_index][node_index].weights.size(); ++weights_index){
                network[layer_index][node_index].weights[weights_index] += mutateStrength*randomize_num();
            }
            network[layer_index][node_index].bias += mutateStrength*randomize_num();
        }
    }
}
