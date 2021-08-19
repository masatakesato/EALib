#ifndef BINOMIAL_CROSSOVER_H
#define	BINOMIAL_CROSSOVER_H

// https://stackoverflow.com/questions/20393102/all-versions-of-differential-evolution-algorithm

#include	<oreore/mathlib/MersenneTwister.h>

#include	"ICrossoverOperator.h"
#include	"DEAttribute.h"
#include	"IChromosome.h"



namespace ealib
{

	template< typename T, typename enable=void >
	class BinomialCrossover;


	// Binomial crossover operator (for Differential Evolution)
	template< typename T >
	class /*CLASS_DECLSPEC*/ BinomialCrossover< T, std::enable_if_t< (std::is_arithmetic_v<T> || std::is_same_v<T, BitArray>) > > : public ICrossoverOperator
	{
	public:

		BinomialCrossover()
			: ICrossoverOperator( TYPE_ID<T> )
		{
		
		}


		~BinomialCrossover()
		{
		
		}


		virtual void Execute( int numchroms, IChromosome** chromosomes, const void* attribs )
		{
			Execute_( numchroms, chromosomes, attribs );
		}



	private:


		// arithmetic crossover
		template< typename Type=T >
		std::enable_if_t< std::is_arithmetic_v<Type>, void >
		Execute_( int numchroms, IChromosome** chromosomes, const void* attribs )
		{
			const DEAttribute *pAttrib	= (DEAttribute*)attribs;
			IChromosome* pTrial			= chromosomes[0];// trial
			IChromosome** pParents		= &chromosomes[1];// parents

			int numParams = pTrial->Size();
			int numParents = numchroms - 1;
			int jrand = int( OreOreLib::genrand_real2() * numParams );

			// Select Crossover point from dimention
			for( int j = 0; j<numParams; ++j )
			{
				Type* t_j = pTrial->GeneAs<Type>( j );

				// Crossover
				if( OreOreLib::genrand_real1() < pAttrib->CR || j == jrand )
				{
					DesignParameter* pDParam = pTrial->GetDesignParameter( j );

					// rand/n Mutation. pParents[0] + F * ( pParents[1] - pParents[2] ) + F * ( pParents[3] - pParents[4] )...	
					Type accum = 0;
					for( int i=1; i<numParents; i+=2 )
						accum += ( *pParents[i]->GeneAs<Type>( j ) - *pParents[i+1]->GeneAs<Type>( j ) );

					*t_j = Clamp( Type( (float)*pParents[0]->GeneAs<Type>( j ) + Type(pAttrib->F * (float)accum) ), pDParam->LowerBoundary<Type>(), pDParam->UpperBoundary<Type>() );
					// casting accum to pAttrib->F precision(float)
				}
				else
				{
					// *t_j = x_i_j;// pChildren[0] is assumed to be initialized with x_i
				}

			}// end of design parameter loop
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

				int numParams	= pTrial->GeneAs<BitArray>(i)->BitLength();//Size();
				int numParents	= numchroms - 1;
				int jrand		= int( OreOreLib::genrand_real2() * numParams );

				// Select Crossover point from dimention
				for( int j=0; j<numParams; ++j )
				{
					uint32 t_j = pTrial->GeneAs<BitArray>(i)->GetBit( j );

					// Crossover
					if( OreOreLib::genrand_real1() < pAttrib->CR || j==jrand )
					{
						// Apply Mutation. pParents[0] + F * ( pParents[1] - pParents[2] ) + F * ( pParents[3] - pParents[4] )...
						uint32 accum = 0;
						for( int k=1; k<numParents; k+=2 )
							accum |= ( uint32(pParents[k]->GeneAs<BitArray>(i)->GetBit( j )) ^ uint32(pParents[k+1]->GeneAs<BitArray>(i)->GetBit( j )) );// altered '+=' by 'OR', '-' by 'XOR'  //( pParents[i]->Gene( j ) - pParents[i+1]->Gene( j ) );

						t_j	= uint32(pParents[0]->GeneAs<BitArray>(i)->GetBit( j )) | uint32(pAttrib->F * (float)accum);// altered '+' by 'OR' 

						pTrial->GeneAs<BitArray>(i)->SetBit( j, (int)t_j );
					}
					else
					{
						// *t_j = x_i_j;// pChildren[0] is assumed to be initialized with x_i
					}

				}// end of design parameter loop

			}// end of i loop

		}






	};



}// end of namespace



#endif // !BINOMIAL_CROSSOVER_H
