#ifndef BENCHMARK_FUNCS_H
#define	BENCHMARK_FUNCS_H

// https://en.wikipedia.org/wiki/Test_functions_for_optimization
// http://qiita.com/tomitomi3/items/d4318bf7afbc1c835dda
// http://benchmarkfcns.xyz/benchmarkfcns/goldsteinpricefcn.html


#include	"IChromosome.h"
#include	"Evaluator.h"
using namespace ealib;


#include	<oreore/mathlib/MathLib.h>


// Ackley Function (supports n-dinemsional space).
class AckleyFunction : public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=0 )
	{
		double x_sqrd_sum	= 0;
		double x_cos_sum	= 0;
		double result		= 0;
		double n_inv		= 1.0 / double( chromosome->Size() );

		for( int i=0; i<chromosome->Size(); ++i )
		{
			double xi	= double( chromosome->GeneAs<float>( i ) );

			x_sqrd_sum	+= xi * xi;
			x_cos_sum	+= cos( 2.0 * M_PI * xi );
		}

		result	= 20.0 - 20.0 * exp( -0.2 * sqrt( n_inv * x_sqrd_sum ) ) + M_E - exp( n_inv * x_cos_sum );
		return float( result );

	}


	float MinVal() const	{ return -32.768f; }
	float MaxVal() const	{ return +32.768f; }

};



// Sphere Function (supports n-dinemsional space).
// -inf <= xi <= +inf
// f_min(0,...,0) = 0
class SphereFunction : public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=0 )
	{
		double result = 0;

		for( int i=0; i<chromosome->Size(); ++i )
		{
			double xi	= double( chromosome->GeneAs<float>( i ) );
			result	+= xi * xi;
		}

		return float( result );
	}

	float MinVal() const	{ return -FLT_MAX; }
	float MaxVal() const	{ return +FLT_MAX; }

};



// Rosenbrock Function (supports n-dinemsional space)..
// -5 <= xi <= +5
// f_min(1,...,1) = 0
class RosenbrockFunction : public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=0 )
	{
		double result = 0;

		for( int i=0; i<chromosome->Size()-1; ++i )
		{
			double xi		= double( chromosome->GeneAs<float>( i ) );
			double xi_1	= double( chromosome->GeneAs<float>( i + 1 ) );

			result += 100.0 * pow( xi_1 - xi * xi, 2.0 ) + pow( xi-1, 2.0 );
		}

		return float( result );
	}


	float MinVal() const	{ return -5.0f; }
	float MaxVal() const	{ return +5.0f; }
};



// Beale Function (2-dimensioanl space only).
class BealeFunction : public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=0 )
	{
		double x1	= double( chromosome->GeneAs<float>( 0 ) );
		double x2	= double( chromosome->GeneAs<float>( 1 ) );
		double result	= pow( 1.5 - x1 + x1 * x2, 2.0 ) + pow( 2.25 - x1 + x1*x2*x2, 2.0 ) + pow( 2.625 - x1 + x1 * x2*x2*x2, 2.0 );

		return float( result );
	}


	float MinVal() const	{ return -4.5f; }
	float MaxVal() const	{ return +4.5f; }
};



// Goldstein-Price Function (2-dimensional space only).
// f(0,-1)=3
class GoldsteinPriceFunction: public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=0 )
	{
		double x1	= double( chromosome->GeneAs<float>( 0 ) );
		double x2	= double( chromosome->GeneAs<float>( 1 ) );
		double result	= ( 1 + pow( x1 + x2 + 1, 2 ) * ( 19 - 14*x1 + 3*x1*x1 - 14*x2 + 6*x1*x2 + 3*x2*x2 ) ) *
							( 30 + pow( 2*x1 - 3*x2, 2 ) * ( 18 - 32*x1 + 12*x1*x1 + 48*x2 - 36*x1*x2 + 27*x2*x2 ) );

		return float( result );
	}


	float MinVal() const	{ return -2.0f; }
	float MaxVal() const	{ return +2.0f; }
};



// Bukin Function N.6 (2-dimensional space only).
//fmin( -10, 1 )=0
class BukinFunctionN6: public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=0 )
	{
		double x1	= double( chromosome->GeneAs<float>( 0 ) );
		double x2	= double( chromosome->GeneAs<float>( 1 ) );
		double result	= 100 * sqrt( fabs( x2 - 0.01*x2*x2 ) ) + 0.01 * fabs( x1 + 10 );

		return float( result );
	}


	float MinVal_x1() const	{ return -15.0f; }
	float MaxVal_x1() const	{ return -5.0f; }

	float MinVal_x2() const	{ return -3.0f; }
	float MaxVal_x2() const	{ return +3.0f; }
};



// Matyas Function (2-dimensional space only).
// fmin(0,0)=0
class MatyasFunction: public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=0 )
	{
		double x1	= double( chromosome->GeneAs<float>( 0 ) );
		double x2	= double( chromosome->GeneAs<float>( 1 ) );
		double result	= 0.26 * ( x1*x1 + x2*x2 ) - 0.48 * x1 * x2;

		return float( result );
	}


	float MinVal() const	{ return -10.0f; }
	float MaxVal() const	{ return +10.0f; }
};



