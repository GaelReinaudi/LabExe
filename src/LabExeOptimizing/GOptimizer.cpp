#include "GOptimizer.h"
#include "GOptimizingBench.h"
#include "GOptimizingWidget.h"

#include "GNLOptAlgorithm.h"
#include "GEoGeneticAlgorithm.h"

G_REGISTER_PROG_DEVICE_CLASS(GOptimizer)

GOptimizer::GOptimizer(QObject* pParent, QString uniqueIdentifierName /*= ""*/)
	: GProgDevice(pParent, uniqueIdentifierName)
	, m_BucketParamOptim("Optimize", this)
	, m_BucketVariables("Variables", this)
	, m_pCurrentAlgo(0)
	, m_LastFitnessValue("Fitness", this, GParam::ReadOnly)
	, m_MaxFitnessValue("Max fitness", this, GParam::ReadOnly)
	, m_WaitTimeIteration("Waiting after iteration", this)
	, m_MaximizeOrMinimize("Maximize", this)
	, m_WaitingForResult(false)
{
	m_BucketVariables.RemoveExtraField("trig");

	m_BucketVariables.AddExtraFieldBool("opt");

//	m_BucketVariables.AddExtraFieldBool("limit");
// 	m_BucketVariables.AddExtraFieldDouble("min");
// 	m_BucketVariables.AddExtraFieldDouble("max");
	m_BucketVariables.AddExtraFieldDouble("center");
	m_BucketVariables.AddExtraFieldDouble("+/-");

	// when adding extra params such as range, we can perform extra initialization/connections
	connect(&m_BucketVariables, SIGNAL(ParamAdded(GParam*)), this, SLOT(Event_VariableAdded(GParam*)));

	// when the value to optimize is updates we call a function that will make the optimizer to proceed
	connect(&m_BucketParamOptim, SIGNAL(ValueUpdated(double)), this, SLOT(Event_ResultUpdated(double)));

	m_WaitTimeIteration.SetHardLimits(0.0, 99.9);
	m_WaitTimeIteration.SetTypicalStep(0.1);

	AddNewAlgorithm(new GEoGeneticAlgorithm(this)								, "Genetic"						, "Testing the implementation of the EO librabry for genetic algorithm.");

	AddNewAlgorithm(new GNLOptAlgorithm(this, nlopt::GN_DIRECT)					, "GN_DIRECT"					, "deterministic-search algorithms based on systematic division of the search domain into smaller and smaller hyperrectangles. ");
	AddNewAlgorithm(new GNLOptAlgorithm(this, nlopt::GN_DIRECT_L)				, "GN_DIRECT_L"					, "deterministic-search algorithms based on systematic division of the search domain into smaller and smaller hyperrectangles. The Gablonsky version makes the algorithm \"more biased towards local search\" so that it is more efficient for functions without too many local minima. NLopt contains several implementations of both of these algorithms.");
	AddNewAlgorithm(new GNLOptAlgorithm(this, nlopt::GN_CRS2_LM)				, "GN_CRS2_LM"					, "Controlled Random Search (CRS) with local mutation. The CRS algorithms are sometimes compared to genetic algorithms, in that they start with a random \"population\" of points, and randomly \"evolve\" these points by heuristic rules. In this case, the \"evolution\" somewhat resembles a randomized Nelder-Mead algorithm.");
	AddNewAlgorithm(new GNLOptAlgorithm(this, nlopt::GN_MLSL)					, "GN_MLSL"						, "MLSL (Multi-Level Single-Linkage). In either case, MLSL is a \"multistart\" algorithm: it works by doing a sequence of local optimizations (using some other local optimization algorithm) from random or low-discrepancy starting points. MLSL is distinguished, however by a \"clustering\" heuristic that helps it to avoid repeated searches of the same local optima, and has some theoretical guarantees of finding all local optima in a finite number of local minimizations.");
	AddNewAlgorithm(new GNLOptAlgorithm(this, nlopt::GN_ISRES)					, "GN_ISRES"					, "\"Improved Stochastic Ranking Evolution Strategy\" (ISRES) algorithm for nonlinearly-constrained global optimization. The evolution strategy is based on a combination of a mutation rule (with a log-normal step-size update and exponential smoothing) and differential variation (a Nelder–Mead-like update rule). The fitness ranking is simply via the objective function for problems without nonlinear constraints. The population size for ISRES defaults to 20×(n+1) in n dimensions");
	AddNewAlgorithm(new GNLOptAlgorithm(this, nlopt::LN_PRAXIS)					, "LN_PRAXIS"					, "\"PRAXIS\" gradient-free local optimization via the \"principal-axis method\" of Richard Brent");
	AddNewAlgorithm(new GNLOptAlgorithm(this, nlopt::LN_COBYLA)					, "LN_COBYLA"					, "COBYLA (Constrained Optimization BY Linear Approximations). It constructs successive linear approximations of the objective function and constraints via a simplex of n+1 points (in n dimensions), and optimizes these approximations in a trust region at each step.");
	AddNewAlgorithm(new GNLOptAlgorithm(this, nlopt::LN_NEWUOA)					, "LN_NEWUOA"					, "The original NEWUOA performs derivative-free unconstrained optimization using an iteratively constructed quadratic approximation for the objective function. Because NEWUOA constructs a quadratic approximation of the objective, it may perform poorly for objective functions that are not twice-differentiable.");
	AddNewAlgorithm(new GNLOptAlgorithm(this, nlopt::LN_NELDERMEAD)				, "LN_NELDERMEAD"				, "Nelder-Mead Simplex. This method is simple and has demonstrated enduring popularity, despite the later discovery that it fails to converge at all for some functions. Anecdotal evidence suggests that it often performs well even for noisy and/or discontinuous objective functions.");
	AddNewAlgorithm(new GNLOptAlgorithm(this, nlopt::LN_SBPLX)					, "LN_SBPLX"					, "Sbplx (based on Subplex). Subplex (a variant of Nelder-Mead that uses Nelder-Mead on a sequence of subspaces) is claimed to be much more efficient and robust than the original Nelder-Mead, while retaining the latter's facility with discontinuous objectives.");
	AddNewAlgorithm(new GNLOptAlgorithm(this, nlopt::LN_AUGLAG)					, "LN_AUGLAG"					, "Augmented Lagrangian algorithm. This method combines the objective function and the nonlinear inequality/equality constraints (if any) in to a single function: essentially, the objective plus a \"penalty\" for any violated constraints. This modified objective function is then passed to another optimization algorithm with no nonlinear constraints. If the constraints are violated by the solution of this sub-problem, then the size of the penalties is increased and the process is repeated; eventually, the process must converge to the desired solution (if it exists).");
	AddNewAlgorithm(new GNLOptAlgorithm(this, nlopt::LN_BOBYQA)					, "LN_BOBYQA"					, "BOBYQA performs derivative-free bound-constrained optimization using an iteratively constructed quadratic approximation for the objective function. Because BOBYQA constructs a quadratic approximation of the objective, it may perform poorly for objective functions that are not twice-differentiable.");

	SetAlgorithm(m_ListAlgo.value(0));
	m_MaximizeOrMinimize = true;
	m_WaitTimeIteration = 0.1;

	connect(this, SIGNAL(AlgoStopped(bool)), this, SLOT(EventAlgoStopped()));
}

