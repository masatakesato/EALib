
//====================== Common =========================//
#include	"./Typedefs.h"
#include	"./DEAttribute.h"
#include	"./Population.h"



//=================== Initializers =======================//
#include	"./IInitOperator.h"
#include	"./Initializer.h"

#include	"./RandomInitializer.h"



//===================== Crossover ========================//
#include	"./ICrossoverOperator.h"
#include	"./Crossover.h"

// GA real/integer
#include	"./SBX.h"
#include	"./BLXalpha.h"
#include	"./EvenOddCrossover.h"
#include	"./ExponentialCrossover.h"

// GA BitString
#include	"./OnePointCrossover.h"
#include	"./TwoPointCrossover.h"
#include	"./NPointCrossover.h"
#include	"./UniformCrossover.h"

// DE only
#include	"./BDECrossover.h"
#include	"./BinaryBinomialCrossover.h"
#include	"./BinaryExponentialCrossover.h"
#include	"./BinomialCrossover.h"
#include	"./HammingDECrossover.h"



//======================= Mutator =======================//
#include	"./IMutationOperator.h"
#include	"./Mutator.h"

#include	"./GaussianMutation.h"
#include	"./FlipBitMutation.h"
#include	"./RandomResetMutation.h"
#include	"./CreepMutation.h"



//=================== DesignParameter =======================//
#include	"./DesignParameter.h"



//====================== Chromosomes ========================//
#include	"./IChromosome.h"
#include	"./Chromosome1D.h"
#include	"./Chromosome2D.h"



//====================== Selectors ========================//
#include	"./ISelector.h"
#include	"./RouletteWheelSelector.h"
#include	"./TournamentSelector.h"



//====================== Solvers ========================//
#include	"./IEvolutionaryAlgorithm.h"
#include	"./SimpleGA.h"
#include	"./MultiIslandEA.h"
#include	"./DifferentialEvolution.h"
#include	"./CoDE.h"
#include	"./jDE.h"
#include	"./JADE.h"
#include	"./SHADE.h"

#include	"./ParticleSwarmOptimization.h"



//================= Evaluator / result ==================//
#include	"./PopulationResult.h"
#include	"./Evaluator.h"



//================= Termination Criteria =================//
#include	"./ITerminationCriteria.h"
#include	"./TerminateOnIteration.h"
#include	"./TerminateOnConvergence.h"
#include	"./TerminateOnPopConvergence.h"



//====================== File I/O  =======================//
#include	"./DataIO.h"//"./FileIO.h"



//=================== Other helpers =====================//
#include	"./ChromosomeViewer.h"
#include	"./ChromosomeFactory.h"