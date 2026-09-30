#include "AsteroidSpawner.h"
#include "../Object/AAsteroid.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

AAsteroidSpawner::AAsteroidSpawner()
{
    //désactive le Tick comme on a le Timer
    PrimaryActorTick.bCanEverTick = false; 
}

void AAsteroidSpawner::BeginPlay()
{
    Super::BeginPlay();
    
    //Tick de 1 sec
    GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &AAsteroidSpawner::TrySpawnAsteroid, 1.0f, true);
}

void AAsteroidSpawner::TrySpawnAsteroid()
{
    SecondesEcoulees++;
    
    float rand = FMath::RandRange(0.0f, 100.0f);
    float formuleDifficulte = 10 + FMath::Sqrt(static_cast<float>(7*SecondesEcoulees)); //f(x)=10+SQRT(7x)

    if (rand < formuleDifficulte)
    {
        SpawnAsteroid();
    }
}

void AAsteroidSpawner::SpawnAsteroid()
{
    if (AsteroidClasses.Num() > 0) 
    {
        int32 RandomIndex = FMath::RandRange(0, AsteroidClasses.Num() - 1); //Choisi un asteroid random parmis les 3
        TSubclassOf<AAsteroid> SelectedClass = AsteroidClasses[RandomIndex];
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
        AAsteroid* NewAsteroid = GetWorld()->SpawnActor<AAsteroid>(SelectedClass, SpawnLocation, SpawnRotation);

        if (NewAsteroid)
        {
            NewAsteroid->AreneMinX = this->MinX;
            NewAsteroid->AreneMaxX = this->MaxX;
            NewAsteroid->AreneMinZ = this->MinZ;
            NewAsteroid->AreneMaxZ = this->MaxZ;
            
            //vise le spaceShip
            APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
            if (PlayerPawn)
            {
                FVector DirectionToPlayer = PlayerPawn->GetActorLocation() - SpawnLocation;
                DirectionToPlayer.Y = 0.0f; 
                NewAsteroid->SphereHitbox->SetPhysicsLinearVelocity(DirectionToPlayer.GetSafeNormal() * NewAsteroid->MoveSpeed);
            }
        }
    }
}