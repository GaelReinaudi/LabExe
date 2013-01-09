#ifndef GSCENEBENCH_H
#define GSCENEBENCH_H
#include "labexegraphicsmap_global.h"

#include "TheLab/GWorkBench.h"

class GMapGraphicsScene;
class GMapGraphicsView;
class QGraphicsItem;

/////////////////////////////////////////////////////////////////////
//! \brief The GSceneBench class defines a workbench intended to display the Graphics map system. 
/*!
It is a GWorkBench where the central widget has been set to a GMapGraphicsView/Scene.
*/
class LABEXEGRAPHICSMAP_EXPORT GSceneBench : public GWorkBench
{
	Q_OBJECT

public:
	GSceneBench(QWidget *parent);
	~GSceneBench();

	// returns the scene that displays the image in its background.
	GMapGraphicsScene* Scene() const { return m_pScene; }

public:
	//! Re-implemented from GWorkBench in order to put the devices gui in the scene.
	GDeviceWidget* AddDevice(GDevice* pTheDevice, QPoint whereInParent = QPoint(0, 0), bool putOnStack = true);

	void RandomizePosition( QGraphicsItem* pItem );


private:
	//! This is the scene that this bench view.
	GMapGraphicsScene* m_pScene;
	//! the view that shows the scene
	GMapGraphicsView* m_pView;
	
};

#endif // GSCENEBENCH_H
