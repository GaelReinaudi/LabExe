#ifndef GMATHSCRIPTENGINE_H
#define GMATHSCRIPTENGINE_H
#include "labexe_global.h"
#include "param.h"
#include <QJSEngine>
#include <QObject>
#include <QString>
#include <QHash>
#include <QStringList>

// Forward declarations of your parameter classes.
class GParamNum;
class GParamString;

class GMathScriptEngine : public QJSEngine
{
    Q_OBJECT
public:
    explicit GMathScriptEngine(QObject *parent = nullptr);
    ~GMathScriptEngine();

    // Registers a variable by name, exposing it to the script as an object with get() and set() methods.
    void RegisterVariable(GParamString* pName, GParamNum* pParamNum);
    void RemoveVariable(GParamString* pName, GParamNum* pParamNum);
    void UpdateAllVariableNames();

    // Sets the program (script source) to be evaluated.
    void SetProgram(QString newProg);

    // Evaluates the script program and emits Evaluated(result)
    double Evaluate();

signals:
    void Evaluated(double result);

private:
    QString m_EquationProgram;
    QHash<GParamString*, GParamNum*> m_ParamName_ParamNum;
    QStringList m_UsedVariableNames;
};

#endif // GMATHSCRIPTENGINE_H
