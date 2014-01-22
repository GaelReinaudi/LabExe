#include "ScriptFunctions.h"

QScriptValue functionExp(QScriptContext *ctx, QScriptEngine *eng)
{
    Q_UNUSED(eng);
    return exp(ctx->argument(0).toNumber());
}

QScriptValue functionPow(QScriptContext *ctx, QScriptEngine *eng)
{
    Q_UNUSED(eng);
    return pow(ctx->argument(0).toNumber(), (double)ctx->argument(1).toNumber());
}

QScriptValue functionCos(QScriptContext *ctx, QScriptEngine *eng)
{
    Q_UNUSED(eng);
    return cos(ctx->argument(0).toNumber());
}

QScriptValue functionSin(QScriptContext *ctx, QScriptEngine *eng)
{
    Q_UNUSED(eng);
    return sin(ctx->argument(0).toNumber());
}

QScriptValue functionTan(QScriptContext *ctx, QScriptEngine *eng)
{
    Q_UNUSED(eng);
    return tan(ctx->argument(0).toNumber());
}

QScriptValue functionLog( QScriptContext *ctx, QScriptEngine *eng )
{
    Q_UNUSED(eng);
    return log(ctx->argument(0).toNumber())/log(double(10));
}

QScriptValue functionLn( QScriptContext *ctx, QScriptEngine *eng )
{
    Q_UNUSED(eng);
    return log(ctx->argument(0).toNumber());
}

QScriptValue functionArccos( QScriptContext *ctx, QScriptEngine *eng )
{
    Q_UNUSED(eng);
    return acos(ctx->argument(0).toNumber());
}

QScriptValue functionArcsin( QScriptContext *ctx, QScriptEngine *eng )
{
    Q_UNUSED(eng);
    return asin(ctx->argument(0).toNumber());
}

QScriptValue functionArctan( QScriptContext *ctx, QScriptEngine *eng )
{
    Q_UNUSED(eng);
    return atan(ctx->argument(0).toNumber());
}

QScriptValue functionSqrt( QScriptContext *ctx, QScriptEngine *eng )
{
    Q_UNUSED(eng);
    return pow(ctx->argument(0).toNumber(), 0.5);
}

QScriptValue functionAbs( QScriptContext *ctx, QScriptEngine *eng )
{
    Q_UNUSED(eng);
    return qAbs(ctx->argument(0).toNumber());
}

QScriptValue functionH( QScriptContext *ctx, QScriptEngine *eng )
{
    Q_UNUSED(eng);
    return ctx->argument(0).toNumber() >= 0 ? 1.0 : 0.0;
}
