#ifndef GINSTRUCTIONGROUP_H
#define GINSTRUCTIONGROUP_H

#include <QObject>
#include <QVector>
class G_old_Instruction;
typedef QVector<G_old_Instruction*> GListOfInstructions;

/////////////////////////////////////////////////////////////////////
//! \brief A GInstructionGroup defines a G_old_Instruction which is actually a list of G_old_Instruction's (GListOfInstructions).
/*!
You can 
*/
class GInstructionGroup : public GListOfInstructions, public QObject
{
public:
	GInstructionGroup(QObject *parent);
	~GInstructionGroup();

public:

private:
	
};

#endif // GINSTRUCTIONGROUP_H
