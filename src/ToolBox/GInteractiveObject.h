#ifndef GINTERACTIVEOBJECT_H
#define GINTERACTIVEOBJECT_H
#include "labexe_global.h"

#include "ToolBox/GSerializable.h"
#include <QThread>

class QGraphicsItem;
/////////////////////////////////////////////////////////////////////
//! \brief The GInteractiveObject class is the base abstract class of any object in the program that can provide a user interface.
/*!
The user interface will be provided by calling the ProvideNewWidget() function. It is pure virtual since the user has to 
take care of laying out the gui and make appropriate connection (through signals and slots).
*/
class LABEXE_EXPORT GInteractiveObject : public QThread, public GSerializable
{
	Q_OBJECT

public:
	GInteractiveObject(QObject *parent);
	virtual ~GInteractiveObject();

	//! Returns a new Widget for interacting with this GInteractiveObject. 
	virtual QWidget* ProvideNewWidget(QWidget* inWhichWidget) = 0;
	//! Returns a new graphics item that represents the object. The default makes a graphics item by embedding a ProvideNewWidget().
	virtual QGraphicsItem* ProvideNewGraphicsItem(QGraphicsItem* pParentItem) = 0;

private:
	
};

#endif // GINTERACTIVEOBJECT_H
