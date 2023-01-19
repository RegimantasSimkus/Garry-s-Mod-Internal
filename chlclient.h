#pragma once
#include "ibaseclientdll.h"
#include "clientclass.h"
#include "qangle.h"

class CHLClient : public IBaseClientDLL
{
public:
	CHLClient();

	virtual int						Init(void* appSystemFactory, void* physicsFactory, void* pGlobals);

	virtual void					PostInit();
	virtual void					Shutdown(void);

	virtual bool					ReplayInit(void* fnReplayFactory);
	virtual bool					ReplayPostInit();

	virtual void					LevelInitPreEntity(const char* pMapName);
	virtual void					LevelInitPostEntity();
	virtual void					LevelShutdown(void);

	virtual ClientClass* GetAllClasses(void);

	virtual int						HudVidInit(void);
	virtual void					HudProcessInput(bool bActive);
	virtual void					HudUpdate(bool bActive);
	virtual void					HudReset(void);
	virtual void					HudText(const char* message);

	// Mouse Input Interfaces
	virtual void					IN_ActivateMouse(void);
	virtual void					IN_DeactivateMouse(void);
	virtual void					IN_Accumulate(void);
	virtual void					IN_ClearStates(void);
	virtual bool					IN_IsKeyDown(const char* name, bool& isdown);
	virtual void					IN_OnMouseWheeled(int nDelta);
	// Raw signal
	virtual int						IN_KeyEvent(int eventcode, void* keynum, const char* pszCurrentBinding);
	virtual void					IN_SetSampleTime(float frametime);
	// Create movement command
	virtual void					CreateMove(int sequence_number, float input_sample_frametime, bool active);
	virtual void					ExtraMouseSample(float frametime, bool active);
	virtual bool					WriteUsercmdDeltaToBuffer(void* buf, int from, int to, bool isnewcommand);
	virtual void					EncodeUserCmdToBuffer(void*& buf, int slot);
	virtual void					DecodeUserCmdFromBuffer(void*& buf, int slot);


	virtual void					View_Render(void* rect);
	virtual void					RenderView(const void*& view, int nClearFlags, int whatToDraw);
	virtual void					View_Fade(void* pSF);

	virtual void					SetCrosshairAngle(const QAngle& angle);

	virtual void					InitSprite(void** pSprite, const char* loadname);
	virtual void					ShutdownSprite(void** pSprite);

	virtual int						GetSpriteSize(void) const;

	virtual void					VoiceStatus(int entindex, bool bTalking);

	virtual void					InstallStringTableCallback(const char* tableName);

	virtual void					FrameStageNotify(void* curStage);

	virtual bool					DispatchUserMessage(int msg_type, void*& msg_data);

	// Save/restore system hooks
	virtual void** SaveInit(int size);
	virtual void			SaveWriteFields(void**, const char*, void*, void**, void**, int);
	virtual void			SaveReadFields(void**, const char*, void*, void**, void**, int);
	virtual void			PreSave(void**);
	virtual void			Save(void**);
	virtual void			WriteSaveHeaders(void**);
	virtual void			ReadRestoreHeaders(void**);
	virtual void			Restore(void**, bool);
	virtual void			DispatchOnRestore();
	virtual void			WriteSaveGameScreenshot(const char* pFilename);

	// Given a list of "S(wavname) S(wavname2)" tokens, look up the localized text and emit
	//  the appropriate close caption if running with closecaption = 1
	virtual void			EmitSentenceCloseCaption(char const* tokenstream);
	virtual void			EmitCloseCaption(char const* captionname, float duration);

	virtual void* GetStandardRecvProxies();

	virtual bool			CanRecordDemo(char* errorMsg, int length) const;

	virtual void			OnDemoRecordStart(char const* pDemoBaseName);
	virtual void			OnDemoRecordStop();
	virtual void			OnDemoPlaybackStart(char const* pDemoBaseName);
	virtual void			OnDemoPlaybackStop();

	virtual bool			ShouldDrawDropdownConsole();

	// Get client screen dimensions
	virtual int				GetScreenWidth();
	virtual int				GetScreenHeight();

	// save game screenshot writing
	virtual void			WriteSaveGameScreenshotOfSize(const char* pFilename, int width, int height, bool bCreatePowerOf2Padded/*=false*/, bool bWriteVTF/*=false*/);

	// Gets the location of the player viewpoint
	virtual bool			GetPlayerView(void*& playerView);

	// Matchmaking
	virtual void			SetupGameProperties(void*& contexts, void*& properties);
	virtual unsigned int			GetPresenceID(const char* pIDName);
	virtual const char* GetPropertyIdString(const unsigned int id);
	virtual void			GetPropertyDisplayString(unsigned int id, unsigned int value, char* pOutput, int nBytes);
	virtual void			StartStatsReporting(void* handle, bool bArbitrated);

	virtual void			InvalidateMdlCache();

	virtual void			ReloadFilesInList(void** pFilesToReload);

	// Let the client handle UI toggle - if this function returns false, the UI will toggle, otherwise it will not.
	virtual bool			HandleUiToggle();

	// Allow the console to be shown?
	virtual bool			ShouldAllowConsole();

	// Get renamed recv tables
	virtual void* GetRenamedRecvTableInfos();

	// Get the mouthinfo for the sound being played inside UI panels
	virtual void* GetClientUIMouthInfo();

	// Notify the client that a file has been received from the game server
	virtual void			FileReceived(const char* fileName, unsigned int transferID);

	virtual const char* TranslateEffectForVisionFilter(const char* pchEffectType, const char* pchEffectName);

	virtual void			ClientAdjustStartSoundParams(struct StartSoundParams_t& params);

	// Returns true if the disconnect command has been handled by the client
	virtual bool DisconnectAttempt(void);
public:
	void PrecacheMaterial(const char* pMaterialName);

	virtual bool IsConnectedUserInfoChangeAllowed(void* pCvar);

private:
	void UncacheAllMaterials();
	void ResetStringTablePointers();

};
