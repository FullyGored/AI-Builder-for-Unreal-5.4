// AIBuilder.cpp - Main AI Builder Module Implementation
#include "AIBuilder.h"
#include "Modules/ModuleManager.h"
#include "Engine/Engine.h"
#include "AICodeGenerator.h"

DEFINE_LOG_CATEGORY(LogAIBuilder);
DEFINE_LOG_CATEGORY(LogAICodeGen);

UAICodeGenerator* FAIBuilderModule::CodeGenerator = nullptr;

void FAIBuilderModule::StartupModule()
{
    UE_LOG(LogAIBuilder, Log, TEXT("AI Builder Module Starting Up"));
    RegisterComponents();
    InitializeCodeGenerator();
}

void FAIBuilderModule::ShutdownModule()
{
    UE_LOG(LogAIBuilder, Log, TEXT("AI Builder Module Shutting Down"));
    UnregisterComponents();
    
    if (CodeGenerator)
    {
        CodeGenerator = nullptr;
    }
}

void FAIBuilderModule::RegisterComponents()
{
    // Register custom AI components here
    UE_LOG(LogAIBuilder, Log, TEXT("Registering AI Builder Components"));
}

void FAIBuilderModule::UnregisterComponents()
{
    // Unregister components here
    UE_LOG(LogAIBuilder, Log, TEXT("Unregistering AI Builder Components"));
}

void FAIBuilderModule::InitializeCodeGenerator()
{
    CodeGenerator = NewObject<UAICodeGenerator>();
    if (CodeGenerator)
    {
        CodeGenerator->Initialize();
        UE_LOG(LogAICodeGen, Log, TEXT("AI Code Generator initialized"));
    }
}

UAICodeGenerator* FAIBuilderModule::GetCodeGenerator()
{
    return CodeGenerator;
}

IMPLEMENT_MODULE(FAIBuilderModule, AIBuilder)