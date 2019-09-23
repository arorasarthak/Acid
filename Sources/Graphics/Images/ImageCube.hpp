#pragma once

#include "Bitmaps/Bitmap.hpp"
#include "Helpers/NonCopyable.hpp"
#include "Resources/Resource.hpp"
#include "Image.hpp"

namespace acid {
/**
 * @brief Resource that represents a cubemap image.
 */
class ACID_EXPORT ImageCube : public NonCopyable, public Descriptor, public Resource {
public:
	/**
	 * Creates a new cubemap image, or finds one with the same values.
	 * @param node The node to decode values from.
	 * @return The cubemap image with the requested values.
	 */
	static std::shared_ptr<ImageCube> Create(const Node &node);

	/**
	 * Creates a new cubemap image.
	 * @param filename The file to load the image from.
	 * @param fileSuffix The files extension type (ex .png).
	 * @param filter The magnification/minification filter to apply to lookups.
	 * @param addressMode The addressing mode for outside [0..1] range.
	 * @param anisotropic If anisotropic filtering is enabled.
	 * @param mipmap If mapmaps will be generated.
	 * @return The cubemap image with the requested values.
	 */
	static std::shared_ptr<ImageCube> Create(const std::filesystem::path &filename, const std::string &fileSuffix, VkFilter filter = VK_FILTER_LINEAR,
		VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, bool anisotropic = true, bool mipmap = true);

	/**
	 * Creates a new cubemap image.
	 * @param filename The file to load the image from.
	 * @param fileSuffix The files extension type (ex .png).
	 * @param filter The magnification/minification filter to apply to lookups.
	 * @param addressMode The addressing mode for outside [0..1] range.
	 * @param anisotropic If anisotropic filtering is enabled.
	 * @param mipmap If mapmaps will be generated.
	 * @param load If this resource will be loaded immediately, otherwise {@link ImageCube#Load} can be called later.
	 */
	explicit ImageCube(std::filesystem::path filename, std::string fileSuffix = ".png", VkFilter filter = VK_FILTER_LINEAR,
		VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, bool anisotropic = true, bool mipmap = true, bool load = true);

	/**
	 * Creates a new cubemap image.
	 * @param extent The images extent in pixels.
	 * @param format The format and type of the texel blocks that will be contained in the image.
	 * @param layout The layout that the image subresources accessible from.
	 * @param usage The intended usage of the image.
	 * @param filter The magnification/minification filter to apply to lookups.
	 * @param addressMode The addressing mode for outside [0..1] range.
	 * @param samples The number of samples per texel.
	 * @param anisotropic If anisotropic filtering is enabled.
	 * @param mipmap If mapmaps will be generated.
	 */
	ImageCube(const Vector2ui &extent, VkFormat format = VK_FORMAT_R8G8B8A8_UNORM, VkImageLayout layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 
		VkImageUsageFlags usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
		VkFilter filter = VK_FILTER_LINEAR, VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT, bool anisotropic = false, bool mipmap = false);

	/**
	 * Creates a new cubemap image.
	 * @param bitmap The bitmap to load from.
	 * @param format The format and type of the texel blocks that will be contained in the image.
	 * @param layout The layout that the image subresources accessible from.
	 * @param usage The intended usage of the image.
	 * @param filter The magnification/minification filter to apply to lookups.
	 * @param addressMode The addressing mode for outside [0..1] range.
	 * @param samples The number of samples per texel.
	 * @param anisotropic If anisotropic filtering is enabled.
	 * @param mipmap If mapmaps will be generated.
	 */
	ImageCube(std::unique_ptr<Bitmap> &&bitmap, VkFormat format = VK_FORMAT_R8G8B8A8_UNORM, VkImageLayout layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 
		VkImageUsageFlags usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
		VkFilter filter = VK_FILTER_LINEAR, VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT, bool anisotropic = false, bool mipmap = false);

	~ImageCube();

	WriteDescriptorSet GetWriteDescriptor(uint32_t binding, VkDescriptorType descriptorType, const std::optional<OffsetSize> &offsetSize) const override;
	static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stage, uint32_t count);

	/**
	 * Copies the images pixels from memory to a bitmap.
	 * @param mipLevel The mipmap level index to sample.
	 * @param arrayLayer The array layer to sample.
	 * @return A copy of the images pixels.
	 */
	Bitmap GetBitmap(uint32_t mipLevel, uint32_t arrayLayer) const;

	/**
	 * Copies the images pixels from memory to a bitmap. The bitmap height will be scaled by the amount of layers.
	 * @param mipLevel The mipmap level index to sample.
	 * @return A copy of the images pixels.
	 */
	Bitmap GetBitmap(uint32_t mipLevel = 0) const;

	/**
	 * Sets the pixels of this image.
	 * @param pixels The pixels to copy from.
	 * @param layerCount The amount of layers contained in the pixels.
	 * @param baseArrayLayer The first layer to copy into.
	 */
	void SetPixels(const uint8_t *pixels, uint32_t layerCount, uint32_t baseArrayLayer);

	friend const Node &operator>>(const Node &node, ImageCube &image);
	friend Node &operator<<(Node &node, const ImageCube &image);

	const std::filesystem::path &GetFilename() const { return m_filename; }
	const std::string &GetFileSuffix() const { return m_fileSuffix; }
	const std::vector<std::string> &GetFileSides() const { return m_fileSides; }
	VkFilter GetFilter() const { return m_filter; }
	VkSamplerAddressMode GetAddressMode() const { return m_addressMode; }
	bool IsAnisotropic() const { return m_anisotropic; }
	bool IsMipmap() const { return m_mipmap; }
	VkSampleCountFlagBits GetSamples() const { return m_samples; }
	VkImageLayout GetLayout() const { return m_layout; }
	VkImageUsageFlags GetUsage() const { return m_usage; }
	uint32_t GetComponents() const { return m_components; }
	const Vector2ui &GetExtent() const { return m_extent; }
	uint32_t GetMipLevels() const { return m_mipLevels; }
	const VkImage &GetImage() const { return m_image; }
	const VkDeviceMemory &GetMemory() { return m_memory; }
	const VkSampler &GetSampler() const { return m_sampler; }
	const VkImageView &GetView() const { return m_view; }
	const VkFormat &GetFormat() const { return m_format; }

private:
	void Load();

	std::filesystem::path m_filename;
	std::string m_fileSuffix;
	std::vector<std::string> m_fileSides = {"Right", "Left", "Top", "Bottom", "Back", "Front"};

	VkFilter m_filter;
	VkSamplerAddressMode m_addressMode;
	bool m_anisotropic;
	bool m_mipmap;
	VkSampleCountFlagBits m_samples;
	VkImageLayout m_layout;
	VkImageUsageFlags m_usage;

	uint32_t m_components = 0;
	Vector2ui m_extent;
	std::unique_ptr<Bitmap> m_loadBitmap;
	uint32_t m_mipLevels = 0;

	VkImage m_image = VK_NULL_HANDLE;
	VkDeviceMemory m_memory = VK_NULL_HANDLE;
	VkSampler m_sampler = VK_NULL_HANDLE;
	VkImageView m_view = VK_NULL_HANDLE;
	VkFormat m_format;
};
}
