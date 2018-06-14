//
// Created by tuxxi on 3/28/18.
//

#include "application.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QMessageBox>

OpenBurnApplication::OpenBurnApplication()
    : m_ApplicationVersion("0.0.1")
{
    m_UndoStack = new QUndoStack(this);
    m_DesignMotor = std::make_unique<OpenBurnMotor>();
    m_Simulator = std::make_unique<MotorSim>(m_DesignMotor.get());
    m_GlobalSettings = std::make_unique<OpenBurnSettings>();
    LoadSettings("user/settings.json");
    LoadDatabase("user/propellants.json");
}

void OpenBurnApplication::ResetCurrentDesign()
{
    //delete everything and re-create
    m_DesignMotor = std::make_unique<OpenBurnMotor>();
    m_Simulator = std::make_unique<MotorSim>(m_DesignMotor.get());
}
bool OpenBurnApplication::LoadSettings(const QString& filename)
{
    m_SettingsFileName = filename;
    QFile file(m_SettingsFileName);
    if (file.open(QIODevice::ReadOnly))
    {
        if (file.atEnd()) //is file empty?
            return false;
        QByteArray data = file.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(data));
        QJsonObject settingsObject = loadDoc.object();
        m_GlobalSettings->ReadJSON(settingsObject);
        return true;
    }
    return false;
}
bool OpenBurnApplication::SaveSettings()
{
    QFile file(m_SettingsFileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QJsonObject settingsObject;
        m_GlobalSettings->WriteJSON(settingsObject);
        QJsonDocument saveDoc(settingsObject);
        file.write(saveDoc.toJson());
        return true;
    }
    return false;
}
bool OpenBurnApplication::SaveFile(QFile &file)
{
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QString filename = file.fileName();
    if (!filename.isEmpty())
    {
        if (!filename.contains(".obm"))
        {
            file.setFileName(filename + ".obm");
        }
        m_CurrentDesignFilename = filename;

        QJsonObject motorObject;
        m_DesignMotor->WriteJSON(motorObject);
        QJsonDocument saveDoc(motorObject);
        file.write(saveDoc.toJson());
        file.close();
        emit FileSaved(filename);
        m_UndoStack->setClean(); //set undo stack to clean slate, no more undoing when file is saved
        return true;
    }
    return false;
}
bool OpenBurnApplication::SaveCurrentFile()
{
    QFile file(m_CurrentDesignFilename);
    return SaveFile(file);
}

bool OpenBurnApplication::OpenFile(QFile& file)
{
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QString filename = file.fileName();

    if (!filename.isEmpty())
    {
        QFile file(filename);
        QByteArray data = file.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(data));
        QJsonObject motor = loadDoc.object();
        m_DesignMotor->ReadJSON(motor);
        m_CurrentDesignFilename = filename;
        file.close();
        emit FileOpened(filename);
        return true;
    }
    return false;
}

