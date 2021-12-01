#ifndef TWO_POINT_CROSSOVER_H
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
			: ICrossoverOperator( TYPE_ID<T>, { 0.0f, 2, 2 } )
		{
			//static_assert( std::is_arithmetic_v<T> || std::is_same_v<T, BitArray> );// C++17 or above required.
		}


		~TwoPointCrossover()
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
		template< typename Type=T >
		std::enable_if_t< std::is_arithmetic_v<Type>, void >
		Execute_( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs )
		{
			const IChromosome* pParent1	= parents[0];
			const IChromosome* pParent2	= parents[1];
			IChromosome* pChild1		= children[0];
			IChromosome* pChild2		= children[1];

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
					pChild1->GeneAs<Type>( j )	= pParent1->GeneAs<Type>( j );// pChild1をParent1で部分上書きする

					// |---- parent2 ----|++++ parent1 ++++|---- parent2 ----|+++...
					pChild2->GeneAs<Type>( j )	= pParent2->GeneAs<Type>( j );// pChild2をpParent2で部分上書きする
				}

			}// end of i loop

		}


		// bitstring crossover
		template< typename Type=T >
		std::enable_if_t< std::is_same_v<Type, BitArray>, void >
		Execute_( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs )
		{
			for( int i=0; i<(*parents)->Size(); ++i )
			{
				const auto& pParent1	= parents[0]->GeneAs<BitArray>(i);
				const auto& pParent2	= parents[1]->GeneAs<BitArray>(i);
				auto& pChild1			= children[0]->GeneAs<BitArray>(i);
				auto& pChild2			= children[1]->GeneAs<BitArray>(i);

				int32 bitLength			= static_cast<int32>( Min( Min( Min( pParent1.BitLength(), pParent2.BitLength() ), pChild1.BitLength() ), pChild2.BitLength() ) );
				int32 splicePoints[2]	= { int32( OreOreLib::genrand_real2() * bitLength ), int32( OreOreLib::genrand_real2() * bitLength ) };

				if( splicePoints[0] > splicePoints[1] )
				{
					int32 temp = splicePoints[0];
					splicePoints[0]	= splicePoints[1];
					splicePoints[1]	= temp;
				}

				// |++++ parent1 ++++|---- parent2 ----|++++ parent1 ++++|
				pChild1.CopyFrom( &pParent1 );
				pChild1.CopyFrom( splicePoints[0], &pParent2, splicePoints[0], splicePoints[1]-splicePoints[0] );

				// |---- parent2 ----|++++ parent1 ++++|---- parent2 ----|
				pChild2.CopyFrom( &pParent2 );
				pChild2.CopyFrom( splicePoints[0], &pParent1, splicePoints[0], splicePoints[1]-splicePoints[0] );
			}

		}



		// int/short crossover
		template< typename Type=T >
		std::enable_if_t< std::is_arithmetic_v<Type>, void >
		Execute_( OreOreLib::Memory<const IChromosome*>& X, OreOreLib::Memory<IChromosome*>& T, const void* attribs )
		{
			const IChromosome* pParent1	= X[0]->GetChromosomeByType( TypeID );
			const IChromosome* pParent2	= X[1]->GetChromosomeByType( TypeID );
			IChromosome* pChild1		= T[0]->GetChromosomeByType( TypeID );
			IChromosome* pChild2		= T[1]->GetChromosomeByType( TypeID );

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
					pChild1->GeneAs<Type>( j )	= pParent1->GeneAs<Type>( j );// pChild1をParent1で部分上書きする

					// |---- parent2 ----|++++ parent1 ++++|---- parent2 ----|+++...
					pChild2->GeneAs<Type>( j )	= pParent2->GeneAs<Type>( j );// pChild2をpParent2で部分上書きする
				}

			}// end of i loop

		}


		// bitstring crossover
		template< typename Type=T >
		std::enable_if_t< std::is_same_v<Type, BitArray>, void >
		Execute_( OreOreLib::Memory<const IChromosome*>& X, OreOreLib::Memory<IChromosome*>& T, const void* attribs )
		{
			const IChromosome* pParent1	= X[0]->GetChromosomeByType( TypeID );
			const IChromosome* pParent2	= X[1]->GetChromosomeByType( TypeID );
			IChromosome* pChild1		= T[0]->GetChromosomeByType( TypeID );
			IChromosome* pChild2		= T[1]->GetChromosomeByType( TypeID );

			for( int i=0; i<pParent1->Size(); ++i )
			{
				const auto& pBParent1	= pParent1->GeneAs<BitArray>(i);
				const auto& pBParent2	= pParent2->GeneAs<BitArray>(i);
				auto& pBChild1			= pChild1->GeneAs<BitArray>(i);
				auto& pBChild2			= pChild2->GeneAs<BitArray>(i);

				int32 bitLength			= static_cast<int32>( Min( Min( Min( pBParent1.BitLength(), pBParent2.BitLength() ), pBChild1.BitLength() ), pBChild2.BitLength() ) );
				int32 splicePoints[2]	= { int32( OreOreLib::genrand_real2() * bitLength ), int32( OreOreLib::genrand_real2() * bitLength ) };

				if( splicePoints[0] > splicePoints[1] )
				{
					int32 temp = splicePoints[0];
					splicePoints[0]	= splicePoints[1];
					splicePoints[1]	= temp;
				}

				// |++++ parent1 ++++|---- parent2 ----|++++ parent1 ++++|
				pBChild1.CopyFrom( &pBParent1 );
				pBChild1.CopyFrom( splicePoints[0], &pBParent2, splicePoints[0], splicePoints[1]-splicePoints[0] );

				// |---- parent2 ----|++++ parent1 ++++|---- parent2 ----|
				pBChild2.CopyFrom( &pBParent2 );
				pBChild2.CopyFrom( splicePoints[0], &pBParent1, splicePoints[0], splicePoints[1]-splicePoints[0] );
			}

		}



	};



}// end of namespace



#endif // !TWO_POINT_CROSSOVER_H
