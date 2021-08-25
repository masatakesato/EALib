#ifndef EXPONENTIAL_CROSSOVER_H
#define	EXPONENTIAL_CROSSOVER_H

// https://stackoverflow.com/questions/20393102/all-versions-of-differential-evolution-algorithm

#include	<oreore/mathlib/MersenneTwister.h>

#include	"ICrossoverOperator.h"
#include	"DEAttribute.h"
#include	"IChromosome.h"




namespace ealib
{
	
	template< typename T, typename enable=void >
	class ExponentialCrossover;


	template< typename T >
	class /*CLASS_DECLSPEC*/ ExponentialCrossover< T, std::enable_if_t< (std::is_arithmetic_v<T> || std::is_same_v<T, BitArray>) > > : public ICrossoverOperator
	{
	public:

		ExponentialCrossover()
			: ICrossoverOperator( TYPE_ID<T>, { 0.0f, CrossoverAttribute::Dynamic, 1 } )
		{
		
		}


		~ExponentialCrossover()
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


		// arithmetic crossover
		template< typename Type=T >
		std::enable_if_t< std::is_arithmetic_v<Type>, void >
		Execute_( int numchroms, IChromosome** chromosomes, const void* attribs )
		{
			const DEAttribute *pAttrib	= (DEAttribute*)attribs;
			IChromosome* pTrial		= chromosomes[0];// trial
			IChromosome** pParents	= &chromosomes[1];// parents

			int numParams = pTrial->Size();
			int numParents = numchroms - 1;
			int j = int( OreOreLib::genrand_real4()/*genrand_real2()*/ * numParams );
			int L = 0;

			do
			{
				Type *t_j = pTrial->GeneAs<Type>( j );
				DesignParameter* pDParam = pTrial->GetDesignParameter( j );
				
				// Apply Mutation. pParents[0] + F * ( pParents[1] - pParents[2] ) + F * ( pParents[3] - pParents[4] )...
				Type accum = 0;
				for( int i=1; i<numParents; i+=2 )
					accum += ( *pParents[i]->GeneAs<Type>( j ) - *pParents[i+1]->GeneAs<Type>( j ) );

				*t_j = Clamp( Type( *pParents[0]->GeneAs<Type>( j ) + Type(pAttrib->F * (float)accum) ), pDParam->LowerBoundary<Type>(), pDParam->UpperBoundary<Type>() );
				// casting accum to pAttrib->F precision(float)

				j = ( j + 1 ) % numParams;
				L++;
			} while( OreOreLib::genrand_real1()<pAttrib->CR && L<numParams );

		}


		// bitarray crossover
		template< typename Type=T >
		std::enable_if_t< std::is_same_v<Type, BitArray>, void >
		Execute_( int numchroms, IChromosome** chromosomes, const void* attribs )
		{
			const DEAttribute *pAttrib	= (DEAttribute*)attribs;
			IChromosome* pTrial			= chromosomes[0];// trial
			IChromosome** pParents		= &chromosomes[1];// parents
	
			for( int i=0; i<pTrial->Size(); ++i )
			{
				int numParams	= pTrial->GeneAs<BitArray>(i)->BitLength();
				int numParents	= numchroms - 1;
				int j			= int( OreOreLib::genrand_real4()/*genrand_real2()*/ * numParams );
				int L			= 0;
	
				do
				{
					// Apply Mutation. pParents[0] + F * ( pParents[1] - pParents[2] ) + F * ( pParents[3] - pParents[4] )...
					uint32 accum = 0;
					for( int k=1; k<numParents; k+=2 )
						accum |= ( uint32(pParents[k]->GeneAs<BitArray>(i)->GetBit( j )) ^ uint32(pParents[k+1]->GeneAs<BitArray>(i)->GetBit( j )) );// altered '+=' by '|=', '-' by '^'

					uint32 t_j	= uint32(pParents[0]->GeneAs<BitArray>(i)->GetBit( j )) | uint32(pAttrib->F * (float)accum);// altered '+' by '|'

					pTrial->GeneAs<BitArray>(i)->SetBit( j, (int)t_j );

	
					j	= ( j+1 )%numParams;
					L++;
				} while( OreOreLib::genrand_real1()<pAttrib->CR && L<numParams );

			}// end of i loop
	
		}






		// arithmetic crossover
		template< typename Type=T >
		std::enable_if_t< std::is_arithmetic_v<Type>, void >
		Execute_( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs )
		{
			const DEAttribute *pAttrib	= (DEAttribute*)attribs;
			IChromosome* pTrial		= children[0];// trial

			int numParams = pTrial->Size();
			int j = int( OreOreLib::genrand_real4()/*genrand_real2()*/ * numParams );
			int L = 0;

			do
			{
				Type *t_j = pTrial->GeneAs<Type>( j );
				DesignParameter* pDParam = pTrial->GetDesignParameter( j );
				
				// Apply Mutation. parents[0] + F * ( parents[1] - parents[2] ) + F * ( parents[3] - parents[4] )...
				Type accum = 0;
				for( int i=1; i<numparents; i+=2 )
					accum += ( *parents[i]->GeneAs<Type>( j ) - *parents[i+1]->GeneAs<Type>( j ) );

				*t_j = Clamp( Type( *parents[0]->GeneAs<Type>( j ) + Type(pAttrib->F * (float)accum) ), pDParam->LowerBoundary<Type>(), pDParam->UpperBoundary<Type>() );
				// casting accum to pAttrib->F precision(float)

				j = ( j + 1 ) % numParams;
				L++;
			} while( OreOreLib::genrand_real1()<pAttrib->CR && L<numParams );

		}


		// bitarray crossover
		template< typename Type=T >
		std::enable_if_t< std::is_same_v<Type, BitArray>, void >
		Execute_( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs )
		{
			const DEAttribute *pAttrib	= (DEAttribute*)attribs;
			IChromosome* pTrial			= children[0];// trial
	
			for( int i=0; i<pTrial->Size(); ++i )
			{
				const auto* pParentBitArray1 = parents[0]->GeneAs<BitArray>(i);
				auto pTrialBitArray = pTrial->GeneAs<BitArray>(i);

				int numParams	= pTrialBitArray->BitLength();
				int j			= int( OreOreLib::genrand_real4()/*genrand_real2()*/ * numParams );
				int L			= 0;
	
				do
				{
					// Apply Mutation. parents[0] + F * ( parents[1] - parents[2] ) + F * ( parents[3] - parents[4] )...
					uint32 accum = 0;
					for( int k=1; k<numparents; k+=2 )
						accum |= ( uint32(parents[k]->GeneAs<BitArray>(i)->GetBit( j )) ^ uint32(parents[k+1]->GeneAs<BitArray>(i)->GetBit( j )) );// altered '+=' by '|=', '-' by '^'

					uint32 t_j	= uint32(pParentBitArray1->GetBit( j )) | uint32(pAttrib->F * (float)accum);// altered '+' by '|'

					pTrialBitArray->SetBit( j, (int)t_j );

	
					j	= ( j+1 )%numParams;
					L++;
				} while( OreOreLib::genrand_real1()<pAttrib->CR && L<numParams );

			}// end of i loop
	
		}



	};




}// end of namespace



#endif // !EXPONENTIAL_CROSSOVER_H
