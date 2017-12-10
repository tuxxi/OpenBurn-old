#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QDebug>


#include "export.h"

EngExport::EngExport()
    : EngExport(nullptr)
{
}

EngExport::EngExport(MotorSim* sim)
    : m_Simulator(sim), 
    m_totalMass(0), m_propMass(0), m_MotorLen(0), m_MotorDia(0)

{
}

EngExport::EngExport(const EngExport& other)
    : EngExport(other.m_Simulator)
{
}

void EngExport::SetMotorDetails(const double caseMass, const int len, const int dia, 
    const QString& mfgCode, const QString& mfgName)
{
    m_MotorLen = len;
    m_MotorDia = dia;
    m_mfgCode = mfgCode;
    m_mfgName = mfgName;

    m_propMass = OpenBurnUnits::MassUnits::Convert(
        OpenBurnUnits::MassUnits_T::pounds_mass,
        OpenBurnUnits::MassUnits_T::kilograms,
         m_Simulator->GetDesignMotor()->GetMotorPropellantMass());
    m_totalMass = m_propMass + caseMass;
}

void EngExport::WriteToEngFile(const QString& filename) const
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(nullptr, tr("Error"), tr("Could not save file"));
    }
    else
    {
        QTextStream stream(&file);

        stream << "; " << m_Simulator->GetMotorDesignation() << '\n';
        stream << "; Exported with OpenBurn!\n";

        //--eng header: code, dia in mm, len in mm
        stream << m_mfgCode << " " << m_MotorDia << " " << m_MotorLen;
        //'P' because EX motors don't usually have delays. maybe change?
        //prop mass in kn, total mass in kg, mfg name.
        stream << " P " << m_propMass << " " << m_totalMass << " " << m_mfgName << "\n";

        int counter = 0;
        const int divisor = int(m_Simulator->GetNumPoints()) / 30;
        for (auto iter = m_Simulator->GetResultsBegin(); iter != m_Simulator->GetResultsEnd(); ++iter, ++counter)
        {
            const double thrustN = OpenBurnUnits::ForceUnits::Convert(
                OpenBurnUnits::ForceUnits_T::pounds_force,
                OpenBurnUnits::ForceUnits_T::newtons,
                iter->get()->thrust);

            if (counter == 0 || counter % divisor == 0)
            {
                //3 spaces + TIME + 1 space + THRUST (N)
                stream << "   " << iter->get()->time << " " << thrustN << '\n';
            }
        }
        file.close();
    }

}
