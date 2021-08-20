#ifndef I_CONSTRAINER_H
#define	I_CONSTRAINER_H

#include	<oreore/common/DLLExport.h>



namespace ealib
{
	class IChromosome;


	enum CONSTRAINT_TYPE
	{
		CONSTRAINT_UNKNOWN,
		CONSTRAINT_DOMAIN,
		COSNTRAINT_EQUALITY,
		CONSTRAINT_INEQUALITY,
		NUM_CONSTRAINT,
	};


	class CLASS_DECLSPEC IConstrainer
	{
	public:

		IConstrainer(){}
		virtual ~IConstrainer(){}

		//virtual void Execute( int numchroms, IChromosome** chromosomes, const void* attribs )=0;


	private:

		CONSTRAINT_TYPE	m_Type;

	};


	//// 境界条件
	//class CLASS_DECLSPEC DomainConstrainer
	//{
	//public:

	//	DomainConstrainer(){}
	//	~DomainConstrainer(){}

	//	float Execute();


	//private:

	//};


	//// 等式
	//class CLASS_DECLSPEC EqualityConstrainer
	//{
	//public:

	//	EqualityConstrainer(){}
	//	~EqualityConstrainer(){}

	//	virtual float Execute();


	//private:


	//};


	//// 不等式
	//class CLASS_DECLSPEC InequalityConstrainer
	//{
	//public:

	//	InequalityConstrainer(){}
	//	~InequalityConstrainer(){}

	//	virtual float Execute();


	//private:


	//};



}// end of namespace




#endif // !I_CONSTRAINER_H
