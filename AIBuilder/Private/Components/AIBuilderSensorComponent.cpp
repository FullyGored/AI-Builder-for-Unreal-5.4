// AIBuilderSensorComponent.cpp - Advanced Sensor Implementation
#include "Components/AIBuilderSensorComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "AIBuilder.h"

UAIBuilderSensorComponent::UAIBuilderSensorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    LastUpdateTime = 0.0f;
}

void UAIBuilderSensorComponent::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogAIBuilder, Log, TEXT("AI Sensor Component initialized for %s"), *GetOwner()->GetName());
}

void UAIBuilderSensorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    LastUpdateTime += DeltaTime;
    
    if (LastUpdateTime >= UpdateFrequency)
    {
        if (bEnableSightSensor)
        {
            UpdateSightSensor(DeltaTime);
        }
        
        if (bEnableHearingSensor)
        {
            UpdateHearingSensor(DeltaTime);
        }
        
        if (bEnableTouchSensor)
        {
            UpdateTouchSensor(DeltaTime);
        }
        
        RemoveOldDetections(DeltaTime);
        LastUpdateTime = 0.0f;
    }
}

void UAIBuilderSensorComponent::UpdateSightSensor(float DeltaTime)
{
    if (!GetOwner())
        return;

    FVector OwnerLocation = GetOwner()->GetActorLocation();
    FVector OwnerForward = GetOwner()->GetActorForwardVector();

    // Get all actors in sight range
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetOwner());

    bool bHit = GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        OwnerLocation,
        FQuat::Identity,
        ECC_Pawn,
        FCollisionShape::MakeSphere(SightRange),
        QueryParams
    );

    if (bHit)
    {
        for (const FOverlapResult& Result : OverlapResults)
        {
            if (AActor* DetectedActor = Result.GetActor())
            {
                FVector ToTarget = DetectedActor->GetActorLocation() - OwnerLocation;
                float Distance = ToTarget.Size();
                ToTarget.Normalize();

                // Check if within sight angle
                float DotProduct = FVector::DotProduct(OwnerForward, ToTarget);
                float AngleRadians = FMath::Acos(DotProduct);
                float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

                if (AngleDegrees <= SightAngle * 0.5f)
                {
                    FVector HitLocation;
                    if (CanSeeActor(DetectedActor, HitLocation))
                    {
                        float Confidence = CalculateSightConfidence(DetectedActor, Distance);
                        AddOrUpdateDetection(DetectedActor, ESensorType::Sight, Confidence, DetectedActor->GetActorLocation());
                    }
                }
            }
        }
    }
}

void UAIBuilderSensorComponent::UpdateHearingSensor(float DeltaTime)
{
    if (!GetOwner())
        return;

    FVector OwnerLocation = GetOwner()->GetActorLocation();

    // Process recent noise events
    for (int32 i = RecentNoiseEvents.Num() - 1; i >= 0; i--)
    {
        const FNoiseEvent& NoiseEvent = RecentNoiseEvents[i];
        float EventAge = GetWorld()->GetTimeSeconds() - NoiseEvent.TimeStamp;

        if (EventAge > 2.0f) // Remove old noise events
        {
            RecentNoiseEvents.RemoveAt(i);
            continue;
        }

        float Distance = FVector::Dist(OwnerLocation, NoiseEvent.Location);
        if (Distance <= HearingRange)
        {
            float Confidence = CalculateHearingConfidence(NoiseEvent.Location, NoiseEvent.Volume, Distance);
            
            if (NoiseEvent.Instigator)
            {
                AddOrUpdateDetection(NoiseEvent.Instigator, ESensorType::Hearing, Confidence, NoiseEvent.Location);
            }
        }
    }
}

void UAIBuilderSensorComponent::UpdateTouchSensor(float DeltaTime)
{
    if (!GetOwner())
        return;

    FVector OwnerLocation = GetOwner()->GetActorLocation();

    // Get all actors in touch range
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetOwner());

    bool bHit = GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        OwnerLocation,
        FQuat::Identity,
        ECC_Pawn,
        FCollisionShape::MakeSphere(TouchRange),
        QueryParams
    );

    if (bHit)
    {
        for (const FOverlapResult& Result : OverlapResults)
        {
            if (AActor* DetectedActor = Result.GetActor())
            {
                // Touch sensor has high confidence when in range
                AddOrUpdateDetection(DetectedActor, ESensorType::Touch, 1.0f, DetectedActor->GetActorLocation());
            }
        }
    }
}

bool UAIBuilderSensorComponent::CanSeeActor(AActor* Actor, FVector& OutHitLocation) const
{
    if (!Actor || !GetOwner())
        return false;

    FVector StartLocation = GetOwner()->GetActorLocation();
    FVector EndLocation = Actor->GetActorLocation();
    
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetOwner());
    QueryParams.AddIgnoredActor(Actor);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        ECC_Visibility,
        QueryParams
    );

    OutHitLocation = HitResult.ImpactPoint;
    return !bHit; // No obstruction means we can see the actor
}

