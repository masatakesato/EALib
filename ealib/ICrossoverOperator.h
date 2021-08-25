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


		virtual void Execute( int numchroms, IChromosome** chromosomes, const void* attribs )=0;



		virtual void Execute( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs )=0;



		//void BindParents( int numparents, const IChromosome* parents[] )
		//{
		//	m_refParents.Init( parents, numparents );
		//}

		//
		//void BindChildren( int numchildren, IChromosome* children[] )
		//{
		//	m_refChildren.Init( children, numchildren );
		//}


		//void Clear()
		//{
		//	m_refParents.Release();
		//	m_refChildren.Release();
		//}




	protected:

		CrossoverAttribute	m_Attrib;


		//int m_NumParents = 0;
		//int m_NumChildren = 0;

		//OreOreLib::ArrayyView<const IChromosome*>	m_refParents;
		//OreOreLib::ArraryView<IChromosome*>	m_refChildren;


	};


}// end of namespace



#endif // !I_CROSSOVER_OPERATOR_H
