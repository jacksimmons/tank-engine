#include "asset_database.hpp"
#include "asset.hpp"
#include "log.hpp"


namespace Tank
{
	AssetDatabase::AssetDatabase()
	{
	}


	AssetDatabase::~AssetDatabase()
	{
	}


	std::expected<void, AddAssetError> AssetDatabase::addAsset(const fs::path &path, std::unique_ptr<Asset> asset)
	{
		if (m_assets.contains(path))
		{
			return std::unexpected(AddAssetError::KeyAlreadyTaken);
		}

		m_assets.insert({ path, std::move(asset) });
	}

	
	std::expected<std::unique_ptr<Asset>, RemoveAssetError> AssetDatabase::popAsset(const fs::path &path)
	{
		if (!m_assets.contains(path))
		{
			return std::unexpected(RemoveAssetError::KeyNotFound);
		}

		std::unique_ptr<Asset> asset = std::move(m_assets[path]);
		m_assets.erase(path);
	}
}