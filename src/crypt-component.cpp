/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * The original code is copyright (c) 2023, shierru (Nikita).
*/

#include <Server/Components/Pawn/pawn_natives.hpp>

#include <Server/Components/Pawn/pawn_impl.hpp>

#include "crypt-component.hpp"

void CryptComponent::onLoad(ICore* c) 
{
	m_Core = c;
}

void CryptComponent::onInit(IComponentList* components) 
{
	m_Pawn = components->queryComponent<IPawnComponent>();

	if (m_Pawn) 
	{
		setAmxFunctions(m_Pawn->getAmxFunctions());
		setAmxLookups(components);

		m_Pawn->getEventDispatcher().addEventHandler(this);
	}
}

void CryptComponent::onReady() 
{
}

void CryptComponent::onFree(IComponent* component) 
{
	if (component == m_Pawn) 
	{
		m_Pawn = nullptr;

		setAmxFunctions();
		setAmxLookups();
	}
}

void CryptComponent::free() 
{
	delete this;
}

void CryptComponent::reset() 
{
}

void CryptComponent::onAmxLoad(IPawnScript& script) 
{
	pawn_natives::AmxLoad(script.GetAMX());
}

void CryptComponent::onAmxUnload(IPawnScript& script) 
{
}

void CryptComponent::onTick(Microseconds elapsed, TimePoint now) 
{
}

StringView CryptComponent::componentName() const 
{
	return "Crypt";
}

SemanticVersion CryptComponent::componentVersion() const 
{
	return SemanticVersion(1, 0, 0, 0);
}

IHash* CryptComponent::createHashFunction(std::string name)
{
	auto hash = Botan::HashFunction::create(name);

	if (!hash)
	{
		m_Core->printLn("[Hash] Unknown provider");

		return nullptr;
	}

	auto ret = m_Pool.emplace(std::move(hash));

	if (!ret)
	{
		m_Core->printLn("[Hash] Pool is full");

		return nullptr;
	}

	return ret;
}

IHash* CryptComponent::getHashFunction(int id)
{
	return m_Pool.get(id);
}

bool CryptComponent::destroyHashFunction(IHash* hash)
{
	int id = static_cast<Hash*>(hash)->getID();

	m_Pool.release(id, false);

	return true;
}

CryptComponent* CryptComponent::getInstance() 
{
	if (m_Instance == nullptr)
		m_Instance = new CryptComponent();

	return m_Instance;
}

ICore* CryptComponent::getCore()
{
	return m_Instance->m_Core;
}

CryptComponent::~CryptComponent()
{
	if (m_Pawn)
		m_Pawn->getEventDispatcher().removeEventHandler(this);

	if (m_Core)
		m_Core->getEventDispatcher().removeEventHandler(this);
}