// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMMonsterBase.h"

#include "Collision/MMCollision.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Monster/Monster_DamageText/MM_MonsterDamageText.h"

#include "UI/MMWidgetComponent.h"
#include "UI/MMMonsterHPBar.h"

AMMMonsterBase::AMMMonsterBase()
{
	ATK_Mode = false;
	bDie = false;
	HP_Percent = 1.0f;

	LocationData = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Save Location"));

	HpBar = CreateDefaultSubobject<UMMWidgetComponent>(TEXT("Monster HP"));
	// HpBar�� ��ġ�� �����մϴ�. (ĳ���� �Ӹ� ��)
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	// HpBar�� Ŭ���� ���� �����ϱ�
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MysticMaze/UI/WBP_MonsterHPBar.WBP_MonsterHPBar_C'"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		// ���� ���� 2D�� ����(Screen)
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		// ���� ũ�� ����
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		// ���� �浹 ����
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

float AMMMonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UE_LOG(LogTemp, Warning, TEXT("IN Damage : %f"), DamageAmount);

	// ������ �����ϱ�
	Stat->ApplyDamage(DamageAmount);

	HP_Percent = Stat->GetCurrentHp() / Stat->GetMaxHp();
	UE_LOG(LogTemp, Display, TEXT("Cur Percent : %f"), HP_Percent);

	FVector spawnLocation = LocationData->GetComponentLocation() + this->GetActorLocation();

	FVector CameraLocation;
	FRotator CameraRotation;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
	}
	FTransform Transform;
	Transform.SetLocation(spawnLocation);
	AMM_MonsterDamageText* DamageText = GetWorld()->SpawnActor<AMM_MonsterDamageText>(Monster_Damage, Transform);
	DamageText->DamageText(DamageAmount, CameraLocation);

	return DamageAmount;

	// TODO : ���� ������ ��Ʈ �Լ��� ���� �Լ� ���� �۾��� ���� �ؾ���
}

void AMMMonsterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// ��������Ʈ ����
	Stat->OnMovementSpeedChanged.AddUObject(this, &AMMMonsterBase::ApplyMovementSpeed);
	Stat->OnHpZero.AddUObject(this, &AMMMonsterBase::Die);
	Stat->OnHit.AddUObject(this, &AMMMonsterBase::Hit);

	// ���� ������Ʈ �ʱ�ȭ
	Stat->Init();

	// ��ƼŬ ��Ȱ��ȭ
	// ChargeParticleSystemComponent->SetActive(false);
}

void AMMMonsterBase::MonsterDieMontage()
{

}

// ���� ���ݿ� ���õ� �Լ�
void AMMMonsterBase::ATKBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMMMonsterBase* Casting = Cast<AMMMonsterBase>(OtherActor))
	{
		return;
	}

	UGameplayStatics::ApplyDamage(OtherActor,
		Stat->GetAttackDamage(), GetController(),
		this,
		UDamageType::StaticClass());
}
void AMMMonsterBase::ATKEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATK_Collision->SetCollisionProfileName(TEXT("NoCollision"));
}

void AMMMonsterBase::SetCenterLocation(FVector InLocation)
{
	AAIController* MyController = Cast<AAIController>(GetController());
	if (MyController)
	{
		MyController->GetBlackboardComponent()->SetValueAsVector("SpawnLocation", InLocation);
	}
}

void AMMMonsterBase::SetStatLevel(int32 INData)
{
	Stat->SetLevel(INData);
}

void AMMMonsterBase::ApplyMovementSpeed(float MovementSpeed)
{

}

void AMMMonsterBase::Die()
{
	UE_LOG(LogTemp, Display, TEXT("Dying"));
	if (!bDie)
	{
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));

		bDie = true;
		GetCharacterMovement()->StopMovementImmediately();
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			AIController->GetBlackboardComponent()->SetValueAsBool("Die", true);
			MonsterDieMontage();
		}
	}
}

void AMMMonsterBase::Hit()
{
	UE_LOG(LogTemp, Display, TEXT("Hiting"));

	if (HitMontage)
	{
		ATK_Mode = true;
		MonsterHitAnim();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("hit Anim Not"));
	}
}

void AMMMonsterBase::MonsterHitAnim()
{
}

void AMMMonsterBase::SetupCharacterWidget(UMMCustomWidget* InUserWidget)
{
	UMMMonsterHPBar* HP = Cast<UMMMonsterHPBar>(InUserWidget);
	if (HP)
	{
		Stat->OnHpChanged.AddUObject(HP, &UMMMonsterHPBar::UpdateHPBar);
	}
}
