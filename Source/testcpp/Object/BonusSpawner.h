#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BonusSpawner.generated.h"

UCLASS()
class TESTCPP_API ABonusSpawner : public AActor
{
	GENERATED_BODY()
    
public:	 
	ABonusSpawner();

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle SpawnTimer;
	void TrySpawnBonus();
	void SpawnBonus();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Delay Spawn")
	float MinDelayBonus = 20.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Delay Spawn")
	float MaxDelayBonus = 40.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<TSubclassOf<class ABonus>> BonusClasses;
	
	//limite haut
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limites Spawn")
	float MaxZ = 500.0f;
    
	//limite bas
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limites Spawn")
	float MinZ = -400.0f;

	//limite gauche
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limites Spawn")
	float MinX = -200.0f;

	//limite droite
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limites Spawn")
	float MaxX = 1400.0f;
};