// Levi Function N.13 (2-dimensional space only).
// fmin(1,1)=0
class LeviFunctionN13: public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=0 )
	{
		double x1	= double( chromosome->GeneAs<float>( 0 ) );
		double x2	= double( chromosome->GeneAs<float>( 1 ) );
		double result	= pow( sin( 3*M_PI*x1 ), 2 ) + pow( x1-1, 2 ) * ( 1 + pow( sin( 3*M_PI*x2 ), 2 ) ) + pow( x2-1, 2 ) * ( 1 + pow( sin( 2*M_PI*x2 ), 2 ) );

		return float( result );
	}


	float MinVal() const	{ return -10.0f; }
	float MaxVal() const	{ return +10.0f; }
};



// Three-Hump Camel Function (2-dimensional space only).
// fmin(0,0)=0
class ThreeHumpCamelFunction: public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=0 )
	{
		double x1	= double( chromosome->GeneAs<float>( 0 ) );
		double x2	= double( chromosome->GeneAs<float>( 1 ) );
		double result	= 2*x1*x1 - 1.05*pow( x1, 4 ) + pow( x1, 6 )/6 + x1*x2 + x2*x2;

		return float( result );
	}

	
	float MinVal() const	{ return -5.0f; }
	float MaxVal() const	{ return +5.0f; }
};



// Easom Function (2-dimensional space only).
// fmin( M_PI, M_PI )=-1
class EasomFunction: public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=0 )
	{
		double x1	= double( chromosome->GeneAs<float>( 0 ) );
		double x2	= double( chromosome->GeneAs<float>( 1 ) );
		double result	= -cos( x1 ) * cos( x2 ) * exp( -( pow( x1-M_PI, 2 ) + pow( x2-M_PI, 2 ) ) );

		return float( result );
	}


	float MinVal() const	{ return -100.0f; }
	float MaxVal() const	{ return +100.0f; }
};



// Eggholder Function (2-dimensional space only).
// fmin(512,404.2319)=-959.6407
class EggholderFunction: public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=0 )
	{
		double x1	= double( chromosome->GeneAs<float>( 0 ) );
		double x2	= double( chromosome->GeneAs<float>( 1 ) );
		double result	= -( x2 + 47 ) * sin( sqrt( fabs( x2 + x1/2 + 47 ) ) ) - x1 * sin( sqrt( fabs( x1 - ( x2 + 47 ) ) ) );

		return float( result );
	}


	float MinVal() const	{ return -512.0f; }
	float MaxVal() const	{ return +512.0f; }
};



// McCormick Function (2-dimensional space only).
// fmin(-0.54719,-1.54719)=-1.9133
class McCormickFunction : public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=0 )
	{
		double x1	= double( chromosome->GeneAs<float>( 0 ) );
		double x2	= double( chromosome->GeneAs<float>( 1 ) );
		double result	= sin( x1+x2 ) + pow( x1-x2, 2 ) - 1.5*x1 + 2.5*x2 + 1;

		return float( result );
	}


	float MinVal_x1() const	{ return -1.5f; }
	float MaxVal_x1() const	{ return +4.0f; }

	float MinVal_x2() const	{ return -3.0f; }
	float MaxVal_x2() const	{ return +4.0f; }
};



// Schaffer Function N.2 (2-dimensional space only).
class SchafferFunctionN2 : public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=0 )
	{
		double x1	= double( chromosome->GeneAs<float>( 0 ) );
		double x2	= double( chromosome->GeneAs<float>( 1 ) );
		double result	= ( 0.5 + pow( sin( x1*x1 - x2*x2 ), 2.0 ) - 0.5 ) / pow( 1.0 + .001 * ( x1*x1 + x2*x2 ), 2.0 );

		return float( result );
	}


	float MinVal_x1() const { return -100.0f; }
	float MaxVal_x1() const { return +100.0f; }

	float MinVal_x2() const { return -100.0f; }
	float MaxVal_x2() const { return +100.0f; }
};



// Schaffer Function N.4 (2-dimensional space only).
class SchafferFunctionN4: public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=0 )
	{
		double x1	= double( chromosome->GeneAs<float>( 0 ) );
		double x2	= double( chromosome->GeneAs<float>( 1 ) );
		double result	= ( 0.5 + pow(  cos( sin( x1*x1 - x2*x2 )), 2.0 ) - 0.5 ) / pow( 1.0 + .001 * ( x1*x1 + x2*x2 ), 2.0 );

		return float( result );
	}


	float MinVal_x1() const { return -100.0f; }
	float MaxVal_x1() const { return +100.0f; }

	float MinVal_x2() const { return -100.0f; }
	float MaxVal_x2() const { return +100.0f; }
};



// Styblinski-Tank Function (N-dimensional space only).
class StyblinskiTankFunction: public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=0 )
	{
		double result = 0;

		for( int i=0; i<chromosome->Size(); ++i )
		{
			double xi	= double( chromosome->GeneAs<float>( i ) );
			result	+= pow( xi, 4.0 ) - 16.0 * pow( xi, 2.0 ) + 5.0 * xi;
		}

		return float( 0.5 * result );
	}


	float MinVal_x1() const { return -100.0f; }
	float MaxVal_x1() const { return +100.0f; }

	float MinVal_x2() const { return -100.0f; }
	float MaxVal_x2() const { return +100.0f; }
};





#endif // !BENCHMARK_FUNCS_H
