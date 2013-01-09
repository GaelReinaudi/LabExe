#ifndef SCRIPTFUNCTIONS_H
#define SCRIPTFUNCTIONS_H

#include <QScriptContext>
#include <QScriptEngine>
#include <QScriptValue>
#include "math.h"

// exp(x)=e^x
QScriptValue functionExp(QScriptContext *ctx, QScriptEngine *eng);

// pow(a,b)=a^b
QScriptValue functionPow(QScriptContext *ctx, QScriptEngine *eng);

// log(a,b)=ln(a)/ln(b) [log base b of a]
QScriptValue functionLog(QScriptContext *ctx, QScriptEngine *eng);

// ln(a)=ln(a)
QScriptValue functionLn(QScriptContext *ctx, QScriptEngine *eng);

// cos(a)=cos(a)
QScriptValue functionCos(QScriptContext *ctx, QScriptEngine *eng);

// sin(a)=sin(a)
QScriptValue functionSin(QScriptContext *ctx, QScriptEngine *eng);

// tan(a)=tan(a)
QScriptValue functionTan(QScriptContext *ctx, QScriptEngine *eng);

// arccos(a)=arccos(a)
QScriptValue functionArccos(QScriptContext *ctx, QScriptEngine *eng);

// arcsin(a)=arcsin(a)
QScriptValue functionArcsin(QScriptContext *ctx, QScriptEngine *eng);

// arctan(a)=arctan(a)
QScriptValue functionArctan(QScriptContext *ctx, QScriptEngine *eng);

// sqrt(x)
QScriptValue functionSqrt(QScriptContext *ctx, QScriptEngine *eng);

// abs(x)
QScriptValue functionAbs(QScriptContext *ctx, QScriptEngine *eng);

// Heaviside(x)
QScriptValue functionH(QScriptContext *ctx, QScriptEngine *eng);




#endif // SCRIPTFUNCTIONS_H