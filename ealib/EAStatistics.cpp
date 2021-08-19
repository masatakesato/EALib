#include	"EAStatistics.h"

#include	"Population.h"


namespace ealib
{


	EAStatistics::EAStatistics()
		: m_MaxIter( 0 )
		, m_CurrentIter( 0 )
		, m_currSeq( 0 )
		, m_Sequence()
	{

		m_NumLogs	= 0;
	}



	EAStatistics::~EAStatistics()
	{
		Release();
	}



	EAStatistics::EAStatistics( const EAStatistics &obj )
	{
		m_MaxIter		= obj.m_MaxIter;
		m_CurrentIter	= obj.m_CurrentIter;
		DeepCopyArrays( obj );
	}



	EAStatistics &EAStatistics::operator=( const EAStatistics& obj )
	{
		if( this != &obj )
		{
			m_MaxIter		= obj.m_MaxIter;
			m_CurrentIter	= obj.m_CurrentIter;
			DeepCopyArrays( obj );
		}

		return *this;
	}



	void EAStatistics::Init( int maxiter, int interval, int numlogs )
	{
		m_MaxIter		= maxiter;
		m_CurrentIter	= 0;
		InitConvergenceChecker( interval );
		InitLogBuffer( numlogs );
	}



	void EAStatistics::Release()
	{
		m_CurrentIter	= 0;
		ReleaseConvergenceChecker();
		ReleaseLogBuffer();
	}



	void EAStatistics::Reset( const Population& pop )
	{
		m_CurrentIter	= 0;

		if( m_Sequence.Length() > 0 )
		{
			m_currSeq	= 0;
			m_Sequence.Clear();
			m_Sequence[0].first = pop.GetIndividual( 0 )->GetFitness();
		}

		if( m_NumLogs > 0 )
		{
			// TODO: implement log info
		}

	}



	void EAStatistics::Update( const Population& pop )
	{
		++m_CurrentIter;

		// Set Convergence
		++m_currSeq;
		int currIdx = m_currSeq % m_Sequence.Length();
		m_Sequence[ currIdx ].first		= pop.GetIndividual( 0 )->GetFitness();
		m_Sequence[ currIdx ].second	= pop.GetIndividual( pop.PopulationSize()-1 )->GetFitness();
	}



	EAStatistics* EAStatistics::Clone() const
	{
		return new EAStatistics( *this );
	}



	void EAStatistics::InitConvergenceChecker( int interval )
	{
		ReleaseConvergenceChecker();

		m_Sequence.Init( interval );
	}



	void EAStatistics::ReleaseConvergenceChecker()
	{
		m_currSeq	= 0;
		m_Sequence.Release();
	}



	void EAStatistics::InitLogBuffer( int numlogs )
	{
		ReleaseLogBuffer();

		m_NumLogs	= numlogs;
	}



	void EAStatistics::ReleaseLogBuffer()
	{
		m_NumLogs	= 0;
	}




	void EAStatistics::DeepCopyArrays( const EAStatistics& src )
	{
		//============== Copy convergence check buffer =================//
		m_currSeq	= src.m_currSeq;

		if( src.m_Sequence.Length() > 0 )
		{
			m_Sequence.Init( src.m_Sequence.Length() );
			m_Sequence.CopyFrom( src.m_Sequence );
		}
		else
		{
			m_Sequence.Release();
		}

		//====================== Copy log buffer =======================//
		m_NumLogs	= src.m_NumLogs;
	}







}// end of namespace