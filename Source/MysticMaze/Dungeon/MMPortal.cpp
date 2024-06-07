// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon/MMPortal.h"
#include "Player/MMPlayerController.h"
#include "Collision/MMCollision.h"
#include "UI/MMHUDWidget.h"

#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMMPortal::AMMPortal()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> LoadingBPClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MysticMaze/UI/WBP_Loading.WBP_Loading_C'"));
	if (LoadingBPClass.Class != NULL)
	{
		LoadingWidgetClass = LoadingBPClass.Class;
	}

	// 상호작용 범위
	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Collsion"));
	Trigger->InitSphereRadius(600.0f);
	Trigger->SetCollisionProfileName(MMTRIGGER);
	RootComponent = Trigger;

	Particle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle"));
	Particle->SetupAttachment(Trigger);

	HelpText = TEXT("Press F to use the Portal.");
	OpenLevelName = TEXT("TownLevel");
}

// Called when the game starts or when spawned
void AMMPortal::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMMPortal::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMMPortal::OnBeginOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &AMMPortal::OnEndOverlap);
}

void AMMPortal::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);

	if (PlayerCharacter)
	{
		// 플레이어 HUD 위젯에 접근하여 InteractionWidget을 활성화 시켜줍니다.
		AMMPlayerController* PlayerController = Cast<AMMPlayerController>(PlayerCharacter->GetController());
		if (PlayerController)
		{
			PlayerController->GetHUDWidget()->InteractionWidgetHelpText(HelpText);
			PlayerController->GetHUDWidget()->ToggleInteractionWidget(true);
		}
	}
}

void AMMPortal::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);

	if (PlayerCharacter)
	{
		// 플레이어 HUD 위젯에 접근하여 InteractionWidget을 비활성화 시켜줍니다.
		AMMPlayerController* PlayerController = Cast<AMMPlayerController>(PlayerCharacter->GetController());
		if (PlayerController)
		{
			PlayerController->GetHUDWidget()->ToggleInteractionWidget(false);
		}
	}
}

void AMMPortal::Interaction(ACharacter* PlayerCharacter)
{
	UUserWidget* LoadingWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingWidgetClass);
	if (LoadingWidget)
	{
		LoadingWidget->AddToViewport();

		LoadingTimerHandle.Invalidate();
		GetWorld()->GetTimerManager().SetTimer(LoadingTimerHandle, this, &AMMPortal::OpenLevel, 1.0f, false);
	}
}

void AMMPortal::OpenLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), OpenLevelName);
}
