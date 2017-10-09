#pragma once
#include <string>
#include <vector>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

class MonoLoader
{
public:
	MonoLoader(std::string monoAssembliesPath, std::string monoConfigPath);
	void LoadAssembly(std::string path, bool useNewDomain = false);
	int ExecuteMainMethod(MonoAssembly* assembly, std::vector<std::string> arguments);
	void ExecuteEventInAllAssemblies(std::string eventClassName, std::string eventName);
	~MonoLoader();
private:
	MonoDomain* domain;
	std::vector<MonoAssembly*> loadedAssemblies;
};

