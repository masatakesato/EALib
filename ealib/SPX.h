#ifndef SPX_H
#define	SPX_H


#include	"ICrossoverOperator.h"


namespace ealib
{

	// Simplex Crossover
	class CLASS_DECLSPEC SPX : public ICrossoverOperator
	{
	public:

		SPX();
		~SPX();

		virtual void Execute( int numchroms, IChromosome** chromosomes, const void* attribs );


	private:


	};



}// end of namespace



#endif // !SPX_H
