#include "ResourceManager.h"

Basic::ResourceManager Basic::ResourceManager::s_Instance;

Basic::ResourceManager::ResourceManager()
{
	InitLocations();
}

void Basic::ResourceManager::InitLocations()
{
	// give location aliases here:
}

std::shared_ptr<sf::Texture> Basic::ResourceManager::ITextureAcquire(const std::string& strInfo)
{
	if (m_Locations.find(strInfo) != m_Locations.end())
	{
		// location has alias in m_Locations

		if (m_Textures.find(m_Locations[strInfo]) != m_Textures.end())
		{
			// texture was loaded before

			return m_Textures[m_Locations[strInfo]];
		}
		else
		{
			// texture wasn't loaded before

			std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();

			if (texture->loadFromFile(m_Locations[strInfo]))
			{
				m_Textures.insert({ m_Locations[strInfo], texture });

				return texture;
			}
			else
			{
				std::cout << "Resource Manager Error: Cannot find given path: "<< m_Locations[strInfo] << "." << std::endl;

				return std::make_shared<sf::Texture>();
			}
		}
	}
	else
	{
		// location doesn't have alias in m_Locations

		if (m_Textures.find(strInfo) != m_Textures.end())
		{
			// texture was loaded before

			return m_Textures[strInfo];
		}
		else
		{
			// texture wasn't loaded before

			std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();

			if (texture->loadFromFile(strInfo))
			{
				m_Textures.insert({ strInfo, texture });

				return texture;
			}
			else
			{
				std::cout << "Resource Manager Error: Cannot find given path: " << strInfo << "." << std::endl;

				return std::make_shared<sf::Texture>();
			}
		}
	}
}

std::shared_ptr<sf::Texture> Basic::ResourceManager::TextureAcquire(const std::string& strInfo)
{
	return s_Instance.ITextureAcquire(strInfo);
}
