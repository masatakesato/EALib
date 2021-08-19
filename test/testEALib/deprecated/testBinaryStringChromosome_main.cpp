#include	<crtdbg.h>
#include	<iostream>
using namespace std;

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MersenneTwister.h>


//#include	"labworks/EA/BinaryStringChromosome.h"
//
//#include	"labworks/EA/RandomInitializer.h"
//
//#include	"labworks/EA/OnePointCrossover.h"
//#include	"labworks/EA/TwoPointCrossover.h"
//#include	"labworks/EA/NPointCrossover.h"
//#include	"labworks/EA/UniformCrossover.h"

#include	<labworks/EA/EALib.h>

using namespace ealib;



int main( int argc, char **argv )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );


	tcout << "//=============== BinaryStringChromosome test ==================//" << tendl;

	RandomInitializer	initRandom;


	BinaryStringChromosome chrom1, chrom2;

	chrom1.Init( 32 );
	
	tcout << "Init chrom1 with random bits..." << tendl;
	chrom1.Initialize( &initRandom );
	DisplayChromosome( &chrom1, true );

	tcout << "Copy chrom1 to chrom1 and display..." << tendl;
	chrom2	= chrom1;
	DisplayChromosome( &chrom2, true );

	tcout << "Reinit chrom2 with 1..." << tendl;
	chrom2.Gene()->SetAll();
	DisplayChromosome( &chrom2, true );

	tcout << "Reinit chrom2 with 0..." << tendl;
	chrom2.ClearGene();// chrom2.Gene()->UnsetAll();
	DisplayChromosome( &chrom2, true );



	tcout << "//=============== Crossover test ==================//" << tendl;

	OnePointCrossover<BitArray>		operator_OnePoint;
	TwoPointCrossover<BitArray>		operator_TwoPoint;
	NPointCrossover<BitArray, 3>	operator_NPoint;
	UniformCrossover<BitArray>		operator_Uniform;

	BinaryStringChromosome	ch1, ch2, ch3, ch4;
	IChromosome* chroms[] ={ &ch1, &ch2, &ch3, &ch4 };

	ch1.Init( 16 );
	ch2.Init( 16 );
	ch3.Init( 16 );
	ch4.Init( 16 );

	ch1.Gene()->SetAll();// set all bits of ch1 to 1
	ch2.Gene()->UnsetAll();// set all bits ch2 to 0

	tcout << "----------- Parents -----------" << tendl;
	DisplayChromosome( &ch1, true );
	DisplayChromosome( &ch2, true );

	//operator_OnePoint.Execute( 4, chroms, NULL );
	//operator_TwoPoint.Execute( 4, chroms, NULL );
	//operator_NPoint.Execute( 4, chroms, NULL );
	operator_Uniform.Execute( 4, chroms, NULL );

	tcout << "\n----------- Children -----------" << tendl;
	DisplayChromosome( &ch3, true );
	DisplayChromosome( &ch4, true );


	return 0;
}