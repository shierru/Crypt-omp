/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * The original code is copyright (c) 2023, shierru (Nikita).
*/

#ifndef SRC_INTERFACE_HPP_
#define SRC_INTERFACE_HPP_

#include <sdk.hpp>

#include "botan_all.h"

const int HASH_POOL_SIZE = 1000;
const int INVALID_HASH = 0;

struct IHash
{
	virtual int getID() const = 0;

	virtual void update(std::string_view str) = 0;

	virtual std::string process(std::string_view str) = 0;

	virtual std::string final() = 0;
};

struct ICryptComponent : IComponent
{
	PROVIDE_UID(0xbd7ef487bbc24007); /* 0xbd7ef487bbc24007a2e4969c43e85fa7 */

	virtual IHash* createHashFunction(std::string name) = 0;

	virtual IHash* getHashFunction(int id) = 0;

	virtual bool destroyHashFunction(IHash* hash) = 0;
};

#endif // SRC_INTERFACE_HPP_