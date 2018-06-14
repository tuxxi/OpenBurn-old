#pragma once

#include <QGridLayout>

#include "src/nozzle.hpp"
#include "src/settings.hpp"
#include "src/ui/widgets/unitscombobox.hpp"

class BaseNozzleDesign : public QObject
{
    Q_OBJECT
public:
    explicit BaseNozzleDesign(
        QGridLayout* layout,
        OpenBurnNozzle* seed = nullptr,
        OpenBurnSettings* settings = nullptr);

    virtual ~BaseNozzleDesign() = default;

signals:
    void DesignUpdated();
protected:
    void SetupUI();
    QGridLayout* m_Layout;

};
class ConicalNozzleDesign : public BaseNozzleDesign
{
    Q_OBJECT
public:
    explicit ConicalNozzleDesign(
        QWidget* parent = nullptr,
        ConicalNozzle* seed = nullptr,
        OpenBurnSettings* settings = nullptr);
    ~ConicalNozzleDesign() = default;

    double GetDivergentHalfAngle();
protected:
    QDoubleSpinBox *m_sbHalfAngle;
    AngleUnitsComboBox *m_unitsHalfAngle;
};
