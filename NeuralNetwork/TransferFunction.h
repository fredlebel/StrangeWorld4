#ifndef _TransferFunction_h_included_
#define _TransferFunction_h_included_
#pragma once

namespace nn
{
    template <class T, T min, T max>
    class TransferFunction_Linear
    {
    public:
        TransferFunction_Linear()
        {
        }

        inline T compute( T sum )
        {
            if ( sum > max )
                sum = max;
            else if ( sum < min )
                sum = min;
            return sum;
        }

    protected:
        ~TransferFunction_Linear() {}
    };

    //=======================================
    template <typename T, T asymptote, T maxSignal>
    class TransferFunction_nxtx_Linear
    {
    public:
//      static const double _factor = (double)asymptote / (double)maxSignal;
        TransferFunction_nxtx_Linear()
        {
        }

        inline T compute( T sum )
        {
            sum = (T)( asymptote * ( (double)sum / maxSignal ) );
//          sum = _factor * sum;
            if ( sum > asymptote )
                sum = asymptote;
            else if ( sum < -asymptote )
                sum = -asymptote;
            return sum;
        }
    protected:
        ~TransferFunction_nxtx_Linear() {}
    };
    //=======================================

    //=======================================
    template <unsigned int signalCount>
    class TransferFunction_Sigmoid_n1t1
    {
    public:
        TransferFunction_Sigmoid_n1t1()
        {
        }

        inline double compute( double sum )
        {
            sum = ( sum / (double)signalCount );
            sum = std::max( sum, -1.0 );
            sum = std::min( sum, 1.0 );
            return sum;
        }
    protected:
        ~TransferFunction_Sigmoid_n1t1() {}
    };
    //=======================================


//  template <SigmoidFunctionType sigtype, typename T, T asymptote> class TransferFunction_Sigmoid;

    template <typename T, T asymptote, T count>
    class TransferFunction_Sigmoid_nxtx
    {
    public:
        TransferFunction_Sigmoid_nxtx()
        {
        }

        inline T compute( T sum )
        {
/*          if (sum >= 0)
                sum = asymptote - asymptote * (asymptote / ((asymptote*count) + sum));
            else
                sum = -asymptote + asymptote * (asymptote / ((asymptote*count) - sum));
*/
            T maxSum = count * asymptote * asymptote / 100;
            sum = (T)( asymptote * ( (double)sum / maxSum ) );
            if (sum > 0)
                sum = std::min( sum, asymptote );
            else
                sum = std::max( sum, -asymptote );
            return sum;
        }
    protected:
        ~TransferFunction_Sigmoid_nxtx() {}
    };

    template <typename T, T asymptote>
    class TransferFunction_Sigmoid_ztx
    {
    public:
        TransferFunction_Sigmoid_ztx()
        {
        }

        inline T compute( T sum )
        {
            sum = asymptote / ( asymptote + ::exp( -sum ) );
            return sum;
        }
    protected:
        ~TransferFunction_Sigmoid_ztx() {}
    };
}

#endif //_TransferFunction_h_included_