// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Components/ComboBoxString.h"
#include "ControlsUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class BROWNIEPLUGINTESTING_API UControlsUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void onStartLocationClicked();

	UFUNCTION()
	void onStopLocationClicked();

	UFUNCTION()
	void onShowToastClicked();

	UFUNCTION()
	void onTakeScreenshotClicked();

	UFUNCTION()
	void onInitTextToSpeechClicked();

	UFUNCTION()
	void onTextToSpeechClicked();

	UFUNCTION()
	void onLaunchWebviewClicked();

	UFUNCTION()
	void onInAppReviewClicked();

	UFUNCTION()
	void onOpenSettingsClicked();

	UFUNCTION()
	void onPhoneCallClicked();

	void onPhoneCallPermissionGranted(bool granted);

	void onLocationUpdate(float latitude, float longitude, float accuracy, FString provider, bool isFromLastKnownLocation);

	void onLocationPermissionGranted(bool granted);

	void onTakeScreenshotFinished(FString screenshotPath);

	void onTextToSpeechInitFinished(bool success);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* StartLocationBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* StopLocationBtn;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ShowToastBtn;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UEditableText* MessageTxt;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ScreenshotBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* OutputTxt;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* TextToSpeechBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* InitTextToSpeechBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* LaunchWebviewBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UEditableText* UrlTxt;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* InAppReviewBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* OpenSettingsBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UComboBoxString* OpenSettingsCombo;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UEditableText* PhoneCallTxt;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* PhoneCallBtn;
	
};
