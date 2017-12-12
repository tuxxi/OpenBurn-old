#pragma once

#include <QGridLayout>
#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGraphicsView>
#include <QPushButton>
#include <QComboBox>
#include <QToolButton>

#include "src/grain.hpp"
#include "src/settings.hpp"
#include "src/propellant.hpp"
#include "src/ui/widgets/unitscombobox.hpp"

//This class represents the "default" parameters for grain design:
//Length, Diameter, bore shape, propellant, and inhibited faces.
//any custom grain properties, e.g core diameter, should inherit this class and
//call AddNewControls() on the new control widgets in the constructor.
class OpenBurnDesignGrain : public QWidget
{
    Q_OBJECT
public:
    explicit OpenBurnDesignGrain(
        PropellantList* prop, 
        OpenBurnGrain* seed = nullptr, 
        OpenBurnSettings* settings = nullptr,
        QWidget* parent = nullptr);

    virtual ~OpenBurnDesignGrain() = default;

    double GetLength();
    double GetDiameter();
    int GetInhibitedFaces();
    OpenBurnPropellant GetPropellant();
signals:
    void GrainDesignChanged();
protected:
    virtual void SeedValues();
    void AddNewControls(QWidget* widet, int row, int col);

    OpenBurnGrain* m_grainSeed;
    
    QDoubleSpinBox *m_sbGrainDia, *m_sbGrainLen;
    QSpinBox *m_sbGrainInhibit;

    QComboBox *m_cbGrainType;
    QComboBox *m_cbPropellantType;
    LengthUnitsComboBox *m_unitsGrainLen, *m_unitsGrainDia;

    QToolButton *m_btntModifyPropellant;
    OpenBurnSettings* m_GlobalSettings;

private:
    void SetupUI();
    QGridLayout* m_layControls;
    PropellantList* m_Propellants;
    
};

//a BATES grain
class CylindricalGrainDesign : public OpenBurnDesignGrain
{
    Q_OBJECT
public:
    explicit CylindricalGrainDesign(
        PropellantList* prop, 
        OpenBurnGrain* seed = nullptr,
        OpenBurnSettings* settings = nullptr,
        QWidget* parent = nullptr);
        
    ~CylindricalGrainDesign() = default;

    double GetCoreDiameter();
protected:
    virtual void SeedValues() override;
    
    QDoubleSpinBox* m_sbGrainCoreDia;
    LengthUnitsComboBox* m_unitsGrainCoreDia;
};
