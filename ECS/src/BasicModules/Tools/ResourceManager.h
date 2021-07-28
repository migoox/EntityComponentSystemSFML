#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>

namespace Basic {
	class ResourceManager
	{
	private:
		// Location Name -> Location
		std::unordered_map<std::string, std::string> m_Locations;

		// Location Name -> Texture
		std::unordered_map<std::string, std::shared_ptr<sf::Texture>> m_Textures;

		static ResourceManager s_Instance;

	private:
		ResourceManager();

		void InitLocations();

		std::shared_ptr<sf::Texture> ITextureAcquire(const std::string& strInfo);

	public:
		static std::shared_ptr<sf::Texture> TextureAcquire(const std::string& strInfo);

	};

}
