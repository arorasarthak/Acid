#pragma once

#include <utility>

#include "Engine/Engine.hpp"
#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"
#include "Utils/Factory.hpp"
#include "Utils/String.hpp"

namespace acid {
template<typename T>
class MultipleChoice {
public:
	class Option {
	public:
		Option(const T &value, std::string name, bool enabled) :
			value(value), name(std::move(name)), enabled(enabled) {
		}

		T value;
		std::string name;
		bool enabled;
	};
	
	MultipleChoice() = default;
	explicit MultipleChoice(const std::vector<T> &options, std::size_t current = 0) :
		current(current),
		options(options) {
	}

	void Add(T value, const std::string &name, bool enabled = true) {
		options.emplace_back(value, name, enabled);
	}

	std::size_t current = 0;
private:
	std::vector<Option> options;
};



template<typename Base>
class SettingFactory {
public:
	virtual ~SettingFactory() = default;

	static auto &Registry() {
		static std::unordered_map<std::string, std::function<std::unique_ptr<Base>()>> impl;
		return impl;
	}

	template<typename T, typename K, std::uint32_t CategoryId>
	class Setting : public Base {
	public:
		explicit Setting(const std::string &name, K value = {}) :
			Base(name, CategoryId),
			value(std::move(value)) {
		}

		const K &GetValue() const { return value; }
		void SetValue(const K &value) { this->value = value; }

	protected:
		static bool Register(const std::string &name) {
			SettingFactory::Registry()[name] = []() -> std::unique_ptr<Base> {
				return std::make_unique<T>();
			};
			return true;
		}

		K value;
	};
};

class SettingsSetting {
public:
	SettingsSetting(std::string name, std::uint32_t category) :
		name(std::move(name)),
		category(category) {
	}
	virtual ~SettingsSetting() = default;

	const std::string name;
	const std::uint32_t category;
};

template<typename Base>
class SettingsTabFactory {
public:
	virtual ~SettingsTabFactory() = default;

	static auto &Registry() {
		static std::vector<std::function<std::unique_ptr<Base>()>> impl;
		return impl;
	}

	template<typename T>
	class Tab : public Base {
	public:
		explicit Tab(const std::string &name) :
			Base(name) {
		}
	protected:
		static bool Register() {
			SettingsTabFactory::Registry().emplace_back([]() -> std::unique_ptr<Base> {
				auto result = std::make_unique<T>();
				for (const auto &[name, func] : T::Registry()) {
					result->settings[name] = func();
				}
				return std::move(result);
			});
			return true;
		}
	};
};

class SettingsTab : public SettingFactory<SettingsSetting> {
protected:
	template<typename TabType, std::uint32_t TypeId>
	class Category {
	public:
		template<typename T, typename K>
		using Setting = typename TabType::template Setting<T, K, TypeId>;
	};
public:
	using Default = Category<SettingsTab, "Default"_hash>;
	
	explicit SettingsTab(std::string name) :
		name(std::move(name)) {
	}
	virtual ~SettingsTab() = default;
	
	const std::string name;
	std::unordered_map<std::string, std::unique_ptr<SettingsSetting>> settings;
};

/**
 * @brief Module used for managing engine configurations.
 */
class ACID_EXPORT Settings : public Module::Registrar<Settings, Module::Stage::Post>, public SettingsTabFactory<SettingsTab> {
public:
	Settings();

	void Update() override;

	void DoWithSettings(const std::function<void(std::string, SettingsSetting *)> &action) const;

private:
	std::vector<std::unique_ptr<SettingsTab>> tabs;
};
using Color = Colour;



class DeveloperSettingsTab : public Settings::Tab<DeveloperSettingsTab> {
	inline static const bool Registered = Register();
public:
	DeveloperSettingsTab() : Tab("Developer Options") {}
};

enum class LogLevel {
	Debug, Warning, Error
};
class LogLevelSetting : public DeveloperSettingsTab::Default::Setting<LogLevelSetting, MultipleChoice<LogLevel>> {
	inline static const bool Registered = Register("SETTING_DEVELOPER_LOG_LEVEL");
public:
	LogLevelSetting() : Setting("Log Level") {
		value.Add(LogLevel::Debug, "Debug");
		value.Add(LogLevel::Warning, "Warning");
		value.Add(LogLevel::Error, "Error");
	}
};



class ColorSettingsTab : public Settings::Tab<ColorSettingsTab> {
	inline static const bool Registered = Register();
public:
	using Background = Category<ColorSettingsTab, "Background"_hash>;
	using Waveform = Category<ColorSettingsTab, "Waveform"_hash>;
	ColorSettingsTab() : Tab("Colors") {}
};

class BackgroundColorDarkSetting : public ColorSettingsTab::Background::Setting<BackgroundColorDarkSetting, Color> {
	inline static const bool Registered = Register("SETTING_BACKGROUND_COLOR_DARK");
public:
	BackgroundColorDarkSetting() : Setting("High Contrast", 0xFF1c1c1c) {}
};

class BackgroundColorLightSetting : public ColorSettingsTab::Background::Setting<BackgroundColorLightSetting, Color> {
	inline static const bool Registered = Register("SETTING_BACKGROUND_COLOR_LIGHT");
public:
	BackgroundColorLightSetting() : Setting("Normal", 0xFFFFFFFF) {}
};

class WaveformColorDarkSetting : public ColorSettingsTab::Waveform::Setting<WaveformColorDarkSetting, Color> {
	inline static const bool Registered = Register("SETTING_ECG_COLOR_DARK");
public:
	WaveformColorDarkSetting() : Setting("High Contrast", 0xFFFD9F2B) {}
};

class WaveformColorLightSetting : public ColorSettingsTab::Waveform::Setting<WaveformColorLightSetting, Color> {
	inline static const bool Registered = Register("SETTING_ECG_COLOR_LIGHT");
public:
	WaveformColorLightSetting() : Setting("Normal", 0xFF000000) {}
};
}
