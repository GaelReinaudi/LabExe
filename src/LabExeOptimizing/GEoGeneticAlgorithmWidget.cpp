#include "GEoGeneticAlgorithmWidget.h"

GEoGeneticAlgorithmWidget::GEoGeneticAlgorithmWidget(GEoGeneticAlgorithm* pEoAlgo, QWidget *parent /*= 0*/)
	: QWidget(parent)
	, m_pEoAlgo(pEoAlgo)
{
	setupUi(this);

	layout()->addWidget(m_pEoAlgo->m_PopulationSize.ProvideNewLabel(this));
	layout()->addWidget(m_pEoAlgo->m_PopulationSize.ProvideNewParamSpinBox(this));
	layout()->addWidget(m_pEoAlgo->m_CrossOverProbability.ProvideNewLabel(this));
	layout()->addWidget(m_pEoAlgo->m_CrossOverProbability.ProvideNewParamSpinBox(this));
	layout()->addWidget(m_pEoAlgo->m_MutationProbability.ProvideNewLabel(this));
	layout()->addWidget(m_pEoAlgo->m_MutationProbability.ProvideNewParamSpinBox(this));
	layout()->addWidget(m_pEoAlgo->m_CurrentGeneration.ProvideNewLabel(this));
	layout()->addWidget(m_pEoAlgo->m_CurrentGeneration.ProvideNewParamSpinBox(this));

	layout()->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

GEoGeneticAlgorithmWidget::~GEoGeneticAlgorithmWidget()
{

}
