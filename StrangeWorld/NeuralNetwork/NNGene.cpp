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
        geneStr << "@!-1,1!-2,1!-3,1!-4,1!-5,1!-6,1!-7,1!-8,1!-9,1!-10,1!-11,1!-12,1!-13,1!-14,1!-15,1";
        geneStr << "@!-1,1!-2,1!-3,1!-4,1!-5,1!-6,1!-7,1!-8,1!-9,1!-10,1!-11,1!-12,1!-13,1!-14,1!-15,1";
        geneStr << "@!-1,1!-2,1!-3,1!-4,1!-5,1!-6,1!-7,1!-8,1!-9,1!-10,1!-11,1!-12,1!-13,1!-14,1!-15,1";
        geneStr << "@!-1,1!-2,1!-3,1!-4,1!-5,1!-6,1!-7,1!-8,1!-9,1!-10,1!-11,1!-12,1!-13,1!-14,1!-15,1";
        geneStr << "@!-1,1!-2,1!-3,1!-4,1!-5,1!-6,1!-7,1!-8,1!-9,1!-10,1!-11,1!-12,1!-13,1!-14,1!-15,1";
        geneStr << "@!-1,1!-2,1!-3,1!-4,1!-5,1!-6,1!-7,1!-8,1!-9,1!-10,1!-11,1!-12,1!-13,1!-14,1!-15,1";

        // Neurons that connect to those neurons
        // The last neurons in the gene are the outputs
        geneStr << "@!0,1!1,1!2,1!3,1!4,1!5,1";
        geneStr << "@!0,1!1,1!2,1!3,1!4,1!5,1";
        geneStr << "@!0,1!1,1!2,1!3,1!4,1!5,1";
        geneStr << "@!0,1!1,1!2,1!3,1!4,1!5,1";
        geneStr << "@!0,1!1,1!2,1!3,1!4,1!5,1";

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
		// Pick a random neuron.
        Neuron& neuron  = data_[ randomMT() % data_.size() ];
        if ( neuron.empty() )
            continue;
		// Pick a random dendrite
        Neuron::size_type dendriteIndex = randomMT() % neuron.size();
        Neuron::iterator dendriteIt = neuron.begin();
        std::advance( dendriteIt, dendriteIndex );

		// Scale by a specific range.
		double minMul = 0.99;
		double maxMul = 1.01;
		double ganularity = 100000;
		int range = (int)((maxMul - minMul) * ganularity);
        double amount = (randomMT() % range) / ganularity + minMul;

        dendriteIt->second = std::min(NeuralNetwork::BiasMax,
							 std::max(NeuralNetwork::BiasMin,
							 dendriteIt->second * amount));
    }

    // Once in a while, inverse a value
    if ( ( randomMT() % MUTATION_BIAS_INVERSE ) < (globalMutationLevel-1) && !data_.empty() )
    {
        Neuron& neuron  = data_[ randomMT() % data_.size() ];
        if ( ! neuron.empty() )
        {
            Neuron::size_type dendriteIndex = randomMT() % neuron.size();
            Neuron::iterator dendriteIt = neuron.begin();
            std::advance( dendriteIt, dendriteIndex );
            dendriteIt->second = -dendriteIt->second;
        }
    }

    // Once in a while, completely change a value
    if ( ( randomMT() % MUTATION_BIAS_CHANGE ) < (globalMutationLevel-1) && !data_.empty() )
    {
        Neuron& neuron  = data_[ randomMT() % data_.size() ];
        if ( ! neuron.empty() )
        {
            // Take a random dendrite.
            Neuron::size_type dendriteIndex = randomMT() % neuron.size();
            Neuron::iterator dendriteIt = neuron.begin();
            std::advance( dendriteIt, dendriteIndex );
            int value = ( randomMT() % ( (int)NeuralNetwork::BiasMax * 2 ) ) -
                NeuralNetwork::BiasMax;
            // Modify its bias.
            dendriteIt->second = value;
        }
    }

