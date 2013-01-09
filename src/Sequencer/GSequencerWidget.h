#ifndef GSEQUENCERWIDGET_H
#define GSEQUENCERWIDGET_H
#include "labexe_global.h"

#include <QWidget>
#include "ui_GSequencerWidget.h"

using namespace Ui;

/////////////////////////////////////////////////////////////////////
//! \brief The GSequencerWidget class defines a user interface to interact with a GSequence object. 
/*!
It contains:
- a GSequenceGraphicsView to display the tree structure of the events only
- a GSequenceGraphicsView to display the events and instructions in a set of channels
- more? QTableWidget
*/
class GSequencerWidget : public QWidget, public GSequencerWidgetClass
{
	Q_OBJECT

public:
	GSequencerWidget(QWidget *parent = 0);
	~GSequencerWidget();
};

#endif // GSEQUENCERWIDGET_H
