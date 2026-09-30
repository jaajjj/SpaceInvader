#include "BonusSpawner.h"
#include "../Object/ABonus.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

ABonusSpawner::ABonusSpawner()
{
    //désactive le Tick comme on a le Timer
    PrimaryActorTick.bCanEverTick = false; 
}

void ABonusSpawner::BeginPlay()
{
    Super::BeginPlay();
    TrySpawnBonus();
}

void ABonusSpawner::TrySpawnBonus()
{
    float RandomDelay = FMath::RandRange(MinDelayBonus, MaxDelayBonus);
    GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ABonusSpawner::SpawnBonus, RandomDelay, false);}

void ABonusSpawner::SpawnBonus()
{
    if (BonusClasses.Num() > 0) 
    {
        int32 RandomIndex = FMath::RandRange(0, BonusClasses.Num() - 1); //Choisi un Bonus random, pour l'instant y'en a qu'1
        TSubclassOf<ABonus> SelectedClass = BonusClasses[RandomIndex];
        int32 SpawnSide = FMath::RandRange(0, 2); //0=haut, 1=gauche, 2=droite
        FVector SpawnLocation;

        float margeSpawn = 50.0f;
        if (SpawnSide == 0) //haut
        {
            SpawnLocation = FVector(FMath::RandRange(MinX + margeSpawn, MaxX - margeSpawn), 0.0f, MaxZ + 150.0f);
        }
        else if (SpawnSide == 1) //gauche
        {
            SpawnLocation = FVector(MinX - 150.0f, 0.0f, FMath::RandRange(MinZ + margeSpawn, MaxZ - margeSpawn)); 
        }
        else //droite
        {
            SpawnLocation = FVector(MaxX + 150.0f, 0.0f, FMath::RandRange(MinZ + margeSpawn, MaxZ - margeSpawn));
        }

        FRotator SpawnRotation = FRotator::ZeroRotator;
        ABonus* NewBonus = GetWorld()->SpawnActor<ABonus>(SelectedClass, SpawnLocation, SpawnRotation);

        if (NewBonus)
        {
            NewBonus->AreneMinX = this->MinX;
            NewBonus->AreneMaxX = this->MaxX;
            NewBonus->AreneMinZ = this->MinZ;
            NewBonus->AreneMaxZ = this->MaxZ;
            
            //vise le spaceShip
            APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
            if (PlayerPawn)
            {
                FVector DirectionToPlayer = PlayerPawn->GetActorLocation() - SpawnLocation;
                DirectionToPlayer.Y = 0.0f; 
                NewBonus->SphereHitbox->SetPhysicsLinearVelocity(DirectionToPlayer.GetSafeNormal() * NewBonus->MoveSpeed);
            }
        }
    }
    TrySpawnBonus();
}