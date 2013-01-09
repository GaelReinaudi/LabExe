#include "GServoMotor.h"

GServoMotor::GServoMotor(QString uniqueIdentifierName, QObject *parent)
	: GMotor(uniqueIdentifierName, parent)
	, m_TargetPos(DefaultShortName(), this)
	, m_DisengageAfterMove("Disengage after move", this)
	, m_DelayToDisEngageAfterMove("Disengage delay [s]", this)
	, TimerRunning(0)
{
	AUTOSET_UNIQUE_IDENTIFIER(m_TargetPos);

	m_TargetPos.SetExternalCompletionSignal(this, SIGNAL(FinishedMoveToTargetPosition(bool)));
	connect(&m_TargetPos, SIGNAL(ValueUpdated(double)), this, SLOT(MoveAbsolute(double)));
	// let's disengage after each move so that there is no vibration in the servo at rest.
	connect(&m_DisengageAfterMove, SIGNAL(ValueUpdated(bool)), this, SLOT(DisEngageAfterMove(bool)));

	m_DelayToDisEngageAfterMove.SetHardLimits(0.0, 9.99999);
	m_DelayToDisEngageAfterMove.SetTypicalStep(0.010);
	m_DelayToDisEngageAfterMove.SetDisplayDecimals(2);
	m_DelayToDisEngageAfterMove = 0.5;

	connect(this, SIGNAL(NameChanged(QString)), &m_TargetPos, SLOT(SetName(QString)));
}

GServoMotor::~GServoMotor()
{

}

void GServoMotor::DisEngageAfterMove( bool doDisengageAfterMove )
{
 	if(doDisengageAfterMove)
 		connect(this, SIGNAL(FinishedMoveToTargetPosition(bool)), this, SLOT(StartDisengageTimer()));//, Qt::QueuedConnection);
 	else
 		disconnect(this, SIGNAL(FinishedMoveToTargetPosition(bool)), this, SLOT(StartDisengageTimer()));
	TimerRunning = 0;
}

void GServoMotor::StartDisengageTimer()
{
// 	justMovedTo = m_TargetPos;
	if(m_DisengageAfterMove) {
		QTimer::singleShot(m_DelayToDisEngageAfterMove * 1000, this, SLOT(EventTimerDisengage()));
		TimerRunning++;
	}
}

void GServoMotor::EventTimerDisengage()
{
// 	// if the target position is still the one that fires the timer, then we can disengage. Otherwise it might mean that it is still moving.
// 	if(justMovedTo == m_TargetPos)
// 		Disengage();
	TimerRunning--;
	if(TimerRunning == 0)
		Disengage();
}