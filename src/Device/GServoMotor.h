#ifndef GSERVOMOTOR_H
#define GSERVOMOTOR_H
#include "labexe_global.h"

#include "GMotor.h"
#include "Param/GParamDouble.h"
#include "Param/GParamBool.h"

/////////////////////////////////////////////////////////////////////
//! \brief A GServoMotor is a kind of GMotor that describes a servo-motor.
/*!
A servo-motor has a target position that it will try to reach when engaged.
*/
class LABEXE_EXPORT GServoMotor : public GMotor
{
	Q_OBJECT

public:
	GServoMotor(QString uniqueIdentifierName, QObject *parent);
	~GServoMotor();

	//! returns the target position
	double TargetPosition() {return m_TargetPos;}


public slots:
	//! Moves the servo to the absolute position newPos.
	virtual void MoveAbsolute(double newPos) = 0;
	//! Sets the maximum velocity the servomotor should be operated at
	virtual void SetMaxVelocity(double maxVel) = 0;
	//! Sets the maximum acceleration  the servomotor should be operated at
	virtual void SetMaxAcceleration(double maxAcc) = 0;
	//! Makes the motor to disengage everytime it has reach its target position. This can prevent some vibration due to the servomotor feedback.
	void DisEngageAfterMove(bool doDisengageAfterMove);

protected slots:
	//! Engages (powers) or Disengages the motor and go to the TargetPos
	virtual void Engage(bool doEngage) = 0;
	//! Disengages the motor
	virtual void Disengage() = 0;

signals:
	//! Emitted when the target position is reached (or should have been reached). successful = false if any problem (e.g. the servo is not plugged in).
	void FinishedMoveToTargetPosition(bool successful);

protected:
	//! The target position
	GParamDouble m_TargetPos;
	//! The option to disengage the servo after the phidget has performed a move
	GParamBool m_DisengageAfterMove;
	//! The extra time to wait before disengaging after a move. Gives room for the servo to physically reach its destination.
	GParamDouble m_DelayToDisEngageAfterMove;

private slots:
	//! Starts the timer that will disengage the servo after m_DelayToDisEngageAfterMove
	void StartDisengageTimer();
	//! Called by the timer that disengages the servo. Inside, it prevents the servo to be disengaged if another move has been requested since the timer was started from a previous move
	void EventTimerDisengage();

private:
	int TimerRunning;
};

#endif // GSERVOMOTOR_H
