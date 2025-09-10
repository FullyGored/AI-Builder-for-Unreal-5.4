// AIBuilderCharacter.h - Main AI Character Class
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/AIPerceptionComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIBuilderStateMachine.h"
#include "AIBuilderSensorComponent.h"
#include "AIBuilderCharacter.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AIBUILDER_API AAIBuilderCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AAIBuilderCharacter();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // AI Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Builder|Components")
    class UAIPerceptionComponent* AIPerceptionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Builder|Components")
    class UBlackboardComponent* BlackboardComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Builder|Components")
    class UBehaviorTreeComponent* BehaviorTreeComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Builder|Components")
    class UAIBuilderSensorComponent* SensorComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Builder|Components")
    class UAIBuilderStateMachine* StateMachine;

    // AI Configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Builder|Configuration")
    class UBehaviorTree* BehaviorTree;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Builder|Configuration")
    class UBlackboardAsset* BlackboardAsset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Builder|Configuration")
    float SightRadius = 1500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Builder|Configuration")
    float LoseSightRadius = 1600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Builder|Configuration")
    float PeripheralVisionAngleDegrees = 90.0f;

    // AI Behavior
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Builder|Behavior")
    float MovementSpeed = 400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Builder|Behavior")
    float AttackRange = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Builder|Behavior")
    float PatrolRadius = 1000.0f;

public:
    // Blueprint callable functions
    UFUNCTION(BlueprintCallable, Category = "AI Builder")
    void SetMovementSpeed(float NewSpeed);

    UFUNCTION(BlueprintCallable, Category = "AI Builder")
    void StartBehaviorTree();

    UFUNCTION(BlueprintCallable, Category = "AI Builder")
    void StopBehaviorTree();

    UFUNCTION(BlueprintCallable, Category = "AI Builder")
    AActor* GetCurrentTarget() const;

    UFUNCTION(BlueprintCallable, Category = "AI Builder")
    void SetCurrentTarget(AActor* NewTarget);

protected:
    // Perception callbacks
    UFUNCTION()
    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

    UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
    void InitializeAI();
    void SetupPerception();
    void UpdateAIState(float DeltaTime);

    UPROPERTY()
    AActor* CurrentTarget;

    float LastUpdateTime;
};