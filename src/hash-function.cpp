/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * The original code is copyright (c) 2023, shierru (Nikita).
*/

#include "hash-function.hpp"

int Hash::getID() const
{
	return poolID;
}

void Hash::update(std::string_view str)
{
	m_Hash->update(str);
}

std::string Hash::process(std::string_view str)
{
	return Botan::hex_encode(m_Hash->process(str));
}

std::string Hash::final()
{
	return Botan::hex_encode(m_Hash->final());
}

Hash::Hash(std::unique_ptr<Botan::HashFunction> hash) : 
	m_Hash(std::move(hash))
{
}