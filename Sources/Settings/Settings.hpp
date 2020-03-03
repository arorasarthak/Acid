#pragma once

#include <utility>

#include "Engine/Engine.hpp"
#include "Maths/Vector2.hpp"
#include "Utils/Factory.hpp"

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
		options(options),
		current(current) {
	}

	void Add(T value, const std::string &name, bool enabled = true) {
		options.emplace_back(value, name, enabled);
	}

private:
	std::vector<Option> options;
	std::size_t current = 0;
};

template<typename Base>
class SettingFactory {
public:
	virtual ~SettingFactory() = default;

	static auto &Registry() {
		static std::unordered_map<std::string, std::function<std::unique_ptr<Base>()>> impl;
		return impl;
	}

	template<typename T, typename K>
	class SettingType : public Base {
	public:
		explicit SettingType(const std::string &name, const K &value = {}) :
			Base(name),
			value(value) {
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

class Setting {
public:
	explicit Setting(std::string name) :
		name(std::move(name)) {
	}
	virtual ~Setting() = default;

	const std::string name;
};

template<typename Base>
class SettingsTabFactory {
public:
	virtual ~SettingsTabFactory() = default;

	static auto &Registry() {
		static std::unordered_map<std::string, std::function<std::unique_ptr<Base>()>> impl;
		return impl;
	}

	template<typename T>
	class SettingsTabType : public Base {
	public:
		explicit SettingsTabType(const std::string &name) :
			Base(name) {
		}
	protected:
		static bool Register(const std::string &name) {
			SettingsTabFactory::Registry()[name] = []() -> std::unique_ptr<Base> {
				return std::make_unique<T>();
			};
			return true;
		}
	};
};

//class SettingCategory : public 

class SettingsTab : public SettingsTabFactory<SettingsTab>, public SettingFactory<Setting> {
public:
	explicit SettingsTab(std::string name) :
		name(std::move(name)) {
	}
	virtual ~SettingsTab() = default;

	const std::string name;
};






class GraphicsSettingsTab : public SettingsTab::SettingsTabType<GraphicsSettingsTab> {
	inline static const bool Registered = Register("graphics");
public:
	GraphicsSettingsTab() : SettingsTabType("Graphics") {}
};

class WindowSizeSetting : public GraphicsSettingsTab::SettingType<WindowSizeSetting, Vector2i> {
	inline static const bool Registered = Register("windowSize");
public:
	WindowSizeSetting() : SettingType("Window Size", {1080, 720}) {}
};

class WindowModeSetting : public GraphicsSettingsTab::SettingType<WindowModeSetting, MultipleChoice<int>> {
	inline static const bool Registered = Register("windowMode");
public:
	WindowModeSetting() : SettingType("Window Mode") {
		value.Add(1, "Windowed");
		value.Add(2, "Fullscreen Borderless");
		value.Add(4, "Fullscreen Exclusive");
	}
};





/**
 * @brief Module used for managing engine configurations.
 */
class ACID_EXPORT Settings : public Module::Registrar<Settings, Module::Stage::Post> {
public:
	Settings();

	void Update() override;
};
}
