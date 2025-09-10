// AIBuilderStateMachine.cpp - State Machine Implementation
#include "Components/AIBuilderStateMachine.h"
#include "Core/AIBuilderCharacter.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "AIBuilder.h"
#include "Engine/Engine.h"

UAIBuilderStateMachine::UAIBuilderStateMachine()
{
    PrimaryComponentTick.bCanEverTick = false;
    CurrentState = EAIBuilderState::Idle;
    PreviousState = EAIBuilderState::Idle;
    StateTimer = 0.0f;
    LastTransitionTime = 0.0f;
}

void UAIBuilderStateMachine::BeginPlay()
{
    Super::BeginPlay();
    EnterState(CurrentState);
}

void UAIBuilderStateMachine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAIBuilderStateMachine::Initialize(AAIBuilderCharacter* InOwnerCharacter)
{
    OwnerCharacter = InOwnerCharacter;
    UE_LOG(LogAIBuilder, Log, TEXT("State Machine initialized for %s"), 
           OwnerCharacter ? *OwnerCharacter->GetName() : TEXT("Unknown"));
}

void UAIBuilderStateMachine::UpdateState(float DeltaTime)
{
    if (!OwnerCharacter)
    {
        return;
    }

    StateTimer += DeltaTime;

    // Update current state logic
    switch (CurrentState)
    {
        case EAIBuilderState::Idle:
            UpdateIdleState(DeltaTime);
            break;
        case EAIBuilderState::Patrol:
            UpdatePatrolState(DeltaTime);
            break;
        case EAIBuilderState::Chase:
            UpdateChaseState(DeltaTime);
            break;
        case EAIBuilderState::Attack:
            UpdateAttackState(DeltaTime);
            break;
        case EAIBuilderState::Search:
            UpdateSearchState(DeltaTime);
            break;
        case EAIBuilderState::Return:
            UpdateReturnState(DeltaTime);
            break;
    }
}

void UAIBuilderStateMachine::ChangeState(EAIBuilderState NewState)
{
    if (CurrentState == NewState || !CanTransitionTo(NewState))
    {
        return;
    }

    // Check transition delay
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime - LastTransitionTime < StateTransitionDelay)
    {
        return;
    }

    EAIBuilderState OldState = CurrentState;
    ExitState(CurrentState);
    
    PreviousState = CurrentState;
    CurrentState = NewState;
    
    EnterState(NewState);
    LastTransitionTime = CurrentTime;
    
    OnStateChanged.Broadcast(OldState, NewState);
    
    UE_LOG(LogAIBuilder, Log, TEXT("%s: State changed from %s to %s"), 
           *OwnerCharacter->GetName(), 
           *UEnum::GetValueAsString(OldState), 
           *UEnum::GetValueAsString(NewState));
}

bool UAIBuilderStateMachine::CanTransitionTo(EAIBuilderState NewState) const
{
    // Define state transition rules here
    switch (CurrentState)
    {
        case EAIBuilderState::Idle:
            return NewState == EAIBuilderState::Patrol || 
                   NewState == EAIBuilderState::Chase;
        
        case EAIBuilderState::Patrol:
            return NewState == EAIBuilderState::Idle || 
                   NewState == EAIBuilderState::Chase ||
                   NewState == EAIBuilderState::Search;
        
        case EAIBuilderState::Chase:
            return NewState == EAIBuilderState::Attack || 
                   NewState == EAIBuilderState::Search ||
                   NewState == EAIBuilderState::Patrol;
        
        case EAIBuilderState::Attack:
            return NewState == EAIBuilderState::Chase || 
                   NewState == EAIBuilderState::Search;
        
        case EAIBuilderState::Search:
            return NewState == EAIBuilderState::Patrol || 
                   NewState == EAIBuilderState::Chase ||
                   NewState == EAIBuilderState::Return;
        
        case EAIBuilderState::Return:
            return NewState == EAIBuilderState::Patrol || 
                   NewState == EAIBuilderState::Idle;
    }
    
    return false;
}

FString UAIBuilderStateMachine::GetCurrentStateName() const
{
    return UEnum::GetValueAsString(CurrentState);
}

