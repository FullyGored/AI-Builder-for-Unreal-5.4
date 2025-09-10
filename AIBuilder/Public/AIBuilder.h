// AIBuilder.h - Main AI Builder Module
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "AICodeGenerator.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAIBuilder, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogAICodeGen, Log, All);

class AIBUILDER_API FAIBuilderModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    
    // AI Code Generation
    UFUNCTION(BlueprintCallable, Category = "AI Builder|Code Generation")
    static class UAICodeGenerator* GetCodeGenerator();
    
private:
    void RegisterComponents();
    void UnregisterComponents();
    void InitializeCodeGenerator();
    
    UPROPERTY()
    class UAICodeGenerator* CodeGenerator;
};