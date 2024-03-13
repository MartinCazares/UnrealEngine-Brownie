// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "MainScreenActor.generated.h"

UCLASS()
class BROWNIEPLUGINTESTING_API AMainScreenActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainScreenActor();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UUserWidget* controlUserWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
