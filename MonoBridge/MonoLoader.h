#pragma once
#include <string>
#include <vector>
#include <map>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

class MonoLoader
{
public:
	MonoLoader(std::string monoAssembliesPath, std::string monoConfigPath);
	void LoadAssembly(std::string path, bool useNewDomain = false);
	int ExecuteMainMethod(MonoAssembly* assembly, std::vector<std::string> arguments);
	void CacheMethods(std::string eventClassName, std::string eventName, std::string key);
	void RunEvent(std::string eventName);
	void SetNamespace(std::string Namespace);
	~MonoLoader();
private:
	std::map<std::string, std::map<MonoAssembly*, MonoMethod*>> cachedMethods;
	MonoDomain* domain;
	std::vector<MonoAssembly*> loadedAssemblies;
	std::string Namespace;
};

