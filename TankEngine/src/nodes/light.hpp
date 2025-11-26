#pragma once
#include <nodes/interfaces/editor_only.hpp>


namespace Tank
{
	class Model;
	class Shader;
	class Scene;
	struct ShaderSources;
	class ShaderSource;

	enum class LightType
	{
		Any,
		Point,
		Directional
	};

	class TANK_API Light : public IEditorOnly
	{
	public:
		virtual json serialise() override;
		virtual void deserialise(const json &ser) override;

	protected:
		// The scene this light applies to shaders within. Responsibility lies in the Light class
		// to call m_scene->updateShaders() after changes to the light occur.
		Scene *m_scene;
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		// Name of the array in GLSL containing all light structs.
		std::string m_lightArrayName;
		Light(
			const std::string &name = "Light",
			glm::vec3 amb = { 1,1,1 },
			glm::vec3 diff = { 1,1,1 },
			glm::vec3 spec = { 1,1,1 }
		);
	public:
		// Virtual destructor so derived class destructors are always used.
		virtual ~Light();

		virtual void updateShader(const Shader &);
		std::string getLightStruct();

		void setAmbient(glm::vec3 ambient) { m_ambient = ambient; }
		glm::vec3 getAmbient() const { return m_ambient; }
		
		void setDiffuse(glm::vec3 diffuse) { m_diffuse = diffuse; }
		glm::vec3 getDiffuse() const { return m_diffuse; }
		
		void setSpecular(glm::vec3 specular) { m_specular = specular; }
		glm::vec3 getSpecular() const { return m_specular; }

		LightType getType();
	};


	class TANK_API DirLight : public Light
	{
	public:
		virtual json serialise() override;
		virtual void deserialise(const json &ser) override;

	private:
		glm::vec3 m_direction;
	public:
		DirLight(const std::string &name = "DirLight",
			glm::vec3 dir = { 0.0f, -1.0f, 0.0f },
			glm::vec3 amb = { 0.02f, 0.02f, 0.02f },
			glm::vec3 diff = { 0.2f, 0.2f, 0.2f },
			glm::vec3 spec = { 0.1f, 0.1f, 0.1f }
		);
		~DirLight();

		void updateShader(const Shader &) override;

		void setDirection(glm::vec3 direction) { m_direction = direction; }
		glm::vec3 getDirection() const { return m_direction; }
	};


	class TANK_API PointLight : public Light
	{
	public:
		PointLight(const std::string &name = "PointLight",
			glm::vec3 amb = { 0.1f, 0.1f, 0.1f },
			glm::vec3 diff = { 0.1f, 0.1f, 0.1f },
			glm::vec3 spec = { 0.1f, 0.1f, 0.1f });
		~PointLight();

		void updateShader(const Shader &) override;
	};
}