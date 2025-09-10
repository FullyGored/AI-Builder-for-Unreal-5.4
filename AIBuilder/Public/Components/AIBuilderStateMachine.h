// AIBuilderStateMachine.h - State Machine Component
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIBuilderStateMachine.generated.h"

UENUM(BlueprintType)
enum class EAIBuilderState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Patrol      UMETA(DisplayName = "Patrol"),
    Chase       UMETA(DisplayName = "Chase"),
    Attack      UMETA(DisplayName = "Attack"),
    Search      UMETA(DisplayName = "Search"),
    Return      UMETA(DisplayName = "Return")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStateChanged, EAIBuilderState, OldState, EAIBuilderState, NewState);

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class AIBUILDER_API UAIBuilderStateMachine : public UActorComponent
{
    GENERATED_BODY()

public:
    UAIBuilderStateMachine();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    UPROPERTY(BlueprintAssignable, Category = "AI Builder|Events")
    FOnStateChanged OnStateChanged;

    UPROPERTY(BlueprintReadOnly, Category = "AI Builder|State")
    EAIBuilderState CurrentState;

    UPROPERTY(BlueprintReadOnly, Category = "AI Builder|State")
    EAIBuilderState PreviousState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Builder|Configuration")
    float StateTransitionDelay = 0.5f;

    // Blueprint callable functions
    UFUNCTION(BlueprintCallable, Category = "AI Builder")
    void ChangeState(EAIBuilderState NewState);

    UFUNCTION(BlueprintCallable, Category = "AI Builder")
    bool CanTransitionTo(EAIBuilderState NewState) const;

    UFUNCTION(BlueprintCallable, Category = "AI Builder")
    void Initialize(class AAIBuilderCharacter* InOwnerCharacter);

    UFUNCTION(BlueprintCallable, Category = "AI Builder")
    void UpdateState(float DeltaTime);

    UFUNCTION(BlueprintPure, Category = "AI Builder")
    FString GetCurrentStateName() const;

protected:
    UPROPERTY()
    class AAIBuilderCharacter* OwnerCharacter;

    float StateTimer;
    float LastTransitionTime;

    // State logic functions
    void UpdateIdleState(float DeltaTime);
    void UpdatePatrolState(float DeltaTime);
    void UpdateChaseState(float DeltaTime);
    void UpdateAttackState(float DeltaTime);
    void UpdateSearchState(float DeltaTime);
    void UpdateReturnState(float DeltaTime);

    // State transition functions
    void EnterState(EAIBuilderState NewState);
    void ExitState(EAIBuilderState OldState);

    // Utility functions
    bool HasValidTarget() const;
    float GetDistanceToTarget() const;
    bool IsInAttackRange() const;
};