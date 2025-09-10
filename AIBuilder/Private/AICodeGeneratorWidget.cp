// AICodeGeneratorWidget.cpp - Blueprint Widget Implementation
#include "AICodeGeneratorWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableTextBox.h"
#include "AIBuilder.h"
#include "Misc/Paths.h"

UAICodeGeneratorWidget::UAICodeGeneratorWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    CodeGenerator = nullptr;
}

void UAICodeGeneratorWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    InitializeCodeGenerator();
    
    // Bind button events
    if (GenerateButton)
    {
        GenerateButton->OnClicked.AddDynamic(this, &UAICodeGeneratorWidget::OnGenerateButtonClicked);
    }
    
    if (SaveButton)
    {
        SaveButton->OnClicked.AddDynamic(this, &UAICodeGeneratorWidget::OnSaveButtonClicked);
    }
    
    // Set initial UI state
    UpdateUI();
    ShowStatus(TEXT("AI Code Generator ready. Enter your request and click Generate!"));
}

void UAICodeGeneratorWidget::InitializeCodeGenerator()
{
    if (!CodeGenerator)
    {
        CodeGenerator = NewObject<UAICodeGenerator>(this);
        if (CodeGenerator)
        {
            CodeGenerator->Initialize();
            UE_LOG(LogAICodeGen, Log, TEXT("Code Generator initialized in widget"));
        }
    }
}

void UAICodeGeneratorWidget::OnGenerateButtonClicked()
{
    GenerateCodeFromInput();
}

void UAICodeGeneratorWidget::OnSaveButtonClicked()
{
    SaveGeneratedCode();
}

void UAICodeGeneratorWidget::GenerateCodeFromInput()
{
    if (!CodeGenerator || !RequestTextBox)
    {
        ShowStatus(TEXT("Error: Code generator not initialized"), true);
        return;
    }
    
    FString UserRequest = RequestTextBox->GetText().ToString();
    if (UserRequest.IsEmpty())
    {
        ShowStatus(TEXT("Please enter a request"), true);
        return;
    }
    
    ShowStatus(TEXT("Generating code..."));
    
    // Generate code based on request type
    if (UserRequest.ToLower().Contains(TEXT("character")))
    {
        FString CharacterName = TEXT("MyAICharacter");
        CurrentCode = CodeGenerator->CreateAICharacter(CharacterName, UserRequest);
    }
    else if (UserRequest.ToLower().Contains(TEXT("task")))
    {
        FString TaskName = TEXT("MyBehaviorTask");
        CurrentCode = CodeGenerator->CreateBehaviorTreeTask(TaskName, UserRequest);
    }
    else
    {
        CurrentCode = CodeGenerator->GenerateCodeFromRequest(UserRequest);
    }
    
    UpdateUI();
    
    if (CurrentCode.bSuccess)
    {
        ShowStatus(FString::Printf(TEXT("Code generated successfully for %s!"), *CurrentCode.FileName));
    }
    else
    {
        ShowStatus(FString::Printf(TEXT("Code generation failed: %s"), *CurrentCode.ErrorMessage), true);
    }
}

void UAICodeGeneratorWidget::SaveGeneratedCode()
{
    if (!CurrentCode.bSuccess)
    {
        ShowStatus(TEXT("No valid code to save"), true);
        return;
    }
    
    FString SavePath = GetDefaultSavePath();
    if (CodeGenerator)
    {
        CodeGenerator->SaveGeneratedCode(CurrentCode, SavePath);
        ShowStatus(FString::Printf(TEXT("Code saved to %s"), *SavePath));
    }
}

void UAICodeGeneratorWidget::ClearAll()
{
    if (RequestTextBox)
    {
        RequestTextBox->SetText(FText::GetEmpty());
    }
    
    if (HeaderCodeBox)
    {
        HeaderCodeBox->SetText(FText::GetEmpty());
    }
    
    if (SourceCodeBox)
    {
        SourceCodeBox->SetText(FText::GetEmpty());
    }
    
    CurrentCode = FGeneratedCode();
    ShowStatus(TEXT("Cleared all content"));
}

void UAICodeGeneratorWidget::LoadExampleRequest(const FString& ExampleType)
{
    FString ExampleRequest;
    
    if (ExampleType == TEXT("Character"))
    {
        ExampleRequest = TEXT("Create a guard character that patrols between waypoints and chases enemies when detected");
    }
    else if (ExampleType == TEXT("Task"))
    {
        ExampleRequest = TEXT("Create a behavior tree task that makes the AI search for cover when health is low");
    }
    else if (ExampleType == TEXT("Controller"))
    {
        ExampleRequest = TEXT("Create an AI controller with advanced perception and decision making");
    }
    
    if (RequestTextBox && !ExampleRequest.IsEmpty())
    {
        RequestTextBox->SetText(FText::FromString(ExampleRequest));
        ShowStatus(FString::Printf(TEXT("Loaded example: %s"), *ExampleType));
    }
}

void UAICodeGeneratorWidget::UpdateUI()
{
    if (HeaderCodeBox)
    {
        HeaderCodeBox->SetText(FText::FromString(CurrentCode.HeaderCode));
    }
    
    if (SourceCodeBox)
    {
        SourceCodeBox->SetText(FText::FromString(CurrentCode.SourceCode));
    }
    
    // Enable/disable save button based on code validity
    if (SaveButton)
    {
        SaveButton->SetIsEnabled(CurrentCode.bSuccess);
    }
}

void UAICodeGeneratorWidget::ShowStatus(const FString& Message, bool bIsError)
{
    if (StatusText)
    {
        StatusText->SetText(FText::FromString(Message));
        
        // You could change text color based on error state
        FLinearColor TextColor = bIsError ? FLinearColor::Red : FLinearColor::White;
        StatusText->SetColorAndOpacity(FSlateColor(TextColor));
    }
    
    UE_LOG(LogAICodeGen, Log, TEXT("Status: %s"), *Message);
}

FString UAICodeGeneratorWidget::GetDefaultSavePath() const
{
    return FPaths::ProjectDir() / TEXT("Source") / TEXT("Generated");
}