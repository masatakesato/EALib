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


		virtual void Execute( int numchroms, IChromosome** chromosomes, const void* attribs )
		{
			Execute_( numchroms, chromosomes, attribs );
		}



		virtual void Execute( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs )
		{
			Execute_( numparents, parents, numchildren, children, attribs );
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
		Execute_( int numchroms, IChromosome** chromosomes, const void* attribs )
		{
			for( int i=0; i<chromosomes[0]->Size(); ++i )
			{
				const BitArray *pParent1	= chromosomes[0]->GeneAs<BitArray>( i );
				const BitArray *pParent2	= chromosomes[1]->GeneAs<BitArray>( i );
				BitArray *pChild1			= chromosomes[2]->GeneAs<BitArray>( i );
				BitArray *pChild2			= chromosomes[3]->GeneAs<BitArray>( i );

				int bitLength	= Min( Min( Min( pParent1->BitLength(), pParent2->BitLength() ), pChild1->BitLength() ), pChild2->BitLength() );
				int splicePoint	= int( OreOreLib::genrand_real2() * bitLength );


				// |++++ parent1 ++++|---- parent2 ----|
				pChild1->CopyFrom( 0, pParent1, 0, splicePoint );
				pChild1->CopyFrom( splicePoint, pParent2, splicePoint, pChild1->BitLength()-splicePoint );

				// |---- parent2 ----|++++ parent1 ++++|
				pChild2->CopyFrom( 0, pParent2, 0, splicePoint );
				pChild2->CopyFrom( splicePoint, pParent1, splicePoint, pChild2->BitLength()-splicePoint );

			}// end of i loop

		}









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



	};



}// end of namespace



#endif // !ONE_POINT_CROSSOVER_H
