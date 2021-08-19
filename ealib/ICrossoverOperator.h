#ifndef I_CROSSOVER_OPERATOR_H
#define	I_CROSSOVER_OPERATOR_H

#include	"DesignParameter.h"
#include	"DLLExport.h"

#include	<oreore/mathlib/MathLib.h>



namespace ealib
{
	class IChromosome;

	class CLASS_DECLSPEC ICrossoverOperator
	{
	public:

		const int16 TypeID;

		ICrossoverOperator( int16 type=-1 )
			: TypeID( type )
		{

		}

		virtual ~ICrossoverOperator()
		{
		
		}

		virtual void Execute( int numchroms, IChromosome** chromosomes, const void* attribs )=0;

	};


}// end of namespace



#endif // !I_CROSSOVER_OPERATOR_H
