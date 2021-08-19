#include	<oreore/common/Utility.h>

#include	"labworks/EA/EAStatistics.h"
using namespace ealib;



int main( int argc, char **argv )
{
	EAStatistics stats;


	stats.Init( 100, 10, 4 );

	//stats.Update();
	//stats.Reset();

	stats.Release();


	EAStatistics stats2( stats );


	EAStatistics stats3	= stats2;


	EAStatistics stats4;
	stats4 = EAStatistics(stats3);


	EAStatistics *stats5 = new EAStatistics( stats4 );

	stats5->Release();

	stats5->Init( 1111, 111, 55 );

	SafeDelete( stats5 );

	return 0;
}