GOptimizer::~GOptimizer()
{
	if(m_pCurrentAlgo) {
		m_pCurrentAlgo->m_KeepIterating = false;
		m_pCurrentAlgo->wait();
	}
}

void GOptimizer::Event_VariableAdded( GParam* theParam )
{
	GParamNum* pParNum = qobject_cast<GParamNum*>(theParam);
	if(!pParNum)
		return;

	// set the "+/-"to be positive
	GParamNum* pCenter = m_BucketVariables.ExtraParamNum(pParNum, "center");
	GParamNum* pPlusMinus = m_BucketVariables.ExtraParamNum(pParNum, "+/-");
	if(pPlusMinus && pCenter) {
		double parmin = pParNum->Minimum();
		double parmax = pParNum->Maximum();
		double center = 0.5 * (parmax + parmin);
		double plmi = 0.5 * qAbs(parmax - parmin);
		pCenter->SetHardLimits(parmin, parmax);
		pPlusMinus->SetHardLimits(0.0, plmi);

		pCenter->SetParamValue(center);
		pPlusMinus->SetParamValue(plmi);

		//TODO connect(pParNum, SIGNAL(HardLimitsChanged(double, double)), this SLOT(Event_VariableAdded()));
	}
}


GDeviceWidget* GOptimizer::ProvideNewDeviceGroupBox( QWidget* inWhichWidget, QBoxLayout::Direction orientation /* = QBoxLayout::LeftToRight */ )
{
    Q_UNUSED(orientation);
	return new GOptimizingWidget(this, inWhichWidget);
}

