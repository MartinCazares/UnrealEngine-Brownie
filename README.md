# Brownie

Brownie is an Unreal Engine Plugin that works as a Bridge between two different worlds. With this plugin you can easily take actions on Android's Ecosystem from Unreal's C++.

# Actions

 - **Location Services** (Latitude/Longitude/Accuracy/etc)
 - **In App Review** (start the Reviewing Flow seamlessly)
 - **Show Toasts Messages**
 - **Text To Speech** (pass an FString and convert it to voice)
 - **Screenshots** (whatever Unreal Engine is showing we can capture it)
 - **Open WebView** (launch an Internal Web Browser with any URL you want)
 - **Android Settings** (show the default settings screen on the device or redirect to specific settings (e.g WiFi/Location/Date and Time/etc))
 - **Phone Call** (bring the Dialer with a predefined phone number ready to start the call)

Note: If there's any other actions you would like to be added, let me know. (create an issue with the request)

All these Capabilities can be Enabled/Disabled on Unreal Engine's Editor, just go to Edit -> Project Settings -> Plugins -> Brownie.


![PluginSettingsScreen](https://github.com/MartinCazares/UnrealEngine-Brownie/assets/8989676/0e3a7025-e519-4dec-b981-6cb3e7461526)



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

*This method gets executed only if the permission has been granted by the user.*

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
        //This function will be called when the screenshot saving is done process is done and will let us know the whole internal path where the device was saved.
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
*This method gets executed only if the permission has been explicitly granted by the user, following calls for the action will not trigger this event, the action will be taken directly(requires registering to the event first).*

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

# List of all the Events fired by the library
    FBrownieLocationPermissionGranted onLocationPermissionGranted;
    FBrownieOnLocationUpdate onLocationUpdate;
    FBrownieOnTakeScreenshotFinished onTakeScreenshotFinished;
    FBrownieOnTextToSpeechInitFinished onTextToSpeechInitFinished;
    FBrowniePhoneCallPermissionGranted onPhoneCallPermissionGranted;



