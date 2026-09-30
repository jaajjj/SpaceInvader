#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABonus.generated.h"

UCLASS()
class TESTCPP_API ABonus : public AActor
{
	GENERATED_BODY()
    
public:    
	ABonus();
	
	//Hitbox
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SphereHitbox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPaperFlipbookComponent* FlipbookBonus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bonus Settings")
	float MoveSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bonus Settings")
	FVector MoveDirection = FVector(0.0f, 0.0f, -1.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bonus Settings")
	int32 IdBonus = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bonus Settings")
	bool bHasEnteredArena = false;
	
	//limites de l'arène
	float AreneMinX;
	float AreneMaxX;
	float AreneMinZ;
	float AreneMaxZ;

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnHitboxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:    
	virtual void Tick(float DeltaTime) override;
};