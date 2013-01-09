#include "GNomadAlgorithm.h"
#include "GNomadEvaluator.h"
#include "GParamDouble.h"

GNomadAlgorithm::GNomadAlgorithm(QObject *parent)
	: GOptimizationAlgorithm(parent)
	, m_ev(0)
	, m_Mads(0)
{

}

GNomadAlgorithm::~GNomadAlgorithm()
{

}

void GNomadAlgorithm::ConfigureVariables()
{
	// we assume the sender is a GVariableListRange
	const GVariableListRange* pVariables = qobject_cast<GVariableListRange*>(sender());
	if(!pVariables)
		return qWarning("The slot \"GNomadAlgorithm::ConfigureVariables\" was not triggered from a GVariableListRange");

	QList<GParamDouble*> pParamList = pVariables->ParamUsed();
	int nVariables = pParamList.count();

	// display precision:
	cout.precision(DISPLAY_PRECISION_STD);

	try {
		// parameters creation:
		m_NomadParam.reset();
		m_NomadParam.set_DIMENSION(nVariables);           // number of variables

		vector<bb_output_type> bbot (1); // definition of
		bbot[0] = _OBJ_;                 // output types
		//bbot[1] = _PB_;
		//bbot[2] = _EB_;
		m_NomadParam.set_BB_OUTPUT_TYPE ( bbot );

	//	m_NomadParam.set_DISPLAY_STATS ( "bbe ( sol ) obj" );

		// set the initial values and set the lower/upper bound
		Point stratingPoint(nVariables);
		Point lb(nVariables);
		Point ub(nVariables);
		QVector<double> usedLB = pVariables->ParamUsedLowerBounds();
		QVector<double> usedUB = pVariables->ParamUsedUpperBounds();
		QVector<double> usedStarting = pVariables->ParamUsedStartingValues();
		int indCoord = 0;
		foreach(GParamDouble* paramDb, pParamList) {
//			double startValue = 0;
//			if(paramDb)
//				startValue = double(*paramDb);
//			GDoubleRange* range = pVariables->Range(paramDb);
//			if(range) {
				// assigns lower value
				lb[indCoord] = usedLB[indCoord];//range->MinValue();
				// assigns upper value
				ub[indCoord] = usedUB[indCoord];//range->MaxValue();
				// bound the value if out of range
//				startValue = range->Bounded(startValue);
//			}
			// assigns starting value
			stratingPoint[indCoord] = usedStarting[indCoord];//startValue;
			indCoord++;
		}
		m_NomadParam.set_X0(stratingPoint);  // starting point
		m_NomadParam.set_LOWER_BOUND(lb);
		m_NomadParam.set_UPPER_BOUND(ub);

		m_NomadParam.set_MAX_BB_EVAL (100);     // the algorithm terminates after 100 black-box evaluations
//		m_NomadParam.set_INITIAL_MESH_SIZE(NOMAD::Double(0.1), true);
//		m_NomadParam.set_MIN_MESH_SIZE(0.01);
//		m_NomadParam.set_MIN_POLL_SIZE(0.01);

	// m_NomadParam.set_TMP_DIR ("/tmp");     // repertory for temporary files

	// parameters validation:
	m_NomadParam.check();

	m_NomadParam.display(cout);
	}
	catch ( exception & e ) {
		cerr << "\nNOMAD has been interrupted (" << e.what() << ")\n\n";

		return;
	}
}

void GNomadAlgorithm::run()
{
	try {
		// custom evaluator creation:
		GNomadEvaluator m_ev(m_NomadParam, 0);
		// algorithm creation and execution:
		Mads m_Mads(m_NomadParam, &m_ev, cout);

//		qRegisterMetaType<QList<double>>("QList<double>"); //Q_DECLARE_METATYPE(QList<double>) declared in "GOptimizationAlgorithm.h"
//		connect(&m_ev, SIGNAL(RequestEvaluation(QList<double>*, double*)), this, SIGNAL(RequestEvaluation(QList<double>*, double*)));
		connect(&m_ev, SIGNAL(RequestEvaluation(QList<double>*, double*)), this, SIGNAL(RequestEvaluation(QList<double>*, double*)), Qt::DirectConnection);

		m_Mads.run();
	}
	catch ( exception & e ) {
		cerr << "\nNOMAD has been interrupted (" << e.what() << ")\n\n";
		qWarning() << "\nNOMAD has been interrupted (" << e.what() << ")\n\n";
	}
	emit StartedFinished(false);
}

void GNomadAlgorithm::PopulateDeviceWidget( GDeviceWidget* theDeviceWidget )
{
	theDeviceWidget->DontDisplayTitle();
//	// 
//	QPushButton* pStartStop = new QPushButton("Run", theDeviceWidget);
//
//	// A QFormLayout, in which we place the label end the spinbox 
//	QBoxLayout* pLayout = new QVBoxLayout();
//	theDeviceWidget->AddSubLayout(pLayout);
//	pLayout->addWidget(pStartStop);
}
