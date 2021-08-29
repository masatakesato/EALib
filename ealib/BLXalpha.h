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

		virtual void Execute( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs );
		virtual void Execute( OreOreLib::Memory<const IChromosome*>& X, OreOreLib::Memory<IChromosome*>& T, const void* attribs );

	};



}// end of namespace



#endif // !BLX_ALPHA_H
