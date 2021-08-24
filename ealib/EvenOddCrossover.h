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

		virtual void Execute( int numchroms, IChromosome** chromosomes, const void* attribs );
	};



}// end of namespace



#endif // !EVEN_ODD_CROSSOVER_H
