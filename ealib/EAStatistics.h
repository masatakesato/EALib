#ifndef EA_STATISTICS_H
#define	EA_STATISTICS_H

#include	"DLLExport.h"

#include	<oreore/container/Array.h>
#include	<oreore/container/Pair.h>



namespace ealib
{
	class Population;


	class CLASS_DECLSPEC EAStatistics
	{
	public:

		EAStatistics();
		virtual ~EAStatistics();
		EAStatistics( const EAStatistics &obj );

		EAStatistics &operator=( const EAStatistics& obj );

		void Init( int maxiter, int interval, int numlogs );
		void Release();
		void Reset( const Population& pop );
		void Update( const Population& pop );

		EAStatistics* Clone() const;
		
		
		void InitConvergenceChecker( int interval );
		void ReleaseConvergenceChecker();
		void InitLogBuffer( int numlogs );
		void ReleaseLogBuffer();

		int MaxIteration() const		{ return m_MaxIter; }
		void SetMaxIteration( int maxiter ){ m_MaxIter = maxiter; }

		int CurrentIteration() const	{ return m_CurrentIter; }

		int CurrentSequence() const	{ return m_currSeq; }
		int Interval() const { return m_Sequence.Length(); }

		float BestFitness( int iter ) const { return m_Sequence[iter].first; }
		float WorstFitness( int iter ) const { return m_Sequence[iter].second; }



	private:
		
		using Fitness = Pair<float, float>;// best, worst

		int		m_MaxIter;// max iteration
		int		m_CurrentIter;// current iteration

		// Sequential generation info to keep tracking while iteration
		int		m_currSeq;
		OreOreLib::Array<Fitness>	m_Sequence;// best/worst fittness of latest m_numSeq iterations

		// Logs
		int		m_NumLogs;	// number of generations to keep log


		void DeepCopyArrays( const EAStatistics& src );

	};


}// end of namespace



#endif // !EA_STATISTICS_H
