#ifndef GMOTOR_H
#define GMOTOR_H
#include "labexe_global.h"

#include "GHardDevice.h"

/////////////////////////////////////////////////////////////////////
//! \brief A GMotor is a kind of GDevice that describes a motor.
/*!
The signal MovingOrStopped() has to be emitted by the implementation of the class in order to inform the rest of the program.
*/
class LABEXE_EXPORT GMotor : public GHardDevice
{
	Q_OBJECT

public:
	GMotor(QString uniqueIdentifierName, QObject *parent);
	~GMotor();

signals:
	//! Emitted when a move is being performed (true) or stopped (false).
	void MovingOrStopped(bool hasStartedOrHasStopped);

private:
	
};

#endif // GMOTOR_H
