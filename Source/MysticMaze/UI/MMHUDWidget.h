// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MMCustomWidget.h"
#include "MMHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMHUDWidget : public UMMCustomWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UMMInventoryWidget> InventoryWidget;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UMMInteractionWidget> InteractionWidget;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UMMPlayerStatusBarWidget> PlayerStatusBarWidget;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UMMStatusWidget> PlayerStatusWidget;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UMMEquipmentWidget> EquipmentWidget;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UMMSkillWidget> SkillWidget;

public:
	void Init();

	void ToggleInventoryWidget();
	void ToggleStatusWidget();
	void ToggleEquipmentWidget();
	void ToggleSkillWidget();
	void ToggleInteractionWidget(bool InValue);
	void InteractionWidgetHelpText(FString HelpText);

	// 위젯 활성화 여부 반환 함수 (활성화 된 위젯이 있는 경우 true 반환)
	FORCEINLINE bool GetIsVisibility() { return VisibilityFlag != static_cast<uint8>(EWidgetFlags::None); }

private:
	enum class EWidgetFlags
	{
		None		= 0,		// 0000 0000 - 초기화
		INVENTORY	= 1 << 0,	// 0000 0001 - 인벤토리 위젯
		STATUS		= 1 << 1,	// 0000 0010 - 스테이터스 위젯
		EQUIPMENT	= 1 << 2,	// 0000 0100 - 장비 위젯
		SKILL		= 1 << 3,   // 0000 1000 - 스킬 위젯
	};

	// 위젯이 열려있는지 판별하기 위한 비트플래그
	uint8 VisibilityFlag;
};
