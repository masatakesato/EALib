#include	"TerminateOnPopConvergence.h"

#include	"EAStatistics.h"



namespace ealib
{

	const float c_ConvergenceThreshold = 0.5f;// [0, 1]


	TerminateOnPopConvergence::TerminateOnPopConvergence()
	{

	}



	TerminateOnPopConvergence::~TerminateOnPopConvergence()
	{

	}



	bool TerminateOnPopConvergence::CheckTermination( EAStatistics* stat )
	{
		return stat->CurrentIteration() >= stat->MaxIteration();
	}


	// 元の実装が意味不明. 一旦後回し
	//bool TerminateOnPopConvergence::CheckTermination( EAStatistics* stat )
	//{
	//	auto currSeq = stat->CurrentSequence();
	//	auto interval = stat->Interval();


	//	bool result = false;

	//	
	//	float max_fitness	= stat->BestFitness( currSeq % interval );//ga.GetPopulation()->GetIndividual( 0 )->GetFitness();
	//	float min_fitness	= stat->WorstFitness( currSeq % interval );//ga.GetPopulation()->GetIndividual( ga.GetPopulation()->PopulationSize()-1 )->GetFitness();

	//	if( max_fitness== 0 )//ga.statistics().current( GAStatistics::Maximum ) == 0 )
	//	{
	//		return result;
	//	}

	//	//float ratio = min_fitness / max_fitness;
	//	//ga.statistics().current( GAStatistics::Minimum ) /
	//	//ga.statistics().current( GAStatistics::Maximum );

	//	//if( ga.m_Mode == SOLVE_MODE::MINIMIZE )
	//	//{
	//	//	if( ratio <= c_ConvergenceThreshold )//ga.pConvergence() )// min/max収束度合いの閾値を
	//	//		result = true;
	//	//	else
	//	//		result = false;
	//	//}
	//	//else
	//	//{
	//	//	if( ratio >= c_ConvergenceThreshold ) //ga.pConvergence() )
	//	//		result = true;
	//	//	else
	//	//		result = false;
	//	//}
	//
	//	//return result & ( stat->CurrentIteration() >= stat->MaxIteration() );
	//}




}// end of namespace
