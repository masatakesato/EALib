﻿#ifndef	SIMPLE_GA_H
#define	SIMPLE_GA_H

#include	<oreore/container/Pair.h>

#include	"IEvolutionaryAlgorithm.h"



namespace ealib
{

	class CLASS_DECLSPEC SimpleGA : public IEvolutionaryAlgorithm
	{
		using Parents = Pair<int, int>;

	public:

		SimpleGA();
		SimpleGA( const SimpleGA& obj );
		~SimpleGA();

		// Set attribute
		using IEvolutionaryAlgorithm::SetAttribute;
		void SetAttribute( const SGAAttribute& attrib );
		void SetCrossoverRate( float c_rate );	// 交叉率の設定
		void SetMutationRate( float m_rate );	// 突然変異率を設定

		// Virtual Functions Override
		virtual bool InitPopulation( const DesignParamArray& designParams, int numObjectives );
		//virtual void InitPopulation( const IChromosome* pChromosome, int numObjectives );// Deprecated. 2021.08.23
		virtual void ReleasePopulation();
		virtual IEvolutionaryAlgorithm* Clone() const;
		virtual void Step( Evaluator* pEval );
		virtual void Evolve( Evaluator* pEval, unsigned int seed=0 );
		virtual Population* GetPopulation() const{ return (Population *)&m_Population[parentGen]; }
		virtual void TakeSnapshot( Population& pOut ) const;

		
	private:

		SGAAttribute	m_SGAAttrib;

		Population		m_Population[3];
		int				parentGen, childGen, dummy;

		int				m_NumFamilies;//, m_NumParents, m_NumChildren;
		//OreOreLib::Array<IChromosome*>	m_CrossoverBuffer;
		OreOreLib::Array<const IChromosome*>	X;//m_CrossoverBuffer;
		OreOreLib::Array<IChromosome*>	T;


		void Select_Crossover( Population* pParentPopulation, Population* pChildPopulation );// Select and Crossover
		void Mutate( Population* pPopulation );	// Mutation
		void CarryOver( Population* pParentPopulation, Population* pChildPopulation );// Carryover elite individuals

		void ClearAttribute();



		// Depracated.
		//void Select( Population* pPopulation );
		//void Crossover( Population* pParentPopulation, Population* pChildPopulation );

	};




	class CLASS_DECLSPEC MixedSimpleGA: public IEvolutionaryAlgorithm
	{
		using Parents = Pair<int, int>;

	public:

		MixedSimpleGA();
		MixedSimpleGA( const MixedSimpleGA& obj );
		~MixedSimpleGA();

		// Set attribute
		using IEvolutionaryAlgorithm::SetAttribute;
		void SetAttribute( const SGAAttribute& attrib );
		void SetCrossoverRate( float c_rate );	// 交叉率の設定
		void SetMutationRate( float m_rate );	// 突然変異率を設定

		// Virtual Functions Override
		virtual bool InitPopulation( const DesignParamArray& designParams, int numObjectives );
		//virtual void InitPopulation( const IChromosome* pChromosome, int numObjectives );// Deprecated. 2021.08.23
		virtual void ReleasePopulation();
		virtual IEvolutionaryAlgorithm* Clone() const;
		virtual void Step( Evaluator* pEval );
		virtual void Evolve( Evaluator* pEval, unsigned int seed=0 );
		virtual Population* GetPopulation() const{ return (Population *)&m_Population[ parentGen ]; }
		virtual void TakeSnapshot( Population& pOut ) const;



	private:

		SGAAttribute	m_SGAAttrib;

		Population		m_Population[3];
		int				parentGen, childGen, dummy;

		int m_NumFamilies;
		OreOreLib::Array<const IChromosome*> X;
		OreOreLib::Array<IChromosome*> T;


		void Select_Crossover( Population* pParentPopulation, Population* pChildPopulation );// Selection and Crossover
		void Mutate( Population* pPopulation );	// Mutation
		void CarryOver( Population* pParentPopulation, Population* pChildPopulation );// Carryover elite individuals

		void ClearAttribute();




		// Depracated.
		//void Select( Population* pPopulation );
		//void Crossover( Population* pParentPopulation, Population* pChildPopulation );

	};




}// end of namespace


#endif	// SIMPLE_GA_H //