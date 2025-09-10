// AIBuilderController.h - Custom AI Controller
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIBuilderController.generated.h"

UCLASS()
class AIBUILDER_API AAIBuilderController : public AAIController
{
    GENERATED_BODY()

public:
    AAIBuilderController();

protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;

public:
    // AI Configuration
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Builder")
    class UBehaviorTree* DefaultBehaviorTree;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Builder")
    class UBlackboardAsset* DefaultBlackboard;

    // Blueprint callable functions
    UFUNCTION(BlueprintCallable, Category = "AI Builder")
    void StartAI();

    UFUNCTION(BlueprintCallable, Category = "AI Builder")
    void StopAI();

    UFUNCTION(BlueprintCallable, Category = "AI Builder")
    void PauseAI();

    UFUNCTION(BlueprintCallable, Category = "AI Builder")
    void ResumeAI();

    UFUNCTION(BlueprintCallable, Category = "AI Builder")
    bool IsAIRunning() const;

protected:
    virtual void SetupPerceptionSystem();
    virtual void ConfigureBlackboard();

    UFUNCTION()
    virtual void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:
    bool bAIStarted;
    bool bAIPaused;

    void InitializeComponents();
    void LogAIStatus(const FString& Status) const;
};