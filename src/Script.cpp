#include "Script.h"

#include <fstream>

using namespace OdisEngine;

Script::Script()
{
	mono_set_dirs("Mono/lib", "Mono/etc");
    
    root_domain = mono_jit_init("OdisEngine");
    /*
    auto path = script_debug_path + "OdisEngine.exe";
    MonoAssembly* assembly = mono_domain_assembly_open(root_domain, path.c_str());

    if (!assembly)
        std::cout << "HERELP" << std::endl;

    int argc = 1;
    char* argv[10] = { (char*) "OdisEngine" };

    int return_value = mono_jit_exec(root_domain, assembly, argc, argv + 1);

    std::cout << return_value << std::endl;
    */
    
    mono_assembly = load_assembly_from_file(script_debug_path + "OdisEngine.dll");
    print_assembly_types(mono_assembly);
    
    MonoObject* test = instantiate_class("OdisEngine", "Game");
    call_mono_method(test, "print_float_var");
}

MonoClass* Script::get_class_in_assembly(MonoAssembly* assembly, const std::string& name_space, const std::string& name)
{
    MonoImage* image = mono_assembly_get_image(assembly);
    MonoClass* assembly_class = mono_class_from_name(image, name_space.c_str(), name.c_str());

    if (assembly_class == nullptr)
    {
        std::cout << "CLASS NOT FOUND " << std::endl;
        std::abort();
    }

    return assembly_class;
}

MonoObject* Script::instantiate_class(const std::string& name_space, const std::string& name)
{
    MonoClass* class_instance = get_class_in_assembly(mono_assembly, name_space, name);

    MonoObject* instance = mono_object_new(root_domain, class_instance);

    if (instance == nullptr)
    {
        // Log error here and abort
    }

    mono_runtime_object_init(instance);

    return instance;
}

void Script::call_mono_method(MonoObject* object_instance, const std::string& method_name)
{
    MonoClass* class_instance = mono_object_get_class(object_instance);

    MonoMethod* method = mono_class_get_method_from_name(class_instance, method_name.c_str(), 0);

    if (method == nullptr)
    {
        return;
    }

    MonoObject* exception = nullptr;
    mono_runtime_invoke(method, object_instance, nullptr, &exception);

    // TODO: Handle the exception
}

MonoAssembly* Script::load_assembly_from_file(const std::string& assembly_path)
{
    //TODO make sure this actually exists somewhere
    uint32_t file_size = 0;
    char* file_data = read_bytes(assembly_path, &file_size);

    // NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
    MonoImageOpenStatus status;
    MonoImage* image = mono_image_open_from_data_full(file_data, file_size, 1, &status, 0);

    if (status != MONO_IMAGE_OK)
    {
        std::cout << mono_image_strerror(status) << std::endl;
        return nullptr;
    }

    MonoAssembly* assembly = mono_assembly_load_from_full(image, assembly_path.c_str(), &status, 0);
    mono_image_close(image);

    // Don't forget to free the file data
    delete[] file_data;

    return assembly;
}

char* Script::read_bytes(const std::string& file_path, uint32_t* out_size)
{
    std::ifstream stream(file_path, std::ios::binary | std::ios::ate);

    if (!stream)
        return nullptr;

    std::streampos end = stream.tellg();
    stream.seekg(0, std::ios::beg);
    uint32_t size = end - stream.tellg();

    if (size == 0)
    {
        // File is empty
        return nullptr;
    }

    char* buffer = new char[size];
    stream.read((char*)buffer, size);
    stream.close();

    *out_size = size;
    return buffer;
}

void Script::print_assembly_types(MonoAssembly* assembly)
{
    MonoImage* image = mono_assembly_get_image(assembly);
    const MonoTableInfo* type_definitions_table = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
    size_t num_types = mono_table_info_get_rows(type_definitions_table);

    for (int row = 0; row < num_types; row++)
    {
        uint32_t columns[MONO_TYPEDEF_SIZE];
        mono_metadata_decode_row(type_definitions_table, row, columns, MONO_TYPEDEF_SIZE);

        std::string name_space = mono_metadata_string_heap(image, columns[MONO_TYPEDEF_NAMESPACE]);
        std::string name = mono_metadata_string_heap(image, columns[MONO_TYPEDEF_NAME]);

        std::cout << name_space << "::" << name << "\n";
    }
    std::cout << std::endl;
}