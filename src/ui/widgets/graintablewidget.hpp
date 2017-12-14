#pragma once
#include <QTableWidget>
#include <QDropEvent>
#include <QTableView>
#include <QHeaderView>
#include <QList>

#include "src/motor.hpp"
#include "src/settings.hpp"

//TODO: maybe move to tree view?

//This class manages the table of grains displayed in the design window.
class GrainTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit GrainTableWidget(OpenBurnMotor* motor, OpenBurnSettings* settings, QWidget *parent = nullptr);
    ~GrainTableWidget() = default;

    QList<int>GetSelectedGrainIndices() const;
    GrainVector GetSelectedGrains() const;

public slots:
    void OnGrainAdded(OpenBurnGrain* grain);
    void OnGrainEmplaced(OpenBurnGrain* grain, int idx); //

    void OnGrainUpdated(OpenBurnGrain* grain, int idx);
    void OnGrainRemoved(int idx);
    void OnGrainsSwapped(int idx1, int idx2);
    void OnSettingsUpdated();
protected:
    virtual void dropEvent(QDropEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

private:
    QList<QTableWidgetItem*> TakeRow(int row);
    void SetRow(int row, const QList<QTableWidgetItem*>& rowItems);

    OpenBurnMotor* m_Motor;
    OpenBurnSettings* m_Settings;
};

