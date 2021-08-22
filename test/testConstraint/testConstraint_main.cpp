#include	<oreore/common/Utility.h>
#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MathLib.h>
#include	<oreore/container/Array.h>


#include	<ealib/IChromosome.h>
using namespace ealib;


// http://www.dais.is.tohoku.ac.jp/~shioura/teaching/mp07/mp07-12.pdf
// http://www.orsj.or.jp/~archive/pdf/bul/Vol.40_02_112.pdf


// 一旦中止. 以下3点の問題解決が必要. 簡易的な計算式のノードグラフ必要. 2021.08.22

// 可変長パラメータの制約関数をどう定義するか

// 入出力変数群をどう定義するか

// 入出力変数群と制約関数との接続をどう実現するか


class IConstraintOperator
{
public:

	template < typename ... Args >
	void G( const IChromosome* chromosome, Args... args )
	{
		G( chromosome );
		G( args... );
	}



	virtual void G( const IChromosome* chromosome )
	{
		tcout << "IConstraintOperator::G( const IChromosome* chromosome );\n";
	}

};




class MyConstraintOp : public IConstraintOperator
{
public:

	using IConstraintOperator::G;

	virtual void G( IChromosome* chromosome, int threshold )
	{
		tcout << "MyConstraintOp::G( IChromosome* chromosome, int threshold )..." << threshold << tendl;

	}
};



class MyConstraintOp2 : public IConstraintOperator
{
public:

	using IConstraintOperator::G;

	virtual void G( const tstring& tstr )
	{
		tcout << "MyConstraintOp2::G( int threshold )..." << tstr << tendl;

	}
};





int main()
{

	MyConstraintOp op;
	op.G( nullptr, 6 );


	MyConstraintOp2 op2;
	op2.G( _T("gfdsgfds") );


	IConstraintOperator* pOp = &op;

	pOp->G( nullptr, _T("AAAAA") );


	return 0;
}







//template < typename T >
//T G( const IChromosome* chromosome )
//{
//
//	return (T)0;
//}


//class IConstraint
//{
//public:
//
//	virtual double Execute( const IChromosome* chromosome ) = 0;
//
//
//	void AddLessThanOperator(){}
//	void AddEqualOperator(){}
//
//};




//// penalty function method
//class ExteriorPenalty : IConstraint
//{
//public:
//
//	virtual double Execute( const IChromosome* chromosome )
//	{
//		return 0;
//	}
//
//
//
//private:
//
//	OreOreLib::Array< double* >	m_Inequalities;
//	OreOreLib::Array< double* >	m_Equalities;
//
//
//
//	// penalty function for "<= 0"
//	double AccumulateLessEqual( float mu, int numFuncs, const IChromosome* chromosome )
//	{
//		double P = 0;
//
//		for( int i=0; i<numFuncs; ++i )
//			P += Max( G<double>( chromosome ), 0.0 );
//	
//		return P;
//	}
//
//
//
//	// penalty function for "== 0"
//	double AccumulateEqual( float mu, int numFuncs, const IChromosome* chromosome )
//	{
//		double P = 0;
//
//		for( int i=0; i<numFuncs; ++i )
//			//P += abs( G( chromosome ) );
//			P += pow( G<double>( chromosome ), 2.0 );
//	
//		return P;
//	}
//
//
//};



//// Barrier function method
//class InteriorPenalty : IConstraint
//{
//public:
//
//	virtual double Execute( const IChromosome* chromosome )
//	{
//		return 0;
//	}
//
//
//
//private:
//
//
//	// penalty function for "<= 0"
//	double AccumulateLessEqual( float mu, int numFuncs, const IChromosome* x )
//	{
//		double P = 0;
//
//		for( int i=0; i<numFuncs; ++i )
//			P -= 1.0 / G<double>( x );
//	
//		return P;
//	}
//
//
//
//	// penalty function for ">= 0"
//	double AccumulateGreaterEqual( float mu, int numFuncs, const IChromosome* x )
//	{
//		double P = 0;
//
//		for( int i=0; i<numFuncs; ++i )
//			P -= log( -G<double>( x ) );
//	
//		return P;
//	}
//
//
//};
