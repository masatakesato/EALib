#ifndef CHROMOSOME_VIEWER_H
#define	CHROMOSOME_VIEWER_H

#include	<oreore/common/TString.h>

#include	"Chromosome2D.h"
#include	"Population.h"



namespace ealib
{

	//##################################################################################################//
	//																									//
	//								ChromosomeViewer class implementation								//
	//																									//
	//##################################################################################################//

	template < typename TList >
	class ChromosomeViewer
	{
	public:

		ChromosomeViewer()
		{
			Init<TList>::Execute( m_ViewFuncs );
		}

		
		void View( const IChromosome* chromosome, int type, bool viewgene ) const
		{
			assert( chromosome != nullptr );

			tcout << typeid(*chromosome).name() << tendl;


			if( typeid( *chromosome )==typeid( Chromosome2D ) )
			{
				auto chrom2d = (Chromosome2D*)chromosome;

				for( int i=0; i<chrom2d->NumChromosomeTypes(); ++i )
				{
					auto chrom = chrom2d->GetChromosome(i);
					m_ViewFuncs[ chrom->TypeInfo() ]( chrom, viewgene );
				}
			}
			else if( chromosome->TypeInfo() >= 0 )
			{
				m_ViewFuncs[ chromosome->TypeInfo() ]( chromosome, viewgene );
			}
			else
			{
				tcout << _T( "  TYPE_UNKNOWN...\n" );
				return;
			}


			const EvalResultView* const refResult = chromosome->EvalResult();
			if( !refResult )
				return;

			tcout << _T( "  Results:\n" );
			for( int i=0; i<refResult->NumObjectives; ++i )
				tcout << _T("    Objective[") << i << _T("]:    Score = ") << refResult->Score( i ) << _T(",  Fitness = ") << refResult->Fitness( i ) << tendl;

			tcout << tendl;


		}


	private:


		void ( *m_ViewFuncs[ OreOreLib::TypeList::Length<TList>::value ] )( const IChromosome* chromosome, bool viewgene );


		// Display function for arithmetic type
		template <typename T>
		std::enable_if_t< std::is_arithmetic_v<T>, void >
		static DisplayFunc( const IChromosome* chromosome, bool viewgene )
		{
			if( viewgene )
			{
				tcout << _T( "  Params:\n" );
				for( int i=0; i<chromosome->Size(); ++i )
				{
					const DesignParameter* pDParam = chromosome->GetDesignParameter(i);

					tcout	<< _T( "    " ) << pDParam->Key()// key
							<< _T( "(" ) << typeid(T).name() << _T( ") " ) // type
							<< _T( "[" ) << pDParam->LowerBoundary<T>() << ", " << pDParam->UpperBoundary<T>() <<	"]: "// boundary
							<< *( chromosome->GeneAs<T>( i ) ) << tendl;
				}
			}
			
			tcout << tendl;
		}


		template< typename T >
		std::enable_if_t< std::is_same_v<T, BitArray>, void >
		static DisplayFunc( const IChromosome* chromosome, bool viewgene )
		{
			if( viewgene )
			{	
				tcout << _T( "  Params:\n" );
				for( int i=0; i<chromosome->Size(); ++i )
				{
					const DesignParameter* pDParam = chromosome->GetDesignParameter(i);
					const BitArray* bitarray = (BitArray*)chromosome->GetGene(i);

					tcout	<< _T( "    " ) << pDParam->Key()// key
					<< _T( "(" ) << typeid(T).name() << _T( ") " ) // type
					<< _T( "[" ) << pDParam->UpperBoundary<int>() <<	_T("]: ");
					for( int j=bitarray->BitLength()-1; j>=0; --j )
					{
						tcout << bitarray->GetBit(j);
						if( j % BitSize::uInt8 == 0 ) tcout << " ";
					}
					tcout << tendl;
				}
			}

			tcout << tendl;
		}





		template < typename TList >
		struct Init
		{
			static void Execute( void ( *funcs[] )( const IChromosome*, bool ), int i=0 )
			{
				funcs[i] = DisplayFunc< typename TList::head >;
				Init< typename TList::tail >::Execute( funcs, i+1 );
			}
		};


		template <>
		struct Init< OreOreLib::TypeList::NullType  >
		{
			static void Execute( void ( *funcs[] )( const IChromosome*, bool ), int i ){}// Do nothing
		};


	};




	//##################################################################################################//
	//																									//
	//										Helper functions											//
	//																									//
	//##################################################################################################//

	
	// Chromosome display function
	extern CLASS_DECLSPEC void DisplayChromosome( const IChromosome* chromosome, bool viewgene=false );


	// Population display function
	extern CLASS_DECLSPEC void DisplayPopulation( const Population* pop, bool viewgene=false );



}// end of namespace


#endif // !CHROMOSOME_VIEWER_H
