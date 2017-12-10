#pragma once

#include  <QUndoCommand>
#include <memory>
#include "src/motor.h"

class AddGrainCommand : public QUndoCommand
{
public:
	explicit AddGrainCommand(const GrainPtr& grain, 
		OpenBurnMotor* motor, 
		QUndoCommand* parent = nullptr);
	void undo() override;
	void redo() override;
private:
	GrainPtr m_GrainAdded;
	OpenBurnMotor* m_Motor;
};
class RemoveGrainCommand : public QUndoCommand
{
public:
	explicit RemoveGrainCommand(const GrainVector& grainsRemoved,
		OpenBurnMotor* motor, 
		QUndoCommand* parent = nullptr);
	void undo() override;
	void redo() override;
private:
	//the grains we removed, and their indicies
	std::map<int, GrainPtr> m_GrainsRemoved;
	OpenBurnMotor* m_Motor;
	int m_GrainIdx;
};
class ModifyGrainCommand : public QUndoCommand
{
public:
	explicit ModifyGrainCommand(const GrainVector& newGrains,
		const GrainVector& oldGrains,
		OpenBurnMotor* motor, 
		QUndoCommand* parent = nullptr);
	void undo() override;
	void redo() override;
private:
	//old and new grains in a std::pair, and the key is associated with their index in the motor.
	std::map<int, std::pair<GrainPtr, GrainPtr>> m_Grains;

	OpenBurnMotor* m_Motor;
};
class ModifyNozzleCommand : public QUndoCommand
{
public:
	explicit ModifyNozzleCommand(NozzlePtr&& newNozzle,
		NozzlePtr&& oldNozzle,
		OpenBurnMotor* motor,
		QUndoCommand* parent = nullptr);
	void undo() override;
	void redo() override;
private:
	NozzlePtr m_NewNozzle, m_OldNozzle;
	OpenBurnMotor* m_Motor;
};
class NewNozzleCommand : public QUndoCommand
{
public:
	explicit NewNozzleCommand(NozzlePtr&& newNozzle,
		OpenBurnMotor* motor,
		QUndoCommand* parent = nullptr);
	void undo() override;
	void redo() override;
private:
	NozzlePtr m_NewNozzle;
	OpenBurnMotor* m_Motor;
};