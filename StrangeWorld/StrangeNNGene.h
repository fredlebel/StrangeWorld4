#ifndef _StrangeNNGene_h_included_
#define _StrangeNNGene_h_included_

#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include "StrangeWorldSettings.h"

class StrangeNNGene
{
public:

    /** Level from '0' to '9' */
    static void setMutationLevel( unsigned int level );
    static unsigned int getMutationLevel();

    StrangeNNGene( std::wstring const& filename );
    StrangeNNGene( StrangeNNGene* gene );
    ~StrangeNNGene();

    unsigned int getGeneration();
    void mutate();
    void merge( StrangeNNGene* gene );

    bool saveGene( std::wstring const& filename );

public:
    typedef int Bias;
    typedef std::map<int, Bias> Neuron;
    typedef std::vector<Neuron> GeneData;
    GeneData data_;

private:
    bool loadFromFile( std::wstring const& filename );
    bool createGeneData( std::string const& str );
    bool createNeuron( Neuron& neuron, std::string const& str );

private:
    std::vector<std::string> splitToken( std::string const& str, std::string const& token );

    unsigned int    geneGeneration_;

};

#endif //_StrangeNNGene_h_included_
