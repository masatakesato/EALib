﻿#ifndef DE_CURRENT_TO_PBEST_1_H
#define	DE_CURRENT_TO_PBEST_1_H


#include	"IDEMutationStrategy.h"



namespace ealib
{

	class CLASS_DECLSPEC DE_Current_to_pBest_1 : public IDEMutationStrategy
	{
	public:

		DE_Current_to_pBest_1();
		virtual ~DE_Current_to_pBest_1();

		// Virtual Functions Override
		virtual void Execute( int num, IChromosome** selections, int current );


		void SetP( float p );


	private:
		
		float m_P;

	};



}// end of namespace



#endif // !DE_CURRENT_TO_PBEST_1_H