void OpenBurnApplication::OnNewPropellantFound(OpenBurnPropellant prop)
{
    const auto brCoef = QString::number(m_GlobalSettings->m_BurnRateUnits.ConvertFrom(
        OpenBurnUnits::BurnRateUnits_T::inches_per_second,
        prop.GetBurnRateCoef()));
    const auto cStar = QString::number(m_GlobalSettings->m_VelocityUnits.ConvertFrom(
        OpenBurnUnits::VelocityUnits_T::feet_per_second,
        prop.GetCharVelocity()));
    const auto rho = QString::number(m_GlobalSettings->m_DensityUnits.ConvertFrom(
        OpenBurnUnits::DensityUnits_T::lbs_per_in_cu,
        prop.GetDensity()));

    const auto brUnits = m_GlobalSettings->m_BurnRateUnits.GetUnitSymbol();
    const auto cstarUnits = m_GlobalSettings->m_VelocityUnits.GetUnitSymbol();
    const auto rhoUnits = m_GlobalSettings->m_DensityUnits.GetUnitSymbol();

    const QString messageBoxQuestion = tr("Propellant ") + prop.GetPropellantName() + tr(" not found in database!\n\n") +
        tr("BR Coef (a): ") + brCoef + " " + brUnits + "\n" +
        tr("Br Exp (n): ") + QString::number(prop.GetBurnRateExp()) + "\n" +
        tr("Characteristic Velocity (C*): ") + cStar + " " + cstarUnits + "\n" +
        tr("Density (rho): ") + rho + " " + rhoUnits + "\n" +
        tr("Specific Heat Ratio (gamma, Cp/Cv): ") + QString::number(prop.GetSpecificHeatRatio()) + "\n\n" +
        tr("Would you like to add it to the database?\n");
    QMessageBox::StandardButton resBtn =
        QMessageBox::question(nullptr, "New Propellant Found", messageBoxQuestion,
                               QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes)
    {
        AddNewPropellant(prop);
    }
}
void OpenBurnApplication::OnDuplicatePropellantFound(const OpenBurnPropellant& dupe, const OpenBurnPropellant &prop)
{
    const auto brUnits = m_GlobalSettings->m_BurnRateUnits.GetUnitSymbol();
    const auto cstarUnits = m_GlobalSettings->m_VelocityUnits.GetUnitSymbol();
    const auto rhoUnits = m_GlobalSettings->m_DensityUnits.GetUnitSymbol();
    QString brCoef, brExp, cStar, rho, gmma;
    if (dupe.GetBurnRateCoef() != prop.GetBurnRateCoef())
    {
        const auto a = QString::number(m_GlobalSettings->m_BurnRateUnits.ConvertFrom(
            OpenBurnUnits::BurnRateUnits_T::inches_per_second,
            prop.GetBurnRateCoef()));
        const auto a_dupe = QString::number(m_GlobalSettings->m_BurnRateUnits.ConvertFrom(
            OpenBurnUnits::BurnRateUnits_T::inches_per_second,
            dupe.GetBurnRateCoef()));
        brCoef = tr("br Coef (a) ") + a_dupe + "\t" +  a + " " + brUnits + "\n";
    }
    if (dupe.GetBurnRateExp() != prop.GetBurnRateExp())
    {
        const auto n = QString::number(prop.GetBurnRateExp());
        const auto n_dupe = QString::number(dupe.GetBurnRateExp());
        brExp = tr("br Exp (n) ") + n_dupe + "\t" +  n + "\n";
    }
    if (dupe.GetCharVelocity() != prop.GetCharVelocity())
    {
        const auto cs = QString::number(m_GlobalSettings->m_VelocityUnits.ConvertFrom(
            OpenBurnUnits::VelocityUnits_T::feet_per_second,
            prop.GetCharVelocity()));
        const auto cs_dupe = QString::number(m_GlobalSettings->m_VelocityUnits.ConvertFrom(
            OpenBurnUnits::VelocityUnits_T::feet_per_second,
            dupe.GetCharVelocity()));
        cStar = tr("Characteristic Velocity (C*) ") + cs_dupe + "\t" +  cs + " " + cstarUnits + "\n";
    }
    if (dupe.GetDensity() != prop.GetDensity())
    {
        const auto r = QString::number(m_GlobalSettings->m_DensityUnits.ConvertFrom(
            OpenBurnUnits::DensityUnits_T::lbs_per_in_cu,
            prop.GetDensity()));
        const auto r_dupe = QString::number(m_GlobalSettings->m_DensityUnits.ConvertFrom(
            OpenBurnUnits::DensityUnits_T::lbs_per_in_cu,
            dupe.GetDensity()));
        rho = tr("Density (rho) ") + r_dupe + "\t" +  r + " " + rhoUnits + "\n";
    }
    if (dupe.GetSpecificHeatRatio() != prop.GetSpecificHeatRatio())
    {
        const auto y = QString::number(prop.GetSpecificHeatRatio());
        const auto y_dupe =  QString::number(dupe.GetSpecificHeatRatio());
        gmma = tr("Specific Heat Ratio (cp/cv, gamma) ") + y_dupe + "\t" +  y + "\n";
    }
    const QString messageBoxQuestion = tr("Propellant ") + dupe.GetPropellantName() + tr(" different from database!\n\n") +
        tr("Loaded from file: ") + "\t\t" + tr("Database: ") + "\n" +
        brCoef +
        brExp +
        cStar +
        rho +
        gmma +
        tr("\nWould you like to update the database?\n");

    QMessageBox::StandardButton resBtn =
        QMessageBox::question(nullptr, tr("Duplicate Propellant Found"), messageBoxQuestion,
                               QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes)
    {
        auto oldProp = std::find(m_Propellants.begin(), m_Propellants.end(), prop);
        if (oldProp != m_Propellants.end())
        {
            m_Propellants.erase(oldProp);
            m_Propellants.push_back(dupe);
            SaveDatabase();
        }
    }
}
bool OpenBurnApplication::SaveDatabase()
{
    QFile file(m_DatabaseFileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QJsonObject propellantObject;
        QJsonArray propellantArray;
        for (auto prop : m_Propellants)
        {
            QJsonObject obj;
            prop.WriteJSON(obj);
            propellantArray.append(obj);
        }
        propellantObject["propellants"] = propellantArray;
        QJsonDocument saveDoc(propellantObject);
        file.write(saveDoc.toJson());
        return true;
    }
    return false;
}
bool OpenBurnApplication::AddNewPropellant(const OpenBurnPropellant& prop)
{
    m_Propellants.push_back(prop);

    //set all the grains to a new propellant type if the propellant database happens to change
    for (auto prop : m_Propellants)
    {
        for (auto grain : m_DesignMotor->GetGrains())
        {
            if (grain->GetPropellantType().GetPropellantName() == prop.GetPropellantName())
            {
                grain->SetPropellantType(prop);
            }
        }
    }

    if (SaveDatabase())
    {
        emit PropellantDatabaseUpdated(prop);
        return true;
    }
    return false;
}
bool OpenBurnApplication::CurrentDesignSaved() const
{
    return m_UndoStack->isClean() && !m_CurrentDesignFilename.isEmpty();
}

bool OpenBurnApplication::LoadDatabase(const QString& filename)
{
    m_DatabaseFileName = filename;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(data));
        m_Propellants.clear(); //make sure the database is clear before we add new propellants
        QJsonArray propellantArray = loadDoc.object()["propellants"].toArray();
        for (auto obj : propellantArray)
        {
            OpenBurnPropellant prop;
            prop.ReadJSON(obj.toObject());
            m_Propellants.push_back(prop);
        }
        file.close();
        return true;
    }
    return false;
}
