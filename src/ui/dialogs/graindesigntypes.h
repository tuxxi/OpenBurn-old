#pragma once

#include <QGridLayout>
#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGraphicsView>
#include <QPushButton>
#include <QComboBox>
#include <QToolButton>

#include "src/grain.h"
#include "src/propellant.h"

//This class represents the "default" parameters for grain design:
//Length, Diameter, TYPE, propellant, and inhibited faces. 
//The QGridLayout lays out the following:
//idx 0: type, 1: propellant, 2: length, 3: diameter, 255: inhibited faces
//any __custom__ grain properties, e.g core diameter, should inherit this class and
//call AddNewControls() in the constructor.
class OpenBurnDesignGrain : public QWidget
{
    Q_OBJECT
public:
    explicit OpenBurnDesignGrain(QWidget* parent = nullptr, OpenBurnGrain* seed = nullptr);
    virtual ~OpenBurnDesignGrain();

    double GetLength();
    double GetDiameter();
    int GetInhibitedFaces();
    OpenBurnPropellant* GetPropellant();
    GRAINTYPE GetGrainType();

private slots:
    void on_grainType_changed(int);
    
signals:
    void SIG_GrainType_Changed(GRAINTYPE type);
    void SIG_GrainDesign_Changed();
protected:
    virtual void SeedValues();
    void AddNewControls(QWidget* widet, int row, int col);

    OpenBurnGrain* m_seedGrain;
    
    QDoubleSpinBox *m_grainDiameterSpinBox, *m_grainLengthSpinBox;
    QSpinBox *m_grainInhibitedFacesSpinBox;

    QComboBox *m_grainTypeComboBox;
    QComboBox *m_propellantComboBox;
    QComboBox *m_grainLenUnitsComboBox, *m_grainDiaUnitsComboBox;

    QToolButton *m_modifyPropellantDatabase;
private:
    QGridLayout* controlsLayout;
    void SetupUI();
    
};

//a BATES grain
class BatesGrainDesign : public OpenBurnDesignGrain
{
    Q_OBJECT
public:
    explicit BatesGrainDesign(QWidget* parent = nullptr, BatesGrain* seed = nullptr);
    virtual ~BatesGrainDesign();

    double GetCoreDiameter();
    
protected:
    virtual void SeedValues() override;
    
    QDoubleSpinBox* m_grainCoreDiameterSpinBox;
    QComboBox* m_grainCoreDiaUnitsComboBox;
};