// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RigdocksError.generated.h"

UENUM(BlueprintType)
enum class AZ_ErrorCode : uint8
{
		Success = 0,

		UnkownError = 1,

		InvalidTrack = 10,

		InvalidMediaItem = 11,

		NoTake = 12,

		NoSource = 13,

		FailSettingProperty = 14,

		InvalidValue = 15,

		FailDeleetingMedia = 16,

		FailMovingMedia = 17,

		NoSample = 18,

		InvalidMediaType = 19,

		NoMediaLength = 20,

		Some_FailSettingProperty = 21,

		FailOpenFile = 22,

		InvalidOption = 23,

		Some_FailDeleteRegion = 24,

		FailLoadingLibrary = 25,

		Some_FailProcessForMedia = 26,

		NotMatchInLoudnessTable = 27,

		FailAnalyzeLoudness = 28,

		WwiseObjectNothing = 29,

		NoneFileOrDirectory = 30,

		NoneDirectory = 31,

		NoJson = 32,

		FailJsonDump = 33,

		FailGettingProperty = 34,

		FailConnectToWwise = 35,

		NoneConnectionToWwise = 36,

		FailExecuteWaapi = 37,

		NoneWaapiResult = 38,

		InvalidPropertyName = 39,

		WwiseItemIDEmpty = 40,

		WwiseMasterBusNotOne = 41,

		Some_FailInsertMedia = 42,

		FileNotFound = 43,

		InvalidArguments = 44,

		FailCreateWwiseObject = 45,

		FailImportSourcefileToWwise = 46,

		NoWwiseSoundObject = 47,

		NotFoundWwiseObject = 48,

		InvalidFormatFile = 49,

		FileOpendOtherProgram = 50,

		InvalidObject = 51,

		FailOpenWorksheet = 52,

		WorksheetNotExist = 53,

		WorksheetNameExist = 54,

		InvalidCellReference = 55,

		FailGettingCellValue = 56,

		FailSettingCellValue = 57,

		NoWwiseSwitchOrState = 58,

		NoWwiseSwitchContainer = 59,
		
		NotFoundHost = 60,

		NotFoundSlackUser = 61,

		ErrorInSlack = 62,

		FailExcuteSlackAPI = 63,

		NoWwiseEvent = 65,

		SubversionNotInstalled = 66,

		SVNCommandFailed = 67,

		NotSVNWorkingCopy = 68,

	    SVNFolderCantAdded = 69,

		SVNNoneCommitedFiles = 70,

		P4NotConnected = 71,

	    P4CommandFailed = 72,

		P4NotInstalled = 73,

		SoundbankGenerationError = 74,

		SoundbankGenerationWarning = 75,

		OutOfItemRange = 76,

		InvalidMarker = 77,

		FailTranscription = 78,

		GSheetNotConnected = 79,

		GSheetAuthFailed = 80,

		Timeout = 81,

		GSheetConnectFailed = 82,

		HttpForbidden = 83,
		
		HttpServerError = 84,
		
		HttpCommunicationFailure = 85,

		FailLoadAudioFile = 86,

		FailGettingFPS = 87,

		FormatNotSupportted = 88,

		ReaperNotSupportted = 89,

		VSTPluginNotFound = 90,

		TSPInfoMismatch = 91,

		SampleRateMismatch = 92,

		ChannelCountMismatch = 93,

		RecordingTooShort = 94,

		ReferenceSignalNotDetected = 95,

		InvalidChannelCount = 96,

		UnsavedNewProject = 97,

		NoneLicenseModule = 100,

		InvalidLicenseModule = 101,

		NoLicense = 102,

		NotUsingMachine = 103,

		NotFoundLibraryModule = 104,

		FailLoadLibraryModule = 105,

		FailLoadLibraryFunction = 106,

		NotMatchVersion = 107,

		NotSupportedAbiVersion = 108,

		ModuleNotFound = 109,

		ModuleDisabled = 110,

		ReaperNotRunning = 111,

		TerminalNotRunning = 112,

		ReaperPhaseFailed = 113,

		TermsNotAgreed = 114,

		ModuleUnsigned = 115,

		ModuleSignatureInvalid = 116,

		ModuleUnexpectedSigner = 117,

		ModuleUntrustedRoot = 118,

		ProtectedPath = 119,

		PathPolicyInvalid = 120,

		HostVerifierNotSet = 200,

		HostKeyRejected = 201,

		TlsHandshakeFailed = 202,

		ClientAuthRejected = 203,

		EnrollmentPending = 204,

		EnrollmentClosed = 205,

		EnrollmentRejected = 206,

		TlsRequired = 209,

		WinsockInit = 210,

		SocketConnect = 211,

		ResponseTimeout = 212,

		SendTimeout = 213,

		ConnectionLost = 214,
};