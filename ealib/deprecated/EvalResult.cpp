//#include	"EvalResult.h"
//
//
//namespace ealib
//{
//	EvalResult::EvalResult()
//		: m_ParetoRank( 0 )
//		, m_CrowdingDistance( 0.0f )
//	{
//	}
//
//
//	EvalResult::EvalResult( int numelms )
//		: m_Score( numelms )
//		, m_Fitness( numelms )
//		, m_ParetoRank( 0 )
//		, m_CrowdingDistance( 0.0f )
//	{
//	}
//
//
//	EvalResult::EvalResult( const EvalResult& obj )
//		: m_Score( obj.m_Score )
//		, m_Fitness( obj.m_Fitness )
//		, m_ParetoRank( obj.m_ParetoRank )
//		, m_CrowdingDistance( obj.m_CrowdingDistance )
//	{
//	}
//
//
//	EvalResult& EvalResult::operator=( const EvalResult& obj )
//	{
//		m_Score				= obj.m_Score;
//		m_Fitness			= obj.m_Fitness;
//		m_ParetoRank		= obj.m_ParetoRank;
//		m_CrowdingDistance	= obj.m_CrowdingDistance;
//
//		return *this;
//	}
//
//
//	void EvalResult::Init( int numelms )
//	{
//		m_Score.Resize( numelms );
//		m_Fitness.Resize( numelms );
//		m_ParetoRank		= 0;
//		m_CrowdingDistance	= 0.0f;
//	}
//
//	
//	void EvalResult::Release()
//	{
//		m_Score.Release();
//		m_Fitness.Release();
//		m_ParetoRank		= 0;
//		m_CrowdingDistance	= 0.0f;
//	}
//
//
//	void EvalResult::Clear()
//	{
//		m_Score.Clear();
//		m_Fitness.Clear();
//		m_ParetoRank		= 0;
//		m_CrowdingDistance	= 0.0f;
//	}
//
//
//	void EvalResult::Add()
//	{
//		m_Score.AddToTail();
//		m_Fitness.AddToTail();
//	}
//
//
//	void EvalResult::Remove( int elm )
//	{
//		assert( elm>=0 && elm<m_Score.Length() );
//		m_Score.Remove( elm );
//		m_Fitness.Remove( elm );
//	}
//
//
//}// end of namespace
