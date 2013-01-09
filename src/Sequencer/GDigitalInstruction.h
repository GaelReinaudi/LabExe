#ifndef GDIGITALINSTRUCTION_H
#define GDIGITALINSTRUCTION_H

#include "GInstruction.h"

/////////////////////////////////////////////////////////////////////
//! \brief A GDigitalInstruction is an important kind of GInstruction that defines for each time a boolean value: true of false.
/*!

*/
class GDigitalInstruction : public GInstruction
{
	Q_OBJECT
	Q_PROPERTY(bool m_Value READ GetValue WRITE SetValue NOTIFY ValueChanged)

public:
	GDigitalInstruction(GSynchEvent* parentEventSynchro, GChannel* pAssignedChannel = 0);
	~GDigitalInstruction();

	void SetValue(bool newVal) {m_Value = newVal; emit ValueChanged(m_Value);}
	bool GetValue() const {return m_Value;}

signals:
	void ValueChanged(bool newVal);

private:
	bool m_Value;
};

#endif // GDIGITALINSTRUCTION_H
