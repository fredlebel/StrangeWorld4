#ifndef _SummationFunction_h_included_
#define _SummationFunction_h_included_
#pragma once

#include <assert.h>

namespace nn
{
    template <class T, T Null_T, class WeightType>
    class SummationFunction_Sum
    {
    public:
        SummationFunction_Sum() 
            : total_( Null_T )
        {
        }

        inline void push( T synapse, WeightType weight )
        {
            total_ += synapse * weight;
        }

        inline T pop()
        {
            T ret = total_;
            total_ = (T)0;
            return ret;
        }

    protected:
        ~SummationFunction_Sum() {}
    private:
        T       total_;
    };

    template <class T, T Null_T, class WeightType>
    class SummationFunction_Avg
    {
    public:
        SummationFunction_Avg() 
            : total_( Null_T )
            , count_( 0 )
        {
        }

        inline void push( T synapse, WeightType weight )
        {
            total_ += synapse * weight;
            ++count_;
        }

        inline T pop()
        {
            T ret;
            if ( count_ )
                ret = total_ / count_;
            else
                ret = Null_T;
            total_ = Null_T;
            count_ = 0;
            return ret;
        }

    protected:
        ~SummationFunction_Avg() {}
    private:
        T       total_;
        int     count_;
    };

    template <class SynapseType, class WeightType>
    class SummationFunction_WeightAvg
    {
    public:
        SummationFunction_WeightAvg() 
            : total_( (SynapseType)0 )
            , totalWeight_( (WeightType)0 )
            , count_( 0 )
        {
        }

        inline void push( SynapseType synapse, WeightType weight )
        {
            total_ += synapse * weight;
            totalWeight_ += weight;//( weight >= 0 ? weight : weight * -1 );
            ++count_;
        }

        inline SynapseType pop()
        {
            SynapseType ret;
            if ( count_ > 0 )
            {
                if ( totalWeight_ != 0 )
                    ret = total_ / totalWeight_;
                else
                    ret = total_ / count_;
            }
            else
                ret = (SynapseType)0;
            // Init for the next series of pushes
            total_ = (SynapseType)0;
            totalWeight_ = (WeightType)0;
            count_ = 0;
            return ret;
        }

    protected:
        ~SummationFunction_WeightAvg() {}

    private:
        SynapseType total_;
        WeightType  totalWeight_;
        int         count_;
    };

}

#endif //_SummationFunction_h_included_