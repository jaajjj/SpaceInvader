#include "ABonus.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"
#include "SpaceShip.h"

ABonus::ABonus()
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

	FlipbookBonus = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComp"));
	FlipbookBonus->SetupAttachment(RootComponent);
	FlipbookBonus->SetCollisionProfileName(TEXT("NoCollision")); 
}

void ABonus::BeginPlay()
{
	Super::BeginPlay();
	SphereHitbox->OnComponentBeginOverlap.AddDynamic(this, &ABonus::OnHitboxOverlap);
}

void ABonus::OnHitboxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(ASpaceShip::StaticClass()))
	{
		ASpaceShip* Ship = Cast<ASpaceShip>(OtherActor);
		if (Ship) { Ship->UseBonus(IdBonus); } //on utilise le bonus instant
		this->Destroy(); 
	}
}

void ABonus::Tick(float DeltaTime)
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

