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
			bool bIsMultiType = false;
			int currtype = designParams[0].TypeID();

			for( int i=1; i<designParams.Length(); ++i )
			{
				if( currtype != designParams[i].TypeID() )
				{
					bIsMultiType = true;
					break;
				}
			}

			return bIsMultiType
				? new Chromosome2D( designParams )
				: this->m_CreateChromosomeFuncs[ designParams[0].TypeID() ]( designParams );
		}

	};



}// end of namespace


#endif // !CHROMOSOME_FACTORY_H