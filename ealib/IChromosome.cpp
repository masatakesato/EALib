#include	"IChromosome.h"

#include	<float.h>

#include	"Initializer.h"


namespace ealib
{

	IChromosome::IChromosome()
		: m_CurrentEval( 0 )
		, m_pResult( nullptr )
		, m_ID( -1 )
	{

	}



	IChromosome::IChromosome( const IChromosome& obj )
	{
		if( !obj.m_DesignParameters.Empty() )
			m_DesignParameters = obj.m_DesignParameters;

		m_CurrentEval	= obj.m_CurrentEval;
		m_pResult		= nullptr;

		m_ID			= -1;
	}



	IChromosome::~IChromosome()
	{
		Release();
	}



	bool IChromosome::Init( const DesignParamArray& parameters )
	{
		if( !parameters )
			return false;

		m_DesignParameters	= parameters;
		for( int32 i=0; i<m_DesignParameters.Length<int32>(); ++i )
			m_DesignParameters[i].SetSequentialID( i );// Assign SequentialID

		m_CurrentEval	= 0;
		m_pResult		= nullptr;
		m_ID			= -1;

		return true;
	}



	void IChromosome::Release()
	{
		m_DesignParameters.Release();

		m_CurrentEval	= 0;
		m_pResult		= nullptr;
		m_ID			= -1;
	}



	void IChromosome::BindEvalResultView( EvalResultView* result )
	{
		m_pResult = result;
	}



	void IChromosome::UnbindEvalResultView()
	{
		m_pResult = nullptr;
	}



	void IChromosome::Initialize( Initializer* pInit )
	{
		pInit->Execute( this );

		if( m_pResult )	m_pResult->Clear();
	}



	//################## TODO: Test implementation for Multi Objective Optimization ###################//
	//bool IChromosome::Dominates( const IChromosome* chromosome ) const
	//{
	//	const OreOreLib::Array<float>& fitness_self = m_Results.Fitness();
	//	const OreOreLib::Array<float>& fitness_comp = chromosome->m_Results.Fitness();
	//	
	//	for( const auto& fitness: fitness_self )
	//	{
	//		int i = static_cast<int>( &fitness - &fitness_self[0] );
	//		if( fitness > fitness_comp[i] )// Compare Fitness
	//			return false;
	//	}

	//	return true;
	//}



	bool IChromosome::Dominates( const IChromosome* competitor ) const
	{		
		auto compresult = competitor->m_pResult;
		int numObjectives = Min( m_pResult->NumObjectives, compresult->NumObjectives );

		for( int i=0; i<numObjectives; ++i )
		{
			if( m_pResult->Fitness(i) > compresult->Fitness(i) )// Compare Fitness
				return false;
		}

		return true;
	}



}// end of namespace