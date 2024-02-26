// Fill out your copyright notice in the Description page of Project Settings.


#include "items/item.h"
#include "Slash/Public/AllMacro.h"
#include "Components/SphereComponent.h"

// Sets default values
Aitem::Aitem() //: fAmplitude(0.25f)	// init
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	/*fAmplitude = 0.25f;
	fTimeConstant = 5.0f;*/
}

// Called when the game starts or when spawned
void Aitem::BeginPlay()
{
	Super::BeginPlay();
	
	/*Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);*/

	//UWorld* world = GetWorld();
	//FVector Location = GetActorLocation();
	//FVector ForWard = GetActorForwardVector();
	//
	//SetActorLocation(FVector(0, 0, 50.0));
	//SetActorRotation(FRotator(0, 45.e0, 0));

	//DRAW_SPHERE(Location);
	//DRAW_VECTOR(Location, Location + ForWard * 100.0f);

	// TODO : InlineTemplate
	/*int32 AvgInt = Avg<int32>(1, 3);
	UE_LOG(LogTemp, Warning, TEXT("Avg of 1 and 3 : %d"), AvgInt);*/
}

//void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	const FString OtherActorName = OtherActor->GetName();
//	if (GEngine)
//	{
//		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, OtherActorName);
//	}
//}
//
//void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	const FString OtherActorName = FString("Ending Overlap with: ") + OtherActor->GetName();
//	if (GEngine)
//	{
//		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Blue, OtherActorName);
//	}
//}

// Called every frame
void Aitem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	// TODO : InlineTemplate
	//float move = 50.0f;
	//float moveRot = 45.0f;

	//fRunningTime += DeltaTime;

	//float fDelta_Z = fAmplitude * FMath::Sin(fRunningTime * fTimeConstant);		// 5.0f == speed

	//AddActorWorldOffset(FVector(0, 0, fDelta_Z));

	//DRAW_SPHERE_SingleFrame(GetActorLocation());
	//DRAW_VECTOR_SingleFrame(GetActorLocation(), GetActorLocation() * GetActorForwardVector() * 100.0f);

	//FVector AvgVector = Avg<FVector>(GetActorLocation(), FVector::ZeroVector);
	//DRAW_POINT_SingleFrame(AvgVector);

}




