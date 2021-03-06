// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "CrashReportClientApp.h"
#include "WindowsErrorReport.h"
#include "CrashDebugHelperModule.h"
#include "CrashReportUtil.h"

#include "AllowWindowsPlatformTypes.h"
#include <ShlObj.h>
#include "HideWindowsPlatformTypes.h"

#define LOCTEXT_NAMESPACE "CrashReportClient"

namespace
{
	/** Pointer to dynamically loaded crash diagnosis module */
	FCrashDebugHelperModule* CrashHelperModule;
}

/** Helper class used to parse specified string value based on the marker. */
struct FWindowsReportParser
{
	static FString Find( const FString& ReportDirectory, const TCHAR* Marker )
	{
		FString Result;

		TArray<uint8> FileData;
		FFileHelper::LoadFileToArray( FileData, *(ReportDirectory / TEXT( "Report.wer" )) );
		FileData.Add( 0 );
		FileData.Add( 0 );

		const FString FileAsString = reinterpret_cast<TCHAR*>(FileData.GetData());

		TArray<FString> String;
		FileAsString.ParseIntoArray( String, TEXT( "\r\n" ), true );

		for( const auto& StringLine : String )
		{
			if( StringLine.Contains( Marker ) )
			{
				TArray<FString> SeparatedParameters;
				StringLine.ParseIntoArray( SeparatedParameters, Marker, true );

				Result = SeparatedParameters[SeparatedParameters.Num()-1];
				break;
			}
		}

		return Result;
	}
};

FWindowsErrorReport::FWindowsErrorReport(const FString& Directory)
	: FGenericErrorReport(Directory)
{
}

void FWindowsErrorReport::Init()
{
	CrashHelperModule = &FModuleManager::LoadModuleChecked<FCrashDebugHelperModule>(FName("CrashDebugHelper"));
}

void FWindowsErrorReport::ShutDown()
{
	CrashHelperModule->ShutdownModule();
}

FString FWindowsErrorReport::FindCrashedAppPath() const
{
	return FWindowsReportParser::Find(ReportDirectory, TEXT("AppPath="));
}

FText FWindowsErrorReport::DiagnoseReport() const
{
	// Mark the callstack as invalid.
	bValidCallstack = false;

	// Should check if there are local PDBs before doing anything
	auto CrashDebugHelper = CrashHelperModule ? CrashHelperModule->Get() : nullptr;
	if (!CrashDebugHelper)
	{
		// Not localized: should never be seen
		return FText::FromString(TEXT("Failed to load CrashDebugHelper."));
	}

	FString DumpFilename;
	if (!FindFirstReportFileWithExtension(DumpFilename, TEXT(".dmp")))
	{
		if (!FindFirstReportFileWithExtension(DumpFilename, TEXT(".mdmp")))
		{
			return LOCTEXT("MinidumpNotFound", "No minidump found for this crash.");
		}
	}

	if (!CrashDebugHelper->CreateMinidumpDiagnosticReport(ReportDirectory / DumpFilename))
	{
		return LOCTEXT("NoDebuggingSymbols", "You do not have any debugging symbols required to display the callstack for this crash.");
	}

	// No longer required, only for backward compatibility, mark the callstack as valid.
	bValidCallstack = true;
	return FText();
}

static bool TryGetDirectoryCreationTime(const FString& InDirectoryName, FDateTime& OutCreationTime)
{
	FString DirectoryName(InDirectoryName);
	FPaths::MakePlatformFilename(DirectoryName);

	WIN32_FILE_ATTRIBUTE_DATA Info;
	if (!GetFileAttributesExW(*DirectoryName, GetFileExInfoStandard, &Info))
	{
		OutCreationTime = FDateTime();
		return false;
	}

	SYSTEMTIME SysTime;
	if (!FileTimeToSystemTime(&Info.ftCreationTime, &SysTime))
	{
		OutCreationTime = FDateTime();
		return false;
	}

	OutCreationTime = FDateTime(SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	return true;
}

FString FWindowsErrorReport::FindMostRecentErrorReport()
{
	auto& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	auto DirectoryCreationTime = FDateTime::MinValue();
	FString ReportDirectory;
	auto ReportFinder = MakeDirectoryVisitor([&](const TCHAR* FilenameOrDirectory, bool bIsDirectory) 
	{
		if (bIsDirectory)
		{
			FDateTime CreationTime;
			if(TryGetDirectoryCreationTime(FilenameOrDirectory, CreationTime) && CreationTime > DirectoryCreationTime && FCString::Strstr( FilenameOrDirectory, TEXT("UE4-") ) )
			{
				ReportDirectory = FilenameOrDirectory;
				DirectoryCreationTime = CreationTime;
			}
		}
		return true;
	});

	TCHAR LocalAppDataPath[MAX_PATH];
	SHGetFolderPath(0, CSIDL_LOCAL_APPDATA, NULL, 0, LocalAppDataPath);
	PlatformFile.IterateDirectory( *(FString(LocalAppDataPath) / TEXT("Microsoft/Windows/WER/ReportQueue")), ReportFinder);

	if (ReportDirectory.Len() == 0)
	{
		TCHAR LocalAppDataPath[MAX_PATH];
		SHGetFolderPath( 0, CSIDL_COMMON_APPDATA, NULL, 0, LocalAppDataPath );
		PlatformFile.IterateDirectory( *(FString( LocalAppDataPath ) / TEXT( "Microsoft/Windows/WER/ReportQueue" )), ReportFinder );
	}

	return ReportDirectory;
}

#undef LOCTEXT_NAMESPACE
