// Fill out your copyright notice in the Description page of Project Settings.


#include "Controls/ControlsUserWidget.h"
#include "BrowniePubsub.h"

void UControlsUserWidget::NativeConstruct() {
	StartLocationBtn->OnClicked.AddDynamic(this, &UControlsUserWidget::onStartLocationClicked);
	StopLocationBtn->OnClicked.AddDynamic(this, &UControlsUserWidget::onStopLocationClicked);
	ShowToastBtn->OnClicked.AddDynamic(this, &UControlsUserWidget::onShowToastClicked);
	ScreenshotBtn->OnClicked.AddDynamic(this, &UControlsUserWidget::onTakeScreenshotClicked);
	TextToSpeechBtn->OnClicked.AddDynamic(this, &UControlsUserWidget::onTextToSpeechClicked);
	InitTextToSpeechBtn->OnClicked.AddDynamic(this, &UControlsUserWidget::onInitTextToSpeechClicked);
	LaunchWebviewBtn->OnClicked.AddDynamic(this, &UControlsUserWidget::onLaunchWebviewClicked);
	InAppReviewBtn->OnClicked.AddDynamic(this, &UControlsUserWidget::onInAppReviewClicked);
	OpenSettingsBtn->OnClicked.AddDynamic(this, &UControlsUserWidget::onOpenSettingsClicked);
	PhoneCallBtn->OnClicked.AddDynamic(this, &UControlsUserWidget::onPhoneCallClicked);
	EncryptedPrefs->OnClicked.AddDynamic(this, &UControlsUserWidget::onEncryptedPrefClicked);

	BrowniePubsub* browniePubsub = BrowniePubsub::getInstance();
	browniePubsub->onLocationUpdate.AddUObject(this, &UControlsUserWidget::onLocationUpdate);
	browniePubsub->onLocationPermissionGranted.AddUObject(this, &UControlsUserWidget::onLocationPermissionGranted);
	browniePubsub->onTakeScreenshotFinished.AddUObject(this, &UControlsUserWidget::onTakeScreenshotFinished);
	browniePubsub->onTextToSpeechInitFinished.AddUObject(this, &UControlsUserWidget::onTextToSpeechInitFinished);
	browniePubsub->onPhoneCallPermissionGranted.AddUObject(this, &UControlsUserWidget::onPhoneCallPermissionGranted);
}

void UControlsUserWidget::onStartLocationClicked() {
#if PLATFORM_ANDROID
	extern int BrownieThunkCpp_StartGettingLocationUpdates(int);
	int intervalInMillis = 2500;
	BrownieThunkCpp_StartGettingLocationUpdates(intervalInMillis);
#endif
}

void UControlsUserWidget::onStopLocationClicked() {

#if PLATFORM_ANDROID
	extern void BrownieThunkCpp_StopGettingLocationUpdates();
	BrownieThunkCpp_StopGettingLocationUpdates();
#endif
}

void UControlsUserWidget::onShowToastClicked() {

	FString toastMessage = MessageTxt->GetText().ToString();

#if PLATFORM_ANDROID
	extern void BrownieThunkCpp_ShowToast(FString message, int32 length);
	BrownieThunkCpp_ShowToast(toastMessage, 1);
#endif	

}

void UControlsUserWidget::onTakeScreenshotClicked() {

#if PLATFORM_ANDROID
	extern void BrownieThunkCpp_TakeScreenshot(FString screenshotFileName);
	BrownieThunkCpp_TakeScreenshot(FString(TEXT("my_game_picture")));
#endif	

}

void UControlsUserWidget::onInitTextToSpeechClicked() {

#if PLATFORM_ANDROID
	extern void BrownieThunkCpp_InitializeTextToSpeech(FString language, FString country);
	BrownieThunkCpp_InitializeTextToSpeech(FString(TEXT("ENG")), FString(TEXT("US")));
#endif	

}

void UControlsUserWidget::onTextToSpeechClicked() {

	FString textToSpeechMessage = MessageTxt->GetText().ToString();

#if PLATFORM_ANDROID
	extern void BrownieThunkCpp_TextToSpeech(FString message);
	BrownieThunkCpp_TextToSpeech(textToSpeechMessage);
#endif	

}

void UControlsUserWidget::onLocationUpdate(float latitude, float longitude, float accuracy, FString provider, bool isFromLastKnownLocation) {

	FString commandToBeSent = FString::Printf(TEXT("Latitude: %f, Longitude: %f, Accuracy: %f"),
		latitude, longitude, accuracy);
	OutputTxt->SetText(FText::FromString(commandToBeSent));
}

void UControlsUserWidget::onLocationPermissionGranted(bool granted) {
	OutputTxt->SetText(FText::FromString(FString(TEXT("User has granted the permission, go ahead and start location fetching..."))));
}

void UControlsUserWidget::onTakeScreenshotFinished(FString screenshotPath) {
	OutputTxt->SetText(FText::FromString(FString::Printf(TEXT("Image Stored on: %s"), *screenshotPath)));
}

