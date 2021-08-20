#include	<oreore/common/Utility.h>
#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MathLib.h>


#include	<ealib/IChromosome.h>
using namespace ealib;


// http://www.dais.is.tohoku.ac.jp/~shioura/teaching/mp07/mp07-12.pdf


template < typename T >
class IConstraint
{
public:

	T Execute( const IChromosome* chromosome ) = 0;

};




template < typename T >
class PenaltyConstraint : IConstraint<T>
{
public:

	virtual T Execute( const IChromosome* chromosome )
	{
		return 0;
	}

};








template < typename T >
T G( T& param )
{

	return 0;
}



// penalty function for "<= 0"
template < typename T >
T PenaltyFunction_LessThan( float mu, int numparams, const T x[] )
{
	T P = 0;

	for( int i=0; i<numparams; ++i )
		P += Max( G(x[i]), 0 );
	
	return P;
}



// penalty function for "== 0"
template < typename T >
T PenaltyFunction_Equal( float mu, int numparams, const T x[] )
{
	T P = 0;

	for( int i=0; i<numparams; ++i )
		//P += abs( G(x[i]) );
		P += pow( G(x[i]), 2 );
	
	return P;
}




template < typename T >
T BarrierFunction( float mu, int numparams, const T x[] )
{



	return 0;
}





int main()
{




	return 0;
}