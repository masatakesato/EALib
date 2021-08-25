#include	"SPX.h"

#include	<oreore/mathlib/MathLib.h>
#include	<oreore/mathlib/MersenneTwister.h>

//#include	"IChromosome.h"
#include	"Chromosome1DFactory.h"



namespace ealib
{
	const Chromosome1DFactory<g_ChoromosomeTypes>	c_Factory;



	SPX::SPX( const DesignParamArray& designParams )
		: ICrossoverOperator( TYPE_ID<float>, { 0.0f, designParams.Length()+1, 1 } )
		, m_CenterOfMass( c_Factory.Create( designParams ) )
		, m_Vertices( designParams.Length() + 1 )
	{
		for( int i=0; i<m_Vertices.Length(); ++i )
			m_Vertices[i] = m_CenterOfMass->Clone();
	}



	SPX::~SPX()
	{
		Release();
	}
	


	void SPX::Init( const DesignParamArray& designParams )
	{
		Release();

		m_CenterOfMass	= c_Factory.Create( designParams );

		m_Vertices.Init( designParams.Length() + 1 );
		for( int i=0; i<m_Vertices.Length(); ++i )
			m_Vertices[i] = m_CenterOfMass->Clone();
	}



	void SPX::Release()
	{
		for( int i=0; i<m_Vertices.Length(); ++i )
			SafeDelete( m_Vertices[i] );

		m_Vertices.Release();

		SafeDelete( m_CenterOfMass );
	}



	void SPX::Execute( int numchroms, IChromosome** chromosomes, const void* attribs )
	{
		//int numParents = chromosomes[0]->Size() + 1;
		//int numChildren = numchroms - numParents;

		//assert( numChildren > 0 );

		//IChromosome** pTrials	= &chromosomes[ 0 ];
		//IChromosome** pParents	= &chromosomes[ numChildren ];


		// C = Center of Mass( pParents )

		// scale polygon by epsilon// Pi = epsilon * ( pParents[i] - G )


		// random sample polygon surface


// TODO: Refactor ICrossroverOperator to preallocate Pi.


	}




void SPX::Execute( int numparents, const IChromosome** parents, int numchildren, IChromosome** children, const void* attribs )
{
// TODO: Implement

}




}// end of namespace