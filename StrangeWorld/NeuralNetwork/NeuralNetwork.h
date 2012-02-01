#ifndef _NeuralNetwork_h_included_
#define _NeuralNetwork_h_included_

#include <vector>
#include <algorithm>

class NNGene;

class NeuralNetwork
{
public:
	static std::auto_ptr<NeuralNetwork> buildFromGene( int inputCount, NNGene* gene );
	static double BiasMin;
	static double BiasMax;

	NeuralNetwork(int neuronCount, int inputCount);

	void connectIn(int fromIndex, int toIndex, double bias);
	void connectOut(int fromIndex, int toIndex, double bias);

	void push (int index, double value);
	double pop(int index);

	void tick();

private:
	struct Connection
	{
		Connection(double* i, double b)
			: in(i)
			, bias(b)
		{
		}
		double* in;
		double bias;
	};

	struct Neuron
	{
		Neuron() : out(0)
		{
		}

		std::vector<Connection> ins;
		double out;

		void tick()
		{
			out = 0.0;
			for (auto it = ins.begin(); it != ins.end(); ++it)
			{
				out += *(it->in) * it->bias;
			}
		}
	};

	std::vector<Neuron> _neurons;
	std::vector<double> _inputs;
};


#endif //_NeuralNetwork_h_included_