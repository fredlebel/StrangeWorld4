#ifndef _NeuralNetwork_h_included
#define _NeuralNetwork_h_included
#pragma once

#include <assert.h>
#include <algorithm>
#include <list>
#include <vector>

namespace nn {

template <class NodePolicy, int RESERVE_LAYER_COUNT>
class NeuralNetwork 
    : public NodePolicy
{
public: // typedefs
    // The object types
    typedef typename NodePolicy::constructType  NodeType;
    // The layer types
    typedef std::vector<NodeType*>              NodeLayer;
    // The list of layers
    typedef std::vector<NodeLayer*>             NodeLayerList;
    // The type of synapse
    typedef typename NodeType::SynapseType      SynapseType;

public:     // CTOR/DTOR

    NeuralNetwork()
    {
        // Speed optimization, template argument should not have a default
        nodeLayers_.reserve( RESERVE_LAYER_COUNT );
    }

    ~NeuralNetwork()
    {
        // Destroy the network
        unsigned int count = (unsigned int)nodeLayers_.size();
        for ( unsigned int i = 0; i < count; ++i )
        {
            destroyList( nodeLayers_[i] );
        }
    }

private:

    void destroyList( std::vector<NodeType*>* someList )
    {
        std::for_each( someList->begin(), someList->end(), NodePolicy::destroy );
        delete someList;
    }
public:     // Functions

    // Function name   : createNode
    // Description     : 
    // Return type     : NodeType* 
    // Argument        : int layer
    // Argument        : NodeType::SynapseType synapse
    NodeType* createNode( unsigned int layer, SynapseType synapse )
    {
        assert( layer < nodeLayers_.size() );

        NodePolicy& policy = *this;
        NodeType* newNode = policy.create( synapse );
        nodeLayers_[layer]->push_back( newNode );
        return newNode;
    }

    // Function name   : addLayer
    // Description     : 
    // Return type     : unsigned int, index of new layer
    unsigned int addLayer()
    {
        nodeLayers_.push_back( new NodeLayer );
        return (unsigned int)nodeLayers_.size() - 1;
    }

    // Function name   : getNodeLayer
    // Description     : 
    // Return type     : NodeLayer* 
    // Argument        : unsigned int index
    NodeLayer* getNodeLayer( unsigned int layer )
    {
        assert( layer < nodeLayers_.size() );
        return nodeLayers_[layer];
    }

    // Function name   : getLastLayer
    // Description     : 
    // Return type     : NodeLayer* 
    NodeLayer* getLastLayer()
    {
        assert( ! nodeLayers_.empty() );
        return nodeLayers_.back();
    }

    // Function name   : tick
    // Description     : 
    // Return type     : void 
    void tick()
    {
        std::for_each( nodeLayers_.begin(), nodeLayers_.end(), tickLayer );
    }

    // Function name   : int getLayerCount
    // Description     : 
    // Return type     : unsigned 
    unsigned int getLayerCount()
    {
        return (unsigned int)nodeLayers_.size();
    }

    // Function name   : reset
    // Description     : 
    // Return type     : void 
    void reset()
    {
        std::for_each( nodeLayers_.begin(), nodeLayers_.end(), resetLayer );
    }

protected:  // Functions
    static void tickLayer( NodeLayer* layer )
    {
        std::for_each( layer->begin(), layer->end(), std::mem_fun( &NodeType::tick ) );
    }
    
    static void resetLayer( NodeLayer* layer )
    {
        std::for_each( layer->begin(), layer->end(), std::mem_fun( &NodeType::reset ) );
    }

private:    // Variables
    NodeLayerList           nodeLayers_;
};

} // namespace nn

#endif // _NeuralNetwork_h_included
