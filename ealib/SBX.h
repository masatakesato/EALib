#ifndef SBX_H
#define	SBX_H


#include	"ICrossoverOperator.h"


namespace ealib
{

	// Simulated Binary Crossover operator
	class CLASS_DECLSPEC SBX : public ICrossoverOperator
	{
	public:

		SBX();
		~SBX();

		virtual void Execute( int numchroms, IChromosome** chromosomes, const void* attribs );


	private:


	};



}// end of namespace



#endif // !SBX_H
