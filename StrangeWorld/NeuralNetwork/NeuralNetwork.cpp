#include "NeuralNetwork.h"
#include "NNGene.h"
#include "WorldSettings.h"

double NeuralNetwork::BiasMin = -1000;
double NeuralNetwork::BiasMax = 1000;

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
	return std::min( BiasMax, std::max( BiasMin, _neurons[index].out ) );
}

void NeuralNetwork::tick()
{
	for (auto it = _neurons.begin(); it != _neurons.end(); ++it)
	{
		it->tick();
	}
}



