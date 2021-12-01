#include	"Evaluator.h"


namespace ealib
{

	const float	Evaluator::c_ModeCoeff[2] = { -1.0f, 1.0f };



	Evaluator::Evaluator()
	{

	}



	Evaluator::~Evaluator()
	{

	}


		
	void Evaluator::Release()
	{
		m_EvalFuncs.Release();
		m_Modes.Release();
	}



	int32 Evaluator::NumObjectives() const
	{
		return m_EvalFuncs.Length<int32>();
	}



	bool Evaluator::BindObjectiveFunction( IObjectiveFunction* pfunc, SOLVE_MODE mode )
	{
		// Check if pfunc already exists in m_EvalFuncs.
		if( std::find( m_EvalFuncs.begin(), m_EvalFuncs.end(), pfunc ) != std::end(m_EvalFuncs) )
			return false;// operator already exists in m_refEvaluators.

		m_EvalFuncs.AddToTail( pfunc );
		m_Modes.AddToTail( mode );

		return true;
	}

	
	
	bool Evaluator::UnbindObjectiveFunction( IObjectiveFunction* pfunc )
	{	
		int idx = -1;

		for( int32 i=0; i<m_EvalFuncs.Length<int32>(); ++i )
		{
			if( pfunc == m_EvalFuncs[i] )
			{
				idx = i;
				break;
			}
		}

		if( idx==-1 )	return false;

		m_EvalFuncs.Remove( idx );
		m_Modes.Remove( idx );

		return true;
	}



	bool Evaluator::UnbindObjectiveFunction( int i )
	{
		if( i<0 || i>=m_EvalFuncs.Length<int32>() )
			return false;

		m_EvalFuncs.Remove( i );
		m_Modes.Remove( i );

		return true;
	}



	bool Evaluator::SetSolveMode( int i, SOLVE_MODE mode )
	{
		if( i<0 || i>=m_EvalFuncs.Length<int32>() )
			return false;

		m_Modes[ i ] = mode;
		return true;
	}



}// end of namespace



