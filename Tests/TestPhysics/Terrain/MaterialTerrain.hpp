#pragma once

#include <Materials/Material.hpp>
#include <Graphics/Images/Image2d.hpp>

using namespace acid;

namespace test {
class MaterialTerrain : public Material::Registrar<MaterialTerrain> {
public:
	explicit MaterialTerrain(std::shared_ptr<Image2d> imageR = nullptr, std::shared_ptr<Image2d> imageG = nullptr);

	void CreatePipeline(const Shader::VertexInput &vertexInput, bool animated) override;
	void PushUniforms(UniformHandler &uniformObject, const Transform *transform) override;
	void PushDescriptors(DescriptorsHandler &descriptorSet) override;

	friend const Node &operator>>(const Node &node, MaterialTerrain &material);
	friend Node &operator<<(Node &node, const MaterialTerrain &material);

	const std::shared_ptr<Image2d> &GetImageR() const { return m_imageR; }
	void SetImageR(const std::shared_ptr<Image2d> &imageR) { m_imageR = imageR; }

	const std::shared_ptr<Image2d> &GetImageG() const { return m_imageG; }
	void SetImageG(const std::shared_ptr<Image2d> &imageG) { m_imageG = imageG; }

private:
	static bool registered;

	std::shared_ptr<Image2d> m_imageR;
	std::shared_ptr<Image2d> m_imageG;
};
}
