#include	"SBX.h"

#include	<oreore/mathlib/MathLib.h>
#include	<oreore/mathlib/MersenneTwister.h>

#include	"IChromosome.h"



namespace ealib
{

	static void sbx( float p1, float p2, float& c1, float& c2, float n=2.0f )
	{
		const float epsilon	= 1.0e-6f;
		float u	= (float)OreOreLib::genrand_real1();

		//============ Calculate beta	=============//
		float beta;

		if( 1.0f-u < epsilon )	u = 1.0f - epsilon;
		if( u < 0.0f )			u = 0.0f;
		if( u <0.5f )			beta = pow( 2.0f*u, 1.0f/( n+1.0f ) );
		else					beta = pow( 0.5f/( 1.0f-u ), 1.0f/( n+1.0f ) );

		//============ Calculate children	=========//
		c1	= 0.5f * ( ( 1.0f + beta ) * p1 + ( 1.0f - beta ) * p2 );
		c2	= 0.5f * ( ( 1.0f - beta ) * p1 + ( 1.0f + beta ) * p2 );
	}





	SBX::SBX()
		: ICrossoverOperator( TYPE_ID<float>, { 0.0f, 2, 2 } )
	{

	}



	SBX::~SBX()
	{

	}



	void SBX::Execute( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs )
	{
		const IChromosome* pParent1	= parents[0];
		const IChromosome* pParent2	= parents[1];

		IChromosome* pChild1	= children[2];
		IChromosome* pChild2	= children[3];

		for( int i=0; i<pChild1->Size(); ++i )
		{
			DesignParameter* pDParam1	= pChild1->GetDesignParameter( i );// child1's design parameters
			DesignParameter* pDParam2	= pChild2->GetDesignParameter( i );// child2's design parameters

			float* p1_i	= pParent1->GeneAs<float>( i );
			float* p2_i	= pParent2->GeneAs<float>( i );
			float* c1_i	= pChild1->GeneAs<float>( i );
			float* c2_i	= pChild2->GeneAs<float>( i );

			float child1=0, child2=0;
			sbx( *p1_i, *p2_i, child1, child2 );

			*c1_i	= Clamp( child1, pDParam1->LowerBoundary<float>(), pDParam1->UpperBoundary<float>() );
			*c2_i	= Clamp( child2, pDParam2->LowerBoundary<float>(), pDParam2->UpperBoundary<float>() );

		}// end of i loop
	}


	
	void SBX::Execute( OreOreLib::Memory<const IChromosome*>& X, OreOreLib::Memory<IChromosome*>& T, const void* attribs )
	{
		const IChromosome* pParent1	= X[0]->GetChromosomeByType( TypeID );
		const IChromosome* pParent2	= X[1]->GetChromosomeByType( TypeID );
		IChromosome* pChild1		= T[0]->GetChromosomeByType( TypeID );
		IChromosome* pChild2		= T[1]->GetChromosomeByType( TypeID );

		for( int i=0; i<pChild1->Size(); ++i )
		{
			DesignParameter* pDParam1	= pChild1->GetDesignParameter( i );// child1's design parameters
			DesignParameter* pDParam2	= pChild2->GetDesignParameter( i );// child2's design parameters

			float* p1_i	= pParent1->GeneAs<float>( i );
			float* p2_i	= pParent2->GeneAs<float>( i );
			float* c1_i	= pChild1->GeneAs<float>( i );
			float* c2_i	= pChild2->GeneAs<float>( i );

			float child1=0, child2=0;
			sbx( *p1_i, *p2_i, child1, child2 );

			*c1_i	= Clamp( child1, pDParam1->LowerBoundary<float>(), pDParam1->UpperBoundary<float>() );
			*c2_i	= Clamp( child2, pDParam2->LowerBoundary<float>(), pDParam2->UpperBoundary<float>() );

		}// end of i loop
	}


}// end of namespace