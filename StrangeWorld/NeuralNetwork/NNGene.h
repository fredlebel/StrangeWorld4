#ifndef _StrangeNNGene_h_included_
#define _StrangeNNGene_h_included_

#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include "WorldSettings.h"

class NNGene
{
public:

    /** Level from '0' to '9' */
    static void setMutationLevel( unsigned int level );
    static unsigned int getMutationLevel();

    NNGene( std::wstring const& filename );
    NNGene( NNGene* gene );
    ~NNGene();

    unsigned int getGeneration();
    void mutate();
    void merge( NNGene* gene );

    bool saveGene( std::wstring const& filename );

public:
    typedef double Bias;
    typedef std::pair<int, Bias> Dendrite;
    typedef std::vector<Dendrite> Neuron;
    typedef std::vector<Neuron> GeneData;
    GeneData data_;

private:
    bool loadFromFile( std::wstring const& filename );
    bool createGeneData( std::string const& str );
    bool createNeuron( Neuron& neuron, std::string const& str );
    // Mutate functions
    void mutate_tweakDendrite();
    void mutate_invertBias();
    void mutate_changeBias();
    void mutate_addDendrite();
    void mutate_removeDendrite();
    void mutate_moveDendrite();
    void mutate_addNeuron();
    void mutate_removeNeuron();

private:
    std::vector<std::string> splitToken( std::string const& str, std::string const& token );

    unsigned int    geneGeneration_;

};

#endif //_StrangeNNGene_h_included_
