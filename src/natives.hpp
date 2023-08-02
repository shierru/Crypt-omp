/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * The original code is copyright (c) 2023, shierru (Nikita).
*/

#ifndef SRC_NATIVES_HPP_
#define SRC_NATIVES_HPP_

#include <Server/Components/Pawn/pawn_natives.hpp>

#include "crypt-component.hpp"

namespace pawn_natives
{
	template <>
	struct ParamLookup<IHash>
	{
		static IHash& ValReq(cell ref)
		{
			if (auto pool = CryptComponent::getInstance())
			{
				auto ptr = pool->getHashFunction(ref);
				if (ptr)
				{
					return *ptr;
				}
			}
			throw pawn_natives::ParamCastFailure();
		}

		static IHash* Val(cell ref) noexcept
		{
			if (auto pool = CryptComponent::getInstance())
			{
				return pool->getHashFunction(ref);
			}
			return nullptr;
		}
	};

	template <>
	class ParamCast<IHash*>
	{
	public:
		ParamCast(AMX* amx, cell* params, int idx) noexcept
		{
			m_Value = ParamLookup<IHash>::Val(params[idx]);
		}

		~ParamCast()
		{
		}

		ParamCast(ParamCast<IHash*> const&) = delete;
		ParamCast(ParamCast<IHash*>&&) = delete;

		operator IHash* ()
		{
			return m_Value;
		}

		static constexpr int Size = 1;

	private:
		IHash* m_Value;
	};

	template <>
	class ParamCast<IHash&>
	{
	public:
		ParamCast(AMX* amx, cell* params, int idx)
			: m_Value(ParamLookup<IHash>::ValReq(params[idx]))
		{
		}

		~ParamCast()
		{
		}

		ParamCast(ParamCast<IHash&> const&) = delete;
		ParamCast(ParamCast<IHash&>&&) = delete;

		operator IHash& ()
		{
			return m_Value;
		}

		static constexpr int Size = 1;

	private:
		IHash& m_Value;
	};

	template <>
	class ParamCast<const IHash&>
	{
	public:
		ParamCast(AMX*, cell*, int) = delete;
		ParamCast() = delete;
	};
}

#endif	// SRC_NATIVES_HPP_