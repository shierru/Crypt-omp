/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * The original code is copyright (c) 2023, shierru (Nikita).
*/

#ifndef SRC_CRYPT_COMPONENT_HPP_
#define SRC_CRYPT_COMPONENT_HPP_

#include <sdk.hpp>

#include "interface.hpp"

#include "hash-function.hpp"

#include "botan_all.h"

#include <Server/Components/Pawn/pawn.hpp>

#include <Impl/pool_impl.hpp>

class CryptComponent final : public ICryptComponent, public PawnEventHandler, public CoreEventHandler 
{
public:
	StringView componentName()	const override;

	SemanticVersion componentVersion() const override;

	void onLoad(ICore* c) override;

	void onInit(IComponentList* components) override;

	void onReady() override;

	void onFree(IComponent* component) override;

	void free() override;

	void reset() override;

	void onAmxLoad(IPawnScript& script) override;

	void onAmxUnload(IPawnScript& script) override;

	void onTick(Microseconds elapsed, TimePoint now) override;

	IHash* createHashFunction(std::string name) override;

	IHash* getHashFunction(int id) override;

	bool destroyHashFunction(IHash* hash) override;

	static CryptComponent* getInstance();

	static ICore* getCore();

	~CryptComponent();

private:
	ICore* m_Core = nullptr;

	IPawnComponent* m_Pawn = nullptr;
	
	Impl::MarkedPoolStorage<Hash, IHash, 1, HASH_POOL_SIZE> m_Pool;

	inline static CryptComponent* m_Instance = nullptr;
};

#endif  // SRC_CRYPT_COMPONENT_HPP_