float UAIBuilderSensorComponent::CalculateSightConfidence(AActor* Actor, float Distance) const
{
    if (!Actor)
        return 0.0f;

    // Confidence decreases with distance
    float DistanceRatio = 1.0f - (Distance / SightRange);
    return FMath::Clamp(DistanceRatio, 0.0f, 1.0f);
}

float UAIBuilderSensorComponent::CalculateHearingConfidence(FVector NoiseLocation, float Volume, float Distance) const
{
    // Confidence based on volume and distance
    float VolumeRatio = Volume / 100.0f; // Assuming max volume of 100
    float DistanceRatio = 1.0f - (Distance / HearingRange);
    
    return FMath::Clamp(VolumeRatio * DistanceRatio, 0.0f, 1.0f);
}

void UAIBuilderSensorComponent::AddOrUpdateDetection(AActor* Actor, ESensorType SensorType, float Confidence, FVector Location)
{
    if (!Actor)
        return;

    // Find existing detection
    FAISensorData* ExistingData = DetectedActors.FindByPredicate([Actor](const FAISensorData& Data)
    {
        return Data.DetectedActor == Actor;
    });

    if (ExistingData)
    {
        // Update existing detection
        ExistingData->LastKnownLocation = Location;
        ExistingData->DetectionTime = GetWorld()->GetTimeSeconds();
        ExistingData->Confidence = FMath::Max(ExistingData->Confidence, Confidence);
    }
    else
    {
        // Add new detection
        FAISensorData NewData;
        NewData.DetectedActor = Actor;
        NewData.LastKnownLocation = Location;
        NewData.DetectionTime = GetWorld()->GetTimeSeconds();
        NewData.Confidence = Confidence;
        
        DetectedActors.Add(NewData);
        OnActorDetected.Broadcast(Actor, SensorType, Confidence);
        
        UE_LOG(LogAIBuilder, Log, TEXT("%s detected %s via %s sensor"), 
               *GetOwner()->GetName(), 
               *Actor->GetName(), 
               *UEnum::GetValueAsString(SensorType));
    }
}

void UAIBuilderSensorComponent::RemoveOldDetections(float DeltaTime)
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    
    for (int32 i = DetectedActors.Num() - 1; i >= 0; i--)
    {
        FAISensorData& Data = DetectedActors[i];
        float Age = CurrentTime - Data.DetectionTime;
        
        if (Age > ForgetTime || !IsValid(Data.DetectedActor))
        {
            AActor* LostActor = Data.DetectedActor;
            DetectedActors.RemoveAt(i);
            
            if (LostActor)
            {
                OnActorLost.Broadcast(LostActor, ESensorType::Sight);
                UE_LOG(LogAIBuilder, Log, TEXT("%s lost detection of %s"), 
                       *GetOwner()->GetName(), *LostActor->GetName());
            }
        }
    }
}

TArray<FAISensorData> UAIBuilderSensorComponent::GetDetectedActors() const
{
    return DetectedActors;
}

FAISensorData UAIBuilderSensorComponent::GetHighestConfidenceDetection() const
{
    FAISensorData BestDetection;
    float HighestConfidence = -1.0f;
    
    for (const FAISensorData& Data : DetectedActors)
    {
        if (Data.Confidence > HighestConfidence)
        {
            HighestConfidence = Data.Confidence;
            BestDetection = Data;
        }
    }
    
    return BestDetection;
}

bool UAIBuilderSensorComponent::HasDetectedActor(AActor* Actor) const
{
    return DetectedActors.ContainsByPredicate([Actor](const FAISensorData& Data)
    {
        return Data.DetectedActor == Actor;
    });
}

void UAIBuilderSensorComponent::AddNoiseEvent(FVector Location, float Volume, AActor* Instigator)
{
    FNoiseEvent NoiseEvent;
    NoiseEvent.Location = Location;
    NoiseEvent.Volume = Volume;
    NoiseEvent.TimeStamp = GetWorld()->GetTimeSeconds();
    NoiseEvent.Instigator = Instigator;
    
    RecentNoiseEvents.Add(NoiseEvent);
    
    UE_LOG(LogAIBuilder, Log, TEXT("Noise event added at %s with volume %f"), 
           *Location.ToString(), Volume);
}

void UAIBuilderSensorComponent::SetSensorEnabled(ESensorType SensorType, bool bEnabled)
{
    switch (SensorType)
    {
        case ESensorType::Sight:
            bEnableSightSensor = bEnabled;
            break;
        case ESensorType::Hearing:
            bEnableHearingSensor = bEnabled;
            break;
        case ESensorType::Touch:
            bEnableTouchSensor = bEnabled;
            break;
    }
    
    UE_LOG(LogAIBuilder, Log, TEXT("%s sensor %s for %s"), 
           *UEnum::GetValueAsString(SensorType),
           bEnabled ? TEXT("enabled") : TEXT("disabled"),
           *GetOwner()->GetName());
}