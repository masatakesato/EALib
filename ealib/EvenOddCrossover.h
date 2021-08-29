#ifndef EVEN_ODD_CROSSOVER_H
#define	EVEN_ODD_CROSSOVER_H


#include	"ICrossoverOperator.h"


namespace ealib
{

	// Even odd crossover operator (for binary coded GA)
	class CLASS_DECLSPEC EvenOddCrossover : public ICrossoverOperator
	{
	public:

		EvenOddCrossover();
		~EvenOddCrossover();

		virtual void Execute( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs );
		virtual void Execute( OreOreLib::Memory<const IChromosome*>& X, OreOreLib::Memory<IChromosome*>& T, const void* attribs );

	};



}// end of namespace



#endif // !EVEN_ODD_CROSSOVER_H
