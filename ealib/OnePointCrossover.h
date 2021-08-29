#ifndef ONE_POINT_CROSSOVER_H
#define	ONE_POINT_CROSSOVER_H

#include	"ICrossoverOperator.h"
#include	"IChromosome.h"



namespace ealib
{

	// Forward declaration.
	template< typename T, typename enabled=void > class OnePointCrossover;



	// Class implementation
	template< typename T >
	class /*CLASS_DECLSPEC*/ OnePointCrossover< T, std::enable_if_t< (std::is_arithmetic_v<T> || std::is_same_v<T, BitArray>) > > : public ICrossoverOperator
	{
	public:

		OnePointCrossover()
			: ICrossoverOperator( TYPE_ID<T>, { 0.0f, 2, 2 } )
		{
			//static_assert( std::is_arithmetic_v<T> || std::is_same_v<T, BitArray> );// C++17 or above required.
		}


		~OnePointCrossover()
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

			int size		= Min( Min( Min( pParent1->Size(), pParent2->Size() ), pChild1->Size() ), pChild2->Size() );
			int splicePoint	= int( OreOreLib::genrand_real2() * size );

			for( int i=0; i<=splicePoint; ++i )
			{
				*pChild1->GeneAs<Type>( i )	= *pParent1->GeneAs<Type>( i );
				*pChild2->GeneAs<Type>( i )	= *pParent2->GeneAs<Type>( i );

			}// end of i loop

			for( int i=splicePoint+1; i<size; ++i )
			{
				*pChild1->GeneAs<Type>( i )	= *pParent2->GeneAs<Type>( i );
				*pChild2->GeneAs<Type>( i )	= *pParent1->GeneAs<Type>( i );

			}// end of i loop

		}


		// bitstring crossover
		template< typename Type=T >
		std::enable_if_t< std::is_same_v<Type, BitArray>, void >
		Execute_( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs )
		{
			const IChromosome* pParent1	= parents[0];
			const IChromosome* pParent2	= parents[1];
			IChromosome* pChild1		= children[0];
			IChromosome* pChild2		= children[1];

			for( int i=0; i<pParent1->Size(); ++i )
			{
				const BitArray *pParentBitArray1	= pParent1->GeneAs<BitArray>( i );
				const BitArray *pParentBitArray2	= pParent2->GeneAs<BitArray>( i );
				BitArray *pChildBitArray1			= pChild1->GeneAs<BitArray>( i );
				BitArray *pChildBitArray2			= pChild2->GeneAs<BitArray>( i );

				int bitLength	= Min( Min( Min( pParentBitArray1->BitLength(), pParentBitArray2->BitLength() ), pChildBitArray1->BitLength() ), pChildBitArray2->BitLength() );
				int splicePoint	= int( OreOreLib::genrand_real2() * bitLength );


				// |++++ parent1 ++++|---- parent2 ----|
				pChildBitArray1->CopyFrom( 0, pParentBitArray1, 0, splicePoint );
				pChildBitArray1->CopyFrom( splicePoint, pParentBitArray2, splicePoint, pChildBitArray1->BitLength()-splicePoint );

				// |---- parent2 ----|++++ parent1 ++++|
				pChildBitArray2->CopyFrom( 0, pParentBitArray2, 0, splicePoint );
				pChildBitArray2->CopyFrom( splicePoint, pParentBitArray1, splicePoint, pChildBitArray2->BitLength()-splicePoint );

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

			int size		= Min( Min( Min( pParent1->Size(), pParent2->Size() ), pChild1->Size() ), pChild2->Size() );
			int splicePoint	= int( OreOreLib::genrand_real2() * size );

			for( int i=0; i<=splicePoint; ++i )
			{
				*pChild1->GeneAs<Type>( i )	= *pParent1->GeneAs<Type>( i );
				*pChild2->GeneAs<Type>( i )	= *pParent2->GeneAs<Type>( i );

			}// end of i loop

			for( int i=splicePoint+1; i<size; ++i )
			{
				*pChild1->GeneAs<Type>( i )	= *pParent2->GeneAs<Type>( i );
				*pChild2->GeneAs<Type>( i )	= *pParent1->GeneAs<Type>( i );

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
				const BitArray *pBParent1	= pParent1->GeneAs<BitArray>( i );
				const BitArray *pBParent2	= pParent2->GeneAs<BitArray>( i );
				BitArray *pBChild1			= pChild1->GeneAs<BitArray>( i );
				BitArray *pBChild2			= pChild2->GeneAs<BitArray>( i );

				int bitLength	= Min( Min( Min( pBParent1->BitLength(), pBParent2->BitLength() ), pBChild1->BitLength() ), pBChild2->BitLength() );
				int splicePoint	= int( OreOreLib::genrand_real2() * bitLength );


				// |++++ parent1 ++++|---- parent2 ----|
				pBChild1->CopyFrom( 0, pBParent1, 0, splicePoint );
				pBChild1->CopyFrom( splicePoint, pBParent2, splicePoint, pBChild1->BitLength()-splicePoint );

				// |---- parent2 ----|++++ parent1 ++++|
				pBChild2->CopyFrom( 0, pBParent2, 0, splicePoint );
				pBChild2->CopyFrom( splicePoint, pBParent1, splicePoint, pBChild2->BitLength()-splicePoint );

			}// end of i loop

		}




	};



}// end of namespace



#endif // !ONE_POINT_CROSSOVER_H
