#pragma once

#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGraphicsView>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>

#include "src/nozzle.hpp"
#include "src/settings.hpp"
#include "nozzledesigntypes.hpp"
#include "src/ui/graphics/nozzlegraphicsitem.hpp"

class NozzleDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NozzleDialog(const OpenBurnNozzle* seed = nullptr, 
		OpenBurnSettings* settings = nullptr,
		QWidget* parent = nullptr);
	~NozzleDialog();
protected:
	void closeEvent(QCloseEvent* event) override;
signals:
	void NewNozzle(NozzlePtr& nozzle);
    void NozzleModified(NozzlePtr& nozzle, NozzlePtr& oldNozzle);
	void DialogClosed();
private slots:
    void OnApplyButtonClicked();
    void OnOkButtonClicked();
    void OnCloseButtonClicked();
    void OnDesignUpdated();

private:
    void SetupUI();
    void Accept();

    QGroupBox* m_gbFrame;

    QPushButton *m_btnApply, *m_btnOK, *m_btnClose;

    OpenBurnDesignNozzle* m_NozzleDesign;
    NozzlePtr m_Nozzle;
	NozzlePtr m_OldNozzle;
    OpenBurnSettings* m_GlobalSettings;
};
