#ifndef N_POINT_CROSSOVER_H
#define	N_POINT_CROSSOVER_H

#include	"ICrossoverOperator.h"
#include	"IChromosome.h"



namespace ealib
{
	// Forward declaration
	template< typename T, int N, typename enabled=void > class NPointCrossover;


	// Class implementation
	template< typename T, int N >
	class /*CLASS_DECLSPEC*/ NPointCrossover< T, N, std::enable_if_t< (std::is_arithmetic_v<T> || std::is_same_v<T, BitArray>) > > : public ICrossoverOperator
	{
	public:

		NPointCrossover()
			: ICrossoverOperator( TYPE_ID<T>, { 0.0f, 2, 2 } )
		{

		}


		~NPointCrossover()
		{

		}


		virtual void Execute( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs )
		{
			Execute_( numparents,  parents, numchildren, children, attribs );
		}


		virtual void Execute( OreOreLib::Memory<const IChromosome*>& X, OreOreLib::Memory<IChromosome*>& T, const void* attribs )
		{
			Execute_( X, T, attribs );
		}



	private:

		int	m_SplicePoints[ N ];


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
			for( int i=0; i<N; ++i )
				m_SplicePoints[i] = int( OreOreLib::genrand_real2() * size );

			std::sort( m_SplicePoints, m_SplicePoints + N );


			pChild1->CopyGeneFrom( pParent2 );// pChild1を、pParent2の遺伝子で初期化
			pChild2->CopyGeneFrom( pParent1 );// pChild2を、pParent1の遺伝子で初期化

			for( int i=0; i<N; i+=2 )
			{
				int start	= m_SplicePoints[i];
				int end		= i==N-1 ? size-1 : m_SplicePoints[i+1];

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

				int bitLength	= Min( Min( Min( pParent1.BitLength(), pParent2.BitLength() ), pChild1.BitLength() ), pChild2.BitLength() );

				// Generate Splice Points without duplication
				//for( int j=0; j<bitLength; ++j )
				//	m_SplicePoints[j] = j;

				//Shuffle( m_SplicePoints, m_BitLength );

				for( int j=0; j<N; ++j )
					m_SplicePoints[j] = int( OreOreLib::genrand_real2() * bitLength );

				std::sort( m_SplicePoints, m_SplicePoints + N );


				pChild1.CopyFrom( &pParent2 );// pChild1を、pParent2の遺伝子で初期化
				pChild2.CopyFrom( &pParent1 );// pChild2を、pParent1の遺伝子で初期化

				for( int j=0; j<N; j+=2 )
				{
					int start	= m_SplicePoints[j];
					int end		= j==N-1 ? bitLength-1 : m_SplicePoints[j+1];

					// child1 crossover: |++++ parent1 ++++|---- parent2 ----|++++ parent1 ++++|---...
					pChild1.CopyFrom( start, &pParent1, start, end-start );// pChild1をpParent1の遺伝子で部分上書きする
					// child2 crossover: |---- parent2 ----|++++ parent1 ++++|---- parent2 ----|+++...
					pChild2.CopyFrom( start, &pParent2, start, end-start );// pChild2をpParent2の遺伝子で部分上書きする

				}// end of j loop

			}// end of i loop

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
			for( int i=0; i<N; ++i )	m_SplicePoints[i] = int( OreOreLib::genrand_real2() * size );
			std::sort( m_SplicePoints, m_SplicePoints + N );


			pChild1->CopyGeneFrom( pParent2 );// pChild1を、pParent2の遺伝子で初期化
			pChild2->CopyGeneFrom( pParent1 );// pChild2を、pParent1の遺伝子で初期化

			for( int i=0; i<N; i+=2 )
			{
				int start	= m_SplicePoints[i];
				int end		= i==N-1 ? size-1 : m_SplicePoints[i+1];

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

				int bitLength	= Min( Min( Min( pBParent1.BitLength(), pBParent2.BitLength() ), pBChild1.BitLength() ), pBChild2.BitLength() );

				// Generate Splice Points without duplication
				//for( int j=0; j<bitLength; ++j )
				//	m_SplicePoints[j] = j;

				//Shuffle( m_SplicePoints, m_BitLength );

				for( int j=0; j<N; ++j )	m_SplicePoints[j] = int( OreOreLib::genrand_real2() * bitLength );
				std::sort( m_SplicePoints, m_SplicePoints + N );


				pBChild1.CopyFrom( &pBParent2 );// pBChild1を、pBParent2の遺伝子で初期化
				pBChild2.CopyFrom( &pBParent1 );// pBChild2を、pBParent1の遺伝子で初期化

				for( int j=0; j<N; j+=2 )
				{
					int start	= m_SplicePoints[j];
					int end		= j==N-1 ? bitLength-1 : m_SplicePoints[j+1];

					// child1 crossover: |++++ parent1 ++++|---- parent2 ----|++++ parent1 ++++|---...
					pBChild1.CopyFrom( start, &pBParent1, start, end-start );// pBChild1をpBParent1の遺伝子で部分上書きする
					// child2 crossover: |---- parent2 ----|++++ parent1 ++++|---- parent2 ----|+++...
					pBChild2.CopyFrom( start, &pBParent2, start, end-start );// pBChild2をpBParent2の遺伝子で部分上書きする

				}// end of j loop

			}// end of i loop

		}




	};



}// end of namespace



#endif // !N_POINT_CROSSOVER_H
