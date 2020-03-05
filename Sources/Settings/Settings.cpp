#include "Settings.hpp"

namespace acid {
Settings::Settings() {
	for (const auto &tabFunc : SettingsTabFactory::Registry()) {
		tabs.emplace_back(tabFunc());
	}
	
#ifdef DEBUG
	DoWithSettings([](std::string settingName, SettingsSetting *setting) {
		Log::Debug("Setting ", settingName, "\n");
	});
#endif
}

void Settings::Update() {
}

void Settings::DoWithSettings(const std::function<void(std::string, SettingsSetting *)> &action) const {
	for (const auto &tab : tabs) {
		for (const auto &[settingKey, setting] : tab->settings) {
			action(settingKey, setting.get());
		}
	}
}
}
