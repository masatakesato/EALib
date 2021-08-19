﻿#ifndef TWO_POINT_CROSSOVER_H
#define	TWO_POINT_CROSSOVER_H

#include	"ICrossoverOperator.h"
#include	"IChromosome.h"



namespace ealib
{

	// Forward declaration.
	template< typename T, typename enabled=void > class TwoPointCrossover;



	// Class implementation
	template< typename T >
	class /*CLASS_DECLSPEC*/ TwoPointCrossover< T, std::enable_if_t< (std::is_arithmetic_v<T> || std::is_same_v<T, BitArray>) > > : public ICrossoverOperator
	{
	public:

		TwoPointCrossover()
			: ICrossoverOperator( TYPE_ID<T> )
		{
			//static_assert( std::is_arithmetic_v<T> || std::is_same_v<T, BitArray> );// C++17 or above required.
		}


		~TwoPointCrossover()
		{

		}


		virtual void Execute( int numchroms, IChromosome** chromosomes, const void* attribs )
		{
			Execute_( numchroms, chromosomes, attribs );
		}



	private:


		// int/short crossover
		template< typename Type=T >
		std::enable_if_t< std::is_arithmetic_v<Type>, void >
		Execute_( int numchroms, IChromosome** chromosomes, const void* attribs )
		{
			const IChromosome* pParent1	= chromosomes[0];
			const IChromosome* pParent2	= chromosomes[1];
			IChromosome* pChild1		= chromosomes[2];
			IChromosome* pChild2		= chromosomes[3];

			int size = Min( Min( Min( pParent1->Size(), pParent2->Size() ), pChild1->Size() ), pChild2->Size() );
			const int N	= 2;
			int splicePoints[N]	={ int( OreOreLib::genrand_real2() * size ), int( OreOreLib::genrand_real2() * size ) };

			if( splicePoints[0] > splicePoints[1] )
			{
				int temp = splicePoints[0];
				splicePoints[0]	= splicePoints[1];
				splicePoints[1]	= temp;
			}


			pChild1->CopyGeneFrom( pParent2 );// pChild1を、pParent2の遺伝子で初期化
			pChild2->CopyGeneFrom( pParent1 );// pChild2を、pParent1の遺伝子で初期化

			for( int i=0; i<N; i+=2 )
			{
				int start	= splicePoints[i];
				int end		= i==N-1 ? size-1 : splicePoints[i+1];

				for( int j=start; j<=end; ++j )
				{
					// |++++ parent1 ++++|---- parent2 ----|++++ parent1 ++++|---...
					*pChild1->GeneAs<Type>( j )	= *pParent1->GeneAs<Type>( j );// pChild1をParent1で部分上書きする

					// |---- parent2 ----|++++ parent1 ++++|---- parent2 ----|+++...
					*pChild2->GeneAs<Type>( j )	= *pParent2->GeneAs<Type>( j );// pChild2をpParent2で部分上書きする
				}

			}// end of i loop


		}



		// bitstring crossover
		template< typename Type=T >
		std::enable_if_t< std::is_same_v<Type, BitArray>, void >
		Execute_( int numchroms, IChromosome** chromosomes, const void* attribs )
		{
			for( int i=0; i<chromosomes[0]->Size(); ++i )
			{
				const BitArray *pParent1	= chromosomes[0]->GeneAs<BitArray>(i);
				const BitArray *pParent2	= chromosomes[1]->GeneAs<BitArray>(i);
				BitArray *pChild1			= chromosomes[2]->GeneAs<BitArray>(i);
				BitArray *pChild2			= chromosomes[3]->GeneAs<BitArray>(i);

				int bitLength	= Min( Min( Min( pParent1->BitLength(), pParent2->BitLength() ), pChild1->BitLength() ), pChild2->BitLength() );
				int splicePoints[2]	={ int( OreOreLib::genrand_real2() * bitLength ), int( OreOreLib::genrand_real2() * bitLength ) };

				if( splicePoints[0] > splicePoints[1] )
				{
					int temp = splicePoints[0];
					splicePoints[0]	= splicePoints[1];
					splicePoints[1]	= temp;
				}

				// |++++ parent1 ++++|---- parent2 ----|++++ parent1 ++++|
				pChild1->CopyFrom( pParent1 );
				pChild1->CopyFrom( splicePoints[0], pParent2, splicePoints[0], splicePoints[1]-splicePoints[0] );

				// |---- parent2 ----|++++ parent1 ++++|---- parent2 ----|
				pChild2->CopyFrom( pParent2 );
				pChild2->CopyFrom( splicePoints[0], pParent1, splicePoints[0], splicePoints[1]-splicePoints[0] );
			}

		}


	};



}// end of namespace



#endif // !TWO_POINT_CROSSOVER_H
