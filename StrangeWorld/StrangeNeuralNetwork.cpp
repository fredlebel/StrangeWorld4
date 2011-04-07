#include "StrangeNeuralNetwork.h"
#include "StrangeNNGene.h"
#include "StrangeWorldSettings.h"

// Function name   : StrangeNeuralNetwork::StrangeNeuralNetwork
// Description     : 
// Return type     : 
// Argument        : StrangeNNGene* gene
StrangeNeuralNetwork::StrangeNeuralNetwork( StrangeNNGene* gene )
{
    buildFromGene( gene );
}


// Function name   : StrangeNeuralNetwork::~StrangeNeuralNetwork
// Description     : 
// Return type     : 
StrangeNeuralNetwork::~StrangeNeuralNetwork()
{
}


// Function name   : StrangeNeuralNetwork::push
// Description     : 
// Return type     : void 
// Argument        : int index
// Argument        : int value
void StrangeNeuralNetwork::push( unsigned int index, int value )
{
    NeuralNetworkType::NodeLayer* layer = network_.getNodeLayer( 0 );
    NeuronType* n = (*layer)[index];
    n->push( value );
}


// Function name   : StrangeNeuralNetwork::tick
// Description     : 
// Return type     : void 
void StrangeNeuralNetwork::tick()
{
    network_.tick();
}

void StrangeNeuralNetwork::reset()
{
    network_.reset();
}


// Function name   : StrangeNeuralNetwork::pop
// Description     : 
// Return type     : int 
// Argument        : unsigned int index
int StrangeNeuralNetwork::pop( unsigned int index )
{
    NeuralNetworkType::NodeLayer* layer = network_.getLastLayer();
    unsigned int layerSize = (unsigned int)layer->size();
    if ( layerSize <= index )
    {
        // There isn't even a neuron there. So no output.
        return 0;
    }
    // The outputs are the last neurons in the list.
    NeuronType* n = (*layer)[ layerSize - index - 1 ];
    return n->pop();
}

// Function name   : StrangeNeuralNetwork::buildFromGene
// Description     : Assumes an empty network
// Return type     : void 
// Argument        : StrangeNNGene* gene
void StrangeNeuralNetwork::buildFromGene( StrangeNNGene* gene )
{

    std::vector<NeuronType*> inputNodeList;
    std::vector<NeuronType*> networkNodeList;

    // Create the first layer, this is the input layer
    network_.addLayer();
    // Create a node for each input, this is the first layer
    for ( unsigned int i = 0; i < NEURAL_NET_INPUT_COUNT; ++i )
    {
        NeuronType* n = network_.createNode( 0, 0 );
        // Push it to the current list
        inputNodeList.push_back( n );
    }

    // Start from the beginning
    StrangeNNGene::GeneData::iterator neuronIt = gene->data_.begin();

    // First step is to create all neurons in the order
    // the are found in the gene data.
    for ( ; neuronIt != gene->data_.end(); ++neuronIt )
    {
        // Create the neuron
        NeuronType* n = network_.createNode( 0, 0 );
        // Add it to the list
        networkNodeList.push_back( n );
    }

    // Second step is to connect all the neurons together.
    int neuronIndex = 0; // Index of neuron whose dendrites we are connecting.
    neuronIt = gene->data_.begin(); // Start for the start again.
    for ( ; neuronIt != gene->data_.end(); ++neuronIt, ++neuronIndex )
    {
        // For every dendrite on this neuron
        StrangeNNGene::Neuron::iterator dendriteIt = neuronIt->begin();
        for ( ; dendriteIt != neuronIt->end(); ++dendriteIt )
        {
            if ( dendriteIt->first < 0 )
            {
                // Negative index indicates a connection to an input.
                networkNodeList[neuronIndex]->connectTo(
                    inputNodeList[-(dendriteIt->first+1)],
                    dendriteIt->second );
            }
            else
            {
                // Positive index indicates a connection to a neuron.
                networkNodeList[neuronIndex]->connectTo(
                    networkNodeList[dendriteIt->first],
                    dendriteIt->second );
            }
        }
    }
}

StrangeNeuralNetwork::NeuralNetworkType& StrangeNeuralNetwork::getNetwork()
{
    return network_;
}

















