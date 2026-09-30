// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GI_Game.generated.h"

/**
 * 
 */
UCLASS()
class TESTCPP_API UGI_Game : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(BlueprintReadOnly, Category = "Game Data")
	int32 Score = 0;

public:
	UFUNCTION(BlueprintPure, Category = "Game Data")
	int32 GetScore() const { return Score; }
	
	UFUNCTION(BlueprintCallable, Category = "Game Logic")
	void AddScore(int32 Sc = 1); 

	UFUNCTION(BlueprintCallable, Category = "Game Logic")
	void ResetGame();
};
