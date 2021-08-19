#include	"TerminateOnConvergence.h"

#include	<limits>

#include	<oreore/common/Utility.h>
#include	<oreore/mathlib/MathLib.h>

#include	"EAStatistics.h"



namespace ealib
{


	TerminateOnConvergence::TerminateOnConvergence()
	{

	}



	TerminateOnConvergence::~TerminateOnConvergence()
	{

	}



	bool TerminateOnConvergence::CheckTermination( EAStatistics* stat )
	{
		double convergence = 1.0;

		auto currSeq = stat->CurrentSequence();
		auto interval = stat->Interval();

		if( currSeq >= interval-1 )
		{
			// interval個前の適応値と現在の適応値の差分
			double delta1 = double( stat->BestFitness( currSeq % interval ) - stat->BestFitness( ( currSeq + 1 ) % interval ) ) / double( interval );
			// interval/2個前の適応値と現在の適応値の差分
			double delta2 = double( stat->BestFitness( currSeq % interval ) - stat->BestFitness( ( currSeq - interval / 2 ) % interval ) ) / double( interval / 2 );

			double conv = abs( delta1 - delta2 );

			//tcout << "delta1: " << delta1 << tendl;
			//tcout << "conv: " << conv << tendl;

			//if( delta1 < EPSILON_E6 && conv < EPSILON_E6 )
			//{
			//	tcout << "convergence detected" << tendl;
			//}

			convergence = delta1 + conv;
		}

		//double cnv = 0.0;
		//double base = double( stat->BestFitness( ( currSeq + 1 ) % interval ) );
		//double curr = double( stat->BestFitness( currSeq % interval ) );

		//if( m_currSeq >= interval-1 && curr != 0 )
		//{				
		//	cnv = ( curr - base ) / abs( base );
		//	//cnv = (double)( stat->BestFitness( ( m_currSeq + 1 ) % interval ) ) / (double)( stat->BestFitness( m_currSeq % interval ) );
		//}
		//return (float)cnv;


		return convergence < EPSILON_E5 || ( stat->CurrentIteration() >= stat->MaxIteration() );
	}



/*

		inline double convergence() const
		{
			if( m_currSeq >= m_Interval-1 )
			{
				// m_Interval個前の適応値と現在の適応値の差分
				double delta1 = double( m_BestFitnessSequence[m_currSeq%m_Interval] - m_BestFitnessSequence[( m_currSeq+1 )%m_Interval] ) / double( m_Interval );
				// m_Interval/2個前の適応値と現在の適応値の差分
				double delta2 = double( m_BestFitnessSequence[m_currSeq%m_Interval] - m_BestFitnessSequence[( m_currSeq-m_Interval/2 )%m_Interval] ) / double( m_Interval/2 );

				double conv = abs( delta1 - delta2 );

				//tcout << "delta1: " << delta1 << tendl;
				//tcout << "conv: " << conv << tendl;

				//if( delta1 < EPSILON_E6 && conv < EPSILON_E6 )
				//{
				//	tcout << "convergence detected" << tendl;
				//}

				return delta1 + conv;
				//return float( delta1 );
			}

			return 1.0;

			//double cnv = 0.0;
			//double base = double( m_BestFitnessSequence[( m_currSeq+1 )%m_Interval] );
			//double curr = double( m_BestFitnessSequence[m_currSeq%m_Interval] );

			//if( m_currSeq >= m_Interval-1 && curr != 0 )
			//{				
			//	cnv = ( curr - base ) / abs( base );
			//	//cnv = (double)( m_BestFitnessSequence[( m_currSeq+1 )%m_Interval] ) / (double)( m_BestFitnessSequence[m_currSeq%m_Interval] );
			//}
			//return (float)cnv;
		}



*/


}// end of namespace
