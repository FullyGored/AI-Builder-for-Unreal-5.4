// AIBuilderSensorComponent.h - Advanced Sensor System
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/Engine.h"
#include "AIBuilderSensorComponent.generated.h"

USTRUCT(BlueprintType)
struct FAISensorData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AActor* DetectedActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector LastKnownLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DetectionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Confidence;

    FAISensorData()
    {
        DetectedActor = nullptr;
        LastKnownLocation = FVector::ZeroVector;
        DetectionTime = 0.0f;
        Confidence = 0.0f;
    }
};

UENUM(BlueprintType)
enum class ESensorType : uint8
{
    Sight       UMETA(DisplayName = "Sight"),
    Hearing     UMETA(DisplayName = "Hearing"),
    Touch       UMETA(DisplayName = "Touch"),
    Damage      UMETA(DisplayName = "Damage")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorDetected, AActor*, DetectedActor, ESensorType, SensorType, float, Confidence);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorLost, AActor*, LostActor, ESensorType, SensorType);

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class AIBUILDER_API UAIBuilderSensorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAIBuilderSensorComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    // Events
    UPROPERTY(BlueprintAssignable, Category = "AI Builder|Sensors")
    FOnActorDetected OnActorDetected;

    UPROPERTY(BlueprintAssignable, Category = "AI Builder|Sensors")
    FOnActorLost OnActorLost;

    // Sensor Configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Builder|Sight")
    bool bEnableSightSensor = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Builder|Sight")
    float SightRange = 1500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Builder|Sight")
    float SightAngle = 90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Builder|Hearing")
    bool bEnableHearingSensor = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Builder|Hearing")
    float HearingRange = 800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Builder|Touch")
    bool bEnableTouchSensor = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Builder|Touch")
    float TouchRange = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Builder|General")
    float UpdateFrequency = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Builder|General")
    float ForgetTime = 5.0f;

    // Blueprint callable functions
    UFUNCTION(BlueprintCallable, Category = "AI Builder")
    TArray<FAISensorData> GetDetectedActors() const;

    UFUNCTION(BlueprintCallable, Category = "AI Builder")
    FAISensorData GetHighestConfidenceDetection() const;

    UFUNCTION(BlueprintCallable, Category = "AI Builder")
    bool HasDetectedActor(AActor* Actor) const;

    UFUNCTION(BlueprintCallable, Category = "AI Builder")
    void AddNoiseEvent(FVector Location, float Volume, AActor* Instigator = nullptr);

    UFUNCTION(BlueprintCallable, Category = "AI Builder")
    void SetSensorEnabled(ESensorType SensorType, bool bEnabled);

protected:
    UPROPERTY()
    TArray<FAISensorData> DetectedActors;

    float LastUpdateTime;

    // Sensor update functions
    void UpdateSightSensor(float DeltaTime);
    void UpdateHearingSensor(float DeltaTime);
    void UpdateTouchSensor(float DeltaTime);

    // Utility functions
    bool CanSeeActor(AActor* Actor, FVector& OutHitLocation) const;
    float CalculateSightConfidence(AActor* Actor, float Distance) const;
    float CalculateHearingConfidence(FVector NoiseLocation, float Volume, float Distance) const;

    void AddOrUpdateDetection(AActor* Actor, ESensorType SensorType, float Confidence, FVector Location);
    void RemoveOldDetections(float DeltaTime);

    // Noise events for hearing
    USTRUCT()
    struct FNoiseEvent
    {
        GENERATED_BODY()

        FVector Location;
        float Volume;
        float TimeStamp;
        AActor* Instigator;

        FNoiseEvent()
        {
            Location = FVector::ZeroVector;
            Volume = 0.0f;
            TimeStamp = 0.0f;
            Instigator = nullptr;
        }
    };

    TArray<FNoiseEvent> RecentNoiseEvents;
};