#include "ScriptFunctions.h"

QJSValue functionExp(QScriptContext *ctx, QJSEngine *eng)
{
    Q_UNUSED(eng);
    return exp(ctx->argument(0).toNumber());
}

QJSValue functionPow(QScriptContext *ctx, QJSEngine *eng)
{
    Q_UNUSED(eng);
    return pow(ctx->argument(0).toNumber(), (double)ctx->argument(1).toNumber());
}

QJSValue functionCos(QScriptContext *ctx, QJSEngine *eng)
{
    Q_UNUSED(eng);
    return cos(ctx->argument(0).toNumber());
}

QJSValue functionSin(QScriptContext *ctx, QJSEngine *eng)
{
    Q_UNUSED(eng);
    return sin(ctx->argument(0).toNumber());
}

QJSValue functionTan(QScriptContext *ctx, QJSEngine *eng)
{
    Q_UNUSED(eng);
    return tan(ctx->argument(0).toNumber());
}

QJSValue functionLog( QScriptContext *ctx, QJSEngine *eng )
{
    Q_UNUSED(eng);
    return log(ctx->argument(0).toNumber())/log(double(10));
}

QJSValue functionLn( QScriptContext *ctx, QJSEngine *eng )
{
    Q_UNUSED(eng);
    return log(ctx->argument(0).toNumber());
}

QJSValue functionArccos( QScriptContext *ctx, QJSEngine *eng )
{
    Q_UNUSED(eng);
    return acos(ctx->argument(0).toNumber());
}

QJSValue functionArcsin( QScriptContext *ctx, QJSEngine *eng )
{
    Q_UNUSED(eng);
    return asin(ctx->argument(0).toNumber());
}

QJSValue functionArctan( QScriptContext *ctx, QJSEngine *eng )
{
    Q_UNUSED(eng);
    return atan(ctx->argument(0).toNumber());
}

QJSValue functionSqrt( QScriptContext *ctx, QJSEngine *eng )
{
    Q_UNUSED(eng);
    return pow(ctx->argument(0).toNumber(), 0.5);
}

QJSValue functionAbs( QScriptContext *ctx, QJSEngine *eng )
{
    Q_UNUSED(eng);
    return qAbs(ctx->argument(0).toNumber());
}

QJSValue functionH( QScriptContext *ctx, QJSEngine *eng )
{
    Q_UNUSED(eng);
    return ctx->argument(0).toNumber() >= 0 ? 1.0 : 0.0;
}
