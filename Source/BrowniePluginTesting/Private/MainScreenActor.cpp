// Fill out your copyright notice in the Description page of Project Settings.


#include "MainScreenActor.h"

// Sets default values
AMainScreenActor::AMainScreenActor() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainScreenActor::BeginPlay() {
	Super::BeginPlay();

	if (controlUserWidget) {
		controlUserWidget->AddToViewport(1);
	}
	
}

// Called every frame
void AMainScreenActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

