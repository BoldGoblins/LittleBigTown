
#include "ConstructibleSlot.h"

#include "Engine/StaticMesh.h"

#include "MainPlayerController.h"

#include "Kismet/GameplayStatics.h"



// Sets default values
AConstructibleSlot::AConstructibleSlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Delegates :
	OnClicked.AddUniqueDynamic(this, &AConstructibleSlot::OnSlotClicked);

	// Tag used for left clic mouse events
	Tags.Add(FName("ConstructibleSlot"));

}

void AConstructibleSlot::OnSlotClicked(AActor* Target, FKey ButtonPressed)
{
	if (MainPlayerControllerRef)
		MainPlayerControllerRef->OnSlotClickedDelegate.Broadcast(this);
}

// Called when the game starts or when spawned
void AConstructibleSlot::BeginPlay()
{
	Super::BeginPlay();

	MainPlayerControllerRef = Cast <AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
}

// Called every frame
void AConstructibleSlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

