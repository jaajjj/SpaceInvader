// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_Game.h"
#include "Kismet/GameplayStatics.h"

#include <functional>

void UGI_Game::AddScore(int32 Val)
{
	Score += Val;
}


void UGI_Game::ResetGame()
{
	Score = 0;
}
