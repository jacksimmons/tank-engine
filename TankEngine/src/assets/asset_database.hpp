#pragma once
#include <expected>


enum class AddAssetError
{
	KeyAlreadyTaken,
};


enum class RemoveAssetError
{
	KeyNotFound,
};


namespace Tank
{
	class Asset;
	class AssetDatabase final
	{
	private:
		std::unordered_map<fs::path, std::unique_ptr<Asset>> m_assets;

	public:
		AssetDatabase();
		~AssetDatabase();

		std::expected<void, AddAssetError> addAsset(const fs::path &path, std::unique_ptr<Asset> asset);
		std::expected<std::unique_ptr<Asset>, RemoveAssetError> popAsset(const fs::path &path);
	};
}