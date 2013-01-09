#ifndef GMATHSCRIPTENGINE_H
#define GMATHSCRIPTENGINE_H
#include "labexe_global.h"
#include "param.h"

#include <QScriptEngine>
#include <QtScriptTools/QScriptEngineDebugger>
#include <QScriptValue>
#include <QScriptValueIterator>

//! Gets the value of the parameter by using the data() of the callee().
QScriptValue functionParamNumValueFromCallee(QScriptContext *ctx, QScriptEngine *eng);
//! Gets the value of a numerical parameter by using its unique ID.
QScriptValue functionParamValueByID(QScriptContext *ctx, QScriptEngine *eng);


/////////////////////////////////////////////////////////////////////
//! \brief The GMathScriptEngine class implements a QScriptEngine that will interact nicely with GParam objects.
/*!
Making a GParam object available to the script engine is made easy by convenience functions. 
The script can then be called like a Functor with arguments and query the values of the parameters it is using.
*/
class LABEXE_EXPORT GMathScriptEngine : public QScriptEngine
{
	Q_OBJECT

public:
	GMathScriptEngine(QObject *parent = 0);
	~GMathScriptEngine();

public:
	//! Registers the pParamNum as being a variable of the script with the name pName and will adapt to changes in this name by hooking into the GParamString signals.
	void RegisterVariable(GParamString* pName, GParamNum* pParamNum);
	//! Removes the pParamNum as being a variable of the script with the name pName.
	void RemoveVariable(GParamString* pName, GParamNum* pParamNum);

public slots:
	//! Sets the program that is evaluated by the engine
	void SetProgram(QString newProg);
	//! Evaluate the program and return the value. 0.0 if isError is returned by the evaluation.
	double Evaluate();

signals:
	//! emitted when the script was evaluated
	void Evaluated(double newResult);

private slots:
	//! Removes all the 
	void UpdateAllVariableNames();

private:
	QMultiMap<GParamString*, GParamNum*> m_ParamName_ParamNum;
	QList<QString> m_UsedVariableNames;
	QScriptProgram m_EquationProgram;
};

#endif // GMATHSCRIPTENGINE_H
