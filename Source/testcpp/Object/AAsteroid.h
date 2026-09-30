#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AAsteroid.generated.h"

UCLASS()
class TESTCPP_API AAsteroid : public AActor
{
	GENERATED_BODY()
    
public:    
	AAsteroid();
	
	//Hitbox
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SphereHitbox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPaperFlipbookComponent* FlipbookAsteroid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Settings")
	float MoveSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Settings")
	FVector MoveDirection = FVector(0.0f, 0.0f, -1.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Settings")
	int32 MaxHpBase = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Settings")
	int32 MinHpBase = 3;
	
	int32 CurrentHp;

	UFUNCTION(BlueprintCallable, Category = "Asteroid Events")
	void TakeDamage(int32 DamageAmount = 1);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Settings")
	bool bHasEnteredArena = false;
	
	//limites de l'arène
	float AreneMinX;
	float AreneMaxX;
	float AreneMinZ;
	float AreneMaxZ;

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:    
	virtual void Tick(float DeltaTime) override;
};