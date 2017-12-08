#include "commands.h"

AddGrainCommand::AddGrainCommand(const std::shared_ptr<OpenBurnGrain>& grain, OpenBurnMotor* motor, 
	QUndoCommand* parent)
	: QUndoCommand(parent), 
	m_Motor(motor),
	m_GrainAdded(grain)
{
}
void AddGrainCommand::undo()
{
	m_Motor->RemoveGrain(m_GrainAdded);
}

void AddGrainCommand::redo()
{
	m_Motor->AddGrain(m_GrainAdded);
}

RemoveGrainCommand::RemoveGrainCommand(const GrainVector& grainsRemoved, 
	OpenBurnMotor* motor,
	QUndoCommand* parent)
	: QUndoCommand(parent),
	m_Motor(motor)
{
	for (auto& i : grainsRemoved)
	{
		m_GrainsRemoved[m_Motor->GetGrainIndex(i)] = i;
	}
}

void RemoveGrainCommand::undo()
{
	for (auto& i : m_GrainsRemoved)
	{
		m_Motor->AddGrain(i.second, i.first);
	}
}

void RemoveGrainCommand::redo()
{
	for (auto& i : m_GrainsRemoved)
	{
		m_Motor->RemoveGrain(i.second);
	}
}

/*
ModifyGrainCommand::ModifyGrainCommand(const std::shared_ptr<OpenBurnGrain>& grain,
	const std::shared_ptr<OpenBurnGrain>& oldGrain,
	OpenBurnMotor* motor, QUndoCommand* parent)
	: QUndoCommand(parent),
	m_Motor(motor),
	m_NewGrain(grain),
	m_OldGrain(oldGrain),
	m_GrainIdx(motor->GetGrainIndex(grain))
{
}
*/
ModifyGrainCommand::ModifyGrainCommand(const GrainVector& newGrains,
	const GrainVector& oldGrains,
	OpenBurnMotor* motor,
	QUndoCommand* parent)
	: QUndoCommand(parent),
	m_Motor(motor)
{
	//we want to create the map of grains. so we declare two iterators for both grain vectors
	std::pair<GrainVector::const_iterator, GrainVector::const_iterator> iter(newGrains.begin(), oldGrains.begin());
	for (; iter.first != newGrains.end(), iter.second != oldGrains.end(); ++iter.first, ++iter.second)
	{
		//iter.first is the current iterator's newGrain value, this is the current value in the motor
		//since the pointers are modified by the grain dialog
		const auto currentGrain = *iter.first;
		m_Grains[m_Motor->GetGrainIndex(currentGrain)].second = *iter.second;
		m_Grains[m_Motor->GetGrainIndex(currentGrain)].first = *iter.first;
	}
}

void ModifyGrainCommand::undo()
{
	for (auto& i : m_Grains)
	{
		m_Motor->UpdateGrain(i.second.second, i.first);
	}
}

void ModifyGrainCommand::redo()
{
	for (auto& i : m_Grains)
	{
		m_Motor->UpdateGrain(i.second.first, i.first);
	}
}
