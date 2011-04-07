#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sstream>

#include "StrangeNNGene.h"
#include "StrangeNeuralNetwork.h"
#include "StrangeLivingCreature.h"
#include "FastRand.h"

static unsigned int globalMutationLevel = 1;

StrangeNNGene::StrangeNNGene( StrangeNNGene* gene )
{
    data_ = gene->data_;
    geneGeneration_ = gene->geneGeneration_ + 1;
}

StrangeNNGene::StrangeNNGene( std::wstring const& filename )
{
    if ( ! loadFromFile( filename ) )
    {
        // Create a basic initial network.
        int n, i;
        // Neurons that connect to all inputs
        std::ostringstream geneStr;
        int hiddenLayerSize = 6; // TODO: Make configurable

        // Start with generation zero
        geneStr << "0";

        // The hidden layers
        for ( n = 0; n < hiddenLayerSize; ++n )
        {
            // Start with a neuron token
            geneStr << GENEFILE_TOKEN_NEURON;
            // Connect the neuron to all inputs.
            for ( i = 1; i <= StrangeLivingCreature::NNI_Count; ++i )
            {
                geneStr << GENEFILE_TOKEN_DENDRITE << (-i) << ",500";
            }
        }

        // Output layer
        for ( n = 0; n < StrangeLivingCreature::NNO_Count; ++n )
        {
            // Start with a neuron token
            geneStr << GENEFILE_TOKEN_NEURON;
            // Connect the neuron to all those neurons in the hidden layer.
            for ( i = 0; i < hiddenLayerSize; ++i )
            {
                geneStr << GENEFILE_TOKEN_DENDRITE << i << ",500";
            }
        }

        if ( ! createGeneData( geneStr.str() ) )
        {
            assert( false );
        }
    }
}

StrangeNNGene::~StrangeNNGene()
{
}

