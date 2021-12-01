#ifndef CHROMOSOME_1D_FACTORY_H
#define	CHROMOSOME_1D_FACTORY_H

#include	"Chromosome1D.h"



namespace ealib
{

	//======================== Create Chromosome1D functions ========================//

	template < typename T > //std::enable_if_t< !std::is_same_v<T, BitArray> >* = nullptr >
	static IChromosome* CreateChromosome1D( const DesignParamArray& designParams )
	{
		return new Chromosome1D<T>( designParams );
	}



	template <> //typename T, std::enable_if_t< std::is_same_v<T, BitArray> >* = nullptr >
	static IChromosome* CreateChromosome1D<BitArray>( const DesignParamArray& designParams )
	{
		Chromosome1D<BitArray>* pChromosome = new Chromosome1D<BitArray>( designParams );

		// BitArray specific operation. Need to allocate dynamic array
		for( int32 i=0; i<pChromosome->Size(); ++i )
			pChromosome->GeneAs(i)->Init( designParams[i].UpperBoundary<int>() );

		return pChromosome;
	}




	template < typename TList >
	class Chromosome1DFactory
	{
	public:

		Chromosome1DFactory()
		{
			Init<TList>::Execute( m_CreateChromosomeFuncs );
		}


		virtual ~Chromosome1DFactory()
		{

		}

		
		virtual IChromosome* Create( const DesignParamArray& designParams ) const
		{
			assert( designParams );

			uint16 type = designParams[0].TypeID();

			for( int32 i=1; i<designParams.Length<int32>(); ++i )
				if( type != designParams[i].TypeID() )
					return nullptr;

			return m_CreateChromosomeFuncs[ type ]( designParams );
		}



	protected:

		IChromosome *(* m_CreateChromosomeFuncs[ OreOreLib::TypeList::Length<TList>::value ] )( const DesignParamArray& );
		//{
		//	[]()->Base* { return new Derived<int>(); },
		//	...
		//};



		template < typename TList >
		struct Init
		{
			static void Execute( IChromosome* ( *funcs[] )( const DesignParamArray& ), int i=0 )
			{
				funcs[i] = &CreateChromosome1D<typename TList::head>;//[]( const DesignParamArray& designParams )->IChromosome* { return new Chromosome1D< typename TList::head >( designParams ); };
				Init< typename TList::tail >::Execute( funcs, i+1 );
			}
		};


		// typelist end
		template <>
		struct Init< OreOreLib::TypeList::NullType  >
		{
			static void Execute( IChromosome* ( *funcs[] )( const DesignParamArray& ), int i ){}// Do nothing
		};


	};



}// end of namespace


#endif // !CHROMOSOME_1D_FACTORY_H