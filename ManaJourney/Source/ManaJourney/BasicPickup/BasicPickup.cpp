#include "ManaJourney.h"
#include "BasicPickup.h"


// Sets default values
ABasicPickup::ABasicPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->AttachParent = GetStaticMeshComponent();
	TriggerBox->bGenerateOverlapEvents = true;

	//TriggerBox->OnComponentBeginOverlap() HOW TO DO THIS IN C++


	SetActorEnableCollision(true);
}

// Called when the game starts or when spawned
void ABasicPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicPickup::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

