#include "GMathScriptEngine.h"
#include "Param/GParamManager.h"  // Assumes ParamManagerInstance() and related classes are defined here.
#include <QJSValue>
#include <QDebug>
#include <cmath>

//--------------------------------------------------------------------
// Helper class to expose math functions and parameter lookup to scripts
//--------------------------------------------------------------------
class ScriptFunctions : public QObject {
    Q_OBJECT
public:
    explicit ScriptFunctions(QObject* parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE double exp(double x) { return std::exp(x); }
    Q_INVOKABLE double pow(double x, double y) { return std::pow(x, y); }
    Q_INVOKABLE double log(double x) { return std::log10(x); } // logarithm base 10
    Q_INVOKABLE double ln(double x) { return std::log(x); }
    Q_INVOKABLE double cos(double x) { return std::cos(x); }
    Q_INVOKABLE double sin(double x) { return std::sin(x); }
    Q_INVOKABLE double tan(double x) { return std::tan(x); }
    Q_INVOKABLE double arccos(double x) { return std::acos(x); }
    Q_INVOKABLE double arcsin(double x) { return std::asin(x); }
    Q_INVOKABLE double arctan(double x) { return std::atan(x); }
    Q_INVOKABLE double sqrt(double x) { return std::sqrt(x); }
    Q_INVOKABLE double abs(double x) { return std::fabs(x); }
    // H is a placeholder; implement as needed.
    Q_INVOKABLE double H(double x) { return x; }

    // Looks up a parameter value by its ID.
    Q_INVOKABLE double paramValueByID(const QString &id) {
        GParamNum* pParNum = qobject_cast<GParamNum*>(ParamManagerInstance()->GetParam(id));
        return pParNum ? pParNum->DoubleValue() : 0.0;
    }
};

//--------------------------------------------------------------------
// Helper class to wrap a GParamNum for script access via get() and set()
//--------------------------------------------------------------------
class ParamWrapper : public QObject {
    Q_OBJECT
public:
    ParamWrapper(GParamNum* param, QObject* parent = nullptr)
        : QObject(parent), m_param(param) {}

    Q_INVOKABLE double get() const {
        return m_param->DoubleValue();
    }
    Q_INVOKABLE void set(double value) {
        m_param->SetParamValue(value);
    }
private:
    GParamNum* m_param;
};

//--------------------------------------------------------------------
// GMathScriptEngine Implementation
//--------------------------------------------------------------------
GMathScriptEngine::GMathScriptEngine(QObject *parent)
    : QJSEngine(parent)
{
    // Expose the math and parameter functions under a global object "ScriptFunctions"
    ScriptFunctions *scriptFuncs = new ScriptFunctions(this);
    globalObject().setProperty("ScriptFunctions", newQObject(scriptFuncs));

    // (Optional) You could expose paramValueByID directly as "Param", e.g.,
    // globalObject().setProperty("Param", newQObject(scriptFuncs));
}

GMathScriptEngine::~GMathScriptEngine()
{
}

void GMathScriptEngine::RegisterVariable(GParamString* pName, GParamNum* pParamNum)
{
    QString name = pName->StringValue();
    if (!name.isEmpty()) {
        // Expose the parameter via a ParamWrapper. In your scripts, access it as:
        //    x.get()  and  x.set(newValue)
        globalObject().setProperty(name, newQObject(new ParamWrapper(pParamNum, this)));
    }
    m_ParamName_ParamNum.insert(pName, pParamNum);
    UpdateAllVariableNames();

    // When the parameter name changes, update the global object.
    connect(pName, SIGNAL(ValueUpdated(QString)), this, SLOT(UpdateAllVariableNames()));
}

void GMathScriptEngine::RemoveVariable(GParamString* pName, GParamNum* pParamNum)
{
    m_ParamName_ParamNum.remove(pName);
    UpdateAllVariableNames();
    disconnect(pName, SIGNAL(ValueUpdated(QString)), this, SLOT(UpdateAllVariableNames()));
}

void GMathScriptEngine::UpdateAllVariableNames()
{
    // Remove all currently registered variable names from the global object.
    for (const QString &nameVar : m_UsedVariableNames) {
        globalObject().setProperty(nameVar, QJSValue());
    }
    m_UsedVariableNames.clear();

    // Re-add the variables from our internal mapping.
    for (auto it = m_ParamName_ParamNum.constBegin(); it != m_ParamName_ParamNum.constEnd(); ++it) {
        GParamString* pStr = it.key();
        GParamNum* pNum = it.value();
        QString name = pStr->StringValue();
        if (!pNum || name.isEmpty())
            continue;
        globalObject().setProperty(name, newQObject(new ParamWrapper(pNum, this)));
        m_UsedVariableNames.append(name);
    }
}

void GMathScriptEngine::SetProgram(QString newProg)
{
    m_EquationProgram = newProg;
}

double GMathScriptEngine::Evaluate()
{
    if(m_EquationProgram.isEmpty())
        return 0.0;
    QJSValue scriptResult = evaluate(m_EquationProgram);
    double theResult = 0.0;
    if (!scriptResult.isError())
        theResult = scriptResult.toNumber();
    else
        qWarning() << "Script evaluation error:" << scriptResult.toString();

    emit Evaluated(theResult);
    return theResult;
}

#include "GMathScriptEngine.moc"
