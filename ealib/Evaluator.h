#ifndef EVALUATOR_H
#define	EVALUATOR_H

#include	<oreore/common/DLLExport.h>
#include	<oreore/container/Array.h>

#include	"IObjectiveFunction.h"



namespace ealib

{
	enum SOLVE_MODE
	{
		UNKNOWN = -1,
		MINIMIZE,
		MAXIMIZE,
	};


	class CLASS_DECLSPEC Evaluator
	{
	public:

		Evaluator();
		virtual ~Evaluator();

		void Release();

		int NumObjectives() const;
		//void AddObjectiveFunction( IObjectiveFunction* pfunc, SOLVE_MODE mode );
		bool BindObjectiveFunction( IObjectiveFunction* pfunc, SOLVE_MODE mode );
		bool UnbindObjectiveFunction( IObjectiveFunction* pfunc );
		bool UnbindObjectiveFunction( int i );
		
		bool SetSolveMode( int i, SOLVE_MODE mode );



		void Evaluate( IChromosome* chromosome, const void* attribs=0 )
		{
			for( int i=0; i<m_EvalFuncs.Length(); ++i )
			{
				if( m_Modes[i] == SOLVE_MODE::UNKNOWN )
					continue;

				auto* pEval = m_EvalFuncs[i];
				auto score = pEval->Execute( chromosome, attribs );

				chromosome->SetScore( score, i );
				chromosome->SetFitness( score * c_ModeCoeff[ m_Modes[i] ], i );
			}
		}



	private:

		static const float	c_ModeCoeff[2];

		OreOreLib::Array<IObjectiveFunction*>	m_EvalFuncs;
		OreOreLib::Array<SOLVE_MODE>			m_Modes;

	};



}// end of namespace




#endif // !EVALUATOR_H
