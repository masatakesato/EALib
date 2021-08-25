﻿#ifndef BINARY_EXPONENTIAL_CROSSOVER_H
#define	BINARY_EXPONENTIAL_CROSSOVER_H


#include	"ICrossoverOperator.h"


namespace ealib
{

	// Exponential crossover operator (for Binary Differential Evolution). ExponentialCrossover<BitArray> よりもなぜか性能がいい
	class CLASS_DECLSPEC BinaryExponentialCrossover : public ICrossoverOperator
	{
	public:

		BinaryExponentialCrossover();
		~BinaryExponentialCrossover();

		virtual void Execute( int numchroms, IChromosome** chromosomes, const void* attribs );


virtual void Execute( int numparents, const IChromosome** parents, int numchildren, IChromosome** children, const void* attribs );

	private:


	};



}// end of namespace



#endif // !BINARY_EXPONENTIAL_CROSSOVER_OP_H
