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
		: ICrossoverOperator( TYPE_ID<float> )
	{

	}



	SBX::~SBX()
	{

	}



	void SBX::Execute( int numchroms, IChromosome** chromosomes, const void* attribs )
	{
		IChromosome* pParent1	= chromosomes[0];
		IChromosome* pParent2	= chromosomes[1];

		IChromosome* pChild1	= chromosomes[2];
		IChromosome* pChild2	= chromosomes[3];

		for( int i =0; i<pChild1->Size(); ++i )
		{
			DesignParameter* pDParam1	= chromosomes[2]->GetDesignParameter( i );// child1's design parameters
			DesignParameter* pDParam2	= chromosomes[3]->GetDesignParameter( i );// child2's design parameters

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