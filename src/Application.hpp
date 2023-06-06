#pragma once

namespace cjv
{
struct Application
{
	Application() = default;

	int run(const int argc, const char* argv[]);

private:
	bool validate() const;

	std::string m_schemaFile;
	StringList m_files;
};
}
