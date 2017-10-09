#include "MonoLoader.h"
#include <stdexcept>
#include <mono\metadata\class.h>

MonoLoader::MonoLoader(std::string monoAssembliesPath, std::string monoConfigPath)
{
	Namespace = "Placeholder";
	domain = mono_jit_init("TestDomain");
	mono_set_dirs(monoAssembliesPath.c_str(), monoConfigPath.c_str());
}

void MonoLoader::LoadAssembly(std::string path, bool useNewDomain)
{
	auto assembly = mono_domain_assembly_open(domain, path.c_str());
	if (!assembly) {
		throw std::runtime_error("Failed to load assembly");
	}
	loadedAssemblies.push_back(assembly);
}

int MonoLoader::ExecuteMainMethod(MonoAssembly * assembly, std::vector<std::string> arguments)
{
	const char** argv = new const char*[arguments.size() + 2];
	argv[0] = mono_assembly_name_get_name(mono_assembly_get_name(assembly));
	int i = 1;
	for (auto val : arguments) {
		argv[i] = val.c_str();
		i++;
	}
	argv[i] = NULL;
	//Null gehört nicht zu argc
	int retval = mono_jit_exec(domain, assembly, i, (char**)argv);
	delete[]argv;
	return retval;
}

void MonoLoader::ExecuteEventInAllAssemblies(std::string eventClassName, std::string eventName)
{
	//TODO: Add image cache
	for (auto assembly : loadedAssemblies) 
	{
		auto image = mono_assembly_get_image(assembly);
		auto EmbeddedClass = mono_class_from_name(image, Namespace.c_str(), eventClassName.c_str());
		auto Method = mono_class_get_method_from_name(EmbeddedClass, eventName.c_str(), 0);
		void * params[1] = { NULL };
		mono_runtime_invoke(Method, NULL, params, NULL);
	}
}

void MonoLoader::SetNamespace(std::string Namespace)
{
	this->Namespace = Namespace;
}

MonoLoader::~MonoLoader()
{
	mono_jit_cleanup(domain);
}
