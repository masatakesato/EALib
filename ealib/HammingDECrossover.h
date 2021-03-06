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

		virtual void Execute( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs );
		virtual void Execute( OreOreLib::Memory<const IChromosome*>& X, OreOreLib::Memory<IChromosome*>& T, const void* attribs );

	};



}// end of namespace



#endif // !HAMMING_DE_CROSSOVER_H
