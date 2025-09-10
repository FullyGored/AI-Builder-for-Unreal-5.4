// AIBuilderCharacter.cpp - Main AI Character Implementation
#include "Core/AIBuilderCharacter.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIBuilder.h"

AAIBuilderCharacter::AAIBuilderCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Setup AI Perception Component
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
    
    // Setup Blackboard Component
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
    
    // Setup Behavior Tree Component
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
    
    // Setup Sensor Component
    SensorComponent = CreateDefaultSubobject<UAIBuilderSensorComponent>(TEXT("SensorComponent"));
    
    // Setup State Machine
    StateMachine = CreateDefaultSubobject<UAIBuilderStateMachine>(TEXT("StateMachine"));

    // Configure character movement
    GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

    // Initialize variables
    CurrentTarget = nullptr;
    LastUpdateTime = 0.0f;
}

void AAIBuilderCharacter::BeginPlay()
{
    Super::BeginPlay();
    InitializeAI();
}

void AAIBuilderCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateAIState(DeltaTime);
}

void AAIBuilderCharacter::InitializeAI()
{
    // Setup AI Controller
    if (AAIController* AIController = Cast<AAIController>(GetController()))
    {
        if (BlackboardAsset)
        {
            AIController->UseBlackboard(BlackboardAsset);
            BlackboardComponent = AIController->GetBlackboardComponent();
        }

        SetupPerception();
        
        if (BehaviorTree)
        {
            AIController->RunBehaviorTree(BehaviorTree);
        }
    }

    // Initialize state machine
    if (StateMachine)
    {
        StateMachine->Initialize(this);
    }

    UE_LOG(LogAIBuilder, Log, TEXT("AI initialized for %s"), *GetName());
}

void AAIBuilderCharacter::SetupPerception()
{
    if (AIPerceptionComponent)
    {
        // Setup sight sense
        UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
        SightConfig->SightRadius = SightRadius;
        SightConfig->LoseSightRadius = LoseSightRadius;
        SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees;
        SightConfig->SetMaxAge(5.0f);
        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

        AIPerceptionComponent->ConfigureSense(*SightConfig);
        AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

        // Bind perception events
        AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AAIBuilderCharacter::OnPerceptionUpdated);
        AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIBuilderCharacter::OnTargetPerceptionUpdated);
    }
}

void AAIBuilderCharacter::UpdateAIState(float DeltaTime)
{
    LastUpdateTime += DeltaTime;
    
    // Update every 0.1 seconds for performance
    if (LastUpdateTime >= 0.1f)
    {
        if (StateMachine)
        {
            StateMachine->UpdateState(DeltaTime);
        }
        
        LastUpdateTime = 0.0f;
    }
}

void AAIBuilderCharacter::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    for (AActor* Actor : UpdatedActors)
    {
        if (Actor && Actor != this)
        {
            UE_LOG(LogAIBuilder, Log, TEXT("%s detected %s"), *GetName(), *Actor->GetName());
        }
    }
}

void AAIBuilderCharacter::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (Actor && Stimulus.WasSuccessfullySensed())
    {
        SetCurrentTarget(Actor);
        UE_LOG(LogAIBuilder, Log, TEXT("%s acquired target: %s"), *GetName(), *Actor->GetName());
    }
    else if (Actor == CurrentTarget && !Stimulus.WasSuccessfullySensed())
    {
        SetCurrentTarget(nullptr);
        UE_LOG(LogAIBuilder, Log, TEXT("%s lost target: %s"), *GetName(), *Actor->GetName());
    }
}

void AAIBuilderCharacter::SetMovementSpeed(float NewSpeed)
{
    MovementSpeed = FMath::Clamp(NewSpeed, 0.0f, 1000.0f);
    GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

void AAIBuilderCharacter::StartBehaviorTree()
{
    if (AAIController* AIController = Cast<AAIController>(GetController()))
    {
        if (BehaviorTree)
        {
            AIController->RunBehaviorTree(BehaviorTree);
            UE_LOG(LogAIBuilder, Log, TEXT("Started behavior tree for %s"), *GetName());
        }
    }
}

void AAIBuilderCharacter::StopBehaviorTree()
{
    if (AAIController* AIController = Cast<AAIController>(GetController()))
    {
        AIController->StopBehaviorTree(EBTStopMode::Safe);
        UE_LOG(LogAIBuilder, Log, TEXT("Stopped behavior tree for %s"), *GetName());
    }
}

AActor* AAIBuilderCharacter::GetCurrentTarget() const
{
    return CurrentTarget;
}

void AAIBuilderCharacter::SetCurrentTarget(AActor* NewTarget)
{
    CurrentTarget = NewTarget;
    
    if (BlackboardComponent)
    {
        BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), CurrentTarget);
    }
}