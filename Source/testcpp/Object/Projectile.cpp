#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AAsteroid.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(15.0f);
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoveComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
    
	ProjectileMovement->Velocity = FVector(0.0f, 0.0f, 1.0f);
    
	ProjectileMovement->bRotationFollowsVelocity = false; 

	FlipbookProjectile = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComp"));
	FlipbookProjectile->SetupAttachment(RootComponent);
	FlipbookProjectile->SetCollisionProfileName(TEXT("NoCollision"));
	
	ProjectileMovement->Velocity = FVector(0.0f, 0.0f, 1.0f);
	ProjectileMovement->bRotationFollowsVelocity = false;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
	SetLifeSpan(4.0f); 
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = FVector(0.0f, 0.0f, ProjectileMovement->InitialSpeed);
	}
}

void AProjectile::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor->IsA(AAsteroid::StaticClass()))
	{
		if (ImpactParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, GetActorLocation());
		}
		AAsteroid* HitAsteroid = Cast<AAsteroid>(OtherActor);
		if (HitAsteroid) HitAsteroid->TakeDamage(Damage); 
		Destroy();
	}
}