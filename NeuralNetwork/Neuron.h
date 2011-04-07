#ifndef _Neuron_h_included
#define _Neuron_h_included
#pragma once
//#pragma warning(disable:4786)

#include <assert.h>
#include <vector>

namespace nn
{
    /**
     * Templated Neuron class
     * 
     * @param: 
     * @return: 
     * @author: Frederic LeBel
     */
    template <class SynapseType, class WeightType, class SummationFunction, class TransferFunction>
    class Neuron 
        : private SummationFunction
        , private TransferFunction
    {
    public:
        typedef SynapseType SynapseType;

        struct ConnectionType
        {
            ConnectionType( Neuron* adest, WeightType aweight ) : dest( adest ), weight( aweight ) {}
            Neuron* dest;
            WeightType weight;
        };
        typedef std::vector<ConnectionType> ConnectionList;

    public:     // CTOR/DTOR
        Neuron( SynapseType currentSynapse ) 
            : currentSynapse_( currentSynapse ) {}
        ~Neuron() {}

    public:     // Functions

        // Use this function on the topmost neurons in a network
        inline void push( SynapseType synapse )
        {
            SummationFunction& summationF = *this;
            summationF.push( synapse, 1 );
        }

        void tick()
        {
            SummationFunction& summationF = *this;
            unsigned int connectionCount = (unsigned int)connections_.size();
            // For every connection
            for ( unsigned int i = 0; i < connectionCount; ++i )
            {
                ConnectionType& connection = connections_[i];
                // Push the connection's value along with the weight
                // on to the summation function
                summationF.push( connection.dest->pop(), connection.weight );
            }
            // Then calculate the current synapse
            TransferFunction& transferF = *this;
            currentSynapse_ = transferF.compute( summationF.pop() );
        }

        inline SynapseType pop()
        {
            return currentSynapse_;
        }

        inline void connectTo( Neuron* dest, WeightType weight )
        {
            connections_.push_back( ConnectionType( dest, weight ) );
        }

        ConnectionList& getConnections()
        {
            return connections_;
        }

        void reset()
        {
            currentSynapse_ = 0;
        }

    protected:  // Functions
    private:    // Functions

    public:     // Variables
    protected:  // Variables
    private:    // Variables
        ConnectionList  connections_;
        SynapseType     currentSynapse_;
    };

}

//#pragma warning(default:4786)
#endif // _Neuron_h_included
