#pragma once
#include "ibaseinterface.h"
#include "cviewsetup.h"
#include "studio.h"

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(void* pEntity, int contentsMask) = 0;
	virtual TraceType_t	GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}
};

class CTraceFilterEntitiesOnly : public ITraceFilter
{
public:
	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_ENTITIES_ONLY;
	}
};

struct Ray_t
{
	VectorAligned  m_Start;	// starting point, centered within the extents
	VectorAligned  m_Delta;	// direction + length of the ray
	VectorAligned  m_StartOffset;	// Add this to m_Start to get the actual ray start
	VectorAligned  m_Extents;	// Describes an axis aligned box extruded along a ray
	bool	m_IsRay;	// are the extents zero?
	bool	m_IsSwept;	// is delta != 0?

	void Init(Vector const& start, Vector const& end)
	{
		Assert(&end);
		VectorSubtract(end, start, m_Delta);

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		VectorClear(m_Extents);
		m_IsRay = true;

		// Offset m_Start to be in the center of the box...
		VectorClear(m_StartOffset);
		VectorCopy(start, m_Start);
	}

	void Init(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs)
	{
		Assert(&end);
		VectorSubtract(end, start, m_Delta);

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		VectorSubtract(maxs, mins, m_Extents);
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		// Offset m_Start to be in the center of the box...
		VectorAdd(mins, maxs, m_StartOffset);
		m_StartOffset *= 0.5f;
		VectorAdd(start, m_StartOffset, m_Start);
		m_StartOffset *= -1.0f;
	}

	// compute inverse delta
	Vector InvDelta() const
	{
		Vector vecInvDelta;
		for (int iAxis = 0; iAxis < 3; ++iAxis)
		{
			if (m_Delta[iAxis] != 0.0f)
			{
				vecInvDelta[iAxis] = 1.0f / m_Delta[iAxis];
			}
			else
			{
				vecInvDelta[iAxis] = 0;
			}
		}
		return vecInvDelta;
	}

private:
};

#define DISPSURF_FLAG_SURFACE		(1<<0)
#define DISPSURF_FLAG_WALKABLE		(1<<1)
#define DISPSURF_FLAG_BUILDABLE		(1<<2)
#define DISPSURF_FLAG_SURFPROP1		(1<<3)
#define DISPSURF_FLAG_SURFPROP2		(1<<4)

class CBaseTrace
{
public:

