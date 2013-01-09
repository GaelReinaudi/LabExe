#ifndef GEOGENETICALGORITHMWIDGET_H
#define GEOGENETICALGORITHMWIDGET_H

#include <QWidget>
#include "ui_GEoGeneticAlgorithmWidget.h"
#include "GEoGeneticAlgorithm.h"

class GEoGeneticAlgorithmWidget : public QWidget, public Ui::GEoGeneticAlgorithmWidget
{
	Q_OBJECT

public:
	GEoGeneticAlgorithmWidget(GEoGeneticAlgorithm* pEoAlgo, QWidget *parent = 0);
	~GEoGeneticAlgorithmWidget();

private:
	//! the algo
	GEoGeneticAlgorithm* m_pEoAlgo;
};

#endif // GEOGENETICALGORITHMWIDGET_H