void GOptimizer::PopulateSettings( QSettings& inQsettings )
{
	GProgDevice::PopulateSettings(inQsettings);
	inQsettings.setValue("Algo", m_AlgoNames.value(m_pCurrentAlgo));
}

void GOptimizer::InterpretSettings( QSettings& fromQsettings )
{
	GProgDevice::InterpretSettings(fromQsettings);
	QString algoName = fromQsettings.value("Algo", "").toString();
	if(algoName != "")
		SetAlgorithm(m_AlgoNames.key(algoName));
	else
		SetAlgorithm(m_ListAlgo.value(0));
}

void GOptimizer::StartStopAlgorithm( bool doStart )
{
	QAbstractButton* pButton = qobject_cast<QAbstractButton*>(sender());
	if(!m_pCurrentAlgo){
		if(pButton)
			pButton->setChecked(false);
		qWarning() << "There is no algorithm currently assigned to the optimizer.";
		return;
	}
	if(!doStart) {
		m_pCurrentAlgo->m_KeepIterating = false;
		return;
	}
	if(m_pCurrentAlgo->isRunning()) {
		qWarning() << "The algorithm is already running.";
		return;
	}
	if(m_BucketParamOptim.IsEmpty()) {
		qWarning() << "There is no fitness parameter.";
		return;
	}

	// set the access mask of the variables
	m_AccessMask = m_BucketVariables.ExtraParamBoolMask("opt");
	bool configuredOK = m_pCurrentAlgo->Configure();

	// updates the fitness value with the current value of the bucket
	UpdateFitnessValue();
	// makes the value 0 in the  but then set it to ultra low without sending signals. Useful to have a displayed starting value of 0.0, but making sure it will increase for the next update of the m_MaxFitnessValue 
	m_MaxFitnessValue = 0.0;
	m_MaxFitnessValue.SetParamValue(-1e40, false, false);

	if(configuredOK) {
		m_pCurrentAlgo->m_KeepIterating = true;
		m_pCurrentAlgo->start();
	}
}

void GOptimizer::SetAlgorithm( GOptimizationAlgorithm* val )
{
	if(m_pCurrentAlgo)
		disconnect(m_pCurrentAlgo, SIGNAL(FinishedOptimizing()), this, SIGNAL(AlgoStopped()));
	m_pCurrentAlgo = val;
	if(m_pCurrentAlgo)
		connect(m_pCurrentAlgo, SIGNAL(FinishedOptimizing()), this, SIGNAL(AlgoStopped()));
	emit AlgorithmSelected(m_pCurrentAlgo);
}

void GOptimizer::SetAlgorithm( int indexAlgoList )
{
	GOptimizationAlgorithm* pAlgo = m_ListAlgo.value(indexAlgoList);
	if(pAlgo)
		SetAlgorithm(pAlgo);
}

