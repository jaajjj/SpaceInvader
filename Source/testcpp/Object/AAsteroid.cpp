#include "AAsteroid.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"
#include "SpaceShip.h"

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
}

void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		this->Destroy(); 
	}
}

void AAsteroid::TakeDamage(int32 DamageAmount)
{
	CurrentHp -= DamageAmount;
	if (CurrentHp <= 0)
	{
		//explosion ??
		Destroy(); 
	}
}