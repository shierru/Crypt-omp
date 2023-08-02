/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * The original code is copyright (c) 2023, shierru (Nikita).
*/

#include <sdk.hpp>

#include "crypt-component.hpp"

COMPONENT_ENTRY_POINT()
{
	return CryptComponent::getInstance();
}