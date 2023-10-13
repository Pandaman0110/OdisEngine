#ifndef SCRIPT_H
#define SCRIPT_H

#include <iostream>
#include <string>

#include <mono/metadata/assembly.h>
#include <mono/jit/jit.h>

const std::string script_debug_path = "csharp/Debug/net6.0/";
const std::string script_release_path = "csharp/Release/net6.0/";

#include "Input.h"

class Script
{
public:
	Script();

private:
	MonoDomain* root_domain;
	MonoDomain* app_domain;

	MonoAssembly* mono_assembly;

	MonoObject* instantiate_class(const std::string& name_space, const std::string& name);
	void call_mono_method(MonoObject* object_instance, const std::string& method_name);

	MonoClass* get_class_in_assembly(MonoAssembly* assembly, const std::string& name_space, const std::string& name);

	MonoAssembly* load_assembly_from_file(const std::string& assembly_path);
	char* read_bytes(const std::string& file_path, uint32_t* out_size);
	void print_assembly_types(MonoAssembly* assembly);
};


#endif