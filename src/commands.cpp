#include "commands.h"

AddGrainCommand::AddGrainCommand(const GrainPtr& grain, OpenBurnMotor* motor, 
	QUndoCommand* parent)
	: QUndoCommand(parent),
    m_GrainAdded(grain),
    m_Motor(motor)
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

ModifyNozzleCommand::ModifyNozzleCommand(NozzlePtr&& newNozzle, 
	NozzlePtr&& oldNozzle, 
	OpenBurnMotor* motor,
	QUndoCommand* parent)
	: QUndoCommand(parent),
	m_NewNozzle(std::move(newNozzle)),
	m_OldNozzle(std::move(oldNozzle)),
	m_Motor(motor)
{
}

void ModifyNozzleCommand::undo()
{
	auto tempObject = m_OldNozzle->Clone(); //create a temp object that's a clone of the nozzle
	m_Motor->SetNozzle(std::move(m_OldNozzle)); //move the nozzle into the motor, releasing it from this scope
	m_OldNozzle.release(); //we also need to release *this from owning the nozzle ptr or it will be deleted when scope exits
	m_OldNozzle = std::move(tempObject); //finally, move the temp object back into the nozzle so we can undo/redo later
}

void ModifyNozzleCommand::redo()
{
	//see undo()
	auto tempObject = m_NewNozzle->Clone();
	m_Motor->SetNozzle(std::move(m_NewNozzle));
	m_NewNozzle.release();
	m_NewNozzle = std::move(tempObject);
}

NewNozzleCommand::NewNozzleCommand(NozzlePtr&& newNozzle, 
	OpenBurnMotor* motor, 
	QUndoCommand* parent)
	: QUndoCommand(parent),
	m_NewNozzle(std::move(newNozzle)),
	m_Motor(motor)
{
}
void NewNozzleCommand::undo()
{
	m_Motor->RemoveNozzle();
}

void NewNozzleCommand::redo()
{
	//see ModifyNozzleCommand::undo()
	auto tempObject = m_NewNozzle->Clone();
	m_Motor->SetNozzle(std::move(m_NewNozzle));
	m_NewNozzle.release();
	m_NewNozzle = std::move(tempObject);
}
