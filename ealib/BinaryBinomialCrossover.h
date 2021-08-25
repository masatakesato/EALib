#ifndef BINARY_BINOMINAL_CROSSOVER_H
#define	BINARY_BINOMINAL_CROSSOVER_H


#include	"ICrossoverOperator.h"


namespace ealib
{

	// Binomial crossover operator (for Binary Differential Evolution). BinomialCrossover<BitArray> よりもなぜか性能がいい
	class CLASS_DECLSPEC BinaryBinomialCrossover : public ICrossoverOperator
	{
	public:

		BinaryBinomialCrossover();
		~BinaryBinomialCrossover();

		virtual void Execute( int numchroms, IChromosome** chromosomes, const void* attribs );



virtual void Execute( int numparents, const IChromosome** parents, int numchildren, IChromosome** children, const void* attribs );

	private:


	};



}// end of namespace



#endif // !BINARY_BINOMINAL_CROSSOVER_H
