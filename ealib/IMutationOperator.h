#ifndef I_MUTATION_OPERATOR_H
#define	I_MUTATION_OPERATOR_H

#include	<oreore/common/DLLExport.h>

#include	"DesignParameter.h"



namespace ealib
{
	class IChromosome;

	class CLASS_DECLSPEC IMutationOperator
	{
	public:

		const int16 TypeID;

		IMutationOperator( int type=-1 )
			: TypeID( type )
		{

		}


		virtual ~IMutationOperator()
		{

		}


		virtual void Execute( IChromosome* chromosome, float mutate_prob, const void* attribs=nullptr )=0;
		virtual void Execute( int numchroms, IChromosome** chromosomes, float mutate_prob, const void* attribs=nullptr )=0;

	};



}// end of namespace



#endif // !I_MUTATION_OPERATOR_H
