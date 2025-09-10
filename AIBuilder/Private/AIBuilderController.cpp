// AIBuilderController.cpp - Custom AI Controller Implementation
#include "AIBuilderController.h"
#include "Core/AIBuilderCharacter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardAsset.h"
#include "AIBuilder.h"

AAIBuilderController::AAIBuilderController()
{
    // Initialize components
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

    // Set as primary tick enabled
    PrimaryActorTick.bCanEverTick = true;

    // Initialize state
    bAIStarted = false;
    bAIPaused = false;
}

void AAIBuilderController::BeginPlay()
{
    Super::BeginPlay();
    
    InitializeComponents();
    LogAIStatus(TEXT("Controller Begin Play"));
}

void AAIBuilderController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    
    if (AAIBuilderCharacter* AICharacter = Cast<AAIBuilderCharacter>(InPawn))
    {
        LogAIStatus(FString::Printf(TEXT("Possessed %s"), *AICharacter->GetName()));
        
        // Use character's assets if available, otherwise use defaults
        if (AICharacter->BehaviorTree)
        {
            DefaultBehaviorTree = AICharacter->BehaviorTree;
        }
        
        if (AICharacter->BlackboardAsset)
        {
            DefaultBlackboard = AICharacter->BlackboardAsset;
        }
        
        StartAI();
    }
}

void AAIBuilderController::OnUnPossess()
{
    StopAI();
    Super::OnUnPossess();
    LogAIStatus(TEXT("Unpossessed"));
}

void AAIBuilderController::InitializeComponents()
{
    SetupPerceptionSystem();
    ConfigureBlackboard();
}

void AAIBuilderController::SetupPerceptionSystem()
{
    if (AIPerceptionComponent)
    {
        // Configure sight sense
        UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
        if (SightConfig)
        {
            SightConfig->SightRadius = 1500.0f;
            SightConfig->LoseSightRadius = 1600.0f;
            SightConfig->PeripheralVisionAngleDegrees = 90.0f;
            SightConfig->SetMaxAge(5.0f);
            SightConfig->DetectionByAffiliation.bDetectEnemies = true;
            SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
            SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

            AIPerceptionComponent->ConfigureSense(*SightConfig);
            AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
        }

        // Configure hearing sense
        UAISenseConfig_Hearing* HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
        if (HearingConfig)
        {
            HearingConfig->HearingRange = 1000.0f;
            HearingConfig->SetMaxAge(3.0f);
            HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
            HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
            HearingConfig->DetectionByAffiliation.bDetectFriendlies = false;

            AIPerceptionComponent->ConfigureSense(*HearingConfig);
        }

        // Bind perception events
        AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AAIBuilderController::OnPerceptionUpdated);
        
        UE_LOG(LogAIBuilder, Log, TEXT("Perception system configured for %s"), *GetName());
    }
}

void AAIBuilderController::ConfigureBlackboard()
{
    if (DefaultBlackboard)
    {
        UseBlackboard(DefaultBlackboard);
        UE_LOG(LogAIBuilder, Log, TEXT("Blackboard configured for %s"), *GetName());
    }
}

void AAIBuilderController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    for (AActor* Actor : UpdatedActors)
    {
        if (Actor && Actor != GetPawn())
        {
            // Get perception info for this actor
            FActorPerceptionBlueprintInfo PerceptionInfo;
            AIPerceptionComponent->GetActorsPerception(Actor, PerceptionInfo);

            if (PerceptionInfo.LastSensedStimuli.Num() > 0)
            {
                const FAIStimulus& Stimulus = PerceptionInfo.LastSensedStimuli[0];
                
                if (Stimulus.WasSuccessfullySensed())
                {
                    // Update blackboard with target information
                    if (BlackboardComponent)
                    {
                        BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), Actor);
                        BlackboardComponent->SetValueAsVector(TEXT("TargetLocation"), Actor->GetActorLocation());
                        BlackboardComponent->SetValueAsBool(TEXT("HasTarget"), true);
                    }
                    
                    UE_LOG(LogAIBuilder, Log, TEXT("%s detected target: %s"), *GetName(), *Actor->GetName());
                }
                else
                {
                    // Lost sight of target
                    if (BlackboardComponent && BlackboardComponent->GetValueAsObject(TEXT("TargetActor")) == Actor)
                    {
                        BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), nullptr);
                        BlackboardComponent->SetValueAsBool(TEXT("HasTarget"), false);
                        
                        UE_LOG(LogAIBuilder, Log, TEXT("%s lost target: %s"), *GetName(), *Actor->GetName());
                    }
                }
            }
        }
    }
}

void AAIBuilderController::StartAI()
{
    if (bAIStarted)
    {
        UE_LOG(LogAIBuilder, Warning, TEXT("AI already started for %s"), *GetName());
        return;
    }

    if (DefaultBehaviorTree && BehaviorTreeComponent)
    {
        RunBehaviorTree(DefaultBehaviorTree);
        bAIStarted = true;
        bAIPaused = false;
        
        LogAIStatus(TEXT("Started"));
        UE_LOG(LogAIBuilder, Log, TEXT("AI started for %s with behavior tree %s"), 
               *GetName(), *DefaultBehaviorTree->GetName());
    }
    else
    {
        UE_LOG(LogAIBuilder, Error, TEXT("Cannot start AI for %s - missing behavior tree or component"), *GetName());
    }
}

void AAIBuilderController::StopAI()
{
    if (!bAIStarted)
    {
        return;
    }

    if (BehaviorTreeComponent)
    {
        BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
        bAIStarted = false;
        bAIPaused = false;
        
        LogAIStatus(TEXT("Stopped"));
        UE_LOG(LogAIBuilder, Log, TEXT("AI stopped for %s"), *GetName());
    }
}

void AAIBuilderController::PauseAI()
{
    if (!bAIStarted || bAIPaused)
    {
        return;
    }

    if (BehaviorTreeComponent)
    {
        BehaviorTreeComponent->PauseLogic(TEXT("Manual Pause"));
        bAIPaused = true;
        
        LogAIStatus(TEXT("Paused"));
        UE_LOG(LogAIBuilder, Log, TEXT("AI paused for %s"), *GetName());
    }
}

void AAIBuilderController::ResumeAI()
{
    if (!bAIStarted || !bAIPaused)
    {
        return;
    }

    if (BehaviorTreeComponent)
    {
        BehaviorTreeComponent->ResumeLogic(TEXT("Manual Resume"));
        bAIPaused = false;
        
        LogAIStatus(TEXT("Resumed"));
        UE_LOG(LogAIBuilder, Log, TEXT("AI resumed for %s"), *GetName());
    }
}

bool AAIBuilderController::IsAIRunning() const
{
    return bAIStarted && !bAIPaused;
}

void AAIBuilderController::LogAIStatus(const FString& Status) const
{
    UE_LOG(LogAIBuilder, Log, TEXT("AI Controller %s: %s (Started: %s, Paused: %s)"), 
           *GetName(), 
           *Status,
           bAIStarted ? TEXT("Yes") : TEXT("No"),
           bAIPaused ? TEXT("Yes") : TEXT("No"));
}