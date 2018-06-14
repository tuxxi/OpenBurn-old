//
// Created by tuxxi on 3/28/18.
//

#pragma once

#include <QObject>
#include <QFile>
#include <QUndoStack>

#include <memory>
#include "motorsim.hpp"
#include "settings.hpp"

class OpenBurnApplication : public QObject
{
    Q_OBJECT
public:
    OpenBurnApplication();

    void ResetCurrentDesign();

    //settings
    bool LoadSettings(const QString& filename);
    bool SaveSettings();
    bool AreSettingsLoaded() const { return !m_SettingsFileName.isEmpty();}

    //file
    bool SaveFile(QFile& file);
    bool SaveCurrentFile();
    bool OpenFile(QFile& file);
    //has the current design been saved, or are there any changes?
    bool CurrentDesignSaved() const;

    //Propellant Database
    bool LoadDatabase(const QString& filename);
    bool SaveDatabase();

    bool AddNewPropellant(const OpenBurnPropellant& prop);

    void OnNewPropellantFound(OpenBurnPropellant prop);
    void OnDuplicatePropellantFound(const OpenBurnPropellant& dupe, const OpenBurnPropellant& propInDb);

    OpenBurnMotor& GetDesignMotor() const { return *m_DesignMotor; }
    MotorSim& GetSimulator() const { return *m_Simulator; }
    OpenBurnSettings& GetGlobalSettings() const { return *m_GlobalSettings; }
    QUndoStack& GetUndoStack() const { return *m_UndoStack; }
    MotorSimSettings& GetSimSettings() const { return *m_SimSettings; }
    std::vector<OpenBurnPropellant>& GetAllPropellants() { return m_Propellants; };

    QString GetDesignFilename() const { return m_CurrentDesignFilename; }
    QString GetCurrentAppVersion() const { return m_ApplicationVersion; }
signals:
    void FileOpened(QString filename);
    void FileSaved(QString filename);

    void PropellantDatabaseUpdated(OpenBurnPropellant newPropellant);
private:
    std::unique_ptr<OpenBurnMotor> m_DesignMotor;
    std::unique_ptr<MotorSim> m_Simulator;
    std::unique_ptr<OpenBurnSettings> m_GlobalSettings;
    std::unique_ptr<MotorSimSettings> m_SimSettings;

    std::vector<OpenBurnPropellant> m_Propellants;

    QUndoStack *m_UndoStack;

    QString m_DatabaseFileName;
    QString m_CurrentDesignFilename;
    QString m_SettingsFileName;
    QString m_ApplicationVersion;
};


