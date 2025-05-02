
#include "nodes/node.hpp"


namespace Tank
{
	class Model;
	class Shader;
	class Scene;
	class Light : public Node
	{
	public:
		static json serialise(Light *light);
		static void deserialise(const json &serialised, Light **targetPtr);

	protected:
		// The scene this light applies to shaders within. Responsibility lies in the Light class
		// to call m_scene->updateShaders() after changes to the light occur.
		Scene *m_scene;
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		int m_index;
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

		virtual void updateShader(Shader *);
		std::string getLightStruct() { return m_lightArrayName + "[" + std::to_string(m_index) + "]"; };

		void setAmbient(glm::vec3 ambient) { m_ambient = ambient; }
		glm::vec3 getAmbient() const { return m_ambient; }
		
		void setDiffuse(glm::vec3 diffuse) { m_diffuse = diffuse; }
		glm::vec3 getDiffuse() const { return m_diffuse; }
		
		void setSpecular(glm::vec3 specular) { m_specular = specular; }
		glm::vec3 getSpecular() const { return m_specular; }
	};


	class DirLight : public Light
	{
	public:
		static json serialise(DirLight *light);
		static void deserialise(const json &serialised, DirLight **targetPtr);

	private:
		glm::vec3 m_direction;
	public:
		DirLight(const std::string &name = "DirLight",
			glm::vec3 dir = { 0,0,1 },
			glm::vec3 amb = { 1,1,1 },
			glm::vec3 diff = { 1,1,1 },
			glm::vec3 spec = { 1,1,1 });
		~DirLight();

		void updateShader(Shader *) override;

		void setDirection(glm::vec3 direction) { m_direction = direction; }
		glm::vec3 getDirection() const { return m_direction; }
	};


	class PointLight : public Light
	{
	public:
		PointLight(const std::string &name = "PointLight",
			glm::vec3 amb = { 1,1,1 },
			glm::vec3 diff = { 1,1,1 },
			glm::vec3 spec = { 1,1,1 });
		~PointLight();

		void updateShader(Shader *) override;
	};
}