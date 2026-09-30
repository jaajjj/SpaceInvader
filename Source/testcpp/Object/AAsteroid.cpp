#include "AAsteroid.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "SpaceShip.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AAsteroid::AAsteroid()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("SphereHitbox"));
	SphereHitbox->InitSphereRadius(25.0f);
	SphereHitbox->SetSimulatePhysics(true);
	SphereHitbox->SetEnableGravity(false);
	SphereHitbox->GetBodyInstance()->bLockYTranslation = true;
	SphereHitbox->GetBodyInstance()->bLockXRotation = true;
	SphereHitbox->GetBodyInstance()->bLockYRotation = true;
	SphereHitbox->GetBodyInstance()->bLockZRotation = true;
	RootComponent = SphereHitbox; 

	FlipbookAsteroid = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComp"));
	FlipbookAsteroid->SetupAttachment(RootComponent);
	FlipbookAsteroid->SetCollisionProfileName(TEXT("NoCollision")); 
}

void AAsteroid::BeginPlay()
{
	Super::BeginPlay();
	CurrentHp = FMath::RandRange(MinHpBase, MaxHpBase);
	SphereHitbox->OnComponentHit.AddDynamic(this, &AAsteroid::OnHit);
	float RandomDepthOffset = FMath::RandRange(-15.0f, 15.0f);
	FlipbookAsteroid->SetRelativeLocation(FVector(0.0f, RandomDepthOffset, 0.0f));
	RotationSpeed = FMath::RandRange(-150.0f, 150.0f);
	
	if (FlipbookAsteroid)
	{
		InitialScale = FlipbookAsteroid->GetRelativeScale3D();
		InitialColor = FlipbookAsteroid->GetSpriteColor();
	}
}

void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//FlipbookAsteroid->AddLocalRotation(FRotator(RotationSpeed * DeltaTime, 0.0f, 0.0f).Quaternion());
	FlipbookAsteroid->AddWorldRotation(FRotator(RotationSpeed * DeltaTime, 0.0f, 0.0f));
	FVector Loc = GetActorLocation();
	//est entré sur la zone de jeu?
	if (!bHasEnteredArena)
	{
		if (Loc.X > AreneMinX && Loc.X < AreneMaxX && Loc.Z > AreneMinZ && Loc.Z < AreneMaxZ)
		{
			bHasEnteredArena = true;
		}
	}
	else
	{
		//destruction si off-bound
		if (Loc.X < AreneMinX - 150.0f || Loc.X > AreneMaxX + 150.0f || 
			Loc.Z < AreneMinZ - 150.0f || Loc.Z > AreneMaxZ + 150.0f)
		{
			Destroy();
		}
	}
}

void AAsteroid::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor->IsA(ASpaceShip::StaticClass()))
	{
		ASpaceShip* Ship = Cast<ASpaceShip>(OtherActor);
		if (Ship) { Ship->TakeDamage(); }
	}
}

void AAsteroid::TakeDamage(int32 DamageAmount, bool bRewardScore){
	if (bIsDying) return;
	CurrentHp -= DamageAmount;
    
	if (ExplosionCameraShake)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->ClientStartCameraShake(ExplosionCameraShake);
		}
	}

	//mort de l'asteroid
	if (CurrentHp <= 0)
	{
		//reward 
		if (bRewardScore)
		{
			ASpaceShip* Ship = Cast<ASpaceShip>(UGameplayStatics::GetPlayerPawn(this, 0));
			if (Ship)
			{
				Ship->IncScore(ScoreValue);
			}
		}
		//mort de l'asteroid
		bIsDying = true;
		FlipbookAsteroid->SetVisibility(true);
		SphereHitbox->SetSimulatePhysics(false);
		SphereHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (DestroySound)
		{
			UGameplayStatics::PlaySound2D(this, DestroySound);
		}

		RotationSpeed = 0.0f; 
		//anim destroy
		if (DeathAnimation)
		{
			FlipbookAsteroid->SetFlipbook(DeathAnimation);
			FlipbookAsteroid->SetLooping(false);
			FlipbookAsteroid->PlayFromStart();

			if (FlipbookAsteroid)
			{
				FlipbookAsteroid->SetRelativeScale3D(InitialScale);
				FlipbookAsteroid->SetSpriteColor(InitialColor);
				FlipbookAsteroid->SetVisibility(true);
			}
			float AnimDuration = DeathAnimation->GetTotalDuration();
			GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &AAsteroid::OnDeathAnimationFinished, AnimDuration, false);		}
		else
		{
			OnDeathAnimationFinished();
		}
	}
	else //Si il survit
	{
		if (FlipbookAsteroid)
		{
			FlipbookAsteroid->SetRelativeScale3D(InitialScale * HitScaleMultiplier);
			FlipbookAsteroid->SetSpriteColor(HitFlashColor);
		}

		GetWorld()->GetTimerManager().SetTimer(DamageFlickerTimer, this, &AAsteroid::ResetDamageVisuals, HitFlashDuration, false);
	}
}

void AAsteroid::ResetDamageVisuals()
{
	if (!bIsDying && FlipbookAsteroid)
	{
		FlipbookAsteroid->SetRelativeScale3D(InitialScale);
		FlipbookAsteroid->SetSpriteColor(InitialColor);
	}
}

void AAsteroid::OnDeathAnimationFinished()
{
	Destroy();
}