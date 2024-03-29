# Brownie

Brownie is an Unreal Engine Plugin that works as a Bridge between two different worlds. With this plugin you can easily take actions on Android's Ecosystem from Unreal's C++.

# Release Notes

**(NEW) - v1.1.0** (Added Biometrics / Text Sharing / Immediate In App updates)

**v1.0.5** (Added Encrypted Shared Preferences / Added user cancelled permission event)
**v1.0.1** (First release with Location Services / InAppReview / Toasts / WebViews / etc.)

(For sample project help, go to the end of this document)

# Actions

 - **Location Services** (Latitude/Longitude/Accuracy/etc)
 - **In App Review** (start the Reviewing Flow seamlessly)
 - **Biometrics Verification** (Fingerprint or Pin validation)
 - **In App Updates** (trigger the flow to update the app, from within the app itself...)
 - **Show Toasts Messages**
 - **Text To Speech** (pass an FString and convert it to voice)
 - **Encrypted Shared Preferences**
 - **Screenshots** (whatever Unreal Engine is showing we can capture it)
 - **Open WebView** (launch an Internal Web Browser with any URL you want)
 - **Android Settings** (show the default settings screen on the device or redirect to specific settings (e.g WiFi/Location/Date and Time/etc))
 - **Phone Call** (bring the Dialer with a predefined phone number ready to start the call)
 - **Share Text** (any text in your game can be shared with any of the elegible apps (e.g Email, Clipboard, Whatsapp, etc))

**Note:** If there's any other actions you would like to be added, let me know. (create an issue with the request)

All these Capabilities can be Enabled/Disabled on Unreal Engine's Editor, just go to Edit -> Project Settings -> Plugins -> Brownie.

