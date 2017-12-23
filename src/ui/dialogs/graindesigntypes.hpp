#pragma once

#include <QGridLayout>

#include "src/grain.hpp"
#include "src/settings.hpp"
#include "src/ui/widgets/unitscombobox.hpp"

class BaseGrainDesign : public QObject
{
    Q_OBJECT
public:
    explicit BaseGrainDesign(
        QGridLayout* layout,
        OpenBurnGrain* seed = nullptr,
        OpenBurnSettings* settings = nullptr);

    virtual ~BaseGrainDesign();
signals:
    void GrainDesignChanged();
protected:
    OpenBurnGrain* m_grainSeed;
    OpenBurnSettings* m_GlobalSettings;
    QGridLayout* m_layControls;
};

//a BATES grain
class CylindricalGrainDesign : public BaseGrainDesign
{
    Q_OBJECT
public:
    explicit CylindricalGrainDesign(
        QGridLayout* layout,
        OpenBurnGrain* seed = nullptr,
        OpenBurnSettings* settings = nullptr);
        
    ~CylindricalGrainDesign() = default;

    double GetCoreDiameter() const;
    QDoubleSpinBox* m_sbGrainCoreDia;
    LengthUnitsComboBox* m_unitsGrainCoreDia;
};
//a Moonburner grain
class MoonGrainDesign : public BaseGrainDesign
{
Q_OBJECT
public:
    explicit MoonGrainDesign(
        QGridLayout* layout,
        OpenBurnGrain* seed = nullptr,
        OpenBurnSettings* settings = nullptr);

    ~MoonGrainDesign() = default;

    double GetCoreDiameter() const;
    double GetCoreOffset() const;

    QDoubleSpinBox* m_sbGrainCoreDia, *m_sbCoreOffset;
    LengthUnitsComboBox* m_unitsGrainCoreDia, *m_unitsCoreOffset;

};