#if !defined( USE_STATIC_NEURALNET )
	int neuralNetworkInputCount = LivingCreature::NNI_COUNT;

    // Once in a while, remove/add a dendrite
    if ( ( randomMT() % MUTATION_BIAS_ADDREM ) < (globalMutationLevel-1) && !data_.empty() )
    {
        Neuron& neuron  = data_[ randomMT() % data_.size() ];

        if ( randomMT() & 0x00000001 ) // Half the time add a dendrite
        {
            // Random neuron, don't forget to include the input neurons.
            int targetNeuronIndex =
                (randomMT() % (data_.size() + neuralNetworkInputCount))
                - neuralNetworkInputCount;
            // Always create a neural dendrite
            int value = 0;
            // Might already exist, in which case it acts as a random mutation.
            neuron.insert( std::make_pair( targetNeuronIndex, value ) );
        }
        else if ( ! neuron.empty() ) // Remove a dendrite
        {
            Neuron::size_type dendriteIndex = randomMT() % neuron.size();
            Neuron::iterator dendriteIt = neuron.begin();
            std::advance( dendriteIt, dendriteIndex );
            neuron.erase( dendriteIt );
        }
    }

    // Reconnect a dendrite to a different neuron
    if ( ( randomMT() % MUTATION_BIAS_MOVE ) < (globalMutationLevel-1) && !data_.empty() )
    {
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
            int targetNeuronIndex =
                (randomMT() % (data_.size() + neuralNetworkInputCount))
                - neuralNetworkInputCount;
            // Insert the new dendrite
            neuron.insert( std::make_pair( targetNeuronIndex, value ) );
        }
    }

    // Add/Remove a neuron.
    if ( ( randomMT() % MUTATION_NEURON_ADDREM ) < (globalMutationLevel-1) )
    {
//        if ( randomMT() & 0x00000001 ) // Half the time add a neuron.
        if ( randomMT() % 4 )
        {
            Neuron neuron;
            // Place it somewhere
            if ( data_.empty() )
            {
                // Connect it to all inputs
                for ( int i = 1; i <= neuralNetworkInputCount; ++i )
                    neuron.insert( std::make_pair( -i, 500 ) );
                data_.push_back( neuron );
            }
            else
            {
                // Insert it in a random location
                GeneData::iterator insertIt = data_.begin();
                int index = randomMT() % data_.size();
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
                            replaceNeuron.insert( std::make_pair( dendriteIt->first + 1, dendriteIt->second ) );
                        }
                        else
                        {
                            // This connection doesn't need to be fixed.
                            replaceNeuron.insert( std::make_pair( dendriteIt->first, dendriteIt->second ) );
                        }
                        // The replaceNeuron contains the fixed dendrite data.
                    }
                    *neuronIt = replaceNeuron;
                }

                // Connect the new neuron to a random neuron.
                // Don't forget to include the input neurons.
                {
                    int targetNeuronIndex =
                        (randomMT() % (data_.size() + neuralNetworkInputCount))
                        - neuralNetworkInputCount;
                    // A random value for this new dendrite.
                    int value = ( randomMT() % ( (int)NeuralNetwork::BiasMax * 2 ) ) -
                        NeuralNetwork::BiasMax;
                    // Insert the new dendrite.
                    data_[index].insert( std::make_pair( targetNeuronIndex, value ) );
                }

                // Connect a random neuron to this new one.
                {
                    int targetNeuronIndex = randomMT() % data_.size();
                    // A random value for this new dendrite.
                    int value = ( randomMT() % ( (int)NeuralNetwork::BiasMax * 2 ) ) -
                        NeuralNetwork::BiasMax;
                    // Insert the new dendrite.
                    data_[targetNeuronIndex].insert( std::make_pair( index, value ) );
                }

            }
        }
        else if ( ! data_.empty() ) // Remove a neuron.
        {
            // Pick a random neuron to remove.
            GeneData::iterator eraseIt = data_.begin();
            int index = randomMT() % data_.size();
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
                        replaceNeuron.insert( std::make_pair( dendriteIt->first - 1, dendriteIt->second ) );
                    }
                    else if ( dendriteIt->first != index )
                    {
                        // This connection doesn't need to be fixed.
                        replaceNeuron.insert( std::make_pair( dendriteIt->first, dendriteIt->second ) );
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
#endif
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

    const int BUFFER_SIZE = 80;
    char buff[BUFFER_SIZE];

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
        neuron.insert( std::make_pair(
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












