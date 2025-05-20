#pragma once
#include <glm/gtx/matrix_decompose.hpp>


namespace Tank
{
	namespace mat4
	{
		inline glm::quat getRotation(const glm::mat4 &mat)
		{
			glm::vec3 _;
			glm::vec4 __;
			glm::quat rot;
			glm::decompose(mat, _, rot, _, _, __);
			return rot;
		}

		inline const glm::vec3 getScale(const glm::mat4 &mat)
		{
			return { mat[0][0], mat[1][1], mat[2][2] };
		}
		inline void setScale(glm::mat4 &mat, const glm::vec3 &scale)
		{
			mat[0][0] = scale[0]; mat[1][1] = scale[1]; mat[2][2] = scale[2];
		}
		inline void copyScale(const glm::mat4 &src, glm::mat4 &dest)
		{
			setScale(dest, getScale(src));
		}

		inline const glm::vec3 getTranslation(const glm::mat4 &mat)
		{
			return glm::vec3(mat[3]);
		}
		inline void setTranslation(glm::mat4 &mat, const glm::vec3 &trans)
		{
			mat[3] = glm::vec4(trans, 1.0f);
		}
		inline void copyTranslation(const glm::mat4 &src, glm::mat4 &dest)
		{
			setTranslation(dest, getTranslation(src));
		}

		inline glm::mat4 rotateAboutPoint(glm::vec3 point, glm::vec3 origin, glm::mat4 rot)
		{
			glm::mat4 T_PO = glm::translate(glm::mat4(1.0f), origin - point);
			glm::mat4 T_OP = glm::translate(glm::mat4(1.0f), point - origin);
			return T_OP * rot * T_PO;
		}
	}
}