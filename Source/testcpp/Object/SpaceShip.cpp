#include "SpaceShip.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AAsteroid.h"
#include "Projectile.h"
#include "Blueprint/UserWidget.h"

ASpaceShip::ASpaceShip()
{
    PrimaryActorTick.bCanEverTick = true;

    SphereHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("SphereHitbox"));
    SphereHitbox->InitSphereRadius(20.0f);
    RootComponent = SphereHitbox;
    
    FlipbookSpaceship = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComp"));
    FlipbookSpaceship->SetupAttachment(RootComponent);
    FlipbookSpaceship->SetCollisionProfileName(TEXT("NoCollision"));
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
        AAsteroid* Asteroid = Cast<AAsteroid>(OtherActor);
        if (Asteroid)
        {
            Asteroid->TakeDamage(100, false);
        }
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
    
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASpaceShip::FireProjectile);
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

void ASpaceShip::FireProjectile()
{
    TSubclassOf<AProjectile> ProjectileToSpawn = bIsRocketActive ? RocketClass : BulletClass;

    if (ProjectileToSpawn)
    {
        FVector SpawnLocation = GetActorLocation() + FVector(0.0f, -1.0f, 30.0f); 
        FRotator SpawnRotation = FRotator::ZeroRotator; 
        GetWorld()->SpawnActor<AProjectile>(ProjectileToSpawn, SpawnLocation, SpawnRotation);
        if (FireSound)
        {
            UGameplayStatics::PlaySound2D(this, FireSound);
        }
    }
}

//Dash
void ASpaceShip::Dash()
{
    if (CanDash && !CurrentVelocity.IsNearlyZero())
    {
        IsDashing = true;
        CanDash = false;
        
        TriggerGhostTrail();

        //repete le spawn de trail
        GetWorld()->GetTimerManager().SetTimer(GhostTrailTimer, this, &ASpaceShip::TriggerGhostTrail, 0.04f, true);
        
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
        {
            APlayerController* PC = Cast<APlayerController>(GetController());
            if (PC)
            {
                DisableInput(PC);
            }
            OnShipDamaged(0);
            SetActorHiddenInGame(true);
            SetActorEnableCollision(false);
        }
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

void ASpaceShip::TriggerGhostTrail()
{
    OnSpawnGhostTrail();
}

void ASpaceShip::StopDash()
{
    IsDashing = false;
    GetWorld()->GetTimerManager().ClearTimer(GhostTrailTimer);
}

void ASpaceShip::ResetDashCooldown()
{
    CanDash = true;
}

void ASpaceShip::UseBonus(int32 IdBonus)
{
    Score+=100;
    if (IdBonus == 0) //Bonus Rocket
    {
        bIsRocketActive = true;
        GetWorld()->GetTimerManager().SetTimer(RocketBonusTimer, this, &ASpaceShip::DeactivateRocketBonus, RocketBonusDuration, false);
    }
    else if (IdBonus == 1)
    {
        Heal();
    }
}

void ASpaceShip::DeactivateRocketBonus()
{
    bIsRocketActive = false; //fin du timer, arret des rockets
}

void ASpaceShip::Heal()
{
    if (CurrentHp < BASE_HP)
    {
        CurrentHp++;

        if (HealSound)
        {
            UGameplayStatics::PlaySound2D(this, HealSound);
        }
        OnShipDamaged(CurrentHp);
    }
}