#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>


#include "export.h"

EngExport::EngExport(MotorSim* sim)
    : m_Simulator(sim)
    
{
    
}

void EngExport::SetMotorDetails(const double caseMass, const QString& mfgCode, const QString& mfgName)
{
    m_totalMass = OpenBurnUnits::MassUnits::Convert(
        OpenBurnUnits::MassUnits_T::pounds_mass,
        OpenBurnUnits::MassUnits_T::kilograms,
        caseMass + m_Simulator->GetDesignMotor()->GetMotorPropellantMass());
    m_mfgCode = mfgCode;
    m_mfgName = mfgName;
}

void EngExport::WriteToEngFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(nullptr, tr("Error"), tr("Could not save file"));
    }
    else
    {
        QTextStream stream(&file);
        const int diaMM = int(OpenBurnUnits::LengthUnits::Convert(
            OpenBurnUnits::LengthUnits_T::inches,
            OpenBurnUnits::LengthUnits_T::millimeters,
            round(m_Simulator->GetDesignMotor()->GetMotorMajorDiameter())));
        const int lenMM = int(OpenBurnUnits::LengthUnits::Convert(
            OpenBurnUnits::LengthUnits_T::inches,
            OpenBurnUnits::LengthUnits_T::millimeters,
            round(m_Simulator->GetDesignMotor()->GetMotorLength())));
        const double propMass  = OpenBurnUnits::MassUnits::Convert(
            OpenBurnUnits::MassUnits_T::pounds_mass,
            OpenBurnUnits::MassUnits_T::kilograms,
            m_Simulator->GetDesignMotor()->GetMotorPropellantMass());

        stream << "; " << m_Simulator->GetMotorDesignation() << '\n';
        stream << "; Exported with OpenBurn!\n";
        stream << m_mfgCode << " " << diaMM << " " << lenMM << " P " << propMass << " " << m_totalMass << " " << m_mfgCode << "\n";
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
                //3 spaces + TIME + space + thrust in N
                stream << "   " << iter->get()->time << " " << thrustN << '\n';
            }
        }
        file.close();
    }

}
