// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{

	Super::BeginPlay();

	// Ctr + shift + b  to build  from visual studio code
	// ctr + alt + f11  to build from the editor

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

// Saber donde apunta la camara (Scene Component)
	// FRotator MyRotation = GetComponentRotation();
	// FString RotationString = MyRotation.ToCompactString();
	// UE_LOG(LogTemp, Display, TEXT("Grabber Rotation: %s"), *RotationString);

	// float Time = GetWorld()->TimeSeconds;  // Ctr + space for the list of functions 
	// UE_LOG(LogTemp, Display, TEXT("Current Time is: %f"), Time);

// Ejercicios pointer vs reference
	// float Damage = 0;
	// float& DamageRef = Damage;  // reference (sintaxis mas facil de usar que puntero y no se puede reescribir la direccion por lo que es mas seguro) 
	// DamageRef = 5;
	// UE_LOG(LogTemp, Display, TEXT("DamageRef: %f, Damage: %f"), DamageRef, Damage);    // los dos son 5 

	// PrintDamage(Damage);
	// UE_LOG(LogTemp, Display, TEXT("Original Damage: %f"), Damage);

	// float Damage = 0;
	// if(HasDamage(Damage))   // OJO a la llamada del metodo
	// {
	// 	PrintDamage(Damage);
	// }

// Codigo

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();  // < > = compile time arguments (angle brackets),  () = run time arguments (parenthesis)
	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}



}


void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();  // < > = compile time arguments (angle brackets),  () = run time arguments (parenthesis)
	if(PhysicsHandle == nullptr)
	{
		return;
	}

	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);

	if(HasHit)
	{
	// DEBUG
		// DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Green, false, 5);
		// DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, 5);
		// AActor* HitActor = HitResult.GetActor();
		// UE_LOG(LogTemp, Display, TEXT("Hit actor: %s"), *HitActor->GetActorNameOrLabel());

	// CODE
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		AActor* HitActor = HitResult.GetActor();
		HitActor->Tags.Add("Grabbed");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);

	}
}


void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();  // < > = compile time arguments (angle brackets),  () = run time arguments (parenthesis)

	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent())  //  == los nullptr dan false,  el resultado de la derecha solo se calcula si la izquierda no es nullptr, si no rompería el programa al acceder a un null
	{
		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();
	}
	

}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(Result == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Grabber Requires a UPhysicsHandleComponent"));
	}
	return Result;
}


bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(
		OutHitResult, 
		Start, End, 
		FQuat::Identity, 
		ECC_GameTraceChannel2, 
		Sphere);   // Trace Collision Channel:  Config/DefaultEngine.ini -> Channel of Name="Grabber"   Grabber es un canal de colisión que creamos anteriormente  ProjectSettings/Collisions/TraceChannel
	
}

// void UGrabber::PrintDamage(float& Damage)
// {
// 	// Damage = 2;
// 	UE_LOG(LogTemp, Display, TEXT("Damage: %f"), Damage);
// }

// bool UGrabber::HasDamage(float& OutDamage)
// {
// 	OutDamage = 5;
// 	return true;
// }

