#ifndef I_CROSSOVER_OPERATOR_H
#define	I_CROSSOVER_OPERATOR_H

#include	<oreore/common/DLLExport.h>
#include	<oreore/mathlib/MathLib.h>

#include	"DesignParameter.h"



namespace ealib
{
	class IChromosome;

	class CLASS_DECLSPEC ICrossoverOperator
	{
	public:

		const int16 TypeID;

		ICrossoverOperator( int16 type=-1 )
			: TypeID( type )
			, m_NumParents( 0 )
		{

		}


		virtual ~ICrossoverOperator()
		{
		
		}


		int NumParents() const
		{
			return m_NumParents;
		}


		virtual void Execute( int numchroms, IChromosome** chromosomes, const void* attribs )=0;



	protected:

		int m_NumParents = 0;


	};


}// end of namespace



#endif // !I_CROSSOVER_OPERATOR_H
