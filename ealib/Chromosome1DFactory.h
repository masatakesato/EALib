#ifndef CHROMOSOME_1D_FACTORY_H
#define	CHROMOSOME_1D_FACTORY_H

#include	"Chromosome1D.h"



namespace ealib
{

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
			//====== Check if designParams' elements are same typeIDs =====//
			int typecounts[ NUM_TYPES ] = { 0 };

			for( int i=0; i<designParams.Length(); ++i )
				typecounts[ designParams[i].TypeID() ]++;

			int numtypes = 0;
			for( int i=0; i<NUM_TYPES; ++i )
				numtypes += int( typecounts[i] > 0 );


			//=============== return only if numtypes==1 ================//
			if( numtypes==1 )
				return m_CreateChromosomeFuncs[ designParams[0].TypeID() ]( designParams );
	
			return nullptr;
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
				funcs[i] = []( const DesignParamArray& designParams )->IChromosome* { return new Chromosome1D< typename TList::head >( designParams ); };
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