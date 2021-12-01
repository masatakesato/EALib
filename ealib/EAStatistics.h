#ifndef EA_STATISTICS_H
#define	EA_STATISTICS_H

#include	<oreore/common/DLLExport.h>
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

		void Init( uint32 maxiter, uint32 interval, uint32 numlogs );
		void Release();
		void Reset( const Population& pop );
		void Update( const Population& pop );

		EAStatistics* Clone() const;
		
		
		void InitConvergenceChecker( uint32 interval );
		void ReleaseConvergenceChecker();
		void InitLogBuffer( uint32 numlogs );
		void ReleaseLogBuffer();

		uint32 MaxIteration() const		{ return m_MaxIter; }
		void SetMaxIteration( uint32 maxiter ){ m_MaxIter = maxiter; }

		uint32 CurrentIteration() const	{ return m_CurrentIter; }

		uint32 CurrentSequence() const	{ return m_currSeq; }
		uint32 Interval() const { return m_Sequence.Length<uint32>(); }

		float BestFitness( uint32 iter ) const { return m_Sequence[iter].first; }
		float WorstFitness( uint32 iter ) const { return m_Sequence[iter].second; }



	private:
		
		using Fitness = OreOreLib::Pair<float, float>;// best, worst

		uint32	m_MaxIter;// max iteration
		uint32	m_CurrentIter;// current iteration

		// Sequential generation info to keep tracking while iteration
		uint32	m_currSeq;
		OreOreLib::Array<Fitness>	m_Sequence;// best/worst fittness of latest m_numSeq iterations

		// Logs
		uint32	m_NumLogs;	// number of generations to keep log


		void DeepCopyArrays( const EAStatistics& src );

	};


}// end of namespace



#endif // !EA_STATISTICS_H