![PluginEnabledCapabilities](https://github.com/MartinCazares/UnrealEngine-Brownie/assets/8989676/44816051-c730-4ff5-b01d-8c8fc4dd2de8)


![ViewPortScreen](https://github.com/MartinCazares/UnrealEngine-Brownie/assets/8989676/4fecf787-25d7-45b2-af5f-606127ec87f3)



After you have added the plugin, please make sure that you have the following line in your {ProjectName}.Build.cs:

    PrivateDependencyModuleNames.AddRange(new string[] { "BrownieBridge" });

# Location Service
This capability allows you to fetch continuous updates of the actual Location of the device using the best provider available in a very simple manner.

## Documentation
**Start Fetching Location**

    /**
	    Param int locationFetchIntervalInMillis,
    	Return: {
    		FETCHING_IN_PROGRESS -> 0x01; //There's a fetching happening right now...
    		REQUESTED_FOR_PERMISSION -> 0x02; //Failed because we have no permissions (register for broadcast to be notified when the permission is granted)
    		FETCHING_LOCATION -> 0x03; //Fetching location started correctly...
    		ACTIVITY_DESTROYING -> 0x04; //Nothing to do, you wanted to fetch for location while the game is on its way out...
    	}
    */
    int BrownieThunkCpp_StartGettingLocationUpdates(int locationFetchIntervalInMillis)
**Stop Fetching Location**

    void BrownieThunkCpp_StopGettingLocationUpdates()
**New Location Received**

*This method gets executed every single time the device tells us that a new Location point has been found, you will stop receiving these updates until the "stop" method is invoked.*

    void UYourComponent::onLocationUpdate(float latitude, float longitude, float accuracy, FString provider, bool isFromLastKnownLocation)

**Location Permission Granted**

*This method lets you know whther if the permission was granted by the user or not, if granted, following calls for the action will not trigger this event, the action will be taken directly(requires registering to the event first).*

    void UYourComponent::onLocationPermissionGranted(bool granted)

## Example - How to use it

After enabling the capability in Brownie's Plugin Settings screen, all you need to do to ***start fetching*** the device's location is this:

First make sure you are *ready to start receiving the location related events* as follows (this code can be added on any class):

    #include "BrowniePubsub.h"
    //...
    //Register to start listening updates...
    BrowniePubsub* browniePubsub = BrowniePubsub::getInstance();
    browniePubsub->onLocationUpdate.AddUObject(this, &UYourComponent::onLocationUpdate);
    browniePubsub->onLocationPermissionGranted.AddUObject(this, &UYourComponent::onLocationPermissionGranted);
    //...
    //...
    //This function will be called every time the Android device tells us it found a new Location...
    void UYourComponent::onLocationUpdate(float latitude, float longitude, float accuracy, FString provider, bool isFromLastKnownLocation) { }
    //...
    //This function will be called after the user granted the permissions
    void UYourComponent::onLocationPermissionGranted(bool granted) {
	    //What to do after the permission is granted? Probably start fetching location? (look at the example below)
	}

Now tell Android to start fetching the device's location as follows (usually from a click UFUNCTION):

    #if PLATFORM_ANDROID
    	extern int BrownieThunkCpp_StartGettingLocationUpdates(int);
    	int intervalInMillis = 2500;
    	BrownieThunkCpp_StartGettingLocationUpdates(intervalInMillis);
    #endif

# In App Review
This capability starts the In App Review Flow on Android. It is totally linked to the natural flow specified in the documentation where depending on the version of your app and whether if it is already published and without a previous review, the dialog might or might not show up. Read the documentation for a better understanding on what to expect ([https://developer.android.com/guide/playcore/in-app-review/test#troubleshooting](documentation)).

## Documentation
**Start In App Review Flow**

    void BrownieThunkCpp_InAppReview()
    
## Example - How to use it

After enabling the capability in Brownie's Plugin Settings screen, all you need to do to ***start  the In App Review*** flow is this:

    #if PLATFORM_ANDROID
    	extern void BrownieThunkCpp_InAppReview();
    	BrownieThunkCpp_InAppReview();
    #endif	
As simple as that, just add that code in a clicked UFUNCTION or anywhere you want the flow to start and that's it.

# Biometrics
This capability allows you to bring the Fingerprint / Face Recognition or Security Pin popup in order to verify that only authorized owners of the device can take specific actions.

## Documentation
**Popup the Verification Screen**

    /**
    * 
    * Params:
    *	- Title (this is the title that will show up in the verification popup)
    *	- Subtitle (this is the subtitle that will show up in the verification popup)
    * 
    */
    void BrownieThunkCpp_StartBiometrics(FString title, FString subtitle)

**Register for Biometric Success Events**

*This method gets executed only if the user successfully authenticated with any of the available biometric options*

    void UYourComponent::onBiometricsSuccessResult(int result) 

*These are the possible results that the biometrics might return:*

    /**
     * Authentication type reported when the user authenticated via
     * an unknown method.
     *
     * <p>This value may be returned on older Android versions due to partial incompatibility
     * with a newer API. It does NOT necessarily imply that the user authenticated with a method
     * other than those represented by {@link #AUTHENTICATION_RESULT_TYPE_DEVICE_CREDENTIAL} and
     * {@link #AUTHENTICATION_RESULT_TYPE_BIOMETRIC}.
     *
     *
     * AUTHENTICATION_RESULT_TYPE_UNKNOWN = -1;
     *
     */

    /**
     * Authentication type reported when the user authenticated by
     * entering their device PIN, pattern, or password.
     *
     *
     * AUTHENTICATION_RESULT_TYPE_DEVICE_CREDENTIAL = 1;
     *
     */

    /**
     * Authentication type reported when the user authenticated by
     * presenting some form of biometric (e.g. fingerprint or face).
     *
     *
     * AUTHENTICATION_RESULT_TYPE_BIOMETRIC = 2;
     *
     */

**Register for Biometric Failed Events**

*This method gets executed based on the actions taken by the user or the current device's limitations*

    void UYourComponent::onBiometricsErrorResult(int errorCode) 

*These are the possible errorCodes that the biometrics might return:*

    /**
     * Unable to determine whether the user can authenticate.
     *
     * <p>This status code may be returned on older Android versions due to partial incompatibility
     * with a newer API. Applications that wish to enable biometric authentication on affected
     * devices may still call {@code BiometricPrompt#authenticate()} after receiving this status
     * code but should be prepared to handle possible errors.
     * 
     *
     ERROR_BIOMETRIC_STATUS_UNKNOWN = -1;
     * 
     */

    /**
     * The user can't authenticate because the specified options are incompatible with the current
     * Android version.
     * 
     *
     ERROR_BIOMETRIC_ERROR_UNSUPPORTED = -2;
     * 
     */

    /**
     * The hardware is unavailable. Try again later.
     * 
     *
     ERROR_HW_UNAVAILABLE = 1;
     * 
     */

    /**
     * The sensor was unable to process the current image.
     * 
     *
     ERROR_UNABLE_TO_PROCESS = 2;
     * 
     */

    /**
     * The current operation has been running too long and has timed out.
     *
     * <p>This is intended to prevent programs from waiting for the biometric sensor indefinitely.
     * The timeout is platform and sensor-specific, but is generally on the order of ~30 seconds.
     * 
     *
     ERROR_TIMEOUT = 3;
     * 
     */

    /**
     * The operation can't be completed because there is not enough device storage remaining.
     * 
     *
     ERROR_NO_SPACE = 4;
     * 
     */

    /**
     * The operation was canceled because the biometric sensor is unavailable. This may happen when
     * the user is switched, the device is locked, or another pending operation prevents it.
     * 
     *
     ERROR_CANCELED = 5;
     * 
     */

    /**
     * The operation was canceled because the API is locked out due to too many attempts. This
     * occurs after 5 failed attempts, and lasts for 30 seconds.
     * 
     *
     ERROR_LOCKOUT = 7;
     * 
     */

    /**
     * The operation failed due to a vendor-specific error.
     *
     * <p>This error code may be used by hardware vendors to extend this list to cover errors that
     * don't fall under one of the other predefined categories. Vendors are responsible for
     * providing the strings for these errors.
     *
     * <p>These messages are typically reserved for internal operations such as enrollment but may
     * be used to express any error that is not otherwise covered. In this case, applications are
     * expected to show the error message, but they are advised not to rely on the message ID, since
     * this may vary by vendor and device.
     * 
     *
     ERROR_VENDOR = 8;
     * 
     */

    /**
     * The operation was canceled because {@link #ERROR_LOCKOUT} occurred too many times. Biometric
     * authentication is disabled until the user unlocks with their device credential (i.e. PIN,
     * pattern, or password).
     * 
     *
     ERROR_LOCKOUT_PERMANENT = 9;
     * 
     */

    /**
     * The user canceled the operation.
     *
     * <p>Upon receiving this, applications should use alternate authentication, such as a password.
     * The application should also provide the user a way of returning to biometric authentication,
     * such as a button.
     * 
     *
     ERROR_USER_CANCELED = 10;
     * 
     */

    /**
     * The user does not have any biometrics enrolled.
     * Note: Optionally when this error is returned, client might be able to use
     * the method "BrownieThunk_RecommendAddingBiometrics", to recommend the user
     * adding a biometric.
     * IMPORTANT: Only works on SDK version 30+ and its very affected by 
     * fragmentation, some vendors don't even launch it)
     * 
     *
     ERROR_NO_BIOMETRICS = 11;
     * 
     */

    /**
     * The device does not have the required authentication hardware.
     * 
     *
     ERROR_HW_NOT_PRESENT = 12;
     * 
     */

    /**
     * The user pressed the negative button.
     * 
     *
     ERROR_NEGATIVE_BUTTON = 13;
     * 
     */

    /**
     * The device does not have pin, pattern, or password set up.
     * 
     * 
     * ERROR_NO_DEVICE_CREDENTIAL = 14;
     */

    /**
     * A security vulnerability has been discovered with one or more hardware sensors. The
     * affected sensor(s) are unavailable until a security update has addressed the issue.
     * 
     * ERROR_SECURITY_UPDATE_REQUIRED = 15;
     */

    /**
     * Generic error, catch all/exceptions/unknown flows "onAuthenticationFailed"...
     * 
     * 
     * ERROR_SECURITY_GENERIC_1 = 16;
     */

    /**
     * Generic error, catch all/exceptions/unknown flows "BiometricPrompt.PromptInfo.Builder"...
     *
     *
     * ERROR_SECURITY_GENERIC_2 = 17;
     */

## Example - How to use it

First make sure you are *ready to start receiving the biometrics result related events* as follows (this code can be added on any class):

    #include "BrowniePubsub.h"
    //...
    //Register to start listening updates...
    BrowniePubsub* browniePubsub = BrowniePubsub::getInstance();
    browniePubsub->onBiometricsErrorResult.AddUObject(this, &UYourComponent::onBiometricsErrorResult);
	browniePubsub->onBiometricsSuccessResult.AddUObject(this, &UYourComponent::onBiometricsSuccessResult);
  
    //...
    //...
    //This function will be called every time there is an error from user input or device limitations...
    void UYourComponent::onBiometricsErrorResult(int errorCode) { 
        //Handle the errors as you wish, error 10 is probably the most commonly used (user canceled verification)
    }
    //...
    //This function will be called after the user successfully identified him self with one of the 
    //methods available on the device (e.g Pin/Password/Fingerprint/Face recognition/etc)...
    void UYourComponent::onBiometricsSuccessResult(int result) {
	    //If you care what method was used for authentication, validate here, otherwise you can just go on
        //totally sure that the user introduced the right credentials on this device...
	}


After enabling the capability in Brownie's Plugin Settings screen, all you need to do to ***ask for user verification*** is calling the function below:

    #if PLATFORM_ANDROID
	    extern void BrownieThunkCpp_StartBiometrics(FString title, FString subtitle);
	    BrownieThunkCpp_StartBiometrics(FString(TEXT("Here goes the title")), FString(TEXT("Here goes the subtitle")));
    #endif	


Just by adding the code above anywhere you want in your project, you would be able to verify that the user is authorized on that device, the process will find the best match for Biometric Verification and will pop up a dialog asking for ( Pin / Password / Fingerprint / FaceRecognition) verification and only if properly authenticated, the "biometric success" event will be fired.


# In App Update
This capability allows you to pop up a Google Play Screen that would start the App Update flow from your own app. This should happen only if there is a newer version of your app in Google Play.

## Documentation
**Popup the In App Update - Google Play screen**

    void BrownieThunkCpp_InAppUpdate()

**Register for In App Update Failed Events**

*This method gets executed based on the actions taken by the user while updating the app*

    void UYourComponent::onInAppUpdateErrorResult(int errorCode) 

*These are the possible errorCodes that the In App Update logic might return:*

    /**
     *
     * Possible Error Codes:
     *
     * RESULT_CANCELED -> 0: The user denied or canceled the update.
     *
     * RESULT_IN_APP_UPDATE_FAILED -> 1: The flow failed either during the user confirmation, the download, or the installation.
     */

## Example - How to use it

First make sure you are *ready to start receiving the In App Update result related events* as follows (this code can be added on any class):

    #include "BrowniePubsub.h"
    //...
    //Register to start listening updates...
    BrowniePubsub* browniePubsub = BrowniePubsub::getInstance();
    browniePubsub->onInAppUpdateErrorResult.AddUObject(this, &UYourComponent::onInAppUpdateErrorResult);
  
    //...
    //...
    //This function will be called every time there is an error from In App Update flow...
    void UYourComponent::onInAppUpdateErrorResult(int errorCode) { 
        //Handle the errors as you wish...
    }


After enabling the capability in Brownie's Plugin Settings screen, all you need to do to ***show In App Update screen*** is calling the function below:

    #if PLATFORM_ANDROID
	    extern void BrownieThunkCpp_InAppUpdate();
	    BrownieThunkCpp_InAppUpdate();
    #endif	


Just by adding the code above anywhere you want in your project, you would be able to start the In App Update flow...


# Show Toasts
This capability displays a Toast Message on Android in a very simple manner.

## Documentation
**Show Toast**

    /**
    	Params:
    		FString message,
    		int length { 0 -> DURATION_SHORT, 1 -> DURATION_LONG },
    */
    void BrownieThunkCpp_ShowToast(FString message, int32 length)
        

## Example - How to use it

After enabling the capability in Brownie's Plugin Settings screen, all you need to do to ***show a Toast Message*** is this:

    FString toastMessage = MessageTxt->GetText().ToString();//FString(TEXT("Message on Toast..."));
    
    #if PLATFORM_ANDROID
    	extern void BrownieThunkCpp_ShowToast(FString message, int32 length);
    	BrownieThunkCpp_ShowToast(toastMessage, 1);
    #endif	

Just by adding the code above anywhere you want in your project you would be able to display Toasts in your Android Device.

# Text To Speech (TTS)
This capability allows you to speak any string on C++ based on the languages setup on the device itself.

## Documentation
**Initialize Text To Speech Service**

*It is necessary in order to use the TextToSpeech service to first initialize it based on a Language and a Country.*

    /**
    	As long as the user has already set this language in the device you can use it...
    
    	Params:
    		FString language {ISO language e.g "ENG", "SPA"},
    		FString country {ISO country e.g "US", "MEX"},
    */
    void BrownieThunkCpp_InitializeTextToSpeech(FString language, FString country)
 
 **Speak the Text**     

    /**
    	Params:
    		FString textToSpeech,
    */
    void BrownieThunkCpp_TextToSpeech(FString textToSpeech)

**TextToSpeech Initialization Result**

*This method gets executed and returns whether if the service initialization on the device was successful or not*

    void UYourComponent::onTextToSpeechInitFinished(bool success)
          

## Example - How to use it

After enabling the capability in Brownie's Plugin Settings screen, all you need to do to use the ***Text To Speech Service*** is this:

First make sure to register for the initialization result broadcast.

    #include "BrowniePubsub.h"
        //...
        //Register to start listening updates...
        BrowniePubsub* browniePubsub = BrowniePubsub::getInstance();
        browniePubsub->onTextToSpeechInitFinished.AddUObject(this, &UYourComponent::onTextToSpeechInitFinished);
        //...
        //...
        //This function will be called when initialization process is done and will let us know if we are ready to start using the TTS service or not.
        void UYourComponent::onTextToSpeechInitFinished(bool success) { }

Now that we are ready to listen for the initialization result, initialize the service with the following code:

    #if PLATFORM_ANDROID
    	extern void BrownieThunkCpp_InitializeTextToSpeech(FString language, FString country);
    	//The text passed will be spoken in English...
    	BrownieThunkCpp_InitializeTextToSpeech(FString(TEXT("ENG")), FString(TEXT("US")));
    #endif	

If the services is successfully initialized (as it should for any common Android device) we can send texts for the device to speak as follows:

    FString textToSpeechMessage = MessageTxt->GetText().ToString(); //FString(TEXT("Text I want Android to speak"));
    
    #if PLATFORM_ANDROID
    	extern void BrownieThunkCpp_TextToSpeech(FString message);
    	BrownieThunkCpp_TextToSpeech(textToSpeechMessage);
    #endif	

# Encrypted Shared Preferences
This capability allows you to store securely information that needs to be stored in the persistence layer. It works exactly the same way as normal Shared Preferences with the only difference that the values and keys within the file created internally in the device are encrypted.

## Documentation
**Store "String/Int/Long/Float/Bool"**

    /**
     params:
     - preferencesCollectionName (e.g: user_profile)
     - preferenceKey (e.g: displayName)
     - preferenceValue (e.g: Wizzvy)
     
     Note: Based on the example values above, this method would store an encrypted
     "key" -displayName with the "value" - Wizzvy on a preferences file named
     "user_profile". This very same file can be used for many other keys if
     necessary, it is a good practice to gather similar key/values under the
     same file, but you can create new files if required.
    
     IMPORTANT: Only the very first time invoking an encrypted preference it could take
     around 150ms to create the Ciphers/Encrypting mechanisms, this must be
     taken into account, if possible use it off the main thread...
     */
     
    void BrownieThunkCpp_PutEncryptedString(FString preferencesCollectionName, FString preferenceKey, FString preferenceValue)
    
    void BrownieThunkCpp_PutEncryptedInt(FString preferencesCollectionName, FString preferenceKey, int32 preferenceValue)
    
    void BrownieThunkCpp_PutEncryptedLong(FString preferencesCollectionName, FString preferenceKey, long preferenceValue)
    
    void BrownieThunkCpp_PutEncryptedFloat(FString preferencesCollectionName, FString preferenceKey, float preferenceValue)
    
    void BrownieThunkCpp_PutEncryptedBool(FString preferencesCollectionName, FString preferenceKey, bool preferenceValue)

**Get "String/Int/Long/Float/Bool"**

    /**
     params:
     - preferencesCollectionName (e.g: user_profile)
     - preferenceKey (e.g: displayName)
     Note: Based on the example values, this method would return the encrypted
     "value" from the "preferenceKey" on a preferences file named
     "user_profile".
    
     return:
     - Encrypted Value previously stored in device or (false/0/"") if non-exists
    
     Note: Only the very first time invoking an encrypted preference it could take
     around 150ms to create the Ciphers/Encrypting mechanisms, this must be taken
     into account, if possible use it off the main thread...
     */
     
    FString BrownieThunkCpp_GetEncryptedString(FString preferencesCollectionName, FString preferenceKey)
    
    int32 BrownieThunkCpp_GetEncryptedInt(FString preferencesCollectionName, FString preferenceKey)
    
    long BrownieThunkCpp_GetEncryptedLong(FString preferencesCollectionName, FString preferenceKey)
    
    float BrownieThunkCpp_GetEncryptedFloat(FString preferencesCollectionName, FString preferenceKey)
    
    bool BrownieThunkCpp_GetEncryptedBool(FString preferencesCollectionName, FString preferenceKey)

## Example - How to use it

After enabling the capability in Brownie's Plugin Settings screen, you can use one of the following examples to start saving encrypted prefs:

    FString prefFileName = FString(TEXT("fake_user_profile")); //will create a fake_user_profile.xml file
    
    // ===================================== STORE =====================================
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

Now in order to read the preferences stored you can use the following examples as a guide:

    FString prefFileName = FString(TEXT("fake_user_profile")); //will try to read the fake_user_profile.xml file
    
    // ===================================== READ =====================================
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

![image](https://github.com/MartinCazares/UnrealEngine-Brownie/assets/8989676/dff84c72-963a-47d0-8712-219a79762803)

# Screenshots
This capability allows you to take screenshots of the Game, anything displayed in the screen will be captured after the method is invoked.

## Documentation
**Take Screenshot**

    /**
    	Params:
    		FString screenshotFileName //The name you want to be used for the saved file.
    */
    void BrownieThunkCpp_TakeScreenshot(FString screenshotFileName)
    
**Screenshot Result**

*This method gets executed and returns the whole internal path where the screenshot was saved(including the screenshotFileName you passed while taking the screenshot), the file gets save in the "Internal Directory" so it can be used safely by the app for reading/modifying or sharing*

    void UYourComponent::onTakeScreenshotFinished(FString screenshotPath)

## Example - How to use it

After enabling the capability in Brownie's Plugin Settings screen, in order to take ***Screenshots*** on your game, just do this:

First make sure to register for the result of the screenshot broadcast.

    #include "BrowniePubsub.h"
        //...
        //Register to start listening updates...
        BrowniePubsub* browniePubsub = BrowniePubsub::getInstance();
        browniePubsub->onTakeScreenshotFinished.AddUObject(this, &UYourComponent::onTakeScreenshotFinished);
        //...
        //...
        //This function will be called when the screenshot saving process is done and will let us know the whole internal path where the picture was saved.
        void UYourComponent::onTakeScreenshotFinished(FString screenshotPath) { }

Now that we are ready to listen for the result of the screenshot that will provide the internal file path, let's take a picture:

    #if PLATFORM_ANDROID
    	extern void BrownieThunkCpp_TakeScreenshot(FString screenshotFileName);
    	BrownieThunkCpp_TakeScreenshot(FString(TEXT("my_game_picture")));
    #endif	

The previous code can be invoked from anywhere and it will capture the whole surface as is. The file will be stored in the internal directory of the app and can be used for any purposes. The image below shows an example of the image captured and the place where the file lives in the device (notice the name of the file as well as the full path).

![Screenshot-Example](https://github.com/MartinCazares/UnrealEngine-Brownie/assets/8989676/d3e67155-4158-49df-81ef-fafd5924af45)

# Open Web
This capability launches a WebView and loads it with the url that you specify.

## Documentation
**Open Web**

    /**
    	Params:
    		FString urlToLaunch,
    */
    void BrownieThunkCpp_LaunchWebview(FString urlToLaunch)
            

## Example - How to use it

After enabling the capability in Brownie's Plugin Settings screen, all you need to do to ***open a web*** is this:

    FString urlToLoad = UrlTxt->GetText().ToString();//FString(TEXT("https://www.unrealengine.com/en-US"));
    
    #if PLATFORM_ANDROID
    	extern void BrownieThunkCpp_LaunchWebview(FString urlToLoad);
    	BrownieThunkCpp_LaunchWebview(urlToLoad);
    #endif	

Just by adding the code above anywhere you want in your project you would be able to launch a Web on any url you would like to.

# Open Android Settings
This capability launches the Settings Screen or a specific Setting screen like WiFi Setting, Language, Date and Time, etc.

## Documentation
**Open Android Settings**

    /**
    Params:
    int32 selectedIndex { 
		0 -> Main, 
		1 -> Wifi, 
		2 -> Date/Time, 
		3 -> Display, 
		4 -> Location, 
		5 -> Locale }
     */
    void BrownieThunkCpp_OpenSettings(int32 selectedIndex)

## Example - How to use it

After enabling the capability in Brownie's Plugin Settings screen, to be able to ***launch a settings screen***, you have to do the following:

    int32 selectedSetting = OpenSettingsCombo->GetSelectedIndex();//Set any of the numbers expected in the param, any unknown number will default to "Main"
    
    #if PLATFORM_ANDROID
    	extern void BrownieThunkCpp_OpenSettings(int32 selectedSetting);
    	BrownieThunkCpp_OpenSettings(selectedSetting);
    #endif	

With the code above you can redirect the user straight to specific settings screen or to the main one.

# Phone Call
This capability launches the Dialer of the device with a phone number already predefined for the user to just click on the call button and start the call.

## Documentation
**Phone Calling**

    /**
    Return: {
    		CALLING -> 0x01;
    		REQUESTED_FOR_PERMISSION -> 0x02;
    		ACTIVITY_DESTROYING -> 0x03;
    	}
    */
    int BrownieThunkCpp_StartPhoneCall(FString phoneNumber)

**Phone Call Permission Granted**
*This method lets you know whther if the permission was granted by the user or not, if granted, following calls for the action will not trigger this event, the action will be taken directly(requires registering to the event first).*

    void UYourComponent::onPhoneCallPermissionGranted(bool granted)

## Example - How to use it

After enabling the capability in Brownie's Plugin Settings screen, all you need to do to ***start a phone call***, you have to do this:

First make sure to register for the result of the permission granted broadcast.

    #include "BrowniePubsub.h"
        //...
        //Register to start listening updates...
        BrowniePubsub* browniePubsub = BrowniePubsub::getInstance();
        browniePubsub->onPhoneCallPermissionGranted.AddUObject(this, &UYourComponent::onPhoneCallPermissionGranted);
        //...
        //...
        //This function will be called after the user granted the permissions
        void UYourComponent::onPhoneCallPermissionGranted(bool granted) { }

Now that our code is ready to listen for permission granted events, let's try to make a phone call:

    FString phoneNumber = PhoneCallTxt->GetText().ToString();//FString(TEXT("+11234567890"));
    
    #if PLATFORM_ANDROID
    	extern void BrownieThunkCpp_StartPhoneCall(FString phoneNumber);
    	BrownieThunkCpp_StartPhoneCall(phoneNumber);
    #endif	

Just with the code above we can start a phone call. 


# Share Text
This capability allows you to natively share any text to one of the elegible apps in the device.

## Documentation
**Sharing Text**

    /**
     Params:
     FString textToShare,
     */
    void BrownieThunkCpp_ShareText(FString textToShare)
            

## Example - How to use it

After enabling the capability in Brownie's Plugin Settings screen, all you need to do to ***share any text*** is this:

	FString textToShare = MessageTxt->GetText().ToString();

    #if PLATFORM_ANDROID
	    extern void BrownieThunkCpp_ShareText(FString textToShare);
	    BrownieThunkCpp_ShareText(textToShare);
    #endif	

Just by adding the code above anywhere you want in your project you would be able to share any text html/json/plain/etc.

# List of all the Events fired by the library
    FBrownieLocationPermissionGranted onLocationPermissionGranted;
    FBrownieOnLocationUpdate onLocationUpdate;
    FBrownieOnTakeScreenshotFinished onTakeScreenshotFinished;
    FBrownieOnTextToSpeechInitFinished onTextToSpeechInitFinished;
    FBrowniePhoneCallPermissionGranted onPhoneCallPermissionGranted;
    FBrownieBiometricsErrorResult onBiometricsErrorResult;
    FBrownieBiometricsSuccessResult onBiometricsSuccessResult;



# Example Project
In order to get the sample project to work, create a new sample TopDownMap project and Copy the Folders (Character, LevelPrototyping, StarterContent and TopDown) from the Content Folder.
Then just paste them in this projects content folder, go to "BrowniePluginTesting.uproject" and create the solution.





