#ifndef HAMMING_DE_CROSSOVER_H
#define	HAMMING_DE_CROSSOVER_H


#include	"ICrossoverOperator.h"


namespace ealib
{

	// Hamming DE crossover operator (for Binary Differential Evolution).
	class CLASS_DECLSPEC HammingDECrossover : public ICrossoverOperator
	{
	public:

		HammingDECrossover();
		~HammingDECrossover();

		virtual void Execute( int numchroms, IChromosome** chromosomes, const void* attribs );


	private:


	};



}// end of namespace



#endif // !HAMMING_DE_CROSSOVER_H
