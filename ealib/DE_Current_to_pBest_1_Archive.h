#ifndef DE_CURRENT_TO_PBEST_1_ARCHIVE_H
#define	DE_CURRENT_TO_PBEST_1_ARCHIVE_H


#include	"IDEMutationStrategy.h"



namespace ealib
{

	class CLASS_DECLSPEC DE_Current_to_pBest_1_Archive : public IDEMutationStrategy
	{
	public:

		DE_Current_to_pBest_1_Archive();
		virtual ~DE_Current_to_pBest_1_Archive();

		// Virtual Functions Override
		virtual void Execute( int num, IChromosome** selections, int current );

		void SetP( float p );
		void SetNumArchives( int numarchives );
		void BindArchives( int numarchives, IChromosome** archives );
		void UnbindArchives();


	private:
		
		float			m_P;
		int				m_NumArchives;
		IChromosome**	m_refArchives;

		//int				m_NumTotal;
	};



}// end of namespace



#endif // !DE_CURRENT_TO_PBEST_1_ARCHIVE_H
