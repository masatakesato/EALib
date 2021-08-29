#ifndef DE_CURRENT_TO_PBEST_1_ARCHIVE_H
#define	DE_CURRENT_TO_PBEST_1_ARCHIVE_H

#include	"IDEMutationStrategy.h"

#include	<oreore/container/Array.h>
#include	<oreore/container/ArrayView.h>



namespace ealib
{

	class CLASS_DECLSPEC DE_Current_to_pBest_1_Archive : public IDEMutationStrategy
	{
	public:

		DE_Current_to_pBest_1_Archive();
		virtual ~DE_Current_to_pBest_1_Archive();

		void SetP( float p );
		void SetNumArchives( int numarchives );
		void BindArchives( int numarchives, const OreOreLib::Array<IChromosome*>& archives );
		void UnbindArchives();

		// Virtual Functions Override
		virtual void Execute( OreOreLib::Memory<const IChromosome*>& selections, int current );


	private:
		
		float			m_P;
		int				m_NumActiveArchives;
		OreOreLib::ArrayView<IChromosome*>	m_refArchives;

	};



}// end of namespace



#endif // !DE_CURRENT_TO_PBEST_1_ARCHIVE_H
