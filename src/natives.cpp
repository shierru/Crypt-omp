/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * The original code is copyright (c) 2023, shierru (Nikita).
*/

#include <sdk.hpp>

#include "natives.hpp"

SCRIPT_API(Hash_Create, int(const std::string& name))
{
	if (auto component = CryptComponent::getInstance())
	{
		if (auto ret = component->createHashFunction(name))
			return ret->getID();		
	}
	return 0;
}

SCRIPT_API(Hash_Update, bool(IHash& hash, const std::string& value))
{
	if (auto component = CryptComponent::getInstance())
	{
		hash.update(value);

		return true;
	}
	return false;
}

SCRIPT_API(Hash_Final, bool(IHash& hash, OutputOnlyString& output))
{
	if (auto component = CryptComponent::getInstance())
	{
		output = hash.final();

		return true;
	}
	return false;
}

SCRIPT_API(Hash_Process, bool(IHash& hash, const std::string& value, OutputOnlyString& output))
{
	if (auto component = CryptComponent::getInstance())
	{
		output = hash.process(value);

		return true;
	}
	return false;
}

SCRIPT_API(Hash_Destroy, bool(IHash& hash))
{
	if (auto component = CryptComponent::getInstance())
	{
		return component->destroyHashFunction(&hash);
	}
	return false;
}

SCRIPT_API(Hash, bool(const std::string& name, const std::string& value, const std::string& salt, OutputOnlyString& output))
{
	if (auto component = CryptComponent::getInstance())
	{
		auto hash = Botan::HashFunction::create(name);

		if (!hash)
		{
			component->getCore()->printLn("[Hash] Unknown provider");

			return false;
		}
		hash->update(value);
		hash->update(salt);

		output = Botan::hex_encode(hash->final());

		return true;
	}

	return false;
}