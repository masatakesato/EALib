#ifndef UNIFORM_CROSSOVER_H
#define	UNIFORM_CROSSOVER_H

#include	<oreore/mathlib/MersenneTwister.h>

#include	"ICrossoverOperator.h"
#include	"IChromosome.h"




namespace ealib
{
	// Uniform crossover operator (for binary coded GA)


	// Forward declaration
	template< typename T, typename enabled=void > class UniformCrossover;


	// Class implementation
	template< typename T >
	class /*CLASS_DECLSPEC*/ UniformCrossover< T, std::enable_if_t< ( std::is_arithmetic_v<T> || std::is_same_v<T, BitArray> ) > > : public ICrossoverOperator
	{
	public:

		UniformCrossover()
			: ICrossoverOperator( TYPE_ID<T> )
		{

		}


		~UniformCrossover()
		{

		}


		virtual void Execute( int numchroms, IChromosome** chromosomes, const void* attribs )
		{
			Execute_( numchroms, chromosomes, attribs );
		}



	private:

		// int/short crossover
		template < typename Type=T >
		std::enable_if_t< std::is_arithmetic_v<Type>, void >
		Execute_( int numchroms, IChromosome** chromosomes, const void* attribs )
		{
			const IChromosome* pParent1	= chromosomes[0];
			const IChromosome* pParent2	= chromosomes[1];
			IChromosome* pChild1		= chromosomes[2];
			IChromosome* pChild2		= chromosomes[3];

			for( int i =0; i<pChild1->Size(); ++i )
			{
				DesignParameter* pDParam1	= pChild1->GetDesignParameter( i );// child1's design parameter
				DesignParameter* pDParam2	= pChild2->GetDesignParameter( i );// child2's design parameter

				Type* p1_i					= pParent1->GeneAs<Type>( i );
				Type* p2_i					= pParent2->GeneAs<Type>( i );
				Type* c1_i					= pChild1->GeneAs<Type>( i );
				Type* c2_i					= pChild2->GeneAs<Type>( i );


				if( OreOreLib::genrand_real1() > 0.5 )// Crossover children
				{
					*c1_i	= *p2_i;// 子供1のi番目遺伝子を、親2のi番目遺伝子で上書きする
					*c2_i	= *p1_i;// 子供2のi番目遺伝子を、親1のi番目遺伝子で上書きする
				}
				else
				{
					*c1_i	= *p1_i;
					*c2_i	= *p2_i;
				}

				Clamp( *c1_i, pDParam1->LowerBoundary<Type>(), pDParam1->UpperBoundary<Type>() );
				Clamp( *c2_i, pDParam2->LowerBoundary<Type>(), pDParam2->UpperBoundary<Type>() );

			}// end of i loop

		}// end of func



		// bitstring crossover
		template < typename Type=T >
		std::enable_if_t< std::is_same_v<Type, BitArray>, void >
		Execute_( int numchroms, IChromosome** chromosomes, const void* attribs )
		{
			for( int i=0; i<chromosomes[0]->Size(); ++i )
			{
				const BitArray* pParent1	= chromosomes[0]->GeneAs<BitArray>( i );
				const BitArray* pParent2	= chromosomes[1]->GeneAs<BitArray>( i );
				BitArray* pChild1			= chromosomes[2]->GeneAs<BitArray>( i );
				BitArray* pChild2			= chromosomes[3]->GeneAs<BitArray>( i );

				int bitLength = Min( Min( Min( pParent1->BitLength(), pParent2->BitLength() ), pChild1->BitLength() ), pChild2->BitLength() );

				pChild1->CopyFrom( pParent1 );
				pChild2->CopyFrom( pParent2 );

				for( int j = 0; j<bitLength; ++j )
				{
					if( OreOreLib::genrand_real1() > 0.5 )
					{
						pChild1->SetBit( j, pParent2->GetBit( j ) );
						pChild2->SetBit( j, pParent1->GetBit( j ) );
					}
				}// end of j loop

			}// end of i loop

		}


	};



}// end of namespace



#endif // !UNIFORM_CROSSOVER_H
