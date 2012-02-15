#include <assert.h>
#include <sstream>
#include <fstream>

#include "NNGene.h"
#include "NeuralNetwork.h"
#include "FastRand.h"
#include "Creatures/LivingCreature.h"

static unsigned int globalMutationLevel = 1;


NNGene::NNGene( NNGene* gene )
{
    data_ = gene->data_;
    geneGeneration_ = gene->geneGeneration_ + 1;
}

NNGene::NNGene( std::wstring const& filename )
{
    if ( ! loadFromFile( filename ) )
    {
        // Create a basic initial network.

        std::ostringstream geneStr;
        // Neurons that connect to all inputs
        geneStr << "@!-1,0.1!-2,0.1!-3,0.1!-4,0.1!-5,0.1!-6,0.1!-7,0.1!-8,0.1!-9,0.1!-10,0.1!-11,0.1!-12,0.1!-13,0.1!-14,0.1!-15,0.1";
        geneStr << "@!-1,0.1!-2,0.1!-3,0.1!-4,0.1!-5,0.1!-6,0.1!-7,0.1!-8,0.1!-9,0.1!-10,0.1!-11,0.1!-12,0.1!-13,0.1!-14,0.1!-15,0.1";
        geneStr << "@!-1,0.1!-2,0.1!-3,0.1!-4,0.1!-5,0.1!-6,0.1!-7,0.1!-8,0.1!-9,0.1!-10,0.1!-11,0.1!-12,0.1!-13,0.1!-14,0.1!-15,0.1";
        geneStr << "@!-1,0.1!-2,0.1!-3,0.1!-4,0.1!-5,0.1!-6,0.1!-7,0.1!-8,0.1!-9,0.1!-10,0.1!-11,0.1!-12,0.1!-13,0.1!-14,0.1!-15,0.1";
        geneStr << "@!-1,0.1!-2,0.1!-3,0.1!-4,0.1!-5,0.1!-6,0.1!-7,0.1!-8,0.1!-9,0.1!-10,0.1!-11,0.1!-12,0.1!-13,0.1!-14,0.1!-15,0.1";
        geneStr << "@!-1,0.1!-2,0.1!-3,0.1!-4,0.1!-5,0.1!-6,0.1!-7,0.1!-8,0.1!-9,0.1!-10,0.1!-11,0.1!-12,0.1!-13,0.1!-14,0.1!-15,0.1";

        // Neurons that connect to those neurons
        // The last neurons in the gene are the outputs
        geneStr << "@!0,0.1!1,0.1!2,0.1!3,0.1!4,0.1!5,0.1";
        geneStr << "@!0,0.1!1,0.1!2,0.1!3,0.1!4,0.1!5,0.1";
        geneStr << "@!0,0.1!1,0.1!2,0.1!3,0.1!4,0.1!5,0.1";
        geneStr << "@!0,0.1!1,0.1!2,0.1!3,0.1!4,0.1!5,0.1";
        geneStr << "@!0,0.1!1,0.1!2,0.1!3,0.1!4,0.1!5,0.1";

        if ( ! createGeneData( geneStr.str() ) )
        {
            assert( false );
        }
    }
}

NNGene::~NNGene()
{
}

void NNGene::mutate()
{
    if ( globalMutationLevel == 0 )
        return;
    unsigned int i;

    // Small changes to the biases
    for ( i = 0; i < globalMutationLevel && !data_.empty(); ++i )
    {
        mutate_tweakDendrite();
    }

#if 0
    // Once in a while, inverse a value
    if ( ( randomMT() % MUTATION_BIAS_INVERSE ) < (globalMutationLevel-1) && !data_.empty() )
    {
        mutate_invertBias();
    }

    // Once in a while, completely change a value
    if ( ( randomMT() % MUTATION_BIAS_CHANGE ) < (globalMutationLevel-1) && !data_.empty() )
    {
        mutate_changeBias();
    }
#endif

#if !defined( USE_STATIC_NEURALNET )

    // Once in a while, remove/add a dendrite
    if ( ( randomMT() % MUTATION_BIAS_ADDREM ) < (globalMutationLevel-1) && !data_.empty() )
    {
        if ( randomMT() & 0x00000001 ) // Half the time add a dendrite
        {
            mutate_addDendrite();
        }
        else // Remove a dendrite
        {
            mutate_removeDendrite();
        }
    }

#if 0
    // Reconnect a dendrite to a different neuron
    if ( ( randomMT() % MUTATION_BIAS_MOVE ) < (globalMutationLevel-1) && !data_.empty() )
    {
        mutate_moveDendrite();
    }
#endif

    // Add/Remove a neuron.
    if ( ( randomMT() % MUTATION_NEURON_ADDREM ) < (globalMutationLevel-1) )
    {
        if ( randomMT() & 0x00000001 ) // Half the time add a neuron.
//        if ( randomMT() % 4 )
        {
            mutate_addNeuron();
        }
        else if ( ! data_.empty() ) // Remove a neuron.
        {
            mutate_removeNeuron();
        }
    }
#endif
}

