
#include "ConstructibleSlot.h"

#include "Engine/StaticMesh.h"

#include "LittleBigTown/GameSystem/MainPlayerController.h"

#include "Kismet/GameplayStatics.h"



// Sets default values
AConstructibleSlot::AConstructibleSlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Delegates :
	// OnClicked.AddUniqueDynamic(this, &AConstructibleSlot::OnSlotClicked);

	// Tag used for left clic mouse events
	Tags.Add(FName("ConstructibleSlot"));

}

// Called when the game starts or when spawned
void AConstructibleSlot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConstructibleSlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

