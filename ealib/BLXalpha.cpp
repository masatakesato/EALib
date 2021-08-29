#include	"BLXalpha.h"

#include	<oreore/mathlib/MathLib.h>
#include	<oreore/mathlib/MersenneTwister.h>

#include	"Typedefs.h"
#include	"IChromosome.h"



namespace ealib
{

	// BLX-alpha
	static float blx_alpha( float p1, float p2, float alpha = 0.5f )
	{
		float u = (float)OreOreLib::genrand_real1();
		float gamma = (1.0f + 2.0f * alpha) * u - alpha;

		return (1.0f - gamma) * p1 + gamma * p2;
	}




	BLXalpha::BLXalpha()
		: ICrossoverOperator( TYPE_ID<float>, { 0.0f, 2, 2 } )
	{

	}
		
	

	BLXalpha::~BLXalpha()
	{

	}



	void BLXalpha::Execute( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs )
	{
		const IChromosome* pParent1	= parents[0];
		const IChromosome* pParent2	= parents[1];

		IChromosome* pChild1	= children[0];
		IChromosome* pChild2	= children[1];

		for( int i =0; i<pChild1->Size(); ++i )
		{
			DesignParameter* pDParam1 = pChild1->GetDesignParameter( i );// child1's design parameter
			DesignParameter* pDParam2 = pChild2->GetDesignParameter( i );// child2's design parameter

			float *p1_i	= pParent1->GeneAs<float>( i );
			float *p2_i	= pParent2->GeneAs<float>( i );
			float *c1_i	= pChild1->GeneAs<float>( i );
			float *c2_i	= pChild2->GeneAs<float>( i );

			*c1_i	= Clamp( blx_alpha(*p1_i, *p2_i), pDParam1->LowerBoundary<float>(), pDParam1->UpperBoundary<float>() );
			*c2_i	= Clamp( blx_alpha(*p1_i, *p2_i), pDParam2->LowerBoundary<float>(), pDParam2->UpperBoundary<float>() );

		}// end of i loop

	}



	void BLXalpha::Execute( OreOreLib::Memory<const IChromosome*>& X, OreOreLib::Memory<IChromosome*>& T, const void* attribs )
	{
		const IChromosome* pParent1	= X[0]->GetChromosomeByType( TypeID );
		const IChromosome* pParent2	= X[1]->GetChromosomeByType( TypeID );
		IChromosome* pChild1		= T[0]->GetChromosomeByType( TypeID );
		IChromosome* pChild2		= T[1]->GetChromosomeByType( TypeID );

		for( int i =0; i<pChild1->Size(); ++i )
		{
			DesignParameter* pDParam1 = pChild1->GetDesignParameter( i );// child1's design parameter
			DesignParameter* pDParam2 = pChild2->GetDesignParameter( i );// child2's design parameter

			float* p1_i	= pParent1->GeneAs<float>( i );
			float* p2_i	= pParent2->GeneAs<float>( i );
			float* c1_i	= pChild1->GeneAs<float>( i );
			float* c2_i	= pChild2->GeneAs<float>( i );

			*c1_i	= Clamp( blx_alpha(*p1_i, *p2_i), pDParam1->LowerBoundary<float>(), pDParam1->UpperBoundary<float>() );
			*c2_i	= Clamp( blx_alpha(*p1_i, *p2_i), pDParam2->LowerBoundary<float>(), pDParam2->UpperBoundary<float>() );

		}// end of i loop

	}



}// end of namespace