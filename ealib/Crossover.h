#ifndef CROSSOVER_H
#define	CROSSOVER_H

#include	<oreore/common/DLLExport.h>

#include	"Typedefs.h"
#include	"ICrossoverOperator.h"



namespace ealib
{
	class IChromosome;

	class CLASS_DECLSPEC Crossover
	{
	public:

		Crossover();
		virtual ~Crossover();

		void BindOperator( ICrossoverOperator* pOperator );
		void UnbindOperator( int type );
		void Execute( int numchroms, IChromosome** chromosomes, const void* attribs=nullptr );


	private:

		ICrossoverOperator*	m_refOperators[ NUM_TYPES ];

	};



}// end of namespace



#endif // !CROSSOVER_H
