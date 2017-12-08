#pragma once

#include  <QUndoCommand>
#include <memory>
#include "src/motor.h"

class AddGrainCommand : public QUndoCommand
{
public:
	explicit AddGrainCommand(const std::shared_ptr<OpenBurnGrain>& grain, 
		OpenBurnMotor* motor, 
		QUndoCommand* parent = nullptr);
	void undo() override;
	void redo() override;
private:
	std::shared_ptr<OpenBurnGrain> m_GrainAdded;
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
	std::map<int, std::shared_ptr<OpenBurnGrain>> m_GrainsRemoved;
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
	std::map<int, std::pair<std::shared_ptr<OpenBurnGrain>, std::shared_ptr<OpenBurnGrain>>> m_Grains;

	OpenBurnMotor* m_Motor;
};