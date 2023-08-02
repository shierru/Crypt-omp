/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * The original code is copyright (c) 2023, shierru (Nikita).
*/

#ifndef SRC_HASH_FUNCTION_HPP_
#define SRC_HASH_FUNCTION_HPP_

#include <sdk.hpp>

#include <Impl/pool_impl.hpp>

#include "interface.hpp"

#include "botan_all.h"

class Hash final : public IHash, public Impl::PoolIDProvider
{
public:
	int getID() const override;

	void update(std::string_view str) override;

	std::string process(std::string_view str) override;

	std::string final() override;

	Hash(std::unique_ptr<Botan::HashFunction> hash);

private:
	std::unique_ptr<Botan::HashFunction> m_Hash;
};

#endif // SRC_HASH_FUNCTION_HPP_