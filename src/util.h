#pragma once

#include <QStringList>
#include <QString>
#include <QObject>

namespace OpenBurnUtil
{
    char GetMotorClass(double nsec);
    double GetMotorClassPercent(double nsec);
    static const double g_kGasConstantR = 8.3144621; //J / mol * k
    static const double g_kGravityConstantG = 32.1740; //feet/sec/sec
    static const int g_kNumInterations = 200;
}


