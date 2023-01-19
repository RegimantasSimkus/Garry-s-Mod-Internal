#pragma once

class ClientModeShared
{
public:
	ClientModeShared();
	virtual			~ClientModeShared();

	virtual void	Init();
	virtual void	InitViewport();
	virtual void	VGui_Shutdown();
	virtual void	Shutdown();

	virtual void	LevelInit(const char* newmap);
	virtual void	LevelShutdown(void);

	virtual void	Enable();
	virtual void	Disable();
	virtual void	Layout();

	virtual void	ReloadScheme(void);
	virtual void	OverrideView(void* pSetup);
	virtual bool	ShouldDrawDetailObjects();
	virtual bool	ShouldDrawEntity(void* pEnt);
	virtual bool	ShouldDrawLocalPlayer(void* pPlayer);
	virtual bool	ShouldDrawViewModel();
	virtual bool	ShouldDrawParticles();
	virtual bool	ShouldDrawCrosshair(void);
	virtual bool	ShouldBlackoutAroundHUD();
	virtual void*   ShouldOverrideHeadtrackControl();
	virtual void	AdjustEngineViewport(int& x, int& y, int& width, int& height);
	virtual void	PreRender(void* pSetup);
	virtual void	PostRender();
	virtual void	PostRenderVGui();
	virtual void	ProcessInput(bool bActive);
	virtual bool	CreateMove(float flInputSampleTime, void* cmd); // 29
	virtual void	Update();

	// Input
	virtual int		KeyInput(int down, void* keynum, const char* pszCurrentBinding);
	virtual int		HudElementKeyInput(int down, void* keynum, const char* pszCurrentBinding);
	virtual void	OverrideMouseInput(float* x, float* y);
	virtual void	StartMessageMode(int iMessageModeType);
	virtual void* GetMessagePanel();

	virtual void	ActivateInGameVGuiContext(void* pPanel);
	virtual void	DeactivateInGameVGuiContext();

	// The mode can choose to not draw fog
	virtual bool	ShouldDrawFog(void);

	virtual float	GetViewModelFOV(void);
	virtual void* GetViewport();
	// Gets at the viewports vgui panel animation controller, if there is one...
	virtual void* GetViewportAnimationController();

	virtual void FireGameEvent(void* event);

	virtual bool CanRecordDemo(char* errorMsg, int length);

	virtual int HandleSpectatorKeyInput(int down, void* keynum, const char* pszCurrentBinding);

	virtual void	ComputeVguiResConditions(void* pkvConditions);

	//=============================================================================
	// HPE_BEGIN:
	// [menglish] Save server information shown to the client in a persistent place
	//=============================================================================

	virtual wchar_t* GetServerName() { return 0; }
	virtual void SetServerName(wchar_t* name) {};
	virtual wchar_t* GetMapName() { return 0; }
	virtual void SetMapName(wchar_t* name) {};

	//=============================================================================
	// HPE_END
	//=============================================================================

	virtual bool	DoPostScreenSpaceEffects(const void* pSetup);

	virtual void	DisplayReplayMessage(const char* pLocalizeName, float flDuration, bool bUrgent,
		const char* pSound, bool bDlg);

	virtual bool	IsInfoPanelAllowed();
	virtual void	InfoPanelDisplayed();
	virtual bool	IsHTMLInfoPanelAllowed();

protected:
	void* m_pViewport;

	void			DisplayReplayReminder();

private:
	virtual void	UpdateReplayMessages();

	void			ClearReplayMessageList();
};