
#ifndef _C1_EXECUTION_ENGINE_H_
#define _C1_EXECUTION_ENGINE_H_

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JITSymbol.h>
#include <llvm/ExecutionEngine/RuntimeDyld.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/Orc/LambdaResolver.h>
#include <llvm/ExecutionEngine/Orc/ObjectLinkingLayer.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/Mangler.h>
#include <llvm/Support/DynamicLibrary.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <string>
#include <unordered_map>

class execution_engine
{
  private:
    std::unique_ptr<llvm::TargetMachine> machine;
    const llvm::DataLayout layout;
    llvm::orc::ObjectLinkingLayer<> linking_layer;
    llvm::orc::IRCompileLayer<decltype(linking_layer)> compile_layer;
    std::unordered_map<std::string, llvm::JITTargetAddress> runtime_symbols;

  public:
    typedef decltype(compile_layer)::ModuleSetHandleT ModuleHandle;

    execution_engine(std::unordered_map<std::string, llvm::JITTargetAddress>);

    llvm::TargetMachine &get_target_machine() { return *machine; }

    ModuleHandle add_module(std::unique_ptr<llvm::Module>);

    llvm::JITSymbol find_symbol(const std::string);

    void remove_module(ModuleHandle);
};

#endif
