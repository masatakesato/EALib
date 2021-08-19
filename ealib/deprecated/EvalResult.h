#ifndef	EVAL_RESULT_H
#define	EVAL_RESULT_H

//https://stackoverflow.com/questions/6811990/c-typename-as-variable
//
//#include	<memory>
//
//#include	<oreore/container/Array.h>
//#include	<oreore/common/Utility.h>
//
//#include	"DLLExport.h"
//#include	"Typedefs.h"
//
//
//
//namespace ealib
//{
//	enum SOLVE_MODE
//	{
//		UNKNOWN = -1,
//		MINIMIZE,
//		MAXIMIZE,
//	};
//
//	
//	class CLASS_DECLSPEC EvalResult
//	{
////		using fArray = OreOreLib::Array<float>;
//
//	public:
//		
//		EvalResult();
//		EvalResult( int numelms );
//		EvalResult( const EvalResult& obj );
//		virtual ~EvalResult() {}
//
//		EvalResult& operator=( const EvalResult& obj );
//
//		void Init( int numelms );
//		void Release();
//		void Clear();
//		void Add();
//		void Remove( int elm );
//
//		int Length() const	{ return m_Score.Length(); }
//
//		float& Score( int idx ) const			{ return const_cast<float&>(m_Score[idx]); }
//		const fArray& Score() const				{ return m_Score; }
//
//		float& Fitness( int idx ) const			{ return const_cast<float&>(m_Fitness[idx]); }
//		const fArray& Fitness() const			{ return m_Fitness; }
//
//		int ParetoRank( int idx ) const			{ return m_ParetoRank; }
//		float CrowdingDistance( int idx ) const { return m_CrowdingDistance; }
//
//		void Set( int idx, const float& score, SOLVE_MODE mode )
//		{
//			m_Score[idx]	= score;
//			m_Fitness[idx]	= score * c_ModeCoeff[mode];
//		}
//
//
//		inline void CopyFrom( const EvalResult* pSrc )
//		{
//			m_Score.CopyFrom( pSrc->m_Score );
//			m_Fitness.CopyFrom( pSrc->m_Fitness );
//			m_ParetoRank		= pSrc->m_ParetoRank;
//			m_CrowdingDistance	= pSrc->m_CrowdingDistance;
//		}
//
//
//		//void CopyTo( EvalResult* pDst ) const
//		//{
//		//	m_Score.CopyTo( pDst->m_Score );
//		//	m_Fitness.CopyTo( pDst->m_Fitness );
//		//	pDst->m_ParetoRank	= m_ParetoRank;
//		//	pDst->m_CrowdingDistance = m_CrowdingDistance;
//		//}
//
//
//
//	protected:
//		
//		fArray			m_Score;
//		fArray			m_Fitness;
//		int				m_ParetoRank;
//		float			m_CrowdingDistance;
//		
//		const float		c_ModeCoeff[2] ={ -1.0f, 1.0f };
//	};
//
//
//
//}// end of namespace


#endif	// EVAL_RESULT_H //