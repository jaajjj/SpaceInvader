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
	virtual void Tick(float DeltaTime) override;
	
	//Hitbox
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SphereHitbox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPaperFlipbookComponent* FlipbookAsteroid;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	class UParticleSystem* ExplosionParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TSubclassOf<class UCameraShakeBase> ExplosionCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Settings")
	float MoveSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Settings")
	FVector MoveDirection = FVector(0.0f, 0.0f, -1.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Settings")
	int32 MaxHpBase = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Settings")
	int32 MinHpBase = 3;
	
	int32 CurrentHp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Settings")
	int32 ScoreValue = 100;

	UFUNCTION(BlueprintCallable, Category = "Asteroid Events")
	void TakeDamage(int32 DamageAmount = 1, bool bRewardScore = true);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Settings")
	bool bHasEnteredArena = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Feedback")
	float HitFlashDuration = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Feedback")
	float HitScaleMultiplier = 1.15f; //scale on Hit

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Feedback")
	FLinearColor HitFlashColor = FLinearColor(2.5f, 2.5f, 2.5f, 1.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Feedback")
	class USoundBase* DestroySound;
	
	//limites de l'arène
	float AreneMinX;
	float AreneMaxX;
	float AreneMinZ;
	float AreneMaxZ;

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Settings")
	float RotationSpeed = 90.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Settings")
	float tmpDisparait = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	class UPaperFlipbook* DeathAnimation;

	void OnDeathAnimationFinished();
	void ResetDamageVisuals();

private:
	bool bIsDying = false;
	FTimerHandle DeathTimerHandle;
	FTimerHandle DamageFlickerTimer;
	
	FVector InitialScale = FVector::OneVector;
	FLinearColor InitialColor = FLinearColor::White;
};