void StrangeNNGene::mutate()
{
    if ( globalMutationLevel == 0 )
        return;
    unsigned int i;

    // Decrement dendrite bias
    for ( i = 0; i < globalMutationLevel && !data_.empty(); ++i )
    {
        Neuron& neuron  = data_[ randomMT() % data_.size() ];
        if ( neuron.empty() )
            continue;
        Neuron::size_type dendriteIndex = randomMT() % neuron.size();
        Neuron::iterator dendriteIt = neuron.begin();
        std::advance( dendriteIt, dendriteIndex );
        int amount = randomMT() % MUTATION_BIAS_SOFT;
        if ( dendriteIt->second >= StrangeNeuralNetwork::BiasMin + amount )
            dendriteIt->second -= amount;
    }
    // Increment dendrite bias
    for ( i = 0; i < globalMutationLevel && !data_.empty(); ++i )
    {
        Neuron& neuron  = data_[ randomMT() % data_.size() ];
        if ( neuron.empty() )
            continue;
        Neuron::size_type dendriteIndex = randomMT() % neuron.size();
        Neuron::iterator dendriteIt = neuron.begin();
        std::advance( dendriteIt, dendriteIndex );
        int amount = randomMT() % MUTATION_BIAS_SOFT;
        if ( dendriteIt->second <= StrangeNeuralNetwork::BiasMax - amount )
            dendriteIt->second += amount;
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
            int value = ( randomMT() % ( StrangeNeuralNetwork::BiasMax * 2 ) ) -
                StrangeNeuralNetwork::BiasMax;
            // Modify its bias.
            dendriteIt->second = value;
        }
    }

    // Once in a while, remove/add a dendrite
    if ( ( randomMT() % MUTATION_BIAS_ADDREM ) < (globalMutationLevel-1) && !data_.empty() )
    {
        Neuron& neuron  = data_[ randomMT() % data_.size() ];

        if ( randomMT() & 0x00000001 ) // Half the time add a dendrite
        {
            // Random neuron, don't forget to include the input neurons.
            int targetNeuronIndex =
                (randomMT() % (data_.size() + StrangeLivingCreature::NNI_Count))
                - StrangeLivingCreature::NNI_Count;
            // A random value for this new dendrite.
            int value = ( randomMT() % ( StrangeNeuralNetwork::BiasMax * 2 ) ) -
                StrangeNeuralNetwork::BiasMax;
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
                (randomMT() % (data_.size() + StrangeLivingCreature::NNI_Count))
                - StrangeLivingCreature::NNI_Count;
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
                for ( int i = 1; i <= StrangeLivingCreature::NNI_Count; ++i )
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
                        (randomMT() % (data_.size() + StrangeLivingCreature::NNI_Count))
                        - StrangeLivingCreature::NNI_Count;
                    // A random value for this new dendrite.
                    int value = ( randomMT() % ( StrangeNeuralNetwork::BiasMax * 2 ) ) -
                        StrangeNeuralNetwork::BiasMax;
                    // Insert the new dendrite.
                    data_[index].insert( std::make_pair( targetNeuronIndex, value ) );
                }

                // Connect a random neuron to this new one.
                {
                    int targetNeuronIndex = randomMT() % data_.size();
                    // A random value for this new dendrite.
                    int value = ( randomMT() % ( StrangeNeuralNetwork::BiasMax * 2 ) ) -
                        StrangeNeuralNetwork::BiasMax;
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
}

void StrangeNNGene::merge( StrangeNNGene* )
{
}

bool StrangeNNGene::saveGene( std::wstring const& filename )
{
    FILE* pFile = _wfopen( filename.c_str(), L"wb" );
    if ( NULL == pFile )
        return false;

    // Write the file header
    fwrite( GENEFILE_HEADER, 1, strlen( GENEFILE_HEADER ), pFile );

    // Write the generation count
    {
        char buff[80];
        itoa( geneGeneration_, buff, 10 );
        fwrite( buff, 1, strlen( buff ), pFile );
    }

    // For every layer
    GeneData::iterator neuronIt = data_.begin();
    for ( ; neuronIt != data_.end(); ++neuronIt )
    {
        fwrite( GENEFILE_TOKEN_NEURON, 1, 1, pFile );
        // For every dendrite
        Neuron::iterator dendriteIt = neuronIt->begin();
        for ( ; dendriteIt != neuronIt->end(); ++dendriteIt )
        {
            fwrite( GENEFILE_TOKEN_DENDRITE, 1, 1, pFile );
            char buff[80];
            itoa( dendriteIt->first, buff, 10 );
            fwrite( buff, 1, strlen( buff ), pFile );
            fwrite( GENEFILE_TOKEN_COMMA, 1, 1, pFile );
            itoa( dendriteIt->second, buff, 10 );
            fwrite( buff, 1, strlen( buff ), pFile );
        }
    }

    fclose( pFile );
    return true;
}

bool StrangeNNGene::loadFromFile( std::wstring const& filename )
{
    // =@!540!~!258!-123!~@!240!~!~!~!566=@!540!~@!~!566@!-800!12=@!540!12!~@!-800!~!566
    FILE* pFile = _wfopen( filename.c_str(), L"rb" );
    if ( NULL == pFile )
    {
        return false;
    }

    // Validate that this is indeed a StrangeWorld 4 gene file.
    char header[] = GENEFILE_HEADER;
    // Read the header
    fread( (void*)header, strlen(header), sizeof(char), pFile );
    if ( strcmp( header, GENEFILE_HEADER ) != 0 )
    {
        fclose( pFile );
        return false;
    }

    const int BUFFER_SIZE = 80;
    char buff[BUFFER_SIZE];

    // Read all bytes in the file.
    std::string geneStr;
    size_t bytesRead = fread( (void*)buff, 1, BUFFER_SIZE, pFile );
    while ( bytesRead > 0 )
    {
        geneStr.append( buff, bytesRead );
        bytesRead = fread( (void*)buff, 1, BUFFER_SIZE, pFile );
    }
    fclose( pFile );

    if ( ! createGeneData( geneStr ) )
    {
        data_.clear();
        return false;
    }
    return true;
}

bool StrangeNNGene::createGeneData( std::string const& str )
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

bool StrangeNNGene::createNeuron( Neuron& neuron, std::string const& str )
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
            atoi( dendriteData[0].c_str() ),
            atoi( dendriteData[1].c_str() ) ) );
    }
    return true;
}

std::vector<std::string> StrangeNNGene::splitToken( std::string const& str, std::string const& token )
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

void StrangeNNGene::setMutationLevel( unsigned int level )
{
    if ( level >= 0 && level <= 9 )
        globalMutationLevel = level;
}

unsigned int StrangeNNGene::getMutationLevel()
{
    return globalMutationLevel;
}

// Function name   : StrangeNNGene::getGeneration
// Description     : 
// Return type     : unsigned int 
unsigned int StrangeNNGene::getGeneration()
{
    return geneGeneration_;
}

