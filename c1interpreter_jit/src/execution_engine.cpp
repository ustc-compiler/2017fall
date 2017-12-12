
#include "execution_engine.h"

using namespace llvm;
using namespace orc;

execution_engine::execution_engine(std::unordered_map<std::string, JITTargetAddress> _runtime_symbols)
    : machine(EngineBuilder().selectTarget()),
      layout(machine->createDataLayout()),
      compile_layer(linking_layer, SimpleCompiler(*machine)),
      runtime_symbols(_runtime_symbols) {}

execution_engine::ModuleHandle execution_engine::add_module(std::unique_ptr<Module> m)
{
    auto resolver = createLambdaResolver(
        [&](const std::string &name) {
            if (auto symbol = compile_layer.findSymbol(name, false))
                return symbol;
            return JITSymbol(nullptr);
        },
        [&](const std::string &name) {
            if (runtime_symbols.count(name))
                return JITSymbol(runtime_symbols[name], JITSymbolFlags::Exported);
            return JITSymbol(nullptr);
        });

    std::vector<std::unique_ptr<Module>> modules;
    modules.push_back(std::move(m));

    return compile_layer.addModuleSet(std::move(modules),
                                      std::make_unique<SectionMemoryManager>(),
                                      std::move(resolver));
}

JITSymbol execution_engine::find_symbol(const std::string name)
{
    std::string mangled_name;
    raw_string_ostream mangled_name_stream(mangled_name);
    Mangler::getNameWithPrefix(mangled_name_stream, name, layout);
    return compile_layer.findSymbol(mangled_name_stream.str(), true);
}

void execution_engine::remove_module(execution_engine::ModuleHandle handle)
{
    compile_layer.removeModuleSet(handle);
}
