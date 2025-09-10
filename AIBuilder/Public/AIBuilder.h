// AIBuilder.h - Main AI Builder Module
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAIBuilder, Log, All);

class AIBUILDER_API FAIBuilderModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    
private:
    void RegisterComponents();
    void UnregisterComponents();
};