int NNGene::pickRandomSynapse()
{
    int neuralNetworkInputCount = LivingCreature::NNI_COUNT;
    // Random neuron, don't forget to include the input neurons.
    // *3 to increase the odds of connecting internally.
    int ret = (randomMT() % (data_.size() * 3 + neuralNetworkInputCount)) - neuralNetworkInputCount;

    if (ret >= 0)
        ret %= data_.size();

    return ret;
}

void NNGene::mutate_tweakDendrite()
{
    // Pick a random neuron.
    Neuron& neuron  = data_[ randomMT() % data_.size() ];
    if ( neuron.empty() )
        return;
    // Pick a random dendrite
    Neuron::size_type dendriteIndex = randomMT() % neuron.size();
    Neuron::iterator dendriteIt = neuron.begin();
    std::advance( dendriteIt, dendriteIndex );

#if 0
    // Scale by a specific range.
    double minMul = 0.09;
    double maxMul = 1.01;
    double ganularity = 100000;
    int range = (int)((maxMul - minMul) * ganularity);
    double amount = (randomMT() % range) / ganularity + minMul;

    dendriteIt->second *= amount;

    // Limit decimal range to 3 digits
    if (std::abs(dendriteIt->second) < 0.001)
    {
        dendriteIt->second = 0.0;
    }
#else
    // Scale by a specific range.
    double minAdd = -0.01;
    double maxAdd = +0.01;
    double ganularity = 1000;
    int range = (int)((maxAdd - minAdd) * ganularity);
    double amount = (randomMT() % range) / ganularity + minAdd;

    dendriteIt->second += amount;

#endif
    dendriteIt->second = std::min(NeuralNetwork::BiasMax,
                            std::max(NeuralNetwork::BiasMin,
                            dendriteIt->second));
}

void NNGene::mutate_invertBias()
{
    Neuron& neuron  = data_[ randomMT() % data_.size() ];
    if ( ! neuron.empty() )
    {
        Neuron::size_type dendriteIndex = randomMT() % neuron.size();
        Neuron::iterator dendriteIt = neuron.begin();
        std::advance( dendriteIt, dendriteIndex );
        dendriteIt->second *= -1;
    }
}

void NNGene::mutate_changeBias()
{
    Neuron& neuron  = data_[ randomMT() % data_.size() ];
    if ( ! neuron.empty() )
    {
        // Take a random dendrite.
        Neuron::size_type dendriteIndex = randomMT() % neuron.size();
        Neuron::iterator dendriteIt = neuron.begin();
        std::advance( dendriteIt, dendriteIndex );

        double granularity = 1000 * NeuralNetwork::BiasMax;
        double value = (randomMT() % (int)(granularity*2)) / granularity - NeuralNetwork::BiasMax;
        // Modify its bias.
        dendriteIt->second = value;
    }
}

void NNGene::mutate_addDendrite()
{
    int neuralNetworkInputCount = LivingCreature::NNI_COUNT;

    Neuron& neuron  = data_[ randomMT() % data_.size() ];

    // Random neuron, don't forget to include the input neurons.
    int targetNeuronIndex = pickRandomSynapse();

    // Pick a random value.
    double granularity = 1000 * NeuralNetwork::BiasMax;
    double value = 0.0;//(randomMT() % (int)(granularity*2)) / granularity - NeuralNetwork::BiasMax;
    neuron.push_back(std::make_pair( targetNeuronIndex, value ) );
}

