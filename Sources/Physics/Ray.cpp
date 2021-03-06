#include "Ray.hpp"

namespace acid {
Ray::Ray(bool useMouse, const Vector2f &screenStart) :
	m_useMouse(useMouse),
	m_screenStart(screenStart) {
}

void Ray::Update(const Vector3f &currentPosition, const Vector2f &mousePosition, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix) {
	m_origin = currentPosition;

	if (m_useMouse) {
		UpdateNormalizedDeviceCoordinates(mousePosition.m_x, mousePosition.m_y);
	} else {
		m_normalizedCoords = m_screenStart;
	}

	m_viewMatrix = viewMatrix;
	m_projectionMatrix = projectionMatrix;
	m_clipCoords = {m_normalizedCoords.m_x, m_normalizedCoords.m_y, -1.0f};
	UpdateEyeCoords();
	UpdateWorldCoords();
}

Vector3f Ray::GetPointOnRay(float distance) const {
	auto vector = distance * m_currentRay;
	return m_origin + vector;
}

Vector3f Ray::ConvertToScreenSpace(const Vector3f &position) const {
	Vector4f coords(position);
	coords = m_viewMatrix.Transform(coords);
	coords = m_projectionMatrix.Transform(coords);

	if (coords.m_w < 0.0f) {
		return {};
	}

	return {(coords.m_x / coords.m_w + 1.0f) / 2.0f, 1.0f - (coords.m_y / coords.m_w + 1.0f) / 2.0f, coords.m_z};
}

void Ray::UpdateNormalizedDeviceCoordinates(float mouseX, float mouseY) {
	m_normalizedCoords.m_x = (2.0f * mouseX) - 1.0f;
	m_normalizedCoords.m_y = (2.0f * mouseY) - 1.0f;
}

void Ray::UpdateEyeCoords() {
	m_invertedProjection = m_projectionMatrix.Inverse();
	m_eyeCoords = m_invertedProjection.Transform(m_clipCoords);
	m_eyeCoords = {m_eyeCoords.m_x, m_eyeCoords.m_y, -1.0f, 0.0f};
}

void Ray::UpdateWorldCoords() {
	m_invertedView = m_viewMatrix.Inverse();
	m_rayWorld = m_invertedView.Transform(m_eyeCoords);
	m_currentRay = {m_rayWorld};
}
}
