// AIBuilder.cpp - Main AI Builder Module Implementation
#include "AIBuilder.h"
#include "Modules/ModuleManager.h"
#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY(LogAIBuilder);

void FAIBuilderModule::StartupModule()
{
    UE_LOG(LogAIBuilder, Log, TEXT("AI Builder Module Starting Up"));
    RegisterComponents();
}

void FAIBuilderModule::ShutdownModule()
{
    UE_LOG(LogAIBuilder, Log, TEXT("AI Builder Module Shutting Down"));
    UnregisterComponents();
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

IMPLEMENT_MODULE(FAIBuilderModule, AIBuilder)