void GOptimizer::AddNewAlgorithm( GOptimizationAlgorithm* pAlgo, QString name, QString description /*= "No description available" */ )
{
	if(!pAlgo)
		return;
	m_ListAlgo.append(pAlgo);
	m_AlgoNames.insert(pAlgo, name);
	m_AlgoWidgets.insert(pAlgo, pAlgo->SettingsWidget());
	m_AlgoDescritpions.insert(pAlgo, description);
	emit NewAlgorithmAdded(pAlgo);
}

GVectorDouble const GOptimizer::CurrentValuesVariable() const
{
	return m_BucketVariables.DoubleValues("opt");
}


GVectorDouble const GOptimizer::CenterValuesVariable() const
{
	GVectorDouble center;
	foreach(GParamNum* pPar, m_BucketVariables.ExtraParamsNum("center", "opt")) {
		double val = pPar->DoubleValue();
		center.append(val);
	}
	return center;
}

GVectorDouble const GOptimizer::LowerBoundsVariable() const
{
	GVectorDouble lb;
	foreach(GParamNum* pPar, m_BucketVariables.ParamNums(m_AccessMask)) {
// 		double lower = m_BucketVariables.ExtraParamNum(pPar, "min")->DoubleValue();
		double lower = m_BucketVariables.ExtraParamNum(pPar, "center")->DoubleValue() - m_BucketVariables.ExtraParamNum(pPar, "+/-")->DoubleValue();
		lower -= 1e-9;
		lb.append(lower);
	}
	return lb;
}

GVectorDouble const GOptimizer::UpperBoundsVariable() const
{
	GVectorDouble ub;
	foreach(GParamNum* pPar, m_BucketVariables.ParamNums(m_AccessMask)) {
// 		double upper = m_BucketVariables.ExtraParamNum(pPar, "max")->DoubleValue();
		double upper = m_BucketVariables.ExtraParamNum(pPar, "center")->DoubleValue() + m_BucketVariables.ExtraParamNum(pPar, "+/-")->DoubleValue();
		upper += 1e-9;
		ub.append(upper);
	}
	return ub;
}

GVectorDouble const GOptimizer::TypicalStepsVariable() const
{
	GVectorDouble ts;
	foreach(GParamNum* pPar, m_BucketVariables.ParamNums(m_AccessMask)) {
		ts.append(pPar->TypicalStep());
	}
	return ts;
}

void GOptimizer::SetNewValuesVariables( GVectorDouble newValues, double & returnValue )
{
	m_WaitingForResult = true;

	m_BucketVariables.SetValues(newValues, m_AccessMask);
	GVectorDouble allValues = m_BucketVariables.DoubleValues();

	while(m_WaitingForResult)
		msleep(100);

	// TO CHANGE : their should be something listening for the param update colpetion
	if(m_WaitTimeIteration.DoubleValue() > 0.0)
		msleep(m_WaitTimeIteration.DoubleValue() * 1000);

	returnValue = UpdateFitnessValue();

	double maxfit = qMax(returnValue, m_MaxFitnessValue.DoubleValue());
	m_MaxFitnessValue.SetParamValue(maxfit);

	// send the result to the widget to keep a track of it.
	emit ResultToRecord(allValues, returnValue);
}

double GOptimizer::UpdateFitnessValue()
{
	double retValue = 0.0;
	GParamNum* pparnum = m_BucketParamOptim.ParamNum();
	if(m_MaximizeOrMinimize)
		retValue = pparnum->DoubleValue(); 
	else
		retValue = - pparnum->DoubleValue(); 
	m_LastFitnessValue.SetParamValue(retValue);
	return retValue;
}

void GOptimizer::EventAlgoStopped()
{
	// clears the mask so that all variables are accessed by default, unless the algo doStart
	m_AccessMask.clear();
}

void GOptimizer::Event_ResultUpdated( double newVal )
{
    Q_UNUSED(newVal);
    m_WaitingForResult = false;
}






