#ifndef CHROMOSOME_FACTORY_H
#define	CHROMOSOME_FACTORY_H

#include	"Chromosome1DFactory.h"
#include	"Chromosome2D.h"



namespace ealib
{

	template < typename TList >
	class ChromosomeFactory : public Chromosome1DFactory<TList>
	{
	public:

		ChromosomeFactory()
			: Chromosome1DFactory<TList>()
		{
			
		}
		

		virtual ~ChromosomeFactory()
		{

		}


		virtual IChromosome* Create( const DesignParamArray& designParams ) const
		{
			assert( designParams );

			uint16 type = designParams[0].TypeID();

			for( int32 i=1; i<designParams.Length<int32>(); ++i )
				if( type != designParams[i].TypeID() )
					return new Chromosome2D( designParams );

			return this->m_CreateChromosomeFuncs[ type ]( designParams );
		}

	};



}// end of namespace


#endif // !CHROMOSOME_FACTORY_H