void UControlsUserWidget::onTextToSpeechInitFinished(bool success) {
	if (success) {
		OutputTxt->SetText(FText::FromString(FString(TEXT("TextToSpeechInit Was Successful, you can now use TextToSpeech"))));
	}
	else {
		OutputTxt->SetText(FText::FromString(FString(TEXT("TextToSpeechInit Failed..."))));

	}
}

void UControlsUserWidget::onLaunchWebviewClicked() {

	FString toastMessage = UrlTxt->GetText().ToString();

#if PLATFORM_ANDROID
	extern void BrownieThunkCpp_LaunchWebview(FString message);
	BrownieThunkCpp_LaunchWebview(toastMessage);
#endif	

}

void UControlsUserWidget::onInAppReviewClicked() {

#if PLATFORM_ANDROID
	extern void BrownieThunkCpp_InAppReview();
	BrownieThunkCpp_InAppReview();
#endif	

}

void UControlsUserWidget::onOpenSettingsClicked() {

	int32 selectedSetting = OpenSettingsCombo->GetSelectedIndex();

#if PLATFORM_ANDROID
	extern void BrownieThunkCpp_OpenSettings(int32 selectedSetting);
	BrownieThunkCpp_OpenSettings(selectedSetting);
#endif	

}

void UControlsUserWidget::onPhoneCallClicked() {

	FString phoneNumber = PhoneCallTxt->GetText().ToString();

#if PLATFORM_ANDROID
	extern void BrownieThunkCpp_StartPhoneCall(FString phoneNumber);
	BrownieThunkCpp_StartPhoneCall(phoneNumber);
#endif	

}

void UControlsUserWidget::onEncryptedPrefClicked() {

#if PLATFORM_ANDROID
	FString prefFileName = FString(TEXT("fake_user_profile"));

	// ===================================== PUTS =====================================
	extern void BrownieThunkCpp_PutEncryptedString(FString preferencesCollectionName, FString preferenceKey, FString preferenceValue);
	BrownieThunkCpp_PutEncryptedString(prefFileName, FString(TEXT("displayName")), FString(TEXT("Wizzvy!")));

	extern void BrownieThunkCpp_PutEncryptedInt(FString preferencesCollectionName, FString preferenceKey, int32 preferenceValue);
	BrownieThunkCpp_PutEncryptedInt(prefFileName, FString(TEXT("age")), (int32) 123);

	extern void BrownieThunkCpp_PutEncryptedLong(FString preferencesCollectionName, FString preferenceKey, long preferenceValue);
	BrownieThunkCpp_PutEncryptedLong(prefFileName, FString(TEXT("timestamp")), (long) 12387234687236);

	extern void BrownieThunkCpp_PutEncryptedFloat(FString preferencesCollectionName, FString preferenceKey, float preferenceValue);
	BrownieThunkCpp_PutEncryptedFloat(prefFileName, FString(TEXT("ratio")), 1.23456f);

	extern void BrownieThunkCpp_PutEncryptedBool(FString preferencesCollectionName, FString preferenceKey, bool preferenceValue);
	BrownieThunkCpp_PutEncryptedBool(prefFileName, FString(TEXT("enabled")), true);

	//===================================== GETS =====================================
	extern FString BrownieThunkCpp_GetEncryptedString(FString preferencesCollectionName, FString preferenceKey);
	FString stringVal = BrownieThunkCpp_GetEncryptedString(prefFileName, FString(TEXT("displayName")));

	extern int32 BrownieThunkCpp_GetEncryptedInt(FString preferencesCollectionName, FString preferenceKey);
	int32 intVal = BrownieThunkCpp_GetEncryptedInt(prefFileName, FString(TEXT("age")));

	extern long BrownieThunkCpp_GetEncryptedLong(FString preferencesCollectionName, FString preferenceKey);
	long longVal = BrownieThunkCpp_GetEncryptedLong(prefFileName, FString(TEXT("timestamp")));

	extern float BrownieThunkCpp_GetEncryptedFloat(FString preferencesCollectionName, FString preferenceKey);
	float floatVal = BrownieThunkCpp_GetEncryptedFloat(prefFileName, FString(TEXT("ratio")));

	extern bool BrownieThunkCpp_GetEncryptedBool(FString preferencesCollectionName, FString preferenceKey);
	bool boolVal = BrownieThunkCpp_GetEncryptedBool(prefFileName, FString(TEXT("enabled")));

	OutputTxt->SetText(FText::FromString(FString::Printf(TEXT("Prefs - str: %s, int: %d, long: %ld, float: %f, bool: %d"), *stringVal, intVal, longVal, floatVal, boolVal)));
#endif	

}

void UControlsUserWidget::onPhoneCallPermissionGranted(bool granted) {
	OutputTxt->SetText(FText::FromString(FString(TEXT("User has granted the permission, go ahead and make the call..."))));
}



