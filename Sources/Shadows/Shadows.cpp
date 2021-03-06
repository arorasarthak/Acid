#include "Shadows.hpp"

#include "Scenes/Scenes.hpp"

namespace acid {
Shadows::Shadows() :
	m_lightDirection(0.5f, 0.0f, 0.5f),
	m_shadowSize(4096),
	m_shadowPcf(1),
	m_shadowBias(0.001f),
	m_shadowDarkness(0.6f),
	m_shadowTransition(11.0f),
	m_shadowBoxOffset(9.0f),
	m_shadowBoxDistance(70.0f) {
}

void Shadows::Update() {
	if (auto camera = Scenes::Get()->GetCamera()) {
		m_shadowBox.Update(*camera, m_lightDirection, m_shadowBoxOffset, m_shadowBoxDistance);
	}
}
}
