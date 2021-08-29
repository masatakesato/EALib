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

		virtual void Execute( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs );
		virtual void Execute( OreOreLib::Memory<const IChromosome*>& X, OreOreLib::Memory<IChromosome*>& T, const void* attribs );

	};



}// end of namespace



#endif // !SBX_H
