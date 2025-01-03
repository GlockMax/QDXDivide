<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<?xml-stylesheet type="text/xsl" href="is.xsl" ?>
<!DOCTYPE msi [
   <!ELEMENT msi   (summary,table*)>
   <!ATTLIST msi version    CDATA #REQUIRED>
   <!ATTLIST msi xmlns:dt   CDATA #IMPLIED
                 codepage   CDATA #IMPLIED
                 compression (MSZIP|LZX|none) "LZX">
   
   <!ELEMENT summary       (codepage?,title?,subject?,author?,keywords?,comments?,
                            template,lastauthor?,revnumber,lastprinted?,
                            createdtm?,lastsavedtm?,pagecount,wordcount,
                            charcount?,appname?,security?)>
                            
   <!ELEMENT codepage      (#PCDATA)>
   <!ELEMENT title         (#PCDATA)>
   <!ELEMENT subject       (#PCDATA)>
   <!ELEMENT author        (#PCDATA)>
   <!ELEMENT keywords      (#PCDATA)>
   <!ELEMENT comments      (#PCDATA)>
   <!ELEMENT template      (#PCDATA)>
   <!ELEMENT lastauthor    (#PCDATA)>
   <!ELEMENT revnumber     (#PCDATA)>
   <!ELEMENT lastprinted   (#PCDATA)>
   <!ELEMENT createdtm     (#PCDATA)>
   <!ELEMENT lastsavedtm   (#PCDATA)>
   <!ELEMENT pagecount     (#PCDATA)>
   <!ELEMENT wordcount     (#PCDATA)>
   <!ELEMENT charcount     (#PCDATA)>
   <!ELEMENT appname       (#PCDATA)>
   <!ELEMENT security      (#PCDATA)>                            
                                
   <!ELEMENT table         (col+,row*)>
   <!ATTLIST table
                name        CDATA #REQUIRED>

   <!ELEMENT col           (#PCDATA)>
   <!ATTLIST col
                 key       (yes|no) #IMPLIED
                 def       CDATA #IMPLIED>
                 
   <!ELEMENT row            (td+)>
   
   <!ELEMENT td             (#PCDATA)>
   <!ATTLIST td
                 href       CDATA #IMPLIED
                 dt:dt     (string|bin.base64) #IMPLIED
                 md5        CDATA #IMPLIED>
]>
<msi version="2.0" xmlns:dt="urn:schemas-microsoft-com:datatypes" codepage="65001">
	
	<summary>
		<codepage>1252</codepage>
		<title>NCManager</title>
		<subject></subject>
		<author>##ID_STRING6##</author>
		<keywords>Installer,MSI,Database</keywords>
		<comments>NC Simulator</comments>
		<template>Intel;1033</template>
		<lastauthor>Administrator</lastauthor>
		<revnumber>{7F44E132-2A0C-4303-8012-599497A82BE7}</revnumber>
		<lastprinted/>
		<createdtm>06/21/1999 18:00</createdtm>
		<lastsavedtm>07/14/2000 21:50</lastsavedtm>
		<pagecount>200</pagecount>
		<wordcount>0</wordcount>
		<charcount/>
		<appname>InstallShield Express</appname>
		<security>1</security>
	</summary>
	
	<table name="ActionText">
		<col key="yes" def="s72">Action</col>
		<col def="L64">Description</col>
		<col def="L128">Template</col>
		<row><td>Advertise</td><td>##IDS_ACTIONTEXT_Advertising##</td><td/></row>
		<row><td>AllocateRegistrySpace</td><td>##IDS_ACTIONTEXT_AllocatingRegistry##</td><td>##IDS_ACTIONTEXT_FreeSpace##</td></row>
		<row><td>AppSearch</td><td>##IDS_ACTIONTEXT_SearchInstalled##</td><td>##IDS_ACTIONTEXT_PropertySignature##</td></row>
		<row><td>BindImage</td><td>##IDS_ACTIONTEXT_BindingExes##</td><td>##IDS_ACTIONTEXT_File##</td></row>
		<row><td>CCPSearch</td><td>##IDS_ACTIONTEXT_UnregisterModules##</td><td/></row>
		<row><td>CostFinalize</td><td>##IDS_ACTIONTEXT_ComputingSpace3##</td><td/></row>
		<row><td>CostInitialize</td><td>##IDS_ACTIONTEXT_ComputingSpace##</td><td/></row>
		<row><td>CreateFolders</td><td>##IDS_ACTIONTEXT_CreatingFolders##</td><td>##IDS_ACTIONTEXT_Folder##</td></row>
		<row><td>CreateShortcuts</td><td>##IDS_ACTIONTEXT_CreatingShortcuts##</td><td>##IDS_ACTIONTEXT_Shortcut##</td></row>
		<row><td>DeleteServices</td><td>##IDS_ACTIONTEXT_DeletingServices##</td><td>##IDS_ACTIONTEXT_Service##</td></row>
		<row><td>DuplicateFiles</td><td>##IDS_ACTIONTEXT_CreatingDuplicate##</td><td>##IDS_ACTIONTEXT_FileDirectorySize##</td></row>
		<row><td>FileCost</td><td>##IDS_ACTIONTEXT_ComputingSpace2##</td><td/></row>
		<row><td>FindRelatedProducts</td><td>##IDS_ACTIONTEXT_SearchForRelated##</td><td>##IDS_ACTIONTEXT_FoundApp##</td></row>
		<row><td>GenerateScript</td><td>##IDS_ACTIONTEXT_GeneratingScript##</td><td>##IDS_ACTIONTEXT_1##</td></row>
		<row><td>ISLockPermissionsCost</td><td>##IDS_ACTIONTEXT_ISLockPermissionsCost##</td><td/></row>
		<row><td>ISLockPermissionsInstall</td><td>##IDS_ACTIONTEXT_ISLockPermissionsInstall##</td><td/></row>
		<row><td>InstallAdminPackage</td><td>##IDS_ACTIONTEXT_CopyingNetworkFiles##</td><td>##IDS_ACTIONTEXT_FileDirSize##</td></row>
		<row><td>InstallFiles</td><td>##IDS_ACTIONTEXT_CopyingNewFiles##</td><td>##IDS_ACTIONTEXT_FileDirSize2##</td></row>
		<row><td>InstallODBC</td><td>##IDS_ACTIONTEXT_InstallODBC##</td><td/></row>
		<row><td>InstallSFPCatalogFile</td><td>##IDS_ACTIONTEXT_InstallingSystemCatalog##</td><td>##IDS_ACTIONTEXT_FileDependencies##</td></row>
		<row><td>InstallServices</td><td>##IDS_ACTIONTEXT_InstallServices##</td><td>##IDS_ACTIONTEXT_Service2##</td></row>
		<row><td>InstallValidate</td><td>##IDS_ACTIONTEXT_Validating##</td><td/></row>
		<row><td>LaunchConditions</td><td>##IDS_ACTIONTEXT_EvaluateLaunchConditions##</td><td/></row>
		<row><td>MigrateFeatureStates</td><td>##IDS_ACTIONTEXT_MigratingFeatureStates##</td><td>##IDS_ACTIONTEXT_Application##</td></row>
		<row><td>MoveFiles</td><td>##IDS_ACTIONTEXT_MovingFiles##</td><td>##IDS_ACTIONTEXT_FileDirSize3##</td></row>
		<row><td>PatchFiles</td><td>##IDS_ACTIONTEXT_PatchingFiles##</td><td>##IDS_ACTIONTEXT_FileDirSize4##</td></row>
		<row><td>ProcessComponents</td><td>##IDS_ACTIONTEXT_UpdateComponentRegistration##</td><td/></row>
		<row><td>PublishComponents</td><td>##IDS_ACTIONTEXT_PublishingQualifiedComponents##</td><td>##IDS_ACTIONTEXT_ComponentIDQualifier##</td></row>
		<row><td>PublishFeatures</td><td>##IDS_ACTIONTEXT_PublishProductFeatures##</td><td>##IDS_ACTIONTEXT_FeatureColon##</td></row>
		<row><td>PublishProduct</td><td>##IDS_ACTIONTEXT_PublishProductInfo##</td><td/></row>
		<row><td>RMCCPSearch</td><td>##IDS_ACTIONTEXT_SearchingQualifyingProducts##</td><td/></row>
		<row><td>RegisterClassInfo</td><td>##IDS_ACTIONTEXT_RegisterClassServer##</td><td>##IDS_ACTIONTEXT_ClassId##</td></row>
		<row><td>RegisterComPlus</td><td>##IDS_ACTIONTEXT_RegisteringComPlus##</td><td>##IDS_ACTIONTEXT_AppIdAppTypeRSN##</td></row>
		<row><td>RegisterExtensionInfo</td><td>##IDS_ACTIONTEXT_RegisterExtensionServers##</td><td>##IDS_ACTIONTEXT_Extension2##</td></row>
		<row><td>RegisterFonts</td><td>##IDS_ACTIONTEXT_RegisterFonts##</td><td>##IDS_ACTIONTEXT_Font##</td></row>
		<row><td>RegisterMIMEInfo</td><td>##IDS_ACTIONTEXT_RegisterMimeInfo##</td><td>##IDS_ACTIONTEXT_ContentTypeExtension##</td></row>
		<row><td>RegisterProduct</td><td>##IDS_ACTIONTEXT_RegisteringProduct##</td><td>##IDS_ACTIONTEXT_1b##</td></row>
		<row><td>RegisterProgIdInfo</td><td>##IDS_ACTIONTEXT_RegisteringProgIdentifiers##</td><td>##IDS_ACTIONTEXT_ProgID2##</td></row>
		<row><td>RegisterTypeLibraries</td><td>##IDS_ACTIONTEXT_RegisterTypeLibs##</td><td>##IDS_ACTIONTEXT_LibId##</td></row>
		<row><td>RegisterUser</td><td>##IDS_ACTIONTEXT_RegUser##</td><td>##IDS_ACTIONTEXT_1c##</td></row>
		<row><td>RemoveDuplicateFiles</td><td>##IDS_ACTIONTEXT_RemovingDuplicates##</td><td>##IDS_ACTIONTEXT_FileDir##</td></row>
		<row><td>RemoveEnvironmentStrings</td><td>##IDS_ACTIONTEXT_UpdateEnvironmentStrings##</td><td>##IDS_ACTIONTEXT_NameValueAction2##</td></row>
		<row><td>RemoveExistingProducts</td><td>##IDS_ACTIONTEXT_RemoveApps##</td><td>##IDS_ACTIONTEXT_AppCommandLine##</td></row>
		<row><td>RemoveFiles</td><td>##IDS_ACTIONTEXT_RemovingFiles##</td><td>##IDS_ACTIONTEXT_FileDir2##</td></row>
		<row><td>RemoveFolders</td><td>##IDS_ACTIONTEXT_RemovingFolders##</td><td>##IDS_ACTIONTEXT_Folder1##</td></row>
		<row><td>RemoveIniValues</td><td>##IDS_ACTIONTEXT_RemovingIni##</td><td>##IDS_ACTIONTEXT_FileSectionKeyValue##</td></row>
		<row><td>RemoveODBC</td><td>##IDS_ACTIONTEXT_RemovingODBC##</td><td/></row>
		<row><td>RemoveRegistryValues</td><td>##IDS_ACTIONTEXT_RemovingRegistry##</td><td>##IDS_ACTIONTEXT_KeyName##</td></row>
		<row><td>RemoveShortcuts</td><td>##IDS_ACTIONTEXT_RemovingShortcuts##</td><td>##IDS_ACTIONTEXT_Shortcut1##</td></row>
		<row><td>Rollback</td><td>##IDS_ACTIONTEXT_RollingBack##</td><td>##IDS_ACTIONTEXT_1d##</td></row>
		<row><td>RollbackCleanup</td><td>##IDS_ACTIONTEXT_RemovingBackup##</td><td>##IDS_ACTIONTEXT_File2##</td></row>
		<row><td>SelfRegModules</td><td>##IDS_ACTIONTEXT_RegisteringModules##</td><td>##IDS_ACTIONTEXT_FileFolder##</td></row>
		<row><td>SelfUnregModules</td><td>##IDS_ACTIONTEXT_UnregisterModules##</td><td>##IDS_ACTIONTEXT_FileFolder2##</td></row>
		<row><td>SetODBCFolders</td><td>##IDS_ACTIONTEXT_InitializeODBCDirs##</td><td/></row>
		<row><td>StartServices</td><td>##IDS_ACTIONTEXT_StartingServices##</td><td>##IDS_ACTIONTEXT_Service3##</td></row>
		<row><td>StopServices</td><td>##IDS_ACTIONTEXT_StoppingServices##</td><td>##IDS_ACTIONTEXT_Service4##</td></row>
		<row><td>UnmoveFiles</td><td>##IDS_ACTIONTEXT_RemovingMoved##</td><td>##IDS_ACTIONTEXT_FileDir3##</td></row>
		<row><td>UnpublishComponents</td><td>##IDS_ACTIONTEXT_UnpublishQualified##</td><td>##IDS_ACTIONTEXT_ComponentIdQualifier2##</td></row>
		<row><td>UnpublishFeatures</td><td>##IDS_ACTIONTEXT_UnpublishProductFeatures##</td><td>##IDS_ACTIONTEXT_Feature##</td></row>
		<row><td>UnpublishProduct</td><td>##IDS_ACTIONTEXT_UnpublishingProductInfo##</td><td/></row>
		<row><td>UnregisterClassInfo</td><td>##IDS_ACTIONTEXT_UnregisterClassServers##</td><td>##IDS_ACTIONTEXT_ClsID##</td></row>
		<row><td>UnregisterComPlus</td><td>##IDS_ACTIONTEXT_UnregisteringComPlus##</td><td>##IDS_ACTIONTEXT_AppId##</td></row>
		<row><td>UnregisterExtensionInfo</td><td>##IDS_ACTIONTEXT_UnregisterExtensionServers##</td><td>##IDS_ACTIONTEXT_Extension##</td></row>
		<row><td>UnregisterFonts</td><td>##IDS_ACTIONTEXT_UnregisteringFonts##</td><td>##IDS_ACTIONTEXT_Font2##</td></row>
		<row><td>UnregisterMIMEInfo</td><td>##IDS_ACTIONTEXT_UnregisteringMimeInfo##</td><td>##IDS_ACTIONTEXT_ContentTypeExtension2##</td></row>
		<row><td>UnregisterProgIdInfo</td><td>##IDS_ACTIONTEXT_UnregisteringProgramIds##</td><td>##IDS_ACTIONTEXT_ProgID##</td></row>
		<row><td>UnregisterTypeLibraries</td><td>##IDS_ACTIONTEXT_UnregTypeLibs##</td><td>##IDS_ACTIONTEXT_Libid2##</td></row>
		<row><td>WriteEnvironmentStrings</td><td>##IDS_ACTIONTEXT_EnvironmentStrings##</td><td>##IDS_ACTIONTEXT_NameValueAction##</td></row>
		<row><td>WriteIniValues</td><td>##IDS_ACTIONTEXT_WritingINI##</td><td>##IDS_ACTIONTEXT_FileSectionKeyValue2##</td></row>
		<row><td>WriteRegistryValues</td><td>##IDS_ACTIONTEXT_WritingRegistry##</td><td>##IDS_ACTIONTEXT_KeyNameValue##</td></row>
	</table>

	<table name="AdminExecuteSequence">
		<col key="yes" def="s72">Action</col>
		<col def="S255">Condition</col>
		<col def="I2">Sequence</col>
		<col def="S255">ISComments</col>
		<col def="I4">ISAttributes</col>
		<row><td>CostFinalize</td><td/><td>1000</td><td>CostFinalize</td><td/></row>
		<row><td>CostInitialize</td><td/><td>800</td><td>CostInitialize</td><td/></row>
		<row><td>FileCost</td><td/><td>900</td><td>FileCost</td><td/></row>
		<row><td>InstallAdminPackage</td><td/><td>3900</td><td>InstallAdminPackage</td><td/></row>
		<row><td>InstallFiles</td><td/><td>4000</td><td>InstallFiles</td><td/></row>
		<row><td>InstallFinalize</td><td/><td>6600</td><td>InstallFinalize</td><td/></row>
		<row><td>InstallInitialize</td><td/><td>1500</td><td>InstallInitialize</td><td/></row>
		<row><td>InstallValidate</td><td/><td>1400</td><td>InstallValidate</td><td/></row>
		<row><td>ScheduleReboot</td><td>ISSCHEDULEREBOOT</td><td>4010</td><td>ScheduleReboot</td><td/></row>
	</table>

	<table name="AdminUISequence">
		<col key="yes" def="s72">Action</col>
		<col def="S255">Condition</col>
		<col def="I2">Sequence</col>
		<col def="S255">ISComments</col>
		<col def="I4">ISAttributes</col>
		<row><td>AdminWelcome</td><td/><td>1010</td><td>AdminWelcome</td><td/></row>
		<row><td>CostFinalize</td><td/><td>1000</td><td>CostFinalize</td><td/></row>
		<row><td>CostInitialize</td><td/><td>800</td><td>CostInitialize</td><td/></row>
		<row><td>ExecuteAction</td><td/><td>1300</td><td>ExecuteAction</td><td/></row>
		<row><td>FileCost</td><td/><td>900</td><td>FileCost</td><td/></row>
		<row><td>SetupCompleteError</td><td/><td>-3</td><td>SetupCompleteError</td><td/></row>
		<row><td>SetupCompleteSuccess</td><td/><td>-1</td><td>SetupCompleteSuccess</td><td/></row>
		<row><td>SetupInitialization</td><td/><td>50</td><td>SetupInitialization</td><td/></row>
		<row><td>SetupInterrupted</td><td/><td>-2</td><td>SetupInterrupted</td><td/></row>
		<row><td>SetupProgress</td><td/><td>1020</td><td>SetupProgress</td><td/></row>
	</table>

	<table name="AdvtExecuteSequence">
		<col key="yes" def="s72">Action</col>
		<col def="S255">Condition</col>
		<col def="I2">Sequence</col>
		<col def="S255">ISComments</col>
		<col def="I4">ISAttributes</col>
		<row><td>CostFinalize</td><td/><td>1000</td><td>CostFinalize</td><td/></row>
		<row><td>CostInitialize</td><td/><td>800</td><td>CostInitialize</td><td/></row>
		<row><td>CreateShortcuts</td><td/><td>4500</td><td>CreateShortcuts</td><td/></row>
		<row><td>InstallFinalize</td><td/><td>6600</td><td>InstallFinalize</td><td/></row>
		<row><td>InstallInitialize</td><td/><td>1500</td><td>InstallInitialize</td><td/></row>
		<row><td>InstallValidate</td><td/><td>1400</td><td>InstallValidate</td><td/></row>
		<row><td>MsiPublishAssemblies</td><td/><td>6250</td><td>MsiPublishAssemblies</td><td/></row>
		<row><td>PublishComponents</td><td/><td>6200</td><td>PublishComponents</td><td/></row>
		<row><td>PublishFeatures</td><td/><td>6300</td><td>PublishFeatures</td><td/></row>
		<row><td>PublishProduct</td><td/><td>6400</td><td>PublishProduct</td><td/></row>
		<row><td>RegisterClassInfo</td><td/><td>4600</td><td>RegisterClassInfo</td><td/></row>
		<row><td>RegisterExtensionInfo</td><td/><td>4700</td><td>RegisterExtensionInfo</td><td/></row>
		<row><td>RegisterMIMEInfo</td><td/><td>4900</td><td>RegisterMIMEInfo</td><td/></row>
		<row><td>RegisterProgIdInfo</td><td/><td>4800</td><td>RegisterProgIdInfo</td><td/></row>
		<row><td>RegisterTypeLibraries</td><td/><td>4910</td><td>RegisterTypeLibraries</td><td/></row>
		<row><td>ScheduleReboot</td><td>ISSCHEDULEREBOOT</td><td>6410</td><td>ScheduleReboot</td><td/></row>
	</table>

	<table name="AdvtUISequence">
		<col key="yes" def="s72">Action</col>
		<col def="S255">Condition</col>
		<col def="I2">Sequence</col>
		<col def="S255">ISComments</col>
		<col def="I4">ISAttributes</col>
	</table>

	<table name="AppId">
		<col key="yes" def="s38">AppId</col>
		<col def="S255">RemoteServerName</col>
		<col def="S255">LocalService</col>
		<col def="S255">ServiceParameters</col>
		<col def="S255">DllSurrogate</col>
		<col def="I2">ActivateAtStorage</col>
		<col def="I2">RunAsInteractiveUser</col>
	</table>

	<table name="AppSearch">
		<col key="yes" def="s72">Property</col>
		<col key="yes" def="s72">Signature_</col>
	</table>

	<table name="BBControl">
		<col key="yes" def="s50">Billboard_</col>
		<col key="yes" def="s50">BBControl</col>
		<col def="s50">Type</col>
		<col def="i2">X</col>
		<col def="i2">Y</col>
		<col def="i2">Width</col>
		<col def="i2">Height</col>
		<col def="I4">Attributes</col>
		<col def="L50">Text</col>
	</table>

	<table name="Billboard">
		<col key="yes" def="s50">Billboard</col>
		<col def="s38">Feature_</col>
		<col def="S50">Action</col>
		<col def="I2">Ordering</col>
	</table>

	<table name="Binary">
		<col key="yes" def="s72">Name</col>
		<col def="V0">Data</col>
		<col def="S255">ISBuildSourcePath</col>
		<row><td>ISExpHlp.dll</td><td/><td>&lt;ISRedistPlatformDependentFolder&gt;\ISExpHlp.dll</td></row>
		<row><td>ISSELFREG.DLL</td><td/><td>&lt;ISRedistPlatformDependentFolder&gt;\isregsvr.dll</td></row>
		<row><td>NewBinary1</td><td/><td>&lt;ISProductFolder&gt;\Support\Themes\InstallShield Blue Theme\banner.jpg</td></row>
		<row><td>NewBinary10</td><td/><td>&lt;ISProductFolder&gt;\Redist\Language Independent\OS Independent\CompleteSetupIco.ibd</td></row>
		<row><td>NewBinary11</td><td/><td>&lt;ISProductFolder&gt;\Redist\Language Independent\OS Independent\CustomSetupIco.ibd</td></row>
		<row><td>NewBinary12</td><td/><td>&lt;ISProductFolder&gt;\Redist\Language Independent\OS Independent\DestIcon.ibd</td></row>
		<row><td>NewBinary13</td><td/><td>&lt;ISProductFolder&gt;\Redist\Language Independent\OS Independent\NetworkInstall.ico</td></row>
		<row><td>NewBinary14</td><td/><td>&lt;ISProductFolder&gt;\Redist\Language Independent\OS Independent\DontInstall.ico</td></row>
		<row><td>NewBinary15</td><td/><td>&lt;ISProductFolder&gt;\Redist\Language Independent\OS Independent\Install.ico</td></row>
		<row><td>NewBinary16</td><td/><td>&lt;ISProductFolder&gt;\Redist\Language Independent\OS Independent\InstallFirstUse.ico</td></row>
		<row><td>NewBinary17</td><td/><td>&lt;ISProductFolder&gt;\Redist\Language Independent\OS Independent\InstallPartial.ico</td></row>
		<row><td>NewBinary18</td><td/><td>&lt;ISProductFolder&gt;\Redist\Language Independent\OS Independent\InstallStateMenu.ico</td></row>
		<row><td>NewBinary19</td><td/><td>&lt;ISProductFolder&gt;\Support\Themes\InstallShield Blue Theme\welcome.jpg</td></row>
		<row><td>NewBinary2</td><td/><td>&lt;ISProductFolder&gt;\Redist\Language Independent\OS Independent\New.ibd</td></row>
		<row><td>NewBinary20</td><td/><td>&lt;ISProductFolder&gt;\Support\Themes\InstallShield Blue Theme\welcome.jpg</td></row>
		<row><td>NewBinary21</td><td/><td>&lt;ISProductFolder&gt;\Support\Themes\InstallShield Blue Theme\banner.jpg</td></row>
		<row><td>NewBinary22</td><td/><td>&lt;ISProductFolder&gt;\Support\Themes\InstallShield Blue Theme\banner.jpg</td></row>
		<row><td>NewBinary3</td><td/><td>&lt;ISProductFolder&gt;\Redist\Language Independent\OS Independent\Up.ibd</td></row>
		<row><td>NewBinary4</td><td/><td>&lt;ISProductFolder&gt;\Redist\Language Independent\OS Independent\WarningIcon.ibd</td></row>
		<row><td>NewBinary5</td><td/><td>&lt;ISProductFolder&gt;\Support\Themes\InstallShield Blue Theme\welcome.jpg</td></row>
		<row><td>NewBinary6</td><td/><td>&lt;ISProductFolder&gt;\Redist\Language Independent\OS Independent\CustomSetupIco.ibd</td></row>
		<row><td>NewBinary7</td><td/><td>&lt;ISProductFolder&gt;\Redist\Language Independent\OS Independent\ReinstIco.ibd</td></row>
		<row><td>NewBinary8</td><td/><td>&lt;ISProductFolder&gt;\Redist\Language Independent\OS Independent\RemoveIco.ibd</td></row>
		<row><td>NewBinary9</td><td/><td>&lt;ISProductFolder&gt;\Redist\Language Independent\OS Independent\SetupIcon.ibd</td></row>
		<row><td>SetAllUsers.dll</td><td/><td>&lt;ISRedistPlatformDependentFolder&gt;\SetAllUsers.dll</td></row>
	</table>

	<table name="BindImage">
		<col key="yes" def="s72">File_</col>
		<col def="S255">Path</col>
	</table>

	<table name="CCPSearch">
		<col key="yes" def="s72">Signature_</col>
	</table>

	<table name="CheckBox">
		<col key="yes" def="s72">Property</col>
		<col def="S64">Value</col>
		<row><td>ISCHECKFORPRODUCTUPDATES</td><td>1</td></row>
		<row><td>LAUNCHPROGRAM</td><td>1</td></row>
		<row><td>LAUNCHREADME</td><td>1</td></row>
	</table>

	<table name="Class">
		<col key="yes" def="s38">CLSID</col>
		<col key="yes" def="s32">Context</col>
		<col key="yes" def="s72">Component_</col>
		<col def="S255">ProgId_Default</col>
		<col def="L255">Description</col>
		<col def="S38">AppId_</col>
		<col def="S255">FileTypeMask</col>
		<col def="S72">Icon_</col>
		<col def="I2">IconIndex</col>
		<col def="S32">DefInprocHandler</col>
		<col def="S255">Argument</col>
		<col def="s38">Feature_</col>
		<col def="I2">Attributes</col>
	</table>

	<table name="ComboBox">
		<col key="yes" def="s72">Property</col>
		<col key="yes" def="i2">Order</col>
		<col def="s64">Value</col>
		<col def="L64">Text</col>
	</table>

	<table name="CompLocator">
		<col key="yes" def="s72">Signature_</col>
		<col def="s38">ComponentId</col>
		<col def="I2">Type</col>
	</table>

	<table name="Complus">
		<col key="yes" def="s72">Component_</col>
		<col key="yes" def="I2">ExpType</col>
	</table>

	<table name="Component">
		<col key="yes" def="s72">Component</col>
		<col def="S38">ComponentId</col>
		<col def="s72">Directory_</col>
		<col def="i2">Attributes</col>
		<col def="S255">Condition</col>
		<col def="S72">KeyPath</col>
		<col def="I4">ISAttributes</col>
		<col def="S255">ISComments</col>
		<col def="S255">ISScanAtBuildFile</col>
		<col def="S255">ISRegFileToMergeAtBuild</col>
		<col def="S0">ISDotNetInstallerArgsInstall</col>
		<col def="S0">ISDotNetInstallerArgsCommit</col>
		<col def="S0">ISDotNetInstallerArgsUninstall</col>
		<col def="S0">ISDotNetInstallerArgsRollback</col>
		<row><td>C__082C4A2B51334D24A3AC6790D6C8512B</td><td>{F4385029-1077-4EE1-8F40-460B4D5CF518}</td><td>_AE568DA94C254E8DB02AC5111802BB6B</td><td>258</td><td/><td>_082C4A2B51334D24A3AC6790D6C8512B</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__08B0E42E0AE7406A98786249EDB2B56E</td><td>{6F70C38C-FE5D-47D7-AEDB-273A2C524E26}</td><td>INSTALLDIR</td><td>262</td><td/><td>_08B0E42E0AE7406A98786249EDB2B56E</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__0BBF4D1B61464202AA9E41FF332450DB</td><td>{DA229F47-3092-4AC1-A59B-0D17BF692E09}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td>_0BBF4D1B61464202AA9E41FF332450DB</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__122CBDCF2BDA4DE9881ADCC656F030C1</td><td>{CD5F9B5B-0182-4F7E-A17C-6D220CD2B61C}</td><td>INSTALLDIR</td><td>258</td><td/><td>_122CBDCF2BDA4DE9881ADCC656F030C1</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__127D62C316F2443D94ACD0038D656836</td><td>{B02E63A1-2261-48BF-9873-60E6F122B464}</td><td>_AF9184B1F044480AAEC8CE10016E9441</td><td>2</td><td/><td>_127D62C316F2443D94ACD0038D656836</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__1346B53B2129406687E96EC782FD6B14</td><td>{6C86C0F9-D8C8-4E37-B8AD-E74DEBA5B820}</td><td>_ABF01A60B67E49A8953409B9471A5989</td><td>2</td><td/><td>_1346B53B2129406687E96EC782FD6B14</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__137C441D09AE4FB5B9C67421B48492C1</td><td>{A992867F-E9CA-470C-9996-05F74C56979E}</td><td>INSTALLDIR</td><td>258</td><td/><td>_137C441D09AE4FB5B9C67421B48492C1</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__14EF4C63E3B5454D8D5A69C3C590EB86</td><td>{B097304F-D506-4576-8611-F862E22E9717}</td><td>INSTALLDIR</td><td>258</td><td/><td>_14EF4C63E3B5454D8D5A69C3C590EB86</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__1615A4B06DC04A4BAF33E4CA968A4EF4</td><td>{FB065895-3815-4E75-91E2-C41B2A3445A6}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td>_1615A4B06DC04A4BAF33E4CA968A4EF4</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__1C07066DA2C8474AB22F5246BE023333</td><td>{0A0235FE-11EC-4836-8714-EBC9BFD173EE}</td><td>INSTALLDIR</td><td>258</td><td/><td>_1C07066DA2C8474AB22F5246BE023333</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__1D5D687FCC234E39ADEACC9EFBBD2AB7</td><td>{C1CF88CF-369C-41AA-B821-307D62B6890F}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td>_1D5D687FCC234E39ADEACC9EFBBD2AB7</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__25E09A9CCD224D31881BE31AB87732A3</td><td>{5D4F4D10-FF22-4C23-A81E-C72798E6009B}</td><td>_AF9184B1F044480AAEC8CE10016E9441</td><td>2</td><td/><td>_25E09A9CCD224D31881BE31AB87732A3</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__29C12E55443848608C72F3AD69AAB824</td><td>{999BF38E-591E-4720-AB41-D11522E86FA1}</td><td>_D4445AED10494F4C88881E86F0499B93</td><td>258</td><td/><td>_29C12E55443848608C72F3AD69AAB824</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__2A0D3AF6F7264C9A8D50E89172C4A81E</td><td>{B44315E5-98E2-4815-96F9-9F30F9806892}</td><td>INSTALLDIR</td><td>258</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__31F1DADB0D4C4DB1A4A4487740F0A412</td><td>{2CE70A8D-B678-4B90-8118-A253D2057FAB}</td><td>_96CD38D4AC6C4E08AFFFF2C9F3108B20</td><td>2</td><td/><td>_31F1DADB0D4C4DB1A4A4487740F0A412</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__34CD49E7DDF94F629770F9F61A0F4957</td><td>{9A003D1E-5043-4095-A665-A16B436D3526}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td>_34CD49E7DDF94F629770F9F61A0F4957</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__40FF87838DE4496EBC8A597169C87209</td><td>{08233EFA-F9DF-4A7C-8211-ABFCF3320361}</td><td>INSTALLDIR</td><td>258</td><td/><td>_40FF87838DE4496EBC8A597169C87209</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__423104302EE74832984738AC1F76C5AE</td><td>{C48A03E4-69E1-40B2-8008-6B407A9B8DBF}</td><td>_D4445AED10494F4C88881E86F0499B93</td><td>258</td><td/><td>_423104302EE74832984738AC1F76C5AE</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__428099970CD44B46A98E2226D653EE63</td><td>{2245C611-0AD6-4AA1-84BB-518C02BA72E0}</td><td>_96CD38D4AC6C4E08AFFFF2C9F3108B20</td><td>2</td><td/><td>_428099970CD44B46A98E2226D653EE63</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__431D7DF568F44C0499C79B40A1BF6E12</td><td>{69C92F37-7B1C-40B8-ADC7-58EABB54DCF6}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td>_431D7DF568F44C0499C79B40A1BF6E12</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__4BCBDC7592FA4B638AD6631DDD7D122A</td><td>{15BC107D-EBCA-4AA8-B970-B47B5EA61550}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td>_4BCBDC7592FA4B638AD6631DDD7D122A</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__4D66453340ED4B598F05FE3CAD8329D4</td><td>{B42F6804-0916-47FA-801F-D3A1B2A79CFD}</td><td>_AF9184B1F044480AAEC8CE10016E9441</td><td>2</td><td/><td>_4D66453340ED4B598F05FE3CAD8329D4</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__4F10DC0F614541B6ADC8623FE3862D1C</td><td>{1398F4AE-E932-4EA4-9E85-4F98742A5107}</td><td>_96CD38D4AC6C4E08AFFFF2C9F3108B20</td><td>2</td><td/><td>_4F10DC0F614541B6ADC8623FE3862D1C</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__4F4DA107785E44E581213838B3497E30</td><td>{A9516642-2B9F-4B85-9231-FBC7B1500CAB}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td>_4F4DA107785E44E581213838B3497E30</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__578DC43FDCB1484794895CECAEE03768</td><td>{EF75F479-039F-47C6-AB39-73BA8D47BABC}</td><td>INSTALLDIR</td><td>258</td><td/><td>_578DC43FDCB1484794895CECAEE03768</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__5AB0F74B62D24B2797103046CCAAD98E</td><td>{65208836-C0ED-4B72-AC61-20244DAE7235}</td><td>INSTALLDIR</td><td>258</td><td/><td>_5AB0F74B62D24B2797103046CCAAD98E</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__5C6E7AA0836C41DB831039A34D3D7AA0</td><td>{0F80A501-2760-454A-9BA4-38DA1DE257CD}</td><td>_96CD38D4AC6C4E08AFFFF2C9F3108B20</td><td>2</td><td/><td>_5C6E7AA0836C41DB831039A34D3D7AA0</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__5CB24DDE141142B7B0F65A0B5670CFB3</td><td>{2ADC0505-BDD2-4A7E-AEDC-44B14F3A4117}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td>_5CB24DDE141142B7B0F65A0B5670CFB3</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__5F2121D4FBDE491F8A7129617E991A99</td><td>{3AE00DF5-D267-4F02-9E44-83454D682682}</td><td>INSTALLDIR</td><td>258</td><td/><td>_5F2121D4FBDE491F8A7129617E991A99</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__6185CEAD036144B69828C2BA1357F7C7</td><td>{BA178632-EF17-4519-A214-93C7F1F59567}</td><td>INSTALLDIR</td><td>258</td><td/><td>_6185CEAD036144B69828C2BA1357F7C7</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__70BD166A8637415AB595011E5C8869C5</td><td>{597DDC7F-3C1F-400A-85D8-B38BB577DA77}</td><td>INSTALLDIR</td><td>258</td><td/><td>_70BD166A8637415AB595011E5C8869C5</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__70DF0F53D1CE41E184D5BC1D21498AA9</td><td>{AA0D200E-9B27-4CB1-AE82-1E90A979886D}</td><td>INSTALLDIR</td><td>258</td><td/><td>_70DF0F53D1CE41E184D5BC1D21498AA9</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__72CE03E984304A3F8369648DA0BBEC90</td><td>{F2373FE8-6696-48AB-8CE4-F1A7C9BCFF22}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td>_72CE03E984304A3F8369648DA0BBEC90</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__72E841FC40C949838CF7B48362C035BC</td><td>{5B84F23F-5C11-4F59-844A-8AA1ECB68230}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td>_72E841FC40C949838CF7B48362C035BC</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__76A73657BC264AD09EEDE2B6EB407B17</td><td>{0B723952-2BA0-4320-82EC-FC256395DECB}</td><td>_A28F2D48A7A14BEA9D34FCDE011F1513</td><td>258</td><td/><td>_76A73657BC264AD09EEDE2B6EB407B17</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__8306BB83A4434E08BC8580B175683D54</td><td>{E1F369E2-F1BF-4591-BBD8-6D514D596D9B}</td><td>INSTALLDIR</td><td>258</td><td/><td>_8306BB83A4434E08BC8580B175683D54</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__830F99D2CA044919B0C0507A0D17980E</td><td>{ED7FC7FD-E6C7-4639-8547-8E41449589BB}</td><td>_ABF01A60B67E49A8953409B9471A5989</td><td>2</td><td/><td>_830F99D2CA044919B0C0507A0D17980E</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__843F7632BEC24FBE8BF7C63DEFBF0687</td><td>{8A0FEE31-9958-4688-859D-1E7534A13F5D}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td>_843F7632BEC24FBE8BF7C63DEFBF0687</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__9013468A11E6405391A6F141B889603B</td><td>{34D8DF03-741A-4D36-B640-7331A1E73D06}</td><td>INSTALLDIR</td><td>258</td><td/><td>_9013468A11E6405391A6F141B889603B</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__92867AD4217F4FD89E7D44B5A0C02773</td><td>{5C591455-2CC2-415D-830E-E0F15A6A2775}</td><td>_AF9184B1F044480AAEC8CE10016E9441</td><td>2</td><td/><td>_92867AD4217F4FD89E7D44B5A0C02773</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__96B0F2B1BDAF40E2AB16F98C98A045A8</td><td>{F446D2EC-E3F5-49B1-9827-E88264B6D609}</td><td>INSTALLDIR</td><td>258</td><td/><td>_96B0F2B1BDAF40E2AB16F98C98A045A8</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__984DB5E2EFED4B68AFC611CCD78EE906</td><td>{762D981A-C787-4F1F-9063-820332DC5740}</td><td>INSTALLDIR</td><td>258</td><td/><td>_984DB5E2EFED4B68AFC611CCD78EE906</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__9CC9C8957FF0430099D06B9A6E37CB44</td><td>{DF04A6C2-4393-4813-ACA8-CB202B5D38C1}</td><td>_AF9184B1F044480AAEC8CE10016E9441</td><td>2</td><td/><td>_9CC9C8957FF0430099D06B9A6E37CB44</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__A1C4795563494325A174FC7F2F7E25CE</td><td>{B9B367F1-910D-4488-B679-BA1DECD7AC49}</td><td>_ABF01A60B67E49A8953409B9471A5989</td><td>2</td><td/><td>_A1C4795563494325A174FC7F2F7E25CE</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__A24AF738FF7547978A68111A0B90EF5E</td><td>{003127E5-D283-4F2B-AD8C-63A22B458812}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td>_A24AF738FF7547978A68111A0B90EF5E</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__A29FEDE025A24BEC89F0EAC20DAEA4CD</td><td>{6E624E4E-FAC8-4274-A2F4-88D0129B1F32}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td>_A29FEDE025A24BEC89F0EAC20DAEA4CD</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__A2A8E787524045DBA36D5BDB7A77E13F</td><td>{568411B6-47A1-428A-90CE-B9811C0BA7B8}</td><td>INSTALLDIR</td><td>258</td><td/><td>_A2A8E787524045DBA36D5BDB7A77E13F</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__A2DED2051612450A9E2962309C61F19C</td><td>{9F142B10-053B-472B-A54E-5348F842A0AC}</td><td>_ABF01A60B67E49A8953409B9471A5989</td><td>2</td><td/><td>_A2DED2051612450A9E2962309C61F19C</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__A473537B6DB04D899D6E1FD1979FA3CF</td><td>{FAC0314A-8172-4ABA-A28A-A0F159B0387F}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td>_A473537B6DB04D899D6E1FD1979FA3CF</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__A530EDAA5E624663ABB83D59D42186F6</td><td>{D5CCACA6-F728-4F2F-AAFF-E284B06EB532}</td><td>INSTALLDIR</td><td>258</td><td/><td>_A530EDAA5E624663ABB83D59D42186F6</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__A6AFAFB1B104453FADE31BE15EA14353</td><td>{56F821F6-F8F2-4472-9A97-F04C425D7A92}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td>_A6AFAFB1B104453FADE31BE15EA14353</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__B099E4926AFE43EAB395438D0D8D9B77</td><td>{5FCC480F-0F3D-4A5B-B208-9BAE6228B432}</td><td>INSTALLDIR</td><td>258</td><td/><td>_B099E4926AFE43EAB395438D0D8D9B77</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__B39748BC31D6422EBB6B2D72123E92AA</td><td>{D27CAB26-1816-4B8D-8A51-67233DD6BEB6}</td><td>_A28F2D48A7A14BEA9D34FCDE011F1513</td><td>258</td><td/><td>_B39748BC31D6422EBB6B2D72123E92AA</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__BBCC268121A94302A63046EF90320A3A</td><td>{FB3EF68B-FE44-4BB6-AF66-ED4359DC0867}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td>_BBCC268121A94302A63046EF90320A3A</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__C230765F762B47B18C7DBED550986ABA</td><td>{CCC54D6C-1923-4D4F-B488-6615FD0788AA}</td><td>_ABF01A60B67E49A8953409B9471A5989</td><td>2</td><td/><td>_C230765F762B47B18C7DBED550986ABA</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__C3C4913F460544FE9F4C0E2E900DB356</td><td>{6958AF16-2660-423B-9F9D-221B68B950DB}</td><td>_D4445AED10494F4C88881E86F0499B93</td><td>258</td><td/><td>_C3C4913F460544FE9F4C0E2E900DB356</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__C403BA6E9E194CD592A7C50EDE7E939B</td><td>{D10405C9-339A-4E94-8B25-6B42724F7BBE}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td>_C403BA6E9E194CD592A7C50EDE7E939B</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__D1F8CF020B5A451697624F490F28BE54</td><td>{3FD9DA89-9FB5-4570-B61F-F85244C77C35}</td><td>INSTALLDIR</td><td>258</td><td/><td>_D1F8CF020B5A451697624F490F28BE54</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__D3758339EA714A17984DE22F4644FEE9</td><td>{29AB7746-49ED-4336-AA9F-88696D2C83B2}</td><td>INSTALLDIR</td><td>258</td><td/><td>_D3758339EA714A17984DE22F4644FEE9</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__D47657BBBCC549C9A4DA838A1DCD2DF4</td><td>{6D2B75D4-F0E0-4E20-A6CE-A6EC343892DA}</td><td>INSTALLDIR</td><td>262</td><td/><td>_D47657BBBCC549C9A4DA838A1DCD2DF4</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__DB2EB634C1AB4C1FA228A22582375EA2</td><td>{9ADC0E28-460B-4C34-8C1D-FCA3B86DF7B3}</td><td>_ABF01A60B67E49A8953409B9471A5989</td><td>2</td><td/><td>_DB2EB634C1AB4C1FA228A22582375EA2</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__E22EF8970F184A29A67C6B5080E0E021</td><td>{6F05F36E-FE35-4B25-886C-46DA53CDA1E0}</td><td>_ABF01A60B67E49A8953409B9471A5989</td><td>2</td><td/><td>_E22EF8970F184A29A67C6B5080E0E021</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__ECA4F69504D14AFCA9E7F72A8D3B7013</td><td>{02EEA5FB-9216-4536-B2D0-E3AFB2F8E4E8}</td><td>_ABF01A60B67E49A8953409B9471A5989</td><td>2</td><td/><td>_ECA4F69504D14AFCA9E7F72A8D3B7013</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__EFD5EB60662546EF86AC8E0A611C4CA6</td><td>{F7170DAE-9F02-4003-A322-FE45D7F4C9EA}</td><td>_AF9184B1F044480AAEC8CE10016E9441</td><td>2</td><td/><td>_EFD5EB60662546EF86AC8E0A611C4CA6</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__F2555C2B1D7C49559BC04823656E2305</td><td>{F292C3A4-272A-45C3-991B-6B38116B340C}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td>_F2555C2B1D7C49559BC04823656E2305</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__F3D05398C3BA40C69E8709F43B4E3083</td><td>{9BEB5FB1-E095-46FA-BDD0-E203C43CACBE}</td><td>_04C433E6C8404B529DB7930D3A8E7AF4</td><td>2</td><td/><td>_F3D05398C3BA40C69E8709F43B4E3083</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__F4112FDF846B48FB86CE69A0545E366F</td><td>{ABB58E42-A3D9-4B81-8D7A-A55A34B81F33}</td><td>_AE568DA94C254E8DB02AC5111802BB6B</td><td>258</td><td/><td>_F4112FDF846B48FB86CE69A0545E366F</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__F58DFAEFADA3442B9526AA9947119E11</td><td>{A6A0EA1E-FCF9-4046-A3C6-2F21D481DBFA}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td>_F58DFAEFADA3442B9526AA9947119E11</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__FAF9F28B10E04850937D47023FEB058A</td><td>{F34F552C-75F0-4F71-999E-4578D69C1D82}</td><td>INSTALLDIR</td><td>258</td><td/><td>_FAF9F28B10E04850937D47023FEB058A</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>C__FEB18844F3D54D3CBA46FE552A79F646</td><td>{01F1513B-5297-490B-A61C-04800BE84BAA}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td>_FEB18844F3D54D3CBA46FE552A79F646</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>CompBase.dll</td><td>{EE132F7D-FDA3-450D-9678-6AC9D7956A10}</td><td>INSTALLDIR</td><td>258</td><td/><td>compbase.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ExprEval.dll</td><td>{BC2BCF00-D974-4DC6-AFDC-8A8D059A727A}</td><td>INSTALLDIR</td><td>258</td><td/><td>expreval.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT</td><td>{1B7CAF1C-7F44-4A22-8F46-77A9E8CC906F}</td><td>CommonAppDataFolder</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT1</td><td>{6EB1BD2C-4FB2-4E67-9D30-02E8BEBA0664}</td><td>INSTALLDIR</td><td>258</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT10</td><td>{D07646E1-103F-49AB-89E0-A83EB0FA9C1A}</td><td>_X_HERMLE_C30U_INCH</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT11</td><td>{EDC973CC-ADB0-4D96-92A2-95EE00AE84F3}</td><td>DMU80</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT12</td><td>{C98433BE-91E0-4F28-BFF9-89AE4ACBC7AC}</td><td>DM_4X_TABLE_A</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT13</td><td>{3652342C-4085-4435-A592-C5DA4D5557E8}</td><td>FADAL3X_VMC6030_INCH</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT14</td><td>{899EC730-3855-49D8-8F3C-E3326567D448}</td><td>FANMTDEMO2CHUSK</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT15</td><td>{3598CE48-51F9-46FF-A6CC-A0A8CC021AE6}</td><td>_B657F2162C104399B5A08BE8BDDA6480</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT16</td><td>{3F3CAA0F-635C-4E34-9F09-4D377C25FC03}</td><td>_20411C75B4BB4D2EB5301C1DEBF548B9</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT17</td><td>{F488C950-7014-4752-B975-C8268A5281A2}</td><td>MILL_2X</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT18</td><td>{5D23287D-11F9-4398-8327-1B2404B51B88}</td><td>MILL_3X</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT19</td><td>{C2B83223-EA42-4291-A605-31B3B6A3903E}</td><td>MILL_5X</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT2</td><td>{66604069-CDB7-4BD4-A3E2-6840E13CE3D6}</td><td>ProgramFiles64Folder</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT20</td><td>{97721D08-85BC-48E5-B7F7-D22638FE0115}</td><td>MILLTURN</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT21</td><td>{768A48FD-485B-4BDA-B8A0-D5D39F174A48}</td><td>TURN</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT22</td><td>{FD6A8417-1805-4ACD-B918-832FCDA28972}</td><td>LATHE_VERT_1512</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT23</td><td>{6CD43FB5-AE49-4AF9-A04B-4DFDDC0E31B3}</td><td>MILL4AX</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT24</td><td>{B5A2550A-A7D6-4696-85A0-1B188E7060AD}</td><td>MILLTURN_DEMO</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT25</td><td>{968BCCF4-F4BB-4DEB-8DFD-1343D1F14632}</td><td>MILLTURN_DEFAULT</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT26</td><td>{B6BA4526-A3CC-4BB7-B95B-173395EB6B6C}</td><td>MILL_DEFAULT</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT27</td><td>{55930AC9-9DC3-47EE-8A4B-4F7A3F9F0D55}</td><td>R_510</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT28</td><td>{1557F987-EEBE-46AF-A9BF-47AB422F269C}</td><td>_AF9184B1F044480AAEC8CE10016E9441</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT29</td><td>{C099AD21-4027-45FF-9ADA-76EFA91F7571}</td><td>TURN_DEFAULT</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT3</td><td>{9E67C7C6-A175-42EC-AB02-4F0AC83D569E}</td><td>C_</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT30</td><td>{C75D5CCC-E489-48FF-A0B1-9FF1DEC9ECA3}</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT31</td><td>{93F5FE98-CD21-431D-BC6F-9EF6B32C0B57}</td><td>DFN</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT32</td><td>{E953963D-9D10-41F3-AB3D-1C6DCF2180AC}</td><td>WELD</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT33</td><td>{CCABDB4D-8517-4CAB-843D-0B36FCF86D62}</td><td>_D4445AED10494F4C88881E86F0499B93</td><td>258</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT35</td><td>{D598304A-EBEC-43F7-B339-89FFA836DC6A}</td><td>_X_WITH_TOOLS_20</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT36</td><td>{E7E2F8EA-F207-488F-BD33-BF3F8C68DE7F}</td><td>_X_WITH_TOOLS_30</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT37</td><td>{592615D7-92B4-4A10-8E63-F5513C68AB80}</td><td>_X_A_WITH_TOOLS_20</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT38</td><td>{4B9A1FA9-3C34-4145-8144-14ED9CFDF4A6}</td><td>_04C433E6C8404B529DB7930D3A8E7AF4</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT39</td><td>{698BFE2F-1897-4A27-87C4-8DA8EE4AA084}</td><td>TOOLS_T</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT4</td><td>{13D43F54-3A18-4982-83B3-12EA0E94B9F1}</td><td>_2A3D6461C84746B08B96C228301D3D56</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT40</td><td>{389FE24E-4B08-4DC8-BEB4-23BF3BB2D6C8}</td><td>_96CD38D4AC6C4E08AFFFF2C9F3108B20</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT41</td><td>{1A9A0B85-6257-455E-902A-3387B69274FA}</td><td>GOODWAY_GLS_1500_MT</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT42</td><td>{6CEE2036-85ED-417F-BD37-E2D41C653563}</td><td>HYUNDAI_WIA_L230A</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT43</td><td>{1AD728DE-796A-4667-8DF5-E6F4BC5F1580}</td><td>VICTOR_TAICHUNG_A110</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT5</td><td>{C38A1A75-5824-44DE-B6AE-52891D386499}</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT6</td><td>{27F2C31C-E7E3-483B-B385-91E46B1FEBE5}</td><td>_A516MF3</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT7</td><td>{1E7A3BF6-5A12-4157-992B-871B7EACA778}</td><td>_X_DEFAULT</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT8</td><td>{59272BF6-0B08-4441-9DC3-ECFF8E54B592}</td><td>_X_HARDINGE_VMC</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>ISX_DEFAULTCOMPONENT9</td><td>{79347996-2FFC-4E7A-8B10-2F795C8F7E56}</td><td>_X_HEAD_X_ON_HEAD_Z</td><td>2</td><td/><td/><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>NCMHandlers.dll</td><td>{A103DBEC-8C85-451E-AEA0-1BD625CE9067}</td><td>INSTALLDIR</td><td>258</td><td/><td>ncmhandlers.dll</td><td>17</td><td/><td>_5AB0F74B62D24B2797103046CCAAD98E</td><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>NCManager.chm</td><td>{9C072EBF-BE2D-42C3-BB7A-E8553A8E54CF}</td><td>_AE568DA94C254E8DB02AC5111802BB6B</td><td>258</td><td/><td>ncmanager.chm</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>NCManager.chm1</td><td>{E56A0038-3B6C-4978-9B57-2EF841274C81}</td><td>_A28F2D48A7A14BEA9D34FCDE011F1513</td><td>258</td><td/><td>ncmanager.chm1</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>OCImport.dll</td><td>{30739E92-BCA6-4238-BCB4-5B9AEF224FAD}</td><td>INSTALLDIR</td><td>258</td><td/><td>ocimport.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>TKBO.dll</td><td>{632D41F5-2219-4154-BCE0-CA6836ED1830}</td><td>INSTALLDIR</td><td>258</td><td/><td>tkbo.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>TKBRep.dll</td><td>{68907B12-B5F3-4707-A55F-11988391EDFE}</td><td>INSTALLDIR</td><td>258</td><td/><td>tkbrep.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>TKBool.dll</td><td>{CD7F3AE4-B740-4DD3-8C70-FF21FF3E4100}</td><td>INSTALLDIR</td><td>258</td><td/><td>tkbool.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>TKG2d.dll</td><td>{3BAF54D2-4DDC-4C3B-BEE4-3FF90065ADA1}</td><td>INSTALLDIR</td><td>258</td><td/><td>tkg2d.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>TKG3d.dll</td><td>{5058CF47-DF06-4453-8981-A0C1A9DD0C4D}</td><td>INSTALLDIR</td><td>258</td><td/><td>tkg3d.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>TKGeomAlgo.dll</td><td>{EAEE2B27-F7F1-4C69-A6AF-53D147D21011}</td><td>INSTALLDIR</td><td>258</td><td/><td>tkgeomalgo.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>TKGeomBase.dll</td><td>{536B7530-E0D3-4D2C-AEA8-813CF350A595}</td><td>INSTALLDIR</td><td>258</td><td/><td>tkgeombase.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>TKIGES.dll</td><td>{E76090A3-5946-422E-BB3D-F71BB18DFA01}</td><td>INSTALLDIR</td><td>258</td><td/><td>tkiges.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>TKMath.dll</td><td>{B6456C7E-142D-4DAC-B38B-7C07461D0930}</td><td>INSTALLDIR</td><td>258</td><td/><td>tkmath.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>TKMesh.dll</td><td>{509AC830-CCE8-4840-9C7C-861908925ED7}</td><td>INSTALLDIR</td><td>258</td><td/><td>tkmesh.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>TKPrim.dll</td><td>{C923E62D-7482-4DFF-A1FC-3E30EBB97F4E}</td><td>INSTALLDIR</td><td>258</td><td/><td>tkprim.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>TKSTEP.dll</td><td>{B12B8D73-6820-44DB-9863-681B014660E0}</td><td>INSTALLDIR</td><td>258</td><td/><td>tkstep.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>TKSTEP209.dll</td><td>{DAEA5E86-33DF-44F8-91CC-96C880250F33}</td><td>INSTALLDIR</td><td>258</td><td/><td>tkstep209.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>TKSTEPAttr.dll</td><td>{ECF13547-1F8F-4A34-AA6D-9EE63B885286}</td><td>INSTALLDIR</td><td>258</td><td/><td>tkstepattr.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>TKSTEPBase.dll</td><td>{6D809CF3-CFA9-432C-B0D4-AA9E15D434DC}</td><td>INSTALLDIR</td><td>258</td><td/><td>tkstepbase.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>TKSTL.dll</td><td>{7B0B3FD3-475D-4B8C-A495-00E7FFEFF5D0}</td><td>INSTALLDIR</td><td>258</td><td/><td>tkstl.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>TKShHealing.dll</td><td>{BCCA26A7-A149-462C-9B8B-AF66818BD3ED}</td><td>INSTALLDIR</td><td>258</td><td/><td>tkshhealing.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>TKTopAlgo.dll</td><td>{3D80DFBB-7081-4EEC-BE44-AF44C9B4F937}</td><td>INSTALLDIR</td><td>258</td><td/><td>tktopalgo.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>TKXSBase.dll</td><td>{B04F30A9-2B35-4FB6-92D1-68EC8ADAA141}</td><td>INSTALLDIR</td><td>258</td><td/><td>tkxsbase.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>TKernel.dll</td><td>{8728DE33-70F2-4EB9-B98F-134CB0609B52}</td><td>INSTALLDIR</td><td>258</td><td/><td>tkernel.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>header.dll</td><td>{28A51D2B-CAFF-4832-A86A-A8CD7BFA03CF}</td><td>DFN</td><td>2</td><td/><td>header.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
		<row><td>zlibwapi.dll</td><td>{624CA7C2-E00D-471D-83C8-74E87B08F69C}</td><td>INSTALLDIR</td><td>258</td><td/><td>zlibwapi.dll</td><td>17</td><td/><td/><td/><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td><td>/LogFile=</td></row>
	</table>

	<table name="Condition">
		<col key="yes" def="s38">Feature_</col>
		<col key="yes" def="i2">Level</col>
		<col def="S255">Condition</col>
	</table>

	<table name="Control">
		<col key="yes" def="s72">Dialog_</col>
		<col key="yes" def="s50">Control</col>
		<col def="s20">Type</col>
		<col def="i2">X</col>
		<col def="i2">Y</col>
		<col def="i2">Width</col>
		<col def="i2">Height</col>
		<col def="I4">Attributes</col>
		<col def="S72">Property</col>
		<col def="L0">Text</col>
		<col def="S50">Control_Next</col>
		<col def="L50">Help</col>
		<col def="I4">ISWindowStyle</col>
		<col def="I4">ISControlId</col>
		<col def="S255">ISBuildSourcePath</col>
		<col def="S72">Binary_</col>
		<row><td>AdminChangeFolder</td><td>Banner</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>44</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary1</td></row>
		<row><td>AdminChangeFolder</td><td>BannerLine</td><td>Line</td><td>0</td><td>44</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminChangeFolder</td><td>Branding1</td><td>Text</td><td>4</td><td>229</td><td>50</td><td>13</td><td>3</td><td/><td>##IDS_INSTALLSHIELD_FORMATTED##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminChangeFolder</td><td>Branding2</td><td>Text</td><td>3</td><td>228</td><td>50</td><td>13</td><td>65537</td><td/><td>##IDS_INSTALLSHIELD##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminChangeFolder</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td>ComboText</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminChangeFolder</td><td>Combo</td><td>DirectoryCombo</td><td>21</td><td>64</td><td>277</td><td>80</td><td>458755</td><td>TARGETDIR</td><td>##IDS__IsAdminInstallBrowse_4##</td><td>Up</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminChangeFolder</td><td>ComboText</td><td>Text</td><td>21</td><td>50</td><td>99</td><td>14</td><td>3</td><td/><td>##IDS__IsAdminInstallBrowse_LookIn##</td><td>Combo</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminChangeFolder</td><td>DlgDesc</td><td>Text</td><td>21</td><td>23</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsAdminInstallBrowse_BrowseDestination##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminChangeFolder</td><td>DlgLine</td><td>Line</td><td>48</td><td>234</td><td>326</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminChangeFolder</td><td>DlgTitle</td><td>Text</td><td>13</td><td>6</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsAdminInstallBrowse_ChangeDestination##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminChangeFolder</td><td>List</td><td>DirectoryList</td><td>21</td><td>90</td><td>332</td><td>97</td><td>7</td><td>TARGETDIR</td><td>##IDS__IsAdminInstallBrowse_8##</td><td>TailText</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminChangeFolder</td><td>NewFolder</td><td>PushButton</td><td>335</td><td>66</td><td>19</td><td>19</td><td>3670019</td><td/><td/><td>List</td><td>##IDS__IsAdminInstallBrowse_CreateFolder##</td><td>0</td><td/><td/><td>NewBinary2</td></row>
		<row><td>AdminChangeFolder</td><td>OK</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_OK##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminChangeFolder</td><td>Tail</td><td>PathEdit</td><td>21</td><td>207</td><td>332</td><td>17</td><td>3</td><td>TARGETDIR</td><td>##IDS__IsAdminInstallBrowse_11##</td><td>OK</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminChangeFolder</td><td>TailText</td><td>Text</td><td>21</td><td>193</td><td>99</td><td>13</td><td>3</td><td/><td>##IDS__IsAdminInstallBrowse_FolderName##</td><td>Tail</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminChangeFolder</td><td>Up</td><td>PushButton</td><td>310</td><td>66</td><td>19</td><td>19</td><td>3670019</td><td/><td/><td>NewFolder</td><td>##IDS__IsAdminInstallBrowse_UpOneLevel##</td><td>0</td><td/><td/><td>NewBinary3</td></row>
		<row><td>AdminNetworkLocation</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_BACK##</td><td>InstallNow</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminNetworkLocation</td><td>Banner</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>44</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary1</td></row>
		<row><td>AdminNetworkLocation</td><td>BannerLine</td><td>Line</td><td>0</td><td>44</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminNetworkLocation</td><td>Branding1</td><td>Text</td><td>4</td><td>229</td><td>50</td><td>13</td><td>3</td><td/><td>##IDS_INSTALLSHIELD_FORMATTED##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminNetworkLocation</td><td>Branding2</td><td>Text</td><td>3</td><td>228</td><td>50</td><td>13</td><td>65537</td><td/><td>##IDS_INSTALLSHIELD##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminNetworkLocation</td><td>Browse</td><td>PushButton</td><td>286</td><td>124</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsAdminInstallPoint_Change##</td><td>Back</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminNetworkLocation</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td>SetupPathEdit</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminNetworkLocation</td><td>DlgDesc</td><td>Text</td><td>21</td><td>23</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsAdminInstallPoint_SpecifyNetworkLocation##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminNetworkLocation</td><td>DlgLine</td><td>Line</td><td>48</td><td>234</td><td>326</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminNetworkLocation</td><td>DlgText</td><td>Text</td><td>21</td><td>51</td><td>326</td><td>40</td><td>131075</td><td/><td>##IDS__IsAdminInstallPoint_EnterNetworkLocation##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminNetworkLocation</td><td>DlgTitle</td><td>Text</td><td>13</td><td>6</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsAdminInstallPoint_NetworkLocationFormatted##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminNetworkLocation</td><td>InstallNow</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsAdminInstallPoint_Install##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminNetworkLocation</td><td>LBBrowse</td><td>Text</td><td>21</td><td>90</td><td>100</td><td>10</td><td>3</td><td/><td>##IDS__IsAdminInstallPoint_NetworkLocation##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminNetworkLocation</td><td>SetupPathEdit</td><td>PathEdit</td><td>21</td><td>102</td><td>330</td><td>17</td><td>3</td><td>TARGETDIR</td><td/><td>Browse</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminWelcome</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>1</td><td/><td>##IDS_BACK##</td><td>Next</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminWelcome</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td>Back</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminWelcome</td><td>DlgLine</td><td>Line</td><td>0</td><td>234</td><td>326</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminWelcome</td><td>Image</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>234</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary5</td></row>
		<row><td>AdminWelcome</td><td>Next</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_NEXT##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminWelcome</td><td>TextLine1</td><td>Text</td><td>135</td><td>8</td><td>225</td><td>45</td><td>196611</td><td/><td>##IDS__IsAdminInstallPointWelcome_Wizard##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>AdminWelcome</td><td>TextLine2</td><td>Text</td><td>135</td><td>55</td><td>228</td><td>45</td><td>196611</td><td/><td>##IDS__IsAdminInstallPointWelcome_ServerImage##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CancelSetup</td><td>Icon</td><td>Icon</td><td>15</td><td>15</td><td>24</td><td>24</td><td>5242881</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary4</td></row>
		<row><td>CancelSetup</td><td>No</td><td>PushButton</td><td>135</td><td>57</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsCancelDlg_No##</td><td>Yes</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>CancelSetup</td><td>Text</td><td>Text</td><td>48</td><td>15</td><td>194</td><td>30</td><td>131075</td><td/><td>##IDS__IsCancelDlg_ConfirmCancel##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CancelSetup</td><td>Yes</td><td>PushButton</td><td>62</td><td>57</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsCancelDlg_Yes##</td><td>No</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetup</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_BACK##</td><td>Next</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetup</td><td>Banner</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>44</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary1</td></row>
		<row><td>CustomSetup</td><td>BannerLine</td><td>Line</td><td>0</td><td>44</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetup</td><td>Branding1</td><td>Text</td><td>4</td><td>229</td><td>50</td><td>13</td><td>3</td><td/><td>##IDS_INSTALLSHIELD_FORMATTED##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetup</td><td>Branding2</td><td>Text</td><td>3</td><td>228</td><td>50</td><td>13</td><td>65537</td><td/><td>##IDS_INSTALLSHIELD##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetup</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td>Tree</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetup</td><td>ChangeFolder</td><td>PushButton</td><td>301</td><td>203</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsCustomSelectionDlg_Change##</td><td>Help</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetup</td><td>Details</td><td>PushButton</td><td>93</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsCustomSelectionDlg_Space##</td><td>Back</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetup</td><td>DlgDesc</td><td>Text</td><td>17</td><td>23</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsCustomSelectionDlg_SelectFeatures##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetup</td><td>DlgLine</td><td>Line</td><td>48</td><td>234</td><td>326</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetup</td><td>DlgText</td><td>Text</td><td>9</td><td>51</td><td>360</td><td>10</td><td>3</td><td/><td>##IDS__IsCustomSelectionDlg_ClickFeatureIcon##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetup</td><td>DlgTitle</td><td>Text</td><td>9</td><td>6</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsCustomSelectionDlg_CustomSetup##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetup</td><td>FeatureGroup</td><td>GroupBox</td><td>235</td><td>67</td><td>131</td><td>120</td><td>1</td><td/><td>##IDS__IsCustomSelectionDlg_FeatureDescription##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetup</td><td>Help</td><td>PushButton</td><td>22</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsCustomSelectionDlg_Help##</td><td>Details</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetup</td><td>InstallLabel</td><td>Text</td><td>8</td><td>190</td><td>360</td><td>10</td><td>3</td><td/><td>##IDS__IsCustomSelectionDlg_InstallTo##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetup</td><td>ItemDescription</td><td>Text</td><td>241</td><td>80</td><td>120</td><td>50</td><td>3</td><td/><td>##IDS__IsCustomSelectionDlg_MultilineDescription##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetup</td><td>Location</td><td>Text</td><td>8</td><td>203</td><td>291</td><td>20</td><td>3</td><td/><td>##IDS__IsCustomSelectionDlg_FeaturePath##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetup</td><td>Next</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_NEXT##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetup</td><td>Size</td><td>Text</td><td>241</td><td>133</td><td>120</td><td>50</td><td>3</td><td/><td>##IDS__IsCustomSelectionDlg_FeatureSize##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetup</td><td>Tree</td><td>SelectionTree</td><td>8</td><td>70</td><td>220</td><td>118</td><td>7</td><td>_BrowseProperty</td><td/><td>ChangeFolder</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetupTips</td><td>Banner</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>44</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary1</td></row>
		<row><td>CustomSetupTips</td><td>BannerLine</td><td>Line</td><td>0</td><td>44</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetupTips</td><td>Branding1</td><td>Text</td><td>4</td><td>229</td><td>50</td><td>13</td><td>3</td><td/><td>##IDS_INSTALLSHIELD_FORMATTED##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetupTips</td><td>Branding2</td><td>Text</td><td>3</td><td>228</td><td>50</td><td>13</td><td>65537</td><td/><td>##IDS_INSTALLSHIELD##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetupTips</td><td>DlgDesc</td><td>Text</td><td>21</td><td>23</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS_SetupTips_CustomSetupDescription##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetupTips</td><td>DlgLine</td><td>Line</td><td>48</td><td>234</td><td>326</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetupTips</td><td>DlgTitle</td><td>Text</td><td>13</td><td>6</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS_SetupTips_CustomSetup##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetupTips</td><td>DontInstall</td><td>Icon</td><td>21</td><td>155</td><td>24</td><td>24</td><td>5242881</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary14</td></row>
		<row><td>CustomSetupTips</td><td>DontInstallText</td><td>Text</td><td>60</td><td>155</td><td>300</td><td>20</td><td>3</td><td/><td>##IDS_SetupTips_WillNotBeInstalled##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetupTips</td><td>FirstInstallText</td><td>Text</td><td>60</td><td>180</td><td>300</td><td>20</td><td>3</td><td/><td>##IDS_SetupTips_Advertise##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetupTips</td><td>Install</td><td>Icon</td><td>21</td><td>105</td><td>24</td><td>24</td><td>5242881</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary15</td></row>
		<row><td>CustomSetupTips</td><td>InstallFirstUse</td><td>Icon</td><td>21</td><td>180</td><td>24</td><td>24</td><td>5242881</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary16</td></row>
		<row><td>CustomSetupTips</td><td>InstallPartial</td><td>Icon</td><td>21</td><td>130</td><td>24</td><td>24</td><td>5242881</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary17</td></row>
		<row><td>CustomSetupTips</td><td>InstallStateMenu</td><td>Icon</td><td>21</td><td>52</td><td>24</td><td>24</td><td>5242881</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary18</td></row>
		<row><td>CustomSetupTips</td><td>InstallStateText</td><td>Text</td><td>21</td><td>91</td><td>300</td><td>10</td><td>3</td><td/><td>##IDS_SetupTips_InstallState##</td><td/><td/><td>0</td><td>0</td><td/><td/></row>
		<row><td>CustomSetupTips</td><td>InstallText</td><td>Text</td><td>60</td><td>105</td><td>300</td><td>20</td><td>3</td><td/><td>##IDS_SetupTips_AllInstalledLocal##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetupTips</td><td>MenuText</td><td>Text</td><td>50</td><td>52</td><td>300</td><td>36</td><td>3</td><td/><td>##IDS_SetupTips_IconInstallState##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetupTips</td><td>NetworkInstall</td><td>Icon</td><td>21</td><td>205</td><td>24</td><td>24</td><td>5242881</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary13</td></row>
		<row><td>CustomSetupTips</td><td>NetworkInstallText</td><td>Text</td><td>60</td><td>205</td><td>300</td><td>20</td><td>3</td><td/><td>##IDS_SetupTips_Network##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetupTips</td><td>OK</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_SetupTips_OK##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomSetupTips</td><td>PartialText</td><td>Text</td><td>60</td><td>130</td><td>300</td><td>20</td><td>3</td><td/><td>##IDS_SetupTips_SubFeaturesInstalledLocal##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomerInformation</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_BACK##</td><td>Next</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomerInformation</td><td>Banner</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>44</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary1</td></row>
		<row><td>CustomerInformation</td><td>BannerLine</td><td>Line</td><td>0</td><td>44</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomerInformation</td><td>Branding1</td><td>Text</td><td>4</td><td>229</td><td>50</td><td>13</td><td>3</td><td/><td>##IDS_INSTALLSHIELD_FORMATTED##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomerInformation</td><td>Branding2</td><td>Text</td><td>3</td><td>228</td><td>50</td><td>13</td><td>65537</td><td/><td>##IDS_INSTALLSHIELD##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomerInformation</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td>NameLabel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomerInformation</td><td>CompanyEdit</td><td>Edit</td><td>21</td><td>100</td><td>237</td><td>17</td><td>3</td><td>COMPANYNAME</td><td>##IDS__IsRegisterUserDlg_Tahoma80##</td><td>SerialLabel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomerInformation</td><td>CompanyLabel</td><td>Text</td><td>21</td><td>89</td><td>75</td><td>10</td><td>3</td><td/><td>##IDS__IsRegisterUserDlg_Organization##</td><td>CompanyEdit</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomerInformation</td><td>DlgDesc</td><td>Text</td><td>21</td><td>23</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsRegisterUserDlg_PleaseEnterInfo##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomerInformation</td><td>DlgLine</td><td>Line</td><td>48</td><td>234</td><td>326</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomerInformation</td><td>DlgRadioGroupText</td><td>Text</td><td>21</td><td>161</td><td>300</td><td>14</td><td>2</td><td/><td>##IDS__IsRegisterUserDlg_InstallFor##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomerInformation</td><td>DlgTitle</td><td>Text</td><td>13</td><td>6</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsRegisterUserDlg_CustomerInformation##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomerInformation</td><td>NameEdit</td><td>Edit</td><td>21</td><td>63</td><td>237</td><td>17</td><td>3</td><td>USERNAME</td><td>##IDS__IsRegisterUserDlg_Tahoma50##</td><td>CompanyLabel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomerInformation</td><td>NameLabel</td><td>Text</td><td>21</td><td>52</td><td>75</td><td>10</td><td>3</td><td/><td>##IDS__IsRegisterUserDlg_UserName##</td><td>NameEdit</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomerInformation</td><td>Next</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_NEXT##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomerInformation</td><td>RadioGroup</td><td>RadioButtonGroup</td><td>63</td><td>170</td><td>300</td><td>50</td><td>2</td><td>ApplicationUsers</td><td>##IDS__IsRegisterUserDlg_16##</td><td>Back</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomerInformation</td><td>SerialLabel</td><td>Text</td><td>21</td><td>127</td><td>109</td><td>10</td><td>2</td><td/><td>##IDS__IsRegisterUserDlg_SerialNumber##</td><td>SerialNumber</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>CustomerInformation</td><td>SerialNumber</td><td>MaskedEdit</td><td>21</td><td>138</td><td>237</td><td>17</td><td>2</td><td>ISX_SERIALNUM</td><td/><td>RadioGroup</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>DatabaseFolder</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_BACK##</td><td>Next</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>DatabaseFolder</td><td>Banner</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>44</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary1</td></row>
		<row><td>DatabaseFolder</td><td>BannerLine</td><td>Line</td><td>0</td><td>44</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DatabaseFolder</td><td>Branding1</td><td>Text</td><td>4</td><td>229</td><td>50</td><td>13</td><td>3</td><td/><td>##IDS_INSTALLSHIELD_FORMATTED##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DatabaseFolder</td><td>Branding2</td><td>Text</td><td>3</td><td>228</td><td>50</td><td>13</td><td>65537</td><td/><td>##IDS_INSTALLSHIELD##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DatabaseFolder</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td>ChangeFolder</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>DatabaseFolder</td><td>ChangeFolder</td><td>PushButton</td><td>301</td><td>65</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CHANGE##</td><td>Back</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>DatabaseFolder</td><td>DatabaseFolder</td><td>Icon</td><td>21</td><td>52</td><td>24</td><td>24</td><td>5242881</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary12</td></row>
		<row><td>DatabaseFolder</td><td>DlgDesc</td><td>Text</td><td>21</td><td>23</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__DatabaseFolder_ChangeFolder##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DatabaseFolder</td><td>DlgLine</td><td>Line</td><td>48</td><td>234</td><td>326</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DatabaseFolder</td><td>DlgTitle</td><td>Text</td><td>13</td><td>6</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__DatabaseFolder_DatabaseFolder##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DatabaseFolder</td><td>LocLabel</td><td>Text</td><td>57</td><td>52</td><td>290</td><td>10</td><td>131075</td><td/><td>##IDS_DatabaseFolder_InstallDatabaseTo##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DatabaseFolder</td><td>Location</td><td>Text</td><td>57</td><td>65</td><td>240</td><td>40</td><td>3</td><td>_BrowseProperty</td><td>##IDS__DatabaseFolder_DatabaseDir##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DatabaseFolder</td><td>Next</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_NEXT##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>DestinationFolder</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_BACK##</td><td>Next</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>DestinationFolder</td><td>Banner</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>44</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary1</td></row>
		<row><td>DestinationFolder</td><td>BannerLine</td><td>Line</td><td>0</td><td>44</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DestinationFolder</td><td>Branding1</td><td>Text</td><td>4</td><td>229</td><td>50</td><td>13</td><td>3</td><td/><td>##IDS_INSTALLSHIELD_FORMATTED##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DestinationFolder</td><td>Branding2</td><td>Text</td><td>3</td><td>228</td><td>50</td><td>13</td><td>65537</td><td/><td>##IDS_INSTALLSHIELD##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DestinationFolder</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td>ChangeFolder</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>DestinationFolder</td><td>ChangeFolder</td><td>PushButton</td><td>301</td><td>65</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__DestinationFolder_Change##</td><td>Back</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>DestinationFolder</td><td>DestFolder</td><td>Icon</td><td>21</td><td>52</td><td>24</td><td>24</td><td>5242881</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary12</td></row>
		<row><td>DestinationFolder</td><td>DlgDesc</td><td>Text</td><td>21</td><td>23</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__DestinationFolder_ChangeFolder##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DestinationFolder</td><td>DlgLine</td><td>Line</td><td>48</td><td>234</td><td>326</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DestinationFolder</td><td>DlgTitle</td><td>Text</td><td>13</td><td>6</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__DestinationFolder_DestinationFolder##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DestinationFolder</td><td>LocLabel</td><td>Text</td><td>57</td><td>52</td><td>290</td><td>10</td><td>131075</td><td/><td>##IDS__DestinationFolder_InstallTo##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DestinationFolder</td><td>Location</td><td>Text</td><td>57</td><td>65</td><td>240</td><td>40</td><td>3</td><td>_BrowseProperty</td><td>##IDS_INSTALLDIR##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DestinationFolder</td><td>Next</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_NEXT##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>DiskSpaceRequirements</td><td>Banner</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>44</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary1</td></row>
		<row><td>DiskSpaceRequirements</td><td>BannerLine</td><td>Line</td><td>0</td><td>44</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DiskSpaceRequirements</td><td>Branding1</td><td>Text</td><td>4</td><td>229</td><td>50</td><td>13</td><td>3</td><td/><td>##IDS_INSTALLSHIELD_FORMATTED##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DiskSpaceRequirements</td><td>Branding2</td><td>Text</td><td>3</td><td>228</td><td>50</td><td>13</td><td>65537</td><td/><td>##IDS_INSTALLSHIELD##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DiskSpaceRequirements</td><td>DlgDesc</td><td>Text</td><td>17</td><td>23</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsFeatureDetailsDlg_SpaceRequired##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DiskSpaceRequirements</td><td>DlgLine</td><td>Line</td><td>48</td><td>234</td><td>326</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DiskSpaceRequirements</td><td>DlgText</td><td>Text</td><td>10</td><td>185</td><td>358</td><td>41</td><td>3</td><td/><td>##IDS__IsFeatureDetailsDlg_VolumesTooSmall##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DiskSpaceRequirements</td><td>DlgTitle</td><td>Text</td><td>9</td><td>6</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsFeatureDetailsDlg_DiskSpaceRequirements##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DiskSpaceRequirements</td><td>List</td><td>VolumeCostList</td><td>8</td><td>55</td><td>358</td><td>125</td><td>393223</td><td/><td>##IDS__IsFeatureDetailsDlg_Numbers##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>DiskSpaceRequirements</td><td>OK</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsFeatureDetailsDlg_OK##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>FilesInUse</td><td>Banner</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>44</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary1</td></row>
		<row><td>FilesInUse</td><td>BannerLine</td><td>Line</td><td>0</td><td>44</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>FilesInUse</td><td>Branding1</td><td>Text</td><td>4</td><td>229</td><td>50</td><td>13</td><td>3</td><td/><td>##IDS_INSTALLSHIELD_FORMATTED##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>FilesInUse</td><td>Branding2</td><td>Text</td><td>3</td><td>228</td><td>50</td><td>13</td><td>65537</td><td/><td>##IDS_INSTALLSHIELD##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>FilesInUse</td><td>DlgDesc</td><td>Text</td><td>21</td><td>23</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsFilesInUse_FilesInUseMessage##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>FilesInUse</td><td>DlgLine</td><td>Line</td><td>48</td><td>234</td><td>326</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>FilesInUse</td><td>DlgText</td><td>Text</td><td>21</td><td>51</td><td>348</td><td>33</td><td>3</td><td/><td>##IDS__IsFilesInUse_ApplicationsUsingFiles##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>FilesInUse</td><td>DlgTitle</td><td>Text</td><td>13</td><td>6</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsFilesInUse_FilesInUse##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>FilesInUse</td><td>Exit</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsFilesInUse_Exit##</td><td>List</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>FilesInUse</td><td>Ignore</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsFilesInUse_Ignore##</td><td>Exit</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>FilesInUse</td><td>List</td><td>ListBox</td><td>21</td><td>87</td><td>331</td><td>135</td><td>7</td><td>FileInUseProcess</td><td/><td>Retry</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>FilesInUse</td><td>Retry</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsFilesInUse_Retry##</td><td>Ignore</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>InstallChangeFolder</td><td>Banner</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>44</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary1</td></row>
		<row><td>InstallChangeFolder</td><td>BannerLine</td><td>Line</td><td>0</td><td>44</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>InstallChangeFolder</td><td>Branding1</td><td>Text</td><td>4</td><td>229</td><td>50</td><td>13</td><td>3</td><td/><td>##IDS_INSTALLSHIELD_FORMATTED##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>InstallChangeFolder</td><td>Branding2</td><td>Text</td><td>3</td><td>228</td><td>50</td><td>13</td><td>65537</td><td/><td>##IDS_INSTALLSHIELD##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>InstallChangeFolder</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td>ComboText</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>InstallChangeFolder</td><td>Combo</td><td>DirectoryCombo</td><td>21</td><td>64</td><td>277</td><td>80</td><td>4128779</td><td>_BrowseProperty</td><td>##IDS__IsBrowseFolderDlg_4##</td><td>Up</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>InstallChangeFolder</td><td>ComboText</td><td>Text</td><td>21</td><td>50</td><td>99</td><td>14</td><td>3</td><td/><td>##IDS__IsBrowseFolderDlg_LookIn##</td><td>Combo</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>InstallChangeFolder</td><td>DlgDesc</td><td>Text</td><td>21</td><td>23</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsBrowseFolderDlg_BrowseDestFolder##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>InstallChangeFolder</td><td>DlgLine</td><td>Line</td><td>48</td><td>234</td><td>326</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>InstallChangeFolder</td><td>DlgTitle</td><td>Text</td><td>13</td><td>6</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsBrowseFolderDlg_ChangeCurrentFolder##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>InstallChangeFolder</td><td>List</td><td>DirectoryList</td><td>21</td><td>90</td><td>332</td><td>97</td><td>15</td><td>_BrowseProperty</td><td>##IDS__IsBrowseFolderDlg_8##</td><td>TailText</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>InstallChangeFolder</td><td>NewFolder</td><td>PushButton</td><td>335</td><td>66</td><td>19</td><td>19</td><td>3670019</td><td/><td/><td>List</td><td>##IDS__IsBrowseFolderDlg_CreateFolder##</td><td>0</td><td/><td/><td>NewBinary2</td></row>
		<row><td>InstallChangeFolder</td><td>OK</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsBrowseFolderDlg_OK##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>InstallChangeFolder</td><td>Tail</td><td>PathEdit</td><td>21</td><td>207</td><td>332</td><td>17</td><td>15</td><td>_BrowseProperty</td><td>##IDS__IsBrowseFolderDlg_11##</td><td>OK</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>InstallChangeFolder</td><td>TailText</td><td>Text</td><td>21</td><td>193</td><td>99</td><td>13</td><td>3</td><td/><td>##IDS__IsBrowseFolderDlg_FolderName##</td><td>Tail</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>InstallChangeFolder</td><td>Up</td><td>PushButton</td><td>310</td><td>66</td><td>19</td><td>19</td><td>3670019</td><td/><td/><td>NewFolder</td><td>##IDS__IsBrowseFolderDlg_UpOneLevel##</td><td>0</td><td/><td/><td>NewBinary3</td></row>
		<row><td>InstallWelcome</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>1</td><td/><td>##IDS_BACK##</td><td>Copyright</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>InstallWelcome</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td>Back</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>InstallWelcome</td><td>Copyright</td><td>Text</td><td>135</td><td>144</td><td>228</td><td>73</td><td>65539</td><td/><td>##IDS__IsWelcomeDlg_WarningCopyright##</td><td>Next</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>InstallWelcome</td><td>DlgLine</td><td>Line</td><td>0</td><td>234</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>InstallWelcome</td><td>Image</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>234</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary20</td></row>
		<row><td>InstallWelcome</td><td>Next</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_NEXT##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>InstallWelcome</td><td>TextLine1</td><td>Text</td><td>135</td><td>8</td><td>225</td><td>45</td><td>196611</td><td/><td>##IDS__IsWelcomeDlg_WelcomeProductName##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>InstallWelcome</td><td>TextLine2</td><td>Text</td><td>135</td><td>55</td><td>228</td><td>45</td><td>196611</td><td/><td>##IDS__IsWelcomeDlg_InstallProductName##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>LicenseAgreement</td><td>Agree</td><td>RadioButtonGroup</td><td>8</td><td>190</td><td>291</td><td>40</td><td>3</td><td>AgreeToLicense</td><td/><td>Back</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>LicenseAgreement</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_BACK##</td><td>Next</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>LicenseAgreement</td><td>Banner</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>44</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary1</td></row>
		<row><td>LicenseAgreement</td><td>BannerLine</td><td>Line</td><td>0</td><td>44</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>LicenseAgreement</td><td>Branding1</td><td>Text</td><td>4</td><td>229</td><td>50</td><td>13</td><td>3</td><td/><td>##IDS_INSTALLSHIELD_FORMATTED##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>LicenseAgreement</td><td>Branding2</td><td>Text</td><td>3</td><td>228</td><td>50</td><td>13</td><td>65537</td><td/><td>##IDS_INSTALLSHIELD##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>LicenseAgreement</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td>ISPrintButton</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>LicenseAgreement</td><td>DlgDesc</td><td>Text</td><td>21</td><td>23</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsLicenseDlg_ReadLicenseAgreement##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>LicenseAgreement</td><td>DlgLine</td><td>Line</td><td>48</td><td>234</td><td>326</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>LicenseAgreement</td><td>DlgTitle</td><td>Text</td><td>13</td><td>6</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsLicenseDlg_LicenseAgreement##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>LicenseAgreement</td><td>ISPrintButton</td><td>PushButton</td><td>301</td><td>188</td><td>65</td><td>17</td><td>3</td><td/><td>##IDS_PRINT_BUTTON##</td><td>Agree</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>LicenseAgreement</td><td>Memo</td><td>ScrollableText</td><td>8</td><td>55</td><td>358</td><td>130</td><td>7</td><td/><td/><td/><td/><td>0</td><td/><td>&lt;ISProductFolder&gt;\Redist\0409\Eula.rtf</td><td/></row>
		<row><td>LicenseAgreement</td><td>Next</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_NEXT##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>MaintenanceType</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_BACK##</td><td>Next</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>MaintenanceType</td><td>Banner</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>44</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary1</td></row>
		<row><td>MaintenanceType</td><td>BannerLine</td><td>Line</td><td>0</td><td>44</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>MaintenanceType</td><td>Branding1</td><td>Text</td><td>4</td><td>229</td><td>50</td><td>13</td><td>3</td><td/><td>##IDS_INSTALLSHIELD_FORMATTED##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>MaintenanceType</td><td>Branding2</td><td>Text</td><td>3</td><td>228</td><td>50</td><td>13</td><td>65537</td><td/><td>##IDS_INSTALLSHIELD##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>MaintenanceType</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td>RadioGroup</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>MaintenanceType</td><td>DlgDesc</td><td>Text</td><td>21</td><td>23</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsMaintenanceDlg_MaitenanceOptions##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>MaintenanceType</td><td>DlgLine</td><td>Line</td><td>48</td><td>234</td><td>326</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>MaintenanceType</td><td>DlgTitle</td><td>Text</td><td>13</td><td>6</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsMaintenanceDlg_ProgramMaintenance##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>MaintenanceType</td><td>Ico1</td><td>Icon</td><td>35</td><td>75</td><td>24</td><td>24</td><td>5242881</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary6</td></row>
		<row><td>MaintenanceType</td><td>Ico2</td><td>Icon</td><td>35</td><td>135</td><td>24</td><td>24</td><td>5242881</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary7</td></row>
		<row><td>MaintenanceType</td><td>Ico3</td><td>Icon</td><td>35</td><td>195</td><td>24</td><td>24</td><td>5242881</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary8</td></row>
		<row><td>MaintenanceType</td><td>Next</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_NEXT##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>MaintenanceType</td><td>RadioGroup</td><td>RadioButtonGroup</td><td>21</td><td>55</td><td>290</td><td>170</td><td>3</td><td>_IsMaintenance</td><td/><td>Back</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>MaintenanceType</td><td>Text1</td><td>Text</td><td>80</td><td>72</td><td>260</td><td>35</td><td>3</td><td/><td>##IDS__IsMaintenanceDlg_ChangeFeatures##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>MaintenanceType</td><td>Text2</td><td>Text</td><td>80</td><td>135</td><td>260</td><td>35</td><td>3</td><td/><td>##IDS__IsMaintenanceDlg_RepairMessage##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>MaintenanceType</td><td>Text3</td><td>Text</td><td>80</td><td>192</td><td>260</td><td>35</td><td>131075</td><td/><td>##IDS__IsMaintenanceDlg_RemoveProductName##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>MaintenanceWelcome</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>1</td><td/><td>##IDS_BACK##</td><td>Next</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>MaintenanceWelcome</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td>Back</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>MaintenanceWelcome</td><td>DlgLine</td><td>Line</td><td>0</td><td>234</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>MaintenanceWelcome</td><td>Image</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>234</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary5</td></row>
		<row><td>MaintenanceWelcome</td><td>Next</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_NEXT##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>MaintenanceWelcome</td><td>TextLine1</td><td>Text</td><td>135</td><td>8</td><td>225</td><td>45</td><td>196611</td><td/><td>##IDS__IsMaintenanceWelcome_WizardWelcome##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>MaintenanceWelcome</td><td>TextLine2</td><td>Text</td><td>135</td><td>55</td><td>228</td><td>50</td><td>196611</td><td/><td>##IDS__IsMaintenanceWelcome_MaintenanceOptionsDescription##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>MsiRMFilesInUse</td><td>Banner</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>44</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary1</td></row>
		<row><td>MsiRMFilesInUse</td><td>BannerLine</td><td>Line</td><td>0</td><td>44</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>MsiRMFilesInUse</td><td>Branding1</td><td>Text</td><td>4</td><td>229</td><td>50</td><td>13</td><td>3</td><td/><td>##IDS_INSTALLSHIELD_FORMATTED##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>MsiRMFilesInUse</td><td>Branding2</td><td>Text</td><td>3</td><td>228</td><td>50</td><td>13</td><td>65537</td><td/><td>##IDS_INSTALLSHIELD##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>MsiRMFilesInUse</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td>Restart</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>MsiRMFilesInUse</td><td>DlgDesc</td><td>Text</td><td>21</td><td>23</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsFilesInUse_FilesInUseMessage##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>MsiRMFilesInUse</td><td>DlgLine</td><td>Line</td><td>48</td><td>234</td><td>326</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>MsiRMFilesInUse</td><td>DlgText</td><td>Text</td><td>21</td><td>51</td><td>348</td><td>14</td><td>3</td><td/><td>##IDS__IsMsiRMFilesInUse_ApplicationsUsingFiles##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>MsiRMFilesInUse</td><td>DlgTitle</td><td>Text</td><td>13</td><td>6</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsFilesInUse_FilesInUse##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>MsiRMFilesInUse</td><td>List</td><td>ListBox</td><td>21</td><td>66</td><td>331</td><td>130</td><td>3</td><td>FileInUseProcess</td><td/><td>OK</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>MsiRMFilesInUse</td><td>OK</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_OK##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>MsiRMFilesInUse</td><td>Restart</td><td>RadioButtonGroup</td><td>19</td><td>187</td><td>343</td><td>40</td><td>3</td><td>RestartManagerOption</td><td/><td>List</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>OutOfSpace</td><td>Banner</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>44</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary1</td></row>
		<row><td>OutOfSpace</td><td>BannerLine</td><td>Line</td><td>0</td><td>44</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>OutOfSpace</td><td>Branding1</td><td>Text</td><td>4</td><td>229</td><td>50</td><td>13</td><td>3</td><td/><td>##IDS_INSTALLSHIELD_FORMATTED##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>OutOfSpace</td><td>Branding2</td><td>Text</td><td>3</td><td>228</td><td>50</td><td>13</td><td>65537</td><td/><td>##IDS_INSTALLSHIELD##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>OutOfSpace</td><td>DlgDesc</td><td>Text</td><td>21</td><td>23</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsDiskSpaceDlg_DiskSpace##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>OutOfSpace</td><td>DlgLine</td><td>Line</td><td>48</td><td>234</td><td>326</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>OutOfSpace</td><td>DlgText</td><td>Text</td><td>21</td><td>51</td><td>326</td><td>43</td><td>3</td><td/><td>##IDS__IsDiskSpaceDlg_HighlightedVolumes##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>OutOfSpace</td><td>DlgTitle</td><td>Text</td><td>13</td><td>6</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsDiskSpaceDlg_OutOfDiskSpace##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>OutOfSpace</td><td>List</td><td>VolumeCostList</td><td>21</td><td>95</td><td>332</td><td>120</td><td>393223</td><td/><td>##IDS__IsDiskSpaceDlg_Numbers##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>OutOfSpace</td><td>Resume</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsDiskSpaceDlg_OK##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>PatchWelcome</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>1</td><td/><td>##IDS_BACK##</td><td>Next</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>PatchWelcome</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td>Back</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>PatchWelcome</td><td>DlgLine</td><td>Line</td><td>0</td><td>234</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>PatchWelcome</td><td>Image</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>234</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary5</td></row>
		<row><td>PatchWelcome</td><td>Next</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsPatchDlg_Update##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>PatchWelcome</td><td>TextLine1</td><td>Text</td><td>135</td><td>8</td><td>225</td><td>45</td><td>196611</td><td/><td>##IDS__IsPatchDlg_WelcomePatchWizard##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>PatchWelcome</td><td>TextLine2</td><td>Text</td><td>135</td><td>54</td><td>228</td><td>45</td><td>196611</td><td/><td>##IDS__IsPatchDlg_PatchClickUpdate##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadmeInformation</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>1048579</td><td/><td>##IDS_BACK##</td><td>Next</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadmeInformation</td><td>Banner</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>44</td><td>3</td><td/><td/><td>DlgTitle</td><td/><td>0</td><td/><td/><td>NewBinary1</td></row>
		<row><td>ReadmeInformation</td><td>Branding1</td><td>Text</td><td>4</td><td>229</td><td>50</td><td>13</td><td>3</td><td/><td>##IDS_INSTALLSHIELD_FORMATTED##</td><td/><td/><td>0</td><td>0</td><td/><td/></row>
		<row><td>ReadmeInformation</td><td>Branding2</td><td>Text</td><td>3</td><td>228</td><td>50</td><td>13</td><td>65537</td><td/><td>##IDS_INSTALLSHIELD##</td><td/><td/><td>0</td><td>0</td><td/><td/></row>
		<row><td>ReadmeInformation</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>1048579</td><td/><td>##IDS__IsReadmeDlg_Cancel##</td><td>Readme</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadmeInformation</td><td>DlgDesc</td><td>Text</td><td>21</td><td>23</td><td>232</td><td>16</td><td>65539</td><td/><td>##IDS__IsReadmeDlg_PleaseReadInfo##</td><td>Back</td><td/><td>0</td><td>0</td><td/><td/></row>
		<row><td>ReadmeInformation</td><td>DlgLine</td><td>Line</td><td>48</td><td>234</td><td>326</td><td>0</td><td>3</td><td/><td/><td/><td/><td>0</td><td>0</td><td/><td/></row>
		<row><td>ReadmeInformation</td><td>DlgTitle</td><td>Text</td><td>13</td><td>6</td><td>193</td><td>13</td><td>65539</td><td/><td>##IDS__IsReadmeDlg_ReadMeInfo##</td><td>DlgDesc</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadmeInformation</td><td>Next</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>1048579</td><td/><td>##IDS_NEXT##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadmeInformation</td><td>Readme</td><td>ScrollableText</td><td>10</td><td>55</td><td>353</td><td>166</td><td>3</td><td/><td/><td>Banner</td><td/><td>0</td><td/><td>&lt;ISProductFolder&gt;\Redist\0409\Readme.rtf</td><td/></row>
		<row><td>ReadyToInstall</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_BACK##</td><td>GroupBox1</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>Banner</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>44</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary1</td></row>
		<row><td>ReadyToInstall</td><td>BannerLine</td><td>Line</td><td>0</td><td>44</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>Branding1</td><td>Text</td><td>4</td><td>229</td><td>50</td><td>13</td><td>3</td><td/><td>##IDS_INSTALLSHIELD_FORMATTED##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>Branding2</td><td>Text</td><td>3</td><td>228</td><td>50</td><td>13</td><td>65537</td><td/><td>##IDS_INSTALLSHIELD##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td>Back</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>CompanyNameText</td><td>Text</td><td>38</td><td>198</td><td>211</td><td>9</td><td>3</td><td/><td>##IDS__IsVerifyReadyDlg_Company##</td><td>SerialNumberText</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>CurrentSettingsText</td><td>Text</td><td>19</td><td>80</td><td>81</td><td>10</td><td>3</td><td/><td>##IDS__IsVerifyReadyDlg_CurrentSettings##</td><td>InstallNow</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>DlgDesc</td><td>Text</td><td>21</td><td>23</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsVerifyReadyDlg_WizardReady##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>DlgLine</td><td>Line</td><td>48</td><td>234</td><td>326</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td>0</td><td/><td/></row>
		<row><td>ReadyToInstall</td><td>DlgText1</td><td>Text</td><td>21</td><td>54</td><td>330</td><td>24</td><td>3</td><td/><td>##IDS__IsVerifyReadyDlg_BackOrCancel##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>DlgText2</td><td>Text</td><td>21</td><td>99</td><td>330</td><td>20</td><td>2</td><td/><td>##IDS__IsRegisterUserDlg_InstallFor##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>DlgTitle</td><td>Text</td><td>13</td><td>6</td><td>292</td><td>25</td><td>65538</td><td/><td>##IDS__IsVerifyReadyDlg_ModifyReady##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>DlgTitle2</td><td>Text</td><td>13</td><td>6</td><td>292</td><td>25</td><td>65538</td><td/><td>##IDS__IsVerifyReadyDlg_ReadyRepair##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>DlgTitle3</td><td>Text</td><td>13</td><td>6</td><td>292</td><td>25</td><td>65538</td><td/><td>##IDS__IsVerifyReadyDlg_ReadyInstall##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>GroupBox1</td><td>Text</td><td>19</td><td>92</td><td>330</td><td>133</td><td>65541</td><td/><td/><td>SetupTypeText1</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>InstallNow</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>8388611</td><td/><td>##IDS__IsVerifyReadyDlg_Install##</td><td>InstallPerMachine</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>InstallPerMachine</td><td>PushButton</td><td>63</td><td>123</td><td>248</td><td>17</td><td>8388610</td><td/><td>##IDS__IsRegisterUserDlg_Anyone##</td><td>InstallPerUser</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>InstallPerUser</td><td>PushButton</td><td>63</td><td>143</td><td>248</td><td>17</td><td>2</td><td/><td>##IDS__IsRegisterUserDlg_OnlyMe##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>SerialNumberText</td><td>Text</td><td>38</td><td>211</td><td>306</td><td>9</td><td>3</td><td/><td>##IDS__IsVerifyReadyDlg_Serial##</td><td>CurrentSettingsText</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>SetupTypeText1</td><td>Text</td><td>23</td><td>97</td><td>306</td><td>13</td><td>3</td><td/><td>##IDS__IsVerifyReadyDlg_SetupType##</td><td>SetupTypeText2</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>SetupTypeText2</td><td>Text</td><td>37</td><td>114</td><td>306</td><td>14</td><td>3</td><td/><td>##IDS__IsVerifyReadyDlg_SelectedSetupType##</td><td>TargetFolderText1</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>TargetFolderText1</td><td>Text</td><td>24</td><td>136</td><td>306</td><td>11</td><td>3</td><td/><td>##IDS__IsVerifyReadyDlg_DestFolder##</td><td>TargetFolderText2</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>TargetFolderText2</td><td>Text</td><td>37</td><td>151</td><td>306</td><td>13</td><td>3</td><td/><td>##IDS__IsVerifyReadyDlg_Installdir##</td><td>UserInformationText</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>UserInformationText</td><td>Text</td><td>23</td><td>171</td><td>306</td><td>13</td><td>3</td><td/><td>##IDS__IsVerifyReadyDlg_UserInfo##</td><td>UserNameText</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToInstall</td><td>UserNameText</td><td>Text</td><td>38</td><td>184</td><td>306</td><td>9</td><td>3</td><td/><td>##IDS__IsVerifyReadyDlg_UserName##</td><td>CompanyNameText</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToRemove</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_BACK##</td><td>RemoveNow</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToRemove</td><td>Banner</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>44</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary1</td></row>
		<row><td>ReadyToRemove</td><td>BannerLine</td><td>Line</td><td>0</td><td>44</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToRemove</td><td>Branding1</td><td>Text</td><td>4</td><td>229</td><td>50</td><td>13</td><td>3</td><td/><td>##IDS_INSTALLSHIELD_FORMATTED##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToRemove</td><td>Branding2</td><td>Text</td><td>3</td><td>228</td><td>50</td><td>13</td><td>65537</td><td/><td>##IDS_INSTALLSHIELD##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToRemove</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td>Back</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToRemove</td><td>DlgDesc</td><td>Text</td><td>21</td><td>23</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsVerifyRemoveAllDlg_ChoseRemoveProgram##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToRemove</td><td>DlgLine</td><td>Line</td><td>48</td><td>234</td><td>326</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToRemove</td><td>DlgText</td><td>Text</td><td>21</td><td>51</td><td>326</td><td>24</td><td>131075</td><td/><td>##IDS__IsVerifyRemoveAllDlg_ClickRemove##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToRemove</td><td>DlgText1</td><td>Text</td><td>21</td><td>79</td><td>330</td><td>23</td><td>3</td><td/><td>##IDS__IsVerifyRemoveAllDlg_ClickBack##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToRemove</td><td>DlgText2</td><td>Text</td><td>21</td><td>102</td><td>330</td><td>24</td><td>3</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToRemove</td><td>DlgTitle</td><td>Text</td><td>13</td><td>6</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsVerifyRemoveAllDlg_RemoveProgram##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>ReadyToRemove</td><td>RemoveNow</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>8388611</td><td/><td>##IDS__IsVerifyRemoveAllDlg_Remove##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteError</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>1</td><td/><td>##IDS_BACK##</td><td>Finish</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteError</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>1</td><td/><td>##IDS_CANCEL##</td><td>Back</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteError</td><td>CheckShowMsiLog</td><td>CheckBox</td><td>151</td><td>172</td><td>10</td><td>9</td><td>2</td><td>ISSHOWMSILOG</td><td/><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteError</td><td>DlgLine</td><td>Line</td><td>0</td><td>234</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteError</td><td>Finish</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsFatalError_Finish##</td><td>Image</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteError</td><td>FinishText1</td><td>Text</td><td>135</td><td>80</td><td>228</td><td>50</td><td>65539</td><td/><td>##IDS__IsFatalError_NotModified##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteError</td><td>FinishText2</td><td>Text</td><td>135</td><td>135</td><td>228</td><td>25</td><td>65539</td><td/><td>##IDS__IsFatalError_ClickFinish##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteError</td><td>Image</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>234</td><td>1</td><td/><td/><td>CheckShowMsiLog</td><td/><td>0</td><td/><td/><td>NewBinary5</td></row>
		<row><td>SetupCompleteError</td><td>RestContText1</td><td>Text</td><td>135</td><td>80</td><td>228</td><td>50</td><td>65539</td><td/><td>##IDS__IsFatalError_KeepOrRestore##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteError</td><td>RestContText2</td><td>Text</td><td>135</td><td>135</td><td>228</td><td>25</td><td>65539</td><td/><td>##IDS__IsFatalError_RestoreOrContinueLater##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteError</td><td>ShowMsiLogText</td><td>Text</td><td>164</td><td>172</td><td>198</td><td>10</td><td>65538</td><td/><td>##IDS__IsSetupComplete_ShowMsiLog##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteError</td><td>TextLine1</td><td>Text</td><td>135</td><td>8</td><td>225</td><td>45</td><td>65539</td><td/><td>##IDS__IsFatalError_WizardCompleted##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteError</td><td>TextLine2</td><td>Text</td><td>135</td><td>55</td><td>228</td><td>25</td><td>196611</td><td/><td>##IDS__IsFatalError_WizardInterrupted##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteSuccess</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>1</td><td/><td>##IDS_BACK##</td><td>OK</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteSuccess</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>1</td><td/><td>##IDS_CANCEL##</td><td>Image</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteSuccess</td><td>CheckBoxUpdates</td><td>CheckBox</td><td>135</td><td>164</td><td>10</td><td>9</td><td>2</td><td>ISCHECKFORPRODUCTUPDATES</td><td>CheckBox1</td><td>CheckShowMsiLog</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteSuccess</td><td>CheckForUpdatesText</td><td>Text</td><td>152</td><td>162</td><td>190</td><td>30</td><td>65538</td><td/><td>##IDS__IsExitDialog_Update_YesCheckForUpdates##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteSuccess</td><td>CheckLaunchProgram</td><td>CheckBox</td><td>151</td><td>114</td><td>10</td><td>9</td><td>2</td><td>LAUNCHPROGRAM</td><td/><td>CheckLaunchReadme</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteSuccess</td><td>CheckLaunchReadme</td><td>CheckBox</td><td>151</td><td>148</td><td>10</td><td>9</td><td>2</td><td>LAUNCHREADME</td><td/><td>CheckBoxUpdates</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteSuccess</td><td>CheckShowMsiLog</td><td>CheckBox</td><td>151</td><td>182</td><td>10</td><td>9</td><td>2</td><td>ISSHOWMSILOG</td><td/><td>Back</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteSuccess</td><td>DlgLine</td><td>Line</td><td>0</td><td>234</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteSuccess</td><td>Image</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>234</td><td>1</td><td/><td/><td>CheckLaunchProgram</td><td/><td>0</td><td/><td/><td>NewBinary22</td></row>
		<row><td>SetupCompleteSuccess</td><td>LaunchProgramText</td><td>Text</td><td>164</td><td>112</td><td>98</td><td>15</td><td>65538</td><td/><td>##IDS__IsExitDialog_LaunchProgram##</td><td/><td/><td>0</td><td>0</td><td/><td/></row>
		<row><td>SetupCompleteSuccess</td><td>LaunchReadmeText</td><td>Text</td><td>164</td><td>148</td><td>120</td><td>13</td><td>65538</td><td/><td>##IDS__IsExitDialog_ShowReadMe##</td><td/><td/><td>0</td><td>0</td><td/><td/></row>
		<row><td>SetupCompleteSuccess</td><td>OK</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsExitDialog_Finish##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteSuccess</td><td>ShowMsiLogText</td><td>Text</td><td>164</td><td>182</td><td>198</td><td>10</td><td>65538</td><td/><td>##IDS__IsSetupComplete_ShowMsiLog##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteSuccess</td><td>TextLine1</td><td>Text</td><td>135</td><td>8</td><td>225</td><td>45</td><td>65539</td><td/><td>##IDS__IsExitDialog_WizardCompleted##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteSuccess</td><td>TextLine2</td><td>Text</td><td>135</td><td>55</td><td>228</td><td>45</td><td>196610</td><td/><td>##IDS__IsExitDialog_InstallSuccess##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteSuccess</td><td>TextLine3</td><td>Text</td><td>135</td><td>55</td><td>228</td><td>45</td><td>196610</td><td/><td>##IDS__IsExitDialog_UninstallSuccess##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteSuccess</td><td>UpdateTextLine1</td><td>Text</td><td>135</td><td>30</td><td>228</td><td>45</td><td>196610</td><td/><td>##IDS__IsExitDialog_Update_SetupFinished##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteSuccess</td><td>UpdateTextLine2</td><td>Text</td><td>135</td><td>80</td><td>228</td><td>45</td><td>196610</td><td/><td>##IDS__IsExitDialog_Update_PossibleUpdates##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupCompleteSuccess</td><td>UpdateTextLine3</td><td>Text</td><td>135</td><td>120</td><td>228</td><td>45</td><td>65538</td><td/><td>##IDS__IsExitDialog_Update_InternetConnection##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupError</td><td>A</td><td>PushButton</td><td>192</td><td>80</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsErrorDlg_Abort##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupError</td><td>C</td><td>PushButton</td><td>192</td><td>80</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL2##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupError</td><td>ErrorIcon</td><td>Icon</td><td>15</td><td>15</td><td>24</td><td>24</td><td>5242881</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary4</td></row>
		<row><td>SetupError</td><td>ErrorText</td><td>Text</td><td>50</td><td>15</td><td>200</td><td>50</td><td>131075</td><td/><td>##IDS__IsErrorDlg_ErrorText##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupError</td><td>I</td><td>PushButton</td><td>192</td><td>80</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsErrorDlg_Ignore##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupError</td><td>N</td><td>PushButton</td><td>192</td><td>80</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsErrorDlg_NO##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupError</td><td>O</td><td>PushButton</td><td>192</td><td>80</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsErrorDlg_OK##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupError</td><td>R</td><td>PushButton</td><td>192</td><td>80</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsErrorDlg_Retry##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupError</td><td>Y</td><td>PushButton</td><td>192</td><td>80</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsErrorDlg_Yes##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupInitialization</td><td>ActionData</td><td>Text</td><td>135</td><td>125</td><td>228</td><td>12</td><td>65539</td><td/><td>##IDS__IsInitDlg_1##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupInitialization</td><td>ActionText</td><td>Text</td><td>135</td><td>109</td><td>220</td><td>36</td><td>65539</td><td/><td>##IDS__IsInitDlg_2##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupInitialization</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>1</td><td/><td>##IDS_BACK##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupInitialization</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupInitialization</td><td>DlgLine</td><td>Line</td><td>0</td><td>234</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupInitialization</td><td>Image</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>234</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary5</td></row>
		<row><td>SetupInitialization</td><td>Next</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>1</td><td/><td>##IDS_NEXT##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupInitialization</td><td>TextLine1</td><td>Text</td><td>135</td><td>8</td><td>225</td><td>45</td><td>196611</td><td/><td>##IDS__IsInitDlg_WelcomeWizard##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupInitialization</td><td>TextLine2</td><td>Text</td><td>135</td><td>55</td><td>228</td><td>30</td><td>196611</td><td/><td>##IDS__IsInitDlg_PreparingWizard##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupInterrupted</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>1</td><td/><td>##IDS_BACK##</td><td>Finish</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupInterrupted</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>1</td><td/><td>##IDS_CANCEL##</td><td>Image</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupInterrupted</td><td>CheckShowMsiLog</td><td>CheckBox</td><td>151</td><td>172</td><td>10</td><td>9</td><td>2</td><td>ISSHOWMSILOG</td><td/><td>Back</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupInterrupted</td><td>DlgLine</td><td>Line</td><td>0</td><td>234</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupInterrupted</td><td>Finish</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS__IsUserExit_Finish##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupInterrupted</td><td>FinishText1</td><td>Text</td><td>135</td><td>80</td><td>228</td><td>50</td><td>65539</td><td/><td>##IDS__IsUserExit_NotModified##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupInterrupted</td><td>FinishText2</td><td>Text</td><td>135</td><td>135</td><td>228</td><td>25</td><td>65539</td><td/><td>##IDS__IsUserExit_ClickFinish##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupInterrupted</td><td>Image</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>234</td><td>1</td><td/><td/><td>CheckShowMsiLog</td><td/><td>0</td><td/><td/><td>NewBinary5</td></row>
		<row><td>SetupInterrupted</td><td>RestContText1</td><td>Text</td><td>135</td><td>80</td><td>228</td><td>50</td><td>65539</td><td/><td>##IDS__IsUserExit_KeepOrRestore##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupInterrupted</td><td>RestContText2</td><td>Text</td><td>135</td><td>135</td><td>228</td><td>25</td><td>65539</td><td/><td>##IDS__IsUserExit_RestoreOrContinue##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupInterrupted</td><td>ShowMsiLogText</td><td>Text</td><td>164</td><td>172</td><td>198</td><td>10</td><td>65538</td><td/><td>##IDS__IsSetupComplete_ShowMsiLog##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupInterrupted</td><td>TextLine1</td><td>Text</td><td>135</td><td>8</td><td>225</td><td>45</td><td>65539</td><td/><td>##IDS__IsUserExit_WizardCompleted##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupInterrupted</td><td>TextLine2</td><td>Text</td><td>135</td><td>55</td><td>228</td><td>25</td><td>196611</td><td/><td>##IDS__IsUserExit_WizardInterrupted##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupProgress</td><td>ActionProgress95</td><td>ProgressBar</td><td>59</td><td>113</td><td>275</td><td>12</td><td>65537</td><td/><td>##IDS__IsProgressDlg_ProgressDone##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupProgress</td><td>ActionText</td><td>Text</td><td>59</td><td>100</td><td>275</td><td>12</td><td>3</td><td/><td>##IDS__IsProgressDlg_2##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupProgress</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>1</td><td/><td>##IDS_BACK##</td><td>Next</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupProgress</td><td>Banner</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>44</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary21</td></row>
		<row><td>SetupProgress</td><td>BannerLine</td><td>Line</td><td>0</td><td>44</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupProgress</td><td>Branding1</td><td>Text</td><td>4</td><td>229</td><td>50</td><td>13</td><td>3</td><td/><td>##IDS_INSTALLSHIELD_FORMATTED##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupProgress</td><td>Branding2</td><td>Text</td><td>3</td><td>228</td><td>50</td><td>13</td><td>65537</td><td/><td>##IDS_INSTALLSHIELD##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupProgress</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td>Back</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupProgress</td><td>DlgDesc</td><td>Text</td><td>21</td><td>23</td><td>292</td><td>25</td><td>65538</td><td/><td>##IDS__IsProgressDlg_UninstallingFeatures2##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupProgress</td><td>DlgDesc2</td><td>Text</td><td>21</td><td>23</td><td>292</td><td>25</td><td>65538</td><td/><td>##IDS__IsProgressDlg_UninstallingFeatures##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupProgress</td><td>DlgLine</td><td>Line</td><td>48</td><td>234</td><td>326</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupProgress</td><td>DlgText</td><td>Text</td><td>59</td><td>51</td><td>275</td><td>30</td><td>196610</td><td/><td>##IDS__IsProgressDlg_WaitUninstall2##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupProgress</td><td>DlgText2</td><td>Text</td><td>59</td><td>51</td><td>275</td><td>30</td><td>196610</td><td/><td>##IDS__IsProgressDlg_WaitUninstall##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupProgress</td><td>DlgTitle</td><td>Text</td><td>13</td><td>6</td><td>292</td><td>25</td><td>196610</td><td/><td>##IDS__IsProgressDlg_InstallingProductName##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupProgress</td><td>DlgTitle2</td><td>Text</td><td>13</td><td>6</td><td>292</td><td>25</td><td>196610</td><td/><td>##IDS__IsProgressDlg_Uninstalling##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupProgress</td><td>LbSec</td><td>Text</td><td>192</td><td>139</td><td>32</td><td>12</td><td>2</td><td/><td>##IDS__IsProgressDlg_SecHidden##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupProgress</td><td>LbStatus</td><td>Text</td><td>59</td><td>85</td><td>70</td><td>12</td><td>3</td><td/><td>##IDS__IsProgressDlg_Status##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupProgress</td><td>Next</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>1</td><td/><td>##IDS_NEXT##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupProgress</td><td>SetupIcon</td><td>Icon</td><td>21</td><td>51</td><td>24</td><td>24</td><td>5242881</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary9</td></row>
		<row><td>SetupProgress</td><td>ShowTime</td><td>Text</td><td>170</td><td>139</td><td>17</td><td>12</td><td>2</td><td/><td>##IDS__IsProgressDlg_Hidden##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupProgress</td><td>TextTime</td><td>Text</td><td>59</td><td>139</td><td>110</td><td>12</td><td>2</td><td/><td>##IDS__IsProgressDlg_HiddenTimeRemaining##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupResume</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>1</td><td/><td>##IDS_BACK##</td><td>Next</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupResume</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td>Back</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupResume</td><td>DlgLine</td><td>Line</td><td>0</td><td>234</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupResume</td><td>Image</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>234</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary5</td></row>
		<row><td>SetupResume</td><td>Next</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_NEXT##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupResume</td><td>PreselectedText</td><td>Text</td><td>135</td><td>55</td><td>228</td><td>45</td><td>196611</td><td/><td>##IDS__IsResumeDlg_WizardResume##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupResume</td><td>ResumeText</td><td>Text</td><td>135</td><td>46</td><td>228</td><td>45</td><td>196611</td><td/><td>##IDS__IsResumeDlg_ResumeSuspended##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupResume</td><td>TextLine1</td><td>Text</td><td>135</td><td>8</td><td>225</td><td>45</td><td>196611</td><td/><td>##IDS__IsResumeDlg_Resuming##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupType</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_BACK##</td><td>Next</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupType</td><td>Banner</td><td>Bitmap</td><td>0</td><td>0</td><td>374</td><td>44</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary1</td></row>
		<row><td>SetupType</td><td>BannerLine</td><td>Line</td><td>0</td><td>44</td><td>374</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupType</td><td>Branding1</td><td>Text</td><td>4</td><td>229</td><td>50</td><td>13</td><td>3</td><td/><td>##IDS_INSTALLSHIELD_FORMATTED##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupType</td><td>Branding2</td><td>Text</td><td>3</td><td>228</td><td>50</td><td>13</td><td>65537</td><td/><td>##IDS_INSTALLSHIELD##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupType</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td>RadioGroup</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupType</td><td>CompText</td><td>Text</td><td>80</td><td>80</td><td>246</td><td>30</td><td>3</td><td/><td>##IDS__IsSetupTypeMinDlg_AllFeatures##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupType</td><td>CompleteIco</td><td>Icon</td><td>34</td><td>80</td><td>24</td><td>24</td><td>5242881</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary10</td></row>
		<row><td>SetupType</td><td>CustText</td><td>Text</td><td>80</td><td>171</td><td>246</td><td>30</td><td>2</td><td/><td>##IDS__IsSetupTypeMinDlg_ChooseFeatures##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupType</td><td>CustomIco</td><td>Icon</td><td>34</td><td>171</td><td>24</td><td>24</td><td>5242880</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary11</td></row>
		<row><td>SetupType</td><td>DlgDesc</td><td>Text</td><td>21</td><td>23</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsSetupTypeMinDlg_ChooseSetupType##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupType</td><td>DlgLine</td><td>Line</td><td>48</td><td>234</td><td>326</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupType</td><td>DlgText</td><td>Text</td><td>22</td><td>49</td><td>326</td><td>10</td><td>3</td><td/><td>##IDS__IsSetupTypeMinDlg_SelectSetupType##</td><td/><td/><td>0</td><td>0</td><td/><td/></row>
		<row><td>SetupType</td><td>DlgTitle</td><td>Text</td><td>13</td><td>6</td><td>292</td><td>25</td><td>65539</td><td/><td>##IDS__IsSetupTypeMinDlg_SetupType##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupType</td><td>MinIco</td><td>Icon</td><td>34</td><td>125</td><td>24</td><td>24</td><td>5242880</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary11</td></row>
		<row><td>SetupType</td><td>MinText</td><td>Text</td><td>80</td><td>125</td><td>246</td><td>30</td><td>2</td><td/><td>##IDS__IsSetupTypeMinDlg_MinimumFeatures##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupType</td><td>Next</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_NEXT##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SetupType</td><td>RadioGroup</td><td>RadioButtonGroup</td><td>20</td><td>59</td><td>264</td><td>139</td><td>1048579</td><td>_IsSetupTypeMin</td><td/><td>Back</td><td/><td>0</td><td>0</td><td/><td/></row>
		<row><td>SplashBitmap</td><td>Back</td><td>PushButton</td><td>164</td><td>243</td><td>66</td><td>17</td><td>1</td><td/><td>##IDS_BACK##</td><td>Next</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SplashBitmap</td><td>Branding1</td><td>Text</td><td>4</td><td>229</td><td>50</td><td>13</td><td>3</td><td/><td>##IDS_INSTALLSHIELD_FORMATTED##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SplashBitmap</td><td>Branding2</td><td>Text</td><td>3</td><td>228</td><td>50</td><td>13</td><td>65537</td><td/><td>##IDS_INSTALLSHIELD##</td><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SplashBitmap</td><td>Cancel</td><td>PushButton</td><td>301</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_CANCEL##</td><td>Back</td><td/><td>0</td><td/><td/><td/></row>
		<row><td>SplashBitmap</td><td>DlgLine</td><td>Line</td><td>48</td><td>234</td><td>326</td><td>0</td><td>1</td><td/><td/><td/><td/><td>0</td><td/><td/><td/></row>
		<row><td>SplashBitmap</td><td>Image</td><td>Bitmap</td><td>13</td><td>12</td><td>349</td><td>211</td><td>5</td><td/><td/><td/><td/><td>0</td><td/><td/><td>NewBinary19</td></row>
		<row><td>SplashBitmap</td><td>Next</td><td>PushButton</td><td>230</td><td>243</td><td>66</td><td>17</td><td>3</td><td/><td>##IDS_NEXT##</td><td>Cancel</td><td/><td>0</td><td/><td/><td/></row>
	</table>

	<table name="ControlCondition">
		<col key="yes" def="s72">Dialog_</col>
		<col key="yes" def="s50">Control_</col>
		<col key="yes" def="s50">Action</col>
		<col key="yes" def="s255">Condition</col>
		<row><td>CustomSetup</td><td>ChangeFolder</td><td>Hide</td><td>Installed</td></row>
		<row><td>CustomSetup</td><td>Details</td><td>Hide</td><td>Installed</td></row>
		<row><td>CustomSetup</td><td>InstallLabel</td><td>Hide</td><td>Installed</td></row>
		<row><td>CustomerInformation</td><td>DlgRadioGroupText</td><td>Hide</td><td>NOT Privileged</td></row>
		<row><td>CustomerInformation</td><td>DlgRadioGroupText</td><td>Hide</td><td>ProductState &gt; 0</td></row>
		<row><td>CustomerInformation</td><td>DlgRadioGroupText</td><td>Hide</td><td>Version9X</td></row>
		<row><td>CustomerInformation</td><td>DlgRadioGroupText</td><td>Hide</td><td>VersionNT &gt;= "601" AND ISSupportPerUser AND NOT Installed</td></row>
		<row><td>CustomerInformation</td><td>RadioGroup</td><td>Hide</td><td>NOT Privileged</td></row>
		<row><td>CustomerInformation</td><td>RadioGroup</td><td>Hide</td><td>ProductState &gt; 0</td></row>
		<row><td>CustomerInformation</td><td>RadioGroup</td><td>Hide</td><td>Version9X</td></row>
		<row><td>CustomerInformation</td><td>RadioGroup</td><td>Hide</td><td>VersionNT &gt;= "601" AND ISSupportPerUser AND NOT Installed</td></row>
		<row><td>CustomerInformation</td><td>SerialLabel</td><td>Show</td><td>SERIALNUMSHOW</td></row>
		<row><td>CustomerInformation</td><td>SerialNumber</td><td>Show</td><td>SERIALNUMSHOW</td></row>
		<row><td>InstallWelcome</td><td>Copyright</td><td>Hide</td><td>SHOWCOPYRIGHT="No"</td></row>
		<row><td>InstallWelcome</td><td>Copyright</td><td>Show</td><td>SHOWCOPYRIGHT="Yes"</td></row>
		<row><td>LicenseAgreement</td><td>Next</td><td>Disable</td><td>AgreeToLicense &lt;&gt; "Yes"</td></row>
		<row><td>LicenseAgreement</td><td>Next</td><td>Enable</td><td>AgreeToLicense = "Yes"</td></row>
		<row><td>ReadyToInstall</td><td>CompanyNameText</td><td>Hide</td><td>VersionNT &gt;= "601" AND ISSupportPerUser AND NOT Installed</td></row>
		<row><td>ReadyToInstall</td><td>CurrentSettingsText</td><td>Hide</td><td>VersionNT &gt;= "601" AND ISSupportPerUser AND NOT Installed</td></row>
		<row><td>ReadyToInstall</td><td>DlgText2</td><td>Hide</td><td>VersionNT &lt; "601" OR NOT ISSupportPerUser OR Installed</td></row>
		<row><td>ReadyToInstall</td><td>DlgText2</td><td>Show</td><td>VersionNT &gt;= "601" AND ISSupportPerUser AND NOT Installed</td></row>
		<row><td>ReadyToInstall</td><td>DlgTitle</td><td>Show</td><td>ProgressType0="Modify"</td></row>
		<row><td>ReadyToInstall</td><td>DlgTitle2</td><td>Show</td><td>ProgressType0="Repair"</td></row>
		<row><td>ReadyToInstall</td><td>DlgTitle3</td><td>Show</td><td>ProgressType0="install"</td></row>
		<row><td>ReadyToInstall</td><td>GroupBox1</td><td>Hide</td><td>VersionNT &gt;= "601" AND ISSupportPerUser AND NOT Installed</td></row>
		<row><td>ReadyToInstall</td><td>InstallNow</td><td>Disable</td><td>VersionNT &gt;= "601" AND ISSupportPerUser AND NOT Installed</td></row>
		<row><td>ReadyToInstall</td><td>InstallNow</td><td>Enable</td><td>VersionNT &lt; "601" OR NOT ISSupportPerUser OR Installed</td></row>
		<row><td>ReadyToInstall</td><td>InstallPerMachine</td><td>Hide</td><td>VersionNT &lt; "601" OR NOT ISSupportPerUser OR Installed</td></row>
		<row><td>ReadyToInstall</td><td>InstallPerMachine</td><td>Show</td><td>VersionNT &gt;= "601" AND ISSupportPerUser AND NOT Installed</td></row>
		<row><td>ReadyToInstall</td><td>InstallPerUser</td><td>Hide</td><td>VersionNT &lt; "601" OR NOT ISSupportPerUser OR Installed</td></row>
		<row><td>ReadyToInstall</td><td>InstallPerUser</td><td>Show</td><td>VersionNT &gt;= "601" AND ISSupportPerUser AND NOT Installed</td></row>
		<row><td>ReadyToInstall</td><td>SerialNumberText</td><td>Hide</td><td>NOT SERIALNUMSHOW</td></row>
		<row><td>ReadyToInstall</td><td>SerialNumberText</td><td>Hide</td><td>VersionNT &gt;= "601" AND ISSupportPerUser AND NOT Installed</td></row>
		<row><td>ReadyToInstall</td><td>SetupTypeText1</td><td>Hide</td><td>VersionNT &gt;= "601" AND ISSupportPerUser AND NOT Installed</td></row>
		<row><td>ReadyToInstall</td><td>SetupTypeText2</td><td>Hide</td><td>VersionNT &gt;= "601" AND ISSupportPerUser AND NOT Installed</td></row>
		<row><td>ReadyToInstall</td><td>TargetFolderText1</td><td>Hide</td><td>VersionNT &gt;= "601" AND ISSupportPerUser AND NOT Installed</td></row>
		<row><td>ReadyToInstall</td><td>TargetFolderText2</td><td>Hide</td><td>VersionNT &gt;= "601" AND ISSupportPerUser AND NOT Installed</td></row>
		<row><td>ReadyToInstall</td><td>UserInformationText</td><td>Hide</td><td>VersionNT &gt;= "601" AND ISSupportPerUser AND NOT Installed</td></row>
		<row><td>ReadyToInstall</td><td>UserNameText</td><td>Hide</td><td>VersionNT &gt;= "601" AND ISSupportPerUser AND NOT Installed</td></row>
		<row><td>SetupCompleteError</td><td>Back</td><td>Default</td><td>UpdateStarted</td></row>
		<row><td>SetupCompleteError</td><td>Back</td><td>Disable</td><td>NOT UpdateStarted</td></row>
		<row><td>SetupCompleteError</td><td>Back</td><td>Enable</td><td>UpdateStarted</td></row>
		<row><td>SetupCompleteError</td><td>Cancel</td><td>Disable</td><td>NOT UpdateStarted</td></row>
		<row><td>SetupCompleteError</td><td>Cancel</td><td>Enable</td><td>UpdateStarted</td></row>
		<row><td>SetupCompleteError</td><td>CheckShowMsiLog</td><td>Show</td><td>MsiLogFileLocation</td></row>
		<row><td>SetupCompleteError</td><td>Finish</td><td>Default</td><td>NOT UpdateStarted</td></row>
		<row><td>SetupCompleteError</td><td>FinishText1</td><td>Hide</td><td>UpdateStarted</td></row>
		<row><td>SetupCompleteError</td><td>FinishText1</td><td>Show</td><td>NOT UpdateStarted</td></row>
		<row><td>SetupCompleteError</td><td>FinishText2</td><td>Hide</td><td>UpdateStarted</td></row>
		<row><td>SetupCompleteError</td><td>FinishText2</td><td>Show</td><td>NOT UpdateStarted</td></row>
		<row><td>SetupCompleteError</td><td>RestContText1</td><td>Hide</td><td>NOT UpdateStarted</td></row>
		<row><td>SetupCompleteError</td><td>RestContText1</td><td>Show</td><td>UpdateStarted</td></row>
		<row><td>SetupCompleteError</td><td>RestContText2</td><td>Hide</td><td>NOT UpdateStarted</td></row>
		<row><td>SetupCompleteError</td><td>RestContText2</td><td>Show</td><td>UpdateStarted</td></row>
		<row><td>SetupCompleteError</td><td>ShowMsiLogText</td><td>Show</td><td>MsiLogFileLocation</td></row>
		<row><td>SetupCompleteSuccess</td><td>CheckBoxUpdates</td><td>Show</td><td>ISENABLEDWUSFINISHDIALOG And NOT Installed And ACTION="INSTALL"</td></row>
		<row><td>SetupCompleteSuccess</td><td>CheckForUpdatesText</td><td>Show</td><td>ISENABLEDWUSFINISHDIALOG And NOT Installed And ACTION="INSTALL"</td></row>
		<row><td>SetupCompleteSuccess</td><td>CheckLaunchProgram</td><td>Show</td><td>SHOWLAUNCHPROGRAM="-1" And PROGRAMFILETOLAUNCHATEND &lt;&gt; "" And NOT Installed And NOT ISENABLEDWUSFINISHDIALOG</td></row>
		<row><td>SetupCompleteSuccess</td><td>CheckLaunchReadme</td><td>Show</td><td>SHOWLAUNCHREADME="-1"  And READMEFILETOLAUNCHATEND &lt;&gt; "" And NOT Installed And NOT ISENABLEDWUSFINISHDIALOG</td></row>
		<row><td>SetupCompleteSuccess</td><td>CheckShowMsiLog</td><td>Show</td><td>MsiLogFileLocation And NOT ISENABLEDWUSFINISHDIALOG</td></row>
		<row><td>SetupCompleteSuccess</td><td>LaunchProgramText</td><td>Show</td><td>SHOWLAUNCHPROGRAM="-1" And PROGRAMFILETOLAUNCHATEND &lt;&gt; "" And NOT Installed And NOT ISENABLEDWUSFINISHDIALOG</td></row>
		<row><td>SetupCompleteSuccess</td><td>LaunchReadmeText</td><td>Show</td><td>SHOWLAUNCHREADME="-1"  And READMEFILETOLAUNCHATEND &lt;&gt; "" And NOT Installed And NOT ISENABLEDWUSFINISHDIALOG</td></row>
		<row><td>SetupCompleteSuccess</td><td>ShowMsiLogText</td><td>Show</td><td>MsiLogFileLocation And NOT ISENABLEDWUSFINISHDIALOG</td></row>
		<row><td>SetupCompleteSuccess</td><td>TextLine2</td><td>Show</td><td>ProgressType2="installed" And ((ACTION&lt;&gt;"INSTALL") OR (NOT ISENABLEDWUSFINISHDIALOG) OR (ISENABLEDWUSFINISHDIALOG And Installed))</td></row>
		<row><td>SetupCompleteSuccess</td><td>TextLine3</td><td>Show</td><td>ProgressType2="uninstalled" And ((ACTION&lt;&gt;"INSTALL") OR (NOT ISENABLEDWUSFINISHDIALOG) OR (ISENABLEDWUSFINISHDIALOG And Installed))</td></row>
		<row><td>SetupCompleteSuccess</td><td>UpdateTextLine1</td><td>Show</td><td>ISENABLEDWUSFINISHDIALOG And NOT Installed And ACTION="INSTALL"</td></row>
		<row><td>SetupCompleteSuccess</td><td>UpdateTextLine2</td><td>Show</td><td>ISENABLEDWUSFINISHDIALOG And NOT Installed And ACTION="INSTALL"</td></row>
		<row><td>SetupCompleteSuccess</td><td>UpdateTextLine3</td><td>Show</td><td>ISENABLEDWUSFINISHDIALOG And NOT Installed And ACTION="INSTALL"</td></row>
		<row><td>SetupInterrupted</td><td>Back</td><td>Default</td><td>UpdateStarted</td></row>
		<row><td>SetupInterrupted</td><td>Back</td><td>Disable</td><td>NOT UpdateStarted</td></row>
		<row><td>SetupInterrupted</td><td>Back</td><td>Enable</td><td>UpdateStarted</td></row>
		<row><td>SetupInterrupted</td><td>Cancel</td><td>Disable</td><td>NOT UpdateStarted</td></row>
		<row><td>SetupInterrupted</td><td>Cancel</td><td>Enable</td><td>UpdateStarted</td></row>
		<row><td>SetupInterrupted</td><td>CheckShowMsiLog</td><td>Show</td><td>MsiLogFileLocation</td></row>
		<row><td>SetupInterrupted</td><td>Finish</td><td>Default</td><td>NOT UpdateStarted</td></row>
		<row><td>SetupInterrupted</td><td>FinishText1</td><td>Hide</td><td>UpdateStarted</td></row>
		<row><td>SetupInterrupted</td><td>FinishText1</td><td>Show</td><td>NOT UpdateStarted</td></row>
		<row><td>SetupInterrupted</td><td>FinishText2</td><td>Hide</td><td>UpdateStarted</td></row>
		<row><td>SetupInterrupted</td><td>FinishText2</td><td>Show</td><td>NOT UpdateStarted</td></row>
		<row><td>SetupInterrupted</td><td>RestContText1</td><td>Hide</td><td>NOT UpdateStarted</td></row>
		<row><td>SetupInterrupted</td><td>RestContText1</td><td>Show</td><td>UpdateStarted</td></row>
		<row><td>SetupInterrupted</td><td>RestContText2</td><td>Hide</td><td>NOT UpdateStarted</td></row>
		<row><td>SetupInterrupted</td><td>RestContText2</td><td>Show</td><td>UpdateStarted</td></row>
		<row><td>SetupInterrupted</td><td>ShowMsiLogText</td><td>Show</td><td>MsiLogFileLocation</td></row>
		<row><td>SetupProgress</td><td>DlgDesc</td><td>Show</td><td>ProgressType2="installed"</td></row>
		<row><td>SetupProgress</td><td>DlgDesc2</td><td>Show</td><td>ProgressType2="uninstalled"</td></row>
		<row><td>SetupProgress</td><td>DlgText</td><td>Show</td><td>ProgressType3="installs"</td></row>
		<row><td>SetupProgress</td><td>DlgText2</td><td>Show</td><td>ProgressType3="uninstalls"</td></row>
		<row><td>SetupProgress</td><td>DlgTitle</td><td>Show</td><td>ProgressType1="Installing"</td></row>
		<row><td>SetupProgress</td><td>DlgTitle2</td><td>Show</td><td>ProgressType1="Uninstalling"</td></row>
		<row><td>SetupResume</td><td>PreselectedText</td><td>Hide</td><td>RESUME</td></row>
		<row><td>SetupResume</td><td>PreselectedText</td><td>Show</td><td>NOT RESUME</td></row>
		<row><td>SetupResume</td><td>ResumeText</td><td>Hide</td><td>NOT RESUME</td></row>
		<row><td>SetupResume</td><td>ResumeText</td><td>Show</td><td>RESUME</td></row>
	</table>

	<table name="ControlEvent">
		<col key="yes" def="s72">Dialog_</col>
		<col key="yes" def="s50">Control_</col>
		<col key="yes" def="s50">Event</col>
		<col key="yes" def="s255">Argument</col>
		<col key="yes" def="S255">Condition</col>
		<col def="I2">Ordering</col>
		<row><td>AdminChangeFolder</td><td>Cancel</td><td>EndDialog</td><td>Return</td><td>1</td><td>2</td></row>
		<row><td>AdminChangeFolder</td><td>Cancel</td><td>Reset</td><td>0</td><td>1</td><td>1</td></row>
		<row><td>AdminChangeFolder</td><td>NewFolder</td><td>DirectoryListNew</td><td>0</td><td>1</td><td>0</td></row>
		<row><td>AdminChangeFolder</td><td>OK</td><td>EndDialog</td><td>Return</td><td>1</td><td>0</td></row>
		<row><td>AdminChangeFolder</td><td>OK</td><td>SetTargetPath</td><td>TARGETDIR</td><td>1</td><td>1</td></row>
		<row><td>AdminChangeFolder</td><td>Up</td><td>DirectoryListUp</td><td>0</td><td>1</td><td>0</td></row>
		<row><td>AdminNetworkLocation</td><td>Back</td><td>NewDialog</td><td>AdminWelcome</td><td>1</td><td>0</td></row>
		<row><td>AdminNetworkLocation</td><td>Browse</td><td>SpawnDialog</td><td>AdminChangeFolder</td><td>1</td><td>0</td></row>
		<row><td>AdminNetworkLocation</td><td>Cancel</td><td>SpawnDialog</td><td>CancelSetup</td><td>1</td><td>0</td></row>
		<row><td>AdminNetworkLocation</td><td>InstallNow</td><td>EndDialog</td><td>Return</td><td>OutOfNoRbDiskSpace &lt;&gt; 1</td><td>3</td></row>
		<row><td>AdminNetworkLocation</td><td>InstallNow</td><td>NewDialog</td><td>OutOfSpace</td><td>OutOfNoRbDiskSpace = 1</td><td>2</td></row>
		<row><td>AdminNetworkLocation</td><td>InstallNow</td><td>SetTargetPath</td><td>TARGETDIR</td><td>1</td><td>1</td></row>
		<row><td>AdminWelcome</td><td>Cancel</td><td>SpawnDialog</td><td>CancelSetup</td><td>1</td><td>0</td></row>
		<row><td>AdminWelcome</td><td>Next</td><td>NewDialog</td><td>AdminNetworkLocation</td><td>1</td><td>0</td></row>
		<row><td>CancelSetup</td><td>No</td><td>EndDialog</td><td>Return</td><td>1</td><td>0</td></row>
		<row><td>CancelSetup</td><td>Yes</td><td>DoAction</td><td>CleanUp</td><td>ISSCRIPTRUNNING="1"</td><td>1</td></row>
		<row><td>CancelSetup</td><td>Yes</td><td>EndDialog</td><td>Exit</td><td>1</td><td>2</td></row>
		<row><td>CustomSetup</td><td>Back</td><td>NewDialog</td><td>CustomerInformation</td><td>NOT Installed</td><td>0</td></row>
		<row><td>CustomSetup</td><td>Back</td><td>NewDialog</td><td>MaintenanceType</td><td>Installed</td><td>0</td></row>
		<row><td>CustomSetup</td><td>Cancel</td><td>SpawnDialog</td><td>CancelSetup</td><td>1</td><td>0</td></row>
		<row><td>CustomSetup</td><td>ChangeFolder</td><td>SelectionBrowse</td><td>InstallChangeFolder</td><td>1</td><td>0</td></row>
		<row><td>CustomSetup</td><td>Details</td><td>SelectionBrowse</td><td>DiskSpaceRequirements</td><td>1</td><td>1</td></row>
		<row><td>CustomSetup</td><td>Help</td><td>SpawnDialog</td><td>CustomSetupTips</td><td>1</td><td>1</td></row>
		<row><td>CustomSetup</td><td>Next</td><td>NewDialog</td><td>OutOfSpace</td><td>OutOfNoRbDiskSpace = 1</td><td>0</td></row>
		<row><td>CustomSetup</td><td>Next</td><td>NewDialog</td><td>ReadyToInstall</td><td>OutOfNoRbDiskSpace &lt;&gt; 1</td><td>0</td></row>
		<row><td>CustomSetup</td><td>Next</td><td>[_IsSetupTypeMin]</td><td>Custom</td><td>1</td><td>0</td></row>
		<row><td>CustomSetupTips</td><td>OK</td><td>EndDialog</td><td>Return</td><td>1</td><td>1</td></row>
		<row><td>CustomerInformation</td><td>Back</td><td>NewDialog</td><td>LicenseAgreement</td><td>1</td><td>1</td></row>
		<row><td>CustomerInformation</td><td>Cancel</td><td>SpawnDialog</td><td>CancelSetup</td><td>1</td><td>0</td></row>
		<row><td>CustomerInformation</td><td>Next</td><td>EndDialog</td><td>Exit</td><td>(SERIALNUMVALRETRYLIMIT) And (SERIALNUMVALRETRYLIMIT&lt;0) And (SERIALNUMVALRETURN&lt;&gt;SERIALNUMVALSUCCESSRETVAL)</td><td>2</td></row>
		<row><td>CustomerInformation</td><td>Next</td><td>NewDialog</td><td>ReadyToInstall</td><td>(Not SERIALNUMVALRETURN) OR (SERIALNUMVALRETURN=SERIALNUMVALSUCCESSRETVAL)</td><td>3</td></row>
		<row><td>CustomerInformation</td><td>Next</td><td>[ALLUSERS]</td><td>1</td><td>ApplicationUsers = "AllUsers" And Privileged</td><td>1</td></row>
		<row><td>CustomerInformation</td><td>Next</td><td>[ALLUSERS]</td><td>{}</td><td>ApplicationUsers = "OnlyCurrentUser" And Privileged</td><td>2</td></row>
		<row><td>DatabaseFolder</td><td>Back</td><td>NewDialog</td><td>DestinationFolder</td><td>NOT Installed</td><td>1</td></row>
		<row><td>DatabaseFolder</td><td>Cancel</td><td>SpawnDialog</td><td>CancelSetup</td><td>1</td><td>1</td></row>
		<row><td>DatabaseFolder</td><td>ChangeFolder</td><td>SpawnDialog</td><td>InstallChangeFolder</td><td>1</td><td>1</td></row>
		<row><td>DatabaseFolder</td><td>ChangeFolder</td><td>[_BrowseProperty]</td><td>DATABASEDIR</td><td>1</td><td>2</td></row>
		<row><td>DatabaseFolder</td><td>Next</td><td>EndDialog</td><td>Return</td><td>OutOfDiskSpace &lt;&gt; 1</td><td>1</td></row>
		<row><td>DatabaseFolder</td><td>Next</td><td>NewDialog</td><td>SetupType</td><td>0</td><td>2</td></row>
		<row><td>DestinationFolder</td><td>Back</td><td>NewDialog</td><td>InstallWelcome</td><td>NOT Installed</td><td>0</td></row>
		<row><td>DestinationFolder</td><td>Cancel</td><td>SpawnDialog</td><td>CancelSetup</td><td>1</td><td>1</td></row>
		<row><td>DestinationFolder</td><td>ChangeFolder</td><td>SpawnDialog</td><td>InstallChangeFolder</td><td>1</td><td>1</td></row>
		<row><td>DestinationFolder</td><td>ChangeFolder</td><td>[_BrowseProperty]</td><td>INSTALLDIR</td><td>1</td><td>2</td></row>
		<row><td>DestinationFolder</td><td>Next</td><td>NewDialog</td><td>DatabaseFolder</td><td>1</td><td>1</td></row>
		<row><td>DiskSpaceRequirements</td><td>OK</td><td>EndDialog</td><td>Return</td><td>1</td><td>0</td></row>
		<row><td>FilesInUse</td><td>Exit</td><td>EndDialog</td><td>Exit</td><td>1</td><td>0</td></row>
		<row><td>FilesInUse</td><td>Ignore</td><td>EndDialog</td><td>Ignore</td><td>1</td><td>0</td></row>
		<row><td>FilesInUse</td><td>Retry</td><td>EndDialog</td><td>Retry</td><td>1</td><td>0</td></row>
		<row><td>InstallChangeFolder</td><td>Cancel</td><td>EndDialog</td><td>Return</td><td>1</td><td>2</td></row>
		<row><td>InstallChangeFolder</td><td>Cancel</td><td>Reset</td><td>0</td><td>1</td><td>1</td></row>
		<row><td>InstallChangeFolder</td><td>NewFolder</td><td>DirectoryListNew</td><td>0</td><td>1</td><td>0</td></row>
		<row><td>InstallChangeFolder</td><td>OK</td><td>EndDialog</td><td>Return</td><td>1</td><td>3</td></row>
		<row><td>InstallChangeFolder</td><td>OK</td><td>SetTargetPath</td><td>[_BrowseProperty]</td><td>1</td><td>2</td></row>
		<row><td>InstallChangeFolder</td><td>Up</td><td>DirectoryListUp</td><td>0</td><td>1</td><td>0</td></row>
		<row><td>InstallWelcome</td><td>Back</td><td>NewDialog</td><td>SplashBitmap</td><td>NOT Installed</td><td>0</td></row>
		<row><td>InstallWelcome</td><td>Cancel</td><td>SpawnDialog</td><td>CancelSetup</td><td>1</td><td>0</td></row>
		<row><td>InstallWelcome</td><td>Next</td><td>NewDialog</td><td>DestinationFolder</td><td>1</td><td>0</td></row>
		<row><td>LicenseAgreement</td><td>Back</td><td>NewDialog</td><td>InstallWelcome</td><td>NOT Installed</td><td>0</td></row>
		<row><td>LicenseAgreement</td><td>Cancel</td><td>SpawnDialog</td><td>CancelSetup</td><td>1</td><td>0</td></row>
		<row><td>LicenseAgreement</td><td>ISPrintButton</td><td>DoAction</td><td>ISPrint</td><td>1</td><td>0</td></row>
		<row><td>LicenseAgreement</td><td>Next</td><td>NewDialog</td><td>DestinationFolder</td><td>AgreeToLicense = "Yes"</td><td>0</td></row>
		<row><td>MaintenanceType</td><td>Back</td><td>NewDialog</td><td>MaintenanceWelcome</td><td>1</td><td>0</td></row>
		<row><td>MaintenanceType</td><td>Cancel</td><td>SpawnDialog</td><td>CancelSetup</td><td>1</td><td>0</td></row>
		<row><td>MaintenanceType</td><td>Next</td><td>NewDialog</td><td>CustomSetup</td><td>_IsMaintenance = "Change"</td><td>12</td></row>
		<row><td>MaintenanceType</td><td>Next</td><td>NewDialog</td><td>ReadyToInstall</td><td>_IsMaintenance = "Reinstall"</td><td>13</td></row>
		<row><td>MaintenanceType</td><td>Next</td><td>NewDialog</td><td>ReadyToRemove</td><td>_IsMaintenance = "Remove"</td><td>11</td></row>
		<row><td>MaintenanceType</td><td>Next</td><td>Reinstall</td><td>ALL</td><td>_IsMaintenance = "Reinstall"</td><td>10</td></row>
		<row><td>MaintenanceType</td><td>Next</td><td>ReinstallMode</td><td>[ReinstallModeText]</td><td>_IsMaintenance = "Reinstall"</td><td>9</td></row>
		<row><td>MaintenanceType</td><td>Next</td><td>[ProgressType0]</td><td>Modify</td><td>_IsMaintenance = "Change"</td><td>2</td></row>
		<row><td>MaintenanceType</td><td>Next</td><td>[ProgressType0]</td><td>Repair</td><td>_IsMaintenance = "Reinstall"</td><td>1</td></row>
		<row><td>MaintenanceType</td><td>Next</td><td>[ProgressType1]</td><td>Modifying</td><td>_IsMaintenance = "Change"</td><td>3</td></row>
		<row><td>MaintenanceType</td><td>Next</td><td>[ProgressType1]</td><td>Repairing</td><td>_IsMaintenance = "Reinstall"</td><td>4</td></row>
		<row><td>MaintenanceType</td><td>Next</td><td>[ProgressType2]</td><td>modified</td><td>_IsMaintenance = "Change"</td><td>6</td></row>
		<row><td>MaintenanceType</td><td>Next</td><td>[ProgressType2]</td><td>repairs</td><td>_IsMaintenance = "Reinstall"</td><td>5</td></row>
		<row><td>MaintenanceType</td><td>Next</td><td>[ProgressType3]</td><td>modifies</td><td>_IsMaintenance = "Change"</td><td>7</td></row>
		<row><td>MaintenanceType</td><td>Next</td><td>[ProgressType3]</td><td>repairs</td><td>_IsMaintenance = "Reinstall"</td><td>8</td></row>
		<row><td>MaintenanceWelcome</td><td>Cancel</td><td>SpawnDialog</td><td>CancelSetup</td><td>1</td><td>0</td></row>
		<row><td>MaintenanceWelcome</td><td>Next</td><td>NewDialog</td><td>MaintenanceType</td><td>1</td><td>0</td></row>
		<row><td>MsiRMFilesInUse</td><td>Cancel</td><td>EndDialog</td><td>Exit</td><td>1</td><td>1</td></row>
		<row><td>MsiRMFilesInUse</td><td>OK</td><td>EndDialog</td><td>Return</td><td>1</td><td>1</td></row>
		<row><td>MsiRMFilesInUse</td><td>OK</td><td>RMShutdownAndRestart</td><td>0</td><td>RestartManagerOption="CloseRestart"</td><td>2</td></row>
		<row><td>OutOfSpace</td><td>Resume</td><td>NewDialog</td><td>AdminNetworkLocation</td><td>ACTION = "ADMIN"</td><td>0</td></row>
		<row><td>OutOfSpace</td><td>Resume</td><td>NewDialog</td><td>DestinationFolder</td><td>ACTION &lt;&gt; "ADMIN"</td><td>0</td></row>
		<row><td>PatchWelcome</td><td>Cancel</td><td>SpawnDialog</td><td>CancelSetup</td><td>1</td><td>1</td></row>
		<row><td>PatchWelcome</td><td>Next</td><td>EndDialog</td><td>Return</td><td>1</td><td>3</td></row>
		<row><td>PatchWelcome</td><td>Next</td><td>Reinstall</td><td>ALL</td><td>PATCH And REINSTALL=""</td><td>1</td></row>
		<row><td>PatchWelcome</td><td>Next</td><td>ReinstallMode</td><td>omus</td><td>PATCH And REINSTALLMODE=""</td><td>2</td></row>
		<row><td>ReadmeInformation</td><td>Back</td><td>NewDialog</td><td>LicenseAgreement</td><td>1</td><td>1</td></row>
		<row><td>ReadmeInformation</td><td>Cancel</td><td>SpawnDialog</td><td>CancelSetup</td><td>1</td><td>1</td></row>
		<row><td>ReadmeInformation</td><td>Next</td><td>NewDialog</td><td>CustomerInformation</td><td>1</td><td>1</td></row>
		<row><td>ReadyToInstall</td><td>Back</td><td>NewDialog</td><td>CustomSetup</td><td>Installed OR _IsSetupTypeMin = "Custom"</td><td>2</td></row>
		<row><td>ReadyToInstall</td><td>Back</td><td>NewDialog</td><td>DestinationFolder</td><td>NOT Installed</td><td>1</td></row>
		<row><td>ReadyToInstall</td><td>Back</td><td>NewDialog</td><td>MaintenanceType</td><td>Installed AND _IsMaintenance = "Reinstall"</td><td>3</td></row>
		<row><td>ReadyToInstall</td><td>Cancel</td><td>SpawnDialog</td><td>CancelSetup</td><td>1</td><td>0</td></row>
		<row><td>ReadyToInstall</td><td>InstallNow</td><td>EndDialog</td><td>Return</td><td>OutOfNoRbDiskSpace &lt;&gt; 1</td><td>0</td></row>
		<row><td>ReadyToInstall</td><td>InstallNow</td><td>NewDialog</td><td>OutOfSpace</td><td>OutOfNoRbDiskSpace = 1</td><td>0</td></row>
		<row><td>ReadyToInstall</td><td>InstallNow</td><td>[ProgressType1]</td><td>Installing</td><td>1</td><td>0</td></row>
		<row><td>ReadyToInstall</td><td>InstallNow</td><td>[ProgressType2]</td><td>installed</td><td>1</td><td>0</td></row>
		<row><td>ReadyToInstall</td><td>InstallNow</td><td>[ProgressType3]</td><td>installs</td><td>1</td><td>0</td></row>
		<row><td>ReadyToInstall</td><td>InstallPerMachine</td><td>EndDialog</td><td>Return</td><td>OutOfNoRbDiskSpace &lt;&gt; 1</td><td>0</td></row>
		<row><td>ReadyToInstall</td><td>InstallPerMachine</td><td>NewDialog</td><td>OutOfSpace</td><td>OutOfNoRbDiskSpace = 1</td><td>0</td></row>
		<row><td>ReadyToInstall</td><td>InstallPerMachine</td><td>[ALLUSERS]</td><td>1</td><td>1</td><td>0</td></row>
		<row><td>ReadyToInstall</td><td>InstallPerMachine</td><td>[MSIINSTALLPERUSER]</td><td>{}</td><td>1</td><td>0</td></row>
		<row><td>ReadyToInstall</td><td>InstallPerMachine</td><td>[ProgressType1]</td><td>Installing</td><td>1</td><td>0</td></row>
		<row><td>ReadyToInstall</td><td>InstallPerMachine</td><td>[ProgressType2]</td><td>installed</td><td>1</td><td>0</td></row>
		<row><td>ReadyToInstall</td><td>InstallPerMachine</td><td>[ProgressType3]</td><td>installs</td><td>1</td><td>0</td></row>
		<row><td>ReadyToInstall</td><td>InstallPerUser</td><td>EndDialog</td><td>Return</td><td>OutOfNoRbDiskSpace &lt;&gt; 1</td><td>0</td></row>
		<row><td>ReadyToInstall</td><td>InstallPerUser</td><td>NewDialog</td><td>OutOfSpace</td><td>OutOfNoRbDiskSpace = 1</td><td>0</td></row>
		<row><td>ReadyToInstall</td><td>InstallPerUser</td><td>[ALLUSERS]</td><td>2</td><td>1</td><td>0</td></row>
		<row><td>ReadyToInstall</td><td>InstallPerUser</td><td>[MSIINSTALLPERUSER]</td><td>1</td><td>1</td><td>0</td></row>
		<row><td>ReadyToInstall</td><td>InstallPerUser</td><td>[ProgressType1]</td><td>Installing</td><td>1</td><td>0</td></row>
		<row><td>ReadyToInstall</td><td>InstallPerUser</td><td>[ProgressType2]</td><td>installed</td><td>1</td><td>0</td></row>
		<row><td>ReadyToInstall</td><td>InstallPerUser</td><td>[ProgressType3]</td><td>installs</td><td>1</td><td>0</td></row>
		<row><td>ReadyToRemove</td><td>Back</td><td>NewDialog</td><td>MaintenanceType</td><td>1</td><td>0</td></row>
		<row><td>ReadyToRemove</td><td>Cancel</td><td>SpawnDialog</td><td>CancelSetup</td><td>1</td><td>0</td></row>
		<row><td>ReadyToRemove</td><td>RemoveNow</td><td>EndDialog</td><td>Return</td><td>OutOfNoRbDiskSpace &lt;&gt; 1</td><td>2</td></row>
		<row><td>ReadyToRemove</td><td>RemoveNow</td><td>NewDialog</td><td>OutOfSpace</td><td>OutOfNoRbDiskSpace = 1</td><td>2</td></row>
		<row><td>ReadyToRemove</td><td>RemoveNow</td><td>Remove</td><td>ALL</td><td>1</td><td>1</td></row>
		<row><td>ReadyToRemove</td><td>RemoveNow</td><td>[ProgressType1]</td><td>Uninstalling</td><td>1</td><td>0</td></row>
		<row><td>ReadyToRemove</td><td>RemoveNow</td><td>[ProgressType2]</td><td>uninstalled</td><td>1</td><td>0</td></row>
		<row><td>ReadyToRemove</td><td>RemoveNow</td><td>[ProgressType3]</td><td>uninstalls</td><td>1</td><td>0</td></row>
		<row><td>SetupCompleteError</td><td>Back</td><td>EndDialog</td><td>Return</td><td>1</td><td>2</td></row>
		<row><td>SetupCompleteError</td><td>Back</td><td>[Suspend]</td><td>{}</td><td>1</td><td>1</td></row>
		<row><td>SetupCompleteError</td><td>Cancel</td><td>EndDialog</td><td>Return</td><td>1</td><td>2</td></row>
		<row><td>SetupCompleteError</td><td>Cancel</td><td>[Suspend]</td><td>1</td><td>1</td><td>1</td></row>
		<row><td>SetupCompleteError</td><td>Finish</td><td>DoAction</td><td>CleanUp</td><td>ISSCRIPTRUNNING="1"</td><td>1</td></row>
		<row><td>SetupCompleteError</td><td>Finish</td><td>DoAction</td><td>ShowMsiLog</td><td>MsiLogFileLocation And (ISSHOWMSILOG="1")</td><td>3</td></row>
		<row><td>SetupCompleteError</td><td>Finish</td><td>EndDialog</td><td>Exit</td><td>1</td><td>2</td></row>
		<row><td>SetupCompleteSuccess</td><td>OK</td><td>DoAction</td><td>CleanUp</td><td>ISSCRIPTRUNNING="1"</td><td>3</td></row>
		<row><td>SetupCompleteSuccess</td><td>OK</td><td>DoAction</td><td>ShowMsiLog</td><td>MsiLogFileLocation And (ISSHOWMSILOG="1") And NOT ISENABLEDWUSFINISHDIALOG</td><td>8</td></row>
		<row><td>SetupCompleteSuccess</td><td>OK</td><td>EndDialog</td><td>Exit</td><td>1</td><td>4</td></row>
		<row><td>SetupError</td><td>A</td><td>EndDialog</td><td>ErrorAbort</td><td>1</td><td>0</td></row>
		<row><td>SetupError</td><td>C</td><td>EndDialog</td><td>ErrorCancel</td><td>1</td><td>0</td></row>
		<row><td>SetupError</td><td>I</td><td>EndDialog</td><td>ErrorIgnore</td><td>1</td><td>0</td></row>
		<row><td>SetupError</td><td>N</td><td>EndDialog</td><td>ErrorNo</td><td>1</td><td>0</td></row>
		<row><td>SetupError</td><td>O</td><td>EndDialog</td><td>ErrorOk</td><td>1</td><td>0</td></row>
		<row><td>SetupError</td><td>R</td><td>EndDialog</td><td>ErrorRetry</td><td>1</td><td>0</td></row>
		<row><td>SetupError</td><td>Y</td><td>EndDialog</td><td>ErrorYes</td><td>1</td><td>0</td></row>
		<row><td>SetupInitialization</td><td>Cancel</td><td>SpawnDialog</td><td>CancelSetup</td><td>1</td><td>0</td></row>
		<row><td>SetupInterrupted</td><td>Back</td><td>EndDialog</td><td>Exit</td><td>1</td><td>2</td></row>
		<row><td>SetupInterrupted</td><td>Back</td><td>[Suspend]</td><td>{}</td><td>1</td><td>1</td></row>
		<row><td>SetupInterrupted</td><td>Cancel</td><td>EndDialog</td><td>Exit</td><td>1</td><td>2</td></row>
		<row><td>SetupInterrupted</td><td>Cancel</td><td>[Suspend]</td><td>1</td><td>1</td><td>1</td></row>
		<row><td>SetupInterrupted</td><td>Finish</td><td>DoAction</td><td>CleanUp</td><td>ISSCRIPTRUNNING="1"</td><td>1</td></row>
		<row><td>SetupInterrupted</td><td>Finish</td><td>DoAction</td><td>ShowMsiLog</td><td>MsiLogFileLocation And (ISSHOWMSILOG="1")</td><td>3</td></row>
		<row><td>SetupInterrupted</td><td>Finish</td><td>EndDialog</td><td>Exit</td><td>1</td><td>2</td></row>
		<row><td>SetupProgress</td><td>Cancel</td><td>SpawnDialog</td><td>CancelSetup</td><td>1</td><td>0</td></row>
		<row><td>SetupResume</td><td>Cancel</td><td>SpawnDialog</td><td>CancelSetup</td><td>1</td><td>0</td></row>
		<row><td>SetupResume</td><td>Next</td><td>EndDialog</td><td>Return</td><td>OutOfNoRbDiskSpace &lt;&gt; 1</td><td>0</td></row>
		<row><td>SetupResume</td><td>Next</td><td>NewDialog</td><td>OutOfSpace</td><td>OutOfNoRbDiskSpace = 1</td><td>0</td></row>
		<row><td>SetupType</td><td>Back</td><td>NewDialog</td><td>CustomerInformation</td><td>1</td><td>1</td></row>
		<row><td>SetupType</td><td>Cancel</td><td>SpawnDialog</td><td>CancelSetup</td><td>1</td><td>0</td></row>
		<row><td>SetupType</td><td>Next</td><td>NewDialog</td><td>CustomSetup</td><td>_IsSetupTypeMin = "Custom"</td><td>2</td></row>
		<row><td>SetupType</td><td>Next</td><td>NewDialog</td><td>ReadyToInstall</td><td>_IsSetupTypeMin &lt;&gt; "Custom"</td><td>1</td></row>
		<row><td>SetupType</td><td>Next</td><td>SetInstallLevel</td><td>100</td><td>_IsSetupTypeMin="Minimal"</td><td>0</td></row>
		<row><td>SetupType</td><td>Next</td><td>SetInstallLevel</td><td>200</td><td>_IsSetupTypeMin="Typical"</td><td>0</td></row>
		<row><td>SetupType</td><td>Next</td><td>SetInstallLevel</td><td>300</td><td>_IsSetupTypeMin="Custom"</td><td>0</td></row>
		<row><td>SetupType</td><td>Next</td><td>[ISRUNSETUPTYPEADDLOCALEVENT]</td><td>1</td><td>1</td><td>0</td></row>
		<row><td>SetupType</td><td>Next</td><td>[SelectedSetupType]</td><td>[DisplayNameCustom]</td><td>_IsSetupTypeMin = "Custom"</td><td>0</td></row>
		<row><td>SetupType</td><td>Next</td><td>[SelectedSetupType]</td><td>[DisplayNameMinimal]</td><td>_IsSetupTypeMin = "Minimal"</td><td>0</td></row>
		<row><td>SetupType</td><td>Next</td><td>[SelectedSetupType]</td><td>[DisplayNameTypical]</td><td>_IsSetupTypeMin = "Typical"</td><td>0</td></row>
		<row><td>SplashBitmap</td><td>Cancel</td><td>SpawnDialog</td><td>CancelSetup</td><td>1</td><td>0</td></row>
		<row><td>SplashBitmap</td><td>Next</td><td>NewDialog</td><td>InstallWelcome</td><td>1</td><td>0</td></row>
	</table>

	<table name="CreateFolder">
		<col key="yes" def="s72">Directory_</col>
		<col key="yes" def="s72">Component_</col>
		<row><td>C_</td><td>ISX_DEFAULTCOMPONENT3</td></row>
		<row><td>CommonAppDataFolder</td><td>ISX_DEFAULTCOMPONENT</td></row>
		<row><td>DFN</td><td>header.dll</td></row>
		<row><td>GOODWAY_GLS_1500_MT</td><td>ISX_DEFAULTCOMPONENT41</td></row>
		<row><td>HYUNDAI_WIA_L230A</td><td>ISX_DEFAULTCOMPONENT42</td></row>
		<row><td>INSTALLDIR</td><td>C__2A0D3AF6F7264C9A8D50E89172C4A81E</td></row>
		<row><td>INSTALLDIR</td><td>CompBase.dll</td></row>
		<row><td>INSTALLDIR</td><td>ExprEval.dll</td></row>
		<row><td>INSTALLDIR</td><td>ISX_DEFAULTCOMPONENT1</td></row>
		<row><td>INSTALLDIR</td><td>ISX_DEFAULTCOMPONENT33</td></row>
		<row><td>INSTALLDIR</td><td>ISX_DEFAULTCOMPONENT35</td></row>
		<row><td>INSTALLDIR</td><td>ISX_DEFAULTCOMPONENT36</td></row>
		<row><td>INSTALLDIR</td><td>ISX_DEFAULTCOMPONENT37</td></row>
		<row><td>INSTALLDIR</td><td>ISX_DEFAULTCOMPONENT38</td></row>
		<row><td>INSTALLDIR</td><td>ISX_DEFAULTCOMPONENT39</td></row>
		<row><td>INSTALLDIR</td><td>ISX_DEFAULTCOMPONENT40</td></row>
		<row><td>INSTALLDIR</td><td>ISX_DEFAULTCOMPONENT41</td></row>
		<row><td>INSTALLDIR</td><td>ISX_DEFAULTCOMPONENT42</td></row>
		<row><td>INSTALLDIR</td><td>ISX_DEFAULTCOMPONENT43</td></row>
		<row><td>INSTALLDIR</td><td>NCMHandlers.dll</td></row>
		<row><td>INSTALLDIR</td><td>NCManager.chm</td></row>
		<row><td>INSTALLDIR</td><td>NCManager.chm1</td></row>
		<row><td>INSTALLDIR</td><td>OCImport.dll</td></row>
		<row><td>INSTALLDIR</td><td>TKBO.dll</td></row>
		<row><td>INSTALLDIR</td><td>TKBRep.dll</td></row>
		<row><td>INSTALLDIR</td><td>TKBool.dll</td></row>
		<row><td>INSTALLDIR</td><td>TKG2d.dll</td></row>
		<row><td>INSTALLDIR</td><td>TKG3d.dll</td></row>
		<row><td>INSTALLDIR</td><td>TKGeomAlgo.dll</td></row>
		<row><td>INSTALLDIR</td><td>TKGeomBase.dll</td></row>
		<row><td>INSTALLDIR</td><td>TKIGES.dll</td></row>
		<row><td>INSTALLDIR</td><td>TKMath.dll</td></row>
		<row><td>INSTALLDIR</td><td>TKMesh.dll</td></row>
		<row><td>INSTALLDIR</td><td>TKPrim.dll</td></row>
		<row><td>INSTALLDIR</td><td>TKSTEP.dll</td></row>
		<row><td>INSTALLDIR</td><td>TKSTEP209.dll</td></row>
		<row><td>INSTALLDIR</td><td>TKSTEPAttr.dll</td></row>
		<row><td>INSTALLDIR</td><td>TKSTEPBase.dll</td></row>
		<row><td>INSTALLDIR</td><td>TKSTL.dll</td></row>
		<row><td>INSTALLDIR</td><td>TKShHealing.dll</td></row>
		<row><td>INSTALLDIR</td><td>TKTopAlgo.dll</td></row>
		<row><td>INSTALLDIR</td><td>TKXSBase.dll</td></row>
		<row><td>INSTALLDIR</td><td>TKernel.dll</td></row>
		<row><td>INSTALLDIR</td><td>header.dll</td></row>
		<row><td>INSTALLDIR</td><td>zlibwapi.dll</td></row>
		<row><td>ProgramFiles64Folder</td><td>ISX_DEFAULTCOMPONENT2</td></row>
		<row><td>TOOLS_T</td><td>ISX_DEFAULTCOMPONENT39</td></row>
		<row><td>VICTOR_TAICHUNG_A110</td><td>ISX_DEFAULTCOMPONENT43</td></row>
		<row><td>_04C433E6C8404B529DB7930D3A8E7AF4</td><td>ISX_DEFAULTCOMPONENT38</td></row>
		<row><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>ISX_DEFAULTCOMPONENT5</td></row>
		<row><td>_96CD38D4AC6C4E08AFFFF2C9F3108B20</td><td>ISX_DEFAULTCOMPONENT40</td></row>
		<row><td>_A28F2D48A7A14BEA9D34FCDE011F1513</td><td>NCManager.chm1</td></row>
		<row><td>_AE568DA94C254E8DB02AC5111802BB6B</td><td>NCManager.chm</td></row>
		<row><td>_AF9184B1F044480AAEC8CE10016E9441</td><td>ISX_DEFAULTCOMPONENT28</td></row>
		<row><td>_C4714194733E45EFB0371DCD424422AA</td><td>ISX_DEFAULTCOMPONENT30</td></row>
		<row><td>_D4445AED10494F4C88881E86F0499B93</td><td>ISX_DEFAULTCOMPONENT33</td></row>
		<row><td>_X_A_WITH_TOOLS_20</td><td>ISX_DEFAULTCOMPONENT37</td></row>
		<row><td>_X_WITH_TOOLS_20</td><td>ISX_DEFAULTCOMPONENT35</td></row>
		<row><td>_X_WITH_TOOLS_30</td><td>ISX_DEFAULTCOMPONENT36</td></row>
	</table>

	<table name="CustomAction">
		<col key="yes" def="s72">Action</col>
		<col def="i2">Type</col>
		<col def="S64">Source</col>
		<col def="S0">Target</col>
		<col def="I4">ExtendedType</col>
		<col def="S255">ISComments</col>
		<row><td>ISPreventDowngrade</td><td>19</td><td/><td>[IS_PREVENT_DOWNGRADE_EXIT]</td><td/><td>Exits install when a newer version of this product is found</td></row>
		<row><td>ISPrint</td><td>1</td><td>SetAllUsers.dll</td><td>PrintScrollableText</td><td/><td>Prints the contents of a ScrollableText control on a dialog.</td></row>
		<row><td>ISRunSetupTypeAddLocalEvent</td><td>1</td><td>ISExpHlp.dll</td><td>RunSetupTypeAddLocalEvent</td><td/><td>Run the AddLocal events associated with the Next button on the Setup Type dialog.</td></row>
		<row><td>ISSelfRegisterCosting</td><td>1</td><td>ISSELFREG.DLL</td><td>ISSelfRegisterCosting</td><td/><td/></row>
		<row><td>ISSelfRegisterFiles</td><td>3073</td><td>ISSELFREG.DLL</td><td>ISSelfRegisterFiles</td><td/><td/></row>
		<row><td>ISSelfRegisterFinalize</td><td>1</td><td>ISSELFREG.DLL</td><td>ISSelfRegisterFinalize</td><td/><td/></row>
		<row><td>ISUnSelfRegisterFiles</td><td>3073</td><td>ISSELFREG.DLL</td><td>ISUnSelfRegisterFiles</td><td/><td/></row>
		<row><td>SetARPINSTALLLOCATION</td><td>51</td><td>ARPINSTALLLOCATION</td><td>[INSTALLDIR]</td><td/><td/></row>
		<row><td>SetAllUsersProfileNT</td><td>51</td><td>ALLUSERSPROFILE</td><td>[%SystemRoot]\Profiles\All Users</td><td/><td/></row>
		<row><td>ShowMsiLog</td><td>226</td><td>SystemFolder</td><td>[SystemFolder]notepad.exe "[MsiLogFileLocation]"</td><td/><td>Shows Property-driven MSI Log</td></row>
		<row><td>setAllUsersProfile2K</td><td>51</td><td>ALLUSERSPROFILE</td><td>[%ALLUSERSPROFILE]</td><td/><td/></row>
		<row><td>setUserProfileNT</td><td>51</td><td>USERPROFILE</td><td>[%USERPROFILE]</td><td/><td/></row>
	</table>

	<table name="Dialog">
		<col key="yes" def="s72">Dialog</col>
		<col def="i2">HCentering</col>
		<col def="i2">VCentering</col>
		<col def="i2">Width</col>
		<col def="i2">Height</col>
		<col def="I4">Attributes</col>
		<col def="L128">Title</col>
		<col def="s50">Control_First</col>
		<col def="S50">Control_Default</col>
		<col def="S50">Control_Cancel</col>
		<col def="S255">ISComments</col>
		<col def="S72">TextStyle_</col>
		<col def="I4">ISWindowStyle</col>
		<col def="I4">ISResourceId</col>
		<row><td>AdminChangeFolder</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>Tail</td><td>OK</td><td>Cancel</td><td>Install Point Browse</td><td/><td>0</td><td/></row>
		<row><td>AdminNetworkLocation</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>InstallNow</td><td>InstallNow</td><td>Cancel</td><td>Network Location</td><td/><td>0</td><td/></row>
		<row><td>AdminWelcome</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>Next</td><td>Next</td><td>Cancel</td><td>Administration Welcome</td><td/><td>0</td><td/></row>
		<row><td>CancelSetup</td><td>50</td><td>50</td><td>260</td><td>85</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>No</td><td>No</td><td>No</td><td>Cancel</td><td/><td>0</td><td/></row>
		<row><td>CustomSetup</td><td>50</td><td>50</td><td>374</td><td>266</td><td>35</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>Tree</td><td>Next</td><td>Cancel</td><td>Custom Selection</td><td/><td>0</td><td/></row>
		<row><td>CustomSetupTips</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>OK</td><td>OK</td><td>OK</td><td>Custom Setup Tips</td><td/><td>0</td><td/></row>
		<row><td>CustomerInformation</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>NameEdit</td><td>Next</td><td>Cancel</td><td>Identification</td><td/><td>0</td><td/></row>
		<row><td>DatabaseFolder</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>Next</td><td>Next</td><td>Cancel</td><td>Database Folder</td><td/><td>0</td><td/></row>
		<row><td>DestinationFolder</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>Next</td><td>Next</td><td>Cancel</td><td>Destination Folder</td><td/><td>0</td><td/></row>
		<row><td>DiskSpaceRequirements</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>OK</td><td>OK</td><td>OK</td><td>Feature Details</td><td/><td>0</td><td/></row>
		<row><td>FilesInUse</td><td>50</td><td>50</td><td>374</td><td>266</td><td>19</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>Retry</td><td>Retry</td><td>Exit</td><td>Files in Use</td><td/><td>0</td><td/></row>
		<row><td>InstallChangeFolder</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>Tail</td><td>OK</td><td>Cancel</td><td>Browse</td><td/><td>0</td><td/></row>
		<row><td>InstallWelcome</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>Next</td><td>Next</td><td>Cancel</td><td>Welcome Panel</td><td/><td>0</td><td/></row>
		<row><td>LicenseAgreement</td><td>50</td><td>50</td><td>374</td><td>266</td><td>2</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>Agree</td><td>Next</td><td>Cancel</td><td>License Agreement</td><td/><td>0</td><td/></row>
		<row><td>MaintenanceType</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>RadioGroup</td><td>Next</td><td>Cancel</td><td>Change, Reinstall, Remove</td><td/><td>0</td><td/></row>
		<row><td>MaintenanceWelcome</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>Next</td><td>Next</td><td>Cancel</td><td>Maintenance Welcome</td><td/><td>0</td><td/></row>
		<row><td>MsiRMFilesInUse</td><td>50</td><td>50</td><td>374</td><td>266</td><td>19</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>OK</td><td>OK</td><td>Cancel</td><td>RestartManager Files in Use</td><td/><td>0</td><td/></row>
		<row><td>OutOfSpace</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>Resume</td><td>Resume</td><td>Resume</td><td>Out Of Disk Space</td><td/><td>0</td><td/></row>
		<row><td>PatchWelcome</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS__IsPatchDlg_PatchWizard##</td><td>Next</td><td>Next</td><td>Cancel</td><td>Patch Panel</td><td/><td>0</td><td/></row>
		<row><td>ReadmeInformation</td><td>50</td><td>50</td><td>374</td><td>266</td><td>7</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>Next</td><td>Next</td><td>Cancel</td><td>Readme Information</td><td/><td>0</td><td>0</td></row>
		<row><td>ReadyToInstall</td><td>50</td><td>50</td><td>374</td><td>266</td><td>35</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>InstallNow</td><td>InstallNow</td><td>Cancel</td><td>Ready to Install</td><td/><td>0</td><td/></row>
		<row><td>ReadyToRemove</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>RemoveNow</td><td>RemoveNow</td><td>Cancel</td><td>Verify Remove</td><td/><td>0</td><td/></row>
		<row><td>SetupCompleteError</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>Finish</td><td>Finish</td><td>Finish</td><td>Fatal Error</td><td/><td>0</td><td/></row>
		<row><td>SetupCompleteSuccess</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>OK</td><td>OK</td><td>OK</td><td>Exit</td><td/><td>0</td><td/></row>
		<row><td>SetupError</td><td>50</td><td>50</td><td>270</td><td>110</td><td>65543</td><td>##IDS__IsErrorDlg_InstallerInfo##</td><td>ErrorText</td><td>O</td><td>C</td><td>Error</td><td/><td>0</td><td/></row>
		<row><td>SetupInitialization</td><td>50</td><td>50</td><td>374</td><td>266</td><td>5</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>Cancel</td><td>Cancel</td><td>Cancel</td><td>Setup Initialization</td><td/><td>0</td><td/></row>
		<row><td>SetupInterrupted</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>Finish</td><td>Finish</td><td>Finish</td><td>User Exit</td><td/><td>0</td><td/></row>
		<row><td>SetupProgress</td><td>50</td><td>50</td><td>374</td><td>266</td><td>5</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>Cancel</td><td>Cancel</td><td>Cancel</td><td>Progress</td><td/><td>0</td><td/></row>
		<row><td>SetupResume</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>Next</td><td>Next</td><td>Cancel</td><td>Resume</td><td/><td>0</td><td/></row>
		<row><td>SetupType</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>RadioGroup</td><td>Next</td><td>Cancel</td><td>Setup Type</td><td/><td>0</td><td/></row>
		<row><td>SplashBitmap</td><td>50</td><td>50</td><td>374</td><td>266</td><td>3</td><td>##IDS_PRODUCTNAME_INSTALLSHIELD##</td><td>Next</td><td>Next</td><td>Cancel</td><td>Welcome Bitmap</td><td/><td>0</td><td/></row>
	</table>

	<table name="Directory">
		<col key="yes" def="s72">Directory</col>
		<col def="S72">Directory_Parent</col>
		<col def="l255">DefaultDir</col>
		<col def="S255">ISDescription</col>
		<col def="I4">ISAttributes</col>
		<col def="S255">ISFolderName</col>
		<row><td>ALLUSERSPROFILE</td><td>TARGETDIR</td><td>.:ALLUSE~1|All Users</td><td/><td>0</td><td/></row>
		<row><td>AdminToolsFolder</td><td>TARGETDIR</td><td>.:Admint~1|AdminTools</td><td/><td>0</td><td/></row>
		<row><td>AppDataFolder</td><td>TARGETDIR</td><td>.:APPLIC~1|Application Data</td><td/><td>0</td><td/></row>
		<row><td>C_</td><td>TARGETDIR</td><td>.</td><td/><td>0</td><td/></row>
		<row><td>CommonAppDataFolder</td><td>TARGETDIR</td><td>.:Common~1|CommonAppData</td><td/><td>0</td><td/></row>
		<row><td>CommonFiles64Folder</td><td>TARGETDIR</td><td>.:Common64</td><td/><td>0</td><td/></row>
		<row><td>CommonFilesFolder</td><td>TARGETDIR</td><td>.:Common</td><td/><td>0</td><td/></row>
		<row><td>DATABASEDIR</td><td>NCMANAGER53</td><td>.</td><td/><td>0</td><td/></row>
		<row><td>DFN</td><td>_2A3D6461C84746B08B96C228301D3D56</td><td>DFN</td><td/><td>0</td><td/></row>
		<row><td>DIRPROPERTY1</td><td>TARGETDIR</td><td>.</td><td/><td>0</td><td/></row>
		<row><td>DMU80</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>DMU80</td><td/><td>0</td><td/></row>
		<row><td>DM_4X_TABLE_A</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>DM_4X_~1|DM_4X_TABLE_A</td><td/><td>0</td><td/></row>
		<row><td>DesktopFolder</td><td>TARGETDIR</td><td>.:Desktop</td><td/><td>3</td><td/></row>
		<row><td>FADAL3X_VMC6030_INCH</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>FADAL3~1|FADAL3X-VMC6030-INCH</td><td/><td>0</td><td/></row>
		<row><td>FANMTDEMO2CHUSK</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>FANMTD~1|FanMTDemo2Chusk</td><td/><td>0</td><td/></row>
		<row><td>FavoritesFolder</td><td>TARGETDIR</td><td>.:FAVORI~1|Favorites</td><td/><td>0</td><td/></row>
		<row><td>FontsFolder</td><td>TARGETDIR</td><td>.:Fonts</td><td/><td>0</td><td/></row>
		<row><td>GOODWAY_GLS_1500_MT</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>GOODWA~1|GOODWAY_GLS-1500_MT</td><td/><td>0</td><td/></row>
		<row><td>GlobalAssemblyCache</td><td>TARGETDIR</td><td>.:Global~1|GlobalAssemblyCache</td><td/><td>0</td><td/></row>
		<row><td>HYUNDAI_WIA_L230A</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>HYUNDA~1|HYUNDAI_WIA_L230A</td><td/><td>0</td><td/></row>
		<row><td>INSTALLDIR</td><td>NCM50</td><td>.</td><td/><td>0</td><td/></row>
		<row><td>INTEKS</td><td>ProgramFiles64Folder</td><td>Inteks</td><td/><td>0</td><td/></row>
		<row><td>ISCommonFilesFolder</td><td>CommonFilesFolder</td><td>Instal~1|InstallShield</td><td/><td>0</td><td/></row>
		<row><td>ISMyCompanyDir</td><td>ProgramFilesFolder</td><td>MYCOMP~1|My Company Name</td><td/><td>0</td><td/></row>
		<row><td>ISMyProductDir</td><td>ISMyCompanyDir</td><td>MYPROD~1|My Product Name</td><td/><td>0</td><td/></row>
		<row><td>ISYourDataBaseDir</td><td>C_</td><td>NCMANA~1|NCManager</td><td/><td>0</td><td/></row>
		<row><td>LATHE_VERT_1512</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>LATHE-~1|LATHE-VERT-1512</td><td/><td>0</td><td/></row>
		<row><td>LocalAppDataFolder</td><td>TARGETDIR</td><td>.:LocalA~1|LocalAppData</td><td/><td>0</td><td/></row>
		<row><td>MILL4AX</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>MILL4AX</td><td/><td>0</td><td/></row>
		<row><td>MILLTURN</td><td>_20411C75B4BB4D2EB5301C1DEBF548B9</td><td>MillTurn</td><td/><td>0</td><td/></row>
		<row><td>MILLTURN_DEFAULT</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>MILLTU~1|MILLTURN_DEFAULT</td><td/><td>0</td><td/></row>
		<row><td>MILLTURN_DEMO</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>MILLTU~1|MILLTURN-DEMO</td><td/><td>0</td><td/></row>
		<row><td>MILL_2X</td><td>_20411C75B4BB4D2EB5301C1DEBF548B9</td><td>Mill_2X</td><td/><td>0</td><td/></row>
		<row><td>MILL_3X</td><td>_20411C75B4BB4D2EB5301C1DEBF548B9</td><td>Mill_3X</td><td/><td>0</td><td/></row>
		<row><td>MILL_5X</td><td>_20411C75B4BB4D2EB5301C1DEBF548B9</td><td>Mill_5X</td><td/><td>0</td><td/></row>
		<row><td>MILL_DEFAULT</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>MILL_D~1|MILL_DEFAULT</td><td/><td>0</td><td/></row>
		<row><td>MY_PRODUCT_NAME</td><td>YOUR_COMPANY_NAME</td><td>MYPROD~1|My Product Name</td><td/><td>0</td><td/></row>
		<row><td>MyPicturesFolder</td><td>TARGETDIR</td><td>.:MyPict~1|MyPictures</td><td/><td>0</td><td/></row>
		<row><td>NCM43</td><td>INTEKS</td><td>NCM43</td><td/><td>0</td><td/></row>
		<row><td>NCM44</td><td>INTEKS</td><td>NCM44</td><td/><td>0</td><td/></row>
		<row><td>NCM45</td><td>INTEKS</td><td>NCM45</td><td/><td>0</td><td/></row>
		<row><td>NCM46</td><td>INTEKS</td><td>NCM48</td><td/><td>0</td><td/></row>
		<row><td>NCM49</td><td>INTEKS</td><td>NCM49</td><td/><td>0</td><td/></row>
		<row><td>NCM50</td><td>INTEKS</td><td>NCM53</td><td/><td>0</td><td/></row>
		<row><td>NCMANAGER53</td><td>DIRPROPERTY1</td><td>NCMANA~1|NCManager53</td><td/><td>0</td><td/></row>
		<row><td>NetHoodFolder</td><td>TARGETDIR</td><td>.:NetHood</td><td/><td>0</td><td/></row>
		<row><td>PersonalFolder</td><td>TARGETDIR</td><td>.:Personal</td><td/><td>0</td><td/></row>
		<row><td>PrimaryVolumePath</td><td>TARGETDIR</td><td>.:Primar~1|PrimaryVolumePath</td><td/><td>0</td><td/></row>
		<row><td>PrintHoodFolder</td><td>TARGETDIR</td><td>.:PRINTH~1|PrintHood</td><td/><td>0</td><td/></row>
		<row><td>ProgramFiles64Folder</td><td>TARGETDIR</td><td>.:Prog64~1|Program Files 64</td><td/><td>0</td><td/></row>
		<row><td>ProgramFilesFolder</td><td>TARGETDIR</td><td>.:PROGRA~1|program files</td><td/><td>0</td><td/></row>
		<row><td>ProgramMenuFolder</td><td>TARGETDIR</td><td>.:Programs</td><td/><td>3</td><td/></row>
		<row><td>R_510</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>R-510</td><td/><td>0</td><td/></row>
		<row><td>RecentFolder</td><td>TARGETDIR</td><td>.:Recent</td><td/><td>0</td><td/></row>
		<row><td>SendToFolder</td><td>TARGETDIR</td><td>.:SendTo</td><td/><td>3</td><td/></row>
		<row><td>StartMenuFolder</td><td>TARGETDIR</td><td>.:STARTM~1|Start Menu</td><td/><td>3</td><td/></row>
		<row><td>StartupFolder</td><td>TARGETDIR</td><td>.:StartUp</td><td/><td>3</td><td/></row>
		<row><td>System16Folder</td><td>TARGETDIR</td><td>.:System</td><td/><td>0</td><td/></row>
		<row><td>System64Folder</td><td>TARGETDIR</td><td>.:System64</td><td/><td>0</td><td/></row>
		<row><td>SystemFolder</td><td>TARGETDIR</td><td>.:System32</td><td/><td>0</td><td/></row>
		<row><td>TARGETDIR</td><td/><td>SourceDir</td><td/><td>0</td><td/></row>
		<row><td>TOOLS_T</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>TOOLS_T</td><td/><td>0</td><td/></row>
		<row><td>TURN</td><td>_20411C75B4BB4D2EB5301C1DEBF548B9</td><td>Turn</td><td/><td>0</td><td/></row>
		<row><td>TURN_DEFAULT</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>TURN_D~1|TURN_DEFAULT</td><td/><td>0</td><td/></row>
		<row><td>TempFolder</td><td>TARGETDIR</td><td>.:Temp</td><td/><td>0</td><td/></row>
		<row><td>TemplateFolder</td><td>TARGETDIR</td><td>.:ShellNew</td><td/><td>0</td><td/></row>
		<row><td>USERPROFILE</td><td>TARGETDIR</td><td>.:USERPR~1|UserProfile</td><td/><td>0</td><td/></row>
		<row><td>VICTOR_TAICHUNG_A110</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>VICTOR~1|VICTOR_TAICHUNG_A110</td><td/><td>0</td><td/></row>
		<row><td>WELD</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>WELD</td><td/><td>0</td><td/></row>
		<row><td>WindowsFolder</td><td>TARGETDIR</td><td>.:Windows</td><td/><td>0</td><td/></row>
		<row><td>WindowsVolume</td><td>TARGETDIR</td><td>.:WinRoot</td><td/><td>0</td><td/></row>
		<row><td>YOUR_COMPANY_NAME</td><td>ProgramFilesFolder</td><td>YOURCO~1|Your Company Name</td><td/><td>0</td><td/></row>
		<row><td>_04C433E6C8404B529DB7930D3A8E7AF4</td><td>DATABASEDIR</td><td>Work</td><td/><td>0</td><td/></row>
		<row><td>_20411C75B4BB4D2EB5301C1DEBF548B9</td><td>_B657F2162C104399B5A08BE8BDDA6480</td><td>MILLIM~1|Millimeter</td><td/><td>0</td><td/></row>
		<row><td>_2A3D6461C84746B08B96C228301D3D56</td><td>DATABASEDIR</td><td>Def</td><td/><td>0</td><td/></row>
		<row><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>_2A3D6461C84746B08B96C228301D3D56</td><td>Machines</td><td/><td>0</td><td/></row>
		<row><td>_96CD38D4AC6C4E08AFFFF2C9F3108B20</td><td>DATABASEDIR</td><td>Doc</td><td/><td>0</td><td/></row>
		<row><td>_A28F2D48A7A14BEA9D34FCDE011F1513</td><td>_E7BBC487F3C845DA8627EF81556911FF</td><td>Eng</td><td/><td>0</td><td/></row>
		<row><td>_A516MF3</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>1A516MF3</td><td/><td>0</td><td/></row>
		<row><td>_ABF01A60B67E49A8953409B9471A5989</td><td>_C4714194733E45EFB0371DCD424422AA</td><td>DUAL_D~1|dual_depth_peeling</td><td/><td>0</td><td/></row>
		<row><td>_AE568DA94C254E8DB02AC5111802BB6B</td><td>_E7BBC487F3C845DA8627EF81556911FF</td><td>Rus</td><td/><td>0</td><td/></row>
		<row><td>_AF9184B1F044480AAEC8CE10016E9441</td><td>_B657F2162C104399B5A08BE8BDDA6480</td><td>Inch</td><td/><td>0</td><td/></row>
		<row><td>_B657F2162C104399B5A08BE8BDDA6480</td><td>DATABASEDIR</td><td>Demo</td><td/><td>0</td><td/></row>
		<row><td>_C4714194733E45EFB0371DCD424422AA</td><td>_2A3D6461C84746B08B96C228301D3D56</td><td>PRT</td><td/><td>0</td><td/></row>
		<row><td>_D4445AED10494F4C88881E86F0499B93</td><td>INSTALLDIR</td><td>NCMExt</td><td/><td>0</td><td/></row>
		<row><td>_E7BBC487F3C845DA8627EF81556911FF</td><td>INSTALLDIR</td><td>Local</td><td/><td>0</td><td/></row>
		<row><td>_X_A_WITH_TOOLS_20</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>4X_A_W~1|4X_A_WITH_TOOLS_20</td><td/><td>0</td><td/></row>
		<row><td>_X_DEFAULT</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>3X_DEF~1|3X_DEFAULT</td><td/><td>0</td><td/></row>
		<row><td>_X_HARDINGE_VMC</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>4X_HAR~1|4X_HARDINGE_VMC</td><td/><td>0</td><td/></row>
		<row><td>_X_HEAD_X_ON_HEAD_Z</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>5X_HEA~1|5X_Head-X_on_Head-Z</td><td/><td>0</td><td/></row>
		<row><td>_X_HERMLE_C30U_INCH</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>5X_HER~1|5x_HERMLE_C30U_INCH</td><td/><td>0</td><td/></row>
		<row><td>_X_WITH_TOOLS_20</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>3X_WIT~1|3X_WITH_TOOLS_20</td><td/><td>0</td><td/></row>
		<row><td>_X_WITH_TOOLS_30</td><td>_6B5F5EDBB3664F8A95CDD6D04CA70B67</td><td>3X_WIT~1|3X_WITH_TOOLS_30</td><td/><td>0</td><td/></row>
	</table>

	<table name="DrLocator">
		<col key="yes" def="s72">Signature_</col>
		<col key="yes" def="S72">Parent</col>
		<col key="yes" def="S255">Path</col>
		<col def="I2">Depth</col>
	</table>

	<table name="DuplicateFile">
		<col key="yes" def="s72">FileKey</col>
		<col def="s72">Component_</col>
		<col def="s72">File_</col>
		<col def="L255">DestName</col>
		<col def="S72">DestFolder</col>
	</table>

	<table name="Environment">
		<col key="yes" def="s72">Environment</col>
		<col def="l255">Name</col>
		<col def="L255">Value</col>
		<col def="s72">Component_</col>
	</table>

	<table name="Error">
		<col key="yes" def="i2">Error</col>
		<col def="L255">Message</col>
		<row><td>0</td><td>##IDS_ERROR_0##</td></row>
		<row><td>1</td><td>##IDS_ERROR_1##</td></row>
		<row><td>10</td><td>##IDS_ERROR_8##</td></row>
		<row><td>11</td><td>##IDS_ERROR_9##</td></row>
		<row><td>1101</td><td>##IDS_ERROR_22##</td></row>
		<row><td>12</td><td>##IDS_ERROR_10##</td></row>
		<row><td>13</td><td>##IDS_ERROR_11##</td></row>
		<row><td>1301</td><td>##IDS_ERROR_23##</td></row>
		<row><td>1302</td><td>##IDS_ERROR_24##</td></row>
		<row><td>1303</td><td>##IDS_ERROR_25##</td></row>
		<row><td>1304</td><td>##IDS_ERROR_26##</td></row>
		<row><td>1305</td><td>##IDS_ERROR_27##</td></row>
		<row><td>1306</td><td>##IDS_ERROR_28##</td></row>
		<row><td>1307</td><td>##IDS_ERROR_29##</td></row>
		<row><td>1308</td><td>##IDS_ERROR_30##</td></row>
		<row><td>1309</td><td>##IDS_ERROR_31##</td></row>
		<row><td>1310</td><td>##IDS_ERROR_32##</td></row>
		<row><td>1311</td><td>##IDS_ERROR_33##</td></row>
		<row><td>1312</td><td>##IDS_ERROR_34##</td></row>
		<row><td>1313</td><td>##IDS_ERROR_35##</td></row>
		<row><td>1314</td><td>##IDS_ERROR_36##</td></row>
		<row><td>1315</td><td>##IDS_ERROR_37##</td></row>
		<row><td>1316</td><td>##IDS_ERROR_38##</td></row>
		<row><td>1317</td><td>##IDS_ERROR_39##</td></row>
		<row><td>1318</td><td>##IDS_ERROR_40##</td></row>
		<row><td>1319</td><td>##IDS_ERROR_41##</td></row>
		<row><td>1320</td><td>##IDS_ERROR_42##</td></row>
		<row><td>1321</td><td>##IDS_ERROR_43##</td></row>
		<row><td>1322</td><td>##IDS_ERROR_44##</td></row>
		<row><td>1323</td><td>##IDS_ERROR_45##</td></row>
		<row><td>1324</td><td>##IDS_ERROR_46##</td></row>
		<row><td>1325</td><td>##IDS_ERROR_47##</td></row>
		<row><td>1326</td><td>##IDS_ERROR_48##</td></row>
		<row><td>1327</td><td>##IDS_ERROR_49##</td></row>
		<row><td>1328</td><td>##IDS_ERROR_122##</td></row>
		<row><td>1329</td><td>##IDS_ERROR_1329##</td></row>
		<row><td>1330</td><td>##IDS_ERROR_1330##</td></row>
		<row><td>1331</td><td>##IDS_ERROR_1331##</td></row>
		<row><td>1332</td><td>##IDS_ERROR_1332##</td></row>
		<row><td>1333</td><td>##IDS_ERROR_1333##</td></row>
		<row><td>1334</td><td>##IDS_ERROR_1334##</td></row>
		<row><td>1335</td><td>##IDS_ERROR_1335##</td></row>
		<row><td>1336</td><td>##IDS_ERROR_1336##</td></row>
		<row><td>14</td><td>##IDS_ERROR_12##</td></row>
		<row><td>1401</td><td>##IDS_ERROR_50##</td></row>
		<row><td>1402</td><td>##IDS_ERROR_51##</td></row>
		<row><td>1403</td><td>##IDS_ERROR_52##</td></row>
		<row><td>1404</td><td>##IDS_ERROR_53##</td></row>
		<row><td>1405</td><td>##IDS_ERROR_54##</td></row>
		<row><td>1406</td><td>##IDS_ERROR_55##</td></row>
		<row><td>1407</td><td>##IDS_ERROR_56##</td></row>
		<row><td>1408</td><td>##IDS_ERROR_57##</td></row>
		<row><td>1409</td><td>##IDS_ERROR_58##</td></row>
		<row><td>1410</td><td>##IDS_ERROR_59##</td></row>
		<row><td>15</td><td>##IDS_ERROR_13##</td></row>
		<row><td>1500</td><td>##IDS_ERROR_60##</td></row>
		<row><td>1501</td><td>##IDS_ERROR_61##</td></row>
		<row><td>1502</td><td>##IDS_ERROR_62##</td></row>
		<row><td>1503</td><td>##IDS_ERROR_63##</td></row>
		<row><td>16</td><td>##IDS_ERROR_14##</td></row>
		<row><td>1601</td><td>##IDS_ERROR_64##</td></row>
		<row><td>1602</td><td>##IDS_ERROR_65##</td></row>
		<row><td>1603</td><td>##IDS_ERROR_66##</td></row>
		<row><td>1604</td><td>##IDS_ERROR_67##</td></row>
		<row><td>1605</td><td>##IDS_ERROR_68##</td></row>
		<row><td>1606</td><td>##IDS_ERROR_69##</td></row>
		<row><td>1607</td><td>##IDS_ERROR_70##</td></row>
		<row><td>1608</td><td>##IDS_ERROR_71##</td></row>
		<row><td>1609</td><td>##IDS_ERROR_1609##</td></row>
		<row><td>1651</td><td>##IDS_ERROR_1651##</td></row>
		<row><td>17</td><td>##IDS_ERROR_15##</td></row>
		<row><td>1701</td><td>##IDS_ERROR_72##</td></row>
		<row><td>1702</td><td>##IDS_ERROR_73##</td></row>
		<row><td>1703</td><td>##IDS_ERROR_74##</td></row>
		<row><td>1704</td><td>##IDS_ERROR_75##</td></row>
		<row><td>1705</td><td>##IDS_ERROR_76##</td></row>
		<row><td>1706</td><td>##IDS_ERROR_77##</td></row>
		<row><td>1707</td><td>##IDS_ERROR_78##</td></row>
		<row><td>1708</td><td>##IDS_ERROR_79##</td></row>
		<row><td>1709</td><td>##IDS_ERROR_80##</td></row>
		<row><td>1710</td><td>##IDS_ERROR_81##</td></row>
		<row><td>1711</td><td>##IDS_ERROR_82##</td></row>
		<row><td>1712</td><td>##IDS_ERROR_83##</td></row>
		<row><td>1713</td><td>##IDS_ERROR_123##</td></row>
		<row><td>1714</td><td>##IDS_ERROR_124##</td></row>
		<row><td>1715</td><td>##IDS_ERROR_1715##</td></row>
		<row><td>1716</td><td>##IDS_ERROR_1716##</td></row>
		<row><td>1717</td><td>##IDS_ERROR_1717##</td></row>
		<row><td>1718</td><td>##IDS_ERROR_1718##</td></row>
		<row><td>1719</td><td>##IDS_ERROR_1719##</td></row>
		<row><td>1720</td><td>##IDS_ERROR_1720##</td></row>
		<row><td>1721</td><td>##IDS_ERROR_1721##</td></row>
		<row><td>1722</td><td>##IDS_ERROR_1722##</td></row>
		<row><td>1723</td><td>##IDS_ERROR_1723##</td></row>
		<row><td>1724</td><td>##IDS_ERROR_1724##</td></row>
		<row><td>1725</td><td>##IDS_ERROR_1725##</td></row>
		<row><td>1726</td><td>##IDS_ERROR_1726##</td></row>
		<row><td>1727</td><td>##IDS_ERROR_1727##</td></row>
		<row><td>1728</td><td>##IDS_ERROR_1728##</td></row>
		<row><td>1729</td><td>##IDS_ERROR_1729##</td></row>
		<row><td>1730</td><td>##IDS_ERROR_1730##</td></row>
		<row><td>1731</td><td>##IDS_ERROR_1731##</td></row>
		<row><td>1732</td><td>##IDS_ERROR_1732##</td></row>
		<row><td>18</td><td>##IDS_ERROR_16##</td></row>
		<row><td>1801</td><td>##IDS_ERROR_84##</td></row>
		<row><td>1802</td><td>##IDS_ERROR_85##</td></row>
		<row><td>1803</td><td>##IDS_ERROR_86##</td></row>
		<row><td>1804</td><td>##IDS_ERROR_87##</td></row>
		<row><td>1805</td><td>##IDS_ERROR_88##</td></row>
		<row><td>1806</td><td>##IDS_ERROR_89##</td></row>
		<row><td>1807</td><td>##IDS_ERROR_90##</td></row>
		<row><td>19</td><td>##IDS_ERROR_17##</td></row>
		<row><td>1901</td><td>##IDS_ERROR_91##</td></row>
		<row><td>1902</td><td>##IDS_ERROR_92##</td></row>
		<row><td>1903</td><td>##IDS_ERROR_93##</td></row>
		<row><td>1904</td><td>##IDS_ERROR_94##</td></row>
		<row><td>1905</td><td>##IDS_ERROR_95##</td></row>
		<row><td>1906</td><td>##IDS_ERROR_96##</td></row>
		<row><td>1907</td><td>##IDS_ERROR_97##</td></row>
		<row><td>1908</td><td>##IDS_ERROR_98##</td></row>
		<row><td>1909</td><td>##IDS_ERROR_99##</td></row>
		<row><td>1910</td><td>##IDS_ERROR_100##</td></row>
		<row><td>1911</td><td>##IDS_ERROR_101##</td></row>
		<row><td>1912</td><td>##IDS_ERROR_102##</td></row>
		<row><td>1913</td><td>##IDS_ERROR_103##</td></row>
		<row><td>1914</td><td>##IDS_ERROR_104##</td></row>
		<row><td>1915</td><td>##IDS_ERROR_105##</td></row>
		<row><td>1916</td><td>##IDS_ERROR_106##</td></row>
		<row><td>1917</td><td>##IDS_ERROR_107##</td></row>
		<row><td>1918</td><td>##IDS_ERROR_108##</td></row>
		<row><td>1919</td><td>##IDS_ERROR_109##</td></row>
		<row><td>1920</td><td>##IDS_ERROR_110##</td></row>
		<row><td>1921</td><td>##IDS_ERROR_111##</td></row>
		<row><td>1922</td><td>##IDS_ERROR_112##</td></row>
		<row><td>1923</td><td>##IDS_ERROR_113##</td></row>
		<row><td>1924</td><td>##IDS_ERROR_114##</td></row>
		<row><td>1925</td><td>##IDS_ERROR_115##</td></row>
		<row><td>1926</td><td>##IDS_ERROR_116##</td></row>
		<row><td>1927</td><td>##IDS_ERROR_117##</td></row>
		<row><td>1928</td><td>##IDS_ERROR_118##</td></row>
		<row><td>1929</td><td>##IDS_ERROR_119##</td></row>
		<row><td>1930</td><td>##IDS_ERROR_125##</td></row>
		<row><td>1931</td><td>##IDS_ERROR_126##</td></row>
		<row><td>1932</td><td>##IDS_ERROR_127##</td></row>
		<row><td>1933</td><td>##IDS_ERROR_128##</td></row>
		<row><td>1934</td><td>##IDS_ERROR_129##</td></row>
		<row><td>1935</td><td>##IDS_ERROR_1935##</td></row>
		<row><td>1936</td><td>##IDS_ERROR_1936##</td></row>
		<row><td>1937</td><td>##IDS_ERROR_1937##</td></row>
		<row><td>1938</td><td>##IDS_ERROR_1938##</td></row>
		<row><td>2</td><td>##IDS_ERROR_2##</td></row>
		<row><td>20</td><td>##IDS_ERROR_18##</td></row>
		<row><td>21</td><td>##IDS_ERROR_19##</td></row>
		<row><td>2101</td><td>##IDS_ERROR_2101##</td></row>
		<row><td>2102</td><td>##IDS_ERROR_2102##</td></row>
		<row><td>2103</td><td>##IDS_ERROR_2103##</td></row>
		<row><td>2104</td><td>##IDS_ERROR_2104##</td></row>
		<row><td>2105</td><td>##IDS_ERROR_2105##</td></row>
		<row><td>2106</td><td>##IDS_ERROR_2106##</td></row>
		<row><td>2107</td><td>##IDS_ERROR_2107##</td></row>
		<row><td>2108</td><td>##IDS_ERROR_2108##</td></row>
		<row><td>2109</td><td>##IDS_ERROR_2109##</td></row>
		<row><td>2110</td><td>##IDS_ERROR_2110##</td></row>
		<row><td>2111</td><td>##IDS_ERROR_2111##</td></row>
		<row><td>2112</td><td>##IDS_ERROR_2112##</td></row>
		<row><td>2113</td><td>##IDS_ERROR_2113##</td></row>
		<row><td>22</td><td>##IDS_ERROR_120##</td></row>
		<row><td>2200</td><td>##IDS_ERROR_2200##</td></row>
		<row><td>2201</td><td>##IDS_ERROR_2201##</td></row>
		<row><td>2202</td><td>##IDS_ERROR_2202##</td></row>
		<row><td>2203</td><td>##IDS_ERROR_2203##</td></row>
		<row><td>2204</td><td>##IDS_ERROR_2204##</td></row>
		<row><td>2205</td><td>##IDS_ERROR_2205##</td></row>
		<row><td>2206</td><td>##IDS_ERROR_2206##</td></row>
		<row><td>2207</td><td>##IDS_ERROR_2207##</td></row>
		<row><td>2208</td><td>##IDS_ERROR_2208##</td></row>
		<row><td>2209</td><td>##IDS_ERROR_2209##</td></row>
		<row><td>2210</td><td>##IDS_ERROR_2210##</td></row>
		<row><td>2211</td><td>##IDS_ERROR_2211##</td></row>
		<row><td>2212</td><td>##IDS_ERROR_2212##</td></row>
		<row><td>2213</td><td>##IDS_ERROR_2213##</td></row>
		<row><td>2214</td><td>##IDS_ERROR_2214##</td></row>
		<row><td>2215</td><td>##IDS_ERROR_2215##</td></row>
		<row><td>2216</td><td>##IDS_ERROR_2216##</td></row>
		<row><td>2217</td><td>##IDS_ERROR_2217##</td></row>
		<row><td>2218</td><td>##IDS_ERROR_2218##</td></row>
		<row><td>2219</td><td>##IDS_ERROR_2219##</td></row>
		<row><td>2220</td><td>##IDS_ERROR_2220##</td></row>
		<row><td>2221</td><td>##IDS_ERROR_2221##</td></row>
		<row><td>2222</td><td>##IDS_ERROR_2222##</td></row>
		<row><td>2223</td><td>##IDS_ERROR_2223##</td></row>
		<row><td>2224</td><td>##IDS_ERROR_2224##</td></row>
		<row><td>2225</td><td>##IDS_ERROR_2225##</td></row>
		<row><td>2226</td><td>##IDS_ERROR_2226##</td></row>
		<row><td>2227</td><td>##IDS_ERROR_2227##</td></row>
		<row><td>2228</td><td>##IDS_ERROR_2228##</td></row>
		<row><td>2229</td><td>##IDS_ERROR_2229##</td></row>
		<row><td>2230</td><td>##IDS_ERROR_2230##</td></row>
		<row><td>2231</td><td>##IDS_ERROR_2231##</td></row>
		<row><td>2232</td><td>##IDS_ERROR_2232##</td></row>
		<row><td>2233</td><td>##IDS_ERROR_2233##</td></row>
		<row><td>2234</td><td>##IDS_ERROR_2234##</td></row>
		<row><td>2235</td><td>##IDS_ERROR_2235##</td></row>
		<row><td>2236</td><td>##IDS_ERROR_2236##</td></row>
		<row><td>2237</td><td>##IDS_ERROR_2237##</td></row>
		<row><td>2238</td><td>##IDS_ERROR_2238##</td></row>
		<row><td>2239</td><td>##IDS_ERROR_2239##</td></row>
		<row><td>2240</td><td>##IDS_ERROR_2240##</td></row>
		<row><td>2241</td><td>##IDS_ERROR_2241##</td></row>
		<row><td>2242</td><td>##IDS_ERROR_2242##</td></row>
		<row><td>2243</td><td>##IDS_ERROR_2243##</td></row>
		<row><td>2244</td><td>##IDS_ERROR_2244##</td></row>
		<row><td>2245</td><td>##IDS_ERROR_2245##</td></row>
		<row><td>2246</td><td>##IDS_ERROR_2246##</td></row>
		<row><td>2247</td><td>##IDS_ERROR_2247##</td></row>
		<row><td>2248</td><td>##IDS_ERROR_2248##</td></row>
		<row><td>2249</td><td>##IDS_ERROR_2249##</td></row>
		<row><td>2250</td><td>##IDS_ERROR_2250##</td></row>
		<row><td>2251</td><td>##IDS_ERROR_2251##</td></row>
		<row><td>2252</td><td>##IDS_ERROR_2252##</td></row>
		<row><td>2253</td><td>##IDS_ERROR_2253##</td></row>
		<row><td>2254</td><td>##IDS_ERROR_2254##</td></row>
		<row><td>2255</td><td>##IDS_ERROR_2255##</td></row>
		<row><td>2256</td><td>##IDS_ERROR_2256##</td></row>
		<row><td>2257</td><td>##IDS_ERROR_2257##</td></row>
		<row><td>2258</td><td>##IDS_ERROR_2258##</td></row>
		<row><td>2259</td><td>##IDS_ERROR_2259##</td></row>
		<row><td>2260</td><td>##IDS_ERROR_2260##</td></row>
		<row><td>2261</td><td>##IDS_ERROR_2261##</td></row>
		<row><td>2262</td><td>##IDS_ERROR_2262##</td></row>
		<row><td>2263</td><td>##IDS_ERROR_2263##</td></row>
		<row><td>2264</td><td>##IDS_ERROR_2264##</td></row>
		<row><td>2265</td><td>##IDS_ERROR_2265##</td></row>
		<row><td>2266</td><td>##IDS_ERROR_2266##</td></row>
		<row><td>2267</td><td>##IDS_ERROR_2267##</td></row>
		<row><td>2268</td><td>##IDS_ERROR_2268##</td></row>
		<row><td>2269</td><td>##IDS_ERROR_2269##</td></row>
		<row><td>2270</td><td>##IDS_ERROR_2270##</td></row>
		<row><td>2271</td><td>##IDS_ERROR_2271##</td></row>
		<row><td>2272</td><td>##IDS_ERROR_2272##</td></row>
		<row><td>2273</td><td>##IDS_ERROR_2273##</td></row>
		<row><td>2274</td><td>##IDS_ERROR_2274##</td></row>
		<row><td>2275</td><td>##IDS_ERROR_2275##</td></row>
		<row><td>2276</td><td>##IDS_ERROR_2276##</td></row>
		<row><td>2277</td><td>##IDS_ERROR_2277##</td></row>
		<row><td>2278</td><td>##IDS_ERROR_2278##</td></row>
		<row><td>2279</td><td>##IDS_ERROR_2279##</td></row>
		<row><td>2280</td><td>##IDS_ERROR_2280##</td></row>
		<row><td>2281</td><td>##IDS_ERROR_2281##</td></row>
		<row><td>2282</td><td>##IDS_ERROR_2282##</td></row>
		<row><td>23</td><td>##IDS_ERROR_121##</td></row>
		<row><td>2302</td><td>##IDS_ERROR_2302##</td></row>
		<row><td>2303</td><td>##IDS_ERROR_2303##</td></row>
		<row><td>2304</td><td>##IDS_ERROR_2304##</td></row>
		<row><td>2305</td><td>##IDS_ERROR_2305##</td></row>
		<row><td>2306</td><td>##IDS_ERROR_2306##</td></row>
		<row><td>2307</td><td>##IDS_ERROR_2307##</td></row>
		<row><td>2308</td><td>##IDS_ERROR_2308##</td></row>
		<row><td>2309</td><td>##IDS_ERROR_2309##</td></row>
		<row><td>2310</td><td>##IDS_ERROR_2310##</td></row>
		<row><td>2315</td><td>##IDS_ERROR_2315##</td></row>
		<row><td>2318</td><td>##IDS_ERROR_2318##</td></row>
		<row><td>2319</td><td>##IDS_ERROR_2319##</td></row>
		<row><td>2320</td><td>##IDS_ERROR_2320##</td></row>
		<row><td>2321</td><td>##IDS_ERROR_2321##</td></row>
		<row><td>2322</td><td>##IDS_ERROR_2322##</td></row>
		<row><td>2323</td><td>##IDS_ERROR_2323##</td></row>
		<row><td>2324</td><td>##IDS_ERROR_2324##</td></row>
		<row><td>2325</td><td>##IDS_ERROR_2325##</td></row>
		<row><td>2326</td><td>##IDS_ERROR_2326##</td></row>
		<row><td>2327</td><td>##IDS_ERROR_2327##</td></row>
		<row><td>2328</td><td>##IDS_ERROR_2328##</td></row>
		<row><td>2329</td><td>##IDS_ERROR_2329##</td></row>
		<row><td>2330</td><td>##IDS_ERROR_2330##</td></row>
		<row><td>2331</td><td>##IDS_ERROR_2331##</td></row>
		<row><td>2332</td><td>##IDS_ERROR_2332##</td></row>
		<row><td>2333</td><td>##IDS_ERROR_2333##</td></row>
		<row><td>2334</td><td>##IDS_ERROR_2334##</td></row>
		<row><td>2335</td><td>##IDS_ERROR_2335##</td></row>
		<row><td>2336</td><td>##IDS_ERROR_2336##</td></row>
		<row><td>2337</td><td>##IDS_ERROR_2337##</td></row>
		<row><td>2338</td><td>##IDS_ERROR_2338##</td></row>
		<row><td>2339</td><td>##IDS_ERROR_2339##</td></row>
		<row><td>2340</td><td>##IDS_ERROR_2340##</td></row>
		<row><td>2341</td><td>##IDS_ERROR_2341##</td></row>
		<row><td>2342</td><td>##IDS_ERROR_2342##</td></row>
		<row><td>2343</td><td>##IDS_ERROR_2343##</td></row>
		<row><td>2344</td><td>##IDS_ERROR_2344##</td></row>
		<row><td>2345</td><td>##IDS_ERROR_2345##</td></row>
		<row><td>2347</td><td>##IDS_ERROR_2347##</td></row>
		<row><td>2348</td><td>##IDS_ERROR_2348##</td></row>
		<row><td>2349</td><td>##IDS_ERROR_2349##</td></row>
		<row><td>2350</td><td>##IDS_ERROR_2350##</td></row>
		<row><td>2351</td><td>##IDS_ERROR_2351##</td></row>
		<row><td>2352</td><td>##IDS_ERROR_2352##</td></row>
		<row><td>2353</td><td>##IDS_ERROR_2353##</td></row>
		<row><td>2354</td><td>##IDS_ERROR_2354##</td></row>
		<row><td>2355</td><td>##IDS_ERROR_2355##</td></row>
		<row><td>2356</td><td>##IDS_ERROR_2356##</td></row>
		<row><td>2357</td><td>##IDS_ERROR_2357##</td></row>
		<row><td>2358</td><td>##IDS_ERROR_2358##</td></row>
		<row><td>2359</td><td>##IDS_ERROR_2359##</td></row>
		<row><td>2360</td><td>##IDS_ERROR_2360##</td></row>
		<row><td>2361</td><td>##IDS_ERROR_2361##</td></row>
		<row><td>2362</td><td>##IDS_ERROR_2362##</td></row>
		<row><td>2363</td><td>##IDS_ERROR_2363##</td></row>
		<row><td>2364</td><td>##IDS_ERROR_2364##</td></row>
		<row><td>2365</td><td>##IDS_ERROR_2365##</td></row>
		<row><td>2366</td><td>##IDS_ERROR_2366##</td></row>
		<row><td>2367</td><td>##IDS_ERROR_2367##</td></row>
		<row><td>2368</td><td>##IDS_ERROR_2368##</td></row>
		<row><td>2370</td><td>##IDS_ERROR_2370##</td></row>
		<row><td>2371</td><td>##IDS_ERROR_2371##</td></row>
		<row><td>2372</td><td>##IDS_ERROR_2372##</td></row>
		<row><td>2373</td><td>##IDS_ERROR_2373##</td></row>
		<row><td>2374</td><td>##IDS_ERROR_2374##</td></row>
		<row><td>2375</td><td>##IDS_ERROR_2375##</td></row>
		<row><td>2376</td><td>##IDS_ERROR_2376##</td></row>
		<row><td>2379</td><td>##IDS_ERROR_2379##</td></row>
		<row><td>2380</td><td>##IDS_ERROR_2380##</td></row>
		<row><td>2381</td><td>##IDS_ERROR_2381##</td></row>
		<row><td>2382</td><td>##IDS_ERROR_2382##</td></row>
		<row><td>2401</td><td>##IDS_ERROR_2401##</td></row>
		<row><td>2402</td><td>##IDS_ERROR_2402##</td></row>
		<row><td>2501</td><td>##IDS_ERROR_2501##</td></row>
		<row><td>2502</td><td>##IDS_ERROR_2502##</td></row>
		<row><td>2503</td><td>##IDS_ERROR_2503##</td></row>
		<row><td>2601</td><td>##IDS_ERROR_2601##</td></row>
		<row><td>2602</td><td>##IDS_ERROR_2602##</td></row>
		<row><td>2603</td><td>##IDS_ERROR_2603##</td></row>
		<row><td>2604</td><td>##IDS_ERROR_2604##</td></row>
		<row><td>2605</td><td>##IDS_ERROR_2605##</td></row>
		<row><td>2606</td><td>##IDS_ERROR_2606##</td></row>
		<row><td>2607</td><td>##IDS_ERROR_2607##</td></row>
		<row><td>2608</td><td>##IDS_ERROR_2608##</td></row>
		<row><td>2609</td><td>##IDS_ERROR_2609##</td></row>
		<row><td>2611</td><td>##IDS_ERROR_2611##</td></row>
		<row><td>2612</td><td>##IDS_ERROR_2612##</td></row>
		<row><td>2613</td><td>##IDS_ERROR_2613##</td></row>
		<row><td>2614</td><td>##IDS_ERROR_2614##</td></row>
		<row><td>2615</td><td>##IDS_ERROR_2615##</td></row>
		<row><td>2616</td><td>##IDS_ERROR_2616##</td></row>
		<row><td>2617</td><td>##IDS_ERROR_2617##</td></row>
		<row><td>2618</td><td>##IDS_ERROR_2618##</td></row>
		<row><td>2619</td><td>##IDS_ERROR_2619##</td></row>
		<row><td>2620</td><td>##IDS_ERROR_2620##</td></row>
		<row><td>2621</td><td>##IDS_ERROR_2621##</td></row>
		<row><td>2701</td><td>##IDS_ERROR_2701##</td></row>
		<row><td>2702</td><td>##IDS_ERROR_2702##</td></row>
		<row><td>2703</td><td>##IDS_ERROR_2703##</td></row>
		<row><td>2704</td><td>##IDS_ERROR_2704##</td></row>
		<row><td>2705</td><td>##IDS_ERROR_2705##</td></row>
		<row><td>2706</td><td>##IDS_ERROR_2706##</td></row>
		<row><td>2707</td><td>##IDS_ERROR_2707##</td></row>
		<row><td>2708</td><td>##IDS_ERROR_2708##</td></row>
		<row><td>2709</td><td>##IDS_ERROR_2709##</td></row>
		<row><td>2710</td><td>##IDS_ERROR_2710##</td></row>
		<row><td>2711</td><td>##IDS_ERROR_2711##</td></row>
		<row><td>2712</td><td>##IDS_ERROR_2712##</td></row>
		<row><td>2713</td><td>##IDS_ERROR_2713##</td></row>
		<row><td>2714</td><td>##IDS_ERROR_2714##</td></row>
		<row><td>2715</td><td>##IDS_ERROR_2715##</td></row>
		<row><td>2716</td><td>##IDS_ERROR_2716##</td></row>
		<row><td>2717</td><td>##IDS_ERROR_2717##</td></row>
		<row><td>2718</td><td>##IDS_ERROR_2718##</td></row>
		<row><td>2719</td><td>##IDS_ERROR_2719##</td></row>
		<row><td>2720</td><td>##IDS_ERROR_2720##</td></row>
		<row><td>2721</td><td>##IDS_ERROR_2721##</td></row>
		<row><td>2722</td><td>##IDS_ERROR_2722##</td></row>
		<row><td>2723</td><td>##IDS_ERROR_2723##</td></row>
		<row><td>2724</td><td>##IDS_ERROR_2724##</td></row>
		<row><td>2725</td><td>##IDS_ERROR_2725##</td></row>
		<row><td>2726</td><td>##IDS_ERROR_2726##</td></row>
		<row><td>2727</td><td>##IDS_ERROR_2727##</td></row>
		<row><td>2728</td><td>##IDS_ERROR_2728##</td></row>
		<row><td>2729</td><td>##IDS_ERROR_2729##</td></row>
		<row><td>2730</td><td>##IDS_ERROR_2730##</td></row>
		<row><td>2731</td><td>##IDS_ERROR_2731##</td></row>
		<row><td>2732</td><td>##IDS_ERROR_2732##</td></row>
		<row><td>2733</td><td>##IDS_ERROR_2733##</td></row>
		<row><td>2734</td><td>##IDS_ERROR_2734##</td></row>
		<row><td>2735</td><td>##IDS_ERROR_2735##</td></row>
		<row><td>2736</td><td>##IDS_ERROR_2736##</td></row>
		<row><td>2737</td><td>##IDS_ERROR_2737##</td></row>
		<row><td>2738</td><td>##IDS_ERROR_2738##</td></row>
		<row><td>2739</td><td>##IDS_ERROR_2739##</td></row>
		<row><td>2740</td><td>##IDS_ERROR_2740##</td></row>
		<row><td>2741</td><td>##IDS_ERROR_2741##</td></row>
		<row><td>2742</td><td>##IDS_ERROR_2742##</td></row>
		<row><td>2743</td><td>##IDS_ERROR_2743##</td></row>
		<row><td>2744</td><td>##IDS_ERROR_2744##</td></row>
		<row><td>2745</td><td>##IDS_ERROR_2745##</td></row>
		<row><td>2746</td><td>##IDS_ERROR_2746##</td></row>
		<row><td>2747</td><td>##IDS_ERROR_2747##</td></row>
		<row><td>2748</td><td>##IDS_ERROR_2748##</td></row>
		<row><td>2749</td><td>##IDS_ERROR_2749##</td></row>
		<row><td>2750</td><td>##IDS_ERROR_2750##</td></row>
		<row><td>27500</td><td>##IDS_ERROR_130##</td></row>
		<row><td>27501</td><td>##IDS_ERROR_131##</td></row>
		<row><td>27502</td><td>##IDS_ERROR_27502##</td></row>
		<row><td>27503</td><td>##IDS_ERROR_27503##</td></row>
		<row><td>27504</td><td>##IDS_ERROR_27504##</td></row>
		<row><td>27505</td><td>##IDS_ERROR_27505##</td></row>
		<row><td>27506</td><td>##IDS_ERROR_27506##</td></row>
		<row><td>27507</td><td>##IDS_ERROR_27507##</td></row>
		<row><td>27508</td><td>##IDS_ERROR_27508##</td></row>
		<row><td>27509</td><td>##IDS_ERROR_27509##</td></row>
		<row><td>2751</td><td>##IDS_ERROR_2751##</td></row>
		<row><td>27510</td><td>##IDS_ERROR_27510##</td></row>
		<row><td>27511</td><td>##IDS_ERROR_27511##</td></row>
		<row><td>27512</td><td>##IDS_ERROR_27512##</td></row>
		<row><td>27513</td><td>##IDS_ERROR_27513##</td></row>
		<row><td>27514</td><td>##IDS_ERROR_27514##</td></row>
		<row><td>27515</td><td>##IDS_ERROR_27515##</td></row>
		<row><td>27516</td><td>##IDS_ERROR_27516##</td></row>
		<row><td>27517</td><td>##IDS_ERROR_27517##</td></row>
		<row><td>27518</td><td>##IDS_ERROR_27518##</td></row>
		<row><td>27519</td><td>##IDS_ERROR_27519##</td></row>
		<row><td>2752</td><td>##IDS_ERROR_2752##</td></row>
		<row><td>27520</td><td>##IDS_ERROR_27520##</td></row>
		<row><td>27521</td><td>##IDS_ERROR_27521##</td></row>
		<row><td>27522</td><td>##IDS_ERROR_27522##</td></row>
		<row><td>27523</td><td>##IDS_ERROR_27523##</td></row>
		<row><td>27524</td><td>##IDS_ERROR_27524##</td></row>
		<row><td>27525</td><td>##IDS_ERROR_27525##</td></row>
		<row><td>27526</td><td>##IDS_ERROR_27526##</td></row>
		<row><td>27527</td><td>##IDS_ERROR_27527##</td></row>
		<row><td>27528</td><td>##IDS_ERROR_27528##</td></row>
		<row><td>27529</td><td>##IDS_ERROR_27529##</td></row>
		<row><td>2753</td><td>##IDS_ERROR_2753##</td></row>
		<row><td>27530</td><td>##IDS_ERROR_27530##</td></row>
		<row><td>27531</td><td>##IDS_ERROR_27531##</td></row>
		<row><td>27532</td><td>##IDS_ERROR_27532##</td></row>
		<row><td>27533</td><td>##IDS_ERROR_27533##</td></row>
		<row><td>27534</td><td>##IDS_ERROR_27534##</td></row>
		<row><td>27535</td><td>##IDS_ERROR_27535##</td></row>
		<row><td>27536</td><td>##IDS_ERROR_27536##</td></row>
		<row><td>27537</td><td>##IDS_ERROR_27537##</td></row>
		<row><td>27538</td><td>##IDS_ERROR_27538##</td></row>
		<row><td>27539</td><td>##IDS_ERROR_27539##</td></row>
		<row><td>2754</td><td>##IDS_ERROR_2754##</td></row>
		<row><td>27540</td><td>##IDS_ERROR_27540##</td></row>
		<row><td>27541</td><td>##IDS_ERROR_27541##</td></row>
		<row><td>27542</td><td>##IDS_ERROR_27542##</td></row>
		<row><td>27543</td><td>##IDS_ERROR_27543##</td></row>
		<row><td>27544</td><td>##IDS_ERROR_27544##</td></row>
		<row><td>27545</td><td>##IDS_ERROR_27545##</td></row>
		<row><td>27546</td><td>##IDS_ERROR_27546##</td></row>
		<row><td>27547</td><td>##IDS_ERROR_27547##</td></row>
		<row><td>27548</td><td>##IDS_ERROR_27548##</td></row>
		<row><td>27549</td><td>##IDS_ERROR_27549##</td></row>
		<row><td>2755</td><td>##IDS_ERROR_2755##</td></row>
		<row><td>27550</td><td>##IDS_ERROR_27550##</td></row>
		<row><td>27551</td><td>##IDS_ERROR_27551##</td></row>
		<row><td>27552</td><td>##IDS_ERROR_27552##</td></row>
		<row><td>27553</td><td>##IDS_ERROR_27553##</td></row>
		<row><td>27554</td><td>##IDS_ERROR_27554##</td></row>
		<row><td>27555</td><td>##IDS_ERROR_27555##</td></row>
		<row><td>2756</td><td>##IDS_ERROR_2756##</td></row>
		<row><td>2757</td><td>##IDS_ERROR_2757##</td></row>
		<row><td>2758</td><td>##IDS_ERROR_2758##</td></row>
		<row><td>2759</td><td>##IDS_ERROR_2759##</td></row>
		<row><td>2760</td><td>##IDS_ERROR_2760##</td></row>
		<row><td>2761</td><td>##IDS_ERROR_2761##</td></row>
		<row><td>2762</td><td>##IDS_ERROR_2762##</td></row>
		<row><td>2763</td><td>##IDS_ERROR_2763##</td></row>
		<row><td>2765</td><td>##IDS_ERROR_2765##</td></row>
		<row><td>2766</td><td>##IDS_ERROR_2766##</td></row>
		<row><td>2767</td><td>##IDS_ERROR_2767##</td></row>
		<row><td>2768</td><td>##IDS_ERROR_2768##</td></row>
		<row><td>2769</td><td>##IDS_ERROR_2769##</td></row>
		<row><td>2770</td><td>##IDS_ERROR_2770##</td></row>
		<row><td>2771</td><td>##IDS_ERROR_2771##</td></row>
		<row><td>2772</td><td>##IDS_ERROR_2772##</td></row>
		<row><td>2801</td><td>##IDS_ERROR_2801##</td></row>
		<row><td>2802</td><td>##IDS_ERROR_2802##</td></row>
		<row><td>2803</td><td>##IDS_ERROR_2803##</td></row>
		<row><td>2804</td><td>##IDS_ERROR_2804##</td></row>
		<row><td>2806</td><td>##IDS_ERROR_2806##</td></row>
		<row><td>2807</td><td>##IDS_ERROR_2807##</td></row>
		<row><td>2808</td><td>##IDS_ERROR_2808##</td></row>
		<row><td>2809</td><td>##IDS_ERROR_2809##</td></row>
		<row><td>2810</td><td>##IDS_ERROR_2810##</td></row>
		<row><td>2811</td><td>##IDS_ERROR_2811##</td></row>
		<row><td>2812</td><td>##IDS_ERROR_2812##</td></row>
		<row><td>2813</td><td>##IDS_ERROR_2813##</td></row>
		<row><td>2814</td><td>##IDS_ERROR_2814##</td></row>
		<row><td>2815</td><td>##IDS_ERROR_2815##</td></row>
		<row><td>2816</td><td>##IDS_ERROR_2816##</td></row>
		<row><td>2817</td><td>##IDS_ERROR_2817##</td></row>
		<row><td>2818</td><td>##IDS_ERROR_2818##</td></row>
		<row><td>2819</td><td>##IDS_ERROR_2819##</td></row>
		<row><td>2820</td><td>##IDS_ERROR_2820##</td></row>
		<row><td>2821</td><td>##IDS_ERROR_2821##</td></row>
		<row><td>2822</td><td>##IDS_ERROR_2822##</td></row>
		<row><td>2823</td><td>##IDS_ERROR_2823##</td></row>
		<row><td>2824</td><td>##IDS_ERROR_2824##</td></row>
		<row><td>2825</td><td>##IDS_ERROR_2825##</td></row>
		<row><td>2826</td><td>##IDS_ERROR_2826##</td></row>
		<row><td>2827</td><td>##IDS_ERROR_2827##</td></row>
		<row><td>2828</td><td>##IDS_ERROR_2828##</td></row>
		<row><td>2829</td><td>##IDS_ERROR_2829##</td></row>
		<row><td>2830</td><td>##IDS_ERROR_2830##</td></row>
		<row><td>2831</td><td>##IDS_ERROR_2831##</td></row>
		<row><td>2832</td><td>##IDS_ERROR_2832##</td></row>
		<row><td>2833</td><td>##IDS_ERROR_2833##</td></row>
		<row><td>2834</td><td>##IDS_ERROR_2834##</td></row>
		<row><td>2835</td><td>##IDS_ERROR_2835##</td></row>
		<row><td>2836</td><td>##IDS_ERROR_2836##</td></row>
		<row><td>2837</td><td>##IDS_ERROR_2837##</td></row>
		<row><td>2838</td><td>##IDS_ERROR_2838##</td></row>
		<row><td>2839</td><td>##IDS_ERROR_2839##</td></row>
		<row><td>2840</td><td>##IDS_ERROR_2840##</td></row>
		<row><td>2841</td><td>##IDS_ERROR_2841##</td></row>
		<row><td>2842</td><td>##IDS_ERROR_2842##</td></row>
		<row><td>2843</td><td>##IDS_ERROR_2843##</td></row>
		<row><td>2844</td><td>##IDS_ERROR_2844##</td></row>
		<row><td>2845</td><td>##IDS_ERROR_2845##</td></row>
		<row><td>2846</td><td>##IDS_ERROR_2846##</td></row>
		<row><td>2847</td><td>##IDS_ERROR_2847##</td></row>
		<row><td>2848</td><td>##IDS_ERROR_2848##</td></row>
		<row><td>2849</td><td>##IDS_ERROR_2849##</td></row>
		<row><td>2850</td><td>##IDS_ERROR_2850##</td></row>
		<row><td>2851</td><td>##IDS_ERROR_2851##</td></row>
		<row><td>2852</td><td>##IDS_ERROR_2852##</td></row>
		<row><td>2853</td><td>##IDS_ERROR_2853##</td></row>
		<row><td>2854</td><td>##IDS_ERROR_2854##</td></row>
		<row><td>2855</td><td>##IDS_ERROR_2855##</td></row>
		<row><td>2856</td><td>##IDS_ERROR_2856##</td></row>
		<row><td>2857</td><td>##IDS_ERROR_2857##</td></row>
		<row><td>2858</td><td>##IDS_ERROR_2858##</td></row>
		<row><td>2859</td><td>##IDS_ERROR_2859##</td></row>
		<row><td>2860</td><td>##IDS_ERROR_2860##</td></row>
		<row><td>2861</td><td>##IDS_ERROR_2861##</td></row>
		<row><td>2862</td><td>##IDS_ERROR_2862##</td></row>
		<row><td>2863</td><td>##IDS_ERROR_2863##</td></row>
		<row><td>2864</td><td>##IDS_ERROR_2864##</td></row>
		<row><td>2865</td><td>##IDS_ERROR_2865##</td></row>
		<row><td>2866</td><td>##IDS_ERROR_2866##</td></row>
		<row><td>2867</td><td>##IDS_ERROR_2867##</td></row>
		<row><td>2868</td><td>##IDS_ERROR_2868##</td></row>
		<row><td>2869</td><td>##IDS_ERROR_2869##</td></row>
		<row><td>2870</td><td>##IDS_ERROR_2870##</td></row>
		<row><td>2871</td><td>##IDS_ERROR_2871##</td></row>
		<row><td>2872</td><td>##IDS_ERROR_2872##</td></row>
		<row><td>2873</td><td>##IDS_ERROR_2873##</td></row>
		<row><td>2874</td><td>##IDS_ERROR_2874##</td></row>
		<row><td>2875</td><td>##IDS_ERROR_2875##</td></row>
		<row><td>2876</td><td>##IDS_ERROR_2876##</td></row>
		<row><td>2877</td><td>##IDS_ERROR_2877##</td></row>
		<row><td>2878</td><td>##IDS_ERROR_2878##</td></row>
		<row><td>2879</td><td>##IDS_ERROR_2879##</td></row>
		<row><td>2880</td><td>##IDS_ERROR_2880##</td></row>
		<row><td>2881</td><td>##IDS_ERROR_2881##</td></row>
		<row><td>2882</td><td>##IDS_ERROR_2882##</td></row>
		<row><td>2883</td><td>##IDS_ERROR_2883##</td></row>
		<row><td>2884</td><td>##IDS_ERROR_2884##</td></row>
		<row><td>2885</td><td>##IDS_ERROR_2885##</td></row>
		<row><td>2886</td><td>##IDS_ERROR_2886##</td></row>
		<row><td>2887</td><td>##IDS_ERROR_2887##</td></row>
		<row><td>2888</td><td>##IDS_ERROR_2888##</td></row>
		<row><td>2889</td><td>##IDS_ERROR_2889##</td></row>
		<row><td>2890</td><td>##IDS_ERROR_2890##</td></row>
		<row><td>2891</td><td>##IDS_ERROR_2891##</td></row>
		<row><td>2892</td><td>##IDS_ERROR_2892##</td></row>
		<row><td>2893</td><td>##IDS_ERROR_2893##</td></row>
		<row><td>2894</td><td>##IDS_ERROR_2894##</td></row>
		<row><td>2895</td><td>##IDS_ERROR_2895##</td></row>
		<row><td>2896</td><td>##IDS_ERROR_2896##</td></row>
		<row><td>2897</td><td>##IDS_ERROR_2897##</td></row>
		<row><td>2898</td><td>##IDS_ERROR_2898##</td></row>
		<row><td>2899</td><td>##IDS_ERROR_2899##</td></row>
		<row><td>2901</td><td>##IDS_ERROR_2901##</td></row>
		<row><td>2902</td><td>##IDS_ERROR_2902##</td></row>
		<row><td>2903</td><td>##IDS_ERROR_2903##</td></row>
		<row><td>2904</td><td>##IDS_ERROR_2904##</td></row>
		<row><td>2905</td><td>##IDS_ERROR_2905##</td></row>
		<row><td>2906</td><td>##IDS_ERROR_2906##</td></row>
		<row><td>2907</td><td>##IDS_ERROR_2907##</td></row>
		<row><td>2908</td><td>##IDS_ERROR_2908##</td></row>
		<row><td>2909</td><td>##IDS_ERROR_2909##</td></row>
		<row><td>2910</td><td>##IDS_ERROR_2910##</td></row>
		<row><td>2911</td><td>##IDS_ERROR_2911##</td></row>
		<row><td>2912</td><td>##IDS_ERROR_2912##</td></row>
		<row><td>2919</td><td>##IDS_ERROR_2919##</td></row>
		<row><td>2920</td><td>##IDS_ERROR_2920##</td></row>
		<row><td>2924</td><td>##IDS_ERROR_2924##</td></row>
		<row><td>2927</td><td>##IDS_ERROR_2927##</td></row>
		<row><td>2928</td><td>##IDS_ERROR_2928##</td></row>
		<row><td>2929</td><td>##IDS_ERROR_2929##</td></row>
		<row><td>2932</td><td>##IDS_ERROR_2932##</td></row>
		<row><td>2933</td><td>##IDS_ERROR_2933##</td></row>
		<row><td>2934</td><td>##IDS_ERROR_2934##</td></row>
		<row><td>2935</td><td>##IDS_ERROR_2935##</td></row>
		<row><td>2936</td><td>##IDS_ERROR_2936##</td></row>
		<row><td>2937</td><td>##IDS_ERROR_2937##</td></row>
		<row><td>2938</td><td>##IDS_ERROR_2938##</td></row>
		<row><td>2939</td><td>##IDS_ERROR_2939##</td></row>
		<row><td>2940</td><td>##IDS_ERROR_2940##</td></row>
		<row><td>2941</td><td>##IDS_ERROR_2941##</td></row>
		<row><td>2942</td><td>##IDS_ERROR_2942##</td></row>
		<row><td>2943</td><td>##IDS_ERROR_2943##</td></row>
		<row><td>2944</td><td>##IDS_ERROR_2944##</td></row>
		<row><td>2945</td><td>##IDS_ERROR_2945##</td></row>
		<row><td>3001</td><td>##IDS_ERROR_3001##</td></row>
		<row><td>3002</td><td>##IDS_ERROR_3002##</td></row>
		<row><td>32</td><td>##IDS_ERROR_20##</td></row>
		<row><td>33</td><td>##IDS_ERROR_21##</td></row>
		<row><td>4</td><td>##IDS_ERROR_3##</td></row>
		<row><td>5</td><td>##IDS_ERROR_4##</td></row>
		<row><td>7</td><td>##IDS_ERROR_5##</td></row>
		<row><td>8</td><td>##IDS_ERROR_6##</td></row>
		<row><td>9</td><td>##IDS_ERROR_7##</td></row>
	</table>

	<table name="EventMapping">
		<col key="yes" def="s72">Dialog_</col>
		<col key="yes" def="s50">Control_</col>
		<col key="yes" def="s50">Event</col>
		<col def="s50">Attribute</col>
		<row><td>CustomSetup</td><td>ItemDescription</td><td>SelectionDescription</td><td>Text</td></row>
		<row><td>CustomSetup</td><td>Location</td><td>SelectionPath</td><td>Text</td></row>
		<row><td>CustomSetup</td><td>Size</td><td>SelectionSize</td><td>Text</td></row>
		<row><td>SetupInitialization</td><td>ActionData</td><td>ActionData</td><td>Text</td></row>
		<row><td>SetupInitialization</td><td>ActionText</td><td>ActionText</td><td>Text</td></row>
		<row><td>SetupProgress</td><td>ActionProgress95</td><td>AdminInstallFinalize</td><td>Progress</td></row>
		<row><td>SetupProgress</td><td>ActionProgress95</td><td>InstallFiles</td><td>Progress</td></row>
		<row><td>SetupProgress</td><td>ActionProgress95</td><td>MoveFiles</td><td>Progress</td></row>
		<row><td>SetupProgress</td><td>ActionProgress95</td><td>RemoveFiles</td><td>Progress</td></row>
		<row><td>SetupProgress</td><td>ActionProgress95</td><td>RemoveRegistryValues</td><td>Progress</td></row>
		<row><td>SetupProgress</td><td>ActionProgress95</td><td>SetProgress</td><td>Progress</td></row>
		<row><td>SetupProgress</td><td>ActionProgress95</td><td>UnmoveFiles</td><td>Progress</td></row>
		<row><td>SetupProgress</td><td>ActionProgress95</td><td>WriteIniValues</td><td>Progress</td></row>
		<row><td>SetupProgress</td><td>ActionProgress95</td><td>WriteRegistryValues</td><td>Progress</td></row>
		<row><td>SetupProgress</td><td>ActionText</td><td>ActionText</td><td>Text</td></row>
	</table>

	<table name="Extension">
		<col key="yes" def="s255">Extension</col>
		<col key="yes" def="s72">Component_</col>
		<col def="S255">ProgId_</col>
		<col def="S64">MIME_</col>
		<col def="s38">Feature_</col>
		<row><td>NCM</td><td>C__5AB0F74B62D24B2797103046CCAAD98E</td><td>NCManager</td><td/><td>AlwaysInstall</td></row>
		<row><td>NCMJ</td><td>C__5AB0F74B62D24B2797103046CCAAD98E</td><td>NCManager job file</td><td/><td>AlwaysInstall</td></row>
	</table>

	<table name="Feature">
		<col key="yes" def="s38">Feature</col>
		<col def="S38">Feature_Parent</col>
		<col def="L64">Title</col>
		<col def="L255">Description</col>
		<col def="I2">Display</col>
		<col def="i2">Level</col>
		<col def="S72">Directory_</col>
		<col def="i2">Attributes</col>
		<col def="S255">ISReleaseFlags</col>
		<col def="S255">ISComments</col>
		<col def="S255">ISFeatureCabName</col>
		<col def="S255">ISProFeatureName</col>
		<row><td>AlwaysInstall</td><td/><td>##DN_AlwaysInstall##</td><td>Enter the description for this feature here.</td><td>0</td><td>1</td><td>INSTALLDIR</td><td>16</td><td/><td>Enter comments regarding this feature here.</td><td/><td/></row>
	</table>

	<table name="FeatureComponents">
		<col key="yes" def="s38">Feature_</col>
		<col key="yes" def="s72">Component_</col>
		<row><td>AlwaysInstall</td><td>C__082C4A2B51334D24A3AC6790D6C8512B</td></row>
		<row><td>AlwaysInstall</td><td>C__08B0E42E0AE7406A98786249EDB2B56E</td></row>
		<row><td>AlwaysInstall</td><td>C__0BBF4D1B61464202AA9E41FF332450DB</td></row>
		<row><td>AlwaysInstall</td><td>C__122CBDCF2BDA4DE9881ADCC656F030C1</td></row>
		<row><td>AlwaysInstall</td><td>C__127D62C316F2443D94ACD0038D656836</td></row>
		<row><td>AlwaysInstall</td><td>C__1346B53B2129406687E96EC782FD6B14</td></row>
		<row><td>AlwaysInstall</td><td>C__137C441D09AE4FB5B9C67421B48492C1</td></row>
		<row><td>AlwaysInstall</td><td>C__14EF4C63E3B5454D8D5A69C3C590EB86</td></row>
		<row><td>AlwaysInstall</td><td>C__1615A4B06DC04A4BAF33E4CA968A4EF4</td></row>
		<row><td>AlwaysInstall</td><td>C__1C07066DA2C8474AB22F5246BE023333</td></row>
		<row><td>AlwaysInstall</td><td>C__1D5D687FCC234E39ADEACC9EFBBD2AB7</td></row>
		<row><td>AlwaysInstall</td><td>C__25E09A9CCD224D31881BE31AB87732A3</td></row>
		<row><td>AlwaysInstall</td><td>C__29C12E55443848608C72F3AD69AAB824</td></row>
		<row><td>AlwaysInstall</td><td>C__2A0D3AF6F7264C9A8D50E89172C4A81E</td></row>
		<row><td>AlwaysInstall</td><td>C__31F1DADB0D4C4DB1A4A4487740F0A412</td></row>
		<row><td>AlwaysInstall</td><td>C__34CD49E7DDF94F629770F9F61A0F4957</td></row>
		<row><td>AlwaysInstall</td><td>C__40FF87838DE4496EBC8A597169C87209</td></row>
		<row><td>AlwaysInstall</td><td>C__423104302EE74832984738AC1F76C5AE</td></row>
		<row><td>AlwaysInstall</td><td>C__428099970CD44B46A98E2226D653EE63</td></row>
		<row><td>AlwaysInstall</td><td>C__431D7DF568F44C0499C79B40A1BF6E12</td></row>
		<row><td>AlwaysInstall</td><td>C__4BCBDC7592FA4B638AD6631DDD7D122A</td></row>
		<row><td>AlwaysInstall</td><td>C__4D66453340ED4B598F05FE3CAD8329D4</td></row>
		<row><td>AlwaysInstall</td><td>C__4F10DC0F614541B6ADC8623FE3862D1C</td></row>
		<row><td>AlwaysInstall</td><td>C__4F4DA107785E44E581213838B3497E30</td></row>
		<row><td>AlwaysInstall</td><td>C__578DC43FDCB1484794895CECAEE03768</td></row>
		<row><td>AlwaysInstall</td><td>C__5AB0F74B62D24B2797103046CCAAD98E</td></row>
		<row><td>AlwaysInstall</td><td>C__5C6E7AA0836C41DB831039A34D3D7AA0</td></row>
		<row><td>AlwaysInstall</td><td>C__5CB24DDE141142B7B0F65A0B5670CFB3</td></row>
		<row><td>AlwaysInstall</td><td>C__5F2121D4FBDE491F8A7129617E991A99</td></row>
		<row><td>AlwaysInstall</td><td>C__6185CEAD036144B69828C2BA1357F7C7</td></row>
		<row><td>AlwaysInstall</td><td>C__70BD166A8637415AB595011E5C8869C5</td></row>
		<row><td>AlwaysInstall</td><td>C__70DF0F53D1CE41E184D5BC1D21498AA9</td></row>
		<row><td>AlwaysInstall</td><td>C__72CE03E984304A3F8369648DA0BBEC90</td></row>
		<row><td>AlwaysInstall</td><td>C__72E841FC40C949838CF7B48362C035BC</td></row>
		<row><td>AlwaysInstall</td><td>C__76A73657BC264AD09EEDE2B6EB407B17</td></row>
		<row><td>AlwaysInstall</td><td>C__8306BB83A4434E08BC8580B175683D54</td></row>
		<row><td>AlwaysInstall</td><td>C__830F99D2CA044919B0C0507A0D17980E</td></row>
		<row><td>AlwaysInstall</td><td>C__843F7632BEC24FBE8BF7C63DEFBF0687</td></row>
		<row><td>AlwaysInstall</td><td>C__9013468A11E6405391A6F141B889603B</td></row>
		<row><td>AlwaysInstall</td><td>C__92867AD4217F4FD89E7D44B5A0C02773</td></row>
		<row><td>AlwaysInstall</td><td>C__96B0F2B1BDAF40E2AB16F98C98A045A8</td></row>
		<row><td>AlwaysInstall</td><td>C__984DB5E2EFED4B68AFC611CCD78EE906</td></row>
		<row><td>AlwaysInstall</td><td>C__9CC9C8957FF0430099D06B9A6E37CB44</td></row>
		<row><td>AlwaysInstall</td><td>C__A1C4795563494325A174FC7F2F7E25CE</td></row>
		<row><td>AlwaysInstall</td><td>C__A24AF738FF7547978A68111A0B90EF5E</td></row>
		<row><td>AlwaysInstall</td><td>C__A29FEDE025A24BEC89F0EAC20DAEA4CD</td></row>
		<row><td>AlwaysInstall</td><td>C__A2A8E787524045DBA36D5BDB7A77E13F</td></row>
		<row><td>AlwaysInstall</td><td>C__A2DED2051612450A9E2962309C61F19C</td></row>
		<row><td>AlwaysInstall</td><td>C__A473537B6DB04D899D6E1FD1979FA3CF</td></row>
		<row><td>AlwaysInstall</td><td>C__A530EDAA5E624663ABB83D59D42186F6</td></row>
		<row><td>AlwaysInstall</td><td>C__A6AFAFB1B104453FADE31BE15EA14353</td></row>
		<row><td>AlwaysInstall</td><td>C__B099E4926AFE43EAB395438D0D8D9B77</td></row>
		<row><td>AlwaysInstall</td><td>C__B39748BC31D6422EBB6B2D72123E92AA</td></row>
		<row><td>AlwaysInstall</td><td>C__BBCC268121A94302A63046EF90320A3A</td></row>
		<row><td>AlwaysInstall</td><td>C__C230765F762B47B18C7DBED550986ABA</td></row>
		<row><td>AlwaysInstall</td><td>C__C3C4913F460544FE9F4C0E2E900DB356</td></row>
		<row><td>AlwaysInstall</td><td>C__C403BA6E9E194CD592A7C50EDE7E939B</td></row>
		<row><td>AlwaysInstall</td><td>C__D1F8CF020B5A451697624F490F28BE54</td></row>
		<row><td>AlwaysInstall</td><td>C__D3758339EA714A17984DE22F4644FEE9</td></row>
		<row><td>AlwaysInstall</td><td>C__D47657BBBCC549C9A4DA838A1DCD2DF4</td></row>
		<row><td>AlwaysInstall</td><td>C__DB2EB634C1AB4C1FA228A22582375EA2</td></row>
		<row><td>AlwaysInstall</td><td>C__E22EF8970F184A29A67C6B5080E0E021</td></row>
		<row><td>AlwaysInstall</td><td>C__ECA4F69504D14AFCA9E7F72A8D3B7013</td></row>
		<row><td>AlwaysInstall</td><td>C__EFD5EB60662546EF86AC8E0A611C4CA6</td></row>
		<row><td>AlwaysInstall</td><td>C__F2555C2B1D7C49559BC04823656E2305</td></row>
		<row><td>AlwaysInstall</td><td>C__F3D05398C3BA40C69E8709F43B4E3083</td></row>
		<row><td>AlwaysInstall</td><td>C__F4112FDF846B48FB86CE69A0545E366F</td></row>
		<row><td>AlwaysInstall</td><td>C__F58DFAEFADA3442B9526AA9947119E11</td></row>
		<row><td>AlwaysInstall</td><td>C__FAF9F28B10E04850937D47023FEB058A</td></row>
		<row><td>AlwaysInstall</td><td>C__FEB18844F3D54D3CBA46FE552A79F646</td></row>
		<row><td>AlwaysInstall</td><td>CompBase.dll</td></row>
		<row><td>AlwaysInstall</td><td>ExprEval.dll</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT1</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT10</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT11</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT12</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT13</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT14</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT15</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT16</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT17</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT18</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT19</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT2</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT20</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT21</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT22</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT23</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT24</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT25</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT26</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT27</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT28</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT29</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT3</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT30</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT31</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT32</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT33</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT35</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT36</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT37</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT38</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT39</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT4</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT40</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT41</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT42</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT43</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT5</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT6</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT7</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT8</td></row>
		<row><td>AlwaysInstall</td><td>ISX_DEFAULTCOMPONENT9</td></row>
		<row><td>AlwaysInstall</td><td>NCMHandlers.dll</td></row>
		<row><td>AlwaysInstall</td><td>NCManager.chm</td></row>
		<row><td>AlwaysInstall</td><td>NCManager.chm1</td></row>
		<row><td>AlwaysInstall</td><td>OCImport.dll</td></row>
		<row><td>AlwaysInstall</td><td>TKBO.dll</td></row>
		<row><td>AlwaysInstall</td><td>TKBRep.dll</td></row>
		<row><td>AlwaysInstall</td><td>TKBool.dll</td></row>
		<row><td>AlwaysInstall</td><td>TKG2d.dll</td></row>
		<row><td>AlwaysInstall</td><td>TKG3d.dll</td></row>
		<row><td>AlwaysInstall</td><td>TKGeomAlgo.dll</td></row>
		<row><td>AlwaysInstall</td><td>TKGeomBase.dll</td></row>
		<row><td>AlwaysInstall</td><td>TKIGES.dll</td></row>
		<row><td>AlwaysInstall</td><td>TKMath.dll</td></row>
		<row><td>AlwaysInstall</td><td>TKMesh.dll</td></row>
		<row><td>AlwaysInstall</td><td>TKPrim.dll</td></row>
		<row><td>AlwaysInstall</td><td>TKSTEP.dll</td></row>
		<row><td>AlwaysInstall</td><td>TKSTEP209.dll</td></row>
		<row><td>AlwaysInstall</td><td>TKSTEPAttr.dll</td></row>
		<row><td>AlwaysInstall</td><td>TKSTEPBase.dll</td></row>
		<row><td>AlwaysInstall</td><td>TKSTL.dll</td></row>
		<row><td>AlwaysInstall</td><td>TKShHealing.dll</td></row>
		<row><td>AlwaysInstall</td><td>TKTopAlgo.dll</td></row>
		<row><td>AlwaysInstall</td><td>TKXSBase.dll</td></row>
		<row><td>AlwaysInstall</td><td>TKernel.dll</td></row>
		<row><td>AlwaysInstall</td><td>header.dll</td></row>
		<row><td>AlwaysInstall</td><td>zlibwapi.dll</td></row>
	</table>

	<table name="File">
		<col key="yes" def="s72">File</col>
		<col def="s72">Component_</col>
		<col def="s255">FileName</col>
		<col def="i4">FileSize</col>
		<col def="S72">Version</col>
		<col def="S20">Language</col>
		<col def="I2">Attributes</col>
		<col def="i2">Sequence</col>
		<col def="S255">ISBuildSourcePath</col>
		<col def="I4">ISAttributes</col>
		<col def="S72">ISComponentSubFolder_</col>
		<row><td>File</td><td>ISX_DEFAULTCOMPONENT40</td><td>F427~1.PDF|РуководствоПользователяТекущее.pdf</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\Doc\РуководствоПользователяТекущее.pdf</td><td>1</td><td/></row>
		<row><td>File.pdf</td><td>ISX_DEFAULTCOMPONENT40</td><td>0072~1.PDF|КомандыРиббонИнтерфейса.pdf</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\Doc\КомандыРиббонИнтерфейса.pdf</td><td>1</td><td/></row>
		<row><td>File.pdf1</td><td>ISX_DEFAULTCOMPONENT40</td><td>48B57F~1.PDF|Новое в версии 48.pdf</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\Doc\Новое в версии 48.pdf</td><td>1</td><td/></row>
		<row><td>File.pdf2</td><td>ISX_DEFAULTCOMPONENT40</td><td>536B5~1.PDF|Новое в версии 5.pdf</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\Doc\Новое в версии 5.pdf</td><td>1</td><td/></row>
		<row><td>_082C4A2B51334D24A3AC6790D6C8512B</td><td>C__082C4A2B51334D24A3AC6790D6C8512B</td><td>NCM.chm</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Release\local\Rus\NCM.chm</td><td>5</td><td/></row>
		<row><td>_0BBF4D1B61464202AA9E41FF332450DB</td><td>C__0BBF4D1B61464202AA9E41FF332450DB</td><td>SPECUL~1.BMP|specular_c03.bmp</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\specular_c03.bmp</td><td>5</td><td/></row>
		<row><td>_122CBDCF2BDA4DE9881ADCC656F030C1</td><td>C__122CBDCF2BDA4DE9881ADCC656F030C1</td><td>DXFEXP~1.DLL|DXFExport.dll</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\release\DXFExport.dll</td><td>5</td><td/></row>
		<row><td>_127D62C316F2443D94ACD0038D656836</td><td>C__127D62C316F2443D94ACD0038D656836</td><td>G71.ncm</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Demo\Inch\G71.ncm</td><td>5</td><td/></row>
		<row><td>_1346B53B2129406687E96EC782FD6B14</td><td>C__1346B53B2129406687E96EC782FD6B14</td><td>DUAL_P~1.VER|dual_peeling_blend.vert</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\dual_depth_peeling\dual_peeling_blend.vert</td><td>5</td><td/></row>
		<row><td>_137C441D09AE4FB5B9C67421B48492C1</td><td>C__137C441D09AE4FB5B9C67421B48492C1</td><td>NCTool.dll</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\release\NCTool.dll</td><td>5</td><td/></row>
		<row><td>_14EF4C63E3B5454D8D5A69C3C590EB86</td><td>C__14EF4C63E3B5454D8D5A69C3C590EB86</td><td>REMOTE~1.EXE|RemoteControl.exe</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Release\RemoteControl.exe</td><td>5</td><td/></row>
		<row><td>_1615A4B06DC04A4BAF33E4CA968A4EF4</td><td>C__1615A4B06DC04A4BAF33E4CA968A4EF4</td><td>DIFFUS~1.BMP|diffuse_c05.bmp</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\diffuse_c05.bmp</td><td>5</td><td/></row>
		<row><td>_1C07066DA2C8474AB22F5246BE023333</td><td>C__1C07066DA2C8474AB22F5246BE023333</td><td>Machine.dll</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Release\Machine.dll</td><td>5</td><td/></row>
		<row><td>_1D5D687FCC234E39ADEACC9EFBBD2AB7</td><td>C__1D5D687FCC234E39ADEACC9EFBBD2AB7</td><td>PHONG~1.FRA|Phong.frag</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\Phong.frag</td><td>5</td><td/></row>
		<row><td>_25E09A9CCD224D31881BE31AB87732A3</td><td>C__25E09A9CCD224D31881BE31AB87732A3</td><td>G76.ncm</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Demo\Inch\G76.ncm</td><td>5</td><td/></row>
		<row><td>_29C12E55443848608C72F3AD69AAB824</td><td>C__29C12E55443848608C72F3AD69AAB824</td><td>NCMExt.lib</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\lib\Release\NCMExt.lib</td><td>5</td><td/></row>
		<row><td>_31F1DADB0D4C4DB1A4A4487740F0A412</td><td>C__31F1DADB0D4C4DB1A4A4487740F0A412</td><td>NCMInfo.rtf</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Doc\NCMInfo.rtf</td><td>5</td><td/></row>
		<row><td>_34CD49E7DDF94F629770F9F61A0F4957</td><td>C__34CD49E7DDF94F629770F9F61A0F4957</td><td>SPECUL~1.BMP|specular_c04.bmp</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\specular_c04.bmp</td><td>5</td><td/></row>
		<row><td>_40FF87838DE4496EBC8A597169C87209</td><td>C__40FF87838DE4496EBC8A597169C87209</td><td>Compare.dll</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\release\Compare.dll</td><td>5</td><td/></row>
		<row><td>_423104302EE74832984738AC1F76C5AE</td><td>C__423104302EE74832984738AC1F76C5AE</td><td>NCMExt.dll</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Release\NCMExt.dll</td><td>5</td><td/></row>
		<row><td>_428099970CD44B46A98E2226D653EE63</td><td>C__428099970CD44B46A98E2226D653EE63</td><td>JOBDAT~1.RTF|JobdataEng.rtf</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Doc\JobdataEng.rtf</td><td>5</td><td/></row>
		<row><td>_431D7DF568F44C0499C79B40A1BF6E12</td><td>C__431D7DF568F44C0499C79B40A1BF6E12</td><td>SPECUL~1.BMP|specular_c00.bmp</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\specular_c00.bmp</td><td>5</td><td/></row>
		<row><td>_4BCBDC7592FA4B638AD6631DDD7D122A</td><td>C__4BCBDC7592FA4B638AD6631DDD7D122A</td><td>DIFFUS~1.BMP|diffuse_c02.bmp</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\diffuse_c02.bmp</td><td>5</td><td/></row>
		<row><td>_4D66453340ED4B598F05FE3CAD8329D4</td><td>C__4D66453340ED4B598F05FE3CAD8329D4</td><td>MILLPL~1.NCM|MILLPLUS_INCH.ncm</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Demo\Inch\MILLPLUS_INCH.ncm</td><td>5</td><td/></row>
		<row><td>_4F10DC0F614541B6ADC8623FE3862D1C</td><td>C__4F10DC0F614541B6ADC8623FE3862D1C</td><td>UNITeng.RTF</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Doc\UNITeng.RTF</td><td>5</td><td/></row>
		<row><td>_4F4DA107785E44E581213838B3497E30</td><td>C__4F4DA107785E44E581213838B3497E30</td><td>SPECUL~1.BMP|specular_c02.bmp</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\specular_c02.bmp</td><td>5</td><td/></row>
		<row><td>_578DC43FDCB1484794895CECAEE03768</td><td>C__578DC43FDCB1484794895CECAEE03768</td><td>NETCON~1.EXE|NetConfigEdit.exe</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Release\NetConfigEdit.exe</td><td>5</td><td/></row>
		<row><td>_5AB0F74B62D24B2797103046CCAAD98E</td><td>C__5AB0F74B62D24B2797103046CCAAD98E</td><td>NCM30.exe</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\release\NCM30.exe</td><td>5</td><td/></row>
		<row><td>_5C6E7AA0836C41DB831039A34D3D7AA0</td><td>C__5C6E7AA0836C41DB831039A34D3D7AA0</td><td>UNITrus.RTF</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Doc\UNITrus.RTF</td><td>5</td><td/></row>
		<row><td>_5CB24DDE141142B7B0F65A0B5670CFB3</td><td>C__5CB24DDE141142B7B0F65A0B5670CFB3</td><td>GLIT~1.FRA|Glit.frag</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\Glit.frag</td><td>5</td><td/></row>
		<row><td>_5F2121D4FBDE491F8A7129617E991A99</td><td>C__5F2121D4FBDE491F8A7129617E991A99</td><td>Editor.dll</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\release\Editor.dll</td><td>5</td><td/></row>
		<row><td>_6185CEAD036144B69828C2BA1357F7C7</td><td>C__6185CEAD036144B69828C2BA1357F7C7</td><td>NCMb.dll</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\release\NCMb.dll</td><td>5</td><td/></row>
		<row><td>_70BD166A8637415AB595011E5C8869C5</td><td>C__70BD166A8637415AB595011E5C8869C5</td><td>Surface.dll</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\release\Surface.dll</td><td>5</td><td/></row>
		<row><td>_70DF0F53D1CE41E184D5BC1D21498AA9</td><td>C__70DF0F53D1CE41E184D5BC1D21498AA9</td><td>gnclient.ini</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\gnclient.ini</td><td>5</td><td/></row>
		<row><td>_72CE03E984304A3F8369648DA0BBEC90</td><td>C__72CE03E984304A3F8369648DA0BBEC90</td><td>DIFFUS~1.BMP|diffuse_c01.bmp</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\diffuse_c01.bmp</td><td>5</td><td/></row>
		<row><td>_72E841FC40C949838CF7B48362C035BC</td><td>C__72E841FC40C949838CF7B48362C035BC</td><td>DIFFUS~1.BMP|diffuse_c00.bmp</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\diffuse_c00.bmp</td><td>5</td><td/></row>
		<row><td>_76A73657BC264AD09EEDE2B6EB407B17</td><td>C__76A73657BC264AD09EEDE2B6EB407B17</td><td>RESOUR~1.DLL|Resources.dll</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Release\local\Eng\Resources.dll</td><td>5</td><td/></row>
		<row><td>_8306BB83A4434E08BC8580B175683D54</td><td>C__8306BB83A4434E08BC8580B175683D54</td><td>ResInv.dll</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Release\ResInv.dll</td><td>5</td><td/></row>
		<row><td>_830F99D2CA044919B0C0507A0D17980E</td><td>C__830F99D2CA044919B0C0507A0D17980E</td><td>DUAL_P~1.FRA|dual_peeling_peel.frag</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\dual_depth_peeling\dual_peeling_peel.frag</td><td>5</td><td/></row>
		<row><td>_843F7632BEC24FBE8BF7C63DEFBF0687</td><td>C__843F7632BEC24FBE8BF7C63DEFBF0687</td><td>DIFFUS~1.BMP|diffuse_c03.bmp</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\diffuse_c03.bmp</td><td>5</td><td/></row>
		<row><td>_9013468A11E6405391A6F141B889603B</td><td>C__9013468A11E6405391A6F141B889603B</td><td>scilexer.dll</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\release\scilexer.dll</td><td>5</td><td/></row>
		<row><td>_92867AD4217F4FD89E7D44B5A0C02773</td><td>C__92867AD4217F4FD89E7D44B5A0C02773</td><td>DEMOFR~1.NCM|DemoFromEditNC.ncm</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Demo\Inch\DemoFromEditNC.ncm</td><td>5</td><td/></row>
		<row><td>_96B0F2B1BDAF40E2AB16F98C98A045A8</td><td>C__96B0F2B1BDAF40E2AB16F98C98A045A8</td><td>NCMInfo.dll</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\release\NCMInfo.dll</td><td>5</td><td/></row>
		<row><td>_984DB5E2EFED4B68AFC611CCD78EE906</td><td>C__984DB5E2EFED4B68AFC611CCD78EE906</td><td>COLLIS~1.DLL|Collision.dll</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Release\Collision.dll</td><td>5</td><td/></row>
		<row><td>_9CC9C8957FF0430099D06B9A6E37CB44</td><td>C__9CC9C8957FF0430099D06B9A6E37CB44</td><td>Gasket.ncm</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Demo\Inch\Gasket.ncm</td><td>5</td><td/></row>
		<row><td>_A1C4795563494325A174FC7F2F7E25CE</td><td>C__A1C4795563494325A174FC7F2F7E25CE</td><td>DUAL_P~1.FRA|dual_peeling_final.frag</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\dual_depth_peeling\dual_peeling_final.frag</td><td>5</td><td/></row>
		<row><td>_A24AF738FF7547978A68111A0B90EF5E</td><td>C__A24AF738FF7547978A68111A0B90EF5E</td><td>WELD~1.NCM|Weld.NCMPrt</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\Weld.NCMPrt</td><td>5</td><td/></row>
		<row><td>_A29FEDE025A24BEC89F0EAC20DAEA4CD</td><td>C__A29FEDE025A24BEC89F0EAC20DAEA4CD</td><td>MILL~1.NCM|Mill.NCMPrt</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\Mill.NCMPrt</td><td>5</td><td/></row>
		<row><td>_A2A8E787524045DBA36D5BDB7A77E13F</td><td>C__A2A8E787524045DBA36D5BDB7A77E13F</td><td>CONNEX~1.DLL|ConnexInt.dll</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Release\ConnexInt.dll</td><td>5</td><td/></row>
		<row><td>_A2DED2051612450A9E2962309C61F19C</td><td>C__A2DED2051612450A9E2962309C61F19C</td><td>DUAL_P~1.VER|dual_peeling_init.vert</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\dual_depth_peeling\dual_peeling_init.vert</td><td>5</td><td/></row>
		<row><td>_A473537B6DB04D899D6E1FD1979FA3CF</td><td>C__A473537B6DB04D899D6E1FD1979FA3CF</td><td>DIFFUS~1.BMP|diffuse_c04.bmp</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\diffuse_c04.bmp</td><td>5</td><td/></row>
		<row><td>_A530EDAA5E624663ABB83D59D42186F6</td><td>C__A530EDAA5E624663ABB83D59D42186F6</td><td>OPENGL~1.DLL|OpenGLRender.dll</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Release\OpenGLRender.dll</td><td>5</td><td/></row>
		<row><td>_A6AFAFB1B104453FADE31BE15EA14353</td><td>C__A6AFAFB1B104453FADE31BE15EA14353</td><td>PHONG~1.VER|Phong.vert</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\Phong.vert</td><td>5</td><td/></row>
		<row><td>_B099E4926AFE43EAB395438D0D8D9B77</td><td>C__B099E4926AFE43EAB395438D0D8D9B77</td><td>BSP_MO~1.DLL|BSP_Model.dll</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\release\BSP_Model.dll</td><td>5</td><td/></row>
		<row><td>_B39748BC31D6422EBB6B2D72123E92AA</td><td>C__B39748BC31D6422EBB6B2D72123E92AA</td><td>NCM.chm</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Release\local\Eng\NCM.chm</td><td>5</td><td/></row>
		<row><td>_BBCC268121A94302A63046EF90320A3A</td><td>C__BBCC268121A94302A63046EF90320A3A</td><td>SPECUL~1.BMP|specular_c05.bmp</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\specular_c05.bmp</td><td>5</td><td/></row>
		<row><td>_C230765F762B47B18C7DBED550986ABA</td><td>C__C230765F762B47B18C7DBED550986ABA</td><td>DUAL_P~1.VER|dual_peeling_final.vert</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\dual_depth_peeling\dual_peeling_final.vert</td><td>5</td><td/></row>
		<row><td>_C3C4913F460544FE9F4C0E2E900DB356</td><td>C__C3C4913F460544FE9F4C0E2E900DB356</td><td>NCMExt.h</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\NCMExt\NCMExt.h</td><td>5</td><td/></row>
		<row><td>_C403BA6E9E194CD592A7C50EDE7E939B</td><td>C__C403BA6E9E194CD592A7C50EDE7E939B</td><td>TURN~1.NCM|Turn.NCMPrt</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\Turn.NCMPrt</td><td>5</td><td/></row>
		<row><td>_D1F8CF020B5A451697624F490F28BE54</td><td>C__D1F8CF020B5A451697624F490F28BE54</td><td>units.dll</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\release\units.dll</td><td>5</td><td/></row>
		<row><td>_D3758339EA714A17984DE22F4644FEE9</td><td>C__D3758339EA714A17984DE22F4644FEE9</td><td>PROPED~1.DLL|PropEditor.dll</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Release\PropEditor.dll</td><td>5</td><td/></row>
		<row><td>_DB2EB634C1AB4C1FA228A22582375EA2</td><td>C__DB2EB634C1AB4C1FA228A22582375EA2</td><td>DUAL_P~1.FRA|dual_peeling_init.frag</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\dual_depth_peeling\dual_peeling_init.frag</td><td>5</td><td/></row>
		<row><td>_E22EF8970F184A29A67C6B5080E0E021</td><td>C__E22EF8970F184A29A67C6B5080E0E021</td><td>DUAL_P~1.FRA|dual_peeling_blend.frag</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\dual_depth_peeling\dual_peeling_blend.frag</td><td>5</td><td/></row>
		<row><td>_ECA4F69504D14AFCA9E7F72A8D3B7013</td><td>C__ECA4F69504D14AFCA9E7F72A8D3B7013</td><td>DUAL_P~1.VER|dual_peeling_peel.vert</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\dual_depth_peeling\dual_peeling_peel.vert</td><td>5</td><td/></row>
		<row><td>_EFD5EB60662546EF86AC8E0A611C4CA6</td><td>C__EFD5EB60662546EF86AC8E0A611C4CA6</td><td>G71G70~1.NCM|G71G70TURN.ncm</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Demo\Inch\G71G70TURN.ncm</td><td>5</td><td/></row>
		<row><td>_F2555C2B1D7C49559BC04823656E2305</td><td>C__F2555C2B1D7C49559BC04823656E2305</td><td>MILLTU~1.NCM|MillTurn.NCMPrt</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\MillTurn.NCMPrt</td><td>5</td><td/></row>
		<row><td>_F3D05398C3BA40C69E8709F43B4E3083</td><td>C__F3D05398C3BA40C69E8709F43B4E3083</td><td>demo.NC</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\work\demo.NC</td><td>5</td><td/></row>
		<row><td>_F4112FDF846B48FB86CE69A0545E366F</td><td>C__F4112FDF846B48FB86CE69A0545E366F</td><td>RESOUR~1.DLL|Resources.dll</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Release\local\Rus\Resources.dll</td><td>5</td><td/></row>
		<row><td>_F58DFAEFADA3442B9526AA9947119E11</td><td>C__F58DFAEFADA3442B9526AA9947119E11</td><td>GLIT~1.VER|Glit.vert</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\Glit.vert</td><td>5</td><td/></row>
		<row><td>_FAF9F28B10E04850937D47023FEB058A</td><td>C__FAF9F28B10E04850937D47023FEB058A</td><td>NETCON~1.XML|NetConfig.xml</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\NetConfig.xml</td><td>5</td><td/></row>
		<row><td>_FEB18844F3D54D3CBA46FE552A79F646</td><td>C__FEB18844F3D54D3CBA46FE552A79F646</td><td>SPECUL~1.BMP|specular_c01.bmp</td><td>0</td><td/><td/><td>512</td><td>1</td><td>C:\Development\NCM\Def\PRT\specular_c01.bmp</td><td>5</td><td/></row>
		<row><td>_a_.stl</td><td>ISX_DEFAULTCOMPONENT37</td><td>-A-.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_A_WITH_TOOLS_20\-A-.stl</td><td>1</td><td/></row>
		<row><td>_c42demo.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>2C42Demo.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\2C42Demo.sm3</td><td>1</td><td/></row>
		<row><td>_x_.stl</td><td>ISX_DEFAULTCOMPONENT36</td><td>-X-.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_30\-X-.stl</td><td>1</td><td/></row>
		<row><td>_y_.stl</td><td>ISX_DEFAULTCOMPONENT36</td><td>-Y-.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_30\-Y-.stl</td><td>1</td><td/></row>
		<row><td>_z_.stl</td><td>ISX_DEFAULTCOMPONENT36</td><td>-Z-.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_30\-Z-.stl</td><td>1</td><td/></row>
		<row><td>acramatic.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>ACRAMA~1.SM3|Acramatic.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\Acramatic.sm3</td><td>1</td><td/></row>
		<row><td>acrodemocycle.ncm</td><td>ISX_DEFAULTCOMPONENT17</td><td>ACRODE~1.NCM|AcroDemoCycle.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_2X\AcroDemoCycle.ncm</td><td>1</td><td/></row>
		<row><td>acromatikdemo.ncm</td><td>ISX_DEFAULTCOMPONENT17</td><td>ACROMA~1.NCM|AcromatikDemo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_2X\AcromatikDemo.ncm</td><td>1</td><td/></row>
		<row><td>adem_cava_nc.ncm</td><td>ISX_DEFAULTCOMPONENT17</td><td>ADEM_C~1.NCM|ADEM_Cava_NC.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_2X\ADEM_Cava_NC.ncm</td><td>1</td><td/></row>
		<row><td>areopag_fanuc_goodway_gls_15</td><td>ISX_DEFAULTCOMPONENT4</td><td>AREOPA~1.SM3|AREOPAG_FANUC_GOODWAY_GLS-1500_MT.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCMEnv\Def\AREOPAG_FANUC_GOODWAY_GLS-1500_MT.sm3</td><td>1</td><td/></row>
		<row><td>areopag_fanuc_hyundai_wia_l2</td><td>ISX_DEFAULTCOMPONENT4</td><td>AREOPA~1.SM3|AREOPAG_FANUC_HYUNDAI_WIA_L230A.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCMEnv\Def\AREOPAG_FANUC_HYUNDAI_WIA_L230A.sm3</td><td>1</td><td/></row>
		<row><td>areopag_fanuc_victor_taichun</td><td>ISX_DEFAULTCOMPONENT4</td><td>AREOPA~1.SM3|AREOPAG_FANUC_VICTOR_TAICHUNG_A110.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCMEnv\Def\AREOPAG_FANUC_VICTOR_TAICHUNG_A110.sm3</td><td>1</td><td/></row>
		<row><td>balasinaleft_g71turndemo.ncm</td><td>ISX_DEFAULTCOMPONENT21</td><td>BALASI~1.NCM|BalasinaLeft-G71TurnDemo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Turn\BalasinaLeft-G71TurnDemo.ncm</td><td>1</td><td/></row>
		<row><td>balasinaright_g71turndemo.nc</td><td>ISX_DEFAULTCOMPONENT21</td><td>BALASI~1.NCM|BalasinaRight-G71TurnDemo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Turn\BalasinaRight-G71TurnDemo.ncm</td><td>1</td><td/></row>
		<row><td>base.stl</td><td>ISX_DEFAULTCOMPONENT13</td><td>Base.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FADAL3X-VMC6030-INCH\Base.stl</td><td>1</td><td/></row>
		<row><td>base.stl1</td><td>ISX_DEFAULTCOMPONENT35</td><td>Base.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_20\Base.stl</td><td>1</td><td/></row>
		<row><td>base.stl2</td><td>ISX_DEFAULTCOMPONENT36</td><td>Base.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_30\Base.stl</td><td>1</td><td/></row>
		<row><td>base.stl3</td><td>ISX_DEFAULTCOMPONENT37</td><td>Base.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_A_WITH_TOOLS_20\Base.stl</td><td>1</td><td/></row>
		<row><td>base_add.stl</td><td>ISX_DEFAULTCOMPONENT13</td><td>Base_Add.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FADAL3X-VMC6030-INCH\Base_Add.stl</td><td>1</td><td/></row>
		<row><td>base_usi.stl</td><td>ISX_DEFAULTCOMPONENT35</td><td>Base_USI.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_20\Base_USI.stl</td><td>1</td><td/></row>
		<row><td>base_usi.stl1</td><td>ISX_DEFAULTCOMPONENT37</td><td>Base_USI.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_A_WITH_TOOLS_20\Base_USI.stl</td><td>1</td><td/></row>
		<row><td>basec.stl</td><td>ISX_DEFAULTCOMPONENT32</td><td>BaseC.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\WELD\BaseC.stl</td><td>1</td><td/></row>
		<row><td>basedemo.ncm</td><td>ISX_DEFAULTCOMPONENT18</td><td>BaseDemo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_3X\BaseDemo.ncm</td><td>1</td><td/></row>
		<row><td>basex.stl</td><td>ISX_DEFAULTCOMPONENT32</td><td>BaseX.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\WELD\BaseX.stl</td><td>1</td><td/></row>
		<row><td>blocklathe2.stl</td><td>ISX_DEFAULTCOMPONENT14</td><td>BLOCKL~1.STL|BlockLathe2.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FanMTDemo2Chusk\BlockLathe2.stl</td><td>1</td><td/></row>
		<row><td>body.stl</td><td>ISX_DEFAULTCOMPONENT24</td><td>body.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\MILLTURN-DEMO\body.stl</td><td>1</td><td/></row>
		<row><td>bodystock.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>BODYST~1.STL|BodyStock.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\BodyStock.stl</td><td>1</td><td/></row>
		<row><td>box10.stl</td><td>ISX_DEFAULTCOMPONENT32</td><td>Box10.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\WELD\Box10.stl</td><td>1</td><td/></row>
		<row><td>ca.stl</td><td>ISX_DEFAULTCOMPONENT37</td><td>CA.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_A_WITH_TOOLS_20\CA.stl</td><td>1</td><td/></row>
		<row><td>center.stl</td><td>ISX_DEFAULTCOMPONENT24</td><td>center.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\MILLTURN-DEMO\center.stl</td><td>1</td><td/></row>
		<row><td>centerh.stl</td><td>ISX_DEFAULTCOMPONENT24</td><td>centerH.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\MILLTURN-DEMO\centerH.stl</td><td>1</td><td/></row>
		<row><td>centertable.stl</td><td>ISX_DEFAULTCOMPONENT8</td><td>CENTER~1.STL|CenterTable.STL</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_HARDINGE_VMC\CenterTable.STL</td><td>1</td><td/></row>
		<row><td>chuck.stl</td><td>ISX_DEFAULTCOMPONENT14</td><td>chuck.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FanMTDemo2Chusk\chuck.stl</td><td>1</td><td/></row>
		<row><td>chuck.stl1</td><td>ISX_DEFAULTCOMPONENT15</td><td>chuck.STL</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\chuck.STL</td><td>1</td><td/></row>
		<row><td>chuck.stl2</td><td>ISX_DEFAULTCOMPONENT24</td><td>chuck.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\MILLTURN-DEMO\chuck.stl</td><td>1</td><td/></row>
		<row><td>chuck1.stl</td><td>ISX_DEFAULTCOMPONENT14</td><td>Chuck1.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FanMTDemo2Chusk\Chuck1.stl</td><td>1</td><td/></row>
		<row><td>chuck2.stl</td><td>ISX_DEFAULTCOMPONENT14</td><td>Chuck2.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FanMTDemo2Chusk\Chuck2.stl</td><td>1</td><td/></row>
		<row><td>chuck3.stl</td><td>ISX_DEFAULTCOMPONENT14</td><td>Chuck3.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FanMTDemo2Chusk\Chuck3.stl</td><td>1</td><td/></row>
		<row><td>compbase.dll</td><td>CompBase.dll</td><td>CompBase.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\ncm\Release\CompBase.dll</td><td>1</td><td/></row>
		<row><td>contrchuck.stl</td><td>ISX_DEFAULTCOMPONENT14</td><td>CONTRC~1.STL|Contrchuck.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FanMTDemo2Chusk\Contrchuck.stl</td><td>1</td><td/></row>
		<row><td>contrturr.stl</td><td>ISX_DEFAULTCOMPONENT14</td><td>CONTRT~1.STL|ContrTurr.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FanMTDemo2Chusk\ContrTurr.stl</td><td>1</td><td/></row>
		<row><td>coolant.stl</td><td>ISX_DEFAULTCOMPONENT35</td><td>Coolant.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_20\Coolant.stl</td><td>1</td><td/></row>
		<row><td>coredemo.ncm</td><td>ISX_DEFAULTCOMPONENT19</td><td>CoreDemo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_5X\CoreDemo.ncm</td><td>1</td><td/></row>
		<row><td>dclnl4040s19.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>DCLNL4~1.STL|DCLNL4040S19.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\DCLNL4040S19.stl</td><td>1</td><td/></row>
		<row><td>dclnr4040s19.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>DCLNR4~1.STL|DCLNR4040S19.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\DCLNR4040S19.stl</td><td>1</td><td/></row>
		<row><td>defaults.xml</td><td>ISX_DEFAULTCOMPONENT31</td><td>defaults.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\DFN\defaults.xml</td><td>1</td><td/></row>
		<row><td>demo.ncm</td><td>ISX_DEFAULTCOMPONENT17</td><td>Demo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_2X\Demo.ncm</td><td>1</td><td/></row>
		<row><td>demo2c42.ncm</td><td>ISX_DEFAULTCOMPONENT17</td><td>Demo2C42.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_2X\Demo2C42.ncm</td><td>1</td><td/></row>
		<row><td>demo4x.ncm</td><td>ISX_DEFAULTCOMPONENT17</td><td>Demo4X.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_2X\Demo4X.ncm</td><td>1</td><td/></row>
		<row><td>demo4x_cyl.ncm</td><td>ISX_DEFAULTCOMPONENT17</td><td>DEMO4X~1.NCM|Demo4X_CYL.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_2X\Demo4X_CYL.ncm</td><td>1</td><td/></row>
		<row><td>demofanuc0im.ncm</td><td>ISX_DEFAULTCOMPONENT17</td><td>DEMOFA~1.NCM|DemoFanuc0im.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_2X\DemoFanuc0im.ncm</td><td>1</td><td/></row>
		<row><td>demofanuc6sides.ncm</td><td>ISX_DEFAULTCOMPONENT17</td><td>DEMOFA~1.NCM|DemoFanuc6sides.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_2X\DemoFanuc6sides.ncm</td><td>1</td><td/></row>
		<row><td>demolaser.ncm</td><td>ISX_DEFAULTCOMPONENT17</td><td>DEMOLA~1.NCM|Demolaser.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_2X\Demolaser.ncm</td><td>1</td><td/></row>
		<row><td>derjavkaproreznaia.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>DERJAV~1.STL|DerjavkaProreznaia.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\Demo\DerjavkaProreznaia.stl</td><td>1</td><td/></row>
		<row><td>dm_nc210.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>DM_NC210.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\DM_NC210.sm3</td><td>1</td><td/></row>
		<row><td>dmt_fanuc18itb.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>DMT_FA~1.SM3|DMT_Fanuc18iTB.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\DMT_Fanuc18iTB.sm3</td><td>1</td><td/></row>
		<row><td>dmu80_a.stl</td><td>ISX_DEFAULTCOMPONENT11</td><td>dmu80_A.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\DMU80\dmu80_A.stl</td><td>1</td><td/></row>
		<row><td>dmu80_base.stl</td><td>ISX_DEFAULTCOMPONENT11</td><td>DMU80_~1.STL|dmu80_BASE.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\DMU80\dmu80_BASE.stl</td><td>1</td><td/></row>
		<row><td>dmu80_c_axis.stl</td><td>ISX_DEFAULTCOMPONENT11</td><td>DMU80_~1.STL|dmu80_C_AXIS.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\DMU80\dmu80_C_AXIS.stl</td><td>1</td><td/></row>
		<row><td>dmu80_head.stl</td><td>ISX_DEFAULTCOMPONENT11</td><td>DMU80_~1.STL|dmu80_HEAD.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\DMU80\dmu80_HEAD.stl</td><td>1</td><td/></row>
		<row><td>dmu80_xz.stl</td><td>ISX_DEFAULTCOMPONENT11</td><td>dmu80_XZ.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\DMU80\dmu80_XZ.stl</td><td>1</td><td/></row>
		<row><td>dmu80_y_axis.stl</td><td>ISX_DEFAULTCOMPONENT11</td><td>DMU80_~1.STL|dmu80_Y_AXIS.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\DMU80\dmu80_Y_AXIS.stl</td><td>1</td><td/></row>
		<row><td>dnodemo.ncm</td><td>ISX_DEFAULTCOMPONENT18</td><td>DnoDemo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_3X\DnoDemo.ncm</td><td>1</td><td/></row>
		<row><td>dogdemo.ncm</td><td>ISX_DEFAULTCOMPONENT18</td><td>DogDemo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_3X\DogDemo.ncm</td><td>1</td><td/></row>
		<row><td>drillblock.stl</td><td>ISX_DEFAULTCOMPONENT14</td><td>DRILLB~1.STL|DrillBlock.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FanMTDemo2Chusk\DrillBlock.stl</td><td>1</td><td/></row>
		<row><td>dssnl4040s19.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>DSSNL4~1.STL|DSSNL4040S19.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\DSSNL4040S19.stl</td><td>1</td><td/></row>
		<row><td>dssnr4040s19.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>DSSNR4~1.STL|DSSNR4040S19.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\DSSNR4040S19.stl</td><td>1</td><td/></row>
		<row><td>dver_mag.stl</td><td>ISX_DEFAULTCOMPONENT35</td><td>Dver_Mag.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_20\Dver_Mag.stl</td><td>1</td><td/></row>
		<row><td>dver_mag.stl1</td><td>ISX_DEFAULTCOMPONENT37</td><td>Dver_Mag.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_A_WITH_TOOLS_20\Dver_Mag.stl</td><td>1</td><td/></row>
		<row><td>dvig_mag.stl</td><td>ISX_DEFAULTCOMPONENT35</td><td>Dvig_MAG.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_20\Dvig_MAG.stl</td><td>1</td><td/></row>
		<row><td>dvig_mag.stl1</td><td>ISX_DEFAULTCOMPONENT37</td><td>Dvig_MAG.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_A_WITH_TOOLS_20\Dvig_MAG.stl</td><td>1</td><td/></row>
		<row><td>endx.stl</td><td>ISX_DEFAULTCOMPONENT32</td><td>EndX.STL</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\WELD\EndX.STL</td><td>1</td><td/></row>
		<row><td>errconfmesen.txt</td><td>ISX_DEFAULTCOMPONENT31</td><td>ERRCON~1.TXT|ErrConfMesEn.txt</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\Def\DFN\ErrConfMesEn.txt</td><td>1</td><td/></row>
		<row><td>errconfmesru.txt</td><td>ISX_DEFAULTCOMPONENT31</td><td>ERRCON~1.TXT|ErrConfMesRu.txt</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\Def\DFN\ErrConfMesRu.txt</td><td>1</td><td/></row>
		<row><td>expreval.dll</td><td>ExprEval.dll</td><td>ExprEval.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\Release\ExprEval.dll</td><td>1</td><td/></row>
		<row><td>fanmtdemo2chusk.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>FANMTD~1.SM3|FanMTDemo2Chusk.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\FanMTDemo2Chusk.sm3</td><td>1</td><td/></row>
		<row><td>fanuc0_tb_tt.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>FANUC0~1.SM3|Fanuc0-TB-TT.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\Fanuc0-TB-TT.sm3</td><td>1</td><td/></row>
		<row><td>fanuc0im.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>Fanuc0im.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\Fanuc0im.sm3</td><td>1</td><td/></row>
		<row><td>fanuc6m_v1.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>FANUC6~1.SM3|FANUC6M_V1.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\FANUC6M_V1.sm3</td><td>1</td><td/></row>
		<row><td>fanuc_oi_mc.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>FANUCO~1.SM3|Fanuc oi-MC.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\Fanuc oi-MC.sm3</td><td>1</td><td/></row>
		<row><td>fanucdemo.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>FANUCD~1.SM3|FanucDemo.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\FanucDemo.sm3</td><td>1</td><td/></row>
		<row><td>fanucdemo2.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>FanucDemo2.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\FanucDemo2.sm3</td><td>1</td><td/></row>
		<row><td>fanuclathe.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>FANUCL~1.SM3|FanucLathe.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\FanucLathe.sm3</td><td>1</td><td/></row>
		<row><td>fikusdemomt.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>FIKUSD~1.SM3|FikusDemoMT.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\FikusDemoMT.sm3</td><td>1</td><td/></row>
		<row><td>fix.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>fix.STL</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\fix.STL</td><td>1</td><td/></row>
		<row><td>fixmove.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>fixmove.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\fixmove.stl</td><td>1</td><td/></row>
		<row><td>fixnomove.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>FIXNOM~1.STL|fixnomove.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\fixnomove.stl</td><td>1</td><td/></row>
		<row><td>formademo.ncm</td><td>ISX_DEFAULTCOMPONENT18</td><td>FORMAD~1.NCM|FormaDemo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_3X\FormaDemo.ncm</td><td>1</td><td/></row>
		<row><td>frutisdemo.ncm</td><td>ISX_DEFAULTCOMPONENT18</td><td>FRUTIS~1.NCM|FrutisDemo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_3X\FrutisDemo.ncm</td><td>1</td><td/></row>
		<row><td>ghdr_32_10.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>GHDR32~1.STL|GHDR 32-10.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\GHDR 32-10.stl</td><td>1</td><td/></row>
		<row><td>haas.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>Haas.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\Haas.sm3</td><td>1</td><td/></row>
		<row><td>haasmilldemo.ncm</td><td>ISX_DEFAULTCOMPONENT17</td><td>HAASMI~1.NCM|HaasMillDemo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_2X\HaasMillDemo.ncm</td><td>1</td><td/></row>
		<row><td>haasmt.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>HaasMT.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\HaasMT.sm3</td><td>1</td><td/></row>
		<row><td>header.dll</td><td>header.dll</td><td>header.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\ncm\Def\DFN\header.dll</td><td>1</td><td/></row>
		<row><td>holder_hardinger.stl</td><td>ISX_DEFAULTCOMPONENT8</td><td>HOLDER~1.STL|Holder_Hardinger.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_HARDINGE_VMC\Holder_Hardinger.stl</td><td>1</td><td/></row>
		<row><td>holder_hardinger.stl1</td><td>ISX_DEFAULTCOMPONENT23</td><td>HOLDER~1.STL|Holder_Hardinger.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\MILL4AX\Holder_Hardinger.stl</td><td>1</td><td/></row>
		<row><td>holdpcln.stl</td><td>ISX_DEFAULTCOMPONENT14</td><td>HoldPCLN.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FanMTDemo2Chusk\HoldPCLN.stl</td><td>1</td><td/></row>
		<row><td>holdtlhn.stl</td><td>ISX_DEFAULTCOMPONENT14</td><td>HoldTLHN.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FanMTDemo2Chusk\HoldTLHN.stl</td><td>1</td><td/></row>
		<row><td>impeller.ncm</td><td>ISX_DEFAULTCOMPONENT17</td><td>Impeller.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_2X\Impeller.ncm</td><td>1</td><td/></row>
		<row><td>intekdemo.ncm</td><td>ISX_DEFAULTCOMPONENT18</td><td>INTEKD~1.NCM|IntekDemo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_3X\IntekDemo.ncm</td><td>1</td><td/></row>
		<row><td>iso.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>ISO.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\ISO.sm3</td><td>1</td><td/></row>
		<row><td>isoinch.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>ISOINCH.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\ISOINCH.sm3</td><td>1</td><td/></row>
		<row><td>kartersimpledemo.ncm</td><td>ISX_DEFAULTCOMPONENT19</td><td>KARTER~1.NCM|KarterSimpleDemo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_5X\KarterSimpleDemo.ncm</td><td>1</td><td/></row>
		<row><td>karuschip.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>KARUSC~1.STL|KarusChip.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\KarusChip.stl</td><td>1</td><td/></row>
		<row><td>karuspatr.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>KARUSP~1.STL|karusPatr.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\karusPatr.stl</td><td>1</td><td/></row>
		<row><td>korpdemo.ncm</td><td>ISX_DEFAULTCOMPONENT16</td><td>KorpDemo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\KorpDemo.ncm</td><td>1</td><td/></row>
		<row><td>korpus_4_axis.stl</td><td>ISX_DEFAULTCOMPONENT37</td><td>Korpus_4_axis.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_A_WITH_TOOLS_20\Korpus_4_axis.stl</td><td>1</td><td/></row>
		<row><td>l3030.sm2</td><td>ISX_DEFAULTCOMPONENT4</td><td>L3030.sm2</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\L3030.sm2</td><td>1</td><td/></row>
		<row><td>lapa.stl</td><td>ISX_DEFAULTCOMPONENT36</td><td>Lapa.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_30\Lapa.stl</td><td>1</td><td/></row>
		<row><td>lathe1512_d4.ncm</td><td>ISX_DEFAULTCOMPONENT21</td><td>LATHE1~1.NCM|Lathe1512-D4.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Turn\Lathe1512-D4.ncm</td><td>1</td><td/></row>
		<row><td>lathe_cnc_3.sm2</td><td>ISX_DEFAULTCOMPONENT4</td><td>LATHE_~1.SM2|Lathe_CNC_3.sm2</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\Lathe_CNC_3.sm2</td><td>1</td><td/></row>
		<row><td>lathedemo_nc110_1.ncm</td><td>ISX_DEFAULTCOMPONENT21</td><td>LATHED~1.NCM|LatheDemo-NC110-1.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Turn\LatheDemo-NC110-1.ncm</td><td>1</td><td/></row>
		<row><td>lathefancycle.ncm</td><td>ISX_DEFAULTCOMPONENT21</td><td>LATHEF~1.NCM|LatheFanCycle.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Turn\LatheFanCycle.ncm</td><td>1</td><td/></row>
		<row><td>lathenc31_ex1.ncm</td><td>ISX_DEFAULTCOMPONENT21</td><td>LATHEN~1.NCM|LatheNC31-EX1.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Turn\LatheNC31-EX1.ncm</td><td>1</td><td/></row>
		<row><td>lathenc31_ex2.ncm</td><td>ISX_DEFAULTCOMPONENT21</td><td>LATHEN~1.NCM|LatheNC31-EX2.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Turn\LatheNC31-EX2.ncm</td><td>1</td><td/></row>
		<row><td>listcncdemo.txt</td><td>ISX_DEFAULTCOMPONENT4</td><td>LISTCN~1.TXT|ListCNCDemo.txt</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\ListCNCDemo.txt</td><td>1</td><td/></row>
		<row><td>lokostools.ncm</td><td>ISX_DEFAULTCOMPONENT15</td><td>LOKOST~1.NCM|LokosTools.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\Demo\LokosTools.ncm</td><td>1</td><td/></row>
		<row><td>lopatkademo.ncm</td><td>ISX_DEFAULTCOMPONENT19</td><td>LOPATK~1.NCM|LopatkaDemo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_5X\LopatkaDemo.ncm</td><td>1</td><td/></row>
		<row><td>lx123l25_3232b_007.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>LX123L~1.STL|LX123L25-3232B-007.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\LX123L25-3232B-007.stl</td><td>1</td><td/></row>
		<row><td>m08.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>m08.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\Demo\m08.stl</td><td>1</td><td/></row>
		<row><td>macrofanuc.ncm</td><td>ISX_DEFAULTCOMPONENT17</td><td>MACROF~1.NCM|MacroFanuc.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_2X\MacroFanuc.ncm</td><td>1</td><td/></row>
		<row><td>magazin.stl</td><td>ISX_DEFAULTCOMPONENT35</td><td>Magazin.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_20\Magazin.stl</td><td>1</td><td/></row>
		<row><td>magazin.stl1</td><td>ISX_DEFAULTCOMPONENT37</td><td>Magazin.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_A_WITH_TOOLS_20\Magazin.stl</td><td>1</td><td/></row>
		<row><td>main.xml</td><td>ISX_DEFAULTCOMPONENT6</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\1A516MF3\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml1</td><td>ISX_DEFAULTCOMPONENT7</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_DEFAULT\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml10</td><td>ISX_DEFAULTCOMPONENT23</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\MILL4AX\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml11</td><td>ISX_DEFAULTCOMPONENT24</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\MILLTURN-DEMO\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml12</td><td>ISX_DEFAULTCOMPONENT25</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\MILLTURN_DEFAULT\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml13</td><td>ISX_DEFAULTCOMPONENT26</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\MILL_DEFAULT\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml14</td><td>ISX_DEFAULTCOMPONENT27</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\R-510\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml15</td><td>ISX_DEFAULTCOMPONENT29</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\TURN_DEFAULT\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml16</td><td>ISX_DEFAULTCOMPONENT32</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\WELD\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml17</td><td>ISX_DEFAULTCOMPONENT35</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_20\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml18</td><td>ISX_DEFAULTCOMPONENT36</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_30\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml19</td><td>ISX_DEFAULTCOMPONENT37</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_A_WITH_TOOLS_20\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml2</td><td>ISX_DEFAULTCOMPONENT8</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_HARDINGE_VMC\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml20</td><td>ISX_DEFAULTCOMPONENT39</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\Def\machines\TOOLS_T\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml21</td><td>ISX_DEFAULTCOMPONENT41</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCMEnv\Machines\GOODWAY_GLS-1500_MT\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml22</td><td>ISX_DEFAULTCOMPONENT42</td><td>MAIN.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCMEnv\Machines\HYUNDAI_WIA_L230A\MAIN.xml</td><td>1</td><td/></row>
		<row><td>main.xml23</td><td>ISX_DEFAULTCOMPONENT43</td><td>MAIN.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCMEnv\Machines\VICTOR_TAICHUNG_A110\MAIN.xml</td><td>1</td><td/></row>
		<row><td>main.xml3</td><td>ISX_DEFAULTCOMPONENT9</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\5X_Head-X_on_Head-Z\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml4</td><td>ISX_DEFAULTCOMPONENT10</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\5x_HERMLE_C30U_INCH\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml5</td><td>ISX_DEFAULTCOMPONENT11</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\DMU80\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml6</td><td>ISX_DEFAULTCOMPONENT12</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\DM_4X_TABLE_A\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml7</td><td>ISX_DEFAULTCOMPONENT13</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FADAL3X-VMC6030-INCH\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml8</td><td>ISX_DEFAULTCOMPONENT14</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FanMTDemo2Chusk\main.xml</td><td>1</td><td/></row>
		<row><td>main.xml9</td><td>ISX_DEFAULTCOMPONENT22</td><td>main.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\LATHE-VERT-1512\main.xml</td><td>1</td><td/></row>
		<row><td>mainspndl.stl</td><td>ISX_DEFAULTCOMPONENT14</td><td>MAINSP~1.STL|MainSpndl.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FanMTDemo2Chusk\MainSpndl.stl</td><td>1</td><td/></row>
		<row><td>mc032_1.ncm</td><td>ISX_DEFAULTCOMPONENT21</td><td>MC032-1.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Turn\MC032-1.ncm</td><td>1</td><td/></row>
		<row><td>millblock0.stl</td><td>ISX_DEFAULTCOMPONENT14</td><td>MILLBL~1.STL|MillBlock0.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FanMTDemo2Chusk\MillBlock0.stl</td><td>1</td><td/></row>
		<row><td>millblock90.stl</td><td>ISX_DEFAULTCOMPONENT14</td><td>MILLBL~1.STL|MillBlock90.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FanMTDemo2Chusk\MillBlock90.stl</td><td>1</td><td/></row>
		<row><td>millplusinch.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>MILLPL~1.SM3|MillPlusInch.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\MillPlusInch.sm3</td><td>1</td><td/></row>
		<row><td>milltp.ncm</td><td>ISX_DEFAULTCOMPONENT17</td><td>MillTP.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_2X\MillTP.ncm</td><td>1</td><td/></row>
		<row><td>millturn_demo.ncm</td><td>ISX_DEFAULTCOMPONENT20</td><td>MILLTU~1.NCM|MillTurn-Demo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\MillTurn\MillTurn-Demo.ncm</td><td>1</td><td/></row>
		<row><td>millturnpolardemo.ncm</td><td>ISX_DEFAULTCOMPONENT20</td><td>MILLTU~1.NCM|MillTurnPolarDemo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\MillTurn\MillTurnPolarDemo.ncm</td><td>1</td><td/></row>
		<row><td>modeldemo.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>MODELD~1.STL|ModelDemo.STL</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\ModelDemo.STL</td><td>1</td><td/></row>
		<row><td>mtd_transfer.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>MTD_TR~1.SM3|MTD_TRANSFER.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\Def\MTD_TRANSFER.sm3</td><td>1</td><td/></row>
		<row><td>mtdemo19.ncm</td><td>ISX_DEFAULTCOMPONENT20</td><td>MTDemo19.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\MillTurn\MTDemo19.ncm</td><td>1</td><td/></row>
		<row><td>mtdemowhill.ncm</td><td>ISX_DEFAULTCOMPONENT20</td><td>MTDEMO~1.NCM|MTDemoWhill.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\MillTurn\MTDemoWhill.ncm</td><td>1</td><td/></row>
		<row><td>n33.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>N33.SM3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\N33.SM3</td><td>1</td><td/></row>
		<row><td>napr_usi.stl</td><td>ISX_DEFAULTCOMPONENT35</td><td>Napr_USI.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_20\Napr_USI.stl</td><td>1</td><td/></row>
		<row><td>napr_usi.stl1</td><td>ISX_DEFAULTCOMPONENT37</td><td>Napr_USI.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_A_WITH_TOOLS_20\Napr_USI.stl</td><td>1</td><td/></row>
		<row><td>nc1.ncm</td><td>ISX_DEFAULTCOMPONENT17</td><td>NC1.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_2X\NC1.ncm</td><td>1</td><td/></row>
		<row><td>nc31_1.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>NC31-1.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\NC31-1.sm3</td><td>1</td><td/></row>
		<row><td>nc_110_lathe.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>NC-110~1.SM3|NC-110_lathe.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\NC-110_lathe.sm3</td><td>1</td><td/></row>
		<row><td>ncm_fanuc_mill.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>NCM_FA~1.SM3|NCM_FANUC_MILL.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\Def\NCM_FANUC_MILL.sm3</td><td>1</td><td/></row>
		<row><td>ncm_fanuc_millturn.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>NCM_FA~1.SM3|NCM_FANUC_MillTurn.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\Def\NCM_FANUC_MillTurn.sm3</td><td>1</td><td/></row>
		<row><td>ncmanager.chm</td><td>NCManager.chm</td><td>NCMANA~1.CHM|NCManager.chm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Release\local\Rus\NCManager.chm</td><td>1</td><td/></row>
		<row><td>ncmanager.chm1</td><td>NCManager.chm1</td><td>NCManager.chm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Release\local\Eng\NCManager.chm</td><td>1</td><td/></row>
		<row><td>ncmhandlers.dll</td><td>NCMHandlers.dll</td><td>NCMHAN~1.DLL|NCMHandlers.dll</td><td>0</td><td/><td/><td>0</td><td>1</td><td>C:\development\NCM\Release\NCMHandlers.dll</td><td>1</td><td/></row>
		<row><td>ncminitenginch.dfn1</td><td>ISX_DEFAULTCOMPONENT31</td><td>NCMINI~1.DFN|NcminitEngINCH.dfn</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\DFN\NcminitEngINCH.dfn</td><td>1</td><td/></row>
		<row><td>ncminitengmm.dfn1</td><td>ISX_DEFAULTCOMPONENT31</td><td>NCMINI~1.DFN|NcminitEngMM.dfn</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\DFN\NcminitEngMM.dfn</td><td>1</td><td/></row>
		<row><td>ncminitkorinch.dfn1</td><td>ISX_DEFAULTCOMPONENT31</td><td>NCMINI~1.DFN|ncminitKorINCH.dfn</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\DFN\ncminitKorINCH.dfn</td><td>1</td><td/></row>
		<row><td>ncminitkormm.dfn1</td><td>ISX_DEFAULTCOMPONENT31</td><td>NCMINI~1.DFN|ncminitKorMM.dfn</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\DFN\ncminitKorMM.dfn</td><td>1</td><td/></row>
		<row><td>ncminitrusinch.dfn1</td><td>ISX_DEFAULTCOMPONENT31</td><td>NCMINI~1.DFN|NcminitRusINCH.dfn</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\DFN\NcminitRusINCH.dfn</td><td>1</td><td/></row>
		<row><td>ncminitrusmm.dfn1</td><td>ISX_DEFAULTCOMPONENT31</td><td>NCMINI~1.DFN|NcminitRusMM.dfn</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\DFN\NcminitRusMM.dfn</td><td>1</td><td/></row>
		<row><td>nextenums.h</td><td>ISX_DEFAULTCOMPONENT33</td><td>NEXTEN~1.H|NExtEnums.h</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\NCMExt\NExtEnums.h</td><td>1</td><td/></row>
		<row><td>null_sk40.stl</td><td>ISX_DEFAULTCOMPONENT37</td><td>Null_SK40.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_A_WITH_TOOLS_20\Null_SK40.stl</td><td>1</td><td/></row>
		<row><td>ocimport.dll</td><td>OCImport.dll</td><td>OCImport.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\Release\OCImport.dll</td><td>1</td><td/></row>
		<row><td>ox.stl</td><td>ISX_DEFAULTCOMPONENT35</td><td>OX.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_20\OX.stl</td><td>1</td><td/></row>
		<row><td>ox.stl1</td><td>ISX_DEFAULTCOMPONENT37</td><td>OX.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_A_WITH_TOOLS_20\OX.stl</td><td>1</td><td/></row>
		<row><td>oy.stl</td><td>ISX_DEFAULTCOMPONENT35</td><td>OY.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_20\OY.stl</td><td>1</td><td/></row>
		<row><td>oy.stl1</td><td>ISX_DEFAULTCOMPONENT37</td><td>OY.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_A_WITH_TOOLS_20\OY.stl</td><td>1</td><td/></row>
		<row><td>oz.stl</td><td>ISX_DEFAULTCOMPONENT35</td><td>OZ.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_20\OZ.stl</td><td>1</td><td/></row>
		<row><td>oz.stl1</td><td>ISX_DEFAULTCOMPONENT37</td><td>OZ.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_A_WITH_TOOLS_20\OZ.stl</td><td>1</td><td/></row>
		<row><td>p1.stl</td><td>ISX_DEFAULTCOMPONENT36</td><td>P1.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_30\P1.stl</td><td>1</td><td/></row>
		<row><td>parta.stl</td><td>ISX_DEFAULTCOMPONENT32</td><td>PartA.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\WELD\PartA.stl</td><td>1</td><td/></row>
		<row><td>partb.stl</td><td>ISX_DEFAULTCOMPONENT32</td><td>PartB.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\WELD\PartB.stl</td><td>1</td><td/></row>
		<row><td>partx.stl</td><td>ISX_DEFAULTCOMPONENT32</td><td>PartX.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\WELD\PartX.stl</td><td>1</td><td/></row>
		<row><td>party.stl</td><td>ISX_DEFAULTCOMPONENT32</td><td>PartY.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\WELD\PartY.stl</td><td>1</td><td/></row>
		<row><td>partz.stl</td><td>ISX_DEFAULTCOMPONENT32</td><td>PartZ.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\WELD\PartZ.stl</td><td>1</td><td/></row>
		<row><td>planshayiba.stl</td><td>ISX_DEFAULTCOMPONENT22</td><td>PLANSH~1.STL|planshayiba.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\LATHE-VERT-1512\planshayiba.stl</td><td>1</td><td/></row>
		<row><td>platedemo.ncm</td><td>ISX_DEFAULTCOMPONENT18</td><td>PLATED~1.NCM|PlateDemo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_3X\PlateDemo.ncm</td><td>1</td><td/></row>
		<row><td>podkl.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>podkl.STL</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\podkl.STL</td><td>1</td><td/></row>
		<row><td>polarinterpmillturn_demo.ncm</td><td>ISX_DEFAULTCOMPONENT20</td><td>POLARI~1.NCM|PolarInterpMillTurn-Demo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\MillTurn\PolarInterpMillTurn-Demo.ncm</td><td>1</td><td/></row>
		<row><td>puma_lathe_demo.ncm</td><td>ISX_DEFAULTCOMPONENT21</td><td>PUMA-L~1.NCM|Puma-Lathe-Demo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Turn\Puma-Lathe-Demo.ncm</td><td>1</td><td/></row>
		<row><td>quickcamdemolathe.ncm</td><td>ISX_DEFAULTCOMPONENT21</td><td>QUICKC~1.NCM|QuickCamDemoLathe.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Turn\QuickCamDemoLathe.ncm</td><td>1</td><td/></row>
		<row><td>restrictorplate.ncm</td><td>ISX_DEFAULTCOMPONENT17</td><td>RESTRI~1.NCM|RestrictorPlate.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_2X\RestrictorPlate.ncm</td><td>1</td><td/></row>
		<row><td>rev_2.stl</td><td>ISX_DEFAULTCOMPONENT36</td><td>Rev-2.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_30\Rev-2.stl</td><td>1</td><td/></row>
		<row><td>revolver_number_tool.stl</td><td>ISX_DEFAULTCOMPONENT35</td><td>Revolver_number_tool.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_20\Revolver_number_tool.stl</td><td>1</td><td/></row>
		<row><td>revolver_number_tool.stl1</td><td>ISX_DEFAULTCOMPONENT37</td><td>Revolver_number_tool.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_A_WITH_TOOLS_20\Revolver_number_tool.stl</td><td>1</td><td/></row>
		<row><td>rf123l32_3232bm.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>RF123L~1.STL|RF123L32-3232BM.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\RF123L32-3232BM.stl</td><td>1</td><td/></row>
		<row><td>rotate_hardinger.stl</td><td>ISX_DEFAULTCOMPONENT8</td><td>ROTATE~1.STL|Rotate_Hardinger.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_HARDINGE_VMC\Rotate_Hardinger.stl</td><td>1</td><td/></row>
		<row><td>rotate_hardinger.stl1</td><td>ISX_DEFAULTCOMPONENT12</td><td>ROTATE~1.STL|Rotate_Hardinger.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\DM_4X_TABLE_A\Rotate_Hardinger.stl</td><td>1</td><td/></row>
		<row><td>rotate_hardinger.stl2</td><td>ISX_DEFAULTCOMPONENT23</td><td>ROTATE~1.STL|Rotate_Hardinger.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\MILL4AX\Rotate_Hardinger.stl</td><td>1</td><td/></row>
		<row><td>rotatebase_hardinger.stl</td><td>ISX_DEFAULTCOMPONENT8</td><td>ROTATE~1.STL|RotateBase_Hardinger.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_HARDINGE_VMC\RotateBase_Hardinger.stl</td><td>1</td><td/></row>
		<row><td>rotatebase_hardinger.stl1</td><td>ISX_DEFAULTCOMPONENT12</td><td>ROTATE~1.STL|RotateBase_Hardinger.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\DM_4X_TABLE_A\RotateBase_Hardinger.stl</td><td>1</td><td/></row>
		<row><td>rotatebase_hardinger.stl2</td><td>ISX_DEFAULTCOMPONENT23</td><td>ROTATE~1.STL|RotateBase_Hardinger.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\MILL4AX\RotateBase_Hardinger.stl</td><td>1</td><td/></row>
		<row><td>rotc.stl</td><td>ISX_DEFAULTCOMPONENT32</td><td>RotC.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\WELD\RotC.stl</td><td>1</td><td/></row>
		<row><td>rovol_golovka.stl</td><td>ISX_DEFAULTCOMPONENT22</td><td>ROVOL_~1.STL|rovol_golovka.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\LATHE-VERT-1512\rovol_golovka.stl</td><td>1</td><td/></row>
		<row><td>rx123l25_3232b_007.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>RX123L~1.STL|RX123L25-3232B-007.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\RX123L25-3232B-007.stl</td><td>1</td><td/></row>
		<row><td>safedefaults.xml</td><td>ISX_DEFAULTCOMPONENT31</td><td>SAFEDE~1.XML|Safedefaults.xml</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\DFN\Safedefaults.xml</td><td>1</td><td/></row>
		<row><td>shnekdemo.ncm</td><td>ISX_DEFAULTCOMPONENT19</td><td>SHNEKD~1.NCM|ShnekDemo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_5X\ShnekDemo.ncm</td><td>1</td><td/></row>
		<row><td>shuck1.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>shuck1.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\shuck1.stl</td><td>1</td><td/></row>
		<row><td>shuck2.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>shuck2.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\shuck2.stl</td><td>1</td><td/></row>
		<row><td>shuck3.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>shuck3.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\shuck3.stl</td><td>1</td><td/></row>
		<row><td>sin840_dt_mach1512.sm2</td><td>ISX_DEFAULTCOMPONENT4</td><td>SIN840~1.SM2|SIN840-DT-MACH1512.sm2</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\SIN840-DT-MACH1512.sm2</td><td>1</td><td/></row>
		<row><td>sinumeric840d.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>SINUME~1.SM3|Sinumeric840D.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\Sinumeric840D.sm3</td><td>1</td><td/></row>
		<row><td>sk40.stl</td><td>ISX_DEFAULTCOMPONENT35</td><td>SK40.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_20\SK40.stl</td><td>1</td><td/></row>
		<row><td>sk50.stl</td><td>ISX_DEFAULTCOMPONENT36</td><td>SK50.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_30\SK50.stl</td><td>1</td><td/></row>
		<row><td>spe_demo_49.ncm</td><td>ISX_DEFAULTCOMPONENT17</td><td>SPE-DEMO-49.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_2X\SPE-DEMO-49.ncm</td><td>1</td><td/></row>
		<row><td>spindle.stl</td><td>ISX_DEFAULTCOMPONENT7</td><td>Spindle.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_DEFAULT\Spindle.stl</td><td>1</td><td/></row>
		<row><td>spindle.stl1</td><td>ISX_DEFAULTCOMPONENT35</td><td>Spindle.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_20\Spindle.stl</td><td>1</td><td/></row>
		<row><td>spindle.stl2</td><td>ISX_DEFAULTCOMPONENT36</td><td>Spindle.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_WITH_TOOLS_30\Spindle.stl</td><td>1</td><td/></row>
		<row><td>spindle.stl3</td><td>ISX_DEFAULTCOMPONENT37</td><td>Spindle.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_A_WITH_TOOLS_20\Spindle.stl</td><td>1</td><td/></row>
		<row><td>spindle_hardinger.stl</td><td>ISX_DEFAULTCOMPONENT8</td><td>SPINDL~1.STL|Spindle_Hardinger.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_HARDINGE_VMC\Spindle_Hardinger.stl</td><td>1</td><td/></row>
		<row><td>spindle_hardinger.stl1</td><td>ISX_DEFAULTCOMPONENT12</td><td>SPINDL~1.STL|Spindle_Hardinger.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\DM_4X_TABLE_A\Spindle_Hardinger.stl</td><td>1</td><td/></row>
		<row><td>spindle_hardinger.stl2</td><td>ISX_DEFAULTCOMPONENT23</td><td>SPINDL~1.STL|Spindle_Hardinger.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\MILL4AX\Spindle_Hardinger.stl</td><td>1</td><td/></row>
		<row><td>spindle_z.stl</td><td>ISX_DEFAULTCOMPONENT10</td><td>SPINDL~1.STL|Spindle_Z.STL</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\5x_HERMLE_C30U_INCH\Spindle_Z.STL</td><td>1</td><td/></row>
		<row><td>spindle_z.stl1</td><td>ISX_DEFAULTCOMPONENT13</td><td>SPINDL~1.STL|Spindle_Z.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FADAL3X-VMC6030-INCH\Spindle_Z.stl</td><td>1</td><td/></row>
		<row><td>stanina.stl</td><td>ISX_DEFAULTCOMPONENT22</td><td>stanina.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\LATHE-VERT-1512\stanina.stl</td><td>1</td><td/></row>
		<row><td>stergendemo.ncm</td><td>ISX_DEFAULTCOMPONENT18</td><td>STERGE~1.NCM|StergenDemo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_3X\StergenDemo.ncm</td><td>1</td><td/></row>
		<row><td>stockdmu.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>StockDMU.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\StockDMU.stl</td><td>1</td><td/></row>
		<row><td>suport_poperec.stl</td><td>ISX_DEFAULTCOMPONENT22</td><td>SUPORT~1.STL|suport_poperec.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\LATHE-VERT-1512\suport_poperec.stl</td><td>1</td><td/></row>
		<row><td>suport_prodoln.stl</td><td>ISX_DEFAULTCOMPONENT22</td><td>SUPORT~1.STL|suport_prodoln.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\LATHE-VERT-1512\suport_prodoln.stl</td><td>1</td><td/></row>
		<row><td>table.stl</td><td>ISX_DEFAULTCOMPONENT7</td><td>Table.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\3X_DEFAULT\Table.stl</td><td>1</td><td/></row>
		<row><td>table.stl1</td><td>ISX_DEFAULTCOMPONENT15</td><td>table.STL</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\table.STL</td><td>1</td><td/></row>
		<row><td>table_a.stl</td><td>ISX_DEFAULTCOMPONENT10</td><td>Table_A.STL</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\5x_HERMLE_C30U_INCH\Table_A.STL</td><td>1</td><td/></row>
		<row><td>table_c.stl</td><td>ISX_DEFAULTCOMPONENT10</td><td>Table_C.STL</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\5x_HERMLE_C30U_INCH\Table_C.STL</td><td>1</td><td/></row>
		<row><td>table_hardinger.stl</td><td>ISX_DEFAULTCOMPONENT8</td><td>TABLE_~1.STL|Table_Hardinger.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\4X_HARDINGE_VMC\Table_Hardinger.stl</td><td>1</td><td/></row>
		<row><td>table_hardinger.stl1</td><td>ISX_DEFAULTCOMPONENT12</td><td>TABLE_~1.STL|Table_Hardinger.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\DM_4X_TABLE_A\Table_Hardinger.stl</td><td>1</td><td/></row>
		<row><td>table_hardinger.stl2</td><td>ISX_DEFAULTCOMPONENT23</td><td>TABLE_~1.STL|Table_Hardinger.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\MILL4AX\Table_Hardinger.stl</td><td>1</td><td/></row>
		<row><td>table_x.stl</td><td>ISX_DEFAULTCOMPONENT13</td><td>Table_X.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FADAL3X-VMC6030-INCH\Table_X.stl</td><td>1</td><td/></row>
		<row><td>table_y.stl</td><td>ISX_DEFAULTCOMPONENT13</td><td>Table_Y.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FADAL3X-VMC6030-INCH\Table_Y.stl</td><td>1</td><td/></row>
		<row><td>table_y_add.stl</td><td>ISX_DEFAULTCOMPONENT13</td><td>TABLE_~1.STL|Table_Y_Add.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FADAL3X-VMC6030-INCH\Table_Y_Add.stl</td><td>1</td><td/></row>
		<row><td>tailstock_center.stl</td><td>ISX_DEFAULTCOMPONENT14</td><td>TAILST~1.STL|tailstock_center.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FanMTDemo2Chusk\tailstock_center.stl</td><td>1</td><td/></row>
		<row><td>tkbo.dll</td><td>TKBO.dll</td><td>TKBO.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\OCImport\OCDll\TKBO.dll</td><td>1</td><td/></row>
		<row><td>tkbool.dll</td><td>TKBool.dll</td><td>TKBool.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\OCImport\OCDll\TKBool.dll</td><td>1</td><td/></row>
		<row><td>tkbrep.dll</td><td>TKBRep.dll</td><td>TKBRep.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\OCImport\OCDll\TKBRep.dll</td><td>1</td><td/></row>
		<row><td>tkernel.dll</td><td>TKernel.dll</td><td>TKernel.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\OCImport\OCDll\TKernel.dll</td><td>1</td><td/></row>
		<row><td>tkg2d.dll</td><td>TKG2d.dll</td><td>TKG2d.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\OCImport\OCDll\TKG2d.dll</td><td>1</td><td/></row>
		<row><td>tkg3d.dll</td><td>TKG3d.dll</td><td>TKG3d.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\OCImport\OCDll\TKG3d.dll</td><td>1</td><td/></row>
		<row><td>tkgeomalgo.dll</td><td>TKGeomAlgo.dll</td><td>TKGEOM~1.DLL|TKGeomAlgo.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\OCImport\OCDll\TKGeomAlgo.dll</td><td>1</td><td/></row>
		<row><td>tkgeombase.dll</td><td>TKGeomBase.dll</td><td>TKGEOM~1.DLL|TKGeomBase.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\OCImport\OCDll\TKGeomBase.dll</td><td>1</td><td/></row>
		<row><td>tkiges.dll</td><td>TKIGES.dll</td><td>TKIGES.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\OCImport\OCDll\TKIGES.dll</td><td>1</td><td/></row>
		<row><td>tkmath.dll</td><td>TKMath.dll</td><td>TKMath.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\OCImport\OCDll\TKMath.dll</td><td>1</td><td/></row>
		<row><td>tkmesh.dll</td><td>TKMesh.dll</td><td>TKMesh.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\OCImport\OCDll\TKMesh.dll</td><td>1</td><td/></row>
		<row><td>tkprim.dll</td><td>TKPrim.dll</td><td>TKPrim.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\OCImport\OCDll\TKPrim.dll</td><td>1</td><td/></row>
		<row><td>tkshhealing.dll</td><td>TKShHealing.dll</td><td>TKSHHE~1.DLL|TKShHealing.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\OCImport\OCDll\TKShHealing.dll</td><td>1</td><td/></row>
		<row><td>tkstep.dll</td><td>TKSTEP.dll</td><td>TKSTEP.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\OCImport\OCDll\TKSTEP.dll</td><td>1</td><td/></row>
		<row><td>tkstep209.dll</td><td>TKSTEP209.dll</td><td>TKSTEP~1.DLL|TKSTEP209.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\OCImport\OCDll\TKSTEP209.dll</td><td>1</td><td/></row>
		<row><td>tkstepattr.dll</td><td>TKSTEPAttr.dll</td><td>TKSTEP~1.DLL|TKSTEPAttr.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\OCImport\OCDll\TKSTEPAttr.dll</td><td>1</td><td/></row>
		<row><td>tkstepbase.dll</td><td>TKSTEPBase.dll</td><td>TKSTEP~1.DLL|TKSTEPBase.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\OCImport\OCDll\TKSTEPBase.dll</td><td>1</td><td/></row>
		<row><td>tkstl.dll</td><td>TKSTL.dll</td><td>TKSTL.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\OCImport\OCDll\TKSTL.dll</td><td>1</td><td/></row>
		<row><td>tktopalgo.dll</td><td>TKTopAlgo.dll</td><td>TKTOPA~1.DLL|TKTopAlgo.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\OCImport\OCDll\TKTopAlgo.dll</td><td>1</td><td/></row>
		<row><td>tkxsbase.dll</td><td>TKXSBase.dll</td><td>TKXSBase.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\OCImport\OCDll\TKXSBase.dll</td><td>1</td><td/></row>
		<row><td>tool_t.ncm</td><td>ISX_DEFAULTCOMPONENT15</td><td>TOOL_T.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\Demo\TOOL_T.ncm</td><td>1</td><td/></row>
		<row><td>toolchange_1.sub</td><td>ISX_DEFAULTCOMPONENT4</td><td>ToolChange_1.sub</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\ToolChange_1.sub</td><td>1</td><td/></row>
		<row><td>toolchange_2.sub</td><td>ISX_DEFAULTCOMPONENT4</td><td>ToolChange_2.sub</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\ToolChange_2.sub</td><td>1</td><td/></row>
		<row><td>tools_d.ncm</td><td>ISX_DEFAULTCOMPONENT15</td><td>TOOLS_D.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\Demo\TOOLS_D.ncm</td><td>1</td><td/></row>
		<row><td>toolschange_drumtype.ncm</td><td>ISX_DEFAULTCOMPONENT17</td><td>ToolsChange_DrumType.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_2X\ToolsChange_DrumType.ncm</td><td>1</td><td/></row>
		<row><td>toolschange_drumtype2.ncm</td><td>ISX_DEFAULTCOMPONENT17</td><td>ToolsChange_DrumType2.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_2X\ToolsChange_DrumType2.ncm</td><td>1</td><td/></row>
		<row><td>toolschangetest.nc</td><td>ISX_DEFAULTCOMPONENT38</td><td>ToolsChangeTest.nc</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\work\ToolsChangeTest.nc</td><td>1</td><td/></row>
		<row><td>traub.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>TRAUB.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\TRAUB.sm3</td><td>1</td><td/></row>
		<row><td>traubmillturndemo.ncm</td><td>ISX_DEFAULTCOMPONENT20</td><td>TRAUBM~1.NCM|TraubMillTurnDemo.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\MillTurn\TraubMillTurnDemo.ncm</td><td>1</td><td/></row>
		<row><td>turnbase.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>turnbase.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\turnbase.stl</td><td>1</td><td/></row>
		<row><td>turnblock1.stl</td><td>ISX_DEFAULTCOMPONENT22</td><td>TURNBL~1.STL|TurnBlock1.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\LATHE-VERT-1512\TurnBlock1.stl</td><td>1</td><td/></row>
		<row><td>turnd212_30.stl</td><td>ISX_DEFAULTCOMPONENT24</td><td>TURND2~1.STL|TurnD212-30.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\MILLTURN-DEMO\TurnD212-30.stl</td><td>1</td><td/></row>
		<row><td>turnm03.stl</td><td>ISX_DEFAULTCOMPONENT24</td><td>TurnM03.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\MILLTURN-DEMO\TurnM03.stl</td><td>1</td><td/></row>
		<row><td>turnm04.stl</td><td>ISX_DEFAULTCOMPONENT24</td><td>TurnM04.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\MILLTURN-DEMO\TurnM04.stl</td><td>1</td><td/></row>
		<row><td>turnpokovkad.ncm</td><td>ISX_DEFAULTCOMPONENT21</td><td>TURNPO~1.NCM|TurnPokovkaD.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Turn\TurnPokovkaD.ncm</td><td>1</td><td/></row>
		<row><td>turr.stl</td><td>ISX_DEFAULTCOMPONENT14</td><td>Turr.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FanMTDemo2Chusk\Turr.stl</td><td>1</td><td/></row>
		<row><td>turr.stl1</td><td>ISX_DEFAULTCOMPONENT15</td><td>turr.STL</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\turr.STL</td><td>1</td><td/></row>
		<row><td>turrall.stl</td><td>ISX_DEFAULTCOMPONENT14</td><td>turrAll.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FanMTDemo2Chusk\turrAll.stl</td><td>1</td><td/></row>
		<row><td>turret.stl</td><td>ISX_DEFAULTCOMPONENT14</td><td>Turret.stl</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\FanMTDemo2Chusk\Turret.stl</td><td>1</td><td/></row>
		<row><td>tutorialncmanager44.pdf</td><td>ISX_DEFAULTCOMPONENT40</td><td>TUTORI~1.PDF|TutorialNCManager44.pdf</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\Doc\TutorialNCManager44.pdf</td><td>1</td><td/></row>
		<row><td>tytmultystock.ncm</td><td>ISX_DEFAULTCOMPONENT19</td><td>TYTMUL~1.NCM|TytMultyStock.ncm</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\Millimeter\Mill_5X\TytMultyStock.ncm</td><td>1</td><td/></row>
		<row><td>visbase.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>visbase.STL</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\visbase.STL</td><td>1</td><td/></row>
		<row><td>vismove.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>vismove.STL</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\vismove.STL</td><td>1</td><td/></row>
		<row><td>vispodkl.stl</td><td>ISX_DEFAULTCOMPONENT15</td><td>vispodkl.STL</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Demo\vispodkl.STL</td><td>1</td><td/></row>
		<row><td>vsmpo_sin840_1512.sm2</td><td>ISX_DEFAULTCOMPONENT4</td><td>VSMPO_~1.SM2|VSMPO_SIN840_1512.sm2</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\VSMPO_SIN840_1512.sm2</td><td>1</td><td/></row>
		<row><td>welding.sm3</td><td>ISX_DEFAULTCOMPONENT4</td><td>WELDING.sm3</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\WELDING.sm3</td><td>1</td><td/></row>
		<row><td>y.stl</td><td>ISX_DEFAULTCOMPONENT10</td><td>Y.STL</td><td>0</td><td/><td/><td/><td>1</td><td>C:\Development\NCM\Def\machines\5x_HERMLE_C30U_INCH\Y.STL</td><td>1</td><td/></row>
		<row><td>zlibwapi.dll</td><td>zlibwapi.dll</td><td>zlibwapi.dll</td><td>0</td><td/><td/><td/><td>1</td><td>C:\development\NCM\Release\zlibwapi.dll</td><td>1</td><td/></row>
	</table>

	<table name="FileSFPCatalog">
		<col key="yes" def="s72">File_</col>
		<col key="yes" def="s255">SFPCatalog_</col>
	</table>

	<table name="Font">
		<col key="yes" def="s72">File_</col>
		<col def="S128">FontTitle</col>
	</table>

	<table name="ISAssistantTag">
		<col key="yes" def="s72">Tag</col>
		<col def="S255">Data</col>
	</table>

	<table name="ISBillBoard">
		<col key="yes" def="s72">ISBillboard</col>
		<col def="i2">Duration</col>
		<col def="i2">Origin</col>
		<col def="i2">X</col>
		<col def="i2">Y</col>
		<col def="i2">Effect</col>
		<col def="i2">Sequence</col>
		<col def="i2">Target</col>
		<col def="S72">Color</col>
		<col def="S72">Style</col>
		<col def="S72">Font</col>
		<col def="L72">Title</col>
		<col def="S72">DisplayName</col>
	</table>

	<table name="ISChainPackage">
		<col key="yes" def="s72">Package</col>
		<col def="S255">SourcePath</col>
		<col def="S72">ProductCode</col>
		<col def="i2">Order</col>
		<col def="i4">Options</col>
		<col def="S255">InstallCondition</col>
		<col def="S255">RemoveCondition</col>
		<col def="S0">InstallProperties</col>
		<col def="S0">RemoveProperties</col>
		<col def="S255">ISReleaseFlags</col>
		<col def="S72">DisplayName</col>
	</table>

	<table name="ISChainPackageData">
		<col key="yes" def="s72">Package_</col>
		<col key="yes" def="s72">File</col>
		<col def="s50">FilePath</col>
		<col def="I4">Options</col>
		<col def="V0">Data</col>
		<col def="S255">ISBuildSourcePath</col>
	</table>

	<table name="ISClrWrap">
		<col key="yes" def="s72">Action_</col>
		<col key="yes" def="s72">Name</col>
		<col def="S0">Value</col>
	</table>

	<table name="ISComCatalogAttribute">
		<col key="yes" def="s72">ISComCatalogObject_</col>
		<col key="yes" def="s255">ItemName</col>
		<col def="S0">ItemValue</col>
	</table>

	<table name="ISComCatalogCollection">
		<col key="yes" def="s72">ISComCatalogCollection</col>
		<col def="s72">ISComCatalogObject_</col>
		<col def="s255">CollectionName</col>
	</table>

	<table name="ISComCatalogCollectionObjects">
		<col key="yes" def="s72">ISComCatalogCollection_</col>
		<col key="yes" def="s72">ISComCatalogObject_</col>
	</table>

	<table name="ISComCatalogObject">
		<col key="yes" def="s72">ISComCatalogObject</col>
		<col def="s255">DisplayName</col>
	</table>

	<table name="ISComPlusApplication">
		<col key="yes" def="s72">ISComCatalogObject_</col>
		<col def="S255">ComputerName</col>
		<col def="s72">Component_</col>
		<col def="I2">ISAttributes</col>
		<col def="S0">DepFiles</col>
	</table>

	<table name="ISComPlusApplicationDLL">
		<col key="yes" def="s72">ISComPlusApplicationDLL</col>
		<col def="s72">ISComPlusApplication_</col>
		<col def="s72">ISComCatalogObject_</col>
		<col def="s0">CLSID</col>
		<col def="S0">ProgId</col>
		<col def="S0">DLL</col>
		<col def="S0">AlterDLL</col>
	</table>

	<table name="ISComPlusProxy">
		<col key="yes" def="s72">ISComPlusProxy</col>
		<col def="s72">ISComPlusApplication_</col>
		<col def="S72">Component_</col>
		<col def="I2">ISAttributes</col>
		<col def="S0">DepFiles</col>
	</table>

	<table name="ISComPlusProxyDepFile">
		<col key="yes" def="s72">ISComPlusApplication_</col>
		<col key="yes" def="s72">File_</col>
		<col def="S0">ISPath</col>
	</table>

	<table name="ISComPlusProxyFile">
		<col key="yes" def="s72">File_</col>
		<col key="yes" def="s72">ISComPlusApplicationDLL_</col>
	</table>

	<table name="ISComPlusServerDepFile">
		<col key="yes" def="s72">ISComPlusApplication_</col>
		<col key="yes" def="s72">File_</col>
		<col def="S0">ISPath</col>
	</table>

	<table name="ISComPlusServerFile">
		<col key="yes" def="s72">File_</col>
		<col key="yes" def="s72">ISComPlusApplicationDLL_</col>
	</table>

	<table name="ISComponentExtended">
		<col key="yes" def="s72">Component_</col>
		<col def="I4">OS</col>
		<col def="S0">Language</col>
		<col def="s72">FilterProperty</col>
		<col def="I4">Platforms</col>
		<col def="S0">FTPLocation</col>
		<col def="S0">HTTPLocation</col>
		<col def="S0">Miscellaneous</col>
		<row><td>C__082C4A2B51334D24A3AC6790D6C8512B</td><td/><td/><td>_A8315EE3_A586_4D5A_9BFB_2D1EEF20161D_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__08B0E42E0AE7406A98786249EDB2B56E</td><td/><td/><td>_9672EF6E_29DD_4C86_AFC2_347815E50E69_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__0BBF4D1B61464202AA9E41FF332450DB</td><td/><td/><td>_C5CE62C9_8375_4881_8EB0_AC0377783DCA_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__122CBDCF2BDA4DE9881ADCC656F030C1</td><td/><td/><td>_948C97CE_5568_436F_9CED_32B905F0149A_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__127D62C316F2443D94ACD0038D656836</td><td/><td/><td>_344D4C31_8F30_46ED_B8C1_3B4FE4B59E2B_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__1346B53B2129406687E96EC782FD6B14</td><td/><td/><td>_F3278C1D_B914_4425_B0A2_C67AB77F3EAA_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__137C441D09AE4FB5B9C67421B48492C1</td><td/><td/><td>_2BC78EAB_4D56_4D1A_A458_2BE198A52C06_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__14EF4C63E3B5454D8D5A69C3C590EB86</td><td/><td/><td>_8C12280E_FB26_4A8E_865C_7C740D5DD4AE_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__1615A4B06DC04A4BAF33E4CA968A4EF4</td><td/><td/><td>_94FCC248_8606_46B2_810B_6BC52B093E51_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__1C07066DA2C8474AB22F5246BE023333</td><td/><td/><td>_3AE3685E_1FF9_4244_8888_122BCCFB567A_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__1D5D687FCC234E39ADEACC9EFBBD2AB7</td><td/><td/><td>_3213DF38_C0F3_4A75_BC06_2270464257E8_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__25E09A9CCD224D31881BE31AB87732A3</td><td/><td/><td>_8CF16CC7_E177_4BFD_8A0E_D2F68EDB873A_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__29C12E55443848608C72F3AD69AAB824</td><td/><td/><td>_1E8E95D1_E5A5_4265_8CD1_AC1BE08E5A30_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__2A0D3AF6F7264C9A8D50E89172C4A81E</td><td/><td/><td>_617C8FC7_42C7_4E49_ADF2_57278EF50575_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__31F1DADB0D4C4DB1A4A4487740F0A412</td><td/><td/><td>_F1491DA1_39A6_4359_9FCB_01E486322EDE_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__34CD49E7DDF94F629770F9F61A0F4957</td><td/><td/><td>_D298A425_AA38_4474_A1E4_3F9B8A65CCBB_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__40FF87838DE4496EBC8A597169C87209</td><td/><td/><td>_EBE9300A_1241_46F6_AA7A_7CCA5BFCEBAC_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__423104302EE74832984738AC1F76C5AE</td><td/><td/><td>_C903550D_A02F_4238_9CE0_9857D5162A4C_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__428099970CD44B46A98E2226D653EE63</td><td/><td/><td>_9589E9B5_04CD_49C2_9720_05608DA983D8_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__431D7DF568F44C0499C79B40A1BF6E12</td><td/><td/><td>_B026053B_C1E4_48E2_AD3B_26E3F80366BD_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__4BCBDC7592FA4B638AD6631DDD7D122A</td><td/><td/><td>_B4D0F6DE_4D82_4744_A66B_33D1BA43E44B_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__4D66453340ED4B598F05FE3CAD8329D4</td><td/><td/><td>_18F79E39_AF26_4B94_9EB2_DA3165F06B8C_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__4F10DC0F614541B6ADC8623FE3862D1C</td><td/><td/><td>_E87D8FA1_BB31_4C89_A41C_58F47AA8F56C_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__4F4DA107785E44E581213838B3497E30</td><td/><td/><td>_9833505C_A005_4716_99B8_CA4C29D0CA5A_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__578DC43FDCB1484794895CECAEE03768</td><td/><td/><td>_5AFCFAE1_9313_4AE9_ABA5_60BD962549C8_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__5AB0F74B62D24B2797103046CCAAD98E</td><td/><td/><td>_AD81C207_39FF_4810_881F_DE4946C11DB3_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__5C6E7AA0836C41DB831039A34D3D7AA0</td><td/><td/><td>_FA13F6FB_A27D_4EFD_9DAB_1F8695BE0D6D_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__5CB24DDE141142B7B0F65A0B5670CFB3</td><td/><td/><td>_A76A9D05_4CAA_4435_A18D_E6D33B68D084_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__5F2121D4FBDE491F8A7129617E991A99</td><td/><td/><td>_A50A996F_EA7F_46B5_A901_EB1ABC247955_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__6185CEAD036144B69828C2BA1357F7C7</td><td/><td/><td>_B8A2EF42_381E_481D_A20A_02FE650A0D16_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__70BD166A8637415AB595011E5C8869C5</td><td/><td/><td>_FD851A6F_27D2_40FD_843C_700FB41F643B_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__70DF0F53D1CE41E184D5BC1D21498AA9</td><td/><td/><td>_D2E6D3AB_3428_4AB2_8424_61942A98F0B3_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__72CE03E984304A3F8369648DA0BBEC90</td><td/><td/><td>_C66071A2_5B68_42F8_8F8D_0C97B4456C13_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__72E841FC40C949838CF7B48362C035BC</td><td/><td/><td>_5AB4008F_B48F_424C_B2F0_53CC8F7DD070_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__76A73657BC264AD09EEDE2B6EB407B17</td><td/><td/><td>_DA8521F9_43C7_41C7_BB9E_B255C258C377_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__8306BB83A4434E08BC8580B175683D54</td><td/><td/><td>_B933581E_2671_4AAA_B36F_94E47ADB8A9A_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__830F99D2CA044919B0C0507A0D17980E</td><td/><td/><td>_51BB58A2_C6F0_4ABF_B92A_802BF0B40F05_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__843F7632BEC24FBE8BF7C63DEFBF0687</td><td/><td/><td>_68476BFD_D4CC_4E0B_896F_174A59437962_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__9013468A11E6405391A6F141B889603B</td><td/><td/><td>_D9D9EF77_C51E_43D9_BD00_71B54407F308_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__92867AD4217F4FD89E7D44B5A0C02773</td><td/><td/><td>_6C9D651B_BA62_466C_B52E_44EA96E18984_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__96B0F2B1BDAF40E2AB16F98C98A045A8</td><td/><td/><td>_07AD8812_1C1B_4160_921C_CE283DB79832_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__984DB5E2EFED4B68AFC611CCD78EE906</td><td/><td/><td>_66AB1468_D200_4019_94D0_48ED9CC3198A_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__9CC9C8957FF0430099D06B9A6E37CB44</td><td/><td/><td>_45246D14_B966_4A09_A304_25D64138A865_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__A1C4795563494325A174FC7F2F7E25CE</td><td/><td/><td>_C2D35825_C726_4C9E_8DFB_3D89605F1C8B_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__A24AF738FF7547978A68111A0B90EF5E</td><td/><td/><td>_CC941507_28FD_4190_9788_9C6387E50776_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__A29FEDE025A24BEC89F0EAC20DAEA4CD</td><td/><td/><td>_4A9F1735_233F_4514_90FD_E3120B22D9C4_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__A2A8E787524045DBA36D5BDB7A77E13F</td><td/><td/><td>_8F16B39E_7321_46FE_8C52_5AB2AE7AB763_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__A2DED2051612450A9E2962309C61F19C</td><td/><td/><td>_D508B3B6_C2CB_41DF_8AE8_1CD3ACFCCFE7_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__A473537B6DB04D899D6E1FD1979FA3CF</td><td/><td/><td>_941030B7_1EA4_4D1C_A7E6_16176E0E5F35_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__A530EDAA5E624663ABB83D59D42186F6</td><td/><td/><td>_74B101DA_A997_470D_830B_86156E18D0D8_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__A6AFAFB1B104453FADE31BE15EA14353</td><td/><td/><td>_3582FA98_6556_4439_9C25_11D230A1AC50_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__B099E4926AFE43EAB395438D0D8D9B77</td><td/><td/><td>_BB37C732_146F_4704_85FE_658CE0DCA538_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__B39748BC31D6422EBB6B2D72123E92AA</td><td/><td/><td>_8304093D_D448_4013_A857_33DBFD24F842_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__BBCC268121A94302A63046EF90320A3A</td><td/><td/><td>_C8CCC37F_40D0_4D63_ACE3_2378291D6637_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__C230765F762B47B18C7DBED550986ABA</td><td/><td/><td>_AF7DAC5E_776C_47DA_9870_E9AA893B554F_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__C3C4913F460544FE9F4C0E2E900DB356</td><td/><td/><td>_5DBFFECB_096A_4156_9F56_FE2422586BBA_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__C403BA6E9E194CD592A7C50EDE7E939B</td><td/><td/><td>_6BB3E28F_AECF_45F2_BA8C_6D2AC2D81BE6_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__D1F8CF020B5A451697624F490F28BE54</td><td/><td/><td>_3DE71E53_CB4B_47CC_9642_080F0D2CA000_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__D3758339EA714A17984DE22F4644FEE9</td><td/><td/><td>_95D981BD_722B_4A95_BC2A_CC6528B4D5A3_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__D47657BBBCC549C9A4DA838A1DCD2DF4</td><td/><td/><td>_240B0C21_ABDE_4127_B5FA_C333BAC44815_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__DB2EB634C1AB4C1FA228A22582375EA2</td><td/><td/><td>_4741C33A_BB37_4165_AB60_1F4AEDF27EEB_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__E22EF8970F184A29A67C6B5080E0E021</td><td/><td/><td>_714B4CD3_12EC_4716_B383_0BE7187EF693_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__ECA4F69504D14AFCA9E7F72A8D3B7013</td><td/><td/><td>_4B0565AB_2E72_40C8_B8AB_0A36AAE9A69A_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__EFD5EB60662546EF86AC8E0A611C4CA6</td><td/><td/><td>_10C86481_E7D1_435E_8FEC_DFD72D21CDE7_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__F2555C2B1D7C49559BC04823656E2305</td><td/><td/><td>_86617BC7_9D6F_412A_B828_DD77295B5BA3_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__F3D05398C3BA40C69E8709F43B4E3083</td><td/><td/><td>_06B94981_3ED2_46D8_8AB9_FC643C2BC7CD_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__F4112FDF846B48FB86CE69A0545E366F</td><td/><td/><td>_835BB984_183E_4A24_95B0_92B1FCD0BA00_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__F58DFAEFADA3442B9526AA9947119E11</td><td/><td/><td>_D2103268_ECB5_4E43_AFE4_D221C29C6185_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__FAF9F28B10E04850937D47023FEB058A</td><td/><td/><td>_04B53BBE_1932_4A69_8F31_B0FAC5DF0146_FILTER</td><td/><td/><td/><td/></row>
		<row><td>C__FEB18844F3D54D3CBA46FE552A79F646</td><td/><td/><td>_C92CCC76_BE2B_41B4_947F_6A928F38FD19_FILTER</td><td/><td/><td/><td/></row>
		<row><td>CompBase.dll</td><td/><td/><td>_F540C8A7_1A3C_424C_B39B_B7030AA843E5_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ExprEval.dll</td><td/><td/><td>_3DFD4803_1B9C_4EA5_BA18_B0EB650271B5_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT</td><td/><td/><td>_8665D77B_58ED_4872_8015_61B6A0D14997_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT1</td><td/><td/><td>_143D32A5_6A76_4ED4_9187_B73DADD53E8C_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT10</td><td/><td/><td>_23A511A1_0F83_4A56_B4E0_6CF11014FE1C_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT11</td><td/><td/><td>_715FE5D4_E764_45F2_B8E0_7AD6391BF689_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT12</td><td/><td/><td>_313F8EC0_C9DF_4139_B0D8_62A3F9FD4249_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT13</td><td/><td/><td>_EED78025_7518_4291_9697_36E3B8E5ABF6_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT14</td><td/><td/><td>_BEFB657C_BB57_4176_B137_24F6E01D086A_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT15</td><td/><td/><td>_096FF752_CF0C_4C0F_A2B4_05F3A9A5EF29_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT16</td><td/><td/><td>_48D9B454_8D0C_4FE3_AD28_6996EF97937A_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT17</td><td/><td/><td>_6A1B4194_A438_4025_900C_1997431B3C93_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT18</td><td/><td/><td>_C9455014_C4C4_42A4_8B13_69ED8C948F3B_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT19</td><td/><td/><td>_2C6F61FF_1282_46AE_AD55_8493746DBEFA_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT2</td><td/><td/><td>_3758E1A9_455C_4670_B074_CDF4C81DDE60_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT20</td><td/><td/><td>_918BA777_2289_4305_8F54_EE0D46983AB0_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT21</td><td/><td/><td>_EEEB3664_3097_4D64_90B5_BA2815382618_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT22</td><td/><td/><td>_5555C6B9_7F36_44A0_979E_93052960490A_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT23</td><td/><td/><td>_B0A0E9E9_1973_4876_8E61_679F66AFEE31_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT24</td><td/><td/><td>_866CDDFC_A951_4268_9B6A_57C5B7F6A8DC_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT25</td><td/><td/><td>_1E0FB615_9308_4A5E_9CAC_2D4B3DBC195C_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT26</td><td/><td/><td>_A9F4EB62_032E_4B5B_AD0A_6BD079AAAF49_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT27</td><td/><td/><td>_E6E1A76C_124B_48A6_AB72_CC3F4013E593_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT28</td><td/><td/><td>_097328D0_E79A_4E5E_B1E2_6C2F67F74B1C_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT29</td><td/><td/><td>_FACD4C7F_6DF8_44D2_94DB_EA1B01416137_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT3</td><td/><td/><td>_3F637A23_740F_4EC4_AAFD_9A1D5A365472_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT30</td><td/><td/><td>_179286D4_FDC7_4078_9A47_8B3027E5C9A2_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT31</td><td/><td/><td>_A1BC195A_CD95_4E96_A276_A4F80AAA56F3_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT32</td><td/><td/><td>_50685A3F_2157_4C31_8992_14F2E6C5F9A7_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT33</td><td/><td/><td>_EDD2D3E5_0490_4820_B99C_AF16BEBE7A9F_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT35</td><td/><td/><td>_A29A7A95_FAE3_44F2_8859_B4BEF5FC3959_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT36</td><td/><td/><td>_94D3AB94_4B9D_4FBD_8FDF_0EEF02748301_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT37</td><td/><td/><td>_7D10C66F_D9E6_447C_99F4_C9696156A473_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT38</td><td/><td/><td>_4B15152A_2785_4D91_A3DC_6C5BD605789C_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT39</td><td/><td/><td>_076083F7_86EF_444A_BB2D_183B76F67E77_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT4</td><td/><td/><td>_3BA853F1_B017_4D6D_A1D5_AE68A0DD88CE_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT40</td><td/><td/><td>_C2424390_E4E5_44CC_8F9F_F27CADA985CB_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT41</td><td/><td/><td>_E23F13CF_AF08_4AF4_B7D8_693732C62BF7_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT42</td><td/><td/><td>_7C0F8EAB_2510_4989_BAD0_63389E4C68E2_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT43</td><td/><td/><td>_41DE54DE_0D9C_4DF5_9BDB_A62E675DD55B_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT5</td><td/><td/><td>_43FA3D66_CD4C_4C6A_A1E4_AB59566B0BDF_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT6</td><td/><td/><td>_4D86678B_AC8F_455C_8A86_CEBF4487A343_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT7</td><td/><td/><td>_384944F3_0A3F_4E62_83F3_B97856EF962E_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT8</td><td/><td/><td>_7EF2EF89_A8FA_43B4_ADB3_70C2EC5BD0CC_FILTER</td><td/><td/><td/><td/></row>
		<row><td>ISX_DEFAULTCOMPONENT9</td><td/><td/><td>_6253A782_259A_4B81_A550_A6B4D348DEB3_FILTER</td><td/><td/><td/><td/></row>
		<row><td>NCMHandlers.dll</td><td/><td/><td>_54CE3290_9AD4_42CE_87D1_780C33C20A5E_FILTER</td><td/><td/><td/><td/></row>
		<row><td>NCManager.chm</td><td/><td/><td>_44B6D9BE_F9FB_400D_BF1F_6DC242896CC2_FILTER</td><td/><td/><td/><td/></row>
		<row><td>NCManager.chm1</td><td/><td/><td>_02E3F4B4_17AC_4544_905F_550E9515DEFA_FILTER</td><td/><td/><td/><td/></row>
		<row><td>OCImport.dll</td><td/><td/><td>_F9E8FD19_B57F_4999_BB8F_93899B001D73_FILTER</td><td/><td/><td/><td/></row>
		<row><td>TKBO.dll</td><td/><td/><td>_A9FF4D36_F1BB_4663_AC08_19C30AD5FE47_FILTER</td><td/><td/><td/><td/></row>
		<row><td>TKBRep.dll</td><td/><td/><td>_0FF6564D_CE23_47DC_B1AA_F442D653F02C_FILTER</td><td/><td/><td/><td/></row>
		<row><td>TKBool.dll</td><td/><td/><td>_74BD69FE_75F2_4ED5_A32A_096519A4BBEB_FILTER</td><td/><td/><td/><td/></row>
		<row><td>TKG2d.dll</td><td/><td/><td>_AEDF0366_EE3E_4FD4_A540_B281B50CF17B_FILTER</td><td/><td/><td/><td/></row>
		<row><td>TKG3d.dll</td><td/><td/><td>_BB6B1499_0999_41B4_A9B9_C608FF52D347_FILTER</td><td/><td/><td/><td/></row>
		<row><td>TKGeomAlgo.dll</td><td/><td/><td>_F84D9CBF_E606_407A_8AE3_A8D4DB9AD2FB_FILTER</td><td/><td/><td/><td/></row>
		<row><td>TKGeomBase.dll</td><td/><td/><td>_B3380898_F78C_4E10_8596_A53E8B3494B7_FILTER</td><td/><td/><td/><td/></row>
		<row><td>TKIGES.dll</td><td/><td/><td>_3406A500_6C22_4953_8E55_BDF27A763164_FILTER</td><td/><td/><td/><td/></row>
		<row><td>TKMath.dll</td><td/><td/><td>_D7369175_ABE9_4C4C_82A1_BEBDDB610383_FILTER</td><td/><td/><td/><td/></row>
		<row><td>TKMesh.dll</td><td/><td/><td>_CF9113D5_C3BC_4F58_997D_B443BEEAE2FB_FILTER</td><td/><td/><td/><td/></row>
		<row><td>TKPrim.dll</td><td/><td/><td>_C9500C84_D3C2_431A_81AE_B8B251C2542D_FILTER</td><td/><td/><td/><td/></row>
		<row><td>TKSTEP.dll</td><td/><td/><td>_EA9D2AE4_CE98_4B13_9841_C5F41FAFF9B4_FILTER</td><td/><td/><td/><td/></row>
		<row><td>TKSTEP209.dll</td><td/><td/><td>_E03A8DDF_D13E_4CE2_9558_7A723DC87545_FILTER</td><td/><td/><td/><td/></row>
		<row><td>TKSTEPAttr.dll</td><td/><td/><td>_D11AA027_6CA8_4217_ADF5_5D61C6377244_FILTER</td><td/><td/><td/><td/></row>
		<row><td>TKSTEPBase.dll</td><td/><td/><td>_D516E808_477E_4015_AAEB_71B014E56985_FILTER</td><td/><td/><td/><td/></row>
		<row><td>TKSTL.dll</td><td/><td/><td>_CFA582B5_1C5A_4793_8EFE_4F1EB4125C1A_FILTER</td><td/><td/><td/><td/></row>
		<row><td>TKShHealing.dll</td><td/><td/><td>_102CBFFD_EF61_44DD_87AA_E568731B7040_FILTER</td><td/><td/><td/><td/></row>
		<row><td>TKTopAlgo.dll</td><td/><td/><td>_237FBB7D_CD61_4567_B83F_1B47218BC361_FILTER</td><td/><td/><td/><td/></row>
		<row><td>TKXSBase.dll</td><td/><td/><td>_9F06C470_043B_46CE_BF7E_FEAD687A7888_FILTER</td><td/><td/><td/><td/></row>
		<row><td>TKernel.dll</td><td/><td/><td>_4BD5DB15_D445_4552_B6DB_FF22C4F01763_FILTER</td><td/><td/><td/><td/></row>
		<row><td>header.dll</td><td/><td/><td>_197CAC23_CB1C_4946_8FF8_B8DB2C33C2CD_FILTER</td><td/><td/><td/><td/></row>
		<row><td>zlibwapi.dll</td><td/><td/><td>_2AD82337_2F3B_4D62_BA6C_F6A6D27162A8_FILTER</td><td/><td/><td/><td/></row>
	</table>

	<table name="ISCustomActionReference">
		<col key="yes" def="s72">Action_</col>
		<col def="S0">Description</col>
		<col def="S255">FileType</col>
		<col def="S255">ISCAReferenceFilePath</col>
	</table>

	<table name="ISDIMDependency">
		<col key="yes" def="s72">ISDIMReference_</col>
		<col def="s255">RequiredUUID</col>
		<col def="S255">RequiredMajorVersion</col>
		<col def="S255">RequiredMinorVersion</col>
		<col def="S255">RequiredBuildVersion</col>
		<col def="S255">RequiredRevisionVersion</col>
	</table>

	<table name="ISDIMReference">
		<col key="yes" def="s72">ISDIMReference</col>
		<col def="S0">ISBuildSourcePath</col>
	</table>

	<table name="ISDIMReferenceDependencies">
		<col key="yes" def="s72">ISDIMReference_Parent</col>
		<col key="yes" def="s72">ISDIMDependency_</col>
	</table>

	<table name="ISDIMVariable">
		<col key="yes" def="s72">ISDIMVariable</col>
		<col def="s72">ISDIMReference_</col>
		<col def="s0">Name</col>
		<col def="S0">NewValue</col>
		<col def="I4">Type</col>
	</table>

	<table name="ISDLLWrapper">
		<col key="yes" def="s72">EntryPoint</col>
		<col def="I4">Type</col>
		<col def="s0">Source</col>
		<col def="s255">Target</col>
	</table>

	<table name="ISDRMFile">
		<col key="yes" def="s72">ISDRMFile</col>
		<col def="S72">File_</col>
		<col def="S72">ISDRMLicense_</col>
		<col def="s255">Shell</col>
	</table>

	<table name="ISDRMFileAttribute">
		<col key="yes" def="s72">ISDRMFile_</col>
		<col key="yes" def="s72">Property</col>
		<col def="S0">Value</col>
	</table>

	<table name="ISDRMLicense">
		<col key="yes" def="s72">ISDRMLicense</col>
		<col def="S255">Description</col>
		<col def="S50">ProjectVersion</col>
		<col def="I4">Attributes</col>
		<col def="S255">LicenseNumber</col>
		<col def="S255">RequestCode</col>
		<col def="S255">ResponseCode</col>
	</table>

	<table name="ISDependency">
		<col key="yes" def="S50">ISDependency</col>
		<col def="I2">Exclude</col>
	</table>

	<table name="ISDisk1File">
		<col key="yes" def="s72">ISDisk1File</col>
		<col def="s255">ISBuildSourcePath</col>
		<col def="I4">Disk</col>
	</table>

	<table name="ISDynamicFile">
		<col key="yes" def="s72">Component_</col>
		<col key="yes" def="s255">SourceFolder</col>
		<col def="I2">IncludeFlags</col>
		<col def="S0">IncludeFiles</col>
		<col def="S0">ExcludeFiles</col>
		<col def="I4">ISAttributes</col>
	</table>

	<table name="ISFeatureDIMReferences">
		<col key="yes" def="s38">Feature_</col>
		<col key="yes" def="s72">ISDIMReference_</col>
	</table>

	<table name="ISFeatureMergeModuleExcludes">
		<col key="yes" def="s38">Feature_</col>
		<col key="yes" def="s255">ModuleID</col>
		<col key="yes" def="i2">Language</col>
	</table>

	<table name="ISFeatureMergeModules">
		<col key="yes" def="s38">Feature_</col>
		<col key="yes" def="s255">ISMergeModule_</col>
		<col key="yes" def="i2">Language_</col>
	</table>

	<table name="ISFeatureSetupPrerequisites">
		<col key="yes" def="s38">Feature_</col>
		<col key="yes" def="s72">ISSetupPrerequisites_</col>
	</table>

	<table name="ISFileManifests">
		<col key="yes" def="s72">File_</col>
		<col key="yes" def="s72">Manifest_</col>
	</table>

	<table name="ISIISItem">
		<col key="yes" def="s72">ISIISItem</col>
		<col def="S72">ISIISItem_Parent</col>
		<col def="L255">DisplayName</col>
		<col def="i4">Type</col>
		<col def="S72">Component_</col>
	</table>

	<table name="ISIISProperty">
		<col key="yes" def="s72">ISIISProperty</col>
		<col key="yes" def="s72">ISIISItem_</col>
		<col def="S0">Schema</col>
		<col def="S255">FriendlyName</col>
		<col def="I4">MetaDataProp</col>
		<col def="I4">MetaDataType</col>
		<col def="I4">MetaDataUserType</col>
		<col def="I4">MetaDataAttributes</col>
		<col def="L0">MetaDataValue</col>
		<col def="I4">Order</col>
		<col def="I4">ISAttributes</col>
	</table>

	<table name="ISInstallScriptAction">
		<col key="yes" def="s72">EntryPoint</col>
		<col def="I4">Type</col>
		<col def="s72">Source</col>
		<col def="S255">Target</col>
	</table>

	<table name="ISLanguage">
		<col key="yes" def="s50">ISLanguage</col>
		<col def="I2">Included</col>
		<row><td>1033</td><td>0</td></row>
		<row><td>1049</td><td>1</td></row>
	</table>

	<table name="ISLinkerLibrary">
		<col key="yes" def="s72">ISLinkerLibrary</col>
		<col def="s255">Library</col>
		<col def="i4">Order</col>
		<row><td>isrt.obl</td><td>isrt.obl</td><td>2</td></row>
		<row><td>iswi.obl</td><td>iswi.obl</td><td>1</td></row>
	</table>

	<table name="ISLocalControl">
		<col key="yes" def="s72">Dialog_</col>
		<col key="yes" def="s50">Control_</col>
		<col key="yes" def="s50">ISLanguage_</col>
		<col def="I4">Attributes</col>
		<col def="I2">X</col>
		<col def="I2">Y</col>
		<col def="I2">Width</col>
		<col def="I2">Height</col>
		<col def="S72">Binary_</col>
		<col def="S255">ISBuildSourcePath</col>
	</table>

	<table name="ISLocalDialog">
		<col key="yes" def="S50">Dialog_</col>
		<col key="yes" def="S50">ISLanguage_</col>
		<col def="I4">Attributes</col>
		<col def="S72">TextStyle_</col>
		<col def="i2">Width</col>
		<col def="i2">Height</col>
	</table>

	<table name="ISLocalRadioButton">
		<col key="yes" def="s72">Property</col>
		<col key="yes" def="i2">Order</col>
		<col key="yes" def="s50">ISLanguage_</col>
		<col def="i2">X</col>
		<col def="i2">Y</col>
		<col def="i2">Width</col>
		<col def="i2">Height</col>
	</table>

	<table name="ISLockPermissions">
		<col key="yes" def="s72">LockObject</col>
		<col key="yes" def="s32">Table</col>
		<col key="yes" def="S255">Domain</col>
		<col key="yes" def="s255">User</col>
		<col def="I4">Permission</col>
		<col def="I4">Attributes</col>
	</table>

	<table name="ISLogicalDisk">
		<col key="yes" def="i2">DiskId</col>
		<col key="yes" def="s255">ISProductConfiguration_</col>
		<col key="yes" def="s255">ISRelease_</col>
		<col def="i2">LastSequence</col>
		<col def="L64">DiskPrompt</col>
		<col def="S255">Cabinet</col>
		<col def="S32">VolumeLabel</col>
		<col def="S32">Source</col>
	</table>

	<table name="ISLogicalDiskFeatures">
		<col key="yes" def="i2">ISLogicalDisk_</col>
		<col key="yes" def="s255">ISProductConfiguration_</col>
		<col key="yes" def="s255">ISRelease_</col>
		<col key="yes" def="S38">Feature_</col>
		<col def="i2">Sequence</col>
		<col def="I4">ISAttributes</col>
	</table>

	<table name="ISMergeModule">
		<col key="yes" def="s255">ISMergeModule</col>
		<col key="yes" def="i2">Language</col>
		<col def="s255">Name</col>
		<col def="S255">Destination</col>
		<col def="I4">ISAttributes</col>
	</table>

	<table name="ISMergeModuleCfgValues">
		<col key="yes" def="s255">ISMergeModule_</col>
		<col key="yes" def="i2">Language_</col>
		<col key="yes" def="s72">ModuleConfiguration_</col>
		<col def="L0">Value</col>
		<col def="i2">Format</col>
		<col def="L255">Type</col>
		<col def="L255">ContextData</col>
		<col def="L255">DefaultValue</col>
		<col def="I2">Attributes</col>
		<col def="L255">DisplayName</col>
		<col def="L255">Description</col>
		<col def="L255">HelpLocation</col>
		<col def="L255">HelpKeyword</col>
	</table>

	<table name="ISObject">
		<col key="yes" def="s50">ObjectName</col>
		<col def="s15">Language</col>
	</table>

	<table name="ISObjectProperty">
		<col key="yes" def="S50">ObjectName</col>
		<col key="yes" def="S50">Property</col>
		<col def="S0">Value</col>
		<col def="I2">IncludeInBuild</col>
	</table>

	<table name="ISPatchConfigImage">
		<col key="yes" def="S72">PatchConfiguration_</col>
		<col key="yes" def="s72">UpgradedImage_</col>
	</table>

	<table name="ISPatchConfiguration">
		<col key="yes" def="s72">Name</col>
		<col def="i2">CanPCDiffer</col>
		<col def="i2">CanPVDiffer</col>
		<col def="i2">IncludeWholeFiles</col>
		<col def="i2">LeaveDecompressed</col>
		<col def="i2">OptimizeForSize</col>
		<col def="i2">EnablePatchCache</col>
		<col def="S0">PatchCacheDir</col>
		<col def="i4">Flags</col>
		<col def="S0">PatchGuidsToReplace</col>
		<col def="s0">TargetProductCodes</col>
		<col def="s50">PatchGuid</col>
		<col def="s0">OutputPath</col>
		<col def="i2">MinMsiVersion</col>
		<col def="I4">Attributes</col>
	</table>

	<table name="ISPatchConfigurationProperty">
		<col key="yes" def="S72">ISPatchConfiguration_</col>
		<col key="yes" def="S50">Property</col>
		<col def="S50">Value</col>
	</table>

	<table name="ISPatchExternalFile">
		<col key="yes" def="s50">Name</col>
		<col key="yes" def="s13">ISUpgradedImage_</col>
		<col def="s72">FileKey</col>
		<col def="s255">FilePath</col>
	</table>

	<table name="ISPatchWholeFile">
		<col key="yes" def="s50">UpgradedImage</col>
		<col key="yes" def="s72">FileKey</col>
		<col def="S72">Component</col>
	</table>

	<table name="ISPathVariable">
		<col key="yes" def="s72">ISPathVariable</col>
		<col def="S255">Value</col>
		<col def="S255">TestValue</col>
		<col def="i4">Type</col>
		<row><td>CommonFilesFolder</td><td/><td/><td>1</td></row>
		<row><td>ISPROJECTDIR</td><td/><td/><td>1</td></row>
		<row><td>ISProductFolder</td><td/><td/><td>1</td></row>
		<row><td>ISProjectDataFolder</td><td/><td/><td>1</td></row>
		<row><td>ISProjectFolder</td><td/><td/><td>1</td></row>
		<row><td>ProgramFilesFolder</td><td/><td/><td>1</td></row>
		<row><td>SystemFolder</td><td/><td/><td>1</td></row>
		<row><td>WindowsFolder</td><td/><td/><td>1</td></row>
	</table>

	<table name="ISProductConfiguration">
		<col key="yes" def="s72">ISProductConfiguration</col>
		<col def="S255">ProductConfigurationFlags</col>
		<col def="I4">GeneratePackageCode</col>
		<row><td>Express</td><td/><td>1</td></row>
	</table>

	<table name="ISProductConfigurationInstance">
		<col key="yes" def="s72">ISProductConfiguration_</col>
		<col key="yes" def="i2">InstanceId</col>
		<col key="yes" def="s72">Property</col>
		<col def="s255">Value</col>
	</table>

	<table name="ISProductConfigurationProperty">
		<col key="yes" def="s72">ISProductConfiguration_</col>
		<col key="yes" def="s72">Property</col>
		<col def="L255">Value</col>
		<row><td>Express</td><td>SetupFileName</td><td>NCM50x64</td></row>
	</table>

	<table name="ISRelease">
		<col key="yes" def="s72">ISRelease</col>
		<col key="yes" def="s72">ISProductConfiguration_</col>
		<col def="s255">BuildLocation</col>
		<col def="s255">PackageName</col>
		<col def="i4">Type</col>
		<col def="s0">SupportedLanguagesUI</col>
		<col def="i4">MsiSourceType</col>
		<col def="i4">ReleaseType</col>
		<col def="s72">Platforms</col>
		<col def="S0">SupportedLanguagesData</col>
		<col def="s6">DefaultLanguage</col>
		<col def="i4">SupportedOSs</col>
		<col def="s50">DiskSize</col>
		<col def="i4">DiskSizeUnit</col>
		<col def="i4">DiskClusterSize</col>
		<col def="S0">ReleaseFlags</col>
		<col def="i4">DiskSpanning</col>
		<col def="S255">SynchMsi</col>
		<col def="s255">MediaLocation</col>
		<col def="S255">URLLocation</col>
		<col def="S255">DigitalURL</col>
		<col def="S255">DigitalPVK</col>
		<col def="S255">DigitalSPC</col>
		<col def="S255">Password</col>
		<col def="S255">VersionCopyright</col>
		<col def="i4">Attributes</col>
		<col def="S255">CDBrowser</col>
		<col def="S255">DotNetBuildConfiguration</col>
		<col def="S255">MsiCommandLine</col>
		<col def="I4">ISSetupPrerequisiteLocation</col>
		<row><td>CD_ROM</td><td>Express</td><td>&lt;ISProjectDataFolder&gt;</td><td>Default</td><td>0</td><td>1049</td><td>0</td><td>2</td><td>Intel</td><td/><td>1049</td><td>0</td><td>650</td><td>0</td><td>2048</td><td/><td>0</td><td/><td>MediaLocation</td><td/><td>http://</td><td/><td/><td/><td/><td>75805</td><td/><td/><td/><td>3</td></row>
		<row><td>Custom</td><td>Express</td><td>&lt;ISProjectDataFolder&gt;</td><td>Default</td><td>2</td><td>1033</td><td>0</td><td>2</td><td>Intel</td><td/><td>1033</td><td>0</td><td>100</td><td>0</td><td>1024</td><td/><td>0</td><td/><td>MediaLocation</td><td/><td>http://</td><td/><td/><td/><td/><td>75805</td><td/><td/><td/><td>3</td></row>
		<row><td>DVD-10</td><td>Express</td><td>&lt;ISProjectDataFolder&gt;</td><td>Default</td><td>3</td><td>1033</td><td>0</td><td>2</td><td>Intel</td><td/><td>1033</td><td>0</td><td>8.75</td><td>1</td><td>2048</td><td/><td>0</td><td/><td>MediaLocation</td><td/><td>http://</td><td/><td/><td/><td/><td>75805</td><td/><td/><td/><td>3</td></row>
		<row><td>DVD-18</td><td>Express</td><td>&lt;ISProjectDataFolder&gt;</td><td>Default</td><td>3</td><td>1033</td><td>0</td><td>2</td><td>Intel</td><td/><td>1033</td><td>0</td><td>15.83</td><td>1</td><td>2048</td><td/><td>0</td><td/><td>MediaLocation</td><td/><td>http://</td><td/><td/><td/><td/><td>75805</td><td/><td/><td/><td>3</td></row>
		<row><td>DVD-5</td><td>Express</td><td>&lt;ISProjectDataFolder&gt;</td><td>Default</td><td>3</td><td>1049</td><td>0</td><td>2</td><td>Intel</td><td/><td>1049</td><td>0</td><td>4.38</td><td>1</td><td>2048</td><td/><td>0</td><td/><td>MediaLocation</td><td/><td>http://</td><td/><td/><td/><td/><td>75805</td><td/><td/><td/><td>3</td></row>
		<row><td>DVD-9</td><td>Express</td><td>&lt;ISProjectDataFolder&gt;</td><td>Default</td><td>3</td><td>1033</td><td>0</td><td>2</td><td>Intel</td><td/><td>1033</td><td>0</td><td>7.95</td><td>1</td><td>2048</td><td/><td>0</td><td/><td>MediaLocation</td><td/><td>http://</td><td/><td/><td/><td/><td>75805</td><td/><td/><td/><td>3</td></row>
		<row><td>SingleImage</td><td>Express</td><td>&lt;ISProjectDataFolder&gt;</td><td>PackageName</td><td>1</td><td>1049</td><td>0</td><td>1</td><td>Intel</td><td/><td>1049</td><td>2</td><td>0</td><td>0</td><td>0</td><td/><td>0</td><td/><td>MediaLocation</td><td/><td>http://</td><td/><td/><td/><td/><td>108573</td><td/><td/><td/><td>3</td></row>
		<row><td>WebDeployment</td><td>Express</td><td>&lt;ISProjectDataFolder&gt;</td><td>PackageName</td><td>4</td><td>1033</td><td>2</td><td>1</td><td>Intel</td><td/><td>1033</td><td>0</td><td>0</td><td>0</td><td>0</td><td/><td>0</td><td/><td>MediaLocation</td><td/><td>http://</td><td/><td/><td/><td/><td>124941</td><td/><td/><td/><td>3</td></row>
	</table>

	<table name="ISReleaseASPublishInfo">
		<col key="yes" def="s72">ISRelease_</col>
		<col key="yes" def="s72">ISProductConfiguration_</col>
		<col key="yes" def="S0">Property</col>
		<col def="S0">Value</col>
	</table>

	<table name="ISReleaseExtended">
		<col key="yes" def="s72">ISRelease_</col>
		<col key="yes" def="s72">ISProductConfiguration_</col>
		<col def="I4">WebType</col>
		<col def="S255">WebURL</col>
		<col def="I4">WebCabSize</col>
		<col def="S255">OneClickCabName</col>
		<col def="S255">OneClickHtmlName</col>
		<col def="S255">WebLocalCachePath</col>
		<col def="I4">EngineLocation</col>
		<col def="S255">Win9xMsiUrl</col>
		<col def="S255">WinNTMsiUrl</col>
		<col def="I4">ISEngineLocation</col>
		<col def="S255">ISEngineURL</col>
		<col def="I4">OneClickTargetBrowser</col>
		<col def="S255">DigitalCertificateIdNS</col>
		<col def="S255">DigitalCertificateDBaseNS</col>
		<col def="S255">DigitalCertificatePasswordNS</col>
		<col def="I4">DotNetRedistLocation</col>
		<col def="S255">DotNetRedistURL</col>
		<col def="I4">DotNetVersion</col>
		<col def="S255">DotNetBaseLanguage</col>
		<col def="S0">DotNetLangaugePacks</col>
		<col def="S255">DotNetFxCmdLine</col>
		<col def="S255">DotNetLangPackCmdLine</col>
		<col def="S50">JSharpCmdLine</col>
		<col def="I4">Attributes</col>
		<col def="I4">JSharpRedistLocation</col>
		<col def="I4">MsiEngineVersion</col>
		<col def="S255">WinMsi30Url</col>
		<col def="S255">CertPassword</col>
		<row><td>CD_ROM</td><td>Express</td><td>0</td><td>http://</td><td>0</td><td>install</td><td>install</td><td>[LocalAppDataFolder]Downloaded Installations</td><td>0</td><td>http://www.installengine.com/Msiengine20</td><td>http://www.installengine.com/Msiengine20</td><td>0</td><td>http://www.installengine.com/cert05/isengine</td><td>0</td><td/><td/><td/><td>3</td><td>http://www.installengine.com/cert05/dotnetfx</td><td>0</td><td>1033</td><td/><td/><td/><td/><td/><td>3</td><td/><td>http://www.installengine.com/Msiengine30</td><td/></row>
		<row><td>Custom</td><td>Express</td><td>0</td><td>http://</td><td>0</td><td>install</td><td>install</td><td>[LocalAppDataFolder]Downloaded Installations</td><td>0</td><td>http://www.installengine.com/Msiengine20</td><td>http://www.installengine.com/Msiengine20</td><td>0</td><td>http://www.installengine.com/cert05/isengine</td><td>0</td><td/><td/><td/><td>3</td><td>http://www.installengine.com/cert05/dotnetfx</td><td>0</td><td>1033</td><td/><td/><td/><td/><td/><td>3</td><td/><td>http://www.installengine.com/Msiengine30</td><td/></row>
		<row><td>DVD-10</td><td>Express</td><td>0</td><td>http://</td><td>0</td><td>install</td><td>install</td><td>[LocalAppDataFolder]Downloaded Installations</td><td>0</td><td>http://www.installengine.com/Msiengine20</td><td>http://www.installengine.com/Msiengine20</td><td>0</td><td>http://www.installengine.com/cert05/isengine</td><td>0</td><td/><td/><td/><td>3</td><td>http://www.installengine.com/cert05/dotnetfx</td><td>0</td><td>1033</td><td/><td/><td/><td/><td/><td>3</td><td/><td>http://www.installengine.com/Msiengine30</td><td/></row>
		<row><td>DVD-18</td><td>Express</td><td>0</td><td>http://</td><td>0</td><td>install</td><td>install</td><td>[LocalAppDataFolder]Downloaded Installations</td><td>0</td><td>http://www.installengine.com/Msiengine20</td><td>http://www.installengine.com/Msiengine20</td><td>0</td><td>http://www.installengine.com/cert05/isengine</td><td>0</td><td/><td/><td/><td>3</td><td>http://www.installengine.com/cert05/dotnetfx</td><td>0</td><td>1033</td><td/><td/><td/><td/><td/><td>3</td><td/><td>http://www.installengine.com/Msiengine30</td><td/></row>
		<row><td>DVD-5</td><td>Express</td><td>0</td><td>http://</td><td>0</td><td>install</td><td>install</td><td>[LocalAppDataFolder]Downloaded Installations</td><td>0</td><td>http://www.installengine.com/Msiengine20</td><td>http://www.installengine.com/Msiengine20</td><td>0</td><td>http://www.installengine.com/cert05/isengine</td><td>0</td><td/><td/><td/><td>3</td><td>http://www.installengine.com/cert05/dotnetfx</td><td>0</td><td>1033</td><td/><td/><td/><td/><td/><td>3</td><td/><td>http://www.installengine.com/Msiengine30</td><td/></row>
		<row><td>DVD-9</td><td>Express</td><td>0</td><td>http://</td><td>0</td><td>install</td><td>install</td><td>[LocalAppDataFolder]Downloaded Installations</td><td>0</td><td>http://www.installengine.com/Msiengine20</td><td>http://www.installengine.com/Msiengine20</td><td>0</td><td>http://www.installengine.com/cert05/isengine</td><td>0</td><td/><td/><td/><td>3</td><td>http://www.installengine.com/cert05/dotnetfx</td><td>0</td><td>1033</td><td/><td/><td/><td/><td/><td>3</td><td/><td>http://www.installengine.com/Msiengine30</td><td/></row>
		<row><td>SingleImage</td><td>Express</td><td>0</td><td>http://</td><td>0</td><td>install</td><td>install</td><td>[LocalAppDataFolder]Downloaded Installations</td><td>1</td><td>http://www.installengine.com/Msiengine20</td><td>http://www.installengine.com/Msiengine20</td><td>0</td><td>http://www.installengine.com/cert05/isengine</td><td>0</td><td/><td/><td/><td>3</td><td>http://www.installengine.com/cert05/dotnetfx</td><td>0</td><td>1033</td><td/><td/><td/><td/><td/><td>3</td><td/><td>http://www.installengine.com/Msiengine30</td><td/></row>
		<row><td>WebDeployment</td><td>Express</td><td>0</td><td>http://</td><td>0</td><td>setup</td><td>Default</td><td>[LocalAppDataFolder]Downloaded Installations</td><td>2</td><td>http://www.Installengine.com/Msiengine20</td><td>http://www.Installengine.com/Msiengine20</td><td>0</td><td>http://www.installengine.com/cert05/isengine</td><td>0</td><td/><td/><td/><td>3</td><td>http://www.installengine.com/cert05/dotnetfx</td><td>0</td><td>1033</td><td/><td/><td/><td/><td/><td>3</td><td/><td>http://www.installengine.com/Msiengine30</td><td/></row>
	</table>

	<table name="ISReleaseProperty">
		<col key="yes" def="s72">ISRelease_</col>
		<col key="yes" def="s72">ISProductConfiguration_</col>
		<col key="yes" def="s72">Name</col>
		<col def="s0">Value</col>
	</table>

	<table name="ISReleasePublishInfo">
		<col key="yes" def="s72">ISRelease_</col>
		<col key="yes" def="s72">ISProductConfiguration_</col>
		<col def="S255">Repository</col>
		<col def="S255">DisplayName</col>
		<col def="S255">Publisher</col>
		<col def="S255">Description</col>
		<col def="I4">ISAttributes</col>
	</table>

	<table name="ISSQLConnection">
		<col key="yes" def="s72">ISSQLConnection</col>
		<col def="s255">Server</col>
		<col def="s255">Database</col>
		<col def="s255">UserName</col>
		<col def="s255">Password</col>
		<col def="s255">Authentication</col>
		<col def="i2">Attributes</col>
		<col def="i2">Order</col>
		<col def="S0">Comments</col>
		<col def="I4">CmdTimeout</col>
		<col def="S0">BatchSeparator</col>
		<col def="S0">ScriptVersion_Table</col>
		<col def="S0">ScriptVersion_Column</col>
	</table>

	<table name="ISSQLConnectionDBServer">
		<col key="yes" def="s72">ISSQLConnectionDBServer</col>
		<col key="yes" def="s72">ISSQLConnection_</col>
		<col key="yes" def="s72">ISSQLDBMetaData_</col>
		<col def="i2">Order</col>
	</table>

	<table name="ISSQLConnectionScript">
		<col key="yes" def="s72">ISSQLConnection_</col>
		<col key="yes" def="s72">ISSQLScriptFile_</col>
		<col def="i2">Order</col>
	</table>

	<table name="ISSQLDBMetaData">
		<col key="yes" def="s72">ISSQLDBMetaData</col>
		<col def="S0">DisplayName</col>
		<col def="S0">AdoDriverName</col>
		<col def="S0">AdoCxnDriver</col>
		<col def="S0">AdoCxnServer</col>
		<col def="S0">AdoCxnDatabase</col>
		<col def="S0">AdoCxnUserID</col>
		<col def="S0">AdoCxnPassword</col>
		<col def="S0">AdoCxnWindowsSecurity</col>
		<col def="S0">AdoCxnNetLibrary</col>
		<col def="S0">TestDatabaseCmd</col>
		<col def="S0">TestTableCmd</col>
		<col def="S0">VersionInfoCmd</col>
		<col def="S0">VersionBeginToken</col>
		<col def="S0">VersionEndToken</col>
		<col def="S0">LocalInstanceNames</col>
		<col def="S0">CreateDbCmd</col>
		<col def="S0">SwitchDbCmd</col>
		<col def="I4">ISAttributes</col>
		<col def="S0">TestTableCmd2</col>
		<col def="S0">WinAuthentUserId</col>
		<col def="S0">DsnODBCName</col>
		<col def="S0">AdoCxnPort</col>
		<col def="S0">AdoCxnAdditional</col>
		<col def="S0">QueryDatabasesCmd</col>
		<col def="S0">CreateTableCmd</col>
		<col def="S0">InsertRecordCmd</col>
		<col def="S0">SelectTableCmd</col>
		<col def="S0">ScriptVersion_Table</col>
		<col def="S0">ScriptVersion_Column</col>
		<col def="S0">ScriptVersion_ColumnType</col>
	</table>

	<table name="ISSQLRequirement">
		<col key="yes" def="s72">ISSQLRequirement</col>
		<col key="yes" def="s72">ISSQLConnection_</col>
		<col def="S15">MajorVersion</col>
		<col def="S25">ServicePackLevel</col>
		<col def="i4">Attributes</col>
		<col def="S72">ISSQLConnectionDBServer_</col>
	</table>

	<table name="ISSQLScriptError">
		<col key="yes" def="i4">ErrNumber</col>
		<col key="yes" def="S72">ISSQLScriptFile_</col>
		<col def="i2">ErrHandling</col>
		<col def="L255">Message</col>
		<col def="i2">Attributes</col>
	</table>

	<table name="ISSQLScriptFile">
		<col key="yes" def="s72">ISSQLScriptFile</col>
		<col def="s72">Component_</col>
		<col def="i2">Scheduling</col>
		<col def="L255">InstallText</col>
		<col def="L255">UninstallText</col>
		<col def="S0">ISBuildSourcePath</col>
		<col def="S0">Comments</col>
		<col def="i2">ErrorHandling</col>
		<col def="i2">Attributes</col>
		<col def="S255">Version</col>
		<col def="S255">Condition</col>
		<col def="S0">DisplayName</col>
	</table>

	<table name="ISSQLScriptImport">
		<col key="yes" def="s72">ISSQLScriptFile_</col>
		<col def="S255">Server</col>
		<col def="S255">Database</col>
		<col def="S255">UserName</col>
		<col def="S255">Password</col>
		<col def="i4">Authentication</col>
		<col def="S0">IncludeTables</col>
		<col def="S0">ExcludeTables</col>
		<col def="i4">Attributes</col>
	</table>

	<table name="ISSQLScriptReplace">
		<col key="yes" def="s72">ISSQLScriptReplace</col>
		<col key="yes" def="s72">ISSQLScriptFile_</col>
		<col def="S0">Search</col>
		<col def="S0">Replace</col>
		<col def="i4">Attributes</col>
	</table>

	<table name="ISScriptFile">
		<col key="yes" def="s255">ISScriptFile</col>
	</table>

	<table name="ISSelfReg">
		<col key="yes" def="s72">FileKey</col>
		<col def="I2">Cost</col>
		<col def="I2">Order</col>
		<col def="S50">CmdLine</col>
		<row><td>ncmhandlers.dll</td><td>1</td><td/><td/></row>
	</table>

	<table name="ISSetupFile">
		<col key="yes" def="s72">ISSetupFile</col>
		<col def="S255">FileName</col>
		<col def="V0">Stream</col>
		<col def="S50">Language</col>
		<col def="I2">Splash</col>
		<col def="S0">Path</col>
	</table>

	<table name="ISSetupPrerequisites">
		<col key="yes" def="s72">ISSetupPrerequisites</col>
		<col def="S255">ISBuildSourcePath</col>
		<col def="I2">Order</col>
		<col def="I2">ISSetupLocation</col>
		<col def="S255">ISReleaseFlags</col>
		<row><td>_0A6DC3D3_39B2_4184_95AC_039EC9598E75_</td><td>Microsoft Visual C++ 2019 Redistributable Package (x64).prq</td><td/><td>1</td><td/></row>
	</table>

	<table name="ISSetupType">
		<col key="yes" def="s38">ISSetupType</col>
		<col def="L255">Description</col>
		<col def="L255">Display_Name</col>
		<col def="i2">Display</col>
		<col def="S255">Comments</col>
		<row><td>Custom</td><td>##IDS__IsSetupTypeMinDlg_ChooseFeatures##</td><td>##IDS__IsSetupTypeMinDlg_Custom##</td><td>3</td><td/></row>
		<row><td>Minimal</td><td>##IDS__IsSetupTypeMinDlg_MinimumFeatures##</td><td>##IDS__IsSetupTypeMinDlg_Minimal##</td><td>2</td><td/></row>
		<row><td>Typical</td><td>##IDS__IsSetupTypeMinDlg_AllFeatures##</td><td>##IDS__IsSetupTypeMinDlg_Typical##</td><td>1</td><td/></row>
	</table>

	<table name="ISSetupTypeFeatures">
		<col key="yes" def="s38">ISSetupType_</col>
		<col key="yes" def="s38">Feature_</col>
		<row><td>Custom</td><td>AlwaysInstall</td></row>
		<row><td>Minimal</td><td>AlwaysInstall</td></row>
		<row><td>Typical</td><td>AlwaysInstall</td></row>
	</table>

	<table name="ISStorages">
		<col key="yes" def="s72">Name</col>
		<col def="S0">ISBuildSourcePath</col>
	</table>

	<table name="ISString">
		<col key="yes" def="s255">ISString</col>
		<col key="yes" def="s50">ISLanguage_</col>
		<col def="S0">Value</col>
		<col def="I2">Encoded</col>
		<col def="S0">Comment</col>
		<col def="I4">TimeStamp</col>
		<row><td>COMPANY_NAME</td><td>1049</td><td>INTEKS</td><td>0</td><td/><td>715803980</td></row>
		<row><td>DN_AlwaysInstall</td><td>1049</td><td>Всегда устанавливать</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDPROP_EXPRESS_LAUNCH_CONDITION_COLOR</td><td>1049</td><td>Настройки цвета системы не подходят для работы [ProductName].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDPROP_EXPRESS_LAUNCH_CONDITION_OS</td><td>1049</td><td>Операционная система не подходит для работы [ProductName].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDPROP_EXPRESS_LAUNCH_CONDITION_PROCESSOR</td><td>1049</td><td>Процессор не подходит для работы [ProductName].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDPROP_EXPRESS_LAUNCH_CONDITION_RAM</td><td>1049</td><td>Объем памяти ОЗУ не достаточен для работы [ProductName].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDPROP_EXPRESS_LAUNCH_CONDITION_SCREEN</td><td>1049</td><td>Разрешение экрана не подходит для работы [ProductName].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDPROP_SETUPTYPE_COMPACT</td><td>1049</td><td>Сокращенная</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDPROP_SETUPTYPE_COMPACT_DESC</td><td>1049</td><td>Описание сокращенной установки</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDPROP_SETUPTYPE_COMPLETE</td><td>1049</td><td>Полная</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDPROP_SETUPTYPE_COMPLETE_DESC</td><td>1049</td><td>Описание Полной установки</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDPROP_SETUPTYPE_CUSTOM</td><td>1049</td><td>Выборочная</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDPROP_SETUPTYPE_CUSTOM_DESC</td><td>1049</td><td>Описание выборочной установки</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDPROP_SETUPTYPE_CUSTOM_DESC_PRO</td><td>1049</td><td>Описание выборочной установки</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDPROP_SETUPTYPE_TYPICAL</td><td>1049</td><td>Обычная</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDPROP_SETUPTYPE_TYPICAL_DESC</td><td>1049</td><td>Описание обычной установки</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_1</td><td>1049</td><td>[1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_1b</td><td>1049</td><td>[1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_1c</td><td>1049</td><td>[1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_1d</td><td>1049</td><td>[1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_Advertising</td><td>1049</td><td>Реклама приложения</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_AllocatingRegistry</td><td>1049</td><td>Выделение памяти для реестра</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_AppCommandLine</td><td>1049</td><td>Приложение: [1], Командная строка: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_AppId</td><td>1049</td><td>Код: [1]{{, Тип: [2]}}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_AppIdAppTypeRSN</td><td>1049</td><td>Код: [1]{{, Тип: [2], Пользователи: [3], RSN: [4]}}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_Application</td><td>1049</td><td>Приложение: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_BindingExes</td><td>1049</td><td>Привязка выполняемых файлов</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_ClassId</td><td>1049</td><td>Class Id: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_ClsID</td><td>1049</td><td>Class Id: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_ComponentIDQualifier</td><td>1049</td><td>Код компонента: [1], Описатель: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_ComponentIdQualifier2</td><td>1049</td><td>Код компонента: [1], Описатель: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_ComputingSpace</td><td>1049</td><td>Вычисление необходимого свободного места</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_ComputingSpace2</td><td>1049</td><td>Вычисление необходимого свободного места</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_ComputingSpace3</td><td>1049</td><td>Вычисление необходимого свободного места</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_ContentTypeExtension</td><td>1049</td><td>Тип содержимого MIME: [1], Расширение: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_ContentTypeExtension2</td><td>1049</td><td>Тип содержимого MIME: [1], Расширение: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_CopyingNetworkFiles</td><td>1049</td><td>Копирование файлов сетевой установки</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_CopyingNewFiles</td><td>1049</td><td>Копирование новых файлов</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_CreatingDuplicate</td><td>1049</td><td>Создание дубликатов файлов</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_CreatingFolders</td><td>1049</td><td>Создание папок</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_CreatingIISRoots</td><td>1049</td><td>Создание виртуальных корней службы IIS...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_CreatingShortcuts</td><td>1049</td><td>Создание ярлыков</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_DeletingServices</td><td>1049</td><td>Удаление служб</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_EnvironmentStrings</td><td>1049</td><td>Обновление строк среды</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_EvaluateLaunchConditions</td><td>1049</td><td>Проверка условий запуска</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_Extension</td><td>1049</td><td>Расширение: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_Extension2</td><td>1049</td><td>Расширение: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_Feature</td><td>1049</td><td>Компонент: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_FeatureColon</td><td>1049</td><td>Компонент: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_File</td><td>1049</td><td>Файл: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_File2</td><td>1049</td><td>Файл: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_FileDependencies</td><td>1049</td><td>Файл: [1],  Зависимости: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_FileDir</td><td>1049</td><td>Файл: [1], Каталог: [9]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_FileDir2</td><td>1049</td><td>File: [1], Directory: [9]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_FileDir3</td><td>1049</td><td>Файл: [1], Каталог: [9]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_FileDirSize</td><td>1049</td><td>Файл: [1], Каталог: [9], Размер: [6]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_FileDirSize2</td><td>1049</td><td>File: [1],  Directory: [9],  Size: [6]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_FileDirSize3</td><td>1049</td><td>Файл: [1], Каталог: [9],  Размер: [6]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_FileDirSize4</td><td>1049</td><td>Файл: [1], Каталог: [2],  Размер: [3]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_FileDirectorySize</td><td>1049</td><td>Файл: [1], Каталог: [9],  Размер: [6]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_FileFolder</td><td>1049</td><td>Файл: [1], Папка: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_FileFolder2</td><td>1049</td><td>Файл: [1], Папка: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_FileSectionKeyValue</td><td>1049</td><td>Файл: [1], Раздел: [2],  Подраздел: [3], Значение: [4]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_FileSectionKeyValue2</td><td>1049</td><td>Файл: [1], Раздел: [2],  Подраздел: [3], Значение: [4]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_Folder</td><td>1049</td><td>Папка: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_Folder1</td><td>1049</td><td>Папка: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_Font</td><td>1049</td><td>Шрифт: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_Font2</td><td>1049</td><td>Шрифт: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_FoundApp</td><td>1049</td><td>Найдено приложение: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_FreeSpace</td><td>1049</td><td>Свободно: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_GeneratingScript</td><td>1049</td><td>Создание операций сценария для макрокоманды:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_ISLockPermissionsCost</td><td>1049</td><td>Сбор информации о правах для объектов...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_ISLockPermissionsInstall</td><td>1049</td><td>Применение информации о правах для объектов...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_InitializeODBCDirs</td><td>1049</td><td>Инициализация каталогов ODBC</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_InstallODBC</td><td>1049</td><td>Установка компонентов ODBC</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_InstallServices</td><td>1049</td><td>Установка новых служб</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_InstallingSystemCatalog</td><td>1049</td><td>Установка системного каталога</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_KeyName</td><td>1049</td><td>Раздел: [1], Имя: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_KeyNameValue</td><td>1049</td><td>Раздел: [1], Имя: [2], Значение: [3]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_LibId</td><td>1049</td><td>LibID: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_Libid2</td><td>1049</td><td>LibID: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_MigratingFeatureStates</td><td>1049</td><td>Перенос состояний свойств из связанных приложений</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_MovingFiles</td><td>1049</td><td>Перемещение файлов</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_NameValueAction</td><td>1049</td><td>Имя: [1], Значение: [2], Действие [3]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_NameValueAction2</td><td>1049</td><td>Имя: [1], Значение: [2], Действие [3]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_PatchingFiles</td><td>1049</td><td>Обновление файлов</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_ProgID</td><td>1049</td><td>ProgId: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_ProgID2</td><td>1049</td><td>ProgId: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_PropertySignature</td><td>1049</td><td>Свойство: [1], Подпись: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_PublishProductFeatures</td><td>1049</td><td>Публикация компонентов продукта</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_PublishProductInfo</td><td>1049</td><td>Публикация сведений о продукте</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_PublishingQualifiedComponents</td><td>1049</td><td>Публикация подходящих компонентов</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RegUser</td><td>1049</td><td>Регистрация пользователей</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RegisterClassServer</td><td>1049</td><td>Регистрация серверов классов</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RegisterExtensionServers</td><td>1049</td><td>Регистрация серверов расширений</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RegisterFonts</td><td>1049</td><td>Регистрация шрифтов</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RegisterMimeInfo</td><td>1049</td><td>Регистрация MIME</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RegisterTypeLibs</td><td>1049</td><td>Регистрация библиотек типов</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RegisteringComPlus</td><td>1049</td><td>Регистрация приложений и компонентов COM+</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RegisteringModules</td><td>1049</td><td>Регистрация модулей</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RegisteringProduct</td><td>1049</td><td>Регистрация продукта</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RegisteringProgIdentifiers</td><td>1049</td><td>Отмена регистрации кодов приложений</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RemoveApps</td><td>1049</td><td>Удаление приложений</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RemovingBackup</td><td>1049</td><td>Удаление временных файлов</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RemovingDuplicates</td><td>1049</td><td>Удаление дублирующихся файлов</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RemovingFiles</td><td>1049</td><td>Удаление файлов</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RemovingFolders</td><td>1049</td><td>Удаление папок</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RemovingIISRoots</td><td>1049</td><td>Удаление виртуальных корней службы IIS...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RemovingIni</td><td>1049</td><td>Удаление записей в INI-файлах</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RemovingMoved</td><td>1049</td><td>Удаление перемещенных файлов</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RemovingODBC</td><td>1049</td><td>Удаление компонентов ODBC</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RemovingRegistry</td><td>1049</td><td>Удаление параметров системного реестра</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RemovingShortcuts</td><td>1049</td><td>Удаление ярлыков</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_RollingBack</td><td>1049</td><td>Откат действия:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_SearchForRelated</td><td>1049</td><td>Поиск связанных приложений</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_SearchInstalled</td><td>1049</td><td>Поиск установленных приложений</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_SearchingQualifyingProducts</td><td>1049</td><td>Поиск подходящих продуктов</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_SearchingQualifyingProducts2</td><td>1049</td><td>Поиск подходящих продуктов</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_Service</td><td>1049</td><td>Служба: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_Service2</td><td>1049</td><td>Служба: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_Service3</td><td>1049</td><td>Служба: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_Service4</td><td>1049</td><td>Служба: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_Shortcut</td><td>1049</td><td>Ярлык: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_Shortcut1</td><td>1049</td><td>Ярлык: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_StartingServices</td><td>1049</td><td>Запуск служб</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_StoppingServices</td><td>1049</td><td>Останов служб</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_UnpublishProductFeatures</td><td>1049</td><td>Отмена публикации компонентов продукта</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_UnpublishQualified</td><td>1049</td><td>Отмена публикация подходящих компонентов</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_UnpublishingProductInfo</td><td>1049</td><td>Отмена публикация сведений о продукте</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_UnregTypeLibs</td><td>1049</td><td>Отмена регистрация библиотек типов</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_UnregisterClassServers</td><td>1049</td><td>Отмена регистрация серверов классов</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_UnregisterExtensionServers</td><td>1049</td><td>Отмена регистрация серверов расширений</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_UnregisterModules</td><td>1049</td><td>Отмена регистрации модулей</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_UnregisteringComPlus</td><td>1049</td><td>Отмена регистрации приложений и компонентов COM+</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_UnregisteringFonts</td><td>1049</td><td>Отмена регистрации шрифтов</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_UnregisteringMimeInfo</td><td>1049</td><td>Отмена регистрации MIME</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_UnregisteringProgramIds</td><td>1049</td><td>Отмена регистрации кодов приложений</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_UpdateComponentRegistration</td><td>1049</td><td>Обновление регистрации компонентов</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_UpdateEnvironmentStrings</td><td>1049</td><td>Обновление строк среды</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_Validating</td><td>1049</td><td>Проверка установки</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_WritingINI</td><td>1049</td><td>Сохранение значений в INI-файлах</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ACTIONTEXT_WritingRegistry</td><td>1049</td><td>Запись параметров системного реестра</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_BACK</td><td>1049</td><td>&lt; &amp;Назад</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_CANCEL</td><td>1049</td><td>Отмена</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_CANCEL2</td><td>1049</td><td>{&amp;Tahoma8}&amp;Отмена</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_CHANGE</td><td>1049</td><td>&amp;Изменить...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_COMPLUS_PROGRESSTEXT_COST</td><td>1049</td><td>Оценка необходимо места для приложения COM+: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_COMPLUS_PROGRESSTEXT_INSTALL</td><td>1049</td><td>Установка приложения COM+: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_COMPLUS_PROGRESSTEXT_UNINSTALL</td><td>1049</td><td>Удаление приложения COM+: [1]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_DIALOG_TEXT2_DESCRIPTION</td><td>1049</td><td>Описание - шрифт Dialog, обычный</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_DIALOG_TEXT_DESCRIPTION_EXTERIOR</td><td>1049</td><td>{&amp;TahomaBold10}Заголовок - шрифт Dialog, жирный</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_DIALOG_TEXT_DESCRIPTION_INTERIOR</td><td>1049</td><td>{&amp;MSSansBold8}Заголовок - шрифт Dialog, жирный</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_DIFX_AMD64</td><td>1049</td><td>Для [ProductName] необходим процессор X64. Для выхода из мастера нажмите кнопку OK.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_DIFX_IA64</td><td>1049</td><td>Для [ProductName] необходим процессор IA64. Для выхода из мастера нажмите кнопку ОК.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_DIFX_X86</td><td>1049</td><td>Для [ProductName] необходим процессор X86. Для выхода из мастера нажмите кнопку OK.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_DatabaseFolder_InstallDatabaseTo</td><td>1049</td><td>Установить базу данных [ProductName] в:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_0</td><td>1049</td><td>{{Неустранимая ошибка: }}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1</td><td>1049</td><td>Ошибка [1].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_10</td><td>1049</td><td>=== Начало записи журнала: [Date]  [Time] ===</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_100</td><td>1049</td><td>Не удается удалить ярлык [2]. Проверьте, что файл ярлыка существует и доступен.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_101</td><td>1049</td><td>Не удается зарегистрировать библиотеку типов для файла [2].  Обратитесь в службу поддержки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_102</td><td>1049</td><td>Не удается отменить регистрацию библиотеки типов для файла [2].  Обратитесь в службу поддержки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_103</td><td>1049</td><td>Не удается обновить ini-файл [2][3].  Проверьте, что файл существует и доступен.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_104</td><td>1049</td><td>Не удается спланировать замену файла [3] на [2] при перезагрузке.  Убедитесь, что у вас имеются разрешения на доступ к файлу [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_105</td><td>1049</td><td>Ошибка при удалении диспетчера драйвера ODBC, ошибка ODBC [2]: [3]. Обратитесь в службу поддержки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_106</td><td>1049</td><td>Ошибка при установке диспетчера драйвера ODBC, ошибка ODBC [2]: [3]. Обратитесь в службу поддержки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_107</td><td>1049</td><td>Ошибка при удалении драйвера ODBC: [4], ошибка ODBC [2]: [3]. Убедитесь, что у вас имеются разрешения на удаление драйверов ODBC.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_108</td><td>1049</td><td>Ошибка при установке драйвера ODBC: [4], ошибка ODBC [2]: [3]. Проверьте, что файл [4] существует и доступен.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_109</td><td>1049</td><td>Ошибка при настройке источника данных ODBC: [4], ошибка ODBC [2]: [3]. Проверьте, что файл [4] существует и доступен.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_11</td><td>1049</td><td>=== Завершение записи журнала: [Date]  [Time] ===</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_110</td><td>1049</td><td>Ошибка при запуске службы "[2]" ([3]).  Убедитесь, что у вас имеются разрешения на запуск системных служб.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_111</td><td>1049</td><td>Не удается остановить службу "[2]" ([3]).  Убедитесь, что у вас имеются разрешения на останов системных служб.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_112</td><td>1049</td><td>Не удается удалить службу "[2]" ([3]).  Убедитесь, что у вас имеются разрешения на удаление системных служб.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_113</td><td>1049</td><td>Не удается установить службу "[2]" ([3]).  Убедитесь, что у вас имеются разрешения на установку системных служб.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_114</td><td>1049</td><td>Не удается обновить переменную среды "[2]".  Убедитесь, что у вас имеются разрешения на изменение переменных среды.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_115</td><td>1049</td><td>У вас недостаточно полномочий для выполнения этой установки для всех пользователей данного компьютера.  Войдите в систему в качестве администратора и повторите установку.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_116</td><td>1049</td><td>Не удается установить атрибуты безопасности для файла "[3]". Ошибка: [2].  Убедитесь, что у вас достаточно полномочий для изменения разрешений на доступ к этому файлу.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_117</td><td>1049</td><td>Службы COM+ 1.0 не установлены на данном компьютере.  Они необходимы для успешной установки.  Эти службы входят в состав Windows 2000.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_118</td><td>1049</td><td>Произошла ошибка при регистрации приложения COM+.  За дополнительными сведениями обратитесь в службу поддержки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_119</td><td>1049</td><td>Произошла ошибка при отмене регистрации приложения COM+.  За дополнительными сведениями обратитесь в службу поддержки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_12</td><td>1049</td><td>Начало действия [Time]: [1].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_120</td><td>1049</td><td>Удаление более ранних версий этого приложения...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_121</td><td>1049</td><td>Подготовка к удалению более ранних версий этого приложения...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_122</td><td>1049</td><td>Произошла ошибка при обновлении файла [2].  Возможно, он уже был обновлен другими средствами, и к нему не может быть применен данный пакет исправлений.  За дополнительными сведениями обратитесь к поставщику пакета.  {{Системная ошибка: [3]}}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_123</td><td>1049</td><td>[2] не удается установить один из обязательных продуктов. Обратитесь в группу технической поддержки.  {{Системная ошибка: [3].}}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_124</td><td>1049</td><td>Не удается удалить более раннюю версию [2]. Обратитесь в группу технической поддержки.  {{Системная ошибка: [3].}}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_125</td><td>1049</td><td>Изменение описания службы  '[2]' ([3]) не допускается.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_126</td><td>1049</td><td>Службе Windows Installer не удается обновить системный файл [2], поскольку он защищен средствами Windows.  Возможно, для правильной работы программы потребуется обновление операционной системы. {{Версия пакета: [3], Защищенная версия ОС: [4]}}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_127</td><td>1049</td><td>Службе Windows Installer не удается обновить защищенный системный файл [2]. {{Версия пакета: [3], Защищенная версия ОС: [4], Ошибка SFP: [5]}}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_128</td><td>1049</td><td>Службе установщика Windows не удалось обновить один или несколько защищенных файлов Windows. Ошибка SFP: [2]. Список защищенных файлов: [3]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_129</td><td>1049</td><td>Установки пользователя отключены в соответствии с политикой на этой машине.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_13</td><td>1049</td><td>Завершение действия [Time]: [1]. Возвращенное значение [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_130</td><td>1049</td><td>Установка требует наличия IIS-сервера для осуществления конфигурации виртуальных корней IIS. Убедитесь в том, что IIS установлен.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_131</td><td>1049</td><td>Для этой установки требуются привилегии администратора для настройки виртуальных корней службы IIS.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1329</td><td>1049</td><td>Невозможно установить необходимый файл, поскольку CAB-файл [2] не имеет цифровой подписи. Это может означать, что CAB-файл поврежден.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1330</td><td>1049</td><td>Невозможно установить необходимый файл, поскольку CAB-файл [2] имеет недопустимую цифровую подпись. Возможно, CAB-файл поврежден.{ Ошибка [3] получена WinVerifyTrust.}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1331</td><td>1049</td><td>Не удалось корректно скопировать [2] файл: Ошибка CRC.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1332</td><td>1049</td><td>Не удалось корректно исправить [2] файл: Ошибка CRC.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1333</td><td>1049</td><td>Не удалось корректно исправить [2] файл: Ошибка CRC.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1334</td><td>1049</td><td>Невозможно установить файл '[2]' поскольку файл не найден CAB-файле '[3]'. Возможны: ошибка сети, ошибка чтения с компакт-диска, неполадка пакета установки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1335</td><td>1049</td><td>Невозможно использовать необходимый для данной установки CAB-файл '[2]', поскольку он поврежден. Возможны: ошибка сети, ошибка чтения с компакт-диска, неполадка пакета установки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1336</td><td>1049</td><td>Произошла ошибка при создании временного файла, необходимого для завершения установки. Папка: [3]. Код системной ошибки: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_14</td><td>1049</td><td>Оставшееся время: {[1] мин }{[2] сек}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_15</td><td>1049</td><td>Недостаточно памяти. Закройте другие приложения и повторите попытку.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_16</td><td>1049</td><td>Windows Installer не отвечает.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1609</td><td>1049</td><td>При настр. безоп. произошла ошибка. [2] - недопустимый польз./группа. Проблема м.б. связана с приложением или контроллером домена. Проверьте подключ-е и щелкните Повтор или Отмена для заверш. установки. Невозможно опр. SID пользователя, сист. ошибка [3]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1651</td><td>1049</td><td>Администратору не удалось применить исправление для управляемого пользователем или автоматического приложения в заявленном состоянии.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_17</td><td>1049</td><td>Программа Windows Installer завершена преждевременно.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1715</td><td>1049</td><td>Установлено [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1716</td><td>1049</td><td>Настроено [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1717</td><td>1049</td><td>Удалено [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1718</td><td>1049</td><td>Файл [2] был отклонен политикой цифровой подписи.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1719</td><td>1049</td><td>Не удается получить доступ к программе установки Windows. Обратитесь в службу поддержки для подтверждения регистрации и подключения.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1720</td><td>1049</td><td>Неполадки пакета установщика Windows. Невозможно запустить программу, необходимую для завершения установки. Обратитесь в службу поддержки или поставщику пакета. Действие пользователя [2] ошибка сценария [3], [4]: [5] Строка [6], Столбец [7], [8]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1721</td><td>1049</td><td>Неполадки пакета установщика Windows. Невозможно запустить программу, необходимую для завершения установки. Обратитесь в службу поддержки или к поставщику пакета. Действие: [2], местоположение: [3], команда: [4]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1722</td><td>1049</td><td>Неполадка пакета установщика Windows. Непредвиденное завершение программы, являющейся частью установки. Обратитесь в службу поддержки или к поставщику пакета. Действие [2], местоположение: [3], команда: [4]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1723</td><td>1049</td><td>Неполадка пакета установщика Windows. Невозможно запустить необходимый для завершения установки файл DLL. Обратитесь в службу поддержки или к поставщику пакета. Действие [2], свойство: [3], библиотека: [4]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1724</td><td>1049</td><td>Удаление завершено успешно.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1725</td><td>1049</td><td>Удаление не удалось.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1726</td><td>1049</td><td>Объявление завершено успешно.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1727</td><td>1049</td><td>Объявление не удалось.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1728</td><td>1049</td><td>Конфигурация завершена успешно.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1729</td><td>1049</td><td>Конфигурация не удалась.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1730</td><td>1049</td><td>Для удаления этого приложения необходимо иметь права администратора. Для удаления этого приложения войдите в систему как администратор или обратитесь в группу технической поддержки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1731</td><td>1049</td><td>Источник установочного пакета для продукта [2] не найден. Попробуйте повторить установку, используя работоспособную копию пакета '[3]'.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1732</td><td>1049</td><td>Для завершения установки [2] необходимо перезагрузить компьютер. Другие пользователи в настоящий момент вошли в систему на этом компьютере, перезагрузка может помешать их работе. Перезагрузить компьютер?</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_18</td><td>1049</td><td>Идет настройка [ProductName]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_19</td><td>1049</td><td>Сбор сведений...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1935</td><td>1049</td><td>Возникла ошибка при установке компонента сборки [2]. HRESULT: [3]. {{интерфейс сборки: [4], функция: [5], имя сборки: [6]}}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1936</td><td>1049</td><td>Возникла ошибка при установке сборки '[6]'. Сборка не строго именована или не подписана ключом минимальной длины. HRESULT: [3]. {{интерфейс сборки: [4], функция: [5], компонент: [2]}}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1937</td><td>1049</td><td>Возникла ошибка при установке сборки '[6]'. Подпись или каталог не могут быть проверены или недействительны. HRESULT: [3]. {{интерфейс сборки: [4], функция: [5], компонент: [2]}}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_1938</td><td>1049</td><td>Возникла ошибка при установке компонента сборки '[6]'. Не удается найти один или несколько модулей сборки. HRESULT: [3]. {{интерфейс сборки: [4], функция: [5], компонент: [2]}}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2</td><td>1049</td><td>Предупреждение [1]. </td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_20</td><td>1049</td><td>{[ProductName] }Установка успешно завершена.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_21</td><td>1049</td><td>{[ProductName] }Установка не выполнена.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2101</td><td>1049</td><td>Ярлыки не поддерживаются операционной системой.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2102</td><td>1049</td><td>Недопустимое действие файла INI: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2103</td><td>1049</td><td>Невозможно сопоставить путь для папки оболочек [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2104</td><td>1049</td><td>Запись файла INI: [3]: Системная ошибка: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2105</td><td>1049</td><td>Не удалось создать ярлык [3]. Системная ошибка: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2106</td><td>1049</td><td>Не удалось удалить ярлык [3]. Системная ошибка: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2107</td><td>1049</td><td>Ошибка [3] регистрации библиотеки типов [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2108</td><td>1049</td><td>Ошибка [3] отмены регистрации библиотеки типов [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2109</td><td>1049</td><td>Отсутствует раздел для действия файла INI.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2110</td><td>1049</td><td>Отсутствует ключ для действия файла INI.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2111</td><td>1049</td><td>Не удалось определить запущенные приложения, невозможно получить данные быстродействия. Операция реестра вернула код: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2112</td><td>1049</td><td>Не удалось определить запущенные приложения, невозможно получить индекс быстродействия. Операция реестра вернула код: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2113</td><td>1049</td><td>Не удалось определить запущенные приложения.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_22</td><td>1049</td><td>Ошибка чтения из файла: [2]. {{ Системная ошибка [3].}}  Проверьте, что файл существует и доступен.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2200</td><td>1049</td><td>База данных: [2]. Не удалось создать объект базы данных, режим = [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2201</td><td>1049</td><td>База данных: [2]. Инициализация не удалась, недостаточно памяти.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2202</td><td>1049</td><td>База данных: [2]. Нет доступа к данным, недостаточно памяти.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2203</td><td>1049</td><td>База данных: [2]. Невозможно открыть файл базы данных. Системная ошибка [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2204</td><td>1049</td><td>База данных: [2]. Таблица уже существует: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2205</td><td>1049</td><td>База данных: [2]. Таблица не существует: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2206</td><td>1049</td><td>База данных: [2]. Невозможно пропустить таблицу: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2207</td><td>1049</td><td>База данных: [2]. Нарушение значения.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2208</td><td>1049</td><td>База данных: [2]. Недостаточные параметры выполнения.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2209</td><td>1049</td><td>База данных: [2]. Недопустимое состояние курсора.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2210</td><td>1049</td><td>База данных: [2]. Недопустимый тип данных обновления в столбце [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2211</td><td>1049</td><td>База данных: [2]. Невозможно создать таблицу баз данных [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2212</td><td>1049</td><td>База данных: [2]. База данных не находится в состоянии записи.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2213</td><td>1049</td><td>База данных: [2]. Ошибка при сохранении таблиц баз данных.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2214</td><td>1049</td><td>База данных: [2]. Ошибка записи файла экспорта: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2215</td><td>1049</td><td>База данных: [2]. Невозможно открыть файл импорта: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2216</td><td>1049</td><td>База данных: [2]. Ошибка импорта формата файла: [3], Строка [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2217</td><td>1049</td><td>База данных: [2]. Неверное состояние для CreateOutputDatabase [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2218</td><td>1049</td><td>База данных: [2]. Не указано имя таблицы.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2219</td><td>1049</td><td>База данных: [2]. Недопустимый формат базы данных установщика.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2220</td><td>1049</td><td>База данных: [2]. Недопустимые данные строки/поля.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2221</td><td>1049</td><td>База данных: [2]. Конфликт кодовой страницы в файле импорта: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2222</td><td>1049</td><td>База данных: [2]. Преобразование или объединение кодовой страницы [3] отличается от кодовой страницы базы данных [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2223</td><td>1049</td><td>База данных: [2]. Одинаковые базы данных. Преобразование не создано.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2224</td><td>1049</td><td>База данных: [2]. GenerateTransform: База данных повреждена. Таблица: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2225</td><td>1049</td><td>База данных: [2]. Преобразовать: Невозможно преобразовать временную таблицу. Таблица: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2226</td><td>1049</td><td>База данных: [2]. Преобразование не удалось.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2227</td><td>1049</td><td>База данных: [2]. Недопустимый определитель '[3]' в запросе SQL: [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2228</td><td>1049</td><td>База данных: [2]. Неизвестная таблица '[3]' в запросе SQL: [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2229</td><td>1049</td><td>База данных: [2]. Невозможно загрузить таблицу '[3]' запроса SQL: [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2230</td><td>1049</td><td>База данных: [2]. Повтор таблицы '[3]' в запросе SQL: [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2231</td><td>1049</td><td>База данных: [2]. Отсутствует ')' в запросе SQL: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2232</td><td>1049</td><td>База данных: [2]. Недопустимый маркер '[3]' в запросе SQL: [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2233</td><td>1049</td><td>База данных: [2]. Отсутствуют столбцы в предложении SELECT запроса SQL: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2234</td><td>1049</td><td>База данных: [2]. Отсутствуют столбцы в предложении ORDER BY запроса SQL: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2235</td><td>1049</td><td>База данных: [2]. Столбец '[3]' в запросе SQL отсутствует или неоднозначен: [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2236</td><td>1049</td><td>База данных: [2]. Недопустимый оператор '[3]' в запросе SQL: [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2237</td><td>1049</td><td>База данных: [2]. Недопустимая или отсутствующая строка запроса: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2238</td><td>1049</td><td>База данных: [2]. Отсутствует предложение FROM в запросе SQL: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2239</td><td>1049</td><td>База данных: [2]. Недостаточные значения в инструкции INSERT SQL.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2240</td><td>1049</td><td>База данных: [2]. Отсутствуют столбцы обновлений в инструкции UPDATE SQL.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2241</td><td>1049</td><td>База данных: [2]. Отсутствуют столбцы вставки в инструкции INSERT SQL.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2242</td><td>1049</td><td>База данных: [2]. Столбец '[3]' повторяется.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2243</td><td>1049</td><td>База данных: [2]. Не определены первичные столбцы для создания таблицы.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2244</td><td>1049</td><td>База данных: [2]. Недопустимый определитель типа '[3]' в запросе SQL [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2245</td><td>1049</td><td>Выполнение IStorage::Stat не удалось из-за ошибки [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2246</td><td>1049</td><td>База данных: [2]. Недопустимый формат преобразования установщика.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2247</td><td>1049</td><td>База данных: [2] Ошибка чтения/записи потока преобразования.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2248</td><td>1049</td><td>База данных: [2] GenerateTransform/Merge: Тип столбца в основной таблице не соответствует таблице ссылок. Таблица: [3], Столбец №: [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2249</td><td>1049</td><td>База данных: [2] GenerateTransform: В основной таблице больше столбцов, чем в таблице ссылок. Таблица: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2250</td><td>1049</td><td>База данных: [2] Преобразовать: Невозможно добавить существующую строку. Таблица: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2251</td><td>1049</td><td>База данных: [2] Преобразовать: Невозможно удалить несуществующую строку. Таблица: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2252</td><td>1049</td><td>База данных: [2] Преобразовать: Невозможно добавить существующую таблицу. Таблица: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2253</td><td>1049</td><td>База данных: [2] Преобразовать: Невозможно удалить несуществующую таблицу. Таблица: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2254</td><td>1049</td><td>База данных: [2] Преобразовать: Невозможно обновить несуществующую строку. Таблица: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2255</td><td>1049</td><td>База данных: [2] Преобразовать: Столбец с таким именем уже существует. Таблица: [3] Столбец: [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2256</td><td>1049</td><td>База данных: [2] GenerateTransform/Merge: Количество первичных ключей в основной таблице не соответствует таблице ссылок. Таблица: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2257</td><td>1049</td><td>База данных: [2]. Попытка внесения изменений в таблицу, предназначенную только для чтения: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2258</td><td>1049</td><td>База данных: [2]. Несовпадение типа в параметре: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2259</td><td>1049</td><td>База данных: [2] Не удалось обновить таблицу (-цы)</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2260</td><td>1049</td><td>Не удалось выполнить команду CopyTo для хранилища. Системная ошибка: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2261</td><td>1049</td><td>Невозможно удалить поток [2]. Системная ошибка: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2262</td><td>1049</td><td>Поток не существует: [2]. Системная ошибка: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2263</td><td>1049</td><td>Невозможно открыть поток [2]. Системная ошибка: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2264</td><td>1049</td><td>Невозможно удалить поток [2]. Системная ошибка: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2265</td><td>1049</td><td>Невозможно сохранить хранилище. Системная ошибка: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2266</td><td>1049</td><td>Невозможно откатить хранилище. Системная ошибка: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2267</td><td>1049</td><td>Невозможно удалить хранилище [2]. Системная ошибка: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2268</td><td>1049</td><td>База данных: [2]. Объединить: Возникли конфликты объединения, о которых сообщено в [3] таблицах.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2269</td><td>1049</td><td>База данных: [2]. Объединить: Разное число столбцов в таблице '[3]' в двух базах данных.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2270</td><td>1049</td><td>База данных: [2]. GenerateTransform/Merge: Имя столбца в основной таблице не соответствует таблице ссылок. Таблица: [3], Столбец №: [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2271</td><td>1049</td><td>Не удалось записать сводную информацию для преобразования.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2272</td><td>1049</td><td>База данных: [2]. Команда MergeDatabase данных не запишет внесенные изменения, поскольку база данных открыта только для чтения.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2273</td><td>1049</td><td>База данных: [2]. MergeDatabase: Ссылка на основную базу данных передана как база данных ссылок.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2274</td><td>1049</td><td>База данных: [2]. MergeDatabase: Невозможно внести ошибки в Таблицу ошибок. Возможно, это связано тем, что столбец встроенной Таблицы ошибок не может иметь неопределенное значение.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2275</td><td>1049</td><td>База данных: [2]. Заданная операция изменения [3] невозможна для объединения таблиц.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2276</td><td>1049</td><td>База данных: [2]. Кодовая страница [3] не поддерживается системой.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2277</td><td>1049</td><td>База данных: [2]. Не удалось сохранить таблицу [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2278</td><td>1049</td><td>База данных: [2]. Превышение ограничения числа выражений в 32 в предложении WHERE запроса SQL: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2279</td><td>1049</td><td>База данных: [2] Преобразовать: Слишком много столбцов в основной таблице [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2280</td><td>1049</td><td>База данных: [2]. Невозможно создать столбец [3] в таблице [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2281</td><td>1049</td><td>Невозможно переименовать поток [2]. Системная ошибка: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2282</td><td>1049</td><td>Недопустимое имя потока [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_23</td><td>1049</td><td>Не удается создать файл "[2]".  Каталог с этим именем уже существует.  Прервите установку и укажите другой каталог.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2302</td><td>1049</td><td>Уведомление исправления: [2] байта (-ов) уже исправлено.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2303</td><td>1049</td><td>Ошибка при получении информации тома. GetLastError: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2304</td><td>1049</td><td>Ошибка поиска свободного места на диске. GetLastError: [2]. Том: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2305</td><td>1049</td><td>Ошибка ожидания потока пути. GetLastError: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2306</td><td>1049</td><td>Невозможно создать поток для приложения исправлений. GetLastError: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2307</td><td>1049</td><td>Нулевое имя ключа исходного файла.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2308</td><td>1049</td><td>Нулевое имя файла назначения.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2309</td><td>1049</td><td>Попытка исправления файла [2] при уже запущенном исправлении.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2310</td><td>1049</td><td>Попытка продолжения исправления при отсутствии запущенных исправлений.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2315</td><td>1049</td><td>Отсутствует разделитель пути: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2318</td><td>1049</td><td>Файл не существует: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2319</td><td>1049</td><td>Ошибка при установке атрибутов файла: [3] GetLastError: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2320</td><td>1049</td><td>Незаписываемый файл: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2321</td><td>1049</td><td>Ошибка при создании файла: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2322</td><td>1049</td><td>Пользователь отменен.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2323</td><td>1049</td><td>Недопустимый атрибут файла.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2324</td><td>1049</td><td>Невозможно открыть файл: [3] GetLastError: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2325</td><td>1049</td><td>Невозможно получить время файла для файла: [3] GetLastError: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2326</td><td>1049</td><td>Ошибка в FileToDosDateTime.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2327</td><td>1049</td><td>Невозможно удалить каталог: [3] GetLastError: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2328</td><td>1049</td><td>Ошибка получения информации о версии файла для файла: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2329</td><td>1049</td><td>Ошибка при удалении файла: [3]. GetLastError: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2330</td><td>1049</td><td>Ошибка при получении атрибутов файла: [3]. GetLastError: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2331</td><td>1049</td><td>Ошибка при загрузке библиотеки [2] или поиске точки входа [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2332</td><td>1049</td><td>Ошибка при получении атрибутов файла. GetLastError: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2333</td><td>1049</td><td>Ошибка при установке атрибутов файла. GetLastError: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2334</td><td>1049</td><td>Ошибка преобразования времени файла в местное время для файла: [3]. GetLastError: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2335</td><td>1049</td><td>Путь: [2] не является родительским объектом для [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2336</td><td>1049</td><td>Ошибка создания временного файла на пути: [3]. GetLastError: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2337</td><td>1049</td><td>Невозможно закрыть файл: [3] GetLastError: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2338</td><td>1049</td><td>Невозможно обновить ресурс для файла: [3] GetLastError: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2339</td><td>1049</td><td>Невозможно установить время файла для файла: [3] GetLastError: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2340</td><td>1049</td><td>Невозможно обновить ресурс для файла: [3], Отсутствующий ресурс.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2341</td><td>1049</td><td>Невозможно обновить ресурс для файла: [3], Ресурс слишком велик.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2342</td><td>1049</td><td>Невозможно обновить ресурс для файла: [3] GetLastError: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2343</td><td>1049</td><td>Указанный путь пустой.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2344</td><td>1049</td><td>Невозможно найти требуемый файл IMAGEHLP.DLL для проверки файла: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2345</td><td>1049</td><td>[2]: Файл не содержит допустимого значения контрольной суммы.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2347</td><td>1049</td><td>Отказ пользователя.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2348</td><td>1049</td><td>Ошибка при чтении потока CAB-файла.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2349</td><td>1049</td><td>Копия запущена заново с другой информацией.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2350</td><td>1049</td><td>Ошибка сервера FDI </td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2351</td><td>1049</td><td>Ключ файла '[2]' не найден в CAB-файле '[3]'. Невозможно продолжить установку.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2352</td><td>1049</td><td>Невозможно инициализировать сервер CAB-файлов. Отсутствует требуемый файл 'CABINET.DLL'.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2353</td><td>1049</td><td>Не CAB-файл.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2354</td><td>1049</td><td>Невозможно применить дескриптор к CAB-файлу.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2355</td><td>1049</td><td>CAB-файл поврежден.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2356</td><td>1049</td><td>Невозможно определить CAB-файл в потоке: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2357</td><td>1049</td><td>Невозможно установить атрибуты.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2358</td><td>1049</td><td>Определение ошибки в процессе использования файла: [3]. GetLastError: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2359</td><td>1049</td><td>Невозможно создать целевой файл – возможно, он используется.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2360</td><td>1049</td><td>Отметка прогресса.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2361</td><td>1049</td><td>Необходим следующий CAB-файл.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2362</td><td>1049</td><td>Папка не найдена: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2363</td><td>1049</td><td>Невозможно перечислить подпапки в папке: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2364</td><td>1049</td><td>Неверная константа перечисления при вызове CreateCopier.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2365</td><td>1049</td><td>Невозможно выполнить действие BindImage для файла EXE [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2366</td><td>1049</td><td>Ошибка пользователя.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2367</td><td>1049</td><td>Прерывание пользователя.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2368</td><td>1049</td><td>Невозможно получить информацию о сетевом ресурсе. Ошибка [2], сетевой путь [3]. Ошибка: служба доступа к сети [5], код ошибки [4], описание ошибки [6].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2370</td><td>1049</td><td>Некорректное значение контрольной суммы CRC для файла [2].{ В заголовке указана контрольная сумма [3], вычисленное значение равняется [4].}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2371</td><td>1049</td><td>Невозможно применить исправление к файлу [2]. GetLastError: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2372</td><td>1049</td><td>Файл исправления [2] поврежден или имеет недопустимый формат. Попытка исправления файла [3]. GetLastError: [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2373</td><td>1049</td><td>Файл [2] не является допустимым файлом исправлений.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2374</td><td>1049</td><td>Файл [2] не является допустимым файлом назначения для файла исправлений [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2375</td><td>1049</td><td>Неизвестная ошибка исправления: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2376</td><td>1049</td><td>CAB-файл не найден.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2379</td><td>1049</td><td>Ошибка при открытии файла для чтения: [3] GetLastError: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2380</td><td>1049</td><td>Ошибка при открытии файла для записи: [3]. GetLastError: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2381</td><td>1049</td><td>Каталог не существует: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2382</td><td>1049</td><td>Дисковод не готов: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_24</td><td>1049</td><td>Вставьте диск [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2401</td><td>1049</td><td>Попытка 64-битной операции в реестре в 32-битной операционной системе для ключа [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2402</td><td>1049</td><td>Недостаточно памяти.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_25</td><td>1049</td><td>У программы Windows Installer недостаточно полномочий для получения доступа к этому каталогу: [2].  Установка не может быть продолжена.  Войдите в систему в качестве администратора, либо обратитесь к системному администратору.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2501</td><td>1049</td><td>Невозможно создать перечислитель сценария отката.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2502</td><td>1049</td><td>Попытка окончания установки при отсутствии процесса установки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2503</td><td>1049</td><td>Вызов команды RunScript, когда она не помечена как запущенная.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_26</td><td>1049</td><td>Ошибка записи в файл [2].  Убедитесь, что у вас есть доступ к этому каталогу.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2601</td><td>1049</td><td>Недопустимое значение свойства [2]: '[3]'</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2602</td><td>1049</td><td>Для вхождения '[3]' таблицы [2] не существует связанного вхождения в таблице носителей.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2603</td><td>1049</td><td>Повторяющееся имя таблицы [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2604</td><td>1049</td><td>[2] Свойство не определено.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2605</td><td>1049</td><td>Невозможно найти сервер [2] в [3] или [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2606</td><td>1049</td><td>Значение свойства [2] не является допустимым полным путем: '[3]'.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2607</td><td>1049</td><td>Таблица носителей не найдена или пуста (требуется для установки файлов).</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2608</td><td>1049</td><td>Невозможно создать дескриптор безопасности для объекта. Ошибка: '[2]'.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2609</td><td>1049</td><td>Попытка миграции установок продукта перед инициализацией.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2611</td><td>1049</td><td>Файл [2] помечен как сжатый, но связанная запись носителя не задает CAB-файл.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2612</td><td>1049</td><td>Не найден поток в '[2]' столбце. Первичный ключ: '[3]'.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2613</td><td>1049</td><td>Действие RemoveExistingProducts упорядочено некорректно.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2614</td><td>1049</td><td>Нет доступа к объекту IStorage из пакета установки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2615</td><td>1049</td><td>Пропущена отмена регистрации модуля [2] из-за ошибки разрешения источника.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2616</td><td>1049</td><td>Отсутствует родительский объект вспомогательного файла [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2617</td><td>1049</td><td>Общий компонент приложения [2] не найден в таблице компонентов.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2618</td><td>1049</td><td>Изолированный компонент приложения [2] не найден в таблице компонентов.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2619</td><td>1049</td><td>Изолированные компоненты [2], [3] не являются частью одной и той же функции.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2620</td><td>1049</td><td>Ключ файла изолированного компонента приложения [2] отсутствует в таблице файлов.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2621</td><td>1049</td><td>Информация библиотеки ресурсов или идентификатора ресурсов для ярлыка [2] установлена неверно.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27</td><td>1049</td><td>Ошибка чтения из файла: [2]. {{ Системная ошибка [3].}} Проверьте, что файл существует и доступен.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2701</td><td>1049</td><td>Глубина функции превышает допустимую глубину дерева, составляющую [2] уровня (-ей).</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2702</td><td>1049</td><td>Запись в таблице функций ([2]) ссылается на несуществующий родительский объект в поле атрибутов.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2703</td><td>1049</td><td>Имя свойства для пути корневого источника не определено: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2704</td><td>1049</td><td>Не определено свойство корневой каталога: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2705</td><td>1049</td><td>Недопустимая таблица: [2]; Невозможно связать в качестве дерева.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2706</td><td>1049</td><td>Не созданы исходные пути. Не существует пути для вхождения [2] в таблице каталогов.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2707</td><td>1049</td><td>Не созданы конечные пути. Не существует пути для вхождения [2] в таблице каталогов.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2708</td><td>1049</td><td>Не обнаружено вхождений в таблице файлов.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2709</td><td>1049</td><td>Указанное имя компонента ('[2]') не найдено в таблице компонентов.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2710</td><td>1049</td><td>Требуемая команда 'Выбрать' недопустима для данного компонента.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2711</td><td>1049</td><td>Определенное имя функции ('[2]') не найдено в таблице функций.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2712</td><td>1049</td><td>Недопустимое возвращение из немодального диалога: [3], в действии [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2713</td><td>1049</td><td>Нулевое значение в столбце с неопределенным значением ('[2]' в '[3]' столбце '[4]' таблицы.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2714</td><td>1049</td><td>Недопустимое значение имени папки по умолчанию: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2715</td><td>1049</td><td>Определенный ключ файла ('[2]') не найден в таблице файлов.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2716</td><td>1049</td><td>Невозможно создать случайное имя субкомпонента для компонента '[2]'.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2717</td><td>1049</td><td>Неверное условие действия или ошибка вызванная действием пользователя '[2]'.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2718</td><td>1049</td><td>Отсутствует имя пакета для кода продукта '[2]'.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2719</td><td>1049</td><td>В источнике '[2]' не найдены ни UNC, ни путь буквы диска.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2720</td><td>1049</td><td>Ошибка при открытии ключа списка источников. Ошибка: '[2]'</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2721</td><td>1049</td><td>Действие пользователя [2] не найдено в потоке двоичной таблицы.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2722</td><td>1049</td><td>Действие пользователя [2] не найдено в таблице файлов.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2723</td><td>1049</td><td>Действие пользователя [2] определяет неподдерживаемый тип.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2724</td><td>1049</td><td>Метка тома '[2]' на носителе, с которым вы работаете, не совпадает с меткой '[3]', указанной в таблице носителей. Это допустимо только если в вашей таблице носителей есть всего одна запись.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2725</td><td>1049</td><td>Недопустимые таблицы баз данных</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2726</td><td>1049</td><td>Действие не найдено: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2727</td><td>1049</td><td>Запись в каталоге '[2]' не существует в таблице каталогов.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2728</td><td>1049</td><td>Ошибка определения таблицы: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2729</td><td>1049</td><td>Система установки не инициализирована.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2730</td><td>1049</td><td>Ошибочное значение в базе данных. Таблица: '[2]'; Первичный ключ: '[3]'; Столбец: '[4]'</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2731</td><td>1049</td><td>Диспетчер выбора не инициализирован.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2732</td><td>1049</td><td>Диспетчер каталогов не инициализирован.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2733</td><td>1049</td><td>Неверный внешний ключ ('[2]' в '[3]' столбце '[4]' таблицы.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2734</td><td>1049</td><td>Недопустимый символ режима переустановки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2735</td><td>1049</td><td>Действие пользователя '[2]' вызвало необработанное исключение и было остановлено. Возможно, это результат внутренней ошибки в действии пользователя, например, нарушения доступа.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2736</td><td>1049</td><td>Не удалось создать временный файл действия пользователя: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2737</td><td>1049</td><td>Нет доступа к действию пользователя [2], запись [3], библиотека [4]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2738</td><td>1049</td><td>Нет доступа ко времени запуска VBScript для действия пользователя [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2739</td><td>1049</td><td>Нет доступа ко времени запуска JavaScript для действия пользователя [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2740</td><td>1049</td><td>Действие пользователя [2] ошибка сценария [3], [4]: [5] Строка [6], Столбец [7], [8]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2741</td><td>1049</td><td>Информация о конфигурации продукта [2] повреждена. Недопустимая информация: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2742</td><td>1049</td><td>Не удалась упаковка для сервера: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2743</td><td>1049</td><td>Невозможно выполнить действие пользователя [2], местоположение: [3], команда: [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2744</td><td>1049</td><td>Ошибка EXE вызванная действием пользователя [2], местоположение: [3], команда: [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2745</td><td>1049</td><td>Преобразование [2] недопустимо для пакета [3]. Ожидаемый язык [4], найден язык [5].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2746</td><td>1049</td><td>Преобразование [2] недопустимо для пакета [3]. Ожидаемый продукт [4], найден продукт [5].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2747</td><td>1049</td><td>Преобразование [2] недопустимо для пакета [3]. Ожидаемая версия продукта &lt; [4], найдена версия продукта [5].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2748</td><td>1049</td><td>Преобразование [2] недопустимо для пакета [3]. Ожидаемая версия продукта &lt;= [4], найдена версия продукта [5].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2749</td><td>1049</td><td>Преобразование [2] недопустимо для пакета [3]. Ожидаемая версия продукта == [4], найдена версия продукта [5].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2750</td><td>1049</td><td>Преобразование [2] недопустимо для пакета [3]. Ожидаемая версия продукта &gt;= [4], найдена версия продукта [5].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27502</td><td>1049</td><td>Не удается подключиться к [2] '[3]'. [4]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27503</td><td>1049</td><td>Ошибка при получении строки версии от [2] '[3]'. [4]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27504</td><td>1049</td><td>Не выполнены требования к версии SQL: [3]. Для этой установки требуется [2] [4] или более поздней версии.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27505</td><td>1049</td><td>Не удалось открыть файл сценария SQL [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27506</td><td>1049</td><td>Ошибка при выполнении сценария SQL [2]. Строка [3]. [4]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27507</td><td>1049</td><td>Для просмотра серверов баз данных или подключения к ним должен быть установлен компонент MDAC.  Установка будет прервана.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27508</td><td>1049</td><td>Ошибка при установке приложения COM+ [2]. [3]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27509</td><td>1049</td><td>Ошибка при удалении приложения COM+ [2]. [3]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2751</td><td>1049</td><td>Преобразование [2] недопустимо для пакета [3]. Ожидаемая версия продукта &gt; [4], найдена версия продукта [5].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27510</td><td>1049</td><td>Ошибка при установке приложения COM+ [2]. Не удалось создать объект System.EnterpriseServices.RegistrationHelper. Для регистрации обслуживаемых компонентов Microsoft(R) .NET необходимо установить компонент Microsoft(R) .NET Framework.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27511</td><td>1049</td><td>Не удалось выполнить файл сценария SQL [2]. Сеанс связи не открыт: [3]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27512</td><td>1049</td><td>Ошибка при начале транзакций для [2] '[3]'. База данных [4]. [5]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27513</td><td>1049</td><td>Ошибка при выполнении транзакций для [2] '[3]'. База данных [4]. [5]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27514</td><td>1049</td><td>Для этой установки требуется Microsoft SQL Server. Указанный сервер '[3]' - Microsoft SQL Server Desktop Engine или SQL Server Express.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27515</td><td>1049</td><td>Ошибка при получении версии схемы с [2] '[3]'. База данных: '[4]'. [5]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27516</td><td>1049</td><td>Ошибка при записи версии схемы на [2] '[3]'. База данных: '[4]'. [5]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27517</td><td>1049</td><td>Для установки приложений COM+ необходимо иметь права администратора. Зарегистрируйтесь с правами администратора и повторите установку.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27518</td><td>1049</td><td>Приложение COM+ "[2]" сконфигурировано работать в качестве сервиса NT; для этого требуется, чтобы в системе было установлено COM+ 1.5 или более поздняя версия. Это приложение не будет установлено, потому что в системе установлена версия COM+ 1.0.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27519</td><td>1049</td><td>Ошибка при обновлении файла XML [2]. [3]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2752</td><td>1049</td><td>Невозможно открыть преобразование [2], сохраненное как дочернее хранилище пакета [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27520</td><td>1049</td><td>Ошибка при открытии файла XML [2]. [3]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27521</td><td>1049</td><td>Для этой установки требуется приложение MSXML 3.0 или более поздней версии, позволяющее настраивать файлы XML. Убедитесь, что имеется версия 3.0 или более поздняя версия.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27522</td><td>1049</td><td>Ошибка при создании файла XML [2]. [3]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27523</td><td>1049</td><td>Ошибка при загрузке серверов.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27524</td><td>1049</td><td>Ошибка при загрузке NetApi32.DLL. ISNetApi.dllтребуется правильно загруженный NetApi32.DLL и операционная система на основе NT.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27525</td><td>1049</td><td>Сервер не найден. Убедитесь, что указанный сервер существует. Имя сервера не может быть пустым.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27526</td><td>1049</td><td>Неопределенная ошибка ISNetApi.dll.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27527</td><td>1049</td><td>Недостаточный размер буфера.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27528</td><td>1049</td><td>В доступе отказано. Проверьте права администрирования.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27529</td><td>1049</td><td>Недопустимый компьютер.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2753</td><td>1049</td><td>Файл '[2]' не отмечен для установки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27530</td><td>1049</td><td>Неизвестная ошибка, выданная NetAPI. Системная ошибка: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27531</td><td>1049</td><td>Необрабатываемое исключение.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27532</td><td>1049</td><td>Недопустимое имя пользователя для этого сервера или домена.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27533</td><td>1049</td><td>Пароли с учетом регистра не совпадают.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27534</td><td>1049</td><td>Список пуст.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27535</td><td>1049</td><td>Нарушение прав доступа.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27536</td><td>1049</td><td>Ошибка при получении группы.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27537</td><td>1049</td><td>Ошибка при добавлении пользователя в группу. Убедитесь, что группа существует для этого домена или сервера.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27538</td><td>1049</td><td>Ошибка при создании пользователя.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27539</td><td>1049</td><td>ERROR_NETAPI_ERROR_NOT_PRIMARY возвращено от NetAPI.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2754</td><td>1049</td><td>Файл '[2]' не является допустимым файлом исправлений.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27540</td><td>1049</td><td>Пользователь с указанным именем уже существует.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27541</td><td>1049</td><td>Указанная группа уже существует.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27542</td><td>1049</td><td>Недействительный пароль. Убедитесь, что пароль соответствует сетевой политике использования паролей.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27543</td><td>1049</td><td>Недопустимое имя.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27544</td><td>1049</td><td>Недопустимая группа.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27545</td><td>1049</td><td>Имя пользователя не может быть пустым и должно соответствовать формату ДОМЕН\Имя пользователя.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27546</td><td>1049</td><td>Ошибка при загрузке файла INI или его создании в папке пользователя TEMP.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27547</td><td>1049</td><td>Файл ISNetAPI.dll не загружен, или при загрузке файла DLL произошла ошибка. Для выполнения данного действия требуется загрузить этот файл DLL. Убедитесь, что файл DLL находится в каталоге SUPPORTDIR.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27548</td><td>1049</td><td>Ошибка при удалении файла INI, содержащего новые сведения о пользователе, из папки пользователя TEMP.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27549</td><td>1049</td><td>Ошибка при получении основного контроллера домена (PDC).</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2755</td><td>1049</td><td>Сервер возвратил непредвиденную ошибку [2] при попытке установки пакета [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27550</td><td>1049</td><td>Для создания пользователя требуется заполнить каждое поле.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27551</td><td>1049</td><td>Не обнаружен драйвер ODBC для [2]. Это требуется для подключения к серверам баз данных [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27552</td><td>1049</td><td>Ошибка при создании базы данных [4]. Сервер: [2] [3]. [5]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27553</td><td>1049</td><td>Ошибка при подключении к базе данных [4]. Сервер: [2] [3]. [5]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27554</td><td>1049</td><td>Ошибка при попытке подключения [2]. Отсутствуют правильные метаданные, связанные с этим устройством.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_27555</td><td>1049</td><td>Неудачная попытка применить права к объекту '[2]'. Системная ошибка: [3] ([4])</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2756</td><td>1049</td><td>Свойство '[2]' использовано как свойство службы каталогов в одной или нескольких таблицах, но для него не было задано никакое значение.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2757</td><td>1049</td><td>Невозможно создать сводку для преобразования [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2758</td><td>1049</td><td>Преобразование [2] не содержит версии MSI.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2759</td><td>1049</td><td>Версия [3] преобразования [2] несовместима с системой; мин.: [4], макс.: [5].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2760</td><td>1049</td><td>Преобразование [2] недопустимо для пакета [3]. Ожидаемый код обновления [4], найден [5].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2761</td><td>1049</td><td>Невозможно начать транзакцию. Общий семафор инициализирован неправильно.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2762</td><td>1049</td><td>Невозможно записать запись сценария. Транзакция не начата.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2763</td><td>1049</td><td>Невозможно запустить сценарий. Транзакция не начата.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2765</td><td>1049</td><td>Имя сборки отсутствует в таблице AssemblyName: Компонент: [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2766</td><td>1049</td><td>Файл [2] является недопустимым файлом хранилища MSI.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2767</td><td>1049</td><td>Нет данных{ при перечислении [2]}.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2768</td><td>1049</td><td>Недопустимое преобразование пакета исправлений.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2769</td><td>1049</td><td>Действие пользователя [2] не закрыло [3] MSIHANDLEs.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2770</td><td>1049</td><td>Кэшированная папка [2] не определяется во внутренней таблице кэшированных папок.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2771</td><td>1049</td><td>В обновлении функции [2] пропущен компонент.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2772</td><td>1049</td><td>Новая функция обновления [2] должен добавляться как лист.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_28</td><td>1049</td><td>Другое приложение имеет монопольный доступ к файлу "[2]".  Закройте все другие приложения и повторите попытку.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2801</td><td>1049</td><td>Неизвестное сообщение -- Тип [2]. Не выбрано действие.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2802</td><td>1049</td><td>Не найден издатель события [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2803</td><td>1049</td><td>Представление диалога не обнаружило записи для диалога [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2804</td><td>1049</td><td>При активации управляющего элемента [3] диалога [2] CMsiDialog не смог оценить состояние [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2806</td><td>1049</td><td>Диалог [2] не смог оценить состояние [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2807</td><td>1049</td><td>Действие [2] не опознано.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2808</td><td>1049</td><td>Копка по умолчанию неверно определена в диалоге [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2809</td><td>1049</td><td>В диалоге [2] последовательные контрольные указатели не создают цикл. Существуют указатели с [3] по [4], но следующий указатель отсутствует.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2810</td><td>1049</td><td>В диалоге [2] последовательные контрольные указатели не создают цикл. Существует указатель с [3] и [5] до [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2811</td><td>1049</td><td>В диалоге [2] управляющий элемент [3] должен установить фокус, но не может этого сделать.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2812</td><td>1049</td><td>Событие [2] не опознано.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2813</td><td>1049</td><td>Событие EndDialog названо аргументом [2], но у диалога есть родительский объект.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2814</td><td>1049</td><td>В диалоге [2] управляющий элемент [3] называет несуществующий управляющий элемент [4] следующим элементом.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2815</td><td>1049</td><td>В таблице контроля условий есть строка, не содержащая условия для диалога [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2816</td><td>1049</td><td>Таблица сопоставления событий ссылается на недопустимый управляющий элемент [4] диалога [2] для события [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2817</td><td>1049</td><td>Событие [2] не может установить атрибут для управляющего элемента [4] диалога [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2818</td><td>1049</td><td>В таблице ControlEvent присутствует неопознанный аргумент для EndDialog [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2819</td><td>1049</td><td>Для управляющего элемента[3] диалога [2] необходимо связанное с ним свойство.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2820</td><td>1049</td><td>Попытка инициализации уже инициализированного обработчика.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2821</td><td>1049</td><td>Попытка инициализации уже инициализированного диалога: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2822</td><td>1049</td><td>Невозможно применить к диалогу [2] никакой другой метод, пока не добавлены все элементы контроля.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2823</td><td>1049</td><td>Попытка инициализации уже инициализированного управляющего элемента: [3] в диалоге [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2824</td><td>1049</td><td>Для атрибута диалога [3] требуется запись по меньшей мере [2] поля (полей).</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2825</td><td>1049</td><td>Для управляющего атрибута [3] требуется запись по меньшей мере [2] поля (полей).</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2826</td><td>1049</td><td>Управляющий элемент [3] диалога [2] нарушает границы диалога [4] на [5] пикселей.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2827</td><td>1049</td><td>Кнопка [4] переключателя [3] диалога [2] нарушает границы группы [5] на [6] пикселей.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2828</td><td>1049</td><td>Совершена попытка удалить управляющий элемент [3] из диалога [2], но управляющий элемент не является частью диалога.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2829</td><td>1049</td><td>Попытка использования неинициализированного диалога.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2830</td><td>1049</td><td>Попытка использования неинициализированного управляющего элемента диалога [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2831</td><td>1049</td><td>Управляющий элемент [3] диалога [2] не поддерживает [5] атрибут [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2832</td><td>1049</td><td>Диалог [2] не поддерживает атрибут [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2833</td><td>1049</td><td>Контрольный элемент [4] диалога [3] проигнорировал сообщение [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2834</td><td>1049</td><td>Последовательные указатели в диалоге [2] не создают одиночного цикла.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2835</td><td>1049</td><td>Управляющий элемент [2] не найден в диалоге [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2836</td><td>1049</td><td>Управляющий элемент [3] диалога [2] не может установить фокус.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2837</td><td>1049</td><td>Управляющий элемент [3] диалога [2] требует возврата winproc [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2838</td><td>1049</td><td>Пункт [2] в таблице выбора является родительским объектом сам для себя.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2839</td><td>1049</td><td>Не удалось установить свойство [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2840</td><td>1049</td><td>Не совпадает имя диалога ошибки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2841</td><td>1049</td><td>Не найдено кнопка ОК в диалоге ошибки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2842</td><td>1049</td><td>Не найдено текстовое поле в диалоге ошибки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2843</td><td>1049</td><td>Атрибут ErrorString не поддерживается стандартными диалогами.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2844</td><td>1049</td><td>Невозможно выполнить диалог ошибки, если строка ошибки не установлена.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2845</td><td>1049</td><td>Общая ширина кнопок превышает размер диалога ошибки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2846</td><td>1049</td><td>SetFocus не обнаружил требуемого управляющего элемента в диалоге ошибки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2847</td><td>1049</td><td>У управляющего элемента [3] диалога [2] есть одновременно значок и набор точечных рисунков.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2848</td><td>1049</td><td>Совершена попытка установить контрольный элемент [3] в качестве кнопки, выбираемой по умолчанию, для диалога [2], но такого контрольного элемента не существует.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2849</td><td>1049</td><td>Тип управляющего элемента [3] в диалоге [2] не может быть целочисленным.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2850</td><td>1049</td><td>Неопознанный тип тома.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2851</td><td>1049</td><td>Недопустимые данные для значка [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2852</td><td>1049</td><td>Необходимо добавить как минимум один управляющий элемент в диалог [2] перед его использованием.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2853</td><td>1049</td><td>Диалог [2] не является модальным диалогом. На него невозможно применить метод выполнения.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2854</td><td>1049</td><td>В диалоге [2] управляющий элемент [3] назначен первым активным элементом контроля, но такой элемент отсутствует.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2855</td><td>1049</td><td>В переключателе [3] диалога [2] менее 2 кнопок.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2856</td><td>1049</td><td>Создание второй копии диалога [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2857</td><td>1049</td><td>Каталог [2] упомянута в таблице выбора, но не обнаружена.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2858</td><td>1049</td><td>Недопустимые данные для точечного рисунка [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2859</td><td>1049</td><td>Сообщение об ошибке проверки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2860</td><td>1049</td><td>Кнопка отмены неверно определена в диалоге [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2861</td><td>1049</td><td>Последовательные указатели для переключателя управляющего элемента [3] диалога [2] не создают цикл.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2862</td><td>1049</td><td>Атрибуты управляющего элемента [3] диалога [2] не определяют допустимый размер значка. Установить размер 16.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2863</td><td>1049</td><td>Элемент управления [3] диалогом [2] требует значка [4] размером [5]x[5], но такой размер недоступен. Загрузка первого доступного размера.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2864</td><td>1049</td><td>Элемент управления [3] диалогом [2] получил событие обзора, но для текущего выбора невозможно настроить каталог. Наиболее вероятная причина: кнопка обзора содержит ошибки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2865</td><td>1049</td><td>Управляющий элемент [3] с объявлением [2] нарушает границы объявления[4] на [5] пикселей.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2866</td><td>1049</td><td>Диалог [2] недопустим для возврата аргумента [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2867</td><td>1049</td><td>Свойство диалога ошибки не установлено.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2868</td><td>1049</td><td>Диалог ошибки [2] не имеет набора стилей ошибки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2869</td><td>1049</td><td>В диалоге [2] установлен бит ошибки стилей, но это не диалог сообщения об ошибке.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2870</td><td>1049</td><td>В строке справки [4] для управляющего элемента [3] диалога [2] нет разделителя.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2871</td><td>1049</td><td>Истек срок таблицы [2]: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2872</td><td>1049</td><td>Недопустимый аргумент контрольного события CheckPath в диалоге [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2873</td><td>1049</td><td>Управляющий элемент [3] в диалоге [2] имеет недопустимое ограничение длины строки: [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2874</td><td>1049</td><td>Не удалось изменить шрифт текста на [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2875</td><td>1049</td><td>Не удалось изменить цвет текста на [2]. </td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2876</td><td>1049</td><td>Управляющий элемент [3] диалога [2] должен усечь строку: [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2877</td><td>1049</td><td>Двоичные данные [2] не обнаружены</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2878</td><td>1049</td><td>Возможное значение контрольного элемента [3] в диалоге [2]: [4]. Недопустимое или повторяющееся значение.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2879</td><td>1049</td><td>Управляющий элемент [3] диалога [2] не может интерпретировать строку маски: [4].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2880</td><td>1049</td><td>Не представлены оставшиеся события управления.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2881</td><td>1049</td><td>Не удалось инициализировать обработчик CMsiHandler.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2882</td><td>1049</td><td>Не удалось зарегистрировать класс окна диалога.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2883</td><td>1049</td><td>Не удалось выполнить команду CreateNewDialog для диалога [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2884</td><td>1049</td><td>Не удалось создать окно для диалога [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2885</td><td>1049</td><td>Невозможно создать управляющий элемент [3] диалога [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2886</td><td>1049</td><td>Не удалось создать таблицу [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2887</td><td>1049</td><td>Не удалось создать курсор для таблицы [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2888</td><td>1049</td><td>Не удалось выполнить просмотр [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2889</td><td>1049</td><td>Не удалось создать окно для управляющего элемента [3] диалога [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2890</td><td>1049</td><td>Обработчику не удалось создать инициализированный диалог.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2891</td><td>1049</td><td>Не удалось уничтожить окно для диалога [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2892</td><td>1049</td><td>[2] является управляющим элементом только для целых чисел, [3] не является допустимым значением целого числа.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2893</td><td>1049</td><td>Элемент управления [3] диалогом [2] принимает значения свойств максимум [5] знаков длиной. Значение [4] превышает это ограничение и было усечено.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2894</td><td>1049</td><td>Не удалось загрузить RICHED20.DLL. GetLastError() вернула код: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2895</td><td>1049</td><td>Не удалось опустошить RICHED20.DLL. GetLastError() вернула код: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2896</td><td>1049</td><td>Не удалось выполнить действие [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2897</td><td>1049</td><td>Не удалось создать какой-либо шрифт [2] для данной системы.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2898</td><td>1049</td><td>Для стиля текста [2] система создала '[3]' шрифт в [4] наборе символов.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2899</td><td>1049</td><td>Не удалось создать [2] стиль текста. GetLastError() возвратила код: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_29</td><td>1049</td><td>На диске недостаточно места для установки файла [2].  Освободите место и нажмите кнопку "Повторить" либо кнопку "Отмена" для прекращения установки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2901</td><td>1049</td><td>Недопустимый параметр операции [2]: Параметр [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2902</td><td>1049</td><td>Действие [2] вызвано в нарушение последовательности.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2903</td><td>1049</td><td>Файл [2] отсутствует.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2904</td><td>1049</td><td>Невозможно выполнить действие BindImage для файла [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2905</td><td>1049</td><td>Невозможно прочитать запись из файла сценария [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2906</td><td>1049</td><td>Отсутствует заголовок в файле сценария [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2907</td><td>1049</td><td>Невозможно создать безопасный дескриптор безопасности. Ошибка: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2908</td><td>1049</td><td>Невозможно произвести регистрацию компонента [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2909</td><td>1049</td><td>Невозможно отменить регистрацию компонента [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2910</td><td>1049</td><td>Невозможно определить идентификатор безопасности пользователя.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2911</td><td>1049</td><td>Невозможно удалить папку [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2912</td><td>1049</td><td>Невозможно создать файл расписания[2] для удаления при перезапуске.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2919</td><td>1049</td><td>Не определен CAB-файл для сжатого файла: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2920</td><td>1049</td><td>Не определен исходный каталог для файла [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2924</td><td>1049</td><td>Версия [2] сценария не поддерживается. Версия сценария: [3], минимальная версия: [4], максимальная версия: [5].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2927</td><td>1049</td><td>Недопустимый код [2] ShellFolder.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2928</td><td>1049</td><td>Превышение максимального количества источников. Пропуск источника '[2]'.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2929</td><td>1049</td><td>Невозможно определить корень публикации. Ошибка: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2932</td><td>1049</td><td>Невозможно создать файл [2] из данных сценария. Ошибка: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2933</td><td>1049</td><td>Невозможно инициализировать сценарий отката [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2934</td><td>1049</td><td>Невозможно установить защиту преобразования [2]. Ошибка [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2935</td><td>1049</td><td>Невозможно отменить защиту преобразования [2]. Ошибка [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2936</td><td>1049</td><td>Невозможно найти преобразование [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2937</td><td>1049</td><td>Установщик Windows не может установить каталог защиты системных файлов. Каталог: [2], Ошибка: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2938</td><td>1049</td><td>Установщик Windows не может загрузить каталог защиты системных файлов из кэша. Каталог: [2], Ошибка: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2939</td><td>1049</td><td>Установщик Windows не может удалить каталог защиты системных файлов из кэша. Каталог: [2], Ошибка: [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2940</td><td>1049</td><td>Диспетчер каталогов не поддерживает разрешение источника.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2941</td><td>1049</td><td>Невозможно вычислить CRC для файла [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2942</td><td>1049</td><td>Действие BindImage не было выполнено для [2] файла.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2943</td><td>1049</td><td>Эта версия системы Windows не поддерживает развертывание 64-битных пакетов приложений. Сценарий [2] предназначен для 64-битных пакетов приложений.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2944</td><td>1049</td><td>Выполнить действие GetProductAssignmentType не удалось.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_2945</td><td>1049</td><td>Не удалось установить приложение ComPlus [2] из-за ошибки [3].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_3</td><td>1049</td><td>Сведения [1]. </td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_30</td><td>1049</td><td>Исходный файл не найден: [2].  Проверьте, что этот файл существует и доступен.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_3001</td><td>1049</td><td>Исправления в списке содержат неверную информацию последовательности: [2][3][4][5][6][7][8][9][10][11][12][13][14][15][16].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_3002</td><td>1049</td><td>Путь [2] содержит недопустимую информацию последовательности. </td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_31</td><td>1049</td><td>Ошибка чтения из файла [3]. {{ Системная ошибка [2].}}  Проверьте, что файл существует и доступен.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_32</td><td>1049</td><td>Ошибка записи в файл [3]. {{ Системная ошибка [2].}}  Убедитесь, что у вас есть доступ к этому каталогу.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_33</td><td>1049</td><td>Исходный файл не найден{{(CAB-файл)}}: [2].  Проверьте, что файл существует и доступен.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_34</td><td>1049</td><td>Не удается создать каталог "[2]".  Файл с этим именем уже существует.  Переименуйте или переместите этот файл и повторите попытку, либо нажмите кнопку "Отмена" для прекращения установки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_35</td><td>1049</td><td>Том [2] недоступен.  Выберите другой том.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_36</td><td>1049</td><td>Указанный путь "[2]" недоступен.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_37</td><td>1049</td><td>Запись в указанный каталог "[2]" запрещена.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_38</td><td>1049</td><td>Ошибка в сети при чтении из файла [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_39</td><td>1049</td><td>Ошибка при создании каталога [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_4</td><td>1049</td><td>Внутренняя ошибка [1]. [2]{, [3]}{, [4]}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_40</td><td>1049</td><td>Ошибка в сети при создании каталога [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_41</td><td>1049</td><td>Ошибка в сети при открытии исходного CAB-файла [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_42</td><td>1049</td><td>Указан слишком длинный путь: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_43</td><td>1049</td><td>У программы Windows Installer недостаточно полномочий для изменения этого файла: [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_44</td><td>1049</td><td>Неверная часть пути к папке "[2]". Путь либо пуст, либо превышает максимально допустимую длину.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_45</td><td>1049</td><td>Путь к папке "[2]" содержит недопустимые имена.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_46</td><td>1049</td><td>Путь к папке "[2]" содержит недопустимый символ.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_47</td><td>1049</td><td>"[2]" не является допустимым коротким именем файла.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_48</td><td>1049</td><td>Ошибка при получении сведений о безопасности файла: [3] GetLastError: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_49</td><td>1049</td><td>Недопустимый диск: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_5</td><td>1049</td><td>{{Диск переполнен: }}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_50</td><td>1049</td><td>Не удается создать раздел [2]. {{ Системная ошибка [3].}}  Убедитесь, что у вас достаточно полномочий для доступа к этому разделу, либо обратитесь в службу поддержки. </td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_51</td><td>1049</td><td>Не удается открыть раздел [2]. {{ Системная ошибка [3].}}  Убедитесь, что у вас достаточно полномочий для доступа к этому разделу, либо обратитесь в службу поддержки. </td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_52</td><td>1049</td><td>Не удается удалить значение [2] из раздела [3]. {{ Системная ошибка [4].}}  Убедитесь, что у вас достаточно полномочий для доступа к этому разделу, либо обратитесь в службу поддержки. </td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_53</td><td>1049</td><td>Не удается удалить раздел [2]. {{ Системная ошибка [3].}}  Убедитесь, что у вас достаточно полномочий для доступа к этому разделу, либо обратитесь в службу поддержки. </td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_54</td><td>1049</td><td>Не удается прочитать значение [2] из раздела [3]. {{ Системная ошибка [4].}}  Убедитесь, что у вас достаточно полномочий для доступа к этому разделу, либо обратитесь в службу поддержки. </td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_55</td><td>1049</td><td>Не удается записать значение [2] в раздел [3]. {{ Системная ошибка [4].}}  Убедитесь, что у вас достаточно полномочий для доступа к этому разделу, либо обратитесь в службу поддержки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_56</td><td>1049</td><td>Не удается получить имена значений из раздела [2]. {{ Системная ошибка [3].}}  Убедитесь, что у вас достаточно полномочий для доступа к этому разделу, либо обратитесь в службу поддержки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_57</td><td>1049</td><td>Не удается получить имена подразделов из раздела [2]. {{ Системная ошибка [3].}}  Убедитесь, что у вас достаточно полномочий для доступа к этому разделу, либо обратитесь в службу поддержки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_58</td><td>1049</td><td>Не удается прочитать сведения о безопасности из раздела [2]. {{ Системная ошибка [3].}}  Убедитесь, что у вас достаточно полномочий для доступа к этому разделу, либо обратитесь в службу поддержки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_59</td><td>1049</td><td>Не удается увеличить размер реестра. Для установки данного приложения необходимо [2] Кбайт свободного места в реестре.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_6</td><td>1049</td><td>Действие [Time]: [1]. [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_60</td><td>1049</td><td>Другой процесс установки уже выполняется. Завершите этот процесс перед новой установкой.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_61</td><td>1049</td><td>Ошибка доступа к защищенным данным. Проверьте правильность настройки Windows Installer и повторите попытку.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_62</td><td>1049</td><td>Пользователь "[2]" ранее начал установку продукта "[3]".  Этому пользователю придется выполнить установку заново.  Ваша текущая установка будет продолжена.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_63</td><td>1049</td><td>Пользователь "[2]" ранее начал установку продукта "[3]".  Этому пользователю придется выполнить установку заново.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_64</td><td>1049</td><td>Недостаточно места на диске -- том: "[2]"; требуемый объем: [3] Кбайт; доступный объем: [4] Кбайт.  Освободите место на диске и повторите попытку.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_65</td><td>1049</td><td>Отменить установку?</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_66</td><td>1049</td><td>Файл [2][3] используется{ следующим процессом: Имя: [4], Код: [5], Заголовок окна: "[6]"}.  Закройте это приложение и повторите попытку.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_67</td><td>1049</td><td>Продукт "[2]" уже установлен, установка данного продукта невозможна.  Эти два продукта несовместимы.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_68</td><td>1049</td><td>Недостаточно места на диске -- том: "[2]"; требуемый объем: [3] Кбайт; доступный объем: [4] Кбайт.  При отключении отката места будет достаточно. Нажмите кнопку "Отмена" для выхода, "Повторить" для повторной проверки доступного места на диске или "Пропустить" для продолжения без отката.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_69</td><td>1049</td><td>Не удается подключиться через сеть к [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_7</td><td>1049</td><td>[ProductName]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_70</td><td>1049</td><td>Для продолжения установки следует закрыть следующие приложения:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_71</td><td>1049</td><td>Не удается найти установленные совместимые продукты для установки данного продукта.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_72</td><td>1049</td><td>Неверный ключ [2].  Проверьте правильность ключа.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_73</td><td>1049</td><td>Перед продолжением настройки [2] программа Windows Installer должна перезагрузить систему.  Нажмите кнопку "Да", чтобы перезагрузить компьютер сейчас, либо кнопку "Нет", чтобы выполнить перезагрузку позже вручную.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_74</td><td>1049</td><td>Для учета изменений в настройках [2] требуется перезагрузка системы. Нажмите кнопку "Да", чтобы перезагрузить компьютер сейчас, либо кнопку "Нет", чтобы выполнить перезагрузку позже вручную.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_75</td><td>1049</td><td>Установка [2] приостановлена.  Для продолжения необходимо отменить изменения, внесенные в процессе этой установки.  Отменить изменения?</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_76</td><td>1049</td><td>Выполняется предыдущая установка этого продукта.  Для продолжения необходимо отменить изменения, внесенные в процессе этой установки.  Отменить изменения?</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_77</td><td>1049</td><td>Не удается найти пакет установки для продукта [2]. Повторите установку с использованием правильной копии пакета "[3]".</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_78</td><td>1049</td><td>Операция установки успешно завершена.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_79</td><td>1049</td><td>Операция установки не выполнена.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_8</td><td>1049</td><td>{[2]}{, [3]}{, [4]}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_80</td><td>1049</td><td>Продукт: [2] -- [3]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_81</td><td>1049</td><td>Можно либо восстановить исходное состояние компьютера, либо продолжить установку позже. Восстановить исходное состояние?</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_82</td><td>1049</td><td>Ошибка при записи сведений об установке на диск.  Проверьте наличие свободного места на диске и нажмите кнопку "Повторить", либо нажмите кнопку "Отмена", чтобы прервать установку.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_83</td><td>1049</td><td>Не удается найти файлы, необходимые для восстановления исходного состояния компьютера.  Восстановление невозможно.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_84</td><td>1049</td><td>Недопустимый путь [2].  Укажите правильный путь.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_85</td><td>1049</td><td>Недостаточно памяти. Закройте другие приложения и повторите попытку.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_86</td><td>1049</td><td>В дисководе [2] нет диска. Вставьте диск и нажмите кнопку "Повторить", либо нажмите кнопку "Отмена" для возврата к ранее выбранному тому.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_87</td><td>1049</td><td>В дисководе [2] нет диска. Вставьте диск и нажмите кнопку "Повтор" или нажмите кнопку "Отмена", чтобы вернуться в окно обзора и выбрать другий том.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_88</td><td>1049</td><td>Папка [2] не существует.  Введите путь к существующей папке.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_89</td><td>1049</td><td>У вас недостаточно полномочий для чтения этой папки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_9</td><td>1049</td><td>Тип сообщения: [1], Аргумент: [2]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_90</td><td>1049</td><td>Не удается определить конечную папку для установки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_91</td><td>1049</td><td>Ошибка при чтении исходной базы данных установки. [2].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_92</td><td>1049</td><td>Планирование операции перезагрузки: переименование файла [2] в [3]. Для завершения операции требуется перезагрузка.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_93</td><td>1049</td><td>Планирование операции перезагрузки: удаление файла [2]. Для завершения операции требуется перезагрузка.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_94</td><td>1049</td><td>Не удается зарегистрировать модуль [2].  HRESULT [3].  Обратитесь в службу поддержки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_95</td><td>1049</td><td>Не удается отменить регистрацию модуля [2].  HRESULT [3].  Обратитесь в службу поддержки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_96</td><td>1049</td><td>Ошибка кэширования пакета [2]. Ошибка: [3]. Обратитесь в службу поддержки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_97</td><td>1049</td><td>Не удается зарегистрировать шрифт [2].  Убедитесь, что у вас имеются разрешения на установку шрифтов, и что система поддерживает этот шрифт.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_98</td><td>1049</td><td>Не удается отменить регистрацию шрифта [2]. Убедитесь, что у вас имеются разрешения на удаление шрифтов.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ERROR_99</td><td>1049</td><td>Не удается создать ярлык [2]. Проверьте, что конечная папка существует и доступна.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_INSTALLDIR</td><td>1049</td><td>{&amp;Tahoma8}[INSTALLDIR]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_INSTALLSHIELD</td><td>1049</td><td>InstallShield</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_INSTALLSHIELD_FORMATTED</td><td>1049</td><td>{&amp;MSSWhiteSerif8}InstallShield</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ISSCRIPT_VERSION_MISSING</td><td>1049</td><td>Компонент InstallScript отсутствует на этом компьютере.  Если возможно, запустите пакет ISScript.msi или обратитесь за помощью в службу поддержки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_ISSCRIPT_VERSION_OLD</td><td>1049</td><td>Версия компонента InstallScript на этом компьютере более ранняя, чем требуется для данной установки.  Если возможно, установите последнюю версию пакета ISScript.msi или обратитесь за помощью в службу поддержки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_NEXT</td><td>1049</td><td>&amp;Далее &gt;</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_OK</td><td>1049</td><td>{&amp;Tahoma8}OK</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PREREQUISITE_SETUP_BROWSE</td><td>1049</td><td>Открыть оригинальный файл [SETUPEXENAME] [ProductName]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PREREQUISITE_SETUP_INVALID</td><td>1049</td><td>Похоже исполняемый файл не является оригинальным исполняемым файлом для [ProductName]. Если не использовать оригинальный файл [SETUPEXENAME] для установки дополнительных зависимостей, [ProductName] может не работать. Найти оригинал [SETUPEXENAME]?</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PREREQUISITE_SETUP_SEARCH</td><td>1049</td><td>Эта установка может потребовать дополнительных зависимостей. Без этих зависимостей [ProductName] может не работать. Найти оригинал [SETUPEXENAME]?</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PREVENT_DOWNGRADE_EXIT</td><td>1049</td><td>На компьютере уже установлена более новая версия этого приложения. Чтобы установить приложение, сначала удалите новую версию. Нажмите кнопку «ОК», чтобы закрыть мастер.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PRINT_BUTTON</td><td>1049</td><td>&amp;Печать</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PRODUCTNAME_INSTALLSHIELD</td><td>1049</td><td>[ProductName] - InstallShield Wizard</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_CREATEAPPPOOL</td><td>1049</td><td>Создание группы приложений %s</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_CREATEAPPPOOLS</td><td>1049</td><td>Создание групп приложений...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_CREATEVROOT</td><td>1049</td><td>Создание виртуального каталога IIS %s</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_CREATEVROOTS</td><td>1049</td><td>Создание виртуальных каталогов IIS...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_CREATEWEBSERVICEEXTENSION</td><td>1049</td><td>Создание расширения веб-службы</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_CREATEWEBSERVICEEXTENSIONS</td><td>1049</td><td>Создания расширений веб-службы...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_CREATEWEBSITE</td><td>1049</td><td>Создание веб-сайтов IIS %s</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_CREATEWEBSITES</td><td>1049</td><td>Создание веб-сайтов IIS...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_EXTRACT</td><td>1049</td><td>Извлечение данных для виртуальных каталогов IIS...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_EXTRACTDONE</td><td>1049</td><td>Извлеченные данные для виртуальных каталогов IIS...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_REMOVEAPPPOOL</td><td>1049</td><td>Удаление группы приложений</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_REMOVEAPPPOOLS</td><td>1049</td><td>Удаление групп приложений...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_REMOVESITE</td><td>1049</td><td>Удаление веб-узла для порта %d</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_REMOVEVROOT</td><td>1049</td><td>Удаление виртуального каталога IIS %s</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_REMOVEVROOTS</td><td>1049</td><td>Удаление виртуальных каталогов IIS...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_REMOVEWEBSERVICEEXTENSION</td><td>1049</td><td>Удаление расширения веб-службы</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_REMOVEWEBSERVICEEXTENSIONS</td><td>1049</td><td>Удаление расширений веб-службы...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_REMOVEWEBSITES</td><td>1049</td><td>Удаление веб-сайтов IIS...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_ROLLBACKAPPPOOLS</td><td>1049</td><td>Откат группы приложений...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_ROLLBACKVROOTS</td><td>1049</td><td>Откат виртуального каталога и изменений веб-узла...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_IIS_ROLLBACKWEBSERVICEEXTENSIONS</td><td>1049</td><td>Откат расширений веб-службы...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_TEXTFILECHANGS_REPLACE</td><td>1049</td><td>Замена %s на %s в %s...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_XML_COSTING</td><td>1049</td><td>Оценка необходимого места для файлов XML...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_XML_CREATE_FILE</td><td>1049</td><td>Создание файла XML %s...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_XML_FILES</td><td>1049</td><td>Изменение файла XML...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_XML_REMOVE_FILE</td><td>1049</td><td>Удаление файла XML %s...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_XML_ROLLBACK_FILES</td><td>1049</td><td>Откат изменений файла XML...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_PROGMSG_XML_UPDATE_FILE</td><td>1049</td><td>Обновление файла XML %s...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SETUPEXE_EXPIRE_MSG</td><td>1049</td><td>Эта программа установки может работать до %s. Программа установки будет закрыта.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SETUPEXE_LAUNCH_COND_E</td><td>1049</td><td>Эта программа установки создана на базе пробной версии InstallShield и может быть запущена только из setup.exe.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SHORTCUT_DISPLAY_NAME1</td><td>1033</td><td>NCM53</td><td>0</td><td/><td>715808140</td></row>
		<row><td>IDS_SHORTCUT_DISPLAY_NAME1</td><td>1049</td><td>NCM53</td><td>0</td><td/><td>715808140</td></row>
		<row><td>IDS_SQLBROWSE_INTRO</td><td>1049</td><td>В следующем списке серверов выберите сервер базы данных, который необходимо использовать.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SQLBROWSE_INTRO_DB</td><td>1049</td><td>Из приведенного ниже списка имен каталогов выберите каталог базы данных, которую нужно выбрать.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SQLBROWSE_INTRO_TEMPLATE</td><td>1049</td><td>[IS_SQLBROWSE_INTRO]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SQLLOGIN_BROWSE</td><td>1049</td><td>&amp;Обзор...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SQLLOGIN_BROWSE_DB</td><td>1049</td><td>Об&amp;зор…</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SQLLOGIN_CATALOG</td><td>1049</td><td>И&amp;мя каталога баз данных:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SQLLOGIN_CONNECT</td><td>1049</td><td>Подключение:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SQLLOGIN_DESC</td><td>1049</td><td>Выберите сервер базы данных и метод проверки подлинности.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SQLLOGIN_ID</td><td>1049</td><td>&amp;Имя пользователя:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SQLLOGIN_INTRO</td><td>1049</td><td>Выберите сервер базы данных, который необходимо установить, в следующем списке или нажмите кнопку "Обзор" для просмотра списка всех серверов баз данных. Можно также указать вариант проверки подлинности пользователя с использованием текущих учетных данных или имени пользователя и пароля SQL-сервера.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SQLLOGIN_PSWD</td><td>1049</td><td>П&amp;ароль:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SQLLOGIN_SERVER</td><td>1049</td><td>&amp;Сервер базы данных:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SQLLOGIN_SERVER2</td><td>1049</td><td>&amp;Сервер базы данных, который вы устанавливаете в:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SQLLOGIN_SQL</td><td>1049</td><td>Проверка подлинности с&amp;ервера с использованием следующего имени пользователя и пароля</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SQLLOGIN_TITLE</td><td>1049</td><td>{&amp;MSSansBold8}Сервер базы данных</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SQLLOGIN_WIN</td><td>1049</td><td>&amp;Учетные данные проверки подлинности Windows текущего пользователя</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SQLSCRIPT_INSTALLING</td><td>1049</td><td>Выполнение сценария установки SQL...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SQLSCRIPT_UNINSTALLING</td><td>1049</td><td>Выполнение сценария удаления SQL...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_STANDARD_USE_SETUPEXE</td><td>1049</td><td>Запуск этой установки путем простого запуска пакета MSI невозможен. Необходимо запустить setup.exe.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SetupTips_Advertise</td><td>1049</td><td>{&amp;Tahoma8}будет установлен при первом использовании.</td><td>0</td><td>	 	</td><td>-760690007</td></row>
		<row><td>IDS_SetupTips_AllInstalledLocal</td><td>1049</td><td>{&amp;Tahoma8}будет полностью установлен на локальный жесткий диск.</td><td>0</td><td>	</td><td>-760690007</td></row>
		<row><td>IDS_SetupTips_CustomSetup</td><td>1049</td><td>{&amp;MSSansBold8}Советы по выборочной установке</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SetupTips_CustomSetupDescription</td><td>1049</td><td>{&amp;Tahoma8}Выборочная установка позволяет установить определенные компоненты программы.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SetupTips_IconInstallState</td><td>1049</td><td>{&amp;Tahoma8}Значок рядом с названием компонента показывает состояние установки компонента. Щелкните этот значок, чтобы раскрыть меню состояния установки для каждого компонента.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SetupTips_InstallState</td><td>1049</td><td>{&amp;Tahoma8}Данное состояние установки означает, что компонент...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SetupTips_Network</td><td>1049</td><td>{&amp;Tahoma8}будет установлен для запуска по сети.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SetupTips_OK</td><td>1049</td><td>{&amp;Tahoma8}OK</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SetupTips_SubFeaturesInstalledLocal</td><td>1049</td><td>{&amp;Tahoma8}будет содержать подкомпоненты на локальном жестком диске.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_SetupTips_WillNotBeInstalled</td><td>1049</td><td>{&amp;Tahoma8}не будет установлен.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_Available</td><td>1049</td><td>Доступно</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_Bytes</td><td>1049</td><td>байт</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_CompilingFeaturesCost</td><td>1049</td><td>Оценка дискового пространства для данного компонента...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_Differences</td><td>1049</td><td>Разница</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_DiskSize</td><td>1049</td><td>Размер диска</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureCompletelyRemoved</td><td>1049</td><td>Данный компонент будет полностью удален.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureContinueNetwork</td><td>1049</td><td>Данный компонент по-прежнему будет запускаться по сети</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureFreeSpace</td><td>1049</td><td>Данный компонент освобождает [1] на жестком диске.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureInstalledCD</td><td>1049</td><td>Данный компонент и все подкомпоненты будут установлены для запуска с компакт-диска.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureInstalledCD2</td><td>1049</td><td>Данный компонент будет установлен для запуска с компакт-диска.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureInstalledLocal</td><td>1049</td><td>Данный компонент и все подкомпоненты будут установлены на локальный жесткий диск.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureInstalledLocal2</td><td>1049</td><td>Данный компонент будет установлен на локальный жесткий диск.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureInstalledNetwork</td><td>1049</td><td>Данный компонент и все подкомпоненты будут установлены для запуска по сети.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureInstalledNetwork2</td><td>1049</td><td>Данный компонент будет установлен для запуска по сети.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureInstalledRequired</td><td>1049</td><td>Его при необходимости можно установить.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureInstalledWhenRequired</td><td>1049</td><td>Данный компонент можно при необходимости установить.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureInstalledWhenRequired2</td><td>1049</td><td>Данный компонент можно при необходимости установить.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureLocal</td><td>1049</td><td>Данный компонент будет установлен на локальный жесткий диск.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureLocal2</td><td>1049</td><td>Данный компонент будет установлен на локальный жесткий диск.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureNetwork</td><td>1049</td><td>Данный компонент будет установлен для запуска по сети.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureNetwork2</td><td>1049</td><td>Данный компонент будет доступен для запуска по сети.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureNotAvailable</td><td>1049</td><td>Данный компонент будет недоступен.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureOnCD</td><td>1049</td><td>Данный компонент будет установлен для запуска с компакт-диска.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureOnCD2</td><td>1049</td><td>Данный компонент будет доступен для запуска с компакт-диска.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureRemainLocal</td><td>1049</td><td>Данный компонент останется на локальном жестком диске.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureRemoveNetwork</td><td>1049</td><td>Данный компонент будет удален с локального жесткого диска, но его по-прежнему можно будет запускать по сети.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureRemovedCD</td><td>1049</td><td>Данный компонент будет удален с локального жесткого диска, но его по-прежнему можно будет запускать с компакт-диска.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureRemovedUnlessRequired</td><td>1049</td><td>Данный компонент будет удален с локального жесткого диска, но его при необходимости можно установить.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureRequiredSpace</td><td>1049</td><td>Для данного компонента требуется [1] на жестком диске.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureRunFromCD</td><td>1049</td><td>Данный компонент по-прежнему будет запускаться с компакт-диска</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureSpaceFree</td><td>1049</td><td>Данный компонент освобождает [1] на жестком диске. Для него выбраны [2] из [3] подкомпонентов. Подкомпоненты освобождают [4] на жестком диске.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureSpaceFree2</td><td>1049</td><td>Данный компонент освобождает [1] на жестком диске. Для него выбраны [2] из [3] подкомпонентов. Для подкомпонентов требуется [4] на жестком диске.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureSpaceFree3</td><td>1049</td><td>Для данного компонента требуется [1] на жестком диске. Для него выбраны [2] из [3] подкомпонентов. Подкомпоненты освобождают [4] на жестком диске.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureSpaceFree4</td><td>1049</td><td>Для данного компонента требуется [1] на жестком диске. Для него выбраны [2] из [3] подкомпонентов. Для подкомпонентов требуется [4] на жестком диске.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureUnavailable</td><td>1049</td><td>Данный компонент станет недоступным.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureUninstallNoNetwork</td><td>1049</td><td>Данный компонент будет полностью удален, и его нельзя будет запускать по сети.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureWasCD</td><td>1049</td><td>Данный компонент запущен с компакт-диска, но его при необходимости можно установить.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureWasCDLocal</td><td>1049</td><td>Данный компонент запущен с компакт-диска, но будет установлен на локальный диск.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureWasOnNetworkInstalled</td><td>1049</td><td>Данный компонент запущен по сети, но его при необходимости можно установить.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureWasOnNetworkLocal</td><td>1049</td><td>Данный компонент запущен по сети, но его при необходимости можно установить локальный жесткий диск.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_FeatureWillBeUninstalled</td><td>1049</td><td>Данный компонент будет полностью удален, и его нельзя будет запускать с компакт-диска.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_Folder</td><td>1049</td><td>Папка|Создать папку</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_GB</td><td>1049</td><td>Гб</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_KB</td><td>1049</td><td>Кб</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_MB</td><td>1049</td><td>Мб</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_Required</td><td>1049</td><td>Требуется</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_TimeRemaining</td><td>1049</td><td>Осталось времени: {[1] мин }{[2] сек}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS_UITEXT_Volume</td><td>1049</td><td>Том</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__AgreeToLicense_0</td><td>1049</td><td>{&amp;Tahoma8}Я не п&amp;ринимаю условия лицензионного соглашения</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__AgreeToLicense_1</td><td>1049</td><td>{&amp;Tahoma8}Я &amp;принимаю условия лицензионного соглашения</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__DatabaseFolder_ChangeFolder</td><td>1049</td><td>Нажмите кнопу "Далее", чтобы установить в эту папку. Нажмите кнопку "Изменить", чтобы выполнить установку в другую папку.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__DatabaseFolder_DatabaseDir</td><td>1049</td><td>[DATABASEDIR]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__DatabaseFolder_DatabaseFolder</td><td>1049</td><td>{&amp;MSSansBold8}Папка базы данных</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__DestinationFolder_Change</td><td>1049</td><td>{&amp;Tahoma8}&amp;Изменить...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__DestinationFolder_ChangeFolder</td><td>1049</td><td>{&amp;Tahoma8}Нажмите кнопку "Далее", чтобы установить в эту папку. Нажмите кнопку "Изменить", чтобы выполнить установку в другую папку.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__DestinationFolder_DestinationFolder</td><td>1049</td><td>{&amp;MSSansBold8}Папка назначения</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__DestinationFolder_InstallTo</td><td>1049</td><td>{&amp;Tahoma8}Установка [ProductName] в:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__DisplayName_Custom</td><td>1049</td><td>Выборочная</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__DisplayName_Minimal</td><td>1049</td><td>Минимальная</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__DisplayName_Typical</td><td>1049</td><td>Обычная</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsAdminInstallBrowse_11</td><td>1049</td><td>{&amp;Tahoma8}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsAdminInstallBrowse_4</td><td>1049</td><td>{&amp;Tahoma8}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsAdminInstallBrowse_8</td><td>1049</td><td>{&amp;Tahoma8}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsAdminInstallBrowse_BrowseDestination</td><td>1049</td><td>{&amp;Tahoma8}Перейдите в папку назначения.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsAdminInstallBrowse_ChangeDestination</td><td>1049</td><td>{&amp;MSSansBold8}Изменение текущей папки назначения</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsAdminInstallBrowse_CreateFolder</td><td>1049</td><td>Создать новую папку|</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsAdminInstallBrowse_FolderName</td><td>1049</td><td>{&amp;Tahoma8}&amp;Имя папки:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsAdminInstallBrowse_LookIn</td><td>1049</td><td>{&amp;Tahoma8}&amp;Поиск в папке:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsAdminInstallBrowse_UpOneLevel</td><td>1049</td><td>Переход на один уровень вверх|</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsAdminInstallPointWelcome_ServerImage</td><td>1049</td><td>{&amp;Tahoma8}Программа InstallShield(R) Wizard создаст образ диска [ProductName] на сервере в указанной сетевой папке. Для продолжения нажмите кнопку "Далее".</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsAdminInstallPointWelcome_Wizard</td><td>1049</td><td>{&amp;VerdanaBold14}Вас приветствует программа InstallShield Wizard для [ProductName]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsAdminInstallPoint_Change</td><td>1049</td><td>{&amp;Tahoma8}&amp;Изменить...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsAdminInstallPoint_EnterNetworkLocation</td><td>1049</td><td>{&amp;Tahoma8}Введите имя сетевой папки или нажмите кнопку "Изменить" для перехода в требуемую папку.  Нажмите кнопку "Установить", чтобы создать образ диска [ProductName] на сервере в указанной сетевой папке, или нажмите кнопку "Отмена" для выхода из программы.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsAdminInstallPoint_Install</td><td>1049</td><td>{&amp;Tahoma8}&amp;Установить</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsAdminInstallPoint_NetworkLocation</td><td>1049</td><td>{&amp;Tahoma8}&amp;Сетевая папка:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsAdminInstallPoint_NetworkLocationFormatted</td><td>1049</td><td>{&amp;MSSansBold8}Сетевая папка</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsAdminInstallPoint_SpecifyNetworkLocation</td><td>1049</td><td>{&amp;Tahoma8}Укажите сетевую папку для образа диска продукта на сервере.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsBrowseButton</td><td>1049</td><td>&amp;Обзор...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsBrowseFolderDlg_11</td><td>1049</td><td>{&amp;Tahoma8}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsBrowseFolderDlg_4</td><td>1049</td><td>{&amp;Tahoma8}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsBrowseFolderDlg_8</td><td>1049</td><td>{&amp;Tahoma8}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsBrowseFolderDlg_BrowseDestFolder</td><td>1049</td><td>{&amp;Tahoma8}Перейдите в папку назначения.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsBrowseFolderDlg_ChangeCurrentFolder</td><td>1049</td><td>{&amp;MSSansBold8}Изменение текущей папки назначения</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsBrowseFolderDlg_CreateFolder</td><td>1049</td><td>Создать новую папку|</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsBrowseFolderDlg_FolderName</td><td>1049</td><td>{&amp;Tahoma8}&amp;Имя папки:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsBrowseFolderDlg_LookIn</td><td>1049</td><td>{&amp;Tahoma8}&amp;Поиск в папке:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsBrowseFolderDlg_OK</td><td>1049</td><td>{&amp;Tahoma8}OK</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsBrowseFolderDlg_UpOneLevel</td><td>1049</td><td>Переход на один уровень вверх|</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsBrowseForAccount</td><td>1049</td><td>Поиск учетной записи пользователя</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsBrowseGroup</td><td>1049</td><td>Выберите группу со списком пользователей</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsBrowseUsernameTitle</td><td>1049</td><td>Выберите имя пользователя</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsCancelDlg_ConfirmCancel</td><td>1049</td><td>{&amp;Tahoma8}Отменить установку [ProductName]?</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsCancelDlg_No</td><td>1049</td><td>{&amp;Tahoma8}&amp;Нет</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsCancelDlg_Yes</td><td>1049</td><td>{&amp;Tahoma8}Д&amp;а</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsConfirmPassword</td><td>1049</td><td>Под&amp;тверждение:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsCreateNewUserTitle</td><td>1049</td><td>Сведения о новом пользователе</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsCreateUserBrowse</td><td>1049</td><td>&amp;Сведения о новом пользователе...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsCustomSelectionDlg_Change</td><td>1049</td><td>{&amp;Tahoma8}&amp;Изменить...</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsCustomSelectionDlg_ClickFeatureIcon</td><td>1049</td><td>{&amp;Tahoma8}Щелкните значок в списке ниже, чтобы изменить способ установки компонента.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsCustomSelectionDlg_CustomSetup</td><td>1049</td><td>{&amp;MSSansBold8}Выборочная установка</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsCustomSelectionDlg_FeatureDescription</td><td>1049</td><td>{&amp;Tahoma8}Описание компонента</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsCustomSelectionDlg_FeaturePath</td><td>1049</td><td>{&amp;Tahoma8}&lt;выбранный путь компонента&gt;</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsCustomSelectionDlg_FeatureSize</td><td>1049</td><td>{&amp;Tahoma8}Размер компонента</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsCustomSelectionDlg_Help</td><td>1049</td><td>{&amp;Tahoma8}&amp;Справка</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsCustomSelectionDlg_InstallTo</td><td>1049</td><td>{&amp;Tahoma8}Папка:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsCustomSelectionDlg_MultilineDescription</td><td>1049</td><td>{&amp;Tahoma8}Многострочное описание выбранного элемента</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsCustomSelectionDlg_SelectFeatures</td><td>1049</td><td>{&amp;Tahoma8}Выберите компоненты программы, которые необходимо установить.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsCustomSelectionDlg_Space</td><td>1049</td><td>{&amp;Tahoma8}Дис&amp;к</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsDiskSpaceDlg_DiskSpace</td><td>1049</td><td>{&amp;Tahoma8}Свободного места на диске меньше, чем требуется для установки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsDiskSpaceDlg_HighlightedVolumes</td><td>1049</td><td>{&amp;Tahoma8}На указанных дисках недостаточно места для установки выбранных компонентов. Можно удалить файлы с выбранных дисков, установить меньше компонентов на локальные диски или выбрать другие диски.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsDiskSpaceDlg_Numbers</td><td>1049</td><td>{&amp;Tahoma8}{120}{70}{70}{70}{70}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsDiskSpaceDlg_OK</td><td>1049</td><td>{&amp;Tahoma8}&amp;OK</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsDiskSpaceDlg_OutOfDiskSpace</td><td>1049</td><td>{&amp;MSSansBold8}Не хватает места на диске</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsDomainOrServer</td><td>1049</td><td>Д&amp;омен или сервер:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsErrorDlg_Abort</td><td>1049</td><td>{&amp;Tahoma8}&amp;Прервать</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsErrorDlg_ErrorText</td><td>1049</td><td>{&amp;Tahoma8}&lt;error text goes here&gt;&lt;error text goes here&gt;&lt;error text goes here&gt;&lt;error text goes here&gt;&lt;error text goes here&gt;&lt;error text goes here&gt;&lt;error text goes here&gt;&lt;error text goes here&gt;&lt;error text goes here&gt;&lt;error text goes here&gt;&lt;error text goes here&gt;</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsErrorDlg_Ignore</td><td>1049</td><td>{&amp;Tahoma8}&amp;Пропустить</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsErrorDlg_InstallerInfo</td><td>1049</td><td>Сведения о программе установки [ProductName]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsErrorDlg_NO</td><td>1049</td><td>{&amp;Tahoma8}&amp;Нет</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsErrorDlg_OK</td><td>1049</td><td>{&amp;Tahoma8}&amp;OK</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsErrorDlg_Retry</td><td>1049</td><td>{&amp;Tahoma8}По&amp;вторить</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsErrorDlg_Yes</td><td>1049</td><td>{&amp;Tahoma8}Д&amp;а</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsExitDialog_Finish</td><td>1049</td><td>{&amp;Tahoma8}&amp;Готово</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsExitDialog_InstallSuccess</td><td>1049</td><td>{&amp;Tahoma8}Программа InstallShield Wizard успешно установила [ProductName]. Нажмите кнопку "Готово" для выхода из программы.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsExitDialog_LaunchProgram</td><td>1049</td><td>Запустить программу</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsExitDialog_ShowReadMe</td><td>1049</td><td>Показать файл readme</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsExitDialog_UninstallSuccess</td><td>1049</td><td>{&amp;Tahoma8}Программа установки InstallShield успешно удалила [ProductName]. Нажмите кнопку "Готово" для выхода из программы установки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsExitDialog_Update_InternetConnection</td><td>1049</td><td>Можно воспользоваться интернет-соединением и убедиться, что у вас самые последние обновления.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsExitDialog_Update_PossibleUpdates</td><td>1049</td><td>Некоторые программные файлы, возможно, были обновлены с тех пор, как вы приобрели свою копию [ProductName].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsExitDialog_Update_SetupFinished</td><td>1049</td><td>Операция установки завершила инсталляцию [ProductName].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsExitDialog_Update_YesCheckForUpdates</td><td>1049</td><td>&amp;Да, проверьте программные обновления (Рекомендация) после завершения операции установки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsExitDialog_WizardCompleted</td><td>1049</td><td>{&amp;TahomaBold10}Программа InstallShield Wizard завершена</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsFatalError_ClickFinish</td><td>1049</td><td>{&amp;Tahoma8}Нажмите кнопку "Готово" для выхода из программы.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsFatalError_Finish</td><td>1049</td><td>{&amp;Tahoma8}&amp;Готово</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsFatalError_KeepOrRestore</td><td>1049</td><td>{&amp;Tahoma8}Можно либо сохранить установленные элементы в системе, чтобы продолжить данную установку позднее, либо восстановить первоначальное состояние системы до установки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsFatalError_NotModified</td><td>1049</td><td>{&amp;Tahoma8}Параметры системы не изменены. Чтобы выполнить установку в другое время, запустите программу установки еще раз.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsFatalError_RestoreOrContinueLater</td><td>1049</td><td>{&amp;Tahoma8}Нажмите кнопку "Восстановить" или "Продолжить позже" для выхода из программы установки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsFatalError_WizardCompleted</td><td>1049</td><td>{&amp;TahomaBold10}Программа InstallShield Wizard завершена</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsFatalError_WizardInterrupted</td><td>1049</td><td>{&amp;Tahoma8}Выполнение программы было прервано до завершения установки [ProductName].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsFeatureDetailsDlg_DiskSpaceRequirements</td><td>1049</td><td>{&amp;MSSansBold8}Требования к дисковому пространству</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsFeatureDetailsDlg_Numbers</td><td>1049</td><td>{&amp;Tahoma8}{120}{70}{70}{70}{70}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsFeatureDetailsDlg_OK</td><td>1049</td><td>{&amp;Tahoma8}OK</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsFeatureDetailsDlg_SpaceRequired</td><td>1049</td><td>{&amp;Tahoma8}Требуемое пространство на диске для установки выбранных компонентов.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsFeatureDetailsDlg_VolumesTooSmall</td><td>1049</td><td>{&amp;Tahoma8}На указанных дисках недостаточно места для установки выбранных компонентов. Можно удалить файлы с выбранных дисков, установить меньше компонентов на локальные диски или выбрать другие диски.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsFilesInUse_ApplicationsUsingFiles</td><td>1049</td><td>{&amp;Tahoma8}Следующие приложения используют файлы, которые должна обновить данная программа установки. Закройте эти приложения и для продолжения нажмите кнопку "Повторить".</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsFilesInUse_Exit</td><td>1049</td><td>{&amp;Tahoma8}&amp;Выход</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsFilesInUse_FilesInUse</td><td>1049</td><td>{&amp;MSSansBold8}Используются файлы</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsFilesInUse_FilesInUseMessage</td><td>1049</td><td>{&amp;Tahoma8}Некоторые файлы, которые нуждаются в обновлении, в данный момент используются.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsFilesInUse_Ignore</td><td>1049</td><td>{&amp;Tahoma8}&amp;Пропустить</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsFilesInUse_Retry</td><td>1049</td><td>{&amp;Tahoma8}По&amp;вторить</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsGroup</td><td>1049</td><td>&amp;Группа со списком пользователей:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsGroupLabel</td><td>1049</td><td>&amp;Группа со списком пользователей:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsInitDlg_1</td><td>1049</td><td>{&amp;Tahoma8}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsInitDlg_2</td><td>1049</td><td>{&amp;Tahoma8}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsInitDlg_PreparingWizard</td><td>1049</td><td>{&amp;Tahoma8}Программа установки [ProductName] подготавливает программу InstallShield Wizard, которая поможет выполнить установку.  Пожалуйста, подождите.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsInitDlg_WelcomeWizard</td><td>1049</td><td>{&amp;VerdanaBold14}Вас приветствует программа InstallShield Wizard для [ProductName]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsLicenseDlg_LicenseAgreement</td><td>1049</td><td>{&amp;MSSansBold8}Лицензионное соглашение</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsLicenseDlg_ReadLicenseAgreement</td><td>1049</td><td>{&amp;Tahoma8}Пожалуйста, внимательно прочтите следующее лицензионное соглашение.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsLogonInfoDescription</td><td>1049</td><td>Укажите учетную запись пользователя, которую следует использовать в этом приложении. Учетные записи пользователей должны соответствовать формату ДОМЕН\Имя пользователя.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsLogonInfoTitle</td><td>1049</td><td>{&amp;MSSansBold8}Вход в систему</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsLogonInfoTitleDescription</td><td>1049</td><td>Укажите имя пользователя и пароль</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsLogonNewUserDescription</td><td>1049</td><td>Нажмите одну из следующих кнопок для указания сведений о новом пользователе, который будет создан во время установки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsMaintenanceDlg_ChangeFeatures</td><td>1049</td><td>{&amp;Tahoma8}Изменение устанавливаемых компонентов программы. Появится диалоговое окно "Выборочная установка", в котором можно изменить способ установки компонентов.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsMaintenanceDlg_MaitenanceOptions</td><td>1049</td><td>{&amp;Tahoma8}Изменение, исправление или удаление программы.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsMaintenanceDlg_Modify</td><td>1049</td><td>{&amp;MSSansBold8}&amp;Изменить</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsMaintenanceDlg_ProgramMaintenance</td><td>1049</td><td>{&amp;MSSansBold8}Обслуживание программ</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsMaintenanceDlg_Remove</td><td>1049</td><td>{&amp;MSSansBold8}&amp;Удалить</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsMaintenanceDlg_RemoveProductName</td><td>1049</td><td>{&amp;Tahoma8}Удаление [ProductName] из компьютера.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsMaintenanceDlg_Repair</td><td>1049</td><td>{&amp;MSSansBold8}Ис&amp;править</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsMaintenanceDlg_RepairMessage</td><td>1049</td><td>{&amp;Tahoma8}Исправление ошибок в программе. При выборе данного параметра исправляются отсутствующие или поврежденные файлы, ярлыки и записи реестра.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsMaintenanceWelcome_MaintenanceOptionsDescription</td><td>1049</td><td>{&amp;Tahoma8}Программа InstallShield(R) Wizard предоставит возможность изменить, исправить или удалить [ProductName]. Для продолжения нажмите кнопку "Далее".</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsMaintenanceWelcome_WizardWelcome</td><td>1049</td><td>{&amp;TahomaBold10}Вас приветствует программа InstallShield(R) для [ProductName]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsMsiRMFilesInUse_ApplicationsUsingFiles</td><td>1049</td><td>Следующие приложения используют файлы, которые должна обновить данная программа установки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsMsiRMFilesInUse_CloseRestart</td><td>1049</td><td>Автоматически закрыть и попробовать перезапустить приложения.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsMsiRMFilesInUse_RebootAfter</td><td>1049</td><td>Не закрывать приложения. (Потребуется перезагрузка.)</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsPatchDlg_PatchClickUpdate</td><td>1049</td><td>Мастер InstallShield(R) Wizard установит программу-заплату для [ProductName] на компьютер.  Для продолжения нажмите кнопку ""Обновить"".</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsPatchDlg_PatchWizard</td><td>1049</td><td>Программа-заплата [ProductName] - InstallShield Wizard</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsPatchDlg_Update</td><td>1049</td><td>&amp;Обновить &gt;</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsPatchDlg_WelcomePatchWizard</td><td>1049</td><td>{&amp;TahomaBold10}Добро пожаловать в программу-заплату для [ProductName]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsProgressDlg_2</td><td>1049</td><td>{&amp;Tahoma8}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsProgressDlg_Hidden</td><td>1049</td><td>{&amp;Tahoma8}(Hidden for now)</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsProgressDlg_HiddenTimeRemaining</td><td>1049</td><td>{&amp;Tahoma8}(Hidden for now)Осталось примерно:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsProgressDlg_InstallingProductName</td><td>1049</td><td>{&amp;MSSansBold8}Установка [ProductName]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsProgressDlg_ProgressDone</td><td>1049</td><td>Выполнено</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsProgressDlg_SecHidden</td><td>1049</td><td>{&amp;Tahoma8}(Hidden for now)с.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsProgressDlg_Status</td><td>1049</td><td>{&amp;Tahoma8}Состояние:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsProgressDlg_Uninstalling</td><td>1049</td><td>{&amp;MSSansBold8}Удаление [ProductName]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsProgressDlg_UninstallingFeatures</td><td>1049</td><td>{&amp;Tahoma8}Удаление выбранных компонентов программы.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsProgressDlg_UninstallingFeatures2</td><td>1049</td><td>{&amp;Tahoma8}Устанавливаются выбранные компоненты программы.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsProgressDlg_WaitUninstall</td><td>1049</td><td>{&amp;Tahoma8}Дождитесь окончания удаления [ProductName]. Это может занять несколько минут.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsProgressDlg_WaitUninstall2</td><td>1049</td><td>{&amp;Tahoma8}Дождитесь окончания установки [ProductName]. Это может занять несколько минут.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsReadmeDlg_Cancel</td><td>1049</td><td>&amp;Отмена</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsReadmeDlg_PleaseReadInfo</td><td>1049</td><td>Внимательно прочитайте следующую информацию в файле readme.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsReadmeDlg_ReadMeInfo</td><td>1049</td><td>{&amp;MSSansBold8}Информация в файле Readme</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsRegisterUserDlg_16</td><td>1049</td><td>{&amp;Tahoma8}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsRegisterUserDlg_Anyone</td><td>1049</td><td>{&amp;Tahoma8}&amp;всех пользователей данного компьютера</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsRegisterUserDlg_CustomerInformation</td><td>1049</td><td>{&amp;MSSansBold8}Сведения о пользователе</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsRegisterUserDlg_InstallFor</td><td>1049</td><td>{&amp;Tahoma8}Установить приложение для:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsRegisterUserDlg_OnlyMe</td><td>1049</td><td>{&amp;Tahoma8}только для &amp;меня ([USERNAME])</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsRegisterUserDlg_Organization</td><td>1049</td><td>{&amp;Tahoma8}&amp;Организация:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsRegisterUserDlg_PleaseEnterInfo</td><td>1049</td><td>{&amp;Tahoma8}Укажите сведения о себе.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsRegisterUserDlg_SerialNumber</td><td>1049</td><td>&amp;Серийный номер:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsRegisterUserDlg_Tahoma50</td><td>1049</td><td>{50}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsRegisterUserDlg_Tahoma80</td><td>1049</td><td>{80}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsRegisterUserDlg_UserName</td><td>1049</td><td>{&amp;Tahoma8}&amp;Пользователь:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsResumeDlg_ResumeSuspended</td><td>1049</td><td>{&amp;Tahoma8}Программа InstallShield(R) Wizard выполнит отложенную установку [ProductName] на вашем компьютере. Для продолжения нажмите кнопку "Далее".</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsResumeDlg_Resuming</td><td>1049</td><td>{&amp;TahomaBold10}Продолжение выполнения программы InstallShield Wizard для [ProductName]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsResumeDlg_WizardResume</td><td>1049</td><td>{&amp;Tahoma8}Программа InstallShield(R) Wizard закончит установку [ProductName] на компьютере. Для продолжения нажмите кнопку "Далее".</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsSelectDomainOrServer</td><td>1049</td><td>Выберите домен или сервер</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsSelectDomainUserInstructions</td><td>1049</td><td>С помощью кнопок обзора выберите "домен\сервер" и имя пользователя.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsSetupComplete_ShowMsiLog</td><td>1049</td><td>Показать журнал установщика Windows</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsSetupTypeMinDlg_13</td><td>1049</td><td>{&amp;Tahoma8}</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsSetupTypeMinDlg_AllFeatures</td><td>1049</td><td>{&amp;Tahoma8}Будут установлены все компоненты программы. (Требуется больше всего места на диске.)</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsSetupTypeMinDlg_ChooseFeatures</td><td>1049</td><td>{&amp;Tahoma8}Выбор необходимых компонентов программы и папки, в которой они будут установлены. Рассчитана на опытных пользователей.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsSetupTypeMinDlg_ChooseSetupType</td><td>1049</td><td>{&amp;Tahoma8}Выбор наиболее подходящего вида установки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsSetupTypeMinDlg_Complete</td><td>1049</td><td>{&amp;MSSansBold8}&amp;Полная</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsSetupTypeMinDlg_Custom</td><td>1049</td><td>{&amp;MSSansBold8}&amp;Выборочная</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsSetupTypeMinDlg_Minimal</td><td>1049</td><td>{&amp;MSSansBold8}&amp;Минимальная</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsSetupTypeMinDlg_MinimumFeatures</td><td>1049</td><td>{&amp;Tahoma8}Будет установлен минимальный набор функций</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsSetupTypeMinDlg_SelectSetupType</td><td>1049</td><td>{&amp;Tahoma8}Выберите вид установки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsSetupTypeMinDlg_SetupType</td><td>1049</td><td>{&amp;MSSansBold8}Вид установки</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsSetupTypeMinDlg_Typical</td><td>1049</td><td>{&amp;MSSansBold8}&amp;Обычная</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsUserExit_ClickFinish</td><td>1049</td><td>{&amp;Tahoma8}Нажмите кнопку "Готово" для выхода из программы.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsUserExit_Finish</td><td>1049</td><td>{&amp;Tahoma8}&amp;Готово</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsUserExit_KeepOrRestore</td><td>1049</td><td>{&amp;Tahoma8}Можно либо сохранить установленные элементы в системе, чтобы продолжить данную установку позднее, либо восстановить первоначальное состояние системы до установки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsUserExit_NotModified</td><td>1049</td><td>{&amp;Tahoma8}Параметры системы не изменены. В дальнейшем, чтобы установить программу, запустите программу установки еще раз.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsUserExit_RestoreOrContinue</td><td>1049</td><td>{&amp;Tahoma8}Нажмите кнопку "Восстановить" или "Продолжить позже" для выхода из программы.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsUserExit_WizardCompleted</td><td>1049</td><td>{&amp;TahomaBold10}Программа InstallShield Wizard завершена</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsUserExit_WizardInterrupted</td><td>1049</td><td>{&amp;Tahoma8}Выполнение программы было прервано до завершения установки [ProductName].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsUserNameLabel</td><td>1049</td><td>&amp;Имя пользователя:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyReadyDlg_BackOrCancel</td><td>1049</td><td>{&amp;Tahoma8}Чтобы просмотреть или изменить параметры установки, нажмите кнопку "Назад". Нажмите кнопку "Отмена" для выхода из программы.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyReadyDlg_ClickInstall</td><td>1049</td><td>{&amp;Tahoma8}Нажмите кнопку "Установить", чтобы начать установку.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyReadyDlg_Company</td><td>1049</td><td>Организация: [COMPANYNAME]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyReadyDlg_CurrentSettings</td><td>1049</td><td>Текущие установки:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyReadyDlg_DestFolder</td><td>1049</td><td>Папка назначения:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyReadyDlg_Install</td><td>1049</td><td>{&amp;Tahoma8}&amp;Установить</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyReadyDlg_Installdir</td><td>1049</td><td>[INSTALLDIR]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyReadyDlg_ModifyReady</td><td>1049</td><td>{&amp;MSSansBold8}Готова к изменению программы</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyReadyDlg_ReadyInstall</td><td>1049</td><td>{&amp;MSSansBold8}Готова к установке программы</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyReadyDlg_ReadyRepair</td><td>1049</td><td>{&amp;MSSansBold8}Готова к исправлению программы</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyReadyDlg_SelectedSetupType</td><td>1049</td><td>[SelectedSetupType]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyReadyDlg_Serial</td><td>1049</td><td>Серийный номер: [ISX_SERIALNUM]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyReadyDlg_SetupType</td><td>1049</td><td>Вид установки:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyReadyDlg_UserInfo</td><td>1049</td><td>Сведения о пользователе:</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyReadyDlg_UserName</td><td>1049</td><td>Имя: [USERNAME]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyReadyDlg_WizardReady</td><td>1049</td><td>{&amp;Tahoma8}Программа готова к началу установки.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyRemoveAllDlg_ChoseRemoveProgram</td><td>1049</td><td>{&amp;Tahoma8}Вы собираетесь удалить программу из компьютера.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyRemoveAllDlg_ClickBack</td><td>1049</td><td>{&amp;Tahoma8}Чтобы просмотреть или изменить настройки, нажмите кнопку "Назад".</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyRemoveAllDlg_ClickRemove</td><td>1049</td><td>{&amp;Tahoma8}Нажмите кнопку "Удалить", чтобы удалить [ProductName] из компьютера. После удаления данная программа будет недоступна.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyRemoveAllDlg_Remove</td><td>1049</td><td>{&amp;Tahoma8}&amp;Удалить</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsVerifyRemoveAllDlg_RemoveProgram</td><td>1049</td><td>{&amp;MSSansBold8}Удаление программы</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsWelcomeDlg_InstallProductName</td><td>1049</td><td>{&amp;Tahoma8}Программа InstallShield(R) Wizard выполнит установку [ProductName] на компьютер. Для продолжения нажмите кнопку "Далее".</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsWelcomeDlg_WarningCopyright</td><td>1049</td><td>ПРЕДУПРЕЖДЕНИЕ: Данная программа защищена законами об авторских правах и международными соглашениями.</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__IsWelcomeDlg_WelcomeProductName</td><td>1049</td><td>{&amp;TahomaBold10}Вас приветствует программа InstallShield Wizard для [ProductName]</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__TargetReq_DESC_COLOR</td><td>1049</td><td>Настройки цвета системы не подходят для работы [ProductName].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__TargetReq_DESC_OS</td><td>1049</td><td>Операционная система не подходит для работы [ProductName].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__TargetReq_DESC_PROCESSOR</td><td>1049</td><td>Процессор не подходит для работы [ProductName].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__TargetReq_DESC_RAM</td><td>1049</td><td>Объем памяти ОЗУ не достаточен для работы [ProductName].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IDS__TargetReq_DESC_RESOLUTION</td><td>1049</td><td>Разрешение экрана не подходит для работы [ProductName].</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>ID_STRING1</td><td>1049</td><td>https://www.ncmanager.com</td><td>0</td><td/><td>1001043215</td></row>
		<row><td>ID_STRING2</td><td>1049</td><td>Your Company Name</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>ID_STRING3</td><td>1033</td><td>support@ncmanager.com</td><td>0</td><td/><td>1001053519</td></row>
		<row><td>ID_STRING3</td><td>1049</td><td>support@ncmanager.com</td><td>0</td><td/><td>1001053519</td></row>
		<row><td>ID_STRING5</td><td>1049</td><td>0</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>ID_STRING6</td><td>1049</td><td>INTEKS</td><td>0</td><td/><td>715803980</td></row>
		<row><td>ID_STRING7</td><td>1049</td><td>+7</td><td>0</td><td/><td>-760690007</td></row>
		<row><td>IIDS_UITEXT_FeatureUninstalled</td><td>1049</td><td>Данный компонент не будет удален.</td><td>0</td><td/><td>-760690007</td></row>
	</table>

	<table name="ISSwidtagProperty">
		<col key="yes" def="s72">Name</col>
		<col def="s0">Value</col>
		<row><td>UniqueId</td><td>0DBD709C-C74C-405E-B365-0FE34605535F</td></row>
	</table>

	<table name="ISTargetImage">
		<col key="yes" def="s13">UpgradedImage_</col>
		<col key="yes" def="s13">Name</col>
		<col def="s0">MsiPath</col>
		<col def="i2">Order</col>
		<col def="I4">Flags</col>
		<col def="i2">IgnoreMissingFiles</col>
	</table>

	<table name="ISUpgradeMsiItem">
		<col key="yes" def="s72">UpgradeItem</col>
		<col def="s0">ObjectSetupPath</col>
		<col def="S255">ISReleaseFlags</col>
		<col def="i2">ISAttributes</col>
	</table>

	<table name="ISUpgradedImage">
		<col key="yes" def="s13">Name</col>
		<col def="s0">MsiPath</col>
		<col def="s8">Family</col>
	</table>

	<table name="ISVirtualDirectory">
		<col key="yes" def="s72">Directory_</col>
		<col key="yes" def="s72">Name</col>
		<col def="s255">Value</col>
	</table>

	<table name="ISVirtualFile">
		<col key="yes" def="s72">File_</col>
		<col key="yes" def="s72">Name</col>
		<col def="s255">Value</col>
	</table>

	<table name="ISVirtualPackage">
		<col key="yes" def="s72">Name</col>
		<col def="s255">Value</col>
	</table>

	<table name="ISVirtualRegistry">
		<col key="yes" def="s72">Registry_</col>
		<col key="yes" def="s72">Name</col>
		<col def="s255">Value</col>
	</table>

	<table name="ISVirtualRelease">
		<col key="yes" def="s72">ISRelease_</col>
		<col key="yes" def="s72">ISProductConfiguration_</col>
		<col key="yes" def="s255">Name</col>
		<col def="s255">Value</col>
	</table>

	<table name="ISVirtualShortcut">
		<col key="yes" def="s72">Shortcut_</col>
		<col key="yes" def="s72">Name</col>
		<col def="s255">Value</col>
	</table>

	<table name="ISWSEWrap">
		<col key="yes" def="s72">Action_</col>
		<col key="yes" def="s72">Name</col>
		<col def="S0">Value</col>
	</table>

	<table name="ISXmlElement">
		<col key="yes" def="s72">ISXmlElement</col>
		<col def="s72">ISXmlFile_</col>
		<col def="S72">ISXmlElement_Parent</col>
		<col def="L0">XPath</col>
		<col def="L0">Content</col>
		<col def="I4">ISAttributes</col>
	</table>

	<table name="ISXmlElementAttrib">
		<col key="yes" def="s72">ISXmlElementAttrib</col>
		<col key="yes" def="s72">ISXmlElement_</col>
		<col def="L255">Name</col>
		<col def="L0">Value</col>
		<col def="I4">ISAttributes</col>
	</table>

	<table name="ISXmlFile">
		<col key="yes" def="s72">ISXmlFile</col>
		<col def="l255">FileName</col>
		<col def="s72">Component_</col>
		<col def="s72">Directory</col>
		<col def="I4">ISAttributes</col>
		<col def="S0">SelectionNamespaces</col>
		<col def="S255">Encoding</col>
	</table>

	<table name="ISXmlLocator">
		<col key="yes" def="s72">Signature_</col>
		<col key="yes" def="S72">Parent</col>
		<col def="S255">Element</col>
		<col def="S255">Attribute</col>
		<col def="I2">ISAttributes</col>
	</table>

	<table name="Icon">
		<col key="yes" def="s72">Name</col>
		<col def="V0">Data</col>
		<col def="S255">ISBuildSourcePath</col>
		<col def="I2">ISIconIndex</col>
		<row><td>ARPPRODUCTICON.exe</td><td/><td>&lt;ISProductFolder&gt;\redist\Language Independent\OS Independent\setupicon.ico</td><td>0</td></row>
		<row><td>_14C8D792_D9F0_4CDE_AB1B_B628D8D90038</td><td/><td>C:\Development\NCM\Release\ResInv.dll</td><td>0</td></row>
		<row><td>_1C7A9C5E1DEC47A6B_2EC30B151BEA417BA8507245305AA8CB.exe</td><td/><td>C:\Development\NCM\Release\ResInv.dll</td><td>0</td></row>
		<row><td>_4C5652CC_B110_44C0_ABC2_99BF9E09BD49</td><td/><td>C:\Development\NCM\Release\ResInv.dll</td><td>0</td></row>
	</table>

	<table name="IniFile">
		<col key="yes" def="s72">IniFile</col>
		<col def="l255">FileName</col>
		<col def="S72">DirProperty</col>
		<col def="l255">Section</col>
		<col def="l128">Key</col>
		<col def="s255">Value</col>
		<col def="i2">Action</col>
		<col def="s72">Component_</col>
	</table>

	<table name="IniLocator">
		<col key="yes" def="s72">Signature_</col>
		<col def="s255">FileName</col>
		<col def="s96">Section</col>
		<col def="s128">Key</col>
		<col def="I2">Field</col>
		<col def="I2">Type</col>
	</table>

	<table name="InstallExecuteSequence">
		<col key="yes" def="s72">Action</col>
		<col def="S255">Condition</col>
		<col def="I2">Sequence</col>
		<col def="S255">ISComments</col>
		<col def="I4">ISAttributes</col>
		<row><td>AllocateRegistrySpace</td><td>NOT Installed</td><td>1550</td><td>AllocateRegistrySpace</td><td/></row>
		<row><td>AppSearch</td><td/><td>400</td><td>AppSearch</td><td/></row>
		<row><td>BindImage</td><td/><td>4300</td><td>BindImage</td><td/></row>
		<row><td>CCPSearch</td><td>CCP_TEST</td><td>500</td><td>CCPSearch</td><td/></row>
		<row><td>CostFinalize</td><td/><td>1000</td><td>CostFinalize</td><td/></row>
		<row><td>CostInitialize</td><td/><td>800</td><td>CostInitialize</td><td/></row>
		<row><td>CreateFolders</td><td/><td>3700</td><td>CreateFolders</td><td/></row>
		<row><td>CreateShortcuts</td><td/><td>4500</td><td>CreateShortcuts</td><td/></row>
		<row><td>DeleteServices</td><td>VersionNT</td><td>2000</td><td>DeleteServices</td><td/></row>
		<row><td>DuplicateFiles</td><td/><td>4210</td><td>DuplicateFiles</td><td/></row>
		<row><td>FileCost</td><td/><td>900</td><td>FileCost</td><td/></row>
		<row><td>FindRelatedProducts</td><td>NOT ISSETUPDRIVEN</td><td>420</td><td>FindRelatedProducts</td><td/></row>
		<row><td>ISPreventDowngrade</td><td>ISFOUNDNEWERPRODUCTVERSION</td><td>450</td><td>ISPreventDowngrade</td><td/></row>
		<row><td>ISRunSetupTypeAddLocalEvent</td><td>Not Installed And Not ISRUNSETUPTYPEADDLOCALEVENT</td><td>1050</td><td>ISRunSetupTypeAddLocalEvent</td><td/></row>
		<row><td>ISSelfRegisterCosting</td><td/><td>2201</td><td/><td/></row>
		<row><td>ISSelfRegisterFiles</td><td/><td>5601</td><td/><td/></row>
		<row><td>ISSelfRegisterFinalize</td><td/><td>6601</td><td/><td/></row>
		<row><td>ISUnSelfRegisterFiles</td><td/><td>2202</td><td/><td/></row>
		<row><td>InstallFiles</td><td/><td>4000</td><td>InstallFiles</td><td/></row>
		<row><td>InstallFinalize</td><td/><td>6600</td><td>InstallFinalize</td><td/></row>
		<row><td>InstallInitialize</td><td/><td>1501</td><td>InstallInitialize</td><td/></row>
		<row><td>InstallODBC</td><td/><td>5400</td><td>InstallODBC</td><td/></row>
		<row><td>InstallServices</td><td>VersionNT</td><td>5800</td><td>InstallServices</td><td/></row>
		<row><td>InstallValidate</td><td/><td>1400</td><td>InstallValidate</td><td/></row>
		<row><td>IsolateComponents</td><td/><td>950</td><td>IsolateComponents</td><td/></row>
		<row><td>LaunchConditions</td><td>Not Installed</td><td>410</td><td>LaunchConditions</td><td/></row>
		<row><td>MigrateFeatureStates</td><td/><td>1010</td><td>MigrateFeatureStates</td><td/></row>
		<row><td>MoveFiles</td><td/><td>3800</td><td>MoveFiles</td><td/></row>
		<row><td>MsiConfigureServices</td><td>VersionMsi &gt;= "5.00"</td><td>5850</td><td>MSI5 MsiConfigureServices</td><td/></row>
		<row><td>MsiPublishAssemblies</td><td/><td>6250</td><td>MsiPublishAssemblies</td><td/></row>
		<row><td>MsiUnpublishAssemblies</td><td/><td>1750</td><td>MsiUnpublishAssemblies</td><td/></row>
		<row><td>PatchFiles</td><td/><td>4090</td><td>PatchFiles</td><td/></row>
		<row><td>ProcessComponents</td><td/><td>1600</td><td>ProcessComponents</td><td/></row>
		<row><td>PublishComponents</td><td/><td>6200</td><td>PublishComponents</td><td/></row>
		<row><td>PublishFeatures</td><td/><td>6300</td><td>PublishFeatures</td><td/></row>
		<row><td>PublishProduct</td><td/><td>6400</td><td>PublishProduct</td><td/></row>
		<row><td>RMCCPSearch</td><td>Not CCP_SUCCESS And CCP_TEST</td><td>600</td><td>RMCCPSearch</td><td/></row>
		<row><td>RegisterClassInfo</td><td/><td>4600</td><td>RegisterClassInfo</td><td/></row>
		<row><td>RegisterComPlus</td><td/><td>5700</td><td>RegisterComPlus</td><td/></row>
		<row><td>RegisterExtensionInfo</td><td/><td>4700</td><td>RegisterExtensionInfo</td><td/></row>
		<row><td>RegisterFonts</td><td/><td>5300</td><td>RegisterFonts</td><td/></row>
		<row><td>RegisterMIMEInfo</td><td/><td>4900</td><td>RegisterMIMEInfo</td><td/></row>
		<row><td>RegisterProduct</td><td/><td>6100</td><td>RegisterProduct</td><td/></row>
		<row><td>RegisterProgIdInfo</td><td/><td>4800</td><td>RegisterProgIdInfo</td><td/></row>
		<row><td>RegisterTypeLibraries</td><td/><td>5500</td><td>RegisterTypeLibraries</td><td/></row>
		<row><td>RegisterUser</td><td/><td>6000</td><td>RegisterUser</td><td/></row>
		<row><td>RemoveDuplicateFiles</td><td/><td>3400</td><td>RemoveDuplicateFiles</td><td/></row>
		<row><td>RemoveEnvironmentStrings</td><td/><td>3300</td><td>RemoveEnvironmentStrings</td><td/></row>
		<row><td>RemoveExistingProducts</td><td/><td>6601</td><td>RemoveExistingProducts</td><td/></row>
		<row><td>RemoveFiles</td><td/><td>3500</td><td>RemoveFiles</td><td/></row>
		<row><td>RemoveFolders</td><td/><td>3600</td><td>RemoveFolders</td><td/></row>
		<row><td>RemoveIniValues</td><td/><td>3100</td><td>RemoveIniValues</td><td/></row>
		<row><td>RemoveODBC</td><td/><td>2400</td><td>RemoveODBC</td><td/></row>
		<row><td>RemoveRegistryValues</td><td/><td>2600</td><td>RemoveRegistryValues</td><td/></row>
		<row><td>RemoveShortcuts</td><td/><td>3200</td><td>RemoveShortcuts</td><td/></row>
		<row><td>ResolveSource</td><td>Not Installed</td><td>850</td><td>ResolveSource</td><td/></row>
		<row><td>ScheduleReboot</td><td>ISSCHEDULEREBOOT</td><td>6410</td><td>ScheduleReboot</td><td/></row>
		<row><td>SelfRegModules</td><td/><td>5600</td><td>SelfRegModules</td><td/></row>
		<row><td>SelfUnregModules</td><td/><td>2200</td><td>SelfUnregModules</td><td/></row>
		<row><td>SetARPINSTALLLOCATION</td><td/><td>1100</td><td>SetARPINSTALLLOCATION</td><td/></row>
		<row><td>SetAllUsersProfileNT</td><td>VersionNT = 400</td><td>970</td><td/><td/></row>
		<row><td>SetODBCFolders</td><td/><td>1200</td><td>SetODBCFolders</td><td/></row>
		<row><td>StartServices</td><td>VersionNT</td><td>5900</td><td>StartServices</td><td/></row>
		<row><td>StopServices</td><td>VersionNT</td><td>1900</td><td>StopServices</td><td/></row>
		<row><td>UnpublishComponents</td><td/><td>1700</td><td>UnpublishComponents</td><td/></row>
		<row><td>UnpublishFeatures</td><td/><td>1800</td><td>UnpublishFeatures</td><td/></row>
		<row><td>UnregisterClassInfo</td><td/><td>2700</td><td>UnregisterClassInfo</td><td/></row>
		<row><td>UnregisterComPlus</td><td/><td>2100</td><td>UnregisterComPlus</td><td/></row>
		<row><td>UnregisterExtensionInfo</td><td/><td>2800</td><td>UnregisterExtensionInfo</td><td/></row>
		<row><td>UnregisterFonts</td><td/><td>2500</td><td>UnregisterFonts</td><td/></row>
		<row><td>UnregisterMIMEInfo</td><td/><td>3000</td><td>UnregisterMIMEInfo</td><td/></row>
		<row><td>UnregisterProgIdInfo</td><td/><td>2900</td><td>UnregisterProgIdInfo</td><td/></row>
		<row><td>UnregisterTypeLibraries</td><td/><td>2300</td><td>UnregisterTypeLibraries</td><td/></row>
		<row><td>ValidateProductID</td><td/><td>700</td><td>ValidateProductID</td><td/></row>
		<row><td>WriteEnvironmentStrings</td><td/><td>5200</td><td>WriteEnvironmentStrings</td><td/></row>
		<row><td>WriteIniValues</td><td/><td>5100</td><td>WriteIniValues</td><td/></row>
		<row><td>WriteRegistryValues</td><td/><td>5000</td><td>WriteRegistryValues</td><td/></row>
		<row><td>setAllUsersProfile2K</td><td>VersionNT &gt;= 500</td><td>980</td><td/><td/></row>
		<row><td>setUserProfileNT</td><td>VersionNT</td><td>960</td><td/><td/></row>
	</table>

	<table name="InstallShield">
		<col key="yes" def="s72">Property</col>
		<col def="S0">Value</col>
		<row><td>ActiveLanguage</td><td>1049</td></row>
		<row><td>Comments</td><td/></row>
		<row><td>CurrentMedia</td><td dt:dt="bin.base64" md5="de9f554a3bc05c12be9c31b998217995">
UwBpAG4AZwBsAGUASQBtAGEAZwBlAAEARQB4AHAAcgBlAHMAcwA=
			</td></row>
		<row><td>DefaultProductConfiguration</td><td>Express</td></row>
		<row><td>EnableSwidtag</td><td>0</td></row>
		<row><td>ISCompilerOption_CompileBeforeBuild</td><td>1</td></row>
		<row><td>ISCompilerOption_Debug</td><td>0</td></row>
		<row><td>ISCompilerOption_IncludePath</td><td/></row>
		<row><td>ISCompilerOption_LibraryPath</td><td/></row>
		<row><td>ISCompilerOption_MaxErrors</td><td>50</td></row>
		<row><td>ISCompilerOption_MaxWarnings</td><td>50</td></row>
		<row><td>ISCompilerOption_OutputPath</td><td>&lt;ISProjectDataFolder&gt;\Script Files</td></row>
		<row><td>ISCompilerOption_PreProcessor</td><td>_ISSCRIPT_NEW_STYLE_DLG_DEFS</td></row>
		<row><td>ISCompilerOption_WarningLevel</td><td>3</td></row>
		<row><td>ISCompilerOption_WarningsAsErrors</td><td>1</td></row>
		<row><td>ISTheme</td><td>InstallShield Blue.theme</td></row>
		<row><td>ISUSLock</td><td>{8278B1C3-85D9-42BC-9A75-4A3596E55FCF}</td></row>
		<row><td>ISUSSignature</td><td>{9A683E55-EE5F-41CC-A416-8FC90F73DBE8}</td></row>
		<row><td>ISVisitedViews</td><td>viewAssistant,viewISToday,viewAppV,viewProject,viewRealSetupDesign,viewSetupDesign,viewSetupTypes,viewUpgradePaths,viewUpdateService,viewAppFiles,viewFeatureFiles,viewObjects,viewShortcuts,viewRegistry,viewIniFiles,viewTextFiles,viewXMLConfig,viewFileExtensions,viewEnvironmentVariables,viewVRoots,viewUI,viewRelease,viewDesignPatches,viewSupportFiles,viewCustomActions,viewInstallScriptStd,viewSystemSearch,viewDependencies,viewServices,viewTextMessages,viewLearnMore</td></row>
		<row><td>Limited</td><td>1</td></row>
		<row><td>LockPermissionMode</td><td>1</td></row>
		<row><td>MsiExecCmdLineOptions</td><td/></row>
		<row><td>MsiLogFile</td><td/></row>
		<row><td>OnUpgrade</td><td>0</td></row>
		<row><td>Owner</td><td/></row>
		<row><td>PatchFamily</td><td>MyPatchFamily1</td></row>
		<row><td>PatchSequence</td><td>1.0.0</td></row>
		<row><td>SaveAsSchema</td><td/></row>
		<row><td>SccEnabled</td><td>0</td></row>
		<row><td>SccPath</td><td/></row>
		<row><td>SchemaVersion</td><td>776</td></row>
		<row><td>Type</td><td>MSIE</td></row>
	</table>

	<table name="InstallUISequence">
		<col key="yes" def="s72">Action</col>
		<col def="S255">Condition</col>
		<col def="I2">Sequence</col>
		<col def="S255">ISComments</col>
		<col def="I4">ISAttributes</col>
		<row><td>AppSearch</td><td/><td>400</td><td>AppSearch</td><td/></row>
		<row><td>CCPSearch</td><td>CCP_TEST</td><td>500</td><td>CCPSearch</td><td/></row>
		<row><td>CostFinalize</td><td/><td>1000</td><td>CostFinalize</td><td/></row>
		<row><td>CostInitialize</td><td/><td>800</td><td>CostInitialize</td><td/></row>
		<row><td>ExecuteAction</td><td/><td>1300</td><td>ExecuteAction</td><td/></row>
		<row><td>FileCost</td><td/><td>900</td><td>FileCost</td><td/></row>
		<row><td>FindRelatedProducts</td><td/><td>430</td><td>FindRelatedProducts</td><td/></row>
		<row><td>ISPreventDowngrade</td><td>ISFOUNDNEWERPRODUCTVERSION</td><td>450</td><td>ISPreventDowngrade</td><td/></row>
		<row><td>InstallWelcome</td><td>Not UITEST And Not Installed</td><td>1110</td><td/><td/></row>
		<row><td>IsolateComponents</td><td/><td>950</td><td>IsolateComponents</td><td/></row>
		<row><td>LaunchConditions</td><td>Not Installed</td><td>410</td><td>LaunchConditions</td><td/></row>
		<row><td>MaintenanceWelcome</td><td>Installed And Not RESUME And Not Preselected And Not PATCH</td><td>1230</td><td>MaintenanceWelcome</td><td/></row>
		<row><td>MigrateFeatureStates</td><td/><td>1200</td><td>MigrateFeatureStates</td><td/></row>
		<row><td>PatchWelcome</td><td>Installed And PATCH And Not IS_MAJOR_UPGRADE</td><td>1205</td><td>Patch Panel</td><td/></row>
		<row><td>RMCCPSearch</td><td>Not CCP_SUCCESS And CCP_TEST</td><td>600</td><td>RMCCPSearch</td><td/></row>
		<row><td>ResolveSource</td><td>Not Installed</td><td>990</td><td>ResolveSource</td><td/></row>
		<row><td>SetAllUsersProfileNT</td><td>VersionNT = 400</td><td>970</td><td/><td/></row>
		<row><td>SetupCompleteError</td><td/><td>-3</td><td>SetupCompleteError</td><td/></row>
		<row><td>SetupCompleteSuccess</td><td/><td>-1</td><td>SetupCompleteSuccess</td><td/></row>
		<row><td>SetupInitialization</td><td/><td>420</td><td>SetupInitialization</td><td/></row>
		<row><td>SetupInterrupted</td><td/><td>-2</td><td>SetupInterrupted</td><td/></row>
		<row><td>SetupProgress</td><td/><td>1240</td><td>SetupProgress</td><td/></row>
		<row><td>SetupResume</td><td>Installed And (RESUME Or Preselected) And Not PATCH</td><td>1220</td><td>SetupResume</td><td/></row>
		<row><td>ValidateProductID</td><td/><td>700</td><td>ValidateProductID</td><td/></row>
		<row><td>setAllUsersProfile2K</td><td>VersionNT &gt;= 500</td><td>980</td><td/><td/></row>
		<row><td>setUserProfileNT</td><td>VersionNT</td><td>960</td><td/><td/></row>
	</table>

	<table name="IsolatedComponent">
		<col key="yes" def="s72">Component_Shared</col>
		<col key="yes" def="s72">Component_Application</col>
	</table>

	<table name="LaunchCondition">
		<col key="yes" def="s255">Condition</col>
		<col def="l255">Description</col>
		<row><td>Intel &gt;= 5</td><td>##IDPROP_EXPRESS_LAUNCH_CONDITION_PROCESSOR##</td></row>
	</table>

	<table name="ListBox">
		<col key="yes" def="s72">Property</col>
		<col key="yes" def="i2">Order</col>
		<col def="s64">Value</col>
		<col def="L64">Text</col>
	</table>

	<table name="ListView">
		<col key="yes" def="s72">Property</col>
		<col key="yes" def="i2">Order</col>
		<col def="s64">Value</col>
		<col def="L64">Text</col>
		<col def="S72">Binary_</col>
	</table>

	<table name="LockPermissions">
		<col key="yes" def="s72">LockObject</col>
		<col key="yes" def="s32">Table</col>
		<col key="yes" def="S255">Domain</col>
		<col key="yes" def="s255">User</col>
		<col def="I4">Permission</col>
	</table>

	<table name="MIME">
		<col key="yes" def="s64">ContentType</col>
		<col def="s255">Extension_</col>
		<col def="S38">CLSID</col>
		<row><td>MIME Type1</td><td>NCMJ</td><td/></row>
		<row><td>MIME Type2</td><td>NCM</td><td/></row>
	</table>

	<table name="Media">
		<col key="yes" def="i2">DiskId</col>
		<col def="i2">LastSequence</col>
		<col def="L64">DiskPrompt</col>
		<col def="S255">Cabinet</col>
		<col def="S32">VolumeLabel</col>
		<col def="S32">Source</col>
	</table>

	<table name="MoveFile">
		<col key="yes" def="s72">FileKey</col>
		<col def="s72">Component_</col>
		<col def="L255">SourceName</col>
		<col def="L255">DestName</col>
		<col def="S72">SourceFolder</col>
		<col def="s72">DestFolder</col>
		<col def="i2">Options</col>
	</table>

	<table name="MsiAssembly">
		<col key="yes" def="s72">Component_</col>
		<col def="s38">Feature_</col>
		<col def="S72">File_Manifest</col>
		<col def="S72">File_Application</col>
		<col def="I2">Attributes</col>
	</table>

	<table name="MsiAssemblyName">
		<col key="yes" def="s72">Component_</col>
		<col key="yes" def="s255">Name</col>
		<col def="s255">Value</col>
	</table>

	<table name="MsiDigitalCertificate">
		<col key="yes" def="s72">DigitalCertificate</col>
		<col def="v0">CertData</col>
	</table>

	<table name="MsiDigitalSignature">
		<col key="yes" def="s32">Table</col>
		<col key="yes" def="s72">SignObject</col>
		<col def="s72">DigitalCertificate_</col>
		<col def="V0">Hash</col>
	</table>

	<table name="MsiDriverPackages">
		<col key="yes" def="s72">Component</col>
		<col def="i4">Flags</col>
		<col def="I4">Sequence</col>
		<col def="S0">ReferenceComponents</col>
	</table>

	<table name="MsiEmbeddedChainer">
		<col key="yes" def="s72">MsiEmbeddedChainer</col>
		<col def="S255">Condition</col>
		<col def="S255">CommandLine</col>
		<col def="s72">Source</col>
		<col def="I4">Type</col>
	</table>

	<table name="MsiEmbeddedUI">
		<col key="yes" def="s72">MsiEmbeddedUI</col>
		<col def="s255">FileName</col>
		<col def="i2">Attributes</col>
		<col def="I4">MessageFilter</col>
		<col def="V0">Data</col>
		<col def="S255">ISBuildSourcePath</col>
	</table>

	<table name="MsiFileHash">
		<col key="yes" def="s72">File_</col>
		<col def="i2">Options</col>
		<col def="i4">HashPart1</col>
		<col def="i4">HashPart2</col>
		<col def="i4">HashPart3</col>
		<col def="i4">HashPart4</col>
	</table>

	<table name="MsiLockPermissionsEx">
		<col key="yes" def="s72">MsiLockPermissionsEx</col>
		<col def="s72">LockObject</col>
		<col def="s32">Table</col>
		<col def="s0">SDDLText</col>
		<col def="S255">Condition</col>
	</table>

	<table name="MsiPackageCertificate">
		<col key="yes" def="s72">PackageCertificate</col>
		<col def="s72">DigitalCertificate_</col>
	</table>

	<table name="MsiPatchCertificate">
		<col key="yes" def="s72">PatchCertificate</col>
		<col def="s72">DigitalCertificate_</col>
	</table>

	<table name="MsiPatchMetadata">
		<col key="yes" def="s72">PatchConfiguration_</col>
		<col key="yes" def="S72">Company</col>
		<col key="yes" def="s72">Property</col>
		<col def="S0">Value</col>
	</table>

	<table name="MsiPatchOldAssemblyFile">
		<col key="yes" def="s72">File_</col>
		<col key="yes" def="S72">Assembly_</col>
	</table>

	<table name="MsiPatchOldAssemblyName">
		<col key="yes" def="s72">Assembly</col>
		<col key="yes" def="s255">Name</col>
		<col def="S255">Value</col>
	</table>

	<table name="MsiPatchSequence">
		<col key="yes" def="s72">PatchConfiguration_</col>
		<col key="yes" def="s0">PatchFamily</col>
		<col key="yes" def="S0">Target</col>
		<col def="s0">Sequence</col>
		<col def="i2">Supersede</col>
	</table>

	<table name="MsiServiceConfig">
		<col key="yes" def="s72">MsiServiceConfig</col>
		<col def="s255">Name</col>
		<col def="i2">Event</col>
		<col def="i4">ConfigType</col>
		<col def="S0">Argument</col>
		<col def="s72">Component_</col>
	</table>

	<table name="MsiServiceConfigFailureActions">
		<col key="yes" def="s72">MsiServiceConfigFailureActions</col>
		<col def="s255">Name</col>
		<col def="i2">Event</col>
		<col def="I4">ResetPeriod</col>
		<col def="L255">RebootMessage</col>
		<col def="L255">Command</col>
		<col def="S0">Actions</col>
		<col def="S0">DelayActions</col>
		<col def="s72">Component_</col>
	</table>

	<table name="MsiShortcutProperty">
		<col key="yes" def="s72">MsiShortcutProperty</col>
		<col def="s72">Shortcut_</col>
		<col def="s0">PropertyKey</col>
		<col def="s0">PropVariantValue</col>
	</table>

	<table name="ODBCAttribute">
		<col key="yes" def="s72">Driver_</col>
		<col key="yes" def="s40">Attribute</col>
		<col def="S255">Value</col>
	</table>

	<table name="ODBCDataSource">
		<col key="yes" def="s72">DataSource</col>
		<col def="s72">Component_</col>
		<col def="s255">Description</col>
		<col def="s255">DriverDescription</col>
		<col def="i2">Registration</col>
	</table>

	<table name="ODBCDriver">
		<col key="yes" def="s72">Driver</col>
		<col def="s72">Component_</col>
		<col def="s255">Description</col>
		<col def="s72">File_</col>
		<col def="S72">File_Setup</col>
	</table>

	<table name="ODBCSourceAttribute">
		<col key="yes" def="s72">DataSource_</col>
		<col key="yes" def="s32">Attribute</col>
		<col def="S255">Value</col>
	</table>

	<table name="ODBCTranslator">
		<col key="yes" def="s72">Translator</col>
		<col def="s72">Component_</col>
		<col def="s255">Description</col>
		<col def="s72">File_</col>
		<col def="S72">File_Setup</col>
	</table>

	<table name="Patch">
		<col key="yes" def="s72">File_</col>
		<col key="yes" def="i2">Sequence</col>
		<col def="i4">PatchSize</col>
		<col def="i2">Attributes</col>
		<col def="V0">Header</col>
		<col def="S38">StreamRef_</col>
		<col def="S255">ISBuildSourcePath</col>
	</table>

	<table name="PatchPackage">
		<col key="yes" def="s38">PatchId</col>
		<col def="i2">Media_</col>
	</table>

	<table name="ProgId">
		<col key="yes" def="s255">ProgId</col>
		<col def="S255">ProgId_Parent</col>
		<col def="S38">Class_</col>
		<col def="L255">Description</col>
		<col def="S72">Icon_</col>
		<col def="I2">IconIndex</col>
		<col def="I4">ISAttributes</col>
		<row><td>NCManager</td><td/><td/><td>NCManager project</td><td>_4C5652CC_B110_44C0_ABC2_99BF9E09BD49</td><td>1</td><td/></row>
		<row><td>NCManager job file</td><td/><td/><td>NCManager job file</td><td>_14C8D792_D9F0_4CDE_AB1B_B628D8D90038</td><td>1</td><td/></row>
	</table>

	<table name="Property">
		<col key="yes" def="s72">Property</col>
		<col def="L0">Value</col>
		<col def="S255">ISComments</col>
		<row><td>ALLUSERS</td><td>1</td><td/></row>
		<row><td>ARPCOMMENTS</td><td>##ID_STRING5##</td><td/></row>
		<row><td>ARPCONTACT</td><td>##ID_STRING3##</td><td/></row>
		<row><td>ARPHELPTELEPHONE</td><td>##ID_STRING7##</td><td/></row>
		<row><td>ARPINSTALLLOCATION</td><td/><td/></row>
		<row><td>ARPNOMODIFY</td><td>1</td><td/></row>
		<row><td>ARPPRODUCTICON</td><td>ARPPRODUCTICON.exe</td><td/></row>
		<row><td>ARPSIZE</td><td/><td/></row>
		<row><td>ARPURLINFOABOUT</td><td>##ID_STRING1##</td><td/></row>
		<row><td>AgreeToLicense</td><td>No</td><td/></row>
		<row><td>ApplicationUsers</td><td>AllUsers</td><td/></row>
		<row><td>DIRPROPERTY1</td><td>C:\</td><td/></row>
		<row><td>DWUSINTERVAL</td><td>30</td><td/></row>
		<row><td>DWUSLINK</td><td>CEBCA09FCEBBD0E8AEAC4088D95B978FFEBC978F49FBF7BFCE6CE7880E3C90A8AEBBD7EF29AC</td><td/></row>
		<row><td>DefaultUIFont</td><td>ExpressDefault</td><td/></row>
		<row><td>DialogCaption</td><td>InstallShield for Windows Installer</td><td/></row>
		<row><td>DiskPrompt</td><td>[1]</td><td/></row>
		<row><td>DiskSerial</td><td>1234-5678</td><td/></row>
		<row><td>DisplayNameCustom</td><td>##IDS__DisplayName_Custom##</td><td/></row>
		<row><td>DisplayNameMinimal</td><td>##IDS__DisplayName_Minimal##</td><td/></row>
		<row><td>DisplayNameTypical</td><td>##IDS__DisplayName_Typical##</td><td/></row>
		<row><td>Display_IsBitmapDlg</td><td>1</td><td/></row>
		<row><td>ErrorDialog</td><td>SetupError</td><td/></row>
		<row><td>INSTALLLEVEL</td><td>200</td><td/></row>
		<row><td>ISCHECKFORPRODUCTUPDATES</td><td>1</td><td/></row>
		<row><td>ISENABLEDWUSFINISHDIALOG</td><td/><td/></row>
		<row><td>ISSHOWMSILOG</td><td/><td/></row>
		<row><td>ISVROOT_PORT_NO</td><td>0</td><td/></row>
		<row><td>IS_COMPLUS_PROGRESSTEXT_COST</td><td>##IDS_COMPLUS_PROGRESSTEXT_COST##</td><td/></row>
		<row><td>IS_COMPLUS_PROGRESSTEXT_INSTALL</td><td>##IDS_COMPLUS_PROGRESSTEXT_INSTALL##</td><td/></row>
		<row><td>IS_COMPLUS_PROGRESSTEXT_UNINSTALL</td><td>##IDS_COMPLUS_PROGRESSTEXT_UNINSTALL##</td><td/></row>
		<row><td>IS_PREVENT_DOWNGRADE_EXIT</td><td>##IDS_PREVENT_DOWNGRADE_EXIT##</td><td/></row>
		<row><td>IS_PROGMSG_TEXTFILECHANGS_REPLACE</td><td>##IDS_PROGMSG_TEXTFILECHANGS_REPLACE##</td><td/></row>
		<row><td>IS_PROGMSG_XML_COSTING</td><td>##IDS_PROGMSG_XML_COSTING##</td><td/></row>
		<row><td>IS_PROGMSG_XML_CREATE_FILE</td><td>##IDS_PROGMSG_XML_CREATE_FILE##</td><td/></row>
		<row><td>IS_PROGMSG_XML_FILES</td><td>##IDS_PROGMSG_XML_FILES##</td><td/></row>
		<row><td>IS_PROGMSG_XML_REMOVE_FILE</td><td>##IDS_PROGMSG_XML_REMOVE_FILE##</td><td/></row>
		<row><td>IS_PROGMSG_XML_ROLLBACK_FILES</td><td>##IDS_PROGMSG_XML_ROLLBACK_FILES##</td><td/></row>
		<row><td>IS_PROGMSG_XML_UPDATE_FILE</td><td>##IDS_PROGMSG_XML_UPDATE_FILE##</td><td/></row>
		<row><td>IS_SQLSERVER_AUTHENTICATION</td><td>0</td><td/></row>
		<row><td>IS_SQLSERVER_DATABASE</td><td/><td/></row>
		<row><td>IS_SQLSERVER_PASSWORD</td><td/><td/></row>
		<row><td>IS_SQLSERVER_SERVER</td><td/><td/></row>
		<row><td>IS_SQLSERVER_USERNAME</td><td>sa</td><td/></row>
		<row><td>InstallChoice</td><td>AR</td><td/></row>
		<row><td>LAUNCHPROGRAM</td><td>1</td><td/></row>
		<row><td>LAUNCHREADME</td><td>1</td><td/></row>
		<row><td>MSIFASTINSTALL</td><td>1</td><td/></row>
		<row><td>Manufacturer</td><td>##COMPANY_NAME##</td><td/></row>
		<row><td>PIDKEY</td><td/><td/></row>
		<row><td>PIDTemplate</td><td>12345&lt;###-%%%%%%%&gt;@@@@@</td><td/></row>
		<row><td>PROGMSG_IIS_CREATEAPPPOOL</td><td>##IDS_PROGMSG_IIS_CREATEAPPPOOL##</td><td/></row>
		<row><td>PROGMSG_IIS_CREATEAPPPOOLS</td><td>##IDS_PROGMSG_IIS_CREATEAPPPOOLS##</td><td/></row>
		<row><td>PROGMSG_IIS_CREATEVROOT</td><td>##IDS_PROGMSG_IIS_CREATEVROOT##</td><td/></row>
		<row><td>PROGMSG_IIS_CREATEVROOTS</td><td>##IDS_PROGMSG_IIS_CREATEVROOTS##</td><td/></row>
		<row><td>PROGMSG_IIS_CREATEWEBSERVICEEXTENSION</td><td>##IDS_PROGMSG_IIS_CREATEWEBSERVICEEXTENSION##</td><td/></row>
		<row><td>PROGMSG_IIS_CREATEWEBSERVICEEXTENSIONS</td><td>##IDS_PROGMSG_IIS_CREATEWEBSERVICEEXTENSIONS##</td><td/></row>
		<row><td>PROGMSG_IIS_CREATEWEBSITE</td><td>##IDS_PROGMSG_IIS_CREATEWEBSITE##</td><td/></row>
		<row><td>PROGMSG_IIS_CREATEWEBSITES</td><td>##IDS_PROGMSG_IIS_CREATEWEBSITES##</td><td/></row>
		<row><td>PROGMSG_IIS_EXTRACT</td><td>##IDS_PROGMSG_IIS_EXTRACT##</td><td/></row>
		<row><td>PROGMSG_IIS_EXTRACTDONE</td><td>##IDS_PROGMSG_IIS_EXTRACTDONE##</td><td/></row>
		<row><td>PROGMSG_IIS_EXTRACTDONEz</td><td>##IDS_PROGMSG_IIS_EXTRACTDONE##</td><td/></row>
		<row><td>PROGMSG_IIS_EXTRACTzDONE</td><td>##IDS_PROGMSG_IIS_EXTRACTDONE##</td><td/></row>
		<row><td>PROGMSG_IIS_REMOVEAPPPOOL</td><td>##IDS_PROGMSG_IIS_REMOVEAPPPOOL##</td><td/></row>
		<row><td>PROGMSG_IIS_REMOVEAPPPOOLS</td><td>##IDS_PROGMSG_IIS_REMOVEAPPPOOLS##</td><td/></row>
		<row><td>PROGMSG_IIS_REMOVESITE</td><td>##IDS_PROGMSG_IIS_REMOVESITE##</td><td/></row>
		<row><td>PROGMSG_IIS_REMOVEVROOT</td><td>##IDS_PROGMSG_IIS_REMOVEVROOT##</td><td/></row>
		<row><td>PROGMSG_IIS_REMOVEVROOTS</td><td>##IDS_PROGMSG_IIS_REMOVEVROOTS##</td><td/></row>
		<row><td>PROGMSG_IIS_REMOVEWEBSERVICEEXTENSION</td><td>##IDS_PROGMSG_IIS_REMOVEWEBSERVICEEXTENSION##</td><td/></row>
		<row><td>PROGMSG_IIS_REMOVEWEBSERVICEEXTENSIONS</td><td>##IDS_PROGMSG_IIS_REMOVEWEBSERVICEEXTENSIONS##</td><td/></row>
		<row><td>PROGMSG_IIS_REMOVEWEBSITES</td><td>##IDS_PROGMSG_IIS_REMOVEWEBSITES##</td><td/></row>
		<row><td>PROGMSG_IIS_ROLLBACKAPPPOOLS</td><td>##IDS_PROGMSG_IIS_ROLLBACKAPPPOOLS##</td><td/></row>
		<row><td>PROGMSG_IIS_ROLLBACKVROOTS</td><td>##IDS_PROGMSG_IIS_ROLLBACKVROOTS##</td><td/></row>
		<row><td>PROGMSG_IIS_ROLLBACKWEBSERVICEEXTENSIONS</td><td>##IDS_PROGMSG_IIS_ROLLBACKWEBSERVICEEXTENSIONS##</td><td/></row>
		<row><td>ProductCode</td><td>{13ECD694-F7C2-4802-AA29-BB510B42B406}</td><td/></row>
		<row><td>ProductName</td><td>NCM53</td><td/></row>
		<row><td>ProductVersion</td><td>5.3</td><td/></row>
		<row><td>ProgressType0</td><td>install</td><td/></row>
		<row><td>ProgressType1</td><td>Installing</td><td/></row>
		<row><td>ProgressType2</td><td>installed</td><td/></row>
		<row><td>ProgressType3</td><td>installs</td><td/></row>
		<row><td>RebootYesNo</td><td>Yes</td><td/></row>
		<row><td>ReinstallFileVersion</td><td>o</td><td/></row>
		<row><td>ReinstallModeText</td><td>omus</td><td/></row>
		<row><td>ReinstallRepair</td><td>r</td><td/></row>
		<row><td>RestartManagerOption</td><td>CloseRestart</td><td/></row>
		<row><td>SERIALNUMBER</td><td/><td/></row>
		<row><td>SERIALNUMVALSUCCESSRETVAL</td><td>1</td><td/></row>
		<row><td>SecureCustomProperties</td><td>ISFOUNDNEWERPRODUCTVERSION;USERNAME;COMPANYNAME;ISX_SERIALNUM;SUPPORTDIR</td><td/></row>
		<row><td>SelectedSetupType</td><td>##IDS__DisplayName_Typical##</td><td/></row>
		<row><td>SetupType</td><td>Typical</td><td/></row>
		<row><td>UpgradeCode</td><td>{CAC41D5D-69D7-4FD4-A184-BD41C0A76922}</td><td/></row>
		<row><td>_IsMaintenance</td><td>Change</td><td/></row>
		<row><td>_IsSetupTypeMin</td><td>Typical</td><td/></row>
	</table>

	<table name="PublishComponent">
		<col key="yes" def="s38">ComponentId</col>
		<col key="yes" def="s255">Qualifier</col>
		<col key="yes" def="s72">Component_</col>
		<col def="L0">AppData</col>
		<col def="s38">Feature_</col>
	</table>

	<table name="RadioButton">
		<col key="yes" def="s72">Property</col>
		<col key="yes" def="i2">Order</col>
		<col def="s64">Value</col>
		<col def="i2">X</col>
		<col def="i2">Y</col>
		<col def="i2">Width</col>
		<col def="i2">Height</col>
		<col def="L64">Text</col>
		<col def="L50">Help</col>
		<col def="I4">ISControlId</col>
		<row><td>AgreeToLicense</td><td>1</td><td>No</td><td>0</td><td>15</td><td>291</td><td>15</td><td>##IDS__AgreeToLicense_0##</td><td/><td/></row>
		<row><td>AgreeToLicense</td><td>2</td><td>Yes</td><td>0</td><td>0</td><td>291</td><td>15</td><td>##IDS__AgreeToLicense_1##</td><td/><td/></row>
		<row><td>ApplicationUsers</td><td>1</td><td>AllUsers</td><td>1</td><td>7</td><td>290</td><td>14</td><td>##IDS__IsRegisterUserDlg_Anyone##</td><td/><td/></row>
		<row><td>ApplicationUsers</td><td>2</td><td>OnlyCurrentUser</td><td>1</td><td>23</td><td>290</td><td>14</td><td>##IDS__IsRegisterUserDlg_OnlyMe##</td><td/><td/></row>
		<row><td>RestartManagerOption</td><td>1</td><td>CloseRestart</td><td>6</td><td>9</td><td>331</td><td>14</td><td>##IDS__IsMsiRMFilesInUse_CloseRestart##</td><td/><td/></row>
		<row><td>RestartManagerOption</td><td>2</td><td>Reboot</td><td>6</td><td>21</td><td>331</td><td>14</td><td>##IDS__IsMsiRMFilesInUse_RebootAfter##</td><td/><td/></row>
		<row><td>_IsMaintenance</td><td>1</td><td>Change</td><td>0</td><td>0</td><td>290</td><td>14</td><td>##IDS__IsMaintenanceDlg_Modify##</td><td/><td/></row>
		<row><td>_IsMaintenance</td><td>2</td><td>Reinstall</td><td>0</td><td>60</td><td>290</td><td>14</td><td>##IDS__IsMaintenanceDlg_Repair##</td><td/><td/></row>
		<row><td>_IsMaintenance</td><td>3</td><td>Remove</td><td>0</td><td>120</td><td>290</td><td>14</td><td>##IDS__IsMaintenanceDlg_Remove##</td><td/><td/></row>
		<row><td>_IsSetupTypeMin</td><td>1</td><td>Typical</td><td>1</td><td>6</td><td>264</td><td>14</td><td>##IDS__IsSetupTypeMinDlg_Typical##</td><td/><td/></row>
	</table>

	<table name="RegLocator">
		<col key="yes" def="s72">Signature_</col>
		<col def="i2">Root</col>
		<col def="s255">Key</col>
		<col def="S255">Name</col>
		<col def="I2">Type</col>
	</table>

	<table name="Registry">
		<col key="yes" def="s72">Registry</col>
		<col def="i2">Root</col>
		<col def="s255">Key</col>
		<col def="S255">Name</col>
		<col def="S0">Value</col>
		<col def="s72">Component_</col>
		<col def="I4">ISAttributes</col>
		<row><td>Registry1</td><td>2</td><td>Software\Microsoft\Windows\CurrentVersion\App Paths\NCM40.EXE</td><td>DataPath</td><td>[DATABASEDIR]</td><td>C__2A0D3AF6F7264C9A8D50E89172C4A81E</td><td>0</td></row>
		<row><td>Registry2</td><td>2</td><td>Software\Microsoft\Windows\CurrentVersion\App Paths\NCM40.EXE</td><td>Password</td><td/><td>C__2A0D3AF6F7264C9A8D50E89172C4A81E</td><td>0</td></row>
		<row><td>Registry3</td><td>1</td><td>Software\Inteks\NCManager\[ProductVersion]</td><td>DataPath</td><td>[DATABASEDIR]</td><td>C__08B0E42E0AE7406A98786249EDB2B56E</td><td>0</td></row>
		<row><td>Registry4</td><td>1</td><td>Software\Inteks\NCManager</td><td>InterfaceStyle</td><td>Ribbon</td><td>C__D47657BBBCC549C9A4DA838A1DCD2DF4</td><td>0</td></row>
		<row><td>Registry45</td><td>2</td><td>Software\Microsoft\Windows\CurrentVersion\App Paths\NCM40.EXE</td><td>Path</td><td>[INSTALLDIR]</td><td>C__2A0D3AF6F7264C9A8D50E89172C4A81E</td><td>0</td></row>
		<row><td>Registry5</td><td>1</td><td>Software\Inteks\NCManager\[ProductVersion]</td><td>ShowCutHit</td><td>#1</td><td>C__08B0E42E0AE7406A98786249EDB2B56E</td><td>0</td></row>
		<row><td>_08B0E42E0AE7406A98786249EDB2B56E</td><td>1</td><td>Software\Inteks\NCManager\[ProductVersion]</td><td>MainPath</td><td>[INSTALLDIR]</td><td>C__08B0E42E0AE7406A98786249EDB2B56E</td><td/></row>
		<row><td>_D47657BBBCC549C9A4DA838A1DCD2DF4</td><td>1</td><td>Software\Inteks\NCManager</td><td>LastVersion</td><td>[ProductVersion]</td><td>C__D47657BBBCC549C9A4DA838A1DCD2DF4</td><td/></row>
	</table>

	<table name="RemoveFile">
		<col key="yes" def="s72">FileKey</col>
		<col def="s72">Component_</col>
		<col def="L255">FileName</col>
		<col def="s72">DirProperty</col>
		<col def="i2">InstallMode</col>
	</table>

	<table name="RemoveIniFile">
		<col key="yes" def="s72">RemoveIniFile</col>
		<col def="l255">FileName</col>
		<col def="S72">DirProperty</col>
		<col def="l96">Section</col>
		<col def="l128">Key</col>
		<col def="L255">Value</col>
		<col def="i2">Action</col>
		<col def="s72">Component_</col>
	</table>

	<table name="RemoveRegistry">
		<col key="yes" def="s72">RemoveRegistry</col>
		<col def="i2">Root</col>
		<col def="l255">Key</col>
		<col def="L255">Name</col>
		<col def="s72">Component_</col>
	</table>

	<table name="ReserveCost">
		<col key="yes" def="s72">ReserveKey</col>
		<col def="s72">Component_</col>
		<col def="S72">ReserveFolder</col>
		<col def="i4">ReserveLocal</col>
		<col def="i4">ReserveSource</col>
	</table>

	<table name="SFPCatalog">
		<col key="yes" def="s255">SFPCatalog</col>
		<col def="V0">Catalog</col>
		<col def="S0">Dependency</col>
	</table>

	<table name="SelfReg">
		<col key="yes" def="s72">File_</col>
		<col def="I2">Cost</col>
	</table>

	<table name="ServiceControl">
		<col key="yes" def="s72">ServiceControl</col>
		<col def="s255">Name</col>
		<col def="i2">Event</col>
		<col def="S255">Arguments</col>
		<col def="I2">Wait</col>
		<col def="s72">Component_</col>
	</table>

	<table name="ServiceInstall">
		<col key="yes" def="s72">ServiceInstall</col>
		<col def="s255">Name</col>
		<col def="L255">DisplayName</col>
		<col def="i4">ServiceType</col>
		<col def="i4">StartType</col>
		<col def="i4">ErrorControl</col>
		<col def="S255">LoadOrderGroup</col>
		<col def="S255">Dependencies</col>
		<col def="S255">StartName</col>
		<col def="S255">Password</col>
		<col def="S255">Arguments</col>
		<col def="s72">Component_</col>
		<col def="L255">Description</col>
	</table>

	<table name="Shortcut">
		<col key="yes" def="s72">Shortcut</col>
		<col def="s72">Directory_</col>
		<col def="l128">Name</col>
		<col def="s72">Component_</col>
		<col def="s255">Target</col>
		<col def="S255">Arguments</col>
		<col def="L255">Description</col>
		<col def="I2">Hotkey</col>
		<col def="S72">Icon_</col>
		<col def="I2">IconIndex</col>
		<col def="I2">ShowCmd</col>
		<col def="S72">WkDir</col>
		<col def="S255">DisplayResourceDLL</col>
		<col def="I2">DisplayResourceId</col>
		<col def="S255">DescriptionResourceDLL</col>
		<col def="I2">DescriptionResourceId</col>
		<col def="S255">ISComments</col>
		<col def="S255">ISShortcutName</col>
		<col def="I4">ISAttributes</col>
		<row><td>_1C7A9C5E1DEC47A6B12ECBCB3E1CB054</td><td>ProgramMenuFolder</td><td>##IDS_SHORTCUT_DISPLAY_NAME1##</td><td>C__5AB0F74B62D24B2797103046CCAAD98E</td><td>AlwaysInstall</td><td/><td/><td/><td>_1C7A9C5E1DEC47A6B_2EC30B151BEA417BA8507245305AA8CB.exe</td><td>1</td><td>1</td><td>INSTALLDIR</td><td/><td/><td/><td/><td/><td/><td/></row>
	</table>

	<table name="Signature">
		<col key="yes" def="s72">Signature</col>
		<col def="s255">FileName</col>
		<col def="S20">MinVersion</col>
		<col def="S20">MaxVersion</col>
		<col def="I4">MinSize</col>
		<col def="I4">MaxSize</col>
		<col def="I4">MinDate</col>
		<col def="I4">MaxDate</col>
		<col def="S255">Languages</col>
	</table>

	<table name="TextStyle">
		<col key="yes" def="s72">TextStyle</col>
		<col def="s32">FaceName</col>
		<col def="i2">Size</col>
		<col def="I4">Color</col>
		<col def="I2">StyleBits</col>
		<row><td>Arial8</td><td>Arial</td><td>8</td><td/><td/></row>
		<row><td>Arial9</td><td>Arial</td><td>9</td><td/><td/></row>
		<row><td>ArialBlue10</td><td>Arial</td><td>10</td><td>16711680</td><td/></row>
		<row><td>ArialBlueStrike10</td><td>Arial</td><td>10</td><td>16711680</td><td>8</td></row>
		<row><td>CourierNew8</td><td>Courier New</td><td>8</td><td/><td/></row>
		<row><td>CourierNew9</td><td>Courier New</td><td>9</td><td/><td/></row>
		<row><td>ExpressDefault</td><td>Tahoma</td><td>8</td><td/><td/></row>
		<row><td>MSGothic9</td><td>MS Gothic</td><td>9</td><td/><td/></row>
		<row><td>MSSGreySerif8</td><td>MS Sans Serif</td><td>8</td><td>8421504</td><td/></row>
		<row><td>MSSWhiteSerif8</td><td>Tahoma</td><td>8</td><td>16777215</td><td/></row>
		<row><td>MSSansBold8</td><td>Tahoma</td><td>8</td><td/><td>1</td></row>
		<row><td>MSSansSerif8</td><td>MS Sans Serif</td><td>8</td><td/><td/></row>
		<row><td>MSSansSerif9</td><td>MS Sans Serif</td><td>9</td><td/><td/></row>
		<row><td>Tahoma10</td><td>Tahoma</td><td>10</td><td/><td/></row>
		<row><td>Tahoma8</td><td>Tahoma</td><td>8</td><td/><td/></row>
		<row><td>Tahoma9</td><td>Tahoma</td><td>9</td><td/><td/></row>
		<row><td>TahomaBold10</td><td>Tahoma</td><td>10</td><td/><td>1</td></row>
		<row><td>TahomaBold8</td><td>Tahoma</td><td>8</td><td/><td>1</td></row>
		<row><td>Times8</td><td>Times New Roman</td><td>8</td><td/><td/></row>
		<row><td>Times9</td><td>Times New Roman</td><td>9</td><td/><td/></row>
		<row><td>TimesItalic12</td><td>Times New Roman</td><td>12</td><td/><td>2</td></row>
		<row><td>TimesItalicBlue10</td><td>Times New Roman</td><td>10</td><td>16711680</td><td>2</td></row>
		<row><td>TimesRed16</td><td>Times New Roman</td><td>16</td><td>255</td><td/></row>
		<row><td>VerdanaBold14</td><td>Verdana</td><td>13</td><td/><td>1</td></row>
	</table>

	<table name="TypeLib">
		<col key="yes" def="s38">LibID</col>
		<col key="yes" def="i2">Language</col>
		<col key="yes" def="s72">Component_</col>
		<col def="I4">Version</col>
		<col def="L128">Description</col>
		<col def="S72">Directory_</col>
		<col def="s38">Feature_</col>
		<col def="I4">Cost</col>
	</table>

	<table name="UIText">
		<col key="yes" def="s72">Key</col>
		<col def="L255">Text</col>
		<row><td>AbsentPath</td><td/></row>
		<row><td>GB</td><td>##IDS_UITEXT_GB##</td></row>
		<row><td>KB</td><td>##IDS_UITEXT_KB##</td></row>
		<row><td>MB</td><td>##IDS_UITEXT_MB##</td></row>
		<row><td>MenuAbsent</td><td>##IDS_UITEXT_FeatureNotAvailable##</td></row>
		<row><td>MenuAdvertise</td><td>##IDS_UITEXT_FeatureInstalledWhenRequired2##</td></row>
		<row><td>MenuAllCD</td><td>##IDS_UITEXT_FeatureInstalledCD##</td></row>
		<row><td>MenuAllLocal</td><td>##IDS_UITEXT_FeatureInstalledLocal##</td></row>
		<row><td>MenuAllNetwork</td><td>##IDS_UITEXT_FeatureInstalledNetwork##</td></row>
		<row><td>MenuCD</td><td>##IDS_UITEXT_FeatureInstalledCD2##</td></row>
		<row><td>MenuLocal</td><td>##IDS_UITEXT_FeatureInstalledLocal2##</td></row>
		<row><td>MenuNetwork</td><td>##IDS_UITEXT_FeatureInstalledNetwork2##</td></row>
		<row><td>NewFolder</td><td>##IDS_UITEXT_Folder##</td></row>
		<row><td>SelAbsentAbsent</td><td>##IDS_UITEXT_GB##</td></row>
		<row><td>SelAbsentAdvertise</td><td>##IDS_UITEXT_FeatureInstalledWhenRequired##</td></row>
		<row><td>SelAbsentCD</td><td>##IDS_UITEXT_FeatureOnCD##</td></row>
		<row><td>SelAbsentLocal</td><td>##IDS_UITEXT_FeatureLocal##</td></row>
		<row><td>SelAbsentNetwork</td><td>##IDS_UITEXT_FeatureNetwork##</td></row>
		<row><td>SelAdvertiseAbsent</td><td>##IDS_UITEXT_FeatureUnavailable##</td></row>
		<row><td>SelAdvertiseAdvertise</td><td>##IDS_UITEXT_FeatureInstalledRequired##</td></row>
		<row><td>SelAdvertiseCD</td><td>##IDS_UITEXT_FeatureOnCD2##</td></row>
		<row><td>SelAdvertiseLocal</td><td>##IDS_UITEXT_FeatureLocal2##</td></row>
		<row><td>SelAdvertiseNetwork</td><td>##IDS_UITEXT_FeatureNetwork2##</td></row>
		<row><td>SelCDAbsent</td><td>##IDS_UITEXT_FeatureWillBeUninstalled##</td></row>
		<row><td>SelCDAdvertise</td><td>##IDS_UITEXT_FeatureWasCD##</td></row>
		<row><td>SelCDCD</td><td>##IDS_UITEXT_FeatureRunFromCD##</td></row>
		<row><td>SelCDLocal</td><td>##IDS_UITEXT_FeatureWasCDLocal##</td></row>
		<row><td>SelChildCostNeg</td><td>##IDS_UITEXT_FeatureFreeSpace##</td></row>
		<row><td>SelChildCostPos</td><td>##IDS_UITEXT_FeatureRequiredSpace##</td></row>
		<row><td>SelCostPending</td><td>##IDS_UITEXT_CompilingFeaturesCost##</td></row>
		<row><td>SelLocalAbsent</td><td>##IDS_UITEXT_FeatureCompletelyRemoved##</td></row>
		<row><td>SelLocalAdvertise</td><td>##IDS_UITEXT_FeatureRemovedUnlessRequired##</td></row>
		<row><td>SelLocalCD</td><td>##IDS_UITEXT_FeatureRemovedCD##</td></row>
		<row><td>SelLocalLocal</td><td>##IDS_UITEXT_FeatureRemainLocal##</td></row>
		<row><td>SelLocalNetwork</td><td>##IDS_UITEXT_FeatureRemoveNetwork##</td></row>
		<row><td>SelNetworkAbsent</td><td>##IDS_UITEXT_FeatureUninstallNoNetwork##</td></row>
		<row><td>SelNetworkAdvertise</td><td>##IDS_UITEXT_FeatureWasOnNetworkInstalled##</td></row>
		<row><td>SelNetworkLocal</td><td>##IDS_UITEXT_FeatureWasOnNetworkLocal##</td></row>
		<row><td>SelNetworkNetwork</td><td>##IDS_UITEXT_FeatureContinueNetwork##</td></row>
		<row><td>SelParentCostNegNeg</td><td>##IDS_UITEXT_FeatureSpaceFree##</td></row>
		<row><td>SelParentCostNegPos</td><td>##IDS_UITEXT_FeatureSpaceFree2##</td></row>
		<row><td>SelParentCostPosNeg</td><td>##IDS_UITEXT_FeatureSpaceFree3##</td></row>
		<row><td>SelParentCostPosPos</td><td>##IDS_UITEXT_FeatureSpaceFree4##</td></row>
		<row><td>TimeRemaining</td><td>##IDS_UITEXT_TimeRemaining##</td></row>
		<row><td>VolumeCostAvailable</td><td>##IDS_UITEXT_Available##</td></row>
		<row><td>VolumeCostDifference</td><td>##IDS_UITEXT_Differences##</td></row>
		<row><td>VolumeCostRequired</td><td>##IDS_UITEXT_Required##</td></row>
		<row><td>VolumeCostSize</td><td>##IDS_UITEXT_DiskSize##</td></row>
		<row><td>VolumeCostVolume</td><td>##IDS_UITEXT_Volume##</td></row>
		<row><td>bytes</td><td>##IDS_UITEXT_Bytes##</td></row>
	</table>

	<table name="Upgrade">
		<col key="yes" def="s38">UpgradeCode</col>
		<col key="yes" def="S20">VersionMin</col>
		<col key="yes" def="S20">VersionMax</col>
		<col key="yes" def="S255">Language</col>
		<col key="yes" def="i4">Attributes</col>
		<col def="S255">Remove</col>
		<col def="s72">ActionProperty</col>
		<col def="S72">ISDisplayName</col>
		<row><td>{00000000-0000-0000-0000-000000000000}</td><td>***ALL_VERSIONS***</td><td></td><td></td><td>2</td><td/><td>ISFOUNDNEWERPRODUCTVERSION</td><td>ISPreventDowngrade</td></row>
	</table>

	<table name="Verb">
		<col key="yes" def="s255">Extension_</col>
		<col key="yes" def="s32">Verb</col>
		<col def="I2">Sequence</col>
		<col def="L255">Command</col>
		<col def="L255">Argument</col>
		<row><td>NCM</td><td>open</td><td>0</td><td>&amp;Open</td><td>"%1"</td></row>
		<row><td>NCMJ</td><td>open</td><td>0</td><td>&amp;Open</td><td>"%1"</td></row>
	</table>

	<table name="_Validation">
		<col key="yes" def="s32">Table</col>
		<col key="yes" def="s32">Column</col>
		<col def="s4">Nullable</col>
		<col def="I4">MinValue</col>
		<col def="I4">MaxValue</col>
		<col def="S255">KeyTable</col>
		<col def="I2">KeyColumn</col>
		<col def="S32">Category</col>
		<col def="S255">Set</col>
		<col def="S255">Description</col>
		<row><td>ActionText</td><td>Action</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of action to be described.</td></row>
		<row><td>ActionText</td><td>Description</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Localized description displayed in progress dialog and log when action is executing.</td></row>
		<row><td>ActionText</td><td>Template</td><td>Y</td><td/><td/><td/><td/><td>Template</td><td/><td>Optional localized format template used to format action data records for display during action execution.</td></row>
		<row><td>AdminExecuteSequence</td><td>Action</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of action to invoke, either in the engine or the handler DLL.</td></row>
		<row><td>AdminExecuteSequence</td><td>Condition</td><td>Y</td><td/><td/><td/><td/><td>Condition</td><td/><td>Optional expression which skips the action if evaluates to expFalse.If the expression syntax is invalid, the engine will terminate, returning iesBadActionData.</td></row>
		<row><td>AdminExecuteSequence</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>This is used to store MM Custom Action Types</td></row>
		<row><td>AdminExecuteSequence</td><td>ISComments</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Author’s comments on this Sequence.</td></row>
		<row><td>AdminExecuteSequence</td><td>Sequence</td><td>Y</td><td>-4</td><td>32767</td><td/><td/><td/><td/><td>Number that determines the sort order in which the actions are to be executed.  Leave blank to suppress action.</td></row>
		<row><td>AdminUISequence</td><td>Action</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of action to invoke, either in the engine or the handler DLL.</td></row>
		<row><td>AdminUISequence</td><td>Condition</td><td>Y</td><td/><td/><td/><td/><td>Condition</td><td/><td>Optional expression which skips the action if evaluates to expFalse.If the expression syntax is invalid, the engine will terminate, returning iesBadActionData.</td></row>
		<row><td>AdminUISequence</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>This is used to store MM Custom Action Types</td></row>
		<row><td>AdminUISequence</td><td>ISComments</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Author’s comments on this Sequence.</td></row>
		<row><td>AdminUISequence</td><td>Sequence</td><td>Y</td><td>-4</td><td>32767</td><td/><td/><td/><td/><td>Number that determines the sort order in which the actions are to be executed.  Leave blank to suppress action.</td></row>
		<row><td>AdvtExecuteSequence</td><td>Action</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of action to invoke, either in the engine or the handler DLL.</td></row>
		<row><td>AdvtExecuteSequence</td><td>Condition</td><td>Y</td><td/><td/><td/><td/><td>Condition</td><td/><td>Optional expression which skips the action if evaluates to expFalse.If the expression syntax is invalid, the engine will terminate, returning iesBadActionData.</td></row>
		<row><td>AdvtExecuteSequence</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>This is used to store MM Custom Action Types</td></row>
		<row><td>AdvtExecuteSequence</td><td>ISComments</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Author’s comments on this Sequence.</td></row>
		<row><td>AdvtExecuteSequence</td><td>Sequence</td><td>Y</td><td>-4</td><td>32767</td><td/><td/><td/><td/><td>Number that determines the sort order in which the actions are to be executed.  Leave blank to suppress action.</td></row>
		<row><td>AdvtUISequence</td><td>Action</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of action to invoke, either in the engine or the handler DLL.</td></row>
		<row><td>AdvtUISequence</td><td>Condition</td><td>Y</td><td/><td/><td/><td/><td>Condition</td><td/><td>Optional expression which skips the action if evaluates to expFalse.If the expression syntax is invalid, the engine will terminate, returning iesBadActionData.</td></row>
		<row><td>AdvtUISequence</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>This is used to store MM Custom Action Types</td></row>
		<row><td>AdvtUISequence</td><td>ISComments</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Author’s comments on this Sequence.</td></row>
		<row><td>AdvtUISequence</td><td>Sequence</td><td>Y</td><td>-4</td><td>32767</td><td/><td/><td/><td/><td>Number that determines the sort order in which the actions are to be executed.  Leave blank to suppress action.</td></row>
		<row><td>AppId</td><td>ActivateAtStorage</td><td>Y</td><td>0</td><td>1</td><td/><td/><td/><td/><td/></row>
		<row><td>AppId</td><td>AppId</td><td>N</td><td/><td/><td/><td/><td>Guid</td><td/><td/></row>
		<row><td>AppId</td><td>DllSurrogate</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td/></row>
		<row><td>AppId</td><td>LocalService</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td/></row>
		<row><td>AppId</td><td>RemoteServerName</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td/></row>
		<row><td>AppId</td><td>RunAsInteractiveUser</td><td>Y</td><td>0</td><td>1</td><td/><td/><td/><td/><td/></row>
		<row><td>AppId</td><td>ServiceParameters</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td/></row>
		<row><td>AppSearch</td><td>Property</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>The property associated with a Signature</td></row>
		<row><td>AppSearch</td><td>Signature_</td><td>N</td><td/><td/><td>ISXmlLocator;Signature</td><td>1</td><td>Identifier</td><td/><td>The Signature_ represents a unique file signature and is also the foreign key in the Signature,  RegLocator, IniLocator, CompLocator and the DrLocator tables.</td></row>
		<row><td>BBControl</td><td>Attributes</td><td>Y</td><td>0</td><td>2147483647</td><td/><td/><td/><td/><td>A 32-bit word that specifies the attribute flags to be applied to this control.</td></row>
		<row><td>BBControl</td><td>BBControl</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of the control. This name must be unique within a billboard, but can repeat on different billboard.</td></row>
		<row><td>BBControl</td><td>Billboard_</td><td>N</td><td/><td/><td>Billboard</td><td>1</td><td>Identifier</td><td/><td>External key to the Billboard table, name of the billboard.</td></row>
		<row><td>BBControl</td><td>Height</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Height of the bounding rectangle of the control.</td></row>
		<row><td>BBControl</td><td>Text</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>A string used to set the initial text contained within a control (if appropriate).</td></row>
		<row><td>BBControl</td><td>Type</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>The type of the control.</td></row>
		<row><td>BBControl</td><td>Width</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Width of the bounding rectangle of the control.</td></row>
		<row><td>BBControl</td><td>X</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Horizontal coordinate of the upper left corner of the bounding rectangle of the control.</td></row>
		<row><td>BBControl</td><td>Y</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Vertical coordinate of the upper left corner of the bounding rectangle of the control.</td></row>
		<row><td>Billboard</td><td>Action</td><td>Y</td><td/><td/><td/><td/><td>Identifier</td><td/><td>The name of an action. The billboard is displayed during the progress messages received from this action.</td></row>
		<row><td>Billboard</td><td>Billboard</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of the billboard.</td></row>
		<row><td>Billboard</td><td>Feature_</td><td>N</td><td/><td/><td>Feature</td><td>1</td><td>Identifier</td><td/><td>An external key to the Feature Table. The billboard is shown only if this feature is being installed.</td></row>
		<row><td>Billboard</td><td>Ordering</td><td>Y</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>A positive integer. If there is more than one billboard corresponding to an action they will be shown in the order defined by this column.</td></row>
		<row><td>Binary</td><td>Data</td><td>Y</td><td/><td/><td/><td/><td>Binary</td><td/><td>Binary stream. The binary icon data in PE (.DLL or .EXE) or icon (.ICO) format.</td></row>
		<row><td>Binary</td><td>ISBuildSourcePath</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Full path to the ICO or EXE file.</td></row>
		<row><td>Binary</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Unique key identifying the binary data.</td></row>
		<row><td>BindImage</td><td>File_</td><td>N</td><td/><td/><td>File</td><td>1</td><td>Identifier</td><td/><td>The index into the File table. This must be an executable file.</td></row>
		<row><td>BindImage</td><td>Path</td><td>Y</td><td/><td/><td/><td/><td>Paths</td><td/><td>A list of ;  delimited paths that represent the paths to be searched for the import DLLS. The list is usually a list of properties each enclosed within square brackets [] .</td></row>
		<row><td>CCPSearch</td><td>Signature_</td><td>N</td><td/><td/><td>Signature</td><td>1</td><td>Identifier</td><td/><td>The Signature_ represents a unique file signature and is also the foreign key in the Signature,  RegLocator, IniLocator, CompLocator and the DrLocator tables.</td></row>
		<row><td>CheckBox</td><td>Property</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>A named property to be tied to the item.</td></row>
		<row><td>CheckBox</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>The value string associated with the item.</td></row>
		<row><td>Class</td><td>AppId_</td><td>Y</td><td/><td/><td>AppId</td><td>1</td><td>Guid</td><td/><td>Optional AppID containing DCOM information for associated application (string GUID).</td></row>
		<row><td>Class</td><td>Argument</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>optional argument for LocalServers.</td></row>
		<row><td>Class</td><td>Attributes</td><td>Y</td><td/><td>32767</td><td/><td/><td/><td/><td>Class registration attributes.</td></row>
		<row><td>Class</td><td>CLSID</td><td>N</td><td/><td/><td/><td/><td>Guid</td><td/><td>The CLSID of an OLE factory.</td></row>
		<row><td>Class</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Required foreign key into the Component Table, specifying the component for which to return a path when called through LocateComponent.</td></row>
		<row><td>Class</td><td>Context</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>The numeric server context for this server. CLSCTX_xxxx</td></row>
		<row><td>Class</td><td>DefInprocHandler</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td>1;2;3</td><td>Optional default inproc handler.  Only optionally provided if Context=CLSCTX_LOCAL_SERVER.  Typically "ole32.dll" or "mapi32.dll"</td></row>
		<row><td>Class</td><td>Description</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Localized description for the Class.</td></row>
		<row><td>Class</td><td>Feature_</td><td>N</td><td/><td/><td>Feature</td><td>1</td><td>Identifier</td><td/><td>Required foreign key into the Feature Table, specifying the feature to validate or install in order for the CLSID factory to be operational.</td></row>
		<row><td>Class</td><td>FileTypeMask</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Optional string containing information for the HKCRthis CLSID) key. If multiple patterns exist, they must be delimited by a semicolon, and numeric subkeys will be generated: 0,1,2...</td></row>
		<row><td>Class</td><td>IconIndex</td><td>Y</td><td>-32767</td><td>32767</td><td/><td/><td/><td/><td>Optional icon index.</td></row>
		<row><td>Class</td><td>Icon_</td><td>Y</td><td/><td/><td>Icon</td><td>1</td><td>Identifier</td><td/><td>Optional foreign key into the Icon Table, specifying the icon file associated with this CLSID. Will be written under the DefaultIcon key.</td></row>
		<row><td>Class</td><td>ProgId_Default</td><td>Y</td><td/><td/><td>ProgId</td><td>1</td><td>Text</td><td/><td>Optional ProgId associated with this CLSID.</td></row>
		<row><td>ComboBox</td><td>Order</td><td>N</td><td>1</td><td>32767</td><td/><td/><td/><td/><td>A positive integer used to determine the ordering of the items within one list.	The integers do not have to be consecutive.</td></row>
		<row><td>ComboBox</td><td>Property</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>A named property to be tied to this item. All the items tied to the same property become part of the same combobox.</td></row>
		<row><td>ComboBox</td><td>Text</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>The visible text to be assigned to the item. Optional. If this entry or the entire column is missing, the text is the same as the value.</td></row>
		<row><td>ComboBox</td><td>Value</td><td>N</td><td/><td/><td/><td/><td>Formatted</td><td/><td>The value string associated with this item. Selecting the line will set the associated property to this value.</td></row>
		<row><td>CompLocator</td><td>ComponentId</td><td>N</td><td/><td/><td/><td/><td>Guid</td><td/><td>A string GUID unique to this component, version, and language.</td></row>
		<row><td>CompLocator</td><td>Signature_</td><td>N</td><td/><td/><td>Signature</td><td>1</td><td>Identifier</td><td/><td>The table key. The Signature_ represents a unique file signature and is also the foreign key in the Signature table.</td></row>
		<row><td>CompLocator</td><td>Type</td><td>Y</td><td>0</td><td>1</td><td/><td/><td/><td/><td>A boolean value that determines if the registry value is a filename or a directory location.</td></row>
		<row><td>Complus</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key referencing Component that controls the ComPlus component.</td></row>
		<row><td>Complus</td><td>ExpType</td><td>Y</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>ComPlus component attributes.</td></row>
		<row><td>Component</td><td>Attributes</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Remote execution option, one of irsEnum</td></row>
		<row><td>Component</td><td>Component</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key used to identify a particular component record.</td></row>
		<row><td>Component</td><td>ComponentId</td><td>Y</td><td/><td/><td/><td/><td>Guid</td><td/><td>A string GUID unique to this component, version, and language.</td></row>
		<row><td>Component</td><td>Condition</td><td>Y</td><td/><td/><td/><td/><td>Condition</td><td/><td>A conditional statement that will disable this component if the specified condition evaluates to the 'True' state. If a component is disabled, it will not be installed, regardless of the 'Action' state associated with the component.</td></row>
		<row><td>Component</td><td>Directory_</td><td>N</td><td/><td/><td>Directory</td><td>1</td><td>Identifier</td><td/><td>Required key of a Directory table record. This is actually a property name whose value contains the actual path, set either by the AppSearch action or with the default setting obtained from the Directory table.</td></row>
		<row><td>Component</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>This is used to store Installshield custom properties of a component.</td></row>
		<row><td>Component</td><td>ISComments</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>User Comments.</td></row>
		<row><td>Component</td><td>ISDotNetInstallerArgsCommit</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Arguments passed to the key file of the component if if implements the .NET Installer class</td></row>
		<row><td>Component</td><td>ISDotNetInstallerArgsInstall</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Arguments passed to the key file of the component if if implements the .NET Installer class</td></row>
		<row><td>Component</td><td>ISDotNetInstallerArgsRollback</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Arguments passed to the key file of the component if if implements the .NET Installer class</td></row>
		<row><td>Component</td><td>ISDotNetInstallerArgsUninstall</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Arguments passed to the key file of the component if if implements the .NET Installer class</td></row>
		<row><td>Component</td><td>ISRegFileToMergeAtBuild</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Path and File name of a .REG file to merge into the component at build time.</td></row>
		<row><td>Component</td><td>ISScanAtBuildFile</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>File used by the Dot Net scanner to populate dependant assemblies' File_Application field.</td></row>
		<row><td>Component</td><td>KeyPath</td><td>Y</td><td/><td/><td>File;ODBCDataSource;Registry</td><td>1</td><td>Identifier</td><td/><td>Either the primary key into the File table, Registry table, or ODBCDataSource table. This extract path is stored when the component is installed, and is used to detect the presence of the component and to return the path to it.</td></row>
		<row><td>Condition</td><td>Condition</td><td>Y</td><td/><td/><td/><td/><td>Condition</td><td/><td>Expression evaluated to determine if Level in the Feature table is to change.</td></row>
		<row><td>Condition</td><td>Feature_</td><td>N</td><td/><td/><td>Feature</td><td>1</td><td>Identifier</td><td/><td>Reference to a Feature entry in Feature table.</td></row>
		<row><td>Condition</td><td>Level</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>New selection Level to set in Feature table if Condition evaluates to TRUE.</td></row>
		<row><td>Control</td><td>Attributes</td><td>Y</td><td>0</td><td>2147483647</td><td/><td/><td/><td/><td>A 32-bit word that specifies the attribute flags to be applied to this control.</td></row>
		<row><td>Control</td><td>Binary_</td><td>Y</td><td/><td/><td>Binary</td><td>1</td><td>Identifier</td><td/><td>External key to the Binary table.</td></row>
		<row><td>Control</td><td>Control</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of the control. This name must be unique within a dialog, but can repeat on different dialogs.</td></row>
		<row><td>Control</td><td>Control_Next</td><td>Y</td><td/><td/><td>Control</td><td>2</td><td>Identifier</td><td/><td>The name of an other control on the same dialog. This link defines the tab order of the controls. The links have to form one or more cycles!</td></row>
		<row><td>Control</td><td>Dialog_</td><td>N</td><td/><td/><td>Dialog</td><td>1</td><td>Identifier</td><td/><td>External key to the Dialog table, name of the dialog.</td></row>
		<row><td>Control</td><td>Height</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Height of the bounding rectangle of the control.</td></row>
		<row><td>Control</td><td>Help</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The help strings used with the button. The text is optional.</td></row>
		<row><td>Control</td><td>ISBuildSourcePath</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Full path to .rtf file for scrollable text control</td></row>
		<row><td>Control</td><td>ISControlId</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>A number used to represent the control ID of the Control, Used in Dialog export</td></row>
		<row><td>Control</td><td>ISWindowStyle</td><td>Y</td><td>0</td><td>2147483647</td><td/><td/><td/><td/><td>A 32-bit word that specifies non-MSI window styles to be applied to this control.</td></row>
		<row><td>Control</td><td>Property</td><td>Y</td><td/><td/><td/><td/><td>Identifier</td><td/><td>The name of a defined property to be linked to this control.</td></row>
		<row><td>Control</td><td>Text</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>A string used to set the initial text contained within a control (if appropriate).</td></row>
		<row><td>Control</td><td>Type</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>The type of the control.</td></row>
		<row><td>Control</td><td>Width</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Width of the bounding rectangle of the control.</td></row>
		<row><td>Control</td><td>X</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Horizontal coordinate of the upper left corner of the bounding rectangle of the control.</td></row>
		<row><td>Control</td><td>Y</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Vertical coordinate of the upper left corner of the bounding rectangle of the control.</td></row>
		<row><td>ControlCondition</td><td>Action</td><td>N</td><td/><td/><td/><td/><td/><td>Default;Disable;Enable;Hide;Show</td><td>The desired action to be taken on the specified control.</td></row>
		<row><td>ControlCondition</td><td>Condition</td><td>N</td><td/><td/><td/><td/><td>Condition</td><td/><td>A standard conditional statement that specifies under which conditions the action should be triggered.</td></row>
		<row><td>ControlCondition</td><td>Control_</td><td>N</td><td/><td/><td>Control</td><td>2</td><td>Identifier</td><td/><td>A foreign key to the Control table, name of the control.</td></row>
		<row><td>ControlCondition</td><td>Dialog_</td><td>N</td><td/><td/><td>Dialog</td><td>1</td><td>Identifier</td><td/><td>A foreign key to the Dialog table, name of the dialog.</td></row>
		<row><td>ControlEvent</td><td>Argument</td><td>N</td><td/><td/><td/><td/><td>Formatted</td><td/><td>A value to be used as a modifier when triggering a particular event.</td></row>
		<row><td>ControlEvent</td><td>Condition</td><td>Y</td><td/><td/><td/><td/><td>Condition</td><td/><td>A standard conditional statement that specifies under which conditions an event should be triggered.</td></row>
		<row><td>ControlEvent</td><td>Control_</td><td>N</td><td/><td/><td>Control</td><td>2</td><td>Identifier</td><td/><td>A foreign key to the Control table, name of the control</td></row>
		<row><td>ControlEvent</td><td>Dialog_</td><td>N</td><td/><td/><td>Dialog</td><td>1</td><td>Identifier</td><td/><td>A foreign key to the Dialog table, name of the dialog.</td></row>
		<row><td>ControlEvent</td><td>Event</td><td>N</td><td/><td/><td/><td/><td>Formatted</td><td/><td>An identifier that specifies the type of the event that should take place when the user interacts with control specified by the first two entries.</td></row>
		<row><td>ControlEvent</td><td>Ordering</td><td>Y</td><td>0</td><td>2147483647</td><td/><td/><td/><td/><td>An integer used to order several events tied to the same control. Can be left blank.</td></row>
		<row><td>CreateFolder</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the Component table.</td></row>
		<row><td>CreateFolder</td><td>Directory_</td><td>N</td><td/><td/><td>Directory</td><td>1</td><td>Identifier</td><td/><td>Primary key, could be foreign key into the Directory table.</td></row>
		<row><td>CustomAction</td><td>Action</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key, name of action, normally appears in sequence table unless private use.</td></row>
		<row><td>CustomAction</td><td>ExtendedType</td><td>Y</td><td>0</td><td>2147483647</td><td/><td/><td/><td/><td>The numeric custom action type info flags.</td></row>
		<row><td>CustomAction</td><td>ISComments</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Author’s comments for this custom action.</td></row>
		<row><td>CustomAction</td><td>Source</td><td>Y</td><td/><td/><td/><td/><td>CustomSource</td><td/><td>The table reference of the source of the code.</td></row>
		<row><td>CustomAction</td><td>Target</td><td>Y</td><td/><td/><td>ISDLLWrapper;ISInstallScriptAction</td><td>1</td><td>Formatted</td><td/><td>Excecution parameter, depends on the type of custom action</td></row>
		<row><td>CustomAction</td><td>Type</td><td>N</td><td>1</td><td>32767</td><td/><td/><td/><td/><td>The numeric custom action type, consisting of source location, code type, entry, option flags.</td></row>
		<row><td>Dialog</td><td>Attributes</td><td>Y</td><td>0</td><td>2147483647</td><td/><td/><td/><td/><td>A 32-bit word that specifies the attribute flags to be applied to this dialog.</td></row>
		<row><td>Dialog</td><td>Control_Cancel</td><td>Y</td><td/><td/><td>Control</td><td>2</td><td>Identifier</td><td/><td>Defines the cancel control. Hitting escape or clicking on the close icon on the dialog is equivalent to pushing this button.</td></row>
		<row><td>Dialog</td><td>Control_Default</td><td>Y</td><td/><td/><td>Control</td><td>2</td><td>Identifier</td><td/><td>Defines the default control. Hitting return is equivalent to pushing this button.</td></row>
		<row><td>Dialog</td><td>Control_First</td><td>N</td><td/><td/><td>Control</td><td>2</td><td>Identifier</td><td/><td>Defines the control that has the focus when the dialog is created.</td></row>
		<row><td>Dialog</td><td>Dialog</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of the dialog.</td></row>
		<row><td>Dialog</td><td>HCentering</td><td>N</td><td>0</td><td>100</td><td/><td/><td/><td/><td>Horizontal position of the dialog on a 0-100 scale. 0 means left end, 100 means right end of the screen, 50 center.</td></row>
		<row><td>Dialog</td><td>Height</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Height of the bounding rectangle of the dialog.</td></row>
		<row><td>Dialog</td><td>ISComments</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Author’s comments for this dialog.</td></row>
		<row><td>Dialog</td><td>ISResourceId</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>A Number the Specifies the Dialog ID to be used in Dialog Export</td></row>
		<row><td>Dialog</td><td>ISWindowStyle</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>A 32-bit word that specifies non-MSI window styles to be applied to this control. This is only used in Script Based Setups.</td></row>
		<row><td>Dialog</td><td>TextStyle_</td><td>Y</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Foreign Key into TextStyle table, only used in Script Based Projects.</td></row>
		<row><td>Dialog</td><td>Title</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>A text string specifying the title to be displayed in the title bar of the dialog's window.</td></row>
		<row><td>Dialog</td><td>VCentering</td><td>N</td><td>0</td><td>100</td><td/><td/><td/><td/><td>Vertical position of the dialog on a 0-100 scale. 0 means top end, 100 means bottom end of the screen, 50 center.</td></row>
		<row><td>Dialog</td><td>Width</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Width of the bounding rectangle of the dialog.</td></row>
		<row><td>Directory</td><td>DefaultDir</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The default sub-path under parent's path.</td></row>
		<row><td>Directory</td><td>Directory</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Unique identifier for directory entry, primary key. If a property by this name is defined, it contains the full path to the directory.</td></row>
		<row><td>Directory</td><td>Directory_Parent</td><td>Y</td><td/><td/><td>Directory</td><td>1</td><td>Identifier</td><td/><td>Reference to the entry in this table specifying the default parent directory. A record parented to itself or with a Null parent represents a root of the install tree.</td></row>
		<row><td>Directory</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td>0;1;2;3;4;5;6;7</td><td>This is used to store Installshield custom properties of a directory.  Currently the only one is Shortcut.</td></row>
		<row><td>Directory</td><td>ISDescription</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Description of folder</td></row>
		<row><td>Directory</td><td>ISFolderName</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>This is used in Pro projects because the pro identifier used in the tree wasn't necessarily unique.</td></row>
		<row><td>DrLocator</td><td>Depth</td><td>Y</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>The depth below the path to which the Signature_ is recursively searched. If absent, the depth is assumed to be 0.</td></row>
		<row><td>DrLocator</td><td>Parent</td><td>Y</td><td/><td/><td/><td/><td>Identifier</td><td/><td>The parent file signature. It is also a foreign key in the Signature table. If null and the Path column does not expand to a full path, then all the fixed drives of the user system are searched using the Path.</td></row>
		<row><td>DrLocator</td><td>Path</td><td>Y</td><td/><td/><td/><td/><td>AnyPath</td><td/><td>The path on the user system. This is a either a subpath below the value of the Parent or a full path. The path may contain properties enclosed within [ ] that will be expanded.</td></row>
		<row><td>DrLocator</td><td>Signature_</td><td>N</td><td/><td/><td>Signature</td><td>1</td><td>Identifier</td><td/><td>The Signature_ represents a unique file signature and is also the foreign key in the Signature table.</td></row>
		<row><td>DuplicateFile</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key referencing Component that controls the duplicate file.</td></row>
		<row><td>DuplicateFile</td><td>DestFolder</td><td>Y</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of a property whose value is assumed to resolve to the full pathname to a destination folder.</td></row>
		<row><td>DuplicateFile</td><td>DestName</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Filename to be given to the duplicate file.</td></row>
		<row><td>DuplicateFile</td><td>FileKey</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key used to identify a particular file entry</td></row>
		<row><td>DuplicateFile</td><td>File_</td><td>N</td><td/><td/><td>File</td><td>1</td><td>Identifier</td><td/><td>Foreign key referencing the source file to be duplicated.</td></row>
		<row><td>Environment</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the Component table referencing component that controls the installing of the environmental value.</td></row>
		<row><td>Environment</td><td>Environment</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Unique identifier for the environmental variable setting</td></row>
		<row><td>Environment</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The name of the environmental value.</td></row>
		<row><td>Environment</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>The value to set in the environmental settings.</td></row>
		<row><td>Error</td><td>Error</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Integer error number, obtained from header file IError(...) macros.</td></row>
		<row><td>Error</td><td>Message</td><td>Y</td><td/><td/><td/><td/><td>Template</td><td/><td>Error formatting template, obtained from user ed. or localizers.</td></row>
		<row><td>EventMapping</td><td>Attribute</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>The name of the control attribute, that is set when this event is received.</td></row>
		<row><td>EventMapping</td><td>Control_</td><td>N</td><td/><td/><td>Control</td><td>2</td><td>Identifier</td><td/><td>A foreign key to the Control table, name of the control.</td></row>
		<row><td>EventMapping</td><td>Dialog_</td><td>N</td><td/><td/><td>Dialog</td><td>1</td><td>Identifier</td><td/><td>A foreign key to the Dialog table, name of the Dialog.</td></row>
		<row><td>EventMapping</td><td>Event</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>An identifier that specifies the type of the event that the control subscribes to.</td></row>
		<row><td>Extension</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Required foreign key into the Component Table, specifying the component for which to return a path when called through LocateComponent.</td></row>
		<row><td>Extension</td><td>Extension</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The extension associated with the table row.</td></row>
		<row><td>Extension</td><td>Feature_</td><td>N</td><td/><td/><td>Feature</td><td>1</td><td>Identifier</td><td/><td>Required foreign key into the Feature Table, specifying the feature to validate or install in order for the CLSID factory to be operational.</td></row>
		<row><td>Extension</td><td>MIME_</td><td>Y</td><td/><td/><td>MIME</td><td>1</td><td>Text</td><td/><td>Optional Context identifier, typically "type/format" associated with the extension</td></row>
		<row><td>Extension</td><td>ProgId_</td><td>Y</td><td/><td/><td>ProgId</td><td>1</td><td>Text</td><td/><td>Optional ProgId associated with this extension.</td></row>
		<row><td>Feature</td><td>Attributes</td><td>N</td><td/><td/><td/><td/><td/><td>0;1;2;4;5;6;8;9;10;16;17;18;20;21;22;24;25;26;32;33;34;36;37;38;48;49;50;52;53;54</td><td>Feature attributes</td></row>
		<row><td>Feature</td><td>Description</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Longer descriptive text describing a visible feature item.</td></row>
		<row><td>Feature</td><td>Directory_</td><td>Y</td><td/><td/><td>Directory</td><td>1</td><td>UpperCase</td><td/><td>The name of the Directory that can be configured by the UI. A non-null value will enable the browse button.</td></row>
		<row><td>Feature</td><td>Display</td><td>Y</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Numeric sort order, used to force a specific display ordering.</td></row>
		<row><td>Feature</td><td>Feature</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key used to identify a particular feature record.</td></row>
		<row><td>Feature</td><td>Feature_Parent</td><td>Y</td><td/><td/><td>Feature</td><td>1</td><td>Identifier</td><td/><td>Optional key of a parent record in the same table. If the parent is not selected, then the record will not be installed. Null indicates a root item.</td></row>
		<row><td>Feature</td><td>ISComments</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Comments</td></row>
		<row><td>Feature</td><td>ISFeatureCabName</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Name of CAB used when compressing CABs by Feature. Used to override build generated name for CAB file.</td></row>
		<row><td>Feature</td><td>ISProFeatureName</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The name of the feature used by pro projects.  This doesn't have to be unique.</td></row>
		<row><td>Feature</td><td>ISReleaseFlags</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Release Flags that specify whether this  feature will be built in a particular release.</td></row>
		<row><td>Feature</td><td>Level</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>The install level at which record will be initially selected. An install level of 0 will disable an item and prevent its display.</td></row>
		<row><td>Feature</td><td>Title</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Short text identifying a visible feature item.</td></row>
		<row><td>FeatureComponents</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key into Component table.</td></row>
		<row><td>FeatureComponents</td><td>Feature_</td><td>N</td><td/><td/><td>Feature</td><td>1</td><td>Identifier</td><td/><td>Foreign key into Feature table.</td></row>
		<row><td>File</td><td>Attributes</td><td>Y</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Integer containing bit flags representing file attributes (with the decimal value of each bit position in parentheses)</td></row>
		<row><td>File</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key referencing Component that controls the file.</td></row>
		<row><td>File</td><td>File</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key, non-localized token, must match identifier in cabinet.  For uncompressed files, this field is ignored.</td></row>
		<row><td>File</td><td>FileName</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>File name used for installation.  This may contain a "short name|long name" pair.  It may be just a long name, hence it cannot be of the Filename data type.</td></row>
		<row><td>File</td><td>FileSize</td><td>N</td><td>0</td><td>2147483647</td><td/><td/><td/><td/><td>Size of file in bytes (long integer).</td></row>
		<row><td>File</td><td>ISAttributes</td><td>Y</td><td>0</td><td>2147483647</td><td/><td/><td/><td/><td>This field contains the following attributes: UseSystemSettings(0x1)</td></row>
		<row><td>File</td><td>ISBuildSourcePath</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Full path, the category is of Text instead of Path because of potential use of path variables.</td></row>
		<row><td>File</td><td>ISComponentSubFolder_</td><td>Y</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Foreign key referencing component subfolder containing this file.  Only for Pro.</td></row>
		<row><td>File</td><td>Language</td><td>Y</td><td/><td/><td/><td/><td>Language</td><td/><td>List of decimal language Ids, comma-separated if more than one.</td></row>
		<row><td>File</td><td>Sequence</td><td>N</td><td>1</td><td>32767</td><td/><td/><td/><td/><td>Sequence with respect to the media images; order must track cabinet order.</td></row>
		<row><td>File</td><td>Version</td><td>Y</td><td/><td/><td>File</td><td>1</td><td>Version</td><td/><td>Version string for versioned files;  Blank for unversioned files.</td></row>
		<row><td>FileSFPCatalog</td><td>File_</td><td>N</td><td/><td/><td>File</td><td>1</td><td>Identifier</td><td/><td>File associated with the catalog</td></row>
		<row><td>FileSFPCatalog</td><td>SFPCatalog_</td><td>N</td><td/><td/><td>SFPCatalog</td><td>1</td><td>Text</td><td/><td>Catalog associated with the file</td></row>
		<row><td>Font</td><td>File_</td><td>N</td><td/><td/><td>File</td><td>1</td><td>Identifier</td><td/><td>Primary key, foreign key into File table referencing font file.</td></row>
		<row><td>Font</td><td>FontTitle</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Font name.</td></row>
		<row><td>ISAssistantTag</td><td>Data</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISAssistantTag</td><td>Tag</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISBillBoard</td><td>Color</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISBillBoard</td><td>DisplayName</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISBillBoard</td><td>Duration</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td/></row>
		<row><td>ISBillBoard</td><td>Effect</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td/></row>
		<row><td>ISBillBoard</td><td>Font</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISBillBoard</td><td>ISBillboard</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISBillBoard</td><td>Origin</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td/></row>
		<row><td>ISBillBoard</td><td>Sequence</td><td>N</td><td>-32767</td><td>32767</td><td/><td/><td/><td/><td/></row>
		<row><td>ISBillBoard</td><td>Style</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISBillBoard</td><td>Target</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td/></row>
		<row><td>ISBillBoard</td><td>Title</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISBillBoard</td><td>X</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td/></row>
		<row><td>ISBillBoard</td><td>Y</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td/></row>
		<row><td>ISChainPackage</td><td>DisplayName</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Display name for the chained package. Used only in the IDE.</td></row>
		<row><td>ISChainPackage</td><td>ISReleaseFlags</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISChainPackage</td><td>InstallCondition</td><td>Y</td><td/><td/><td/><td/><td>Condition</td><td/><td/></row>
		<row><td>ISChainPackage</td><td>InstallProperties</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td/></row>
		<row><td>ISChainPackage</td><td>Options</td><td>N</td><td/><td/><td/><td/><td>Integer</td><td/><td/></row>
		<row><td>ISChainPackage</td><td>Order</td><td>N</td><td/><td/><td/><td/><td>Integer</td><td/><td/></row>
		<row><td>ISChainPackage</td><td>Package</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td/></row>
		<row><td>ISChainPackage</td><td>ProductCode</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISChainPackage</td><td>RemoveCondition</td><td>Y</td><td/><td/><td/><td/><td>Condition</td><td/><td/></row>
		<row><td>ISChainPackage</td><td>RemoveProperties</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td/></row>
		<row><td>ISChainPackage</td><td>SourcePath</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISChainPackageData</td><td>Data</td><td>Y</td><td/><td/><td/><td/><td>Binary</td><td/><td>Binary stream. The binary icon data in PE (.DLL or .EXE) or icon (.ICO) format.</td></row>
		<row><td>ISChainPackageData</td><td>File</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td/></row>
		<row><td>ISChainPackageData</td><td>FilePath</td><td>N</td><td/><td/><td/><td/><td>Formatted</td><td/><td/></row>
		<row><td>ISChainPackageData</td><td>ISBuildSourcePath</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Full path to the ICO or EXE file.</td></row>
		<row><td>ISChainPackageData</td><td>Options</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISChainPackageData</td><td>Package_</td><td>N</td><td/><td/><td>ISChainPackage</td><td>1</td><td>Identifier</td><td/><td/></row>
		<row><td>ISClrWrap</td><td>Action_</td><td>N</td><td/><td/><td>CustomAction</td><td>1</td><td>Identifier</td><td/><td>Foreign key into CustomAction table</td></row>
		<row><td>ISClrWrap</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Property associated with this Action</td></row>
		<row><td>ISClrWrap</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Value associated with this Property</td></row>
		<row><td>ISComCatalogAttribute</td><td>ISComCatalogObject_</td><td>N</td><td/><td/><td>ISComCatalogObject</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the ISComCatalogObject table.</td></row>
		<row><td>ISComCatalogAttribute</td><td>ItemName</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The named attribute for a catalog object.</td></row>
		<row><td>ISComCatalogAttribute</td><td>ItemValue</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>A value associated with the attribute defined in the ItemName column.</td></row>
		<row><td>ISComCatalogCollection</td><td>CollectionName</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>A catalog collection name.</td></row>
		<row><td>ISComCatalogCollection</td><td>ISComCatalogCollection</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>A unique key for the ISComCatalogCollection table.</td></row>
		<row><td>ISComCatalogCollection</td><td>ISComCatalogObject_</td><td>N</td><td/><td/><td>ISComCatalogObject</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the ISComCatalogObject table.</td></row>
		<row><td>ISComCatalogCollectionObjects</td><td>ISComCatalogCollection_</td><td>N</td><td/><td/><td>ISComCatalogCollection</td><td>1</td><td>Identifier</td><td/><td>A unique key for the ISComCatalogCollection table.</td></row>
		<row><td>ISComCatalogCollectionObjects</td><td>ISComCatalogObject_</td><td>N</td><td/><td/><td>ISComCatalogObject</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the ISComCatalogObject table.</td></row>
		<row><td>ISComCatalogObject</td><td>DisplayName</td><td>N</td><td/><td/><td/><td/><td/><td/><td>The display name of a catalog object.</td></row>
		<row><td>ISComCatalogObject</td><td>ISComCatalogObject</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>A unique key for the ISComCatalogObject table.</td></row>
		<row><td>ISComPlusApplication</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the Component table that a COM+ application belongs to.</td></row>
		<row><td>ISComPlusApplication</td><td>ComputerName</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Computer name that a COM+ application belongs to.</td></row>
		<row><td>ISComPlusApplication</td><td>DepFiles</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>List of the dependent files.</td></row>
		<row><td>ISComPlusApplication</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>InstallShield custom attributes associated with a COM+ application.</td></row>
		<row><td>ISComPlusApplication</td><td>ISComCatalogObject_</td><td>N</td><td/><td/><td>ISComCatalogObject</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the ISComCatalogObject table.</td></row>
		<row><td>ISComPlusApplicationDLL</td><td>AlterDLL</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Alternate filename of the COM+ application component. Will be used for a .NET serviced component.</td></row>
		<row><td>ISComPlusApplicationDLL</td><td>CLSID</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>CLSID of the COM+ application component.</td></row>
		<row><td>ISComPlusApplicationDLL</td><td>DLL</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Filename of the COM+ application component.</td></row>
		<row><td>ISComPlusApplicationDLL</td><td>ISComCatalogObject_</td><td>N</td><td/><td/><td>ISComCatalogObject</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the ISComCatalogObject table.</td></row>
		<row><td>ISComPlusApplicationDLL</td><td>ISComPlusApplicationDLL</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>A unique key for the ISComPlusApplicationDLL table.</td></row>
		<row><td>ISComPlusApplicationDLL</td><td>ISComPlusApplication_</td><td>N</td><td/><td/><td>ISComPlusApplication</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the ISComPlusApplication table.</td></row>
		<row><td>ISComPlusApplicationDLL</td><td>ProgId</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>ProgId of the COM+ application component.</td></row>
		<row><td>ISComPlusProxy</td><td>Component_</td><td>Y</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the Component table that a COM+ application proxy belongs to.</td></row>
		<row><td>ISComPlusProxy</td><td>DepFiles</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>List of the dependent files.</td></row>
		<row><td>ISComPlusProxy</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>InstallShield custom attributes associated with a COM+ application proxy.</td></row>
		<row><td>ISComPlusProxy</td><td>ISComPlusApplication_</td><td>N</td><td/><td/><td>ISComPlusApplication</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the ISComPlusApplication table that a COM+ application proxy belongs to.</td></row>
		<row><td>ISComPlusProxy</td><td>ISComPlusProxy</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>A unique key for the ISComPlusProxy table.</td></row>
		<row><td>ISComPlusProxyDepFile</td><td>File_</td><td>N</td><td/><td/><td>File</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the File table.</td></row>
		<row><td>ISComPlusProxyDepFile</td><td>ISComPlusApplication_</td><td>N</td><td/><td/><td>ISComPlusApplication</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the ISComPlusApplication table.</td></row>
		<row><td>ISComPlusProxyDepFile</td><td>ISPath</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Full path of the dependent file.</td></row>
		<row><td>ISComPlusProxyFile</td><td>File_</td><td>N</td><td/><td/><td>File</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the File table.</td></row>
		<row><td>ISComPlusProxyFile</td><td>ISComPlusApplicationDLL_</td><td>N</td><td/><td/><td>ISComPlusApplicationDLL</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the ISComPlusApplicationDLL table.</td></row>
		<row><td>ISComPlusServerDepFile</td><td>File_</td><td>N</td><td/><td/><td>File</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the File table.</td></row>
		<row><td>ISComPlusServerDepFile</td><td>ISComPlusApplication_</td><td>N</td><td/><td/><td>ISComPlusApplication</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the ISComPlusApplication table.</td></row>
		<row><td>ISComPlusServerDepFile</td><td>ISPath</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Full path of the dependent file.</td></row>
		<row><td>ISComPlusServerFile</td><td>File_</td><td>N</td><td/><td/><td>File</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the File table.</td></row>
		<row><td>ISComPlusServerFile</td><td>ISComPlusApplicationDLL_</td><td>N</td><td/><td/><td>ISComPlusApplicationDLL</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the ISComPlusApplicationDLL table.</td></row>
		<row><td>ISComponentExtended</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Primary key used to identify a particular component record.</td></row>
		<row><td>ISComponentExtended</td><td>FTPLocation</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>FTP Location</td></row>
		<row><td>ISComponentExtended</td><td>FilterProperty</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Property to set if you want to filter a component</td></row>
		<row><td>ISComponentExtended</td><td>HTTPLocation</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>HTTP Location</td></row>
		<row><td>ISComponentExtended</td><td>Language</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Language</td></row>
		<row><td>ISComponentExtended</td><td>Miscellaneous</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Miscellaneous</td></row>
		<row><td>ISComponentExtended</td><td>OS</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>bitwise addition of OSs</td></row>
		<row><td>ISComponentExtended</td><td>Platforms</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>bitwise addition of Platforms.</td></row>
		<row><td>ISCustomActionReference</td><td>Action_</td><td>N</td><td/><td/><td>CustomAction</td><td>1</td><td>Identifier</td><td/><td>Foreign key into theICustomAction table.</td></row>
		<row><td>ISCustomActionReference</td><td>Description</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Contents of the file speciifed in ISCAReferenceFilePath. This column is only used by MSI.</td></row>
		<row><td>ISCustomActionReference</td><td>FileType</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>file type of the file specified  ISCAReferenceFilePath. This column is only used by MSI.</td></row>
		<row><td>ISCustomActionReference</td><td>ISCAReferenceFilePath</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Full path, the category is of Text instead of Path because of potential use of path variables.  This column only exists in ISM.</td></row>
		<row><td>ISDIMDependency</td><td>ISDIMReference_</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>This is the primary key to the ISDIMDependency table</td></row>
		<row><td>ISDIMDependency</td><td>RequiredBuildVersion</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>the build version identifying the required DIM</td></row>
		<row><td>ISDIMDependency</td><td>RequiredMajorVersion</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>the major version identifying the required DIM</td></row>
		<row><td>ISDIMDependency</td><td>RequiredMinorVersion</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>the minor version identifying the required DIM</td></row>
		<row><td>ISDIMDependency</td><td>RequiredRevisionVersion</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>the revision version identifying the required DIM</td></row>
		<row><td>ISDIMDependency</td><td>RequiredUUID</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>the UUID identifying the required DIM</td></row>
		<row><td>ISDIMReference</td><td>ISBuildSourcePath</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Full path, the category is of Text instead of Path because of potential use of path variables.</td></row>
		<row><td>ISDIMReference</td><td>ISDIMReference</td><td>N</td><td/><td/><td>ISDIMDependency</td><td>1</td><td>Identifier</td><td/><td>This is the primary key to the ISDIMReference table</td></row>
		<row><td>ISDIMReferenceDependencies</td><td>ISDIMDependency_</td><td>N</td><td/><td/><td>ISDIMDependency</td><td>1</td><td>Identifier</td><td/><td>Foreign key into ISDIMDependency table.</td></row>
		<row><td>ISDIMReferenceDependencies</td><td>ISDIMReference_Parent</td><td>N</td><td/><td/><td>ISDIMReference</td><td>1</td><td>Identifier</td><td/><td>Foreign key into ISDIMReference table.</td></row>
		<row><td>ISDIMVariable</td><td>ISDIMReference_</td><td>N</td><td/><td/><td>ISDIMReference</td><td>1</td><td>Identifier</td><td/><td>Foreign key into ISDIMReference table.</td></row>
		<row><td>ISDIMVariable</td><td>ISDIMVariable</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>This is the primary key to the ISDIMVariable table</td></row>
		<row><td>ISDIMVariable</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Name of a variable defined in the .dim file</td></row>
		<row><td>ISDIMVariable</td><td>NewValue</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>New value that you want to override with</td></row>
		<row><td>ISDIMVariable</td><td>Type</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Type of the variable. 0: Build Variable, 1: Runtime Variable</td></row>
		<row><td>ISDLLWrapper</td><td>EntryPoint</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>This is a foreign key to the target column in the CustomAction table</td></row>
		<row><td>ISDLLWrapper</td><td>Source</td><td>N</td><td/><td/><td/><td/><td>Formatted</td><td/><td>This is column points to the source file for the DLLWrapper Custom Action</td></row>
		<row><td>ISDLLWrapper</td><td>Target</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The function signature</td></row>
		<row><td>ISDLLWrapper</td><td>Type</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Type</td></row>
		<row><td>ISDRMFile</td><td>File_</td><td>Y</td><td/><td/><td>File</td><td>1</td><td>Identifier</td><td/><td>Foreign key into File table.  A null value will cause a build warning.</td></row>
		<row><td>ISDRMFile</td><td>ISDRMFile</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Unique identifier for this item.</td></row>
		<row><td>ISDRMFile</td><td>ISDRMLicense_</td><td>Y</td><td/><td/><td>ISDRMLicense</td><td>1</td><td>Identifier</td><td/><td>Foreign key referencing License that packages this file.</td></row>
		<row><td>ISDRMFile</td><td>Shell</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Text indicating the activation shell used at runtime.</td></row>
		<row><td>ISDRMFileAttribute</td><td>ISDRMFile_</td><td>N</td><td/><td/><td>ISDRMFile</td><td>1</td><td>Identifier</td><td/><td>Primary foreign key into ISDRMFile table.</td></row>
		<row><td>ISDRMFileAttribute</td><td>Property</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The name of the attribute</td></row>
		<row><td>ISDRMFileAttribute</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The value of the attribute</td></row>
		<row><td>ISDRMLicense</td><td>Attributes</td><td>Y</td><td/><td/><td/><td/><td>Number</td><td/><td>Bitwise field used to specify binary attributes of this license.</td></row>
		<row><td>ISDRMLicense</td><td>Description</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>An internal description of this license.</td></row>
		<row><td>ISDRMLicense</td><td>ISDRMLicense</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Unique key identifying the license record.</td></row>
		<row><td>ISDRMLicense</td><td>LicenseNumber</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The license number.</td></row>
		<row><td>ISDRMLicense</td><td>ProjectVersion</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The version of the project that this license is tied to.</td></row>
		<row><td>ISDRMLicense</td><td>RequestCode</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The request code.</td></row>
		<row><td>ISDRMLicense</td><td>ResponseCode</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The response code.</td></row>
		<row><td>ISDependency</td><td>Exclude</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISDependency</td><td>ISDependency</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISDisk1File</td><td>Disk</td><td>Y</td><td/><td/><td/><td/><td/><td>-1;0;1</td><td>Used to differentiate between disk1(1), last disk(-1), and other(0).</td></row>
		<row><td>ISDisk1File</td><td>ISBuildSourcePath</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Full path of file to be copied to Disk1 folder</td></row>
		<row><td>ISDisk1File</td><td>ISDisk1File</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key for ISDisk1File table</td></row>
		<row><td>ISDynamicFile</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key referencing Component that controls the file.</td></row>
		<row><td>ISDynamicFile</td><td>ExcludeFiles</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Wildcards for excluded files.</td></row>
		<row><td>ISDynamicFile</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td>0;1;2;3;4;5;6;7;8;9;10;11;12;13;14;15</td><td>This is used to store Installshield custom properties of a dynamic filet.  Currently the only one is SelfRegister.</td></row>
		<row><td>ISDynamicFile</td><td>IncludeFiles</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Wildcards for included files.</td></row>
		<row><td>ISDynamicFile</td><td>IncludeFlags</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Include flags.</td></row>
		<row><td>ISDynamicFile</td><td>SourceFolder</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Full path, the category is of Text instead of Path because of potential use of path variables.</td></row>
		<row><td>ISFeatureDIMReferences</td><td>Feature_</td><td>N</td><td/><td/><td>Feature</td><td>1</td><td>Identifier</td><td/><td>Foreign key into Feature table.</td></row>
		<row><td>ISFeatureDIMReferences</td><td>ISDIMReference_</td><td>N</td><td/><td/><td>ISDIMReference</td><td>1</td><td>Identifier</td><td/><td>Foreign key into ISDIMReference table.</td></row>
		<row><td>ISFeatureMergeModuleExcludes</td><td>Feature_</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Foreign key into Feature table.</td></row>
		<row><td>ISFeatureMergeModuleExcludes</td><td>Language</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Foreign key into ISMergeModule table.</td></row>
		<row><td>ISFeatureMergeModuleExcludes</td><td>ModuleID</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Foreign key into ISMergeModule table.</td></row>
		<row><td>ISFeatureMergeModules</td><td>Feature_</td><td>N</td><td/><td/><td>Feature</td><td>1</td><td>Identifier</td><td/><td>Foreign key into Feature table.</td></row>
		<row><td>ISFeatureMergeModules</td><td>ISMergeModule_</td><td>N</td><td/><td/><td>ISMergeModule</td><td>1</td><td>Text</td><td/><td>Foreign key into ISMergeModule table.</td></row>
		<row><td>ISFeatureMergeModules</td><td>Language_</td><td>N</td><td/><td/><td>ISMergeModule</td><td>2</td><td/><td/><td>Foreign key into ISMergeModule table.</td></row>
		<row><td>ISFeatureSetupPrerequisites</td><td>Feature_</td><td>N</td><td/><td/><td>Feature</td><td>1</td><td>Identifier</td><td/><td>Foreign key into Feature table.</td></row>
		<row><td>ISFeatureSetupPrerequisites</td><td>ISSetupPrerequisites_</td><td>N</td><td/><td/><td>ISSetupPrerequisites</td><td>1</td><td/><td/><td/></row>
		<row><td>ISFileManifests</td><td>File_</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Foreign key into File table.</td></row>
		<row><td>ISFileManifests</td><td>Manifest_</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Foreign key into File table.</td></row>
		<row><td>ISIISItem</td><td>Component_</td><td>Y</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key to Component table.</td></row>
		<row><td>ISIISItem</td><td>DisplayName</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Localizable Item Name.</td></row>
		<row><td>ISIISItem</td><td>ISIISItem</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key for each item.</td></row>
		<row><td>ISIISItem</td><td>ISIISItem_Parent</td><td>Y</td><td/><td/><td>ISIISItem</td><td>1</td><td>Identifier</td><td/><td>This record's parent record.</td></row>
		<row><td>ISIISItem</td><td>Type</td><td>N</td><td/><td/><td/><td/><td/><td/><td>IIS resource type.</td></row>
		<row><td>ISIISProperty</td><td>FriendlyName</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>IIS property name.</td></row>
		<row><td>ISIISProperty</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Flags.</td></row>
		<row><td>ISIISProperty</td><td>ISIISItem_</td><td>N</td><td/><td/><td>ISIISItem</td><td>1</td><td>Identifier</td><td/><td>Primary key for table, foreign key into ISIISItem.</td></row>
		<row><td>ISIISProperty</td><td>ISIISProperty</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key for table.</td></row>
		<row><td>ISIISProperty</td><td>MetaDataAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>IIS property attributes.</td></row>
		<row><td>ISIISProperty</td><td>MetaDataProp</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>IIS property ID.</td></row>
		<row><td>ISIISProperty</td><td>MetaDataType</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>IIS property data type.</td></row>
		<row><td>ISIISProperty</td><td>MetaDataUserType</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>IIS property user data type.</td></row>
		<row><td>ISIISProperty</td><td>MetaDataValue</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>IIS property value.</td></row>
		<row><td>ISIISProperty</td><td>Order</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Order sequencing.</td></row>
		<row><td>ISIISProperty</td><td>Schema</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>IIS7 schema information.</td></row>
		<row><td>ISInstallScriptAction</td><td>EntryPoint</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>This is a foreign key to the target column in the CustomAction table</td></row>
		<row><td>ISInstallScriptAction</td><td>Source</td><td>N</td><td/><td/><td/><td/><td>Formatted</td><td/><td>This is column points to the source file for the DLLWrapper Custom Action</td></row>
		<row><td>ISInstallScriptAction</td><td>Target</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The function signature</td></row>
		<row><td>ISInstallScriptAction</td><td>Type</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Type</td></row>
		<row><td>ISLanguage</td><td>ISLanguage</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>This is the language ID.</td></row>
		<row><td>ISLanguage</td><td>Included</td><td>Y</td><td/><td/><td/><td/><td/><td>0;1</td><td>Specify whether this language should be included.</td></row>
		<row><td>ISLinkerLibrary</td><td>ISLinkerLibrary</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Unique identifier for the link library.</td></row>
		<row><td>ISLinkerLibrary</td><td>Library</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Full path of the object library (.obl file).</td></row>
		<row><td>ISLinkerLibrary</td><td>Order</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Order of the Library</td></row>
		<row><td>ISLocalControl</td><td>Attributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>A 32-bit word that specifies the attribute flags to be applied to this control.</td></row>
		<row><td>ISLocalControl</td><td>Binary_</td><td>Y</td><td/><td/><td>Binary</td><td>1</td><td>Identifier</td><td/><td>External key to the Binary table.</td></row>
		<row><td>ISLocalControl</td><td>Control_</td><td>N</td><td/><td/><td>Control</td><td>2</td><td>Identifier</td><td/><td>Name of the control. This name must be unique within a dialog, but can repeat on different dialogs.</td></row>
		<row><td>ISLocalControl</td><td>Dialog_</td><td>N</td><td/><td/><td>Dialog</td><td>1</td><td>Identifier</td><td/><td>External key to the Dialog table, name of the dialog.</td></row>
		<row><td>ISLocalControl</td><td>Height</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Height of the bounding rectangle of the control.</td></row>
		<row><td>ISLocalControl</td><td>ISBuildSourcePath</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Full path to .rtf file for scrollable text control</td></row>
		<row><td>ISLocalControl</td><td>ISLanguage_</td><td>N</td><td/><td/><td>ISLanguage</td><td>1</td><td>Text</td><td/><td>This is a foreign key to the ISLanguage table.</td></row>
		<row><td>ISLocalControl</td><td>Width</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Width of the bounding rectangle of the control.</td></row>
		<row><td>ISLocalControl</td><td>X</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Horizontal coordinate of the upper left corner of the bounding rectangle of the control.</td></row>
		<row><td>ISLocalControl</td><td>Y</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Vertical coordinate of the upper left corner of the bounding rectangle of the control.</td></row>
		<row><td>ISLocalDialog</td><td>Attributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>A 32-bit word that specifies the attribute flags to be applied to this dialog.</td></row>
		<row><td>ISLocalDialog</td><td>Dialog_</td><td>Y</td><td/><td/><td>Dialog</td><td>1</td><td>Identifier</td><td/><td>Name of the dialog.</td></row>
		<row><td>ISLocalDialog</td><td>Height</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Height of the bounding rectangle of the dialog.</td></row>
		<row><td>ISLocalDialog</td><td>ISLanguage_</td><td>Y</td><td/><td/><td>ISLanguage</td><td>1</td><td>Text</td><td/><td>This is a foreign key to the ISLanguage table.</td></row>
		<row><td>ISLocalDialog</td><td>TextStyle_</td><td>Y</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Foreign Key into TextStyle table, only used in Script Based Projects.</td></row>
		<row><td>ISLocalDialog</td><td>Width</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Width of the bounding rectangle of the dialog.</td></row>
		<row><td>ISLocalRadioButton</td><td>Height</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>The height of the button.</td></row>
		<row><td>ISLocalRadioButton</td><td>ISLanguage_</td><td>N</td><td/><td/><td>ISLanguage</td><td>1</td><td>Text</td><td/><td>This is a foreign key to the ISLanguage table.</td></row>
		<row><td>ISLocalRadioButton</td><td>Order</td><td>N</td><td>1</td><td>32767</td><td>RadioButton</td><td>2</td><td/><td/><td>A positive integer used to determine the ordering of the items within one list..The integers do not have to be consecutive.</td></row>
		<row><td>ISLocalRadioButton</td><td>Property</td><td>N</td><td/><td/><td>RadioButton</td><td>1</td><td>Identifier</td><td/><td>A named property to be tied to this radio button. All the buttons tied to the same property become part of the same group.</td></row>
		<row><td>ISLocalRadioButton</td><td>Width</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>The width of the button.</td></row>
		<row><td>ISLocalRadioButton</td><td>X</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>The horizontal coordinate of the upper left corner of the bounding rectangle of the radio button.</td></row>
		<row><td>ISLocalRadioButton</td><td>Y</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>The vertical coordinate of the upper left corner of the bounding rectangle of the radio button.</td></row>
		<row><td>ISLockPermissions</td><td>Attributes</td><td>Y</td><td>-2147483647</td><td>2147483647</td><td/><td/><td/><td/><td>Permissions attributes mask, 1==Deny access; 2==No inherit, 4==Ignore apply failures, 8==Target object is 64-bit</td></row>
		<row><td>ISLockPermissions</td><td>Domain</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Domain name for user whose permissions are being set.</td></row>
		<row><td>ISLockPermissions</td><td>LockObject</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Foreign key into CreateFolder, Registry, or File table</td></row>
		<row><td>ISLockPermissions</td><td>Permission</td><td>Y</td><td>-2147483647</td><td>2147483647</td><td/><td/><td/><td/><td>Permission Access mask.</td></row>
		<row><td>ISLockPermissions</td><td>Table</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td>CreateFolder;File;Registry</td><td>Reference to another table name</td></row>
		<row><td>ISLockPermissions</td><td>User</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>User for permissions to be set. This can be a property, hardcoded named, or SID string</td></row>
		<row><td>ISLogicalDisk</td><td>Cabinet</td><td>Y</td><td/><td/><td/><td/><td>Cabinet</td><td/><td>If some or all of the files stored on the media are compressed in a cabinet, the name of that cabinet.</td></row>
		<row><td>ISLogicalDisk</td><td>DiskId</td><td>N</td><td>1</td><td>32767</td><td/><td/><td/><td/><td>Primary key, integer to determine sort order for table.</td></row>
		<row><td>ISLogicalDisk</td><td>DiskPrompt</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Disk name: the visible text actually printed on the disk.  This will be used to prompt the user when this disk needs to be inserted.</td></row>
		<row><td>ISLogicalDisk</td><td>ISProductConfiguration_</td><td>N</td><td/><td/><td>ISProductConfiguration</td><td>1</td><td>Text</td><td/><td>Foreign key into the ISProductConfiguration table.</td></row>
		<row><td>ISLogicalDisk</td><td>ISRelease_</td><td>N</td><td/><td/><td>ISRelease</td><td>1</td><td>Text</td><td/><td>Foreign key into the ISRelease table.</td></row>
		<row><td>ISLogicalDisk</td><td>LastSequence</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>File sequence number for the last file for this media.</td></row>
		<row><td>ISLogicalDisk</td><td>Source</td><td>Y</td><td/><td/><td/><td/><td>Property</td><td/><td>The property defining the location of the cabinet file.</td></row>
		<row><td>ISLogicalDisk</td><td>VolumeLabel</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The label attributed to the volume.</td></row>
		<row><td>ISLogicalDiskFeatures</td><td>Feature_</td><td>Y</td><td/><td/><td>Feature</td><td>1</td><td>Identifier</td><td/><td>Required foreign key into the Feature Table,</td></row>
		<row><td>ISLogicalDiskFeatures</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>This is used to store Installshield custom properties, like Compressed, etc.</td></row>
		<row><td>ISLogicalDiskFeatures</td><td>ISLogicalDisk_</td><td>N</td><td>1</td><td>32767</td><td>ISLogicalDisk</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the ISLogicalDisk table.</td></row>
		<row><td>ISLogicalDiskFeatures</td><td>ISProductConfiguration_</td><td>N</td><td/><td/><td>ISProductConfiguration</td><td>1</td><td>Text</td><td/><td>Foreign key into the ISProductConfiguration table.</td></row>
		<row><td>ISLogicalDiskFeatures</td><td>ISRelease_</td><td>N</td><td/><td/><td>ISRelease</td><td>1</td><td>Text</td><td/><td>Foreign key into the ISRelease table.</td></row>
		<row><td>ISLogicalDiskFeatures</td><td>Sequence</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>File sequence number for the file for this media.</td></row>
		<row><td>ISMergeModule</td><td>Destination</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Destination.</td></row>
		<row><td>ISMergeModule</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>This is used to store Installshield custom properties of a merge module.</td></row>
		<row><td>ISMergeModule</td><td>ISMergeModule</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The GUID identifying the merge module.</td></row>
		<row><td>ISMergeModule</td><td>Language</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Default decimal language of module.</td></row>
		<row><td>ISMergeModule</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Name of the merge module.</td></row>
		<row><td>ISMergeModuleCfgValues</td><td>Attributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Attributes (from configurable merge module)</td></row>
		<row><td>ISMergeModuleCfgValues</td><td>ContextData</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>ContextData  (from configurable merge module)</td></row>
		<row><td>ISMergeModuleCfgValues</td><td>DefaultValue</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>DefaultValue  (from configurable merge module)</td></row>
		<row><td>ISMergeModuleCfgValues</td><td>Description</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Description (from configurable merge module)</td></row>
		<row><td>ISMergeModuleCfgValues</td><td>DisplayName</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>DisplayName (from configurable merge module)</td></row>
		<row><td>ISMergeModuleCfgValues</td><td>Format</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Format (from configurable merge module)</td></row>
		<row><td>ISMergeModuleCfgValues</td><td>HelpKeyword</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>HelpKeyword (from configurable merge module)</td></row>
		<row><td>ISMergeModuleCfgValues</td><td>HelpLocation</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>HelpLocation (from configurable merge module)</td></row>
		<row><td>ISMergeModuleCfgValues</td><td>ISMergeModule_</td><td>N</td><td/><td/><td>ISMergeModule</td><td>1</td><td>Text</td><td/><td>The module signature, a foreign key into the ISMergeModule table</td></row>
		<row><td>ISMergeModuleCfgValues</td><td>Language_</td><td>N</td><td/><td/><td>ISMergeModule</td><td>2</td><td/><td/><td>Default decimal language of module.</td></row>
		<row><td>ISMergeModuleCfgValues</td><td>ModuleConfiguration_</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Identifier, foreign key into ModuleConfiguration table (ModuleConfiguration.Name)</td></row>
		<row><td>ISMergeModuleCfgValues</td><td>Type</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Type (from configurable merge module)</td></row>
		<row><td>ISMergeModuleCfgValues</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Value for this item.</td></row>
		<row><td>ISObject</td><td>Language</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td/></row>
		<row><td>ISObject</td><td>ObjectName</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td/></row>
		<row><td>ISObjectProperty</td><td>IncludeInBuild</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Boolean, 0 for false non 0 for true</td></row>
		<row><td>ISObjectProperty</td><td>ObjectName</td><td>Y</td><td/><td/><td>ISObject</td><td>1</td><td>Text</td><td/><td/></row>
		<row><td>ISObjectProperty</td><td>Property</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td/></row>
		<row><td>ISObjectProperty</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td/></row>
		<row><td>ISPatchConfigImage</td><td>PatchConfiguration_</td><td>Y</td><td/><td/><td>ISPatchConfiguration</td><td>1</td><td>Text</td><td/><td>Foreign key to the ISPatchConfigurationTable</td></row>
		<row><td>ISPatchConfigImage</td><td>UpgradedImage_</td><td>N</td><td/><td/><td>ISUpgradedImage</td><td>1</td><td>Text</td><td/><td>Foreign key to the ISUpgradedImageTable</td></row>
		<row><td>ISPatchConfiguration</td><td>Attributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>PatchConfiguration attributes</td></row>
		<row><td>ISPatchConfiguration</td><td>CanPCDiffer</td><td>N</td><td/><td/><td/><td/><td/><td/><td>This is determine whether Product Codes may differ</td></row>
		<row><td>ISPatchConfiguration</td><td>CanPVDiffer</td><td>N</td><td/><td/><td/><td/><td/><td/><td>This is determine whether the Major Product Version may differ</td></row>
		<row><td>ISPatchConfiguration</td><td>EnablePatchCache</td><td>N</td><td/><td/><td/><td/><td/><td/><td>This is determine whether to Enable Patch cacheing</td></row>
		<row><td>ISPatchConfiguration</td><td>Flags</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Patching API Flags</td></row>
		<row><td>ISPatchConfiguration</td><td>IncludeWholeFiles</td><td>N</td><td/><td/><td/><td/><td/><td/><td>This is determine whether to build a binary level patch</td></row>
		<row><td>ISPatchConfiguration</td><td>LeaveDecompressed</td><td>N</td><td/><td/><td/><td/><td/><td/><td>This is determine whether to leave intermediate files devcompressed when finished</td></row>
		<row><td>ISPatchConfiguration</td><td>MinMsiVersion</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Minimum Required MSI Version</td></row>
		<row><td>ISPatchConfiguration</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Name of the Patch Configuration</td></row>
		<row><td>ISPatchConfiguration</td><td>OptimizeForSize</td><td>N</td><td/><td/><td/><td/><td/><td/><td>This is determine whether to Optimize for large files</td></row>
		<row><td>ISPatchConfiguration</td><td>OutputPath</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Build Location</td></row>
		<row><td>ISPatchConfiguration</td><td>PatchCacheDir</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Directory to recieve the Patch Cache information</td></row>
		<row><td>ISPatchConfiguration</td><td>PatchGuid</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Unique Patch Identifier</td></row>
		<row><td>ISPatchConfiguration</td><td>PatchGuidsToReplace</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>List Of Patch Guids to unregister</td></row>
		<row><td>ISPatchConfiguration</td><td>TargetProductCodes</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>List Of target Product Codes</td></row>
		<row><td>ISPatchConfigurationProperty</td><td>ISPatchConfiguration_</td><td>Y</td><td/><td/><td>ISPatchConfiguration</td><td>1</td><td>Text</td><td/><td>Name of the Patch Configuration</td></row>
		<row><td>ISPatchConfigurationProperty</td><td>Property</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Name of the Patch Configuration Property value</td></row>
		<row><td>ISPatchConfigurationProperty</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Value of the Patch Configuration Property</td></row>
		<row><td>ISPatchExternalFile</td><td>FileKey</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Filekey</td></row>
		<row><td>ISPatchExternalFile</td><td>FilePath</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Filepath</td></row>
		<row><td>ISPatchExternalFile</td><td>ISUpgradedImage_</td><td>N</td><td/><td/><td>ISUpgradedImage</td><td>1</td><td>Text</td><td/><td>Foreign key to the isupgraded image table</td></row>
		<row><td>ISPatchExternalFile</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Uniqu name to identify this record.</td></row>
		<row><td>ISPatchWholeFile</td><td>Component</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Component containing file key</td></row>
		<row><td>ISPatchWholeFile</td><td>FileKey</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Key of file to be included as whole</td></row>
		<row><td>ISPatchWholeFile</td><td>UpgradedImage</td><td>N</td><td/><td/><td>ISUpgradedImage</td><td>1</td><td>Text</td><td/><td>Foreign key to ISUpgradedImage Table</td></row>
		<row><td>ISPathVariable</td><td>ISPathVariable</td><td>N</td><td/><td/><td/><td/><td/><td/><td>The name of the path variable.</td></row>
		<row><td>ISPathVariable</td><td>TestValue</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The test value of the path variable.</td></row>
		<row><td>ISPathVariable</td><td>Type</td><td>N</td><td/><td/><td/><td/><td/><td>1;2;4;8</td><td>The type of the path variable.</td></row>
		<row><td>ISPathVariable</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The value of the path variable.</td></row>
		<row><td>ISProductConfiguration</td><td>GeneratePackageCode</td><td>Y</td><td/><td/><td/><td/><td>Number</td><td>0;1</td><td>Indicates whether or not to generate a package code.</td></row>
		<row><td>ISProductConfiguration</td><td>ISProductConfiguration</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The name of the product configuration.</td></row>
		<row><td>ISProductConfiguration</td><td>ProductConfigurationFlags</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Product configuration (release) flags.</td></row>
		<row><td>ISProductConfigurationInstance</td><td>ISProductConfiguration_</td><td>N</td><td/><td/><td>ISProductConfiguration</td><td>1</td><td>Text</td><td/><td>Foreign key into the ISProductConfiguration table.</td></row>
		<row><td>ISProductConfigurationInstance</td><td>InstanceId</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Identifies the instance number of this instance. This value is stored in the Property InstanceId.</td></row>
		<row><td>ISProductConfigurationInstance</td><td>Property</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Product Congiuration property name</td></row>
		<row><td>ISProductConfigurationInstance</td><td>Value</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>String value for property.</td></row>
		<row><td>ISProductConfigurationProperty</td><td>ISProductConfiguration_</td><td>N</td><td/><td/><td>ISProductConfiguration</td><td>1</td><td>Text</td><td/><td>Foreign key into the ISProductConfiguration table.</td></row>
		<row><td>ISProductConfigurationProperty</td><td>Property</td><td>N</td><td/><td/><td>Property</td><td>1</td><td>Text</td><td/><td>Product Congiuration property name</td></row>
		<row><td>ISProductConfigurationProperty</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>String value for property. Never null or empty.</td></row>
		<row><td>ISRelease</td><td>Attributes</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Bitfield holding boolean values for various release attributes.</td></row>
		<row><td>ISRelease</td><td>BuildLocation</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Build location.</td></row>
		<row><td>ISRelease</td><td>CDBrowser</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Demoshield browser location.</td></row>
		<row><td>ISRelease</td><td>DefaultLanguage</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Default language for setup.</td></row>
		<row><td>ISRelease</td><td>DigitalPVK</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Digital signing private key (.pvk) file.</td></row>
		<row><td>ISRelease</td><td>DigitalSPC</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Digital signing Software Publisher Certificate (.spc) file.</td></row>
		<row><td>ISRelease</td><td>DigitalURL</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Digital signing URL.</td></row>
		<row><td>ISRelease</td><td>DiskClusterSize</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Disk cluster size.</td></row>
		<row><td>ISRelease</td><td>DiskSize</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Disk size.</td></row>
		<row><td>ISRelease</td><td>DiskSizeUnit</td><td>N</td><td/><td/><td/><td/><td/><td>0;1;2</td><td>Disk size units (KB or MB).</td></row>
		<row><td>ISRelease</td><td>DiskSpanning</td><td>N</td><td/><td/><td/><td/><td/><td>0;1;2</td><td>Disk spanning (automatic, enforce size, etc.).</td></row>
		<row><td>ISRelease</td><td>DotNetBuildConfiguration</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Build Configuration for .NET solutions.</td></row>
		<row><td>ISRelease</td><td>ISProductConfiguration_</td><td>N</td><td/><td/><td>ISProductConfiguration</td><td>1</td><td>Text</td><td/><td>Foreign key into the ISProductConfiguration table.</td></row>
		<row><td>ISRelease</td><td>ISRelease</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The name of the release.</td></row>
		<row><td>ISRelease</td><td>ISSetupPrerequisiteLocation</td><td>Y</td><td/><td/><td/><td/><td/><td>0;1;2;3</td><td>Location the Setup Prerequisites will be placed in</td></row>
		<row><td>ISRelease</td><td>MediaLocation</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Media location on disk.</td></row>
		<row><td>ISRelease</td><td>MsiCommandLine</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Command line passed to the msi package from setup.exe</td></row>
		<row><td>ISRelease</td><td>MsiSourceType</td><td>N</td><td>-1</td><td>4</td><td/><td/><td/><td/><td>MSI media source type.</td></row>
		<row><td>ISRelease</td><td>PackageName</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Package name.</td></row>
		<row><td>ISRelease</td><td>Password</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Password.</td></row>
		<row><td>ISRelease</td><td>Platforms</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Platforms supported (Intel, Alpha, etc.).</td></row>
		<row><td>ISRelease</td><td>ReleaseFlags</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Release flags.</td></row>
		<row><td>ISRelease</td><td>ReleaseType</td><td>N</td><td/><td/><td/><td/><td/><td>1;2;4</td><td>Release type (single, uncompressed, etc.).</td></row>
		<row><td>ISRelease</td><td>SupportedLanguagesData</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Languages supported (for component filtering).</td></row>
		<row><td>ISRelease</td><td>SupportedLanguagesUI</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>UI languages supported.</td></row>
		<row><td>ISRelease</td><td>SupportedOSs</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Indicate which operating systmes are supported.</td></row>
		<row><td>ISRelease</td><td>SynchMsi</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>MSI file to synchronize file keys and other data with (patch-like functionality).</td></row>
		<row><td>ISRelease</td><td>Type</td><td>N</td><td>0</td><td>6</td><td/><td/><td/><td/><td>Release type (CDROM, Network, etc.).</td></row>
		<row><td>ISRelease</td><td>URLLocation</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Media location via URL.</td></row>
		<row><td>ISRelease</td><td>VersionCopyright</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Version stamp information.</td></row>
		<row><td>ISReleaseASPublishInfo</td><td>ISProductConfiguration_</td><td>N</td><td/><td/><td>ISProductConfiguration</td><td>1</td><td>Text</td><td/><td>Foreign key into the ISProductConfiguration table.</td></row>
		<row><td>ISReleaseASPublishInfo</td><td>ISRelease_</td><td>N</td><td/><td/><td>ISRelease</td><td>1</td><td>Text</td><td/><td>Foreign key into the ISRelease table.</td></row>
		<row><td>ISReleaseASPublishInfo</td><td>Property</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>AS Repository property name</td></row>
		<row><td>ISReleaseASPublishInfo</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>AS Repository property value</td></row>
		<row><td>ISReleaseExtended</td><td>Attributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Bitfield holding boolean values for various release attributes.</td></row>
		<row><td>ISReleaseExtended</td><td>CertPassword</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Digital certificate password</td></row>
		<row><td>ISReleaseExtended</td><td>DigitalCertificateDBaseNS</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Path to cerificate database for Netscape digital  signature</td></row>
		<row><td>ISReleaseExtended</td><td>DigitalCertificateIdNS</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Path to cerificate ID for Netscape digital  signature</td></row>
		<row><td>ISReleaseExtended</td><td>DigitalCertificatePasswordNS</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Password for Netscape digital  signature</td></row>
		<row><td>ISReleaseExtended</td><td>DotNetBaseLanguage</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Base Languge of .NET Redist</td></row>
		<row><td>ISReleaseExtended</td><td>DotNetFxCmdLine</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Command Line to pass to DotNetFx.exe</td></row>
		<row><td>ISReleaseExtended</td><td>DotNetLangPackCmdLine</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Command Line to pass to LangPack.exe</td></row>
		<row><td>ISReleaseExtended</td><td>DotNetLangaugePacks</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>.NET Redist language packs to include</td></row>
		<row><td>ISReleaseExtended</td><td>DotNetRedistLocation</td><td>Y</td><td>0</td><td>3</td><td/><td/><td/><td/><td>Location of .NET framework Redist (Web, SetupExe, Source, None)</td></row>
		<row><td>ISReleaseExtended</td><td>DotNetRedistURL</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>URL to .NET framework Redist</td></row>
		<row><td>ISReleaseExtended</td><td>DotNetVersion</td><td>Y</td><td>0</td><td>2</td><td/><td/><td/><td/><td>Version of .NET framework Redist (1.0, 1.1)</td></row>
		<row><td>ISReleaseExtended</td><td>EngineLocation</td><td>Y</td><td>0</td><td>2</td><td/><td/><td/><td/><td>Location of msi engine (Web, SetupExe...)</td></row>
		<row><td>ISReleaseExtended</td><td>ISEngineLocation</td><td>Y</td><td>0</td><td>2</td><td/><td/><td/><td/><td>Location of ISScript  engine (Web, SetupExe...)</td></row>
		<row><td>ISReleaseExtended</td><td>ISEngineURL</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>URL to InstallShield scripting engine</td></row>
		<row><td>ISReleaseExtended</td><td>ISProductConfiguration_</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Foreign key into the ISProductConfiguration table.</td></row>
		<row><td>ISReleaseExtended</td><td>ISRelease_</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The name of the release.</td></row>
		<row><td>ISReleaseExtended</td><td>JSharpCmdLine</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Command Line to pass to vjredist.exe</td></row>
		<row><td>ISReleaseExtended</td><td>JSharpRedistLocation</td><td>Y</td><td>0</td><td>3</td><td/><td/><td/><td/><td>Location of J# framework Redist (Web, SetupExe, Source, None)</td></row>
		<row><td>ISReleaseExtended</td><td>MsiEngineVersion</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Bitfield holding selected MSI engine versions included in this release</td></row>
		<row><td>ISReleaseExtended</td><td>OneClickCabName</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>File name of generated cabfile</td></row>
		<row><td>ISReleaseExtended</td><td>OneClickHtmlName</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>File name of generated html page</td></row>
		<row><td>ISReleaseExtended</td><td>OneClickTargetBrowser</td><td>Y</td><td>0</td><td>2</td><td/><td/><td/><td/><td>Target browser (IE, Netscape, both...)</td></row>
		<row><td>ISReleaseExtended</td><td>WebCabSize</td><td>Y</td><td>0</td><td>2147483647</td><td/><td/><td/><td/><td>Size of the cabfile</td></row>
		<row><td>ISReleaseExtended</td><td>WebLocalCachePath</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Directory to cache downloaded package</td></row>
		<row><td>ISReleaseExtended</td><td>WebType</td><td>Y</td><td>0</td><td>2</td><td/><td/><td/><td/><td>Type of web install (One Executable, Downloader...)</td></row>
		<row><td>ISReleaseExtended</td><td>WebURL</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>URL to .msi package</td></row>
		<row><td>ISReleaseExtended</td><td>Win9xMsiUrl</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>URL to Ansi MSI engine</td></row>
		<row><td>ISReleaseExtended</td><td>WinMsi30Url</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>URL to MSI 3.0 engine</td></row>
		<row><td>ISReleaseExtended</td><td>WinNTMsiUrl</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>URL to Unicode MSI engine</td></row>
		<row><td>ISReleaseProperty</td><td>ISProductConfiguration_</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Foreign key into ISProductConfiguration table.</td></row>
		<row><td>ISReleaseProperty</td><td>ISRelease_</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Foreign key into ISRelease table.</td></row>
		<row><td>ISReleaseProperty</td><td>Name</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Property name</td></row>
		<row><td>ISReleaseProperty</td><td>Value</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Property value</td></row>
		<row><td>ISReleasePublishInfo</td><td>Description</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Repository item description</td></row>
		<row><td>ISReleasePublishInfo</td><td>DisplayName</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Repository item display name</td></row>
		<row><td>ISReleasePublishInfo</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Bitfield holding various attributes</td></row>
		<row><td>ISReleasePublishInfo</td><td>ISProductConfiguration_</td><td>N</td><td/><td/><td>ISProductConfiguration</td><td>1</td><td>Text</td><td/><td>Foreign key into the ISProductConfiguration table.</td></row>
		<row><td>ISReleasePublishInfo</td><td>ISRelease_</td><td>N</td><td/><td/><td>ISRelease</td><td>1</td><td>Text</td><td/><td>The name of the release.</td></row>
		<row><td>ISReleasePublishInfo</td><td>Publisher</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Repository item publisher</td></row>
		<row><td>ISReleasePublishInfo</td><td>Repository</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Repository which to  publish the built merge module</td></row>
		<row><td>ISSQLConnection</td><td>Attributes</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLConnection</td><td>Authentication</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLConnection</td><td>BatchSeparator</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLConnection</td><td>CmdTimeout</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLConnection</td><td>Comments</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLConnection</td><td>Database</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLConnection</td><td>ISSQLConnection</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key used to identify a particular ISSQLConnection record.</td></row>
		<row><td>ISSQLConnection</td><td>Order</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLConnection</td><td>Password</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLConnection</td><td>ScriptVersion_Column</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLConnection</td><td>ScriptVersion_Table</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLConnection</td><td>Server</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLConnection</td><td>UserName</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLConnectionDBServer</td><td>ISSQLConnectionDBServer</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key used to identify a particular ISSQLConnectionDBServer record.</td></row>
		<row><td>ISSQLConnectionDBServer</td><td>ISSQLConnection_</td><td>N</td><td/><td/><td>ISSQLConnection</td><td>1</td><td>Identifier</td><td/><td>Foreign key into ISSQLConnection table.</td></row>
		<row><td>ISSQLConnectionDBServer</td><td>ISSQLDBMetaData_</td><td>N</td><td/><td/><td>ISSQLDBMetaData</td><td>1</td><td>Identifier</td><td/><td>Foreign key into ISSQLDBMetaData table.</td></row>
		<row><td>ISSQLConnectionDBServer</td><td>Order</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLConnectionScript</td><td>ISSQLConnection_</td><td>N</td><td/><td/><td>ISSQLConnection</td><td>1</td><td>Identifier</td><td/><td>Foreign key into ISSQLConnection table.</td></row>
		<row><td>ISSQLConnectionScript</td><td>ISSQLScriptFile_</td><td>N</td><td/><td/><td>ISSQLScriptFile</td><td>1</td><td>Identifier</td><td/><td>Foreign key into ISSQLScriptFile table.</td></row>
		<row><td>ISSQLConnectionScript</td><td>Order</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>AdoCxnAdditional</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>AdoCxnDatabase</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>AdoCxnDriver</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>AdoCxnNetLibrary</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>AdoCxnPassword</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>AdoCxnPort</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>AdoCxnServer</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>AdoCxnUserID</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>AdoCxnWindowsSecurity</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>AdoDriverName</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>CreateDbCmd</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>CreateTableCmd</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>DisplayName</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>DsnODBCName</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>ISSQLDBMetaData</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key used to identify a particular ISSQLDBMetaData record.</td></row>
		<row><td>ISSQLDBMetaData</td><td>InsertRecordCmd</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>LocalInstanceNames</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>QueryDatabasesCmd</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>ScriptVersion_Column</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>ScriptVersion_ColumnType</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>ScriptVersion_Table</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>SelectTableCmd</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>SwitchDbCmd</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>TestDatabaseCmd</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>TestTableCmd</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>TestTableCmd2</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>VersionBeginToken</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>VersionEndToken</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>VersionInfoCmd</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLDBMetaData</td><td>WinAuthentUserId</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLRequirement</td><td>Attributes</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLRequirement</td><td>ISSQLConnectionDBServer_</td><td>Y</td><td/><td/><td>ISSQLConnectionDBServer</td><td>1</td><td>Identifier</td><td/><td>Foreign key into ISSQLConnectionDBServer table.</td></row>
		<row><td>ISSQLRequirement</td><td>ISSQLConnection_</td><td>N</td><td/><td/><td>ISSQLConnection</td><td>1</td><td>Identifier</td><td/><td>Foreign key into ISSQLConnection table.</td></row>
		<row><td>ISSQLRequirement</td><td>ISSQLRequirement</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key used to identify a particular ISSQLRequirement record.</td></row>
		<row><td>ISSQLRequirement</td><td>MajorVersion</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLRequirement</td><td>ServicePackLevel</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLScriptError</td><td>Attributes</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLScriptError</td><td>ErrHandling</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLScriptError</td><td>ErrNumber</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLScriptError</td><td>ISSQLScriptFile_</td><td>Y</td><td/><td/><td>ISSQLScriptFile</td><td>1</td><td>Identifier</td><td/><td>Foreign key into ISSQLScriptFile table</td></row>
		<row><td>ISSQLScriptError</td><td>Message</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Custom end-user message. Reserved for future use.</td></row>
		<row><td>ISSQLScriptFile</td><td>Attributes</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLScriptFile</td><td>Comments</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Comments</td></row>
		<row><td>ISSQLScriptFile</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key referencing Component that controls the SQL script.</td></row>
		<row><td>ISSQLScriptFile</td><td>Condition</td><td>Y</td><td/><td/><td/><td/><td>Condition</td><td/><td>A conditional statement that will disable this script if the specified condition evaluates to the 'False' state. If a script is disabled, it will not be installed regardless of the 'Action' state associated with the component.</td></row>
		<row><td>ISSQLScriptFile</td><td>DisplayName</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Display name for the SQL script file.</td></row>
		<row><td>ISSQLScriptFile</td><td>ErrorHandling</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLScriptFile</td><td>ISBuildSourcePath</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Full path, the category is of Text instead of Path because of potential use of path variables.</td></row>
		<row><td>ISSQLScriptFile</td><td>ISSQLScriptFile</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>This is the primary key to the ISSQLScriptFile table</td></row>
		<row><td>ISSQLScriptFile</td><td>InstallText</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Feedback end-user text at install</td></row>
		<row><td>ISSQLScriptFile</td><td>Scheduling</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLScriptFile</td><td>UninstallText</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Feedback end-user text at Uninstall</td></row>
		<row><td>ISSQLScriptFile</td><td>Version</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Schema Version (#####.#####.#####.#####)</td></row>
		<row><td>ISSQLScriptImport</td><td>Attributes</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLScriptImport</td><td>Authentication</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLScriptImport</td><td>Database</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLScriptImport</td><td>ExcludeTables</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLScriptImport</td><td>ISSQLScriptFile_</td><td>N</td><td/><td/><td>ISSQLScriptFile</td><td>1</td><td>Identifier</td><td/><td>Foreign key into ISSQLScriptFile table.</td></row>
		<row><td>ISSQLScriptImport</td><td>IncludeTables</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLScriptImport</td><td>Password</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLScriptImport</td><td>Server</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLScriptImport</td><td>UserName</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLScriptReplace</td><td>Attributes</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLScriptReplace</td><td>ISSQLScriptFile_</td><td>N</td><td/><td/><td>ISSQLScriptFile</td><td>1</td><td>Identifier</td><td/><td>Foreign key into ISSQLScriptFile table.</td></row>
		<row><td>ISSQLScriptReplace</td><td>ISSQLScriptReplace</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key used to identify a particular ISSQLScriptReplace record.</td></row>
		<row><td>ISSQLScriptReplace</td><td>Replace</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSQLScriptReplace</td><td>Search</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISScriptFile</td><td>ISScriptFile</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>This is the full path of the script file. The path portion may be expressed in path variable form.</td></row>
		<row><td>ISSelfReg</td><td>CmdLine</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSelfReg</td><td>Cost</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSelfReg</td><td>FileKey</td><td>N</td><td/><td/><td>File</td><td>1</td><td>Identifier</td><td/><td>Foreign key to the file table</td></row>
		<row><td>ISSelfReg</td><td>Order</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSetupFile</td><td>FileName</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>This is the file name to use when streaming the file to the support files location</td></row>
		<row><td>ISSetupFile</td><td>ISSetupFile</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>This is the primary key to the ISSetupFile table</td></row>
		<row><td>ISSetupFile</td><td>Language</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Four digit language identifier.  0 for Language Neutral</td></row>
		<row><td>ISSetupFile</td><td>Path</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Link to the source file on the build machine</td></row>
		<row><td>ISSetupFile</td><td>Splash</td><td>Y</td><td/><td/><td/><td/><td>Short</td><td/><td>Boolean value indication whether his setup file entry belongs in the Splasc Screen section</td></row>
		<row><td>ISSetupFile</td><td>Stream</td><td>Y</td><td/><td/><td/><td/><td>Binary</td><td/><td>Binary stream. The bits to stream to the support location</td></row>
		<row><td>ISSetupPrerequisites</td><td>ISBuildSourcePath</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSetupPrerequisites</td><td>ISReleaseFlags</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Release Flags that specify whether this prereq  will be included in a particular release.</td></row>
		<row><td>ISSetupPrerequisites</td><td>ISSetupLocation</td><td>Y</td><td/><td/><td/><td/><td/><td>0;1;2</td><td/></row>
		<row><td>ISSetupPrerequisites</td><td>ISSetupPrerequisites</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSetupPrerequisites</td><td>Order</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISSetupType</td><td>Comments</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>User Comments.</td></row>
		<row><td>ISSetupType</td><td>Description</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Longer descriptive text describing a visible feature item.</td></row>
		<row><td>ISSetupType</td><td>Display</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Numeric sort order, used to force a specific display ordering.</td></row>
		<row><td>ISSetupType</td><td>Display_Name</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>A string used to set the initial text contained within a control (if appropriate).</td></row>
		<row><td>ISSetupType</td><td>ISSetupType</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key used to identify a particular feature record.</td></row>
		<row><td>ISSetupTypeFeatures</td><td>Feature_</td><td>N</td><td/><td/><td>Feature</td><td>1</td><td>Identifier</td><td/><td>Foreign key into Feature table.</td></row>
		<row><td>ISSetupTypeFeatures</td><td>ISSetupType_</td><td>N</td><td/><td/><td>ISSetupType</td><td>1</td><td>Identifier</td><td/><td>Foreign key into ISSetupType table.</td></row>
		<row><td>ISStorages</td><td>ISBuildSourcePath</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Path to the file to stream into sub-storage</td></row>
		<row><td>ISStorages</td><td>Name</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Name of the sub-storage key</td></row>
		<row><td>ISString</td><td>Comment</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Comment</td></row>
		<row><td>ISString</td><td>Encoded</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Encoding for multi-byte strings.</td></row>
		<row><td>ISString</td><td>ISLanguage_</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>This is a foreign key to the ISLanguage table.</td></row>
		<row><td>ISString</td><td>ISString</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>String id.</td></row>
		<row><td>ISString</td><td>TimeStamp</td><td>Y</td><td/><td/><td/><td/><td>Time/Date</td><td/><td>Time Stamp. MSI's Time/Date column type is just an int, with bits packed in a certain order.</td></row>
		<row><td>ISString</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>real string value.</td></row>
		<row><td>ISSwidtagProperty</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Property name</td></row>
		<row><td>ISSwidtagProperty</td><td>Value</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Property value</td></row>
		<row><td>ISTargetImage</td><td>Flags</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>relative order of the target image</td></row>
		<row><td>ISTargetImage</td><td>IgnoreMissingFiles</td><td>N</td><td/><td/><td/><td/><td/><td/><td>If true, ignore missing source files when creating patch</td></row>
		<row><td>ISTargetImage</td><td>MsiPath</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Path to the target image</td></row>
		<row><td>ISTargetImage</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of the TargetImage</td></row>
		<row><td>ISTargetImage</td><td>Order</td><td>N</td><td/><td/><td/><td/><td/><td/><td>relative order of the target image</td></row>
		<row><td>ISTargetImage</td><td>UpgradedImage_</td><td>N</td><td/><td/><td>ISUpgradedImage</td><td>1</td><td>Text</td><td/><td>foreign key to the upgraded Image table</td></row>
		<row><td>ISUpgradeMsiItem</td><td>ISAttributes</td><td>N</td><td/><td/><td/><td/><td/><td>0;1</td><td/></row>
		<row><td>ISUpgradeMsiItem</td><td>ISReleaseFlags</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>ISUpgradeMsiItem</td><td>ObjectSetupPath</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The path to the setup you want to upgrade.</td></row>
		<row><td>ISUpgradeMsiItem</td><td>UpgradeItem</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The name of the Upgrade Item.</td></row>
		<row><td>ISUpgradedImage</td><td>Family</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Name of the image family</td></row>
		<row><td>ISUpgradedImage</td><td>MsiPath</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Path to the upgraded image</td></row>
		<row><td>ISUpgradedImage</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of the UpgradedImage</td></row>
		<row><td>ISVirtualDirectory</td><td>Directory_</td><td>N</td><td/><td/><td>Directory</td><td>1</td><td>Identifier</td><td/><td>Foreign key into Directory table.</td></row>
		<row><td>ISVirtualDirectory</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Property name</td></row>
		<row><td>ISVirtualDirectory</td><td>Value</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Property value</td></row>
		<row><td>ISVirtualFile</td><td>File_</td><td>N</td><td/><td/><td>File</td><td>1</td><td>Identifier</td><td/><td>Foreign key into File  table.</td></row>
		<row><td>ISVirtualFile</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Property name</td></row>
		<row><td>ISVirtualFile</td><td>Value</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Property value</td></row>
		<row><td>ISVirtualPackage</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Property name</td></row>
		<row><td>ISVirtualPackage</td><td>Value</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Property value</td></row>
		<row><td>ISVirtualRegistry</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Property name</td></row>
		<row><td>ISVirtualRegistry</td><td>Registry_</td><td>N</td><td/><td/><td>Registry</td><td>1</td><td>Identifier</td><td/><td>Foreign key into Registry table.</td></row>
		<row><td>ISVirtualRegistry</td><td>Value</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Property value</td></row>
		<row><td>ISVirtualRelease</td><td>ISProductConfiguration_</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Foreign key into ISProductConfiguration table.</td></row>
		<row><td>ISVirtualRelease</td><td>ISRelease_</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Foreign key into ISRelease table.</td></row>
		<row><td>ISVirtualRelease</td><td>Name</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Property name</td></row>
		<row><td>ISVirtualRelease</td><td>Value</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Property value</td></row>
		<row><td>ISVirtualShortcut</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Property name</td></row>
		<row><td>ISVirtualShortcut</td><td>Shortcut_</td><td>N</td><td/><td/><td>Shortcut</td><td>1</td><td>Identifier</td><td/><td>Foreign key into Shortcut table.</td></row>
		<row><td>ISVirtualShortcut</td><td>Value</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Property value</td></row>
		<row><td>ISWSEWrap</td><td>Action_</td><td>N</td><td/><td/><td>CustomAction</td><td>1</td><td>Identifier</td><td/><td>Foreign key into CustomAction table</td></row>
		<row><td>ISWSEWrap</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Property associated with this Action</td></row>
		<row><td>ISWSEWrap</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Value associated with this Property</td></row>
		<row><td>ISXmlElement</td><td>Content</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Element contents</td></row>
		<row><td>ISXmlElement</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td>Number</td><td/><td>Internal XML element attributes</td></row>
		<row><td>ISXmlElement</td><td>ISXmlElement</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key, non-localized, internal token for Xml element</td></row>
		<row><td>ISXmlElement</td><td>ISXmlElement_Parent</td><td>Y</td><td/><td/><td>ISXmlElement</td><td>1</td><td>Identifier</td><td/><td>Foreign key into ISXMLElement table.</td></row>
		<row><td>ISXmlElement</td><td>ISXmlFile_</td><td>N</td><td/><td/><td>ISXmlFile</td><td>1</td><td>Identifier</td><td/><td>Foreign key into XmlFile table.</td></row>
		<row><td>ISXmlElement</td><td>XPath</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>XPath fragment including any operators</td></row>
		<row><td>ISXmlElementAttrib</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td>Number</td><td/><td>Internal XML elementattib attributes</td></row>
		<row><td>ISXmlElementAttrib</td><td>ISXmlElementAttrib</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key, non-localized, internal token for Xml element attribute</td></row>
		<row><td>ISXmlElementAttrib</td><td>ISXmlElement_</td><td>N</td><td/><td/><td>ISXmlElement</td><td>1</td><td>Identifier</td><td/><td>Foreign key into ISXMLElement table.</td></row>
		<row><td>ISXmlElementAttrib</td><td>Name</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Localized attribute name</td></row>
		<row><td>ISXmlElementAttrib</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Localized attribute value</td></row>
		<row><td>ISXmlFile</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key into Component table.</td></row>
		<row><td>ISXmlFile</td><td>Directory</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Foreign key into Directory table.</td></row>
		<row><td>ISXmlFile</td><td>Encoding</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>XML File Encoding</td></row>
		<row><td>ISXmlFile</td><td>FileName</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Localized XML file name</td></row>
		<row><td>ISXmlFile</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td>Number</td><td/><td>Internal XML file attributes</td></row>
		<row><td>ISXmlFile</td><td>ISXmlFile</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key, non-localized,internal token for Xml file</td></row>
		<row><td>ISXmlFile</td><td>SelectionNamespaces</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Selection namespaces</td></row>
		<row><td>ISXmlLocator</td><td>Attribute</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>The name of an attribute within the XML element.</td></row>
		<row><td>ISXmlLocator</td><td>Element</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>XPath query that will locate an element in an XML file.</td></row>
		<row><td>ISXmlLocator</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td>0;1;2</td><td/></row>
		<row><td>ISXmlLocator</td><td>Parent</td><td>Y</td><td/><td/><td/><td/><td>Identifier</td><td/><td>The parent file signature. It is also a foreign key in the Signature table.</td></row>
		<row><td>ISXmlLocator</td><td>Signature_</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>The Signature_ represents a unique file signature and is also the foreign key in the Signature,  RegLocator, IniLocator, ISXmlLocator, CompLocator and the DrLocator tables.</td></row>
		<row><td>Icon</td><td>Data</td><td>Y</td><td/><td/><td/><td/><td>Binary</td><td/><td>Binary stream. The binary icon data in PE (.DLL or .EXE) or icon (.ICO) format.</td></row>
		<row><td>Icon</td><td>ISBuildSourcePath</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Full path to the ICO or EXE file.</td></row>
		<row><td>Icon</td><td>ISIconIndex</td><td>Y</td><td>-32767</td><td>32767</td><td/><td/><td/><td/><td>Optional icon index to be extracted.</td></row>
		<row><td>Icon</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key. Name of the icon file.</td></row>
		<row><td>IniFile</td><td>Action</td><td>N</td><td/><td/><td/><td/><td/><td>0;1;3</td><td>The type of modification to be made, one of iifEnum</td></row>
		<row><td>IniFile</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the Component table referencing component that controls the installing of the .INI value.</td></row>
		<row><td>IniFile</td><td>DirProperty</td><td>Y</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Foreign key into the Directory table denoting the directory where the .INI file is.</td></row>
		<row><td>IniFile</td><td>FileName</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The .INI file name in which to write the information</td></row>
		<row><td>IniFile</td><td>IniFile</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key, non-localized token.</td></row>
		<row><td>IniFile</td><td>Key</td><td>N</td><td/><td/><td/><td/><td>Formatted</td><td/><td>The .INI file key below Section.</td></row>
		<row><td>IniFile</td><td>Section</td><td>N</td><td/><td/><td/><td/><td>Formatted</td><td/><td>The .INI file Section.</td></row>
		<row><td>IniFile</td><td>Value</td><td>N</td><td/><td/><td/><td/><td>Formatted</td><td/><td>The value to be written.</td></row>
		<row><td>IniLocator</td><td>Field</td><td>Y</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>The field in the .INI line. If Field is null or 0 the entire line is read.</td></row>
		<row><td>IniLocator</td><td>FileName</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The .INI file name.</td></row>
		<row><td>IniLocator</td><td>Key</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Key value (followed by an equals sign in INI file).</td></row>
		<row><td>IniLocator</td><td>Section</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Section name within in file (within square brackets in INI file).</td></row>
		<row><td>IniLocator</td><td>Signature_</td><td>N</td><td/><td/><td>Signature</td><td>1</td><td>Identifier</td><td/><td>The table key. The Signature_ represents a unique file signature and is also the foreign key in the Signature table.</td></row>
		<row><td>IniLocator</td><td>Type</td><td>Y</td><td>0</td><td>2</td><td/><td/><td/><td/><td>An integer value that determines if the .INI value read is a filename or a directory location or to be used as is w/o interpretation.</td></row>
		<row><td>InstallExecuteSequence</td><td>Action</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of action to invoke, either in the engine or the handler DLL.</td></row>
		<row><td>InstallExecuteSequence</td><td>Condition</td><td>Y</td><td/><td/><td/><td/><td>Condition</td><td/><td>Optional expression which skips the action if evaluates to expFalse.If the expression syntax is invalid, the engine will terminate, returning iesBadActionData.</td></row>
		<row><td>InstallExecuteSequence</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>This is used to store MM Custom Action Types</td></row>
		<row><td>InstallExecuteSequence</td><td>ISComments</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Author’s comments on this Sequence.</td></row>
		<row><td>InstallExecuteSequence</td><td>Sequence</td><td>Y</td><td>-4</td><td>32767</td><td/><td/><td/><td/><td>Number that determines the sort order in which the actions are to be executed.  Leave blank to suppress action.</td></row>
		<row><td>InstallShield</td><td>Property</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of property, uppercase if settable by launcher or loader.</td></row>
		<row><td>InstallShield</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>String value for property.</td></row>
		<row><td>InstallUISequence</td><td>Action</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of action to invoke, either in the engine or the handler DLL.</td></row>
		<row><td>InstallUISequence</td><td>Condition</td><td>Y</td><td/><td/><td/><td/><td>Condition</td><td/><td>Optional expression which skips the action if evaluates to expFalse.If the expression syntax is invalid, the engine will terminate, returning iesBadActionData.</td></row>
		<row><td>InstallUISequence</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>This is used to store MM Custom Action Types</td></row>
		<row><td>InstallUISequence</td><td>ISComments</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Author’s comments on this Sequence.</td></row>
		<row><td>InstallUISequence</td><td>Sequence</td><td>Y</td><td>-4</td><td>32767</td><td/><td/><td/><td/><td>Number that determines the sort order in which the actions are to be executed.  Leave blank to suppress action.</td></row>
		<row><td>IsolatedComponent</td><td>Component_Application</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Key to Component table item for application</td></row>
		<row><td>IsolatedComponent</td><td>Component_Shared</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Key to Component table item to be isolated</td></row>
		<row><td>LaunchCondition</td><td>Condition</td><td>N</td><td/><td/><td/><td/><td>Condition</td><td/><td>Expression which must evaluate to TRUE in order for install to commence.</td></row>
		<row><td>LaunchCondition</td><td>Description</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Localizable text to display when condition fails and install must abort.</td></row>
		<row><td>ListBox</td><td>Order</td><td>N</td><td>1</td><td>32767</td><td/><td/><td/><td/><td>A positive integer used to determine the ordering of the items within one list..The integers do not have to be consecutive.</td></row>
		<row><td>ListBox</td><td>Property</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>A named property to be tied to this item. All the items tied to the same property become part of the same listbox.</td></row>
		<row><td>ListBox</td><td>Text</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The visible text to be assigned to the item. Optional. If this entry or the entire column is missing, the text is the same as the value.</td></row>
		<row><td>ListBox</td><td>Value</td><td>N</td><td/><td/><td/><td/><td>Formatted</td><td/><td>The value string associated with this item. Selecting the line will set the associated property to this value.</td></row>
		<row><td>ListView</td><td>Binary_</td><td>Y</td><td/><td/><td>Binary</td><td>1</td><td>Identifier</td><td/><td>The name of the icon to be displayed with the icon. The binary information is looked up from the Binary Table.</td></row>
		<row><td>ListView</td><td>Order</td><td>N</td><td>1</td><td>32767</td><td/><td/><td/><td/><td>A positive integer used to determine the ordering of the items within one list..The integers do not have to be consecutive.</td></row>
		<row><td>ListView</td><td>Property</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>A named property to be tied to this item. All the items tied to the same property become part of the same listview.</td></row>
		<row><td>ListView</td><td>Text</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The visible text to be assigned to the item. Optional. If this entry or the entire column is missing, the text is the same as the value.</td></row>
		<row><td>ListView</td><td>Value</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The value string associated with this item. Selecting the line will set the associated property to this value.</td></row>
		<row><td>LockPermissions</td><td>Domain</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Domain name for user whose permissions are being set. (usually a property)</td></row>
		<row><td>LockPermissions</td><td>LockObject</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Foreign key into Registry or File table</td></row>
		<row><td>LockPermissions</td><td>Permission</td><td>Y</td><td>-2147483647</td><td>2147483647</td><td/><td/><td/><td/><td>Permission Access mask.  Full Control = 268435456 (GENERIC_ALL = 0x10000000)</td></row>
		<row><td>LockPermissions</td><td>Table</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td>Directory;File;Registry</td><td>Reference to another table name</td></row>
		<row><td>LockPermissions</td><td>User</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>User for permissions to be set.  (usually a property)</td></row>
		<row><td>MIME</td><td>CLSID</td><td>Y</td><td/><td/><td>Class</td><td>1</td><td>Guid</td><td/><td>Optional associated CLSID.</td></row>
		<row><td>MIME</td><td>ContentType</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Primary key. Context identifier, typically "type/format".</td></row>
		<row><td>MIME</td><td>Extension_</td><td>N</td><td/><td/><td>Extension</td><td>1</td><td>Text</td><td/><td>Optional associated extension (without dot)</td></row>
		<row><td>Media</td><td>Cabinet</td><td>Y</td><td/><td/><td/><td/><td>Cabinet</td><td/><td>If some or all of the files stored on the media are compressed in a cabinet, the name of that cabinet.</td></row>
		<row><td>Media</td><td>DiskId</td><td>N</td><td>1</td><td>32767</td><td/><td/><td/><td/><td>Primary key, integer to determine sort order for table.</td></row>
		<row><td>Media</td><td>DiskPrompt</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Disk name: the visible text actually printed on the disk.  This will be used to prompt the user when this disk needs to be inserted.</td></row>
		<row><td>Media</td><td>LastSequence</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>File sequence number for the last file for this media.</td></row>
		<row><td>Media</td><td>Source</td><td>Y</td><td/><td/><td/><td/><td>Property</td><td/><td>The property defining the location of the cabinet file.</td></row>
		<row><td>Media</td><td>VolumeLabel</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The label attributed to the volume.</td></row>
		<row><td>MoveFile</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>If this component is not "selected" for installation or removal, no action will be taken on the associated MoveFile entry</td></row>
		<row><td>MoveFile</td><td>DestFolder</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of a property whose value is assumed to resolve to the full path to the destination directory</td></row>
		<row><td>MoveFile</td><td>DestName</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Name to be given to the original file after it is moved or copied.  If blank, the destination file will be given the same name as the source file</td></row>
		<row><td>MoveFile</td><td>FileKey</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key that uniquely identifies a particular MoveFile record</td></row>
		<row><td>MoveFile</td><td>Options</td><td>N</td><td>0</td><td>1</td><td/><td/><td/><td/><td>Integer value specifying the MoveFile operating mode, one of imfoEnum</td></row>
		<row><td>MoveFile</td><td>SourceFolder</td><td>Y</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of a property whose value is assumed to resolve to the full path to the source directory</td></row>
		<row><td>MoveFile</td><td>SourceName</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Name of the source file(s) to be moved or copied.  Can contain the '*' or '?' wildcards.</td></row>
		<row><td>MsiAssembly</td><td>Attributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Assembly attributes</td></row>
		<row><td>MsiAssembly</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key into Component table.</td></row>
		<row><td>MsiAssembly</td><td>Feature_</td><td>N</td><td/><td/><td>Feature</td><td>1</td><td>Identifier</td><td/><td>Foreign key into Feature table.</td></row>
		<row><td>MsiAssembly</td><td>File_Application</td><td>Y</td><td/><td/><td>File</td><td>1</td><td>Identifier</td><td/><td>Foreign key into File table, denoting the application context for private assemblies. Null for global assemblies.</td></row>
		<row><td>MsiAssembly</td><td>File_Manifest</td><td>Y</td><td/><td/><td>File</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the File table denoting the manifest file for the assembly.</td></row>
		<row><td>MsiAssemblyName</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key into Component table.</td></row>
		<row><td>MsiAssemblyName</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The name part of the name-value pairs for the assembly name.</td></row>
		<row><td>MsiAssemblyName</td><td>Value</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The value part of the name-value pairs for the assembly name.</td></row>
		<row><td>MsiDigitalCertificate</td><td>CertData</td><td>N</td><td/><td/><td/><td/><td>Binary</td><td/><td>A certificate context blob for a signer certificate</td></row>
		<row><td>MsiDigitalCertificate</td><td>DigitalCertificate</td><td>N</td><td/><td/><td>MsiPackageCertificate</td><td>2</td><td>Identifier</td><td/><td>A unique identifier for the row</td></row>
		<row><td>MsiDigitalSignature</td><td>DigitalCertificate_</td><td>N</td><td/><td/><td>MsiDigitalCertificate</td><td>1</td><td>Identifier</td><td/><td>Foreign key to MsiDigitalCertificate table identifying the signer certificate</td></row>
		<row><td>MsiDigitalSignature</td><td>Hash</td><td>Y</td><td/><td/><td/><td/><td>Binary</td><td/><td>The encoded hash blob from the digital signature</td></row>
		<row><td>MsiDigitalSignature</td><td>SignObject</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Foreign key to Media table</td></row>
		<row><td>MsiDigitalSignature</td><td>Table</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Reference to another table name (only Media table is supported)</td></row>
		<row><td>MsiDriverPackages</td><td>Component</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Primary key used to identify a particular component record.</td></row>
		<row><td>MsiDriverPackages</td><td>Flags</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Driver package flags</td></row>
		<row><td>MsiDriverPackages</td><td>ReferenceComponents</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>MsiDriverPackages</td><td>Sequence</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>Installation sequence number</td></row>
		<row><td>MsiEmbeddedChainer</td><td>CommandLine</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td/></row>
		<row><td>MsiEmbeddedChainer</td><td>Condition</td><td>Y</td><td/><td/><td/><td/><td>Condition</td><td/><td/></row>
		<row><td>MsiEmbeddedChainer</td><td>MsiEmbeddedChainer</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td/></row>
		<row><td>MsiEmbeddedChainer</td><td>Source</td><td>N</td><td/><td/><td/><td/><td>CustomSource</td><td/><td/></row>
		<row><td>MsiEmbeddedChainer</td><td>Type</td><td>Y</td><td/><td/><td/><td/><td>Integer</td><td>2;18;50</td><td/></row>
		<row><td>MsiEmbeddedUI</td><td>Attributes</td><td>N</td><td>0</td><td>3</td><td/><td/><td>Integer</td><td/><td>Information about the data in the Data column.</td></row>
		<row><td>MsiEmbeddedUI</td><td>Data</td><td>Y</td><td/><td/><td/><td/><td>Binary</td><td/><td>This column contains binary information.</td></row>
		<row><td>MsiEmbeddedUI</td><td>FileName</td><td>N</td><td/><td/><td/><td/><td>Filename</td><td/><td>The name of the file that receives the binary information in the Data column.</td></row>
		<row><td>MsiEmbeddedUI</td><td>ISBuildSourcePath</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td/></row>
		<row><td>MsiEmbeddedUI</td><td>MessageFilter</td><td>Y</td><td>0</td><td>234913791</td><td/><td/><td>Integer</td><td/><td>Specifies the types of messages that are sent to the user interface DLL. This column is only relevant for rows with the msidbEmbeddedUI attribute.</td></row>
		<row><td>MsiEmbeddedUI</td><td>MsiEmbeddedUI</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>The primary key for the table.</td></row>
		<row><td>MsiFileHash</td><td>File_</td><td>N</td><td/><td/><td>File</td><td>1</td><td>Identifier</td><td/><td>Primary key, foreign key into File table referencing file with this hash</td></row>
		<row><td>MsiFileHash</td><td>HashPart1</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Size of file in bytes (long integer).</td></row>
		<row><td>MsiFileHash</td><td>HashPart2</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Size of file in bytes (long integer).</td></row>
		<row><td>MsiFileHash</td><td>HashPart3</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Size of file in bytes (long integer).</td></row>
		<row><td>MsiFileHash</td><td>HashPart4</td><td>N</td><td/><td/><td/><td/><td/><td/><td>Size of file in bytes (long integer).</td></row>
		<row><td>MsiFileHash</td><td>Options</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Various options and attributes for this hash.</td></row>
		<row><td>MsiLockPermissionsEx</td><td>Condition</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>Expression which must evaluate to TRUE in order for this set of permissions to be applied</td></row>
		<row><td>MsiLockPermissionsEx</td><td>LockObject</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Foreign key into Registry, File, CreateFolder, or ServiceInstall table</td></row>
		<row><td>MsiLockPermissionsEx</td><td>MsiLockPermissionsEx</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key, non-localized token</td></row>
		<row><td>MsiLockPermissionsEx</td><td>SDDLText</td><td>N</td><td/><td/><td/><td/><td>FormattedSDDLText</td><td/><td>String to indicate permissions to be applied to the LockObject</td></row>
		<row><td>MsiLockPermissionsEx</td><td>Table</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td>CreateFolder;File;Registry;ServiceInstall</td><td>Reference to another table name</td></row>
		<row><td>MsiPackageCertificate</td><td>DigitalCertificate_</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>A foreign key to the digital certificate table</td></row>
		<row><td>MsiPackageCertificate</td><td>PackageCertificate</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>A unique identifier for the row</td></row>
		<row><td>MsiPatchCertificate</td><td>DigitalCertificate_</td><td>N</td><td/><td/><td>MsiDigitalCertificate</td><td>1</td><td>Identifier</td><td/><td>A foreign key to the digital certificate table</td></row>
		<row><td>MsiPatchCertificate</td><td>PatchCertificate</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>A unique identifier for the row</td></row>
		<row><td>MsiPatchMetadata</td><td>Company</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Optional company name</td></row>
		<row><td>MsiPatchMetadata</td><td>PatchConfiguration_</td><td>N</td><td/><td/><td>ISPatchConfiguration</td><td>1</td><td>Text</td><td/><td>Foreign key to the ISPatchConfiguration table</td></row>
		<row><td>MsiPatchMetadata</td><td>Property</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Name of the metadata</td></row>
		<row><td>MsiPatchMetadata</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Value of the metadata</td></row>
		<row><td>MsiPatchOldAssemblyFile</td><td>Assembly_</td><td>Y</td><td/><td/><td>MsiPatchOldAssemblyName</td><td>1</td><td/><td/><td/></row>
		<row><td>MsiPatchOldAssemblyFile</td><td>File_</td><td>N</td><td/><td/><td>File</td><td>1</td><td/><td/><td/></row>
		<row><td>MsiPatchOldAssemblyName</td><td>Assembly</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>MsiPatchOldAssemblyName</td><td>Name</td><td>N</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>MsiPatchOldAssemblyName</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td/><td/><td/></row>
		<row><td>MsiPatchSequence</td><td>PatchConfiguration_</td><td>N</td><td/><td/><td>ISPatchConfiguration</td><td>1</td><td>Text</td><td/><td>Foreign key to the patch configuration table</td></row>
		<row><td>MsiPatchSequence</td><td>PatchFamily</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Name of the family to which this patch belongs</td></row>
		<row><td>MsiPatchSequence</td><td>Sequence</td><td>N</td><td/><td/><td/><td/><td>Version</td><td/><td>The version of this patch in this family</td></row>
		<row><td>MsiPatchSequence</td><td>Supersede</td><td>N</td><td/><td/><td/><td/><td>Integer</td><td/><td>Supersede</td></row>
		<row><td>MsiPatchSequence</td><td>Target</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Target product codes for this patch family</td></row>
		<row><td>MsiServiceConfig</td><td>Argument</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Argument(s) for service configuration. Value depends on the content of the ConfigType field</td></row>
		<row><td>MsiServiceConfig</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Required foreign key into the Component Table that controls the configuration of the service</td></row>
		<row><td>MsiServiceConfig</td><td>ConfigType</td><td>N</td><td>-2147483647</td><td>2147483647</td><td/><td/><td/><td/><td>Service Configuration Option</td></row>
		<row><td>MsiServiceConfig</td><td>Event</td><td>N</td><td>0</td><td>7</td><td/><td/><td/><td/><td>Bit field:   0x1 = Install, 0x2 = Uninstall, 0x4 = Reinstall</td></row>
		<row><td>MsiServiceConfig</td><td>MsiServiceConfig</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key, non-localized token.</td></row>
		<row><td>MsiServiceConfig</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Formatted</td><td/><td>Name of a service. /, \, comma and space are invalid</td></row>
		<row><td>MsiServiceConfigFailureActions</td><td>Actions</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>A list of integer actions separated by [~] delimiters: 0 = SC_ACTION_NONE, 1 = SC_ACTION_RESTART, 2 = SC_ACTION_REBOOT, 3 = SC_ACTION_RUN_COMMAND. Terminate with [~][~]</td></row>
		<row><td>MsiServiceConfigFailureActions</td><td>Command</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>Command line of the process to CreateProcess function to execute</td></row>
		<row><td>MsiServiceConfigFailureActions</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Required foreign key into the Component Table that controls the configuration of the service</td></row>
		<row><td>MsiServiceConfigFailureActions</td><td>DelayActions</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>A list of delays (time in milli-seconds), separated by [~] delmiters, to wait before taking the corresponding Action. Terminate with [~][~]</td></row>
		<row><td>MsiServiceConfigFailureActions</td><td>Event</td><td>N</td><td>0</td><td>7</td><td/><td/><td/><td/><td>Bit field:   0x1 = Install, 0x2 = Uninstall, 0x4 = Reinstall</td></row>
		<row><td>MsiServiceConfigFailureActions</td><td>MsiServiceConfigFailureActions</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key, non-localized token</td></row>
		<row><td>MsiServiceConfigFailureActions</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Formatted</td><td/><td>Name of a service. /, \, comma and space are invalid</td></row>
		<row><td>MsiServiceConfigFailureActions</td><td>RebootMessage</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>Message to be broadcast to server users before rebooting</td></row>
		<row><td>MsiServiceConfigFailureActions</td><td>ResetPeriod</td><td>Y</td><td>0</td><td>2147483647</td><td/><td/><td/><td/><td>Time in seconds after which to reset the failure count to zero. Leave blank if it should never be reset</td></row>
		<row><td>MsiShortcutProperty</td><td>MsiShortcutProperty</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key, non-localized token</td></row>
		<row><td>MsiShortcutProperty</td><td>PropVariantValue</td><td>N</td><td/><td/><td/><td/><td>Formatted</td><td/><td>String representation of the value in the property</td></row>
		<row><td>MsiShortcutProperty</td><td>PropertyKey</td><td>N</td><td/><td/><td/><td/><td>Formatted</td><td/><td>Canonical string representation of the Property Key being set</td></row>
		<row><td>MsiShortcutProperty</td><td>Shortcut_</td><td>N</td><td/><td/><td>Shortcut</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the Shortcut table</td></row>
		<row><td>ODBCAttribute</td><td>Attribute</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Name of ODBC driver attribute</td></row>
		<row><td>ODBCAttribute</td><td>Driver_</td><td>N</td><td/><td/><td>ODBCDriver</td><td>1</td><td>Identifier</td><td/><td>Reference to ODBC driver in ODBCDriver table</td></row>
		<row><td>ODBCAttribute</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Value for ODBC driver attribute</td></row>
		<row><td>ODBCDataSource</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Reference to associated component</td></row>
		<row><td>ODBCDataSource</td><td>DataSource</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key, non-localized.internal token for data source</td></row>
		<row><td>ODBCDataSource</td><td>Description</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Text used as registered name for data source</td></row>
		<row><td>ODBCDataSource</td><td>DriverDescription</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Reference to driver description, may be existing driver</td></row>
		<row><td>ODBCDataSource</td><td>Registration</td><td>N</td><td>0</td><td>1</td><td/><td/><td/><td/><td>Registration option: 0=machine, 1=user, others t.b.d.</td></row>
		<row><td>ODBCDriver</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Reference to associated component</td></row>
		<row><td>ODBCDriver</td><td>Description</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Text used as registered name for driver, non-localized</td></row>
		<row><td>ODBCDriver</td><td>Driver</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key, non-localized.internal token for driver</td></row>
		<row><td>ODBCDriver</td><td>File_</td><td>N</td><td/><td/><td>File</td><td>1</td><td>Identifier</td><td/><td>Reference to key driver file</td></row>
		<row><td>ODBCDriver</td><td>File_Setup</td><td>Y</td><td/><td/><td>File</td><td>1</td><td>Identifier</td><td/><td>Optional reference to key driver setup DLL</td></row>
		<row><td>ODBCSourceAttribute</td><td>Attribute</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Name of ODBC data source attribute</td></row>
		<row><td>ODBCSourceAttribute</td><td>DataSource_</td><td>N</td><td/><td/><td>ODBCDataSource</td><td>1</td><td>Identifier</td><td/><td>Reference to ODBC data source in ODBCDataSource table</td></row>
		<row><td>ODBCSourceAttribute</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Value for ODBC data source attribute</td></row>
		<row><td>ODBCTranslator</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Reference to associated component</td></row>
		<row><td>ODBCTranslator</td><td>Description</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>Text used as registered name for translator</td></row>
		<row><td>ODBCTranslator</td><td>File_</td><td>N</td><td/><td/><td>File</td><td>1</td><td>Identifier</td><td/><td>Reference to key translator file</td></row>
		<row><td>ODBCTranslator</td><td>File_Setup</td><td>Y</td><td/><td/><td>File</td><td>1</td><td>Identifier</td><td/><td>Optional reference to key translator setup DLL</td></row>
		<row><td>ODBCTranslator</td><td>Translator</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key, non-localized.internal token for translator</td></row>
		<row><td>Patch</td><td>Attributes</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Integer containing bit flags representing patch attributes</td></row>
		<row><td>Patch</td><td>File_</td><td>N</td><td/><td/><td>File</td><td>1</td><td>Identifier</td><td/><td>Primary key, non-localized token, foreign key to File table, must match identifier in cabinet.</td></row>
		<row><td>Patch</td><td>Header</td><td>Y</td><td/><td/><td/><td/><td>Binary</td><td/><td>Binary stream. The patch header, used for patch validation.</td></row>
		<row><td>Patch</td><td>ISBuildSourcePath</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Full path to patch header.</td></row>
		<row><td>Patch</td><td>PatchSize</td><td>N</td><td>0</td><td>2147483647</td><td/><td/><td/><td/><td>Size of patch in bytes (long integer).</td></row>
		<row><td>Patch</td><td>Sequence</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Primary key, sequence with respect to the media images; order must track cabinet order.</td></row>
		<row><td>Patch</td><td>StreamRef_</td><td>Y</td><td/><td/><td/><td/><td>Identifier</td><td/><td>External key into the MsiPatchHeaders table specifying the row that contains the patch header stream.</td></row>
		<row><td>PatchPackage</td><td>Media_</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Foreign key to DiskId column of Media table. Indicates the disk containing the patch package.</td></row>
		<row><td>PatchPackage</td><td>PatchId</td><td>N</td><td/><td/><td/><td/><td>Guid</td><td/><td>A unique string GUID representing this patch.</td></row>
		<row><td>ProgId</td><td>Class_</td><td>Y</td><td/><td/><td>Class</td><td>1</td><td>Guid</td><td/><td>The CLSID of an OLE factory corresponding to the ProgId.</td></row>
		<row><td>ProgId</td><td>Description</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Localized description for the Program identifier.</td></row>
		<row><td>ProgId</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>This is used to store Installshield custom properties of a component, like ExtractIcon, etc.</td></row>
		<row><td>ProgId</td><td>IconIndex</td><td>Y</td><td>-32767</td><td>32767</td><td/><td/><td/><td/><td>Optional icon index.</td></row>
		<row><td>ProgId</td><td>Icon_</td><td>Y</td><td/><td/><td>Icon</td><td>1</td><td>Identifier</td><td/><td>Optional foreign key into the Icon Table, specifying the icon file associated with this ProgId. Will be written under the DefaultIcon key.</td></row>
		<row><td>ProgId</td><td>ProgId</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The Program Identifier. Primary key.</td></row>
		<row><td>ProgId</td><td>ProgId_Parent</td><td>Y</td><td/><td/><td>ProgId</td><td>1</td><td>Text</td><td/><td>The Parent Program Identifier. If specified, the ProgId column becomes a version independent prog id.</td></row>
		<row><td>Property</td><td>ISComments</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>User Comments.</td></row>
		<row><td>Property</td><td>Property</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of property, uppercase if settable by launcher or loader.</td></row>
		<row><td>Property</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>String value for property.</td></row>
		<row><td>PublishComponent</td><td>AppData</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>This is localisable Application specific data that can be associated with a Qualified Component.</td></row>
		<row><td>PublishComponent</td><td>ComponentId</td><td>N</td><td/><td/><td/><td/><td>Guid</td><td/><td>A string GUID that represents the component id that will be requested by the alien product.</td></row>
		<row><td>PublishComponent</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the Component table.</td></row>
		<row><td>PublishComponent</td><td>Feature_</td><td>N</td><td/><td/><td>Feature</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the Feature table.</td></row>
		<row><td>PublishComponent</td><td>Qualifier</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>This is defined only when the ComponentId column is an Qualified Component Id. This is the Qualifier for ProvideComponentIndirect.</td></row>
		<row><td>RadioButton</td><td>Height</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>The height of the button.</td></row>
		<row><td>RadioButton</td><td>Help</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The help strings used with the button. The text is optional.</td></row>
		<row><td>RadioButton</td><td>ISControlId</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>A number used to represent the control ID of the Control, Used in Dialog export</td></row>
		<row><td>RadioButton</td><td>Order</td><td>N</td><td>1</td><td>32767</td><td/><td/><td/><td/><td>A positive integer used to determine the ordering of the items within one list..The integers do not have to be consecutive.</td></row>
		<row><td>RadioButton</td><td>Property</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>A named property to be tied to this radio button. All the buttons tied to the same property become part of the same group.</td></row>
		<row><td>RadioButton</td><td>Text</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The visible title to be assigned to the radio button.</td></row>
		<row><td>RadioButton</td><td>Value</td><td>N</td><td/><td/><td/><td/><td>Formatted</td><td/><td>The value string associated with this button. Selecting the button will set the associated property to this value.</td></row>
		<row><td>RadioButton</td><td>Width</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>The width of the button.</td></row>
		<row><td>RadioButton</td><td>X</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>The horizontal coordinate of the upper left corner of the bounding rectangle of the radio button.</td></row>
		<row><td>RadioButton</td><td>Y</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>The vertical coordinate of the upper left corner of the bounding rectangle of the radio button.</td></row>
		<row><td>RegLocator</td><td>Key</td><td>N</td><td/><td/><td/><td/><td>RegPath</td><td/><td>The key for the registry value.</td></row>
		<row><td>RegLocator</td><td>Name</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>The registry value name.</td></row>
		<row><td>RegLocator</td><td>Root</td><td>N</td><td>0</td><td>3</td><td/><td/><td/><td/><td>The predefined root key for the registry value, one of rrkEnum.</td></row>
		<row><td>RegLocator</td><td>Signature_</td><td>N</td><td/><td/><td>Signature</td><td>1</td><td>Identifier</td><td/><td>The table key. The Signature_ represents a unique file signature and is also the foreign key in the Signature table. If the type is 0, the registry values refers a directory, and _Signature is not a foreign key.</td></row>
		<row><td>RegLocator</td><td>Type</td><td>Y</td><td>0</td><td>18</td><td/><td/><td/><td/><td>An integer value that determines if the registry value is a filename or a directory location or to be used as is w/o interpretation.</td></row>
		<row><td>Registry</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the Component table referencing component that controls the installing of the registry value.</td></row>
		<row><td>Registry</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>This is used to store Installshield custom properties of a registry item.  Currently the only one is Automatic.</td></row>
		<row><td>Registry</td><td>Key</td><td>N</td><td/><td/><td/><td/><td>RegPath</td><td/><td>The key for the registry value.</td></row>
		<row><td>Registry</td><td>Name</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>The registry value name.</td></row>
		<row><td>Registry</td><td>Registry</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key, non-localized token.</td></row>
		<row><td>Registry</td><td>Root</td><td>N</td><td>-1</td><td>3</td><td/><td/><td/><td/><td>The predefined root key for the registry value, one of rrkEnum.</td></row>
		<row><td>Registry</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The registry value.</td></row>
		<row><td>RemoveFile</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key referencing Component that controls the file to be removed.</td></row>
		<row><td>RemoveFile</td><td>DirProperty</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of a property whose value is assumed to resolve to the full pathname to the folder of the file to be removed.</td></row>
		<row><td>RemoveFile</td><td>FileKey</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key used to identify a particular file entry</td></row>
		<row><td>RemoveFile</td><td>FileName</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Name of the file to be removed.</td></row>
		<row><td>RemoveFile</td><td>InstallMode</td><td>N</td><td/><td/><td/><td/><td/><td>1;2;3</td><td>Installation option, one of iimEnum.</td></row>
		<row><td>RemoveIniFile</td><td>Action</td><td>N</td><td/><td/><td/><td/><td/><td>2;4</td><td>The type of modification to be made, one of iifEnum.</td></row>
		<row><td>RemoveIniFile</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the Component table referencing component that controls the deletion of the .INI value.</td></row>
		<row><td>RemoveIniFile</td><td>DirProperty</td><td>Y</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Foreign key into the Directory table denoting the directory where the .INI file is.</td></row>
		<row><td>RemoveIniFile</td><td>FileName</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The .INI file name in which to delete the information</td></row>
		<row><td>RemoveIniFile</td><td>Key</td><td>N</td><td/><td/><td/><td/><td>Formatted</td><td/><td>The .INI file key below Section.</td></row>
		<row><td>RemoveIniFile</td><td>RemoveIniFile</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key, non-localized token.</td></row>
		<row><td>RemoveIniFile</td><td>Section</td><td>N</td><td/><td/><td/><td/><td>Formatted</td><td/><td>The .INI file Section.</td></row>
		<row><td>RemoveIniFile</td><td>Value</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>The value to be deleted. The value is required when Action is iifIniRemoveTag</td></row>
		<row><td>RemoveRegistry</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the Component table referencing component that controls the deletion of the registry value.</td></row>
		<row><td>RemoveRegistry</td><td>Key</td><td>N</td><td/><td/><td/><td/><td>RegPath</td><td/><td>The key for the registry value.</td></row>
		<row><td>RemoveRegistry</td><td>Name</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>The registry value name.</td></row>
		<row><td>RemoveRegistry</td><td>RemoveRegistry</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key, non-localized token.</td></row>
		<row><td>RemoveRegistry</td><td>Root</td><td>N</td><td>-1</td><td>3</td><td/><td/><td/><td/><td>The predefined root key for the registry value, one of rrkEnum</td></row>
		<row><td>ReserveCost</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Reserve a specified amount of space if this component is to be installed.</td></row>
		<row><td>ReserveCost</td><td>ReserveFolder</td><td>Y</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of a property whose value is assumed to resolve to the full path to the destination directory</td></row>
		<row><td>ReserveCost</td><td>ReserveKey</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key that uniquely identifies a particular ReserveCost record</td></row>
		<row><td>ReserveCost</td><td>ReserveLocal</td><td>N</td><td>0</td><td>2147483647</td><td/><td/><td/><td/><td>Disk space to reserve if linked component is installed locally.</td></row>
		<row><td>ReserveCost</td><td>ReserveSource</td><td>N</td><td>0</td><td>2147483647</td><td/><td/><td/><td/><td>Disk space to reserve if linked component is installed to run from the source location.</td></row>
		<row><td>SFPCatalog</td><td>Catalog</td><td>Y</td><td/><td/><td/><td/><td>Binary</td><td/><td>SFP Catalog</td></row>
		<row><td>SFPCatalog</td><td>Dependency</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>Parent catalog - only used by SFP</td></row>
		<row><td>SFPCatalog</td><td>SFPCatalog</td><td>N</td><td/><td/><td/><td/><td>Filename</td><td/><td>File name for the catalog.</td></row>
		<row><td>SelfReg</td><td>Cost</td><td>Y</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>The cost of registering the module.</td></row>
		<row><td>SelfReg</td><td>File_</td><td>N</td><td/><td/><td>File</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the File table denoting the module that needs to be registered.</td></row>
		<row><td>ServiceControl</td><td>Arguments</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>Arguments for the service.  Separate by [~].</td></row>
		<row><td>ServiceControl</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Required foreign key into the Component Table that controls the startup of the service</td></row>
		<row><td>ServiceControl</td><td>Event</td><td>N</td><td>0</td><td>187</td><td/><td/><td/><td/><td>Bit field:  Install:  0x1 = Start, 0x2 = Stop, 0x8 = Delete, Uninstall: 0x10 = Start, 0x20 = Stop, 0x80 = Delete</td></row>
		<row><td>ServiceControl</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Formatted</td><td/><td>Name of a service. /, \, comma and space are invalid</td></row>
		<row><td>ServiceControl</td><td>ServiceControl</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key, non-localized token.</td></row>
		<row><td>ServiceControl</td><td>Wait</td><td>Y</td><td>0</td><td>1</td><td/><td/><td/><td/><td>Boolean for whether to wait for the service to fully start</td></row>
		<row><td>ServiceInstall</td><td>Arguments</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>Arguments to include in every start of the service, passed to WinMain</td></row>
		<row><td>ServiceInstall</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Required foreign key into the Component Table that controls the startup of the service</td></row>
		<row><td>ServiceInstall</td><td>Dependencies</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>Other services this depends on to start.  Separate by [~], and end with [~][~]</td></row>
		<row><td>ServiceInstall</td><td>Description</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Description of service.</td></row>
		<row><td>ServiceInstall</td><td>DisplayName</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>External Name of the Service</td></row>
		<row><td>ServiceInstall</td><td>ErrorControl</td><td>N</td><td>-2147483647</td><td>2147483647</td><td/><td/><td/><td/><td>Severity of error if service fails to start</td></row>
		<row><td>ServiceInstall</td><td>LoadOrderGroup</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>LoadOrderGroup</td></row>
		<row><td>ServiceInstall</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Formatted</td><td/><td>Internal Name of the Service</td></row>
		<row><td>ServiceInstall</td><td>Password</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>password to run service with.  (with StartName)</td></row>
		<row><td>ServiceInstall</td><td>ServiceInstall</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key, non-localized token.</td></row>
		<row><td>ServiceInstall</td><td>ServiceType</td><td>N</td><td>-2147483647</td><td>2147483647</td><td/><td/><td/><td/><td>Type of the service</td></row>
		<row><td>ServiceInstall</td><td>StartName</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>User or object name to run service as</td></row>
		<row><td>ServiceInstall</td><td>StartType</td><td>N</td><td>0</td><td>4</td><td/><td/><td/><td/><td>Type of the service</td></row>
		<row><td>Shortcut</td><td>Arguments</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>The command-line arguments for the shortcut.</td></row>
		<row><td>Shortcut</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the Component table denoting the component whose selection gates the the shortcut creation/deletion.</td></row>
		<row><td>Shortcut</td><td>Description</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The description for the shortcut.</td></row>
		<row><td>Shortcut</td><td>DescriptionResourceDLL</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>This field contains a Formatted string value for the full path to the language neutral file that contains the MUI manifest.</td></row>
		<row><td>Shortcut</td><td>DescriptionResourceId</td><td>Y</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>The description name index for the shortcut.</td></row>
		<row><td>Shortcut</td><td>Directory_</td><td>N</td><td/><td/><td>Directory</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the Directory table denoting the directory where the shortcut file is created.</td></row>
		<row><td>Shortcut</td><td>DisplayResourceDLL</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>This field contains a Formatted string value for the full path to the language neutral file that contains the MUI manifest.</td></row>
		<row><td>Shortcut</td><td>DisplayResourceId</td><td>Y</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>The display name index for the shortcut.</td></row>
		<row><td>Shortcut</td><td>Hotkey</td><td>Y</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>The hotkey for the shortcut. It has the virtual-key code for the key in the low-order byte, and the modifier flags in the high-order byte.</td></row>
		<row><td>Shortcut</td><td>ISAttributes</td><td>Y</td><td/><td/><td/><td/><td/><td/><td>This is used to store Installshield custom properties of a shortcut.  Mainly used in pro project types.</td></row>
		<row><td>Shortcut</td><td>ISComments</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Author’s comments on this Shortcut.</td></row>
		<row><td>Shortcut</td><td>ISShortcutName</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>A non-unique name for the shortcut.  Mainly used by pro pro project types.</td></row>
		<row><td>Shortcut</td><td>IconIndex</td><td>Y</td><td>-32767</td><td>32767</td><td/><td/><td/><td/><td>The icon index for the shortcut.</td></row>
		<row><td>Shortcut</td><td>Icon_</td><td>Y</td><td/><td/><td>Icon</td><td>1</td><td>Identifier</td><td/><td>Foreign key into the File table denoting the external icon file for the shortcut.</td></row>
		<row><td>Shortcut</td><td>Name</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The name of the shortcut to be created.</td></row>
		<row><td>Shortcut</td><td>Shortcut</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Primary key, non-localized token.</td></row>
		<row><td>Shortcut</td><td>ShowCmd</td><td>Y</td><td/><td/><td/><td/><td/><td>1;3;7</td><td>The show command for the application window.The following values may be used.</td></row>
		<row><td>Shortcut</td><td>Target</td><td>N</td><td/><td/><td/><td/><td>Shortcut</td><td/><td>The shortcut target. This is usually a property that is expanded to a file or a folder that the shortcut points to.</td></row>
		<row><td>Shortcut</td><td>WkDir</td><td>Y</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of property defining location of working directory.</td></row>
		<row><td>Signature</td><td>FileName</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The name of the file. This may contain a "short name|long name" pair.</td></row>
		<row><td>Signature</td><td>Languages</td><td>Y</td><td/><td/><td/><td/><td>Language</td><td/><td>The languages supported by the file.</td></row>
		<row><td>Signature</td><td>MaxDate</td><td>Y</td><td>0</td><td>2147483647</td><td/><td/><td/><td/><td>The maximum creation date of the file.</td></row>
		<row><td>Signature</td><td>MaxSize</td><td>Y</td><td>0</td><td>2147483647</td><td/><td/><td/><td/><td>The maximum size of the file.</td></row>
		<row><td>Signature</td><td>MaxVersion</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The maximum version of the file.</td></row>
		<row><td>Signature</td><td>MinDate</td><td>Y</td><td>0</td><td>2147483647</td><td/><td/><td/><td/><td>The minimum creation date of the file.</td></row>
		<row><td>Signature</td><td>MinSize</td><td>Y</td><td>0</td><td>2147483647</td><td/><td/><td/><td/><td>The minimum size of the file.</td></row>
		<row><td>Signature</td><td>MinVersion</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The minimum version of the file.</td></row>
		<row><td>Signature</td><td>Signature</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>The table key. The Signature represents a unique file signature.</td></row>
		<row><td>TextStyle</td><td>Color</td><td>Y</td><td>0</td><td>16777215</td><td/><td/><td/><td/><td>A long integer indicating the color of the string in the RGB format (Red, Green, Blue each 0-255, RGB = R + 256*G + 256^2*B).</td></row>
		<row><td>TextStyle</td><td>FaceName</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>A string indicating the name of the font used. Required. The string must be at most 31 characters long.</td></row>
		<row><td>TextStyle</td><td>Size</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>The size of the font used. This size is given in our units (1/12 of the system font height). Assuming that the system font is set to 12 point size, this is equivalent to the point size.</td></row>
		<row><td>TextStyle</td><td>StyleBits</td><td>Y</td><td>0</td><td>15</td><td/><td/><td/><td/><td>A combination of style bits.</td></row>
		<row><td>TextStyle</td><td>TextStyle</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of the style. The primary key of this table. This name is embedded in the texts to indicate a style change.</td></row>
		<row><td>TypeLib</td><td>Component_</td><td>N</td><td/><td/><td>Component</td><td>1</td><td>Identifier</td><td/><td>Required foreign key into the Component Table, specifying the component for which to return a path when called through LocateComponent.</td></row>
		<row><td>TypeLib</td><td>Cost</td><td>Y</td><td>0</td><td>2147483647</td><td/><td/><td/><td/><td>The cost associated with the registration of the typelib. This column is currently optional.</td></row>
		<row><td>TypeLib</td><td>Description</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td/></row>
		<row><td>TypeLib</td><td>Directory_</td><td>Y</td><td/><td/><td>Directory</td><td>1</td><td>Identifier</td><td/><td>Optional. The foreign key into the Directory table denoting the path to the help file for the type library.</td></row>
		<row><td>TypeLib</td><td>Feature_</td><td>N</td><td/><td/><td>Feature</td><td>1</td><td>Identifier</td><td/><td>Required foreign key into the Feature Table, specifying the feature to validate or install in order for the type library to be operational.</td></row>
		<row><td>TypeLib</td><td>Language</td><td>N</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>The language of the library.</td></row>
		<row><td>TypeLib</td><td>LibID</td><td>N</td><td/><td/><td/><td/><td>Guid</td><td/><td>The GUID that represents the library.</td></row>
		<row><td>TypeLib</td><td>Version</td><td>Y</td><td>0</td><td>2147483647</td><td/><td/><td/><td/><td>The version of the library. The major version is in the upper 8 bits of the short integer. The minor version is in the lower 8 bits.</td></row>
		<row><td>UIText</td><td>Key</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>A unique key that identifies the particular string.</td></row>
		<row><td>UIText</td><td>Text</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The localized version of the string.</td></row>
		<row><td>Upgrade</td><td>ActionProperty</td><td>N</td><td/><td/><td/><td/><td>UpperCase</td><td/><td>The property to set when a product in this set is found.</td></row>
		<row><td>Upgrade</td><td>Attributes</td><td>N</td><td>0</td><td>2147483647</td><td/><td/><td/><td/><td>The attributes of this product set.</td></row>
		<row><td>Upgrade</td><td>ISDisplayName</td><td>Y</td><td/><td/><td>ISUpgradeMsiItem</td><td>1</td><td/><td/><td/></row>
		<row><td>Upgrade</td><td>Language</td><td>Y</td><td/><td/><td/><td/><td>Language</td><td/><td>A comma-separated list of languages for either products in this set or products not in this set.</td></row>
		<row><td>Upgrade</td><td>Remove</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>The list of features to remove when uninstalling a product from this set.  The default is "ALL".</td></row>
		<row><td>Upgrade</td><td>UpgradeCode</td><td>N</td><td/><td/><td/><td/><td>Guid</td><td/><td>The UpgradeCode GUID belonging to the products in this set.</td></row>
		<row><td>Upgrade</td><td>VersionMax</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The maximum ProductVersion of the products in this set.  The set may or may not include products with this particular version.</td></row>
		<row><td>Upgrade</td><td>VersionMin</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>The minimum ProductVersion of the products in this set.  The set may or may not include products with this particular version.</td></row>
		<row><td>Verb</td><td>Argument</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>Optional value for the command arguments.</td></row>
		<row><td>Verb</td><td>Command</td><td>Y</td><td/><td/><td/><td/><td>Formatted</td><td/><td>The command text.</td></row>
		<row><td>Verb</td><td>Extension_</td><td>N</td><td/><td/><td>Extension</td><td>1</td><td>Text</td><td/><td>The extension associated with the table row.</td></row>
		<row><td>Verb</td><td>Sequence</td><td>Y</td><td>0</td><td>32767</td><td/><td/><td/><td/><td>Order within the verbs for a particular extension. Also used simply to specify the default verb.</td></row>
		<row><td>Verb</td><td>Verb</td><td>N</td><td/><td/><td/><td/><td>Text</td><td/><td>The verb for the command.</td></row>
		<row><td>_Validation</td><td>Category</td><td>Y</td><td/><td/><td/><td/><td/><td>"Text";"Formatted";"Template";"Condition";"Guid";"Path";"Version";"Language";"Identifier";"Binary";"UpperCase";"LowerCase";"Filename";"Paths";"AnyPath";"WildCardFilename";"RegPath";"KeyFormatted";"CustomSource";"Property";"Cabinet";"Shortcut";"URL";"DefaultDir"</td><td>String category</td></row>
		<row><td>_Validation</td><td>Column</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of column</td></row>
		<row><td>_Validation</td><td>Description</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Description of column</td></row>
		<row><td>_Validation</td><td>KeyColumn</td><td>Y</td><td>1</td><td>32</td><td/><td/><td/><td/><td>Column to which foreign key connects</td></row>
		<row><td>_Validation</td><td>KeyTable</td><td>Y</td><td/><td/><td/><td/><td>Identifier</td><td/><td>For foreign key, Name of table to which data must link</td></row>
		<row><td>_Validation</td><td>MaxValue</td><td>Y</td><td>-2147483647</td><td>2147483647</td><td/><td/><td/><td/><td>Maximum value allowed</td></row>
		<row><td>_Validation</td><td>MinValue</td><td>Y</td><td>-2147483647</td><td>2147483647</td><td/><td/><td/><td/><td>Minimum value allowed</td></row>
		<row><td>_Validation</td><td>Nullable</td><td>N</td><td/><td/><td/><td/><td/><td>Y;N;@</td><td>Whether the column is nullable</td></row>
		<row><td>_Validation</td><td>Set</td><td>Y</td><td/><td/><td/><td/><td>Text</td><td/><td>Set of values that are permitted</td></row>
		<row><td>_Validation</td><td>Table</td><td>N</td><td/><td/><td/><td/><td>Identifier</td><td/><td>Name of table</td></row>
	</table>
</msi>
