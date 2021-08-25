#ifndef BDE_CROSSOVER_H
#define	BDE_CROSSOVER_H


#include	"ICrossoverOperator.h"


namespace ealib
{

	// Binary Differentioal Evolution crossover operator (for binary coded DE)
	class CLASS_DECLSPEC BDECrossover : public ICrossoverOperator
	{
	public:

		BDECrossover();
		~BDECrossover();

		virtual void Execute( int numchroms, IChromosome** chromosomes, const void* attribs );

		virtual void Execute( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs );
	};



}// end of namespace



#endif // !BDE_CROSSOVER_H
