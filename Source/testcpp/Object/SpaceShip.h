// TEXTE CONFLIT BRANCHE MAIN
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpaceShip.generated.h"

UCLASS()
class TESTCPP_API ASpaceShip : public APawn
{
	GENERATED_BODY()

public:
	ASpaceShip();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void TakeDamage();
	UFUNCTION(BlueprintCallable, Category = "Ship Stats")
	int32 GetCurrentHp() const;

	UFUNCTION(BlueprintPure, Category = "Ship Stats")
	int32 GetScore() const;
	
	UFUNCTION(BlueprintCallable, Category = "Ship Logic")
	void UseBonus(int32 Id);
    
	UFUNCTION(BlueprintCallable, Category = "Ship Stats")
	void IncScore(int32 sc = 1);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SphereHitbox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPaperFlipbookComponent* FlipbookSpaceship;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship Settings")
	float DashDuration = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship Settings")
	float DashCooldown = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship Settings")
	float RocketBonusDuration = 5.0f;
	
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship Settings")
	TSubclassOf<class AProjectile> BulletClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship Settings")
	TSubclassOf<class AProjectile> RocketClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	class USoundBase* FireSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	class USoundBase* HealSound;


protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship Settings")
	float MoveSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship Settings")
	float DashForce = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship Settings")
	int BASE_HP = 3;
	
	int Score = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship Settings")
	int CurrentHp = BASE_HP;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Ship Events")
	void OnShipDamaged(int32 stadeHP);

	bool bIsRocketActive = false;
	FTimerHandle RocketBonusTimer;
	void DeactivateRocketBonus();
	
	UFUNCTION(BlueprintCallable, Category = "Ship Actions")
	void FireProjectile();
	UFUNCTION(BlueprintImplementableEvent, Category = "Ship Events")
	void OnSpawnGhostTrail();



private:
	FVector CurrentVelocity;
	bool IsDashing = false;
	bool CanDash = true;

	void MoveRight(float Value);
	void MoveUp(float Value);
	void Dash();
	void StopDash();
	void ResetDashCooldown();
	void Heal();
	void TriggerGhostTrail();

	FTimerHandle DashDurationTimer;
	FTimerHandle DashCooldownTimer;
	FTimerHandle GhostTrailTimer;
};