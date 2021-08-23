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
			for( int i=0; i<designParams.Length()-1; ++i )
				if( designParams[i].TypeID() != designParams[i+1].TypeID() )
					return new Chromosome2D( designParams );

			return this->m_CreateChromosomeFuncs[ designParams[0].TypeID() ]( designParams );
		}

	};



}// end of namespace


#endif // !CHROMOSOME_FACTORY_H