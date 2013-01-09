#ifndef GNLOptAlgorithm_H
#define GNLOptAlgorithm_H

#include "GOptimizationAlgorithm.h"
#include "nlopt/nlopt.hpp"

/////////////////////////////////////////////////////////////////////
//! \brief The GNLOptAlgorithm class implements the abstract GOptimizationAlgorithm to provide optimization by the NLOpt library. 
/*!
Its constructor actually takes an extra argument to specify which algorithm from the library is agoing to be used.
*/
class GNLOptAlgorithm : public GOptimizationAlgorithm
{
	Q_OBJECT

public:
	GNLOptAlgorithm(GOptimizer* parentOptimizer, nlopt::algorithm whichAlgorithm);
	~GNLOptAlgorithm();

	//! Re-implemented. Starts the optimization by instantiating a new nlopt::opt and calling optimize
	void RunOptimization();
	//! Re-implemented. Configures the algorithm parameters. Returns true if configuration went well.
	bool Configure();

protected:
	//! .....
	void run();

private:

	//! cleans up the m_pImmutableOptimzer if any, and returns a new one
	nlopt::opt* MakeNewNLoptOpt(nlopt::algorithm theAlgoIdentifier, uint NumVar) {
		if(m_pImmutableOptimzer) 
			delete m_pImmutableOptimzer;
		m_pImmutableOptimzer = new nlopt::opt(theAlgoIdentifier, NumVar);
		return m_pImmutableOptimzer;
	}
	nlopt::opt* m_pImmutableOptimzer;
	//! the int corresponding to the algo to call when running
	nlopt::algorithm m_AlgoNLoptIdentifier;
	friend double GlobalNLoptEvaluationFunction(const std::vector<double> &x, std::vector<double> &grad, void* f_data);
};

#endif // GNLOptAlgorithm_H
