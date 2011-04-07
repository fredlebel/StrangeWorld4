#ifndef _StrangeNeuralNetwork_h_included_
#define _StrangeNeuralNetwork_h_included_

#include "NeuralNetwork.h"
#include "SummationFunction.h"
#include "TransferFunction.h"
#include "Neuron.h"

class StrangeNNGene;

class StrangeNeuralNetwork
{
private: // Typedefs
    template <class T, class Param1>
    class NewCreationPolicy_1Param
    {
    public:
        NewCreationPolicy_1Param() {};
        static T* create( Param1 p1 )
        {
            return new T( p1 );
        }

        static void destroy( T* value )
        {
            delete value;
        }

        typedef T constructType;
    protected:
        ~NewCreationPolicy_1Param() {};
    };

public: // Consts

    typedef int SynapseType;
    typedef int WeightType;
    typedef nn::SummationFunction_WeightAvg<SynapseType, WeightType> SummationFunctionType;
    typedef nn::TransferFunction_Linear<SynapseType, -1000, 1000> TransferFunctionType;
    typedef nn::Neuron<SynapseType, WeightType, SummationFunctionType, TransferFunctionType> NeuronType;
    typedef NewCreationPolicy_1Param<NeuronType, SynapseType> NodePolicyType;
    typedef nn::NeuralNetwork<NodePolicyType, 8> NeuralNetworkType;

    static const int SynapseMax =  1000;
    static const int SynapseMin = -1000;
    static const int BiasMax    =  1000;
    static const int BiasMin    = -1000;
    
public:
    StrangeNeuralNetwork( StrangeNNGene* gene );
    ~StrangeNeuralNetwork();

    void push( unsigned int index, int value );
    int pop( unsigned int index );
    void tick();
    void reset();

    NeuralNetworkType& getNetwork();

private:
    void buildFromGene( StrangeNNGene* gene );
    NeuralNetworkType network_;
};


#endif //_StrangeNeuralNetwork_h_included_