	// Displacement flags tests.
	bool IsDispSurface(void) { return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
	bool IsDispSurfaceWalkable(void) { return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
	bool IsDispSurfaceBuildable(void) { return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
	bool IsDispSurfaceProp1(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
	bool IsDispSurfaceProp2(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:

	// these members are aligned!!
	Vector			startpos;				// start position
	Vector			endpos;					// final position
	void*		plane;					// surface normal at impact

	float			fraction;				// time completed, 1.0 = didn't hit anything

	int				contents;				// contents on other side of surface hit
	unsigned short	dispFlags;				// displacement flags for marking surfaces with data

	bool			allsolid;				// if true, plane is not valid
	bool			startsolid;				// if true, the initial point was in a solid area

	CBaseTrace() {}

private:
	// No copy constructors allowed
	CBaseTrace(const CBaseTrace& vOther);
};

class IToolSystem;
struct AudioState_t;

class IEngineToolFramework : public IBaseInterface
{
public:
	// Input system overrides TBD
	// Something like this
	//virtual void		AddMessageHandler( int wm_message, bool (*pfnCallback)( int wm_message, int wParam, int lParam ) ) = 0;
	//virtual void		RemoveMessageHanlder( int wm_message, bool (*pfnCallbackToRemove)( int wm_message, int wParam, int lParam ) ) = 0;

	// Helpers for implementing a tool switching UI
	virtual int			GetToolCount() const = 0;
	virtual char const* GetToolName(int index) const = 0;
	virtual void		SwitchToTool(int index) = 0;

	virtual bool		IsTopmostTool(const IToolSystem* sys) const = 0;

	virtual const IToolSystem* GetToolSystem(int index) const = 0;
	virtual IToolSystem* GetTopmostTool() = 0;

	// Take over input
	virtual void		ShowCursor(bool show) = 0;
	virtual bool		IsCursorVisible() const = 0;
};

class IEngineTool : public IEngineToolFramework
{
public:
	virtual void		GetServerFactory(void*& factory) = 0;
	virtual void		GetClientFactory(void*& factory) = 0;

	virtual float		GetSoundDuration(const char* pszName) = 0;
	virtual bool		IsSoundStillPlaying(int guid) = 0;
	// Returns the guid of the sound
	virtual int			StartSound(
		int iUserData,
		bool staticsound,
		int iEntIndex,
		int iChannel,
		const char* pSample,
		float flVolume,
		void* iSoundlevel,
		const void*& origin,
		const void*& direction,
		int iFlags = 0,
		int iPitch = 0,
		bool bUpdatePositions = true,
		float delay = 0.0f,
		int speakerentity = -1) = 0;

	virtual void		StopSoundByGuid(int guid) = 0;

	// Returns how long the sound is
	virtual float		GetSoundDuration(int guid) = 0;

	// Returns if the sound is looping
	virtual bool		IsLoopingSound(int guid) = 0;
	virtual void		ReloadSound(const char* pSample) = 0;
	virtual void		StopAllSounds() = 0;
	virtual float		GetMono16Samples(const char* pszName, void*& sampleList) = 0;
	virtual void		SetAudioState(const AudioState_t& audioState) = 0;

	// Issue a console command
	virtual void		Command(char const* cmd) = 0;
	// Flush console command buffer right away
	virtual void		Execute() = 0;

	virtual char const* GetCurrentMap() = 0;
	virtual void		ChangeToMap(char const* mapname) = 0;
	virtual bool		IsMapValid(char const* mapname) = 0;

	// Method for causing engine to call client to render scene with no view model or overlays
	// See cdll_int.h for enum RenderViewInfo_t for specifying whatToRender
	virtual void		RenderView(CViewSetup& view, int nFlags, int whatToRender) = 0;

	// Returns true if the player is fully connected and active in game (i.e, not still loading)
	virtual bool		IsInGame() = 0;
	// Returns true if the player is connected, but not necessarily active in game (could still be loading)
	virtual bool		IsConnected() = 0;

	virtual int			GetMaxClients() = 0; // Tools might want to ensure single player, e.g.

	virtual bool		IsGamePaused() = 0;
	virtual void		SetGamePaused(bool paused) = 0;

	virtual float		GetTimescale() = 0; // Could do this via ConVar system, too
	virtual void		SetTimescale(float scale) = 0;

	// Real time is unscaled, but is updated once per frame
	virtual float		GetRealTime() = 0;
	virtual float		GetRealFrameTime() = 0; // unscaled

	// Get high precision timer (for profiling?)
	virtual float		Time() = 0;

	// Host time is scaled
	virtual float		HostFrameTime() = 0; // host_frametime
	virtual float		HostTime() = 0; // host_time
	virtual int			HostTick() = 0; // host_tickcount
	virtual int			HostFrameCount() = 0; // total famecount

	virtual float		ServerTime() = 0; // gpGlobals->curtime on server
	virtual float		ServerFrameTime() = 0; // gpGlobals->frametime on server
	virtual int			ServerTick() = 0; // gpGlobals->tickcount on server
	virtual float		ServerTickInterval() = 0; // tick interval on server

	virtual float		ClientTime() = 0; // gpGlobals->curtime on client
	virtual float		ClientFrameTime() = 0; // gpGlobals->frametime on client
	virtual int			ClientTick() = 0; // gpGlobals->tickcount on client

	virtual void		SetClientFrameTime(float frametime) = 0; // gpGlobals->frametime on client

	// Currently the engine doesn't like to do networking when it's paused, but if a tool changes entity state, it can be useful to force 
	//  a network update to get that state over to the client
	virtual void		ForceUpdateDuringPause() = 0;

	// Maybe through modelcache???
	virtual void* GetModel(void* hEntity) = 0;
	// Get the .mdl file used by entity (if it's a cbaseanimating)
	virtual studiohdr_t* GetStudioModel(void* hEntity) = 0;

	// SINGLE PLAYER/LISTEN SERVER ONLY (just matching the client .dll api for this)
	// Prints the formatted string to the notification area of the screen ( down the right hand edge
	//  numbered lines starting at position 0
	virtual void		Con_NPrintf(int pos, const char* fmt, ...) = 0;
	// SINGLE PLAYER/LISTEN SERVER ONLY(just matching the client .dll api for this)
	// Similar to Con_NPrintf, but allows specifying custom text color and duration information
	virtual void		Con_NXPrintf(const struct con_nprint_s* info, const char* fmt, ...) = 0;

	// Get the current game directory (hl2, tf2, hl1, cstrike, etc.)
	virtual void        GetGameDir(char* szGetGameDir, int maxlength) = 0;

	// Do we need separate rects for the 3d "viewport" vs. the tools surface??? and can we control viewports from
	virtual void		GetScreenSize(int& width, int& height) = 0;

	// GetRootPanel(VPANEL)

	// Sets the location of the main view
	virtual void		SetMainView(const Vector& vecOrigin, const QAngle& angles) = 0;

	// Gets the player view
	virtual bool		GetPlayerView(CViewSetup& playerView, int x, int y, int w, int h) = 0;

	// From a location on the screen, figure out the vector into the world
	virtual void		CreatePickingRay(const CViewSetup& viewSetup, int x, int y, Vector& org, Vector& forward) = 0;

	// precache methods
	virtual bool		PrecacheSound(const char* pName, bool bPreload = false) = 0;
	virtual bool		PrecacheModel(const char* pName, bool bPreload = false) = 0;

	virtual void		InstallQuitHandler(void* pvUserData, void* func) = 0;
	virtual void		TakeTGAScreenShot(char const* filename, int width, int height) = 0;
	// Even if game is paused, force networking to update to get new server state down to client
	virtual void		ForceSend() = 0;

	virtual bool		IsRecordingMovie() = 0;

	// NOTE: Params can contain file name, frame rate, output avi, output raw, and duration
	virtual void		StartMovieRecording(void* pMovieParams) = 0;
	virtual void		EndMovieRecording() = 0;
	virtual void		CancelMovieRecording() = 0;
	virtual void*		GetActiveVideoRecorder() = 0;

	virtual void		StartRecordingVoiceToFile(char const* filename, char const* pPathID = 0) = 0;
	virtual void		StopRecordingVoiceToFile() = 0;
	virtual bool		IsVoiceRecording() = 0;

	// A version that simply accepts a ray (can work as a traceline or tracehull)
	virtual void		TraceRay(const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, CBaseTrace* pTrace) = 0; // client version
	virtual void		TraceRayServer(const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, CBaseTrace* pTrace) = 0;

	virtual bool		IsConsoleVisible() = 0;

	virtual int			GetPointContents(const Vector& vecPosition) = 0;

	virtual int			GetActiveDLights(void* pList[32]) = 0;
	virtual int			GetLightingConditions(const Vector& vecPosition, Vector* pColors, int nMaxLocalLights, void* pLocalLights) = 0;

	virtual void		GetWorldToScreenMatrixForView(const CViewSetup& view, VMatrix* pVMatrix) = 0;

	// Collision support
	virtual void* CreatePartitionHandle(void* pEntity,
		void* listMask, const Vector& mins, const Vector& maxs) = 0;
	virtual void DestroyPartitionHandle(void* hPartition) = 0;
	virtual void InstallPartitionQueryCallback(void* pQuery) = 0;
	virtual void RemovePartitionQueryCallback(void* pQuery) = 0;
	virtual void ElementMoved(void* handle,
		const Vector& mins, const Vector& maxs) = 0;
};
