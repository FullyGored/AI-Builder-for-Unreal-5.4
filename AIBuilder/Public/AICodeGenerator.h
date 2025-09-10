// AICodeGenerator.h - AI Assistant for C++ Code Generation
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "AICodeGenerator.generated.h"

USTRUCT(BlueprintType)
struct FCodeRequest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString UserRequest;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ClassName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BaseClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> RequiredIncludes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Properties;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Functions;

    FCodeRequest()
    {
        UserRequest = TEXT("");
        ClassName = TEXT("");
        BaseClass = TEXT("UObject");
    }
};

USTRUCT(BlueprintType)
struct FGeneratedCode
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HeaderCode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SourceCode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FileName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSuccess;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ErrorMessage;

    FGeneratedCode()
    {
        HeaderCode = TEXT("");
        SourceCode = TEXT("");
        FileName = TEXT("");
        bSuccess = false;
        ErrorMessage = TEXT("");
    }
};

UCLASS(BlueprintType, Blueprintable)
class AIBUILDER_API UAICodeGenerator : public UObject
{
    GENERATED_BODY()

public:
    UAICodeGenerator();

    // Main AI Code Generation Function
    UFUNCTION(BlueprintCallable, Category = "AI Code Generator")
    FGeneratedCode GenerateCodeFromRequest(const FString& UserRequest);

    // Specific Code Generation Functions
    UFUNCTION(BlueprintCallable, Category = "AI Code Generator")
    FGeneratedCode CreateAICharacter(const FString& CharacterName, const FString& BehaviorDescription);

    UFUNCTION(BlueprintCallable, Category = "AI Code Generator")
    FGeneratedCode CreateAIController(const FString& ControllerName, const FString& ControllerType);

    UFUNCTION(BlueprintCallable, Category = "AI Code Generator")
    FGeneratedCode CreateBehaviorTreeTask(const FString& TaskName, const FString& TaskDescription);

    UFUNCTION(BlueprintCallable, Category = "AI Code Generator")
    FGeneratedCode CreateAIComponent(const FString& ComponentName, const FString& ComponentPurpose);

    // Utility Functions
    UFUNCTION(BlueprintCallable, Category = "AI Code Generator")
    void SaveGeneratedCode(const FGeneratedCode& Code, const FString& OutputPath);

    UFUNCTION(BlueprintCallable, Category = "AI Code Generator")
    TArray<FString> GetAvailableTemplates() const;

    UFUNCTION(BlueprintCallable, Category = "AI Code Generator")
    void Initialize();

protected:
    // Natural Language Processing
    FCodeRequest ParseUserRequest(const FString& UserRequest);
    FString ExtractClassName(const FString& Request);
    FString DetermineBaseClass(const FString& Request);
    TArray<FString> ExtractRequiredFeatures(const FString& Request);

    // Code Generation Templates
    FString GenerateHeaderTemplate(const FCodeRequest& Request);
    FString GenerateSourceTemplate(const FCodeRequest& Request);
    FString GenerateClassDeclaration(const FCodeRequest& Request);
    FString GenerateConstructor(const FCodeRequest& Request);
    FString GenerateFunctions(const FCodeRequest& Request);
    FString GenerateProperties(const FCodeRequest& Request);

    // AI-Specific Code Generation
    FString GenerateAICharacterCode(const FString& CharacterName, const FString& BehaviorDescription);
    FString GenerateAIControllerCode(const FString& ControllerName, const FString& ControllerType);
    FString GenerateBehaviorTreeTaskCode(const FString& TaskName, const FString& TaskDescription);
    FString GenerateAIComponentCode(const FString& ComponentName, const FString& ComponentPurpose);

    // Code Validation and Formatting
    bool ValidateGeneratedCode(const FString& Code);
    FString FormatCode(const FString& Code);
    FString AddIncludes(const TArray<FString>& Includes);
    FString AddNamespaces();

private:
    // Template Storage
    TMap<FString, FString> HeaderTemplates;
    TMap<FString, FString> SourceTemplates;
    TMap<FString, FString> CodeSnippets;

    // AI Keywords and Patterns
    TMap<FString, FString> AIKeywords;
    TMap<FString, FString> ClassTypePatterns;
    TMap<FString, TArray<FString>> RequiredIncludesMap;

    void InitializeTemplates();
    void InitializeAIKeywords();
    void InitializeCodeSnippets();

    // Helper Functions
    FString ReplaceTemplateVariables(const FString& Template, const FCodeRequest& Request);
    bool ContainsKeyword(const FString& Text, const FString& Keyword);
    FString CapitalizeFirstLetter(const FString& Input);
    FString GenerateUniqueClassName(const FString& BaseName);
};