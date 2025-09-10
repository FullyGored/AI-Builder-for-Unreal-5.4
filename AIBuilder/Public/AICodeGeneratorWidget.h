// AICodeGeneratorWidget.h - Blueprint Widget for AI Code Generation
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableTextBox.h"
#include "AICodeGenerator.h"
#include "AICodeGeneratorWidget.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AIBUILDER_API UAICodeGeneratorWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UAICodeGeneratorWidget(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void NativeConstruct() override;

    // UI Components
    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* RequestTextBox;

    UPROPERTY(meta = (BindWidget))
    class UButton* GenerateButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* SaveButton;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* StatusText;

    UPROPERTY(meta = (BindWidget))
    class UMultiLineEditableTextBox* HeaderCodeBox;

    UPROPERTY(meta = (BindWidget))
    class UMultiLineEditableTextBox* SourceCodeBox;

    // AI Code Generator Reference
    UPROPERTY()
    class UAICodeGenerator* CodeGenerator;

    // Current Generated Code
    UPROPERTY()
    FGeneratedCode CurrentCode;

public:
    // Blueprint callable functions
    UFUNCTION(BlueprintCallable, Category = "AI Code Generator")
    void GenerateCodeFromInput();

    UFUNCTION(BlueprintCallable, Category = "AI Code Generator")
    void SaveGeneratedCode();

    UFUNCTION(BlueprintCallable, Category = "AI Code Generator")
    void ClearAll();

    // Example requests
    UFUNCTION(BlueprintCallable, Category = "AI Code Generator")
    void LoadExampleRequest(const FString& ExampleType);

protected:
    UFUNCTION()
    void OnGenerateButtonClicked();

    UFUNCTION()
    void OnSaveButtonClicked();

    void UpdateUI();
    void ShowStatus(const FString& Message, bool bIsError = false);

private:
    void InitializeCodeGenerator();
    FString GetDefaultSavePath() const;
};