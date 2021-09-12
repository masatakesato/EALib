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
			: ICrossoverOperator( TYPE_ID<T>, { 0.0f, CrossoverAttribute::Dynamic, 1 } )
		{
		
		}


		~BinomialCrossover()
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


		// arithmetic crossover
		template< typename Type=T >
		std::enable_if_t< std::is_arithmetic_v<Type>, void >
		Execute_( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs )
		{
			const DEAttribute *pAttrib	= (DEAttribute*)attribs;
			IChromosome* pTrial			= children[0];// trial

			int numParams = pTrial->Size();
			int jrand = int( OreOreLib::genrand_real2() * numParams );

			// Select Crossover point from dimention
			for( int j = 0; j<numParams; ++j )
			{
				auto& t_j = pTrial->GeneAs<Type>( j );

				// Crossover
				if( OreOreLib::genrand_real1() < pAttrib->CR || j == jrand )
				{
					const DesignParameter& pDParam = pTrial->GetDesignParameter( j );

					// rand/n Mutation. parents[0] + F * ( parents[1] - parents[2] ) + F * ( parents[3] - parents[4] )...	
					Type accum = 0;
					for( int i=1; i<numparents; i+=2 )
						accum += ( parents[i]->GeneAs<Type>( j ) - parents[i+1]->GeneAs<Type>( j ) );

					t_j = Clamp( Type( (float)parents[0]->GeneAs<Type>( j ) + Type(pAttrib->F * (float)accum) ), pDParam.LowerBoundary<Type>(), pDParam.UpperBoundary<Type>() );
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
		Execute_( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs )
		{
			const DEAttribute *pAttrib	= (DEAttribute*)attribs;
			IChromosome* pTrial			= children[0];// trial

			for( int i=0; i<pTrial->Size(); ++i )
			{
				auto& pTrialBitArray = pTrial->GeneAs<BitArray>(i);
				const auto& pParentBitArray1 = parents[0]->GeneAs<BitArray>(i);

				int numParams	= pTrialBitArray.BitLength();
				int jrand		= int( OreOreLib::genrand_real2() * numParams );

				// Select Crossover point from dimention
				for( int j=0; j<numParams; ++j )
				{
					uint32 t_j = pTrialBitArray.GetBit( j );

					// Crossover
					if( OreOreLib::genrand_real1() < pAttrib->CR || j==jrand )
					{
						// Apply Mutation. parents[0] + F * ( parents[1] - parents[2] ) + F * ( parents[3] - parents[4] )...
						uint32 accum = 0;
						for( int k=1; k<numparents; k+=2 )
							accum |= ( uint32(parents[k]->GeneAs<BitArray>(i).GetBit( j )) ^ uint32(parents[k+1]->GeneAs<BitArray>(i).GetBit( j )) );// altered '+=' by 'OR', '-' by 'XOR'  //( pParents[i]->Gene( j ) - pParents[i+1]->Gene( j ) );

						t_j	= uint32(pParentBitArray1.GetBit( j )) | uint32(pAttrib->F * (float)accum);// altered '+' by 'OR' 

						pTrialBitArray.SetBit( j, (int)t_j );
					}
					else
					{
						// *t_j = x_i_j;// pChildren[0] is assumed to be initialized with x_i
					}

				}// end of design parameter loop

			}// end of i loop

		}



		// arithmetic crossover
		template< typename Type=T >
		std::enable_if_t< std::is_arithmetic_v<Type>, void >
		Execute_( OreOreLib::Memory<const IChromosome*>& X, OreOreLib::Memory<IChromosome*>& T, const void* attribs )
		{
			const DEAttribute *pAttrib	= (DEAttribute*)attribs;
			const IChromosome* pX0		= X[0]->GetChromosomeByType( TypeID );
			IChromosome* pTrial			= T[0]->GetChromosomeByType( TypeID );// trial

			int numParams = pTrial->Size();
			int jrand = int( OreOreLib::genrand_real2() * numParams );

			// Select Crossover point from dimention
			for( int j = 0; j<numParams; ++j )
			{
				auto& t_j = pTrial->GeneAs<Type>( j );

				// Crossover
				if( OreOreLib::genrand_real1() < pAttrib->CR || j == jrand )
				{
					const DesignParameter& pDParam = pTrial->GetDesignParameter( j );

					// rand/n Mutation. X[0] + F * ( X[1] - X[2] ) + F * ( X[3] - X[4] )...	
					Type accum = 0;
					for( int i=1; i<X.Length(); i+=2 )
						accum += ( X[i]->GetChromosomeByType(TypeID)->GeneAs<Type>( j ) - X[i+1]->GetChromosomeByType(TypeID)->GeneAs<Type>( j ) );

					t_j = Clamp( Type( (float)pX0->GeneAs<Type>( j ) + Type(pAttrib->F * (float)accum) ), pDParam.LowerBoundary<Type>(), pDParam.UpperBoundary<Type>() );
					// casting accum to pAttrib->F precision(float)
				}
				else
				{
					// t_j = x_i_j;// pChildren[0] is assumed to be initialized with x_i
				}

			}// end of design parameter loop
		}


		// bitarray crossover
		template< typename Type=T >
		std::enable_if_t< std::is_same_v<Type, BitArray>, void >
		Execute_( OreOreLib::Memory<const IChromosome*>& X, OreOreLib::Memory<IChromosome*>& T, const void* attribs )
		{
			const DEAttribute *pAttrib	= (DEAttribute*)attribs;
			const IChromosome* pX0		= X[0]->GetChromosomeByType( TypeID );
			IChromosome* pTrial			= T[0]->GetChromosomeByType( TypeID );// trial

			for( int i=0; i<pTrial->Size(); ++i )
			{
				auto& pBTrial = pTrial->GeneAs<BitArray>(i);
				const auto& pBParent1 = pX0->GeneAs<BitArray>(i);

				int numParams	= pBTrial.BitLength();
				int jrand		= int( OreOreLib::genrand_real2() * numParams );

				// Select Crossover point from dimention
				for( int j=0; j<numParams; ++j )
				{
					uint32 t_j = pBTrial.GetBit( j );

					// Crossover
					if( OreOreLib::genrand_real1() < pAttrib->CR || j==jrand )
					{
						// Apply Mutation. X[0] + F * ( X[1] - X[2] ) + F * ( X[3] - X[4] )...
						uint32 accum = 0;
						for( int k=1; k<X.Length(); k+=2 )
							accum |= (	uint32(X[k]->GetChromosomeByType(TypeID)->GeneAs<BitArray>(i).GetBit( j )) ^
										uint32(X[k+1]->GetChromosomeByType(TypeID)->GeneAs<BitArray>(i).GetBit( j ))
									);// altered '+=' by 'OR', '-' by 'XOR'  //( pParents[i]->Gene( j ) - pParents[i+1]->Gene( j ) );

						t_j	= uint32(pBParent1.GetBit( j )) | uint32(pAttrib->F * (float)accum);// altered '+' by 'OR' 

						pBTrial.SetBit( j, (int)t_j );
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
