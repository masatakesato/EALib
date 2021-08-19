#ifndef CHROMOSOME_FACTORY_H
#define	CHROMOSOME_FACTORY_H

#include	"Chromosome1D.h"



namespace ealib
{

	template < typename TList >
	class ChromosomeFactory
	{
	public:

		ChromosomeFactory()
		{
			Init<TList>::Execute( m_CreateChromosomeFuncs );
		}

		
		IChromosome* Create( const DesignParamArray& designParams ) const
		{
			IChromosome* chromosome = m_CreateChromosomeFuncs[ designParams[0].TypeID() ]( designParams );
	
			return chromosome;
		}



	private:


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


#endif // !ALLELE_FACTORY_H