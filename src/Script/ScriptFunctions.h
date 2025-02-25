#ifndef SCRIPTFUNCTIONS_H
#define SCRIPTFUNCTIONS_H

#include <QScriptContext>
#include <QJSEngine>
#include <QJSValue>
#include "math.h"

// exp(x)=e^x
QJSValue functionExp(QScriptContext *ctx, QJSEngine *eng);

// pow(a,b)=a^b
QJSValue functionPow(QScriptContext *ctx, QJSEngine *eng);

// log(a,b)=ln(a)/ln(b) [log base b of a]
QJSValue functionLog(QScriptContext *ctx, QJSEngine *eng);

// ln(a)=ln(a)
QJSValue functionLn(QScriptContext *ctx, QJSEngine *eng);

// cos(a)=cos(a)
QJSValue functionCos(QScriptContext *ctx, QJSEngine *eng);

// sin(a)=sin(a)
QJSValue functionSin(QScriptContext *ctx, QJSEngine *eng);

// tan(a)=tan(a)
QJSValue functionTan(QScriptContext *ctx, QJSEngine *eng);

// arccos(a)=arccos(a)
QJSValue functionArccos(QScriptContext *ctx, QJSEngine *eng);

// arcsin(a)=arcsin(a)
QJSValue functionArcsin(QScriptContext *ctx, QJSEngine *eng);

// arctan(a)=arctan(a)
QJSValue functionArctan(QScriptContext *ctx, QJSEngine *eng);

// sqrt(x)
QJSValue functionSqrt(QScriptContext *ctx, QJSEngine *eng);

// abs(x)
QJSValue functionAbs(QScriptContext *ctx, QJSEngine *eng);

// Heaviside(x)
QJSValue functionH(QScriptContext *ctx, QJSEngine *eng);




#endif // SCRIPTFUNCTIONS_H
