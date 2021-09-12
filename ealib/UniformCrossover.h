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
			: ICrossoverOperator( TYPE_ID<T>, { 0.0f, 2, 2 } )
		{

		}


		~UniformCrossover()
		{

		}


		virtual void Execute( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs )
		{
			Execute_( numparents, parents, numchildren, children, attribs );
		}


		virtual void Execute( OreOreLib::Memory<const IChromosome*>& X, OreOreLib::Memory<IChromosome*>& T, const void* attribs )
		{
			Execute_( X, T, attribs );
		}



	private:


		// int/short crossover
		template < typename Type=T >
		std::enable_if_t< std::is_arithmetic_v<Type>, void >
		Execute_( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs )
		{
			const IChromosome* pParent1	= parents[0];
			const IChromosome* pParent2	= parents[1];
			IChromosome* pChild1		= children[0];
			IChromosome* pChild2		= children[1];

			for( int i =0; i<pChild1->Size(); ++i )
			{
				const DesignParameter& pDParam1	= pChild1->GetDesignParameter( i );// child1's design parameter
				const DesignParameter& pDParam2	= pChild2->GetDesignParameter( i );// child2's design parameter

				const auto& p1_i	= pParent1->GeneAs<Type>( i );
				const auto& p2_i	= pParent2->GeneAs<Type>( i );
				auto& c1_i			= pChild1->GeneAs<Type>( i );
				auto& c2_i			= pChild2->GeneAs<Type>( i );


				if( OreOreLib::genrand_real1() > 0.5 )// Crossover children
				{
					c1_i = p2_i;// 子供1のi番目遺伝子を、親2のi番目遺伝子で上書きする
					c2_i = p1_i;// 子供2のi番目遺伝子を、親1のi番目遺伝子で上書きする
				}
				else
				{
					c1_i = p1_i;
					c2_i = p2_i;
				}

				Clamp( c1_i, pDParam1.LowerBoundary<Type>(), pDParam1.UpperBoundary<Type>() );
				Clamp( c2_i, pDParam2.LowerBoundary<Type>(), pDParam2.UpperBoundary<Type>() );

			}// end of i loop

		}// end of func


		// bitstring crossover
		template < typename Type=T >
		std::enable_if_t< std::is_same_v<Type, BitArray>, void >
		Execute_( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs )
		{
			for( int i=0; i<(*parents)->Size(); ++i )
			{
				const auto& pParent1	= parents[0]->GeneAs<BitArray>( i );
				const auto& pParent2	= parents[1]->GeneAs<BitArray>( i );
				auto& pChild1			= children[0]->GeneAs<BitArray>( i );
				auto& pChild2			= children[1]->GeneAs<BitArray>( i );

				int bitLength = Min( Min( Min( pParent1.BitLength(), pParent2.BitLength() ), pChild1.BitLength() ), pChild2.BitLength() );

				pChild1.CopyFrom( &pParent1 );
				pChild2.CopyFrom( &pParent2 );

				for( int j = 0; j<bitLength; ++j )
				{
					if( OreOreLib::genrand_real1() > 0.5 )
					{
						pChild1.SetBit( j, pParent2.GetBit( j ) );
						pChild2.SetBit( j, pParent1.GetBit( j ) );
					}
				}// end of j loop

			}// end of i loop

		}



		template < typename Type=T >
		std::enable_if_t< std::is_arithmetic_v<Type>, void >
		Execute_( OreOreLib::Memory<const IChromosome*>& X, OreOreLib::Memory<IChromosome*>& T, const void* attribs )
		{
			const IChromosome* pParent1	= X[0]->GetChromosomeByType( TypeID );
			const IChromosome* pParent2	= X[1]->GetChromosomeByType( TypeID );
			IChromosome* pChild1		= T[0]->GetChromosomeByType( TypeID );
			IChromosome* pChild2		= T[1]->GetChromosomeByType( TypeID );

			for( int i =0; i<pChild1->Size(); ++i )
			{
				const DesignParameter& pDParam1	= pChild1->GetDesignParameter( i );// child1's design parameter
				const DesignParameter& pDParam2	= pChild2->GetDesignParameter( i );// child2's design parameter

				const auto& p1_i	= pParent1->GeneAs<Type>( i );
				const auto& p2_i	= pParent2->GeneAs<Type>( i );
				auto& c1_i			= pChild1->GeneAs<Type>( i );
				auto& c2_i			= pChild2->GeneAs<Type>( i );


				if( OreOreLib::genrand_real1() > 0.5 )// Crossover children
				{
					c1_i = p2_i;// 子供1のi番目遺伝子を、親2のi番目遺伝子で上書きする
					c2_i = p1_i;// 子供2のi番目遺伝子を、親1のi番目遺伝子で上書きする
				}
				else
				{
					c1_i = p1_i;
					c2_i = p2_i;
				}

				Clamp( c1_i, pDParam1.LowerBoundary<Type>(), pDParam1.UpperBoundary<Type>() );
				Clamp( c2_i, pDParam2.LowerBoundary<Type>(), pDParam2.UpperBoundary<Type>() );

			}// end of i loop

		}// end of func



		// bitstring crossover
		template < typename Type=T >
		std::enable_if_t< std::is_same_v<Type, BitArray>, void >
		Execute_( OreOreLib::Memory<const IChromosome*>& X, OreOreLib::Memory<IChromosome*>& T, const void* attribs )
		{
			const IChromosome* pParent1	= X[0]->GetChromosomeByType( TypeID );
			const IChromosome* pParent2	= X[1]->GetChromosomeByType( TypeID );
			IChromosome* pChild1		= T[0]->GetChromosomeByType( TypeID );
			IChromosome* pChild2		= T[1]->GetChromosomeByType( TypeID );


			for( int i=0; i<pParent1->Size(); ++i )
			{
				const auto& pBParent1	= pParent1->GeneAs<BitArray>( i );
				const auto& pBParent2	= pParent2->GeneAs<BitArray>( i );
				auto& pBChild1			= pChild1->GeneAs<BitArray>( i );
				auto& pBChild2			= pChild2->GeneAs<BitArray>( i );

				int bitLength = Min( Min( Min( pBParent1.BitLength(), pBParent2.BitLength() ), pBChild1.BitLength() ), pBChild2.BitLength() );

				pBChild1.CopyFrom( &pBParent1 );
				pBChild2.CopyFrom( &pBParent2 );

				for( int j = 0; j<bitLength; ++j )
				{
					if( OreOreLib::genrand_real1() > 0.5 )
					{
						pBChild1.SetBit( j, pBParent2.GetBit( j ) );
						pBChild2.SetBit( j, pBParent1.GetBit( j ) );
					}
				}// end of j loop

			}// end of i loop

		}



	};



}// end of namespace



#endif // !UNIFORM_CROSSOVER_H
