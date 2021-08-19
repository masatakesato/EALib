#ifndef BLX_ALPHA_H
#define	BLX_ALPHA_H


#include	"ICrossoverOperator.h"


namespace ealib
{

	// Blend Crossover-alpha operator
	class CLASS_DECLSPEC BLXalpha : public ICrossoverOperator
	{
	public:

		BLXalpha();
		~BLXalpha();

		virtual void Execute( int numchroms, IChromosome** chromosomes, const void* attribs );


	private:


	};



}// end of namespace



#endif // !BLX_ALPHA_H
