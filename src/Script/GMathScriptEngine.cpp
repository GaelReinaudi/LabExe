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
    // Create ScriptFunctions instance
    ScriptFunctions *scriptFuncs = new ScriptFunctions(this);
    
    // Expose ScriptFunctions object
    QJSValue scriptFunctionsObj = newQObject(scriptFuncs);
    globalObject().setProperty("ScriptFunctions", scriptFunctionsObj);
    
    // Create global function wrappers for math functions
    // This creates the functions in the JavaScript environment
    evaluate("function exp(x) { return ScriptFunctions.exp(x); }");
    evaluate("function pow(x, y) { return ScriptFunctions.pow(x, y); }");
    evaluate("function log(x) { return ScriptFunctions.log(x); }");
    evaluate("function ln(x) { return ScriptFunctions.ln(x); }");
    evaluate("function cos(x) { return ScriptFunctions.cos(x); }");
    evaluate("function sin(x) { return ScriptFunctions.sin(x); }");
    evaluate("function tan(x) { return ScriptFunctions.tan(x); }");
    evaluate("function arccos(x) { return ScriptFunctions.arccos(x); }");
    evaluate("function arcsin(x) { return ScriptFunctions.arcsin(x); }");
    evaluate("function arctan(x) { return ScriptFunctions.arctan(x); }");
    evaluate("function sqrt(x) { return ScriptFunctions.sqrt(x); }");
    evaluate("function abs(x) { return ScriptFunctions.abs(x); }");
    evaluate("function H(x) { return ScriptFunctions.H(x); }");
    
    // Expose paramValueByID as a global function as well
    evaluate("function paramValueByID(id) { return ScriptFunctions.paramValueByID(id); }");
}

GMathScriptEngine::~GMathScriptEngine()
{
}

void GMathScriptEngine::RegisterVariable(GParamString* pName, GParamNum* pParamNum)
{
    QString name = pName->StringValue();
    if (!name.isEmpty()) {
        // We no longer need to expose the parameter via a ParamWrapper
        // Instead, we'll set the actual value directly in UpdateAllVariableNames
    }
    m_ParamName_ParamNum.insert(pName, pParamNum);
    UpdateAllVariableNames();

    // When the parameter name changes, update the global object.
    connect(pName, SIGNAL(ValueUpdated(QString)), this, SLOT(UpdateAllVariableNames()));
    
    // Also update when the parameter value changes
    connect(pParamNum, SIGNAL(ValueUpdated(double)), this, SLOT(UpdateAllVariableNames()));
}

void GMathScriptEngine::RemoveVariable(GParamString* pName, GParamNum* pParamNum)
{
    m_ParamName_ParamNum.remove(pName);
    UpdateAllVariableNames();
    
    // Disconnect both the name and value update signals
    disconnect(pName, SIGNAL(ValueUpdated(QString)), this, SLOT(UpdateAllVariableNames()));
    disconnect(pParamNum, SIGNAL(ValueUpdated(double)), this, SLOT(UpdateAllVariableNames()));
}

void GMathScriptEngine::UpdateAllVariableNames()
{
    // Remove all currently registered variable names from the global object.
    for (const QString &nameVar : std::as_const(m_UsedVariableNames)) {
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
            
        // Create a direct value property for the variable
        // This allows using "a" directly in expressions instead of "a.get()"
        globalObject().setProperty(name, pNum->DoubleValue());
        
        // Store the variable name for later cleanup
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
