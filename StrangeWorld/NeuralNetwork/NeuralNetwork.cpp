#include "NeuralNetwork.h"
#include "NNGene.h"
#include "WorldSettings.h"

double NeuralNetwork::BiasMin = -1.0;
double NeuralNetwork::BiasMax = 1.0;

std::auto_ptr<NeuralNetwork> NeuralNetwork::buildFromGene( int inputCount, NNGene* gene )
{
    std::auto_ptr<NeuralNetwork> ret( new NeuralNetwork(gene->data_.size(), inputCount) );

    // Connect all the neurons together.
    int neuronIndex = 0; // Index of neuron whose dendrites we are connecting.
    for ( auto neuronIt = gene->data_.begin(); neuronIt != gene->data_.end(); ++neuronIt, ++neuronIndex )
    {
        // For every dendrite on this neuron
        for ( auto dendriteIt = neuronIt->begin(); dendriteIt != neuronIt->end(); ++dendriteIt )
        {
            if ( dendriteIt->first < 0 )
            {
                ret->connectOut(neuronIndex, std::abs(dendriteIt->first) - 1, dendriteIt->second);
            }
            else
            {
                ret->connectIn(neuronIndex, dendriteIt->first, dendriteIt->second);
            }
        }
    }

    return ret;
}

NeuralNetwork::NeuralNetwork(int neuronCount, int inputCount)
{
    _neurons.resize(neuronCount);
    _inputs.resize(inputCount, 0);
}

void NeuralNetwork::connectIn(int fromIndex, int toIndex, double bias)
{
    _neurons[fromIndex].ins.push_back(Connection(&(_neurons[toIndex].out), bias));
}

void NeuralNetwork::connectOut(int fromIndex, int toIndex, double bias)
{
    _neurons[fromIndex].ins.push_back(Connection(&(_inputs[toIndex]), bias));
}

void NeuralNetwork::push(int index, double value)
{
    _inputs[index] = value;
}

double NeuralNetwork::pop(int index)
{
    index = _neurons.size() - index - 1;
    return _neurons[index].out;
}

void NeuralNetwork::tick()
{
    for (auto it = _neurons.begin(); it != _neurons.end(); ++it)
    {
        it->tick();
    }
}



NeuralNetwork::Neuron::Neuron() : out(0)
{
}

#if 0
void NeuralNetwork::Neuron::tick()
{
    if (ins.size() == 0)
    {
        out = 0.0;
        return;
    }

    double tempOut = 0.0;

    for (auto it = ins.begin(); it != ins.end(); ++it)
    {
        tempOut += *(it->in) * it->bias;
    }
    tempOut /= BiasMax;
    tempOut = std::min( BiasMax, std::max( BiasMin, tempOut ) );
    out = tempOut;
}
#else
void NeuralNetwork::Neuron::tick()
{
    if (ins.size() == 0)
    {
        out = 0.0;
        return;
    }

    double tempOut = 0.0;
    double biasSum = 0.0;

    for (auto it = ins.begin(); it != ins.end(); ++it)
    {
        tempOut += *(it->in) * it->bias;
        biasSum += it->bias;
    }

    //tempOut /= ins.size() * BiasMax;
    tempOut /= ins.size() * 0.8;
    /*
    if (tempOut > BiasMax || tempOut < BiasMin)
    {
        assert(false);
        throw "Range error in neuron output.";
    }*/

    tempOut = std::min( BiasMax, std::max( BiasMin, tempOut ) );
    out = tempOut;
}
#endif