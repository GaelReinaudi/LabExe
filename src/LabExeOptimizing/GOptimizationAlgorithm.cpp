#include "GOptimizationAlgorithm.h"
#include "GOptimizer.h"
#include <QtCore>

GOptimizationAlgorithm::GOptimizationAlgorithm(GOptimizer* parentOptimizer)
	: QThread(parentOptimizer)
	, m_pOptimizer(parentOptimizer)
    , m_pSettingsWidget(new QWidget(nullptr))
    , m_KeepIterating(false)
{
	m_CleanUpHandler.add(m_pSettingsWidget);
	QLayout* pLay = new QVBoxLayout();
	pLay->setContentsMargins(1, 1, 1, 1);
	m_pSettingsWidget->setLayout(pLay);
}

GOptimizationAlgorithm::~GOptimizationAlgorithm()
{
	m_KeepIterating = false;
	wait();
}

double GOptimizationAlgorithm::EvalNewPosition( GVectorDouble NewPosVextor )
{
	if(!m_pOptimizer)
		return 1.23456789;
	
	double returnValue = 0.0;
	m_pOptimizer->SetNewValuesVariables(NewPosVextor, returnValue);

	return returnValue;
}
