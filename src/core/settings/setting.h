#pragma once
#include <cstdint>
#include <type_traits>
#include "hardware/storage.h"

template <class T>
class Setting
{

		static_assert(
			std::is_integral_v<T> || std::is_enum_v<T>,
			"Setting supports only bool, integer and enum types"
		);
		static_assert(
			sizeof(T) <= sizeof(int32_t),
			"Setting value must fit into 32 bits"
		);

	public:
		constexpr Setting(
			const char *space_name,
			const char *key_name,
			T default_value
		)
			: space(space_name), key(key_name), defaultValue(default_value)
		{
		}

		T get() const
		{
			if constexpr (std::is_same_v<T, bool>) {
				return get_storage_bool(
					this->space,
					this->key,
					this->defaultValue
				);
			} else {
				return static_cast<T>(
					get_storage_int(
						this->space,
						this->key,
						static_cast<int32_t>(this->defaultValue)
					)
				);
			}
		}

		void set(T value) const
		{
			if constexpr (std::is_same_v<T, bool>) {
				set_storage_bool(
					this->space,
					this->key,
					value
				);
			} else {
				set_storage_int(
					this->space,
					this->key,
					static_cast<int32_t>(value)
				);
			}
		}

	protected:
		const char *const space;
		const char *const key;
		const T defaultValue;
};