void UAIBuilderStateMachine::UpdateIdleState(float DeltaTime)
{
    // Check if we should start patrolling
    if (StateTimer > 2.0f)
    {
        ChangeState(EAIBuilderState::Patrol);
    }
    
    // Check for targets
    if (HasValidTarget())
    {
        ChangeState(EAIBuilderState::Chase);
    }
}

void UAIBuilderStateMachine::UpdatePatrolState(float DeltaTime)
{
    // Check for targets
    if (HasValidTarget())
    {
        ChangeState(EAIBuilderState::Chase);
        return;
    }
    
    // Continue patrolling logic would be handled by behavior tree
    // This is just state management
}

void UAIBuilderStateMachine::UpdateChaseState(float DeltaTime)
{
    if (!HasValidTarget())
    {
        ChangeState(EAIBuilderState::Search);
        return;
    }
    
    if (IsInAttackRange())
    {
        ChangeState(EAIBuilderState::Attack);
    }
}

void UAIBuilderStateMachine::UpdateAttackState(float DeltaTime)
{
    if (!HasValidTarget())
    {
        ChangeState(EAIBuilderState::Search);
        return;
    }
    
    if (!IsInAttackRange())
    {
        ChangeState(EAIBuilderState::Chase);
    }
    
    // Attack logic would be handled elsewhere
    // This manages state transitions only
}

void UAIBuilderStateMachine::UpdateSearchState(float DeltaTime)
{
    if (HasValidTarget())
    {
        ChangeState(EAIBuilderState::Chase);
        return;
    }
    
    // Search for a limited time then return to patrol
    if (StateTimer > 5.0f)
    {
        ChangeState(EAIBuilderState::Return);
    }
}

void UAIBuilderStateMachine::UpdateReturnState(float DeltaTime)
{
    if (HasValidTarget())
    {
        ChangeState(EAIBuilderState::Chase);
        return;
    }
    
    // Logic to return to patrol point would be in behavior tree
    // Transition back to patrol when close to original position
    if (StateTimer > 3.0f)
    {
        ChangeState(EAIBuilderState::Patrol);
    }
}

void UAIBuilderStateMachine::EnterState(EAIBuilderState NewState)
{
    StateTimer = 0.0f;
    
    // State-specific entry logic
    switch (NewState)
    {
        case EAIBuilderState::Idle:
            // Stop movement
            break;
        case EAIBuilderState::Patrol:
            // Start patrol behavior
            break;
        case EAIBuilderState::Chase:
            // Increase movement speed
            if (OwnerCharacter)
            {
                OwnerCharacter->SetMovementSpeed(OwnerCharacter->MovementSpeed * 1.5f);
            }
            break;
        case EAIBuilderState::Attack:
            // Prepare attack
            break;
        case EAIBuilderState::Search:
            // Start search behavior
            break;
        case EAIBuilderState::Return:
            // Return to normal speed
            if (OwnerCharacter)
            {
                OwnerCharacter->SetMovementSpeed(OwnerCharacter->MovementSpeed / 1.5f);
            }
            break;
    }
}

void UAIBuilderStateMachine::ExitState(EAIBuilderState OldState)
{
    // State-specific exit logic
    switch (OldState)
    {
        case EAIBuilderState::Attack:
            // Stop attack animations/effects
            break;
        default:
            break;
    }
}

bool UAIBuilderStateMachine::HasValidTarget() const
{
    return OwnerCharacter && OwnerCharacter->GetCurrentTarget() != nullptr;
}

float UAIBuilderStateMachine::GetDistanceToTarget() const
{
    if (!HasValidTarget())
    {
        return -1.0f;
    }
    
    return FVector::Dist(OwnerCharacter->GetActorLocation(), 
                        OwnerCharacter->GetCurrentTarget()->GetActorLocation());
}

bool UAIBuilderStateMachine::IsInAttackRange() const
{
    if (!HasValidTarget())
    {
        return false;
    }
    
    return GetDistanceToTarget() <= OwnerCharacter->AttackRange;
}