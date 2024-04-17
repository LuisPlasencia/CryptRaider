// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"

#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation();
	
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

// EJERCICIOS DE PUNTEROS: 
	// AActor* Owner = GetOwner();
	// FString Name = Owner->GetActorNameOrLabel();   // easier to use arrow operator on pointers than the point operator
	// FString Name = (*Owner).GetActorNameOrLabel();

	// float MyFloat = 5;
	// float* YourFloat = &MyFloat;
	// UE_LOG(LogTemp, Display, TEXT("YourFloat Value: %f"), *YourFloat);
	// UE_LOG(LogTemp, Display, TEXT("Mover Owner Address: %u"), Owner);

	// UE_LOG(LogTemp, Display, TEXT("Mover Owner: %s"), *Name);

	// FVector OwnerLocation;
	// OwnerLocation = Owner->GetActorLocation();
	// FString OwnerLocationString = OwnerLocation.ToCompactString();
	// UE_LOG(LogTemp, Display, TEXT("Mover Owner: %s with location %s"), *Name, *OwnerLocationString);


// CODIGO:

	// mover el código con tab = indent the code 1 level (indentation)
	// ctr + alt + f11  para buildear en unity rapido


	FVector TargetLocation = OriginalLocation;

	if(ShouldMove)
	{
		TargetLocation = OriginalLocation + MoveOffset;
	}

	FVector CurrentLocation = GetOwner()->GetActorLocation();

	float Speed = MoveOffset.Length() / MoveTime;

	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);


	GetOwner()->SetActorLocation(NewLocation);

}

void UMover::SetShouldMove(bool NewShouldMove)
{
	ShouldMove = NewShouldMove;
}
