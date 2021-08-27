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
//			, m_NumActiveParents( 0 )
//			, m_NumActiveChildren( 0 )
		{

		}


		ICrossoverOperator( int16 type, const CrossoverAttribute& attrib )
			: TypeID( type )
			, m_Attrib( attrib )
//			, m_NumActiveParents( 0 )
//			, m_NumActiveChildren( 0 )
		{

		}


		virtual ~ICrossoverOperator()
		{
//			m_refParents.Release();
//			m_refChildren.Release();
		}


		//void Init( int numMaxParents, int numMaxChildren )
		//{
		//	assert( numMaxParents>0 && numMaxChildren>0 );

		//	m_refParents.Init( numMaxParents );
		//	m_refChildren.Init( numMaxChildren );
		//}


		const CrossoverAttribute& Attribute() const
		{
			return m_Attrib;
		}


		virtual void Execute( int, IChromosome**, const void* )=0;



		virtual void Execute( int, const IChromosome*[], int, IChromosome*[], const void* )=0;


		virtual void Execute( OreOreLib::Memory<const IChromosome*>&, OreOreLib::Memory<IChromosome*>&, const void* )=0;



		//bool BindParent( const IChromosome* parent )
		//{
		//	if( m_NumActiveParents==m_refParents.Length() )
		//		return false
		//	m_refParents[ m_NumActiveParents++ ] = parent;
		//}

		//
		//bool BindChild( IChromosome* child )
		//{
		//	m_refChildren[ m_NumActiveChildren++ ] = child;
		//}


		//void Clear()
		//{
		//	m_refParents.Release();
		//	m_refChildren.Release();
		//}



	protected:

		CrossoverAttribute	m_Attrib;


		//int m_NumActiveParents = 0;
		//int m_NumActiveChildren = 0;

		//OreOreLib::Array<const IChromosome*>	m_refParents;
		//OreOreLib::Array<IChromosome*>			m_refChildren;


	};


}// end of namespace



#endif // !I_CROSSOVER_OPERATOR_H
