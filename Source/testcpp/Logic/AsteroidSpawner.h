#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AsteroidSpawner.generated.h"

UCLASS()
class TESTCPP_API AAsteroidSpawner : public AActor
{
	GENERATED_BODY()
    
public:	
	AAsteroidSpawner();

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle SpawnTimer;
    
	int32 SecondesEcoulees = 0;

	void TrySpawnAsteroid();
	void SpawnAsteroid();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<TSubclassOf<class AAsteroid>> AsteroidClasses;
	
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