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

		
		void Execute( int numparents, const IChromosome** parents, int numchildren, IChromosome** children, const void* attribs=nullptr );

void Execute2( OreOreLib::Memory<const IChromosome*>& parents, OreOreLib::Memory<IChromosome*>& children, const void* attribs=nullptr );

		int NumParents( int type ) const;
		int NumChildren( int type ) const;


	private:

		ICrossoverOperator*	m_refOperators[ NUM_TYPES ];

	};



}// end of namespace



#endif // !CROSSOVER_H
