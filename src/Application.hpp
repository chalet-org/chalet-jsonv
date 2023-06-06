#pragma once

namespace cjv
{
struct Application
{
	Application() = default;

	i32 run(const i32 argc, const char* argv[]);

private:
	bool validate() const;

	std::string m_schemaFile;
	StringList m_files;
};
}
