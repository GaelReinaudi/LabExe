#ifndef GCURVEINSTRUCTION_H
#define GCURVEINSTRUCTION_H

#include "G_old_Instruction.h"
#include "device.h"
#include "Curve/GCurve.h"

/////////////////////////////////////////////////////////////////////
//! \brief A GCurveInstruction is an important kind of G_old_Instruction that defines a signal (analog or digital), represented by a GCurve.
/*!
This instruction will be used by a GSignalDevice.
*/
class GCurveInstruction : public G_old_Instruction
{
public:
	GCurveInstruction(G_old_TimeEvent* parent);
	~GCurveInstruction();

	double Duration() const;
	void setCurve(GCurve* pCurve);
	//! Sets the GDevice that is associated with the instruction
	void SetSignalDevice(/*Signal*/GDevice* pDevice);
	//! Gets the GDevice (base class of the GSignalDevice) associated with the instruction
	GDevice* Device() const {return m_pDevice;}
	//! Gets the GSignalDevice associated with the instruction
	virtual /*Signal*/GDevice* SignalDevice() const {return m_pDevice;}

protected:
//	//! The relevant QrectF that defines the display bounding rect of a QGraphicsScene in which we are drawing the instruction. 
//	virtual QRectF BoundingSceneRect() const;
//	//! Updates the graphics item accordingly to the wishes of the derived class. 
//	//! The default implementation is a rectangle given by BoundingSceneRect()
//	virtual void UpdateGraphicsItem();

private:
	/*GSignal*/GDevice* m_pDevice;
	GCurve* m_pCurve;
};

#endif // GCURVEINSTRUCTION_H
