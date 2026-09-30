#include "SpaceShip.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AAsteroid.h"

ASpaceShip::ASpaceShip()
{
    PrimaryActorTick.bCanEverTick = true;

    SphereHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("SphereHitbox"));
    SphereHitbox->InitSphereRadius(20.0f); // Rayon de base de la sphère
    RootComponent = SphereHitbox;
    
    FlipbookSpaceship = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComp"));
    FlipbookSpaceship->SetupAttachment(RootComponent);
    FlipbookSpaceship->SetCollisionProfileName(TEXT("NoCollision"));
    
    LocationCannon = CreateDefaultSubobject<USceneComponent>(TEXT("GunMuzzle"));
    LocationCannon->SetupAttachment(RootComponent);
    LocationCannon->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); //j'enleve ca 
}

void ASpaceShip::BeginPlay()
{
    Super::BeginPlay();
    OnActorBeginOverlap.AddDynamic(this, &ASpaceShip::OnOverlapBegin);
}

void ASpaceShip::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor && OtherActor->IsA(AAsteroid::StaticClass()))
    {
        TakeDamage();
        OtherActor->Destroy();
    }
}

void ASpaceShip::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //on bouge
    if (!CurrentVelocity.IsNearlyZero())
    {
        FVector Direction = CurrentVelocity.GetSafeNormal();
        float CurrentSpeed;
        if (IsDashing) { CurrentSpeed = (MoveSpeed * DashForce); } //vitesse avec dash
        else { CurrentSpeed = MoveSpeed; }
        
        FVector DeltaMove = Direction * CurrentSpeed * DeltaTime;
        AddActorWorldOffset(FVector(DeltaMove.X, 0.0f, 0.0f), true);
        AddActorWorldOffset(FVector(0.0f, 0.0f, DeltaMove.Z), true);
    }
}

void ASpaceShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    PlayerInputComponent->BindAxis("MoveRight", this, &ASpaceShip::MoveRight);
    PlayerInputComponent->BindAxis("MoveUp", this, &ASpaceShip::MoveUp);
    
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASpaceShip::FireLaser);
    PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASpaceShip::Dash);
}

//Inputs

void ASpaceShip::MoveRight(float Value)
{
    CurrentVelocity.X = Value;
}

void ASpaceShip::MoveUp(float Value)
{
    CurrentVelocity.Z = Value;
}

void ASpaceShip::FireLaser()
{
    if (LaserClass)
    {
        FVector SpawnLocation = LocationCannon->GetComponentLocation();
        FRotator SpawnRotation = LocationCannon->GetComponentRotation();
        GetWorld()->SpawnActor<AActor>(LaserClass, SpawnLocation, SpawnRotation);
    }
}

//Dash

void ASpaceShip::Dash()
{
    if (CanDash && !CurrentVelocity.IsNearlyZero())
    {
        IsDashing = true;
        CanDash = false;
        
        GetWorld()->GetTimerManager().SetTimer(DashDurationTimer, this, &ASpaceShip::StopDash, DashDuration, false);
        //cooldown
        GetWorld()->GetTimerManager().SetTimer(DashCooldownTimer, this, &ASpaceShip::ResetDashCooldown, DashCooldown, false);
    }
}
void ASpaceShip::TakeDamage()
{
    CurrentHp--;
    if (CurrentHp <= 0) 
    {
        UGameplayStatics::OpenLevel(this, FName("DeathScreen"));
        Destroy(); 
    }
    else 
    {
        OnShipDamaged(CurrentHp);
    }
}
int32 ASpaceShip::GetCurrentHp() const { return CurrentHp; }
int32 ASpaceShip::GetScore() const { return Score; }

void ASpaceShip::IncScore(int32 sc)
{
    Score += sc;
}

void ASpaceShip::StopDash()
{
    IsDashing = false;
}

void ASpaceShip::ResetDashCooldown()
{
    CanDash = true;
}

void ASpaceShip::UseBonus(int32 IdBonus)
{
    if (IdBonus == 0)
    {
        // Activer les roquettes
    }
    else if (IdBonus == 1)
    {
        // Soigner le vaisseau
    }
}