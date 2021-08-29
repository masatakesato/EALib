#ifndef I_CROSSOVER_OPERATOR_H
#define	I_CROSSOVER_OPERATOR_H

#include	<oreore/common/DLLExport.h>
#include	<oreore/mathlib/MathLib.h>

#include	"DesignParameter.h"



namespace ealib
{
	class IChromosome;



	struct CrossoverAttribute
	{
		float	CrossoverRate;	// [0.0, 1.0]

		// parent/children size. set -1 if dynamic.
		int		NumParents;
		int		NumChildren;

		void Clear()
		{
			CrossoverRate	= 0.0f;
			NumParents		= -1;
			NumChildren		= -1;
		}

		static const int Dynamic = -1;
	};





	class CLASS_DECLSPEC ICrossoverOperator
	{
	public:

		const int16 TypeID;

		ICrossoverOperator( )
			: TypeID( TYPE_UNKNOWN )
			, m_Attrib(  )
		{

		}


		ICrossoverOperator( int16 type, const CrossoverAttribute& attrib )
			: TypeID( type )
			, m_Attrib( attrib )
		{

		}


		virtual ~ICrossoverOperator()
		{

		}


		const CrossoverAttribute& Attribute() const
		{
			return m_Attrib;
		}


		virtual void Execute( int, const IChromosome*[], int, IChromosome*[], const void* )=0;
		virtual void Execute( OreOreLib::Memory<const IChromosome*>&, OreOreLib::Memory<IChromosome*>&, const void* )=0;



	protected:

		CrossoverAttribute	m_Attrib;

	};


}// end of namespace



#endif // !I_CROSSOVER_OPERATOR_H
