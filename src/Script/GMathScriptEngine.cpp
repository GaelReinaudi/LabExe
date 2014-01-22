#include "GMathScriptEngine.h"
#include "ScriptFunctions.h"
#include "Param/GParamManager.h"

QScriptValue functionParamNumValueFromCallee(QScriptContext *ctx, QScriptEngine *eng)
{
    Q_UNUSED(eng);
    QScriptValue theParamPointerData = ctx->callee().data();
	QObject* pObj = theParamPointerData.toQObject();
	GParamNum* pParNum = qobject_cast<GParamNum*>(pObj);
	if(!pParNum)
		return QScriptValue();

	QScriptValue result;
	// for a setter function, there is one argument
	if(ctx->argumentCount() == 1) {
		double valSet = ctx->argument(0).toNumber();
		pParNum->SetParamValue(valSet);
	} 
	result = pParNum->DoubleValue();
	return result;
}

QScriptValue functionParamValueByID( QScriptContext *ctx, QScriptEngine *eng )
{
    Q_UNUSED(eng);
    if(ctx->argumentCount() != 1)
		return QScriptValue();
	QString parID = ctx->argument(0).toString();
	GParamNum* pParNum = qobject_cast<GParamNum*>(ParamManagerInstance()->GetParam(parID));
	if(!pParNum)
		return QScriptValue();
	QScriptValue result;
	result = pParNum->DoubleValue();
	return result;
}

GMathScriptEngine::GMathScriptEngine(QObject *parent /*= 0*/)
	: QScriptEngine(parent)
{
// 	// Destroy all of the old variables and their values, including the built-in from the ECMA-262 standard (see Qt Assistant).
// 	QScriptValueIterator qsvIter(globalObject());
// 	while (qsvIter.hasNext()) {
// 		qsvIter.next();
// 		qsvIter.remove();
// 	}

	// Add the built-in functions that are expected. 
	globalObject().setProperty( "exp", newFunction(&functionExp, 1) );
	globalObject().setProperty( "pow", newFunction(&functionPow, 2) );
	globalObject().setProperty( "log", newFunction(&functionLog, 1) );
	globalObject().setProperty( "ln", newFunction(&functionLn, 1) );
	globalObject().setProperty( "cos", newFunction(&functionCos, 1) );
	globalObject().setProperty( "sin", newFunction(&functionSin, 1) );
	globalObject().setProperty( "tan", newFunction(&functionTan, 1) );
	globalObject().setProperty( "arccos", newFunction(&functionArccos, 1) );
	globalObject().setProperty( "arcsin", newFunction(&functionArcsin, 1) );
	globalObject().setProperty( "arctan", newFunction(&functionArctan, 1) );
	globalObject().setProperty( "sqrt", newFunction(&functionSqrt, 1) );
	globalObject().setProperty( "abs", newFunction(&functionAbs, 1) );
	globalObject().setProperty( "H", newFunction(&functionH, 1) );

	globalObject().setProperty( "Param", newFunction(&functionParamValueByID, 1) );
}

GMathScriptEngine::~GMathScriptEngine()
{

}

void GMathScriptEngine::RegisterVariable( GParamString* pName, GParamNum* pParamNum )
{
	QString name = pName->StringValue();
	double val = pParamNum->DoubleValue();
	if(name != "")
		globalObject().setProperty(name, newVariant(val));

	m_ParamName_ParamNum.insert(pName, pParamNum);
	UpdateAllVariableNames();

	// when renaming, we update the variable names
	connect(pName, SIGNAL(ValueUpdated(QString)), this, SLOT(UpdateAllVariableNames()));
}

void GMathScriptEngine::RemoveVariable( GParamString* pName, GParamNum* pParamNum )
{
	m_ParamName_ParamNum.remove(pName, pParamNum);
	UpdateAllVariableNames();
	disconnect(pName, SIGNAL(ValueUpdated(QString)), this, SLOT(UpdateAllVariableNames()));
}

void GMathScriptEngine::UpdateAllVariableNames()
{
	// removes all the variables first
	foreach(QString nameVar, m_UsedVariableNames) {
		globalObject().setProperty(nameVar, QScriptValue());
	}
	// clears the list of used names for variables
	m_UsedVariableNames.clear();

	// then re-add the variables
	foreach(GParamString* pStr, m_ParamName_ParamNum.keys()) {
		GParamNum* pNum = m_ParamName_ParamNum.value(pStr);
		QString name = pStr->StringValue();
		if(!pNum || name == "")
			continue;
		// sets the function that can read the value of the param
		QScriptValue fun = newFunction(functionParamNumValueFromCallee);
		// sets the data to the pointer of the param. It is used in the function to retrieve which param to read the value from.
		QScriptValue valPointParam = newQObject(pNum);
		fun.setData(valPointParam);
		globalObject().setProperty(name, fun, QScriptValue::PropertyGetter | QScriptValue::PropertySetter);
		m_UsedVariableNames.append(name);
	}
}

void GMathScriptEngine::SetProgram( QString newProg )
{
	m_EquationProgram = newProg;
}

double GMathScriptEngine::Evaluate()
{
	if(m_EquationProgram.isNull())
		return QScriptValue().toNumber();
	QScriptValue scriptResult = evaluate(m_EquationProgram);
	// default value of the result:
	double theResult = 0.0;
	if(!scriptResult.isError())
		theResult = scriptResult.toNumber();
	emit Evaluated(theResult);
	return theResult;
}















