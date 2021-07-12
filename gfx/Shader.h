#pragma once

namespace gfx
{
	class Shader : public GLObject
	{
	public:
		Shader(const char* fp)
		{
			// This can't be set at boot time because gl won't have been bound yet.
			// Set it in the first invocation of this constructor; if the user is
			// calling this before calling gfx::init, that's their fault.
			if (!s_Macros.contains("%MAX_TEXTURES%"))
				s_Macros.insert({ "%MAX_TEXTURES%", std::to_string(getMaxTextureUnits()) });

			m_Id = glCreateProgram();
			// split input file into vertex and fragment components
			ShaderSources sources = Parse(fp);
			// compile each component separately
			uint vertex = Compile(sources.vertex, GL_VERTEX_SHADER);
			uint fragment = Compile(sources.fragment, GL_FRAGMENT_SHADER);
			// attach compiled shaders
			glAttachShader(m_Id, vertex);
			glAttachShader(m_Id, fragment);
			// finalize program
			glLinkProgram(m_Id);
			glValidateProgram(m_Id);
			// delete shaders
			glDeleteShader(vertex);
			glDeleteShader(fragment);
		}
		Shader(const Shader& other) = delete;
		Shader(Shader&& other) = delete;
		~Shader()
		{
			glDeleteProgram(m_Id);
		}


		void Bind() const override
		{
			glUseProgram(m_Id);
		}
		void Unbind() const override
		{
			glUseProgram(0);
		}
		void SetUniform1i(const std::string& name, int i)
		{
			SetUniform(glUniform1i, name, i);
		}
		void SetUniform1f(const std::string& name, float f)
		{
			SetUniform(glUniform1f, name, f);
		}
		void SetUniform2f(const std::string& name, float x, float y)
		{
			SetUniform(glUniform2f, name, x, y);
		}
		void SetUniform3f(const std::string& name, float x, float y, float z)
		{
			SetUniform(glUniform3f, name, x, y, z);
		}
		void SetUniform1iv(const std::string& name, uint length, const int* const data)
		{
			SetUniform(glUniform1iv, name, length, data);
		}
		void SetUniformBlock(const std::string& name, const UniformBuffer& buffer)
		{
			int location = GetUniformBlockLocation(name);
			if (location == -1)
				return;

			Bind();
			buffer.Bind();
			glUniformBlockBinding(m_Id, location, buffer.GetSlot());
		}
	private:
		struct ShaderSources
		{
			std::string vertex, fragment;
		};


		static inline std::unordered_map<std::string, std::string> s_Macros;
		std::unordered_map<std::string, int> m_UniformCache;


		// generic wrapper function for all glUniform* calls
		template<typename T, typename ... Args>
		void SetUniform(T fn, const std::string& name, Args& ... args)
		{
			Bind();
			fn(GetUniformLocation(name), args...);
		}
		int GetUniformBlockLocation(const std::string& name)
		{
			return GetLocation(glGetUniformBlockIndex, name);
		}
		int GetUniformLocation(const std::string& name)
		{
			return GetLocation(glGetUniformLocation, name);
		}
		template<typename T>
		int GetLocation(T fn, const std::string& name)
		{
			// if we've already found a uniform with given name, return cached value
			auto cached = m_UniformCache.find(name);
			if (cached != m_UniformCache.end())
				return cached->second;

			// otherwise, search for it
			int location = fn(m_Id, name.c_str());
			// doesn't exist, don't cache it
			if (location == -1)
			{
				printf("Uniform '%s' not found\n", name.c_str());
				return -1;
			}

			// exists, cache and return it
			m_UniformCache[name] = location;
			return location;
		}
		ShaderSources Parse(const char* fp)
		{
			std::ifstream in(fp);
			// file doesn't exist
			if (!in.is_open())
			{
				printf("Error opening shader file '%s', aborting...\n", fp);
				return { "" , "" };
			}

			// stores current line of the file
			std::string line;
			// 0 - vertex shader, 1 - fragment shader
			std::stringstream streams[2];
			uint currentType = 0;

			// read the file a line at a time
			while (getline(in, line))
			{
				// custom #type directive to specify shader type
				if (line.find("#type") != std::string::npos)
				{
					if (line.find("vertex") != std::string::npos)
						currentType = 0;
					else if (line.find("fragment") != std::string::npos)
						currentType = 1;
					// we only support vertex + fragment shaders
					else
					{
						printf("Invalid shader type on line '%s', aborting...\n", line.c_str());
						return { "", "" };
					}
				}
				// current line is regular shader code, append it to its stream
				else
				{
					for (auto& pair : s_Macros)
						line = std::regex_replace(line, std::regex(pair.first), pair.second);
					streams[currentType] << line << '\n';
				}
			}

			in.close();
			return { streams[0].str(), streams[1].str() };
		}
		uint Compile(const std::string& src, GLenum type)
		{
			uint id = glCreateShader(type);
			const char* ptr = src.c_str();
			glShaderSource(id, 1, &ptr, nullptr);
			glCompileShader(id);

			// check for compiler error
			int result;
			glGetShaderiv(id, GL_COMPILE_STATUS, &result);
			if (!result)
			{
				printf("%s shader compilation failed, aborting...\n", (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"));
				printf("Source:\n%s\n", src.c_str());

				// get and print error message
				int length;
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
				char* buffer = new char[length];
				glGetShaderInfoLog(id, length, nullptr, buffer);
				printf(buffer);
				delete[] buffer;

				glDeleteShader(id);
				return 0;
			}

			return id;
		}
	};
}