void NNGene::mutate_removeDendrite()
{
    int neuralNetworkInputCount = LivingCreature::NNI_COUNT;

    Neuron& neuron  = data_[ randomMT() % data_.size() ];
    if ( neuron.empty() )
        return;

    Neuron::size_type dendriteIndex = randomMT() % neuron.size();
    Neuron::iterator dendriteIt = neuron.begin();
    std::advance( dendriteIt, dendriteIndex );
    neuron.erase( dendriteIt );
}

void NNGene::mutate_moveDendrite()
{
    int neuralNetworkInputCount = LivingCreature::NNI_COUNT;
    Neuron& neuron  = data_[ randomMT() % data_.size() ];
    if ( ! neuron.empty() )
    {
        // Take a random dendrite.
        Neuron::size_type dendriteIndex = randomMT() % neuron.size();
        Neuron::iterator dendriteIt = neuron.begin();
        std::advance( dendriteIt, dendriteIndex );
        Bias value = dendriteIt->second;
        // Erase the dendrite
        neuron.erase( dendriteIt );
        // Random neuron, don't forget to include the input neurons.
        int targetNeuronIndex = pickRandomSynapse();
        // Insert the new dendrite
        neuron.push_back(std::make_pair( targetNeuronIndex, value ) );
    }
}

void NNGene::mutate_addNeuron()
{
    int neuralNetworkInputCount = LivingCreature::NNI_COUNT;
    Neuron neuron;
    // Place it somewhere
    if ( data_.empty() )
    {
        // Connect it to all inputs
        for ( int i = 1; i <= neuralNetworkInputCount; ++i )
            neuron.push_back( std::make_pair( -i, 0.0 ) );
        data_.push_back( neuron );
    }
    else
    {
        // Insert it in a random location
        int index = randomMT() % data_.size();
        GeneData::iterator insertIt = data_.begin();
        std::advance( insertIt, index );
        data_.insert( insertIt, neuron );

        // And fix all the neurons.
        Neuron replaceNeuron;
        GeneData::iterator neuronIt = data_.begin();
        for ( ; neuronIt != data_.end(); ++neuronIt )
        {
            replaceNeuron.clear();
            Neuron::iterator dendriteIt = neuronIt->begin();
            for ( ; dendriteIt != neuronIt->end(); ++dendriteIt )
            {
                if ( dendriteIt->first >= index )
                {
                    // We have to fix the offset so it still
                    // points to the same neuron
                    replaceNeuron.push_back( std::make_pair( dendriteIt->first + 1, dendriteIt->second ) );
                }
                else
                {
                    // This connection doesn't need to be fixed.
                    replaceNeuron.push_back( std::make_pair( dendriteIt->first, dendriteIt->second ) );
                }
                // The replaceNeuron contains the fixed dendrite data.
            }
            *neuronIt = replaceNeuron;
        }
    }
}

void NNGene::mutate_removeNeuron()
{
    // Never remove one of the output neurons
    if ( data_.size() > LivingCreature::NNI_COUNT ) // Remove a neuron.
    {
        // Pick a random neuron to remove.
        int index = randomMT() % (data_.size() - LivingCreature::NNI_COUNT);
        GeneData::iterator eraseIt = data_.begin();
        std::advance( eraseIt, index );
        // And remove it.
        data_.erase( eraseIt );
        // Now remove all dendrites that used to point to this neuron
        // or fix the offsets of their dendrites.
        Neuron replaceNeuron;
        GeneData::iterator neuronIt = data_.begin();
        for ( ; neuronIt != data_.end(); ++neuronIt )
        {
            replaceNeuron.clear();
            Neuron::iterator dendriteIt = neuronIt->begin();
            for ( ; dendriteIt != neuronIt->end(); ++dendriteIt )
            {
                if ( dendriteIt->first > index )
                {
                    // We have to fix the offset so it still
                    // points to the same neuron
                    replaceNeuron.push_back( std::make_pair( dendriteIt->first - 1, dendriteIt->second ) );
                }
                else if ( dendriteIt->first != index )
                {
                    // This connection doesn't need to be fixed.
                    replaceNeuron.push_back( std::make_pair( dendriteIt->first, dendriteIt->second ) );
                }
                else
                {
                    // Drop the dendrite that points to the old neuron.
                }
                // The replaceNeuron contains the fixed dendrite data.
            }
            *neuronIt = replaceNeuron;
        }
    }
}

void NNGene::merge( NNGene* )
{
}

bool NNGene::saveGene( std::wstring const& filename )
{
    std::ofstream file;
    file.open( filename );

    if ( !file.is_open() )
        return false;

    // Write the file header
    file << GENEFILE_HEADER << std::endl;

    // Write the generation count
    file << geneGeneration_ << std::endl;

    // For every layer
    GeneData::iterator neuronIt = data_.begin();
    for ( ; neuronIt != data_.end(); ++neuronIt )
    {
        file << GENEFILE_TOKEN_NEURON;
        // For every dendrite
        Neuron::iterator dendriteIt = neuronIt->begin();
        for ( ; dendriteIt != neuronIt->end(); ++dendriteIt )
        {
            file << GENEFILE_TOKEN_DENDRITE << dendriteIt->first << "," << dendriteIt->second;
        }
    }

    file.close();
    return true;
}

bool NNGene::loadFromFile( std::wstring const& filename )
{
    // =@!540!~!258!-123!~@!240!~!~!~!566=@!540!~@!~!566@!-800!12=@!540!12!~@!-800!~!566
    std::ifstream file;
    file.open( filename );

    if ( !file.is_open() )
        return false;

    // Validate that this is indeed a proper gene file.
    std::string header;
    std::getline(file, header);

    if ( header != GENEFILE_HEADER )
    {
        assert(false);
        file.close();
        return false;
    }

    // Read all bytes in the file.
    std::string geneStr((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    if ( ! createGeneData( geneStr ) )
    {
        data_.clear();
        return false;
    }
    return true;
}

bool NNGene::createGeneData( std::string const& str )
{
    // Extract all the neurons
    std::vector<std::string> values = splitToken( str, GENEFILE_TOKEN_NEURON );
    if ( values.empty() )
        return false;
    std::vector<std::string>::iterator valueIt = values.begin();
    // Read the generation count
    geneGeneration_ = atoi( valueIt->c_str() );
    // The rest are neuron data sets.
    ++valueIt;
    data_.resize( values.size() - 1 );
    GeneData::iterator neuronIt = data_.begin();
    // For every neuron
    for ( ; valueIt != values.end(); ++valueIt, ++neuronIt )
    {
        if ( ! createNeuron( *neuronIt, *valueIt ) )
            return false;
    }
    return true;
}

bool NNGene::createNeuron( Neuron& neuron, std::string const& str )
{
    // Extract all dendrites
    std::vector<std::string> values = splitToken( str, GENEFILE_TOKEN_DENDRITE );
    std::vector<std::string>::iterator valueIt = values.begin();
    // For every dendrite
    for ( ; valueIt != values.end(); ++valueIt )
    {
        // Extract the index and the bias
        std::vector<std::string> dendriteData = splitToken( *valueIt, GENEFILE_TOKEN_COMMA );
        // Some sanity
        if ( dendriteData.size() != 2 )
        {
            assert( false );
            return false;
        }
        // Insert this dendrite.
        neuron.push_back( std::make_pair(
            std::atoi( dendriteData[0].c_str() ),
            std::atof( dendriteData[1].c_str() ) ) );
    }
    return true;
}


std::vector<std::string> NNGene::splitToken( std::string const& str, std::string const& token )
{
    std::vector<std::string> values;

    if ( str.empty() )
        return values;

    std::string::size_type tokenPos = 0;
    std::string::size_type i = 0;
    do
    {
        tokenPos = str.find( token, i );
        if ( tokenPos > 0 )
            values.push_back( str.substr( i, tokenPos - i ) );
        i = tokenPos + 1;
    } while ( tokenPos != std::string::npos );


    return values;
}

void NNGene::setMutationLevel( unsigned int level )
{
    if ( level >= 0 && level <= 9 )
        globalMutationLevel = level;
}

unsigned int NNGene::getMutationLevel()
{
    return globalMutationLevel;
}

// Function name   : NNGene::getGeneration
// Description     : 
// Return type     : unsigned int 
unsigned int NNGene::getGeneration()
{
    return geneGeneration_;
}


/*
@
    !-1,590     // Input 1
    !-2,-340    // Input 2
    !0,600      // Loopback
    !3,32       // Connect to 4th neuron
@
    !-2,-793    // Input 2
    !0,600      // Connect to 1st neuron
    !1,320      // Loopback


*/












