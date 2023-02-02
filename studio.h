#pragma once
#include "qangle.h"
#include "datamap.h"
#include "vmatrix.h"

#define Assert()

class IMaterial;
class CStudioHdr;
class Quaternion
{

};


struct mstudiomodel_t;
struct studiohdr_t;

typedef unsigned char byte;



struct mstudiobonecontroller_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					bone;	// -1 == 0
	int					type;	// X, Y, Z, XR, YR, ZR, M
	float				start;
	float				end;
	int					rest;	// byte index value at rest
	int					inputfield;	// 0-3 user set controller, 4 mouth
	int					unused[8];
};

struct mstudioflexdesc_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					szFACSindex;
	inline char* const pszFACS(void) const { return ((char*)this) + szFACSindex; }
};

// intersection boxes
struct mstudiobbox_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					bone;
	int					group;				// intersection group
	Vector				bbmin;				// bounding box
	Vector				bbmax;
	int					szhitboxnameindex;	// offset to the name of the hitbox.
	int					unused[8];

	const char* pszHitboxName()
	{
		if (szhitboxnameindex == 0)
			return "";

		return ((const char*)this) + szhitboxnameindex;
	}

	mstudiobbox_t() {}

private:
	// No copy constructors allowed
	mstudiobbox_t(const mstudiobbox_t& vOther);
};

// demand loaded sequence groups
struct mstudiomodelgroup_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					szlabelindex;	// textual name
	inline char* const pszLabel(void) const { return ((char*)this) + szlabelindex; }
	int					sznameindex;	// file name
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }
};

struct mstudiomodelgrouplookup_t
{
	int					modelgroup;
	int					indexwithingroup;
};

enum FlexControllerRemapType_t
{
	FLEXCONTROLLER_REMAP_PASSTHRU = 0,
	FLEXCONTROLLER_REMAP_2WAY,	// Control 0 -> ramps from 1-0 from 0->0.5. Control 1 -> ramps from 0-1 from 0.5->1
	FLEXCONTROLLER_REMAP_NWAY,	// StepSize = 1 / (control count-1) Control n -> ramps from 0-1-0 from (n-1)*StepSize to n*StepSize to (n+1)*StepSize. A second control is needed to specify amount to use 
	FLEXCONTROLLER_REMAP_EYELID
};

// events
struct mstudioevent_t
{
	DECLARE_BYTESWAP_DATADESC();
	float				cycle;
	int					event;
	int					type;
	inline const char* pszOptions(void) const { return options; }
	char				options[64];

	int					szeventindex;
	inline char* const pszEventName(void) const { return ((char*)this) + szeventindex; }
};

struct mstudioflexop_t
{
	DECLARE_BYTESWAP_DATADESC();
	int		op;
	union
	{
		int		index;
		float	value;
	} d;
};

struct mstudioflexrule_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					flex;
	int					numops;
	int					opindex;
	inline mstudioflexop_t* iFlexOp(int i) const { return  (mstudioflexop_t*)(((byte*)this) + opindex) + i; };
};


#define	ATTACHMENT_FLAG_WORLD_ALIGN 0x10000

// attachment
struct mstudioattachment_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					sznameindex;
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }
	unsigned int		flags;
	int					localbone;
	matrix3x4_t			local; // attachment point
	int					unused[8];
};

#define IK_SELF 1
#define IK_WORLD 2
#define IK_GROUND 3
#define IK_RELEASE 4
#define IK_ATTACHMENT 5
#define IK_UNLATCH 6

struct mstudioikerror_t
{
	DECLARE_BYTESWAP_DATADESC();
	Vector		pos;
	Quaternion	q;

	mstudioikerror_t() {}

private:
	// No copy constructors allowed
	mstudioikerror_t(const mstudioikerror_t& vOther);
};

struct virtualmodel_t
{
	void AppendSequences(int group, const studiohdr_t* pStudioHdr);
	void AppendAnimations(int group, const studiohdr_t* pStudioHdr);
	void AppendAttachments(int ground, const studiohdr_t* pStudioHdr);
	void AppendPoseParameters(int group, const studiohdr_t* pStudioHdr);
	void AppendBonemap(int group, const studiohdr_t* pStudioHdr);
	void AppendNodes(int group, const studiohdr_t* pStudioHdr);
	void AppendTransitions(int group, const studiohdr_t* pStudioHdr);
	void AppendIKLocks(int group, const studiohdr_t* pStudioHdr);
	void AppendModels(int group, const studiohdr_t* pStudioHdr);
	void UpdateAutoplaySequences(const studiohdr_t* pStudioHdr);

	void* m_Lock;

	void* m_seq;
	void* m_anim;
	void* m_attachment;
	void* m_pose;
	void* m_group;
	void* m_node;
	void* m_iklock;
	void* m_autoplaySequences;
};

// ikinfo
struct mstudioiklink_t
{
	DECLARE_BYTESWAP_DATADESC();
	int		bone;
	Vector	kneeDir;	// ideal bending direction (per link, if applicable)
	Vector	unused0;	// unused

	mstudioiklink_t() {}
private:
	// No copy constructors allowed
	mstudioiklink_t(const mstudioiklink_t& vOther);
};

struct mstudioikchain_t
{
	DECLARE_BYTESWAP_DATADESC();
	int				sznameindex;
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }
	int				linktype;
	int				numlinks;
	int				linkindex;
	inline mstudioiklink_t* pLink(int i) const { return (mstudioiklink_t*)(((byte*)this) + linkindex) + i; };
	// FIXME: add unused entries
};

union mstudioanimvalue_t;

struct mstudiocompressedikerror_t
{
	DECLARE_BYTESWAP_DATADESC();
	float	scale[6];
	short	offset[6];
	inline mstudioanimvalue_t* pAnimvalue(int i) const { if (offset[i] > 0) return  (mstudioanimvalue_t*)(((byte*)this) + offset[i]); else return 0; };
	mstudiocompressedikerror_t() {}

private:
	// No copy constructors allowed
	mstudiocompressedikerror_t(const mstudiocompressedikerror_t& vOther);
};

struct mstudioflex_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					flexdesc;	// input value

	float				target0;	// zero
	float				target1;	// one
	float				target2;	// one
	float				target3;	// zero

	int					numverts;
	int					vertindex;

	inline	byte* pBaseVertanim() const { return ((byte*)this) + vertindex; };

	int					flexpair;	// second flex desc
	unsigned char		vertanimtype;	// See StudioVertAnimType_t
	unsigned char		unusedchar[3];
	int					unused[6];
};

struct mstudio_meshvertexdata_t
{
	DECLARE_BYTESWAP_DATADESC();
	Vector* Position(int i) const;
	Vector* Normal(int i) const;
	void* TangentS(int i) const;
	void* Texcoord(int i) const;
	void* BoneWeights(int i) const;
	void* Vertex(int i) const;
	bool				HasTangentData(void) const;
	int					GetModelVertexIndex(int i) const;
	int					GetGlobalVertexIndex(int i) const;

	// indirection to this mesh's model's vertex data
	const void* modelvertexdata;

	// used for fixup calcs when culling top level lods
	// expected number of mesh verts at desired lod
	int					numLODVertexes[32];
};

struct mstudioikrule_t
{
	DECLARE_BYTESWAP_DATADESC();
	int			index;

	int			type;
	int			chain;

	int			bone;

	int			slot;	// iktarget slot.  Usually same as chain.
	float		height;
	float		radius;
	float		floor;
	Vector		pos;
	void*	q;

	int			compressedikerrorindex;
	inline mstudiocompressedikerror_t* pCompressedError() const { return (mstudiocompressedikerror_t*)(((byte*)this) + compressedikerrorindex); };
	int			unused2;

	int			iStart;
	int			ikerrorindex;
	inline mstudioikerror_t* pError(int i) const { return  (ikerrorindex) ? (mstudioikerror_t*)(((byte*)this) + ikerrorindex) + (i - iStart) : 0; };

	float		start;	// beginning of influence
	float		peak;	// start of full influence
	float		tail;	// end of full influence
	float		end;	// end of all influence

	float		unused3;	// 
	float		contact;	// frame footstep makes ground concact
	float		drop;		// how far down the foot should drop when reaching for IK
	float		top;		// top of the foot box

	int			unused6;
	int			unused7;
	int			unused8;

	int			szattachmentindex;		// name of world attachment
	inline char* const pszAttachment(void) const { return ((char*)this) + szattachmentindex; }

	int			unused[7];

	mstudioikrule_t() {}

private:
	// No copy constructors allowed
	mstudioikrule_t(const mstudioikrule_t& vOther);
};

// skin info
struct mstudiotexture_t
{
	DECLARE_BYTESWAP_DATADESC();
	int						sznameindex;
	inline char* const		pszName(void) const { return ((char*)this) + sznameindex; }
	int						flags;
	int						used;
	int						unused1;
	mutable IMaterial* material;  // fixme: this needs to go away . .isn't used by the engine, but is used by studiomdl
	mutable void* clientmaterial;	// gary, replace with client material pointer if used

	int						unused[10];
};

struct mstudiomesh_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					material;

	int					modelindex;
	mstudiomodel_t* pModel() const;

	int					numvertices;		// number of unique vertices/normals/texcoords
	int					vertexoffset;		// vertex mstudiovertex_t

	// Access thin/fat mesh vertex data (only one will return a non-0 result)
	const mstudio_meshvertexdata_t* GetVertexData(void* pModelData = 0);
	const void* GetThinVertexData(void* pModelData = 0);

	int					numflexes;			// vertex animation
	int					flexindex;
	inline mstudioflex_t* pFlex(int i) const { return (mstudioflex_t*)(((byte*)this) + flexindex) + i; };

	// special codes for material operations
	int					materialtype;
	int					materialparam;

	// a unique ordinal for this mesh
	int					meshid;

	Vector				center;

	mstudio_meshvertexdata_t vertexdata;

	int					unused[8]; // remove as appropriate

	mstudiomesh_t() {}
private:
	// No copy constructors allowed
	mstudiomesh_t(const mstudiomesh_t& vOther);
};

// studio models
struct mstudiomodel_t
{
	DECLARE_BYTESWAP_DATADESC();
	inline const char* pszName(void) const { return name; }
	char				name[64];

	int					type;

	float				boundingradius;

	int					nummeshes;
	int					meshindex;
	inline mstudiomesh_t* pMesh(int i) const { return (mstudiomesh_t*)(((byte*)this) + meshindex) + i; };

	// cache purposes
	int					numvertices;		// number of unique vertices/normals/texcoords
	int					vertexindex;		// vertex Vector
	int					tangentsindex;		// tangents Vector

	// These functions are defined in application-specific code:
	const void* CacheVertexData(void* pModelData);

	// Access thin/fat mesh vertex data (only one will return a non-0 result)
	const void* GetVertexData(void* pModelData = 0);
	const void* GetThinVertexData(void* pModelData = 0);

	int					numattachments;
	int					attachmentindex;

	int					numeyeballs;
	int					eyeballindex;
	inline  void* pEyeball(int i) { return (void**)(((byte*)this) + eyeballindex) + i; };

	void* vertexdata;

	int					unused[8];		// remove as appropriate
};


// body part index
struct mstudiobodyparts_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					sznameindex;
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }
	int					nummodels;
	int					base;
	int					modelindex; // index into models array
	inline mstudiomodel_t* pModel(int i) const { return (mstudiomodel_t*)(((byte*)this) + modelindex) + i; };
};

struct mstudioposeparamdesc_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					sznameindex;
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }
	int					flags;	// ????
	float				start;	// starting value
	float				end;	// ending value
	float				loop;	// looping range, 0 for no looping, 360 for rotations, etc.
};

struct mstudioiklock_t
{
	DECLARE_BYTESWAP_DATADESC();
	int			chain;
	float		flPosWeight;
	float		flLocalQWeight;
	int			flags;

	int			unused[4];
};


struct mstudiolocalhierarchy_t
{
	DECLARE_BYTESWAP_DATADESC();
	int			iBone;			// bone being adjusted
	int			iNewParent;		// the bones new parent

	float		start;			// beginning of influence
	float		peak;			// start of full influence
	float		tail;			// end of full influence
	float		end;			// end of all influence

	int			iStart;			// first frame 

	int			localanimindex;
	inline mstudiocompressedikerror_t* pLocalAnim() const { return (mstudiocompressedikerror_t*)(((byte*)this) + localanimindex); };

	int			unused[4];
};


struct mstudioautolayer_t
{
	DECLARE_BYTESWAP_DATADESC();
	//private:
	short				iSequence;
	short				iPose;
	//public:
	int					flags;
	float				start;	// beginning of influence
	float				peak;	// start of full influence
	float				tail;	// end of full influence
	float				end;	// end of all influence
};

// sequence descriptions
struct mstudioseqdesc_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					baseptr;
	inline studiohdr_t* pStudiohdr(void) const { return (studiohdr_t*)(((byte*)this) + baseptr); }

	int					szlabelindex;
	inline char* const pszLabel(void) const { return ((char*)this) + szlabelindex; }

	int					szactivitynameindex;
	inline char* const pszActivityName(void) const { return ((char*)this) + szactivitynameindex; }

	int					flags;		// looping/non-looping flags

	int					activity;	// initialized at loadtime to game DLL values
	int					actweight;

	int					numevents;
	int					eventindex;
	inline mstudioevent_t* pEvent(int i) const { Assert(i >= 0 && i < numevents); return (mstudioevent_t*)(((byte*)this) + eventindex) + i; };

	Vector				bbmin;		// per sequence bounding box
	Vector				bbmax;

	int					numblends;

	// Index into array of shorts which is groupsize[0] x groupsize[1] in length
	int					animindexindex;

	inline int			anim(int x, int y) const
	{
		if (x >= groupsize[0])
		{
			x = groupsize[0] - 1;
		}

		if (y >= groupsize[1])
		{
			y = groupsize[1] - 1;
		}

		int offset = y * groupsize[0] + x;
		short* blends = (short*)(((byte*)this) + animindexindex);
		int value = (int)blends[offset];
		return value;
	}

	int					movementindex;	// [blend] float array for blended movement
	int					groupsize[2];
	int					paramindex[2];	// X, Y, Z, XR, YR, ZR
	float				paramstart[2];	// local (0..1) starting value
	float				paramend[2];	// local (0..1) ending value
	int					paramparent;

	float				fadeintime;		// ideal cross fate in time (0.2 default)
	float				fadeouttime;	// ideal cross fade out time (0.2 default)

	int					localentrynode;		// transition node at entry
	int					localexitnode;		// transition node at exit
	int					nodeflags;		// transition rules

	float				entryphase;		// used to match entry gait
	float				exitphase;		// used to match exit gait

	float				lastframe;		// frame that should generation EndOfSequence

	int					nextseq;		// auto advancing sequences
	int					pose;			// index of delta animation between end and nextseq

	int					numikrules;

	int					numautolayers;	//
	int					autolayerindex;
	inline mstudioautolayer_t* pAutolayer(int i) const { Assert(i >= 0 && i < numautolayers); return (mstudioautolayer_t*)(((byte*)this) + autolayerindex) + i; };

	int					weightlistindex;
	inline float* pBoneweight(int i) const { return ((float*)(((byte*)this) + weightlistindex) + i); };
	inline float		weight(int i) const { return *(pBoneweight(i)); };

	// FIXME: make this 2D instead of 2x1D arrays
	int					posekeyindex;
	float* pPoseKey(int iParam, int iAnim) const { return (float*)(((byte*)this) + posekeyindex) + iParam * groupsize[0] + iAnim; }
	float				poseKey(int iParam, int iAnim) const { return *(pPoseKey(iParam, iAnim)); }

	int					numiklocks;
	int					iklockindex;
	inline mstudioiklock_t* pIKLock(int i) const { Assert(i >= 0 && i < numiklocks); return (mstudioiklock_t*)(((byte*)this) + iklockindex) + i; };

	// Key values
	int					keyvalueindex;
	int					keyvaluesize;
	inline const char* KeyValueText(void) const { return keyvaluesize != 0 ? ((char*)this) + keyvalueindex : 0; }

	int					cycleposeindex;		// index of pose parameter to use as cycle index

	int					unused[7];		// remove/add as appropriate (grow back to 8 ints on version change!)

	mstudioseqdesc_t() {}
private:
	// No copy constructors allowed
	mstudioseqdesc_t(const mstudioseqdesc_t& vOther);
};



struct mstudioflexcontroller_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					sztypeindex;
	inline char* const pszType(void) const { return ((char*)this) + sztypeindex; }
	int					sznameindex;
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }
	mutable int			localToGlobal;	// remapped at load time to master list
	float				min;
	float				max;
};

#define STUDIOHDR_FLAGS_AUTOGENERATED_HITBOX	( 1 << 0 )

// NOTE:  This flag is set at loadtime, not mdl build time so that we don't have to rebuild
// models when we change materials.
#define STUDIOHDR_FLAGS_USES_ENV_CUBEMAP		( 1 << 1 )

// Use this when there are translucent parts to the model but we're not going to sort it 
#define STUDIOHDR_FLAGS_FORCE_OPAQUE			( 1 << 2 )

// Use this when we want to render the opaque parts during the opaque pass
// and the translucent parts during the translucent pass
#define STUDIOHDR_FLAGS_TRANSLUCENT_TWOPASS		( 1 << 3 )

// This is set any time the .qc files has $staticprop in it
// Means there's no bones and no transforms
#define STUDIOHDR_FLAGS_STATIC_PROP				( 1 << 4 )

// NOTE:  This flag is set at loadtime, not mdl build time so that we don't have to rebuild
// models when we change materials.
#define STUDIOHDR_FLAGS_USES_FB_TEXTURE		    ( 1 << 5 )

// This flag is set by studiomdl.exe if a separate "$shadowlod" entry was present
//  for the .mdl (the shadow lod is the last entry in the lod list if present)
#define STUDIOHDR_FLAGS_HASSHADOWLOD			( 1 << 6 )

// NOTE:  This flag is set at loadtime, not mdl build time so that we don't have to rebuild
// models when we change materials.
#define STUDIOHDR_FLAGS_USES_BUMPMAPPING		( 1 << 7 )

// NOTE:  This flag is set when we should use the actual materials on the shadow LOD
// instead of overriding them with the default one (necessary for translucent shadows)
#define STUDIOHDR_FLAGS_USE_SHADOWLOD_MATERIALS	( 1 << 8 )

// NOTE:  This flag is set when we should use the actual materials on the shadow LOD
// instead of overriding them with the default one (necessary for translucent shadows)
#define STUDIOHDR_FLAGS_OBSOLETE				( 1 << 9 )

#define STUDIOHDR_FLAGS_UNUSED					( 1 << 10 )

// NOTE:  This flag is set at mdl build time
#define STUDIOHDR_FLAGS_NO_FORCED_FADE			( 1 << 11 )

// NOTE:  The npc will lengthen the viseme check to always include two phonemes
#define STUDIOHDR_FLAGS_FORCE_PHONEME_CROSSFADE	( 1 << 12 )

// This flag is set when the .qc has $constantdirectionallight in it
// If set, we use constantdirectionallightdot to calculate light intensity
// rather than the normal directional dot product
// only valid if STUDIOHDR_FLAGS_STATIC_PROP is also set
#define STUDIOHDR_FLAGS_CONSTANT_DIRECTIONAL_LIGHT_DOT ( 1 << 13 )

// Flag to mark delta flexes as already converted from disk format to memory format
#define STUDIOHDR_FLAGS_FLEXES_CONVERTED		( 1 << 14 )

// Indicates the studiomdl was built in preview mode
#define STUDIOHDR_FLAGS_BUILT_IN_PREVIEW_MODE	( 1 << 15 )

// Ambient boost (runtime flag)
#define STUDIOHDR_FLAGS_AMBIENT_BOOST			( 1 << 16 )

// Don't cast shadows from this model (useful on first-person models)
#define STUDIOHDR_FLAGS_DO_NOT_CAST_SHADOWS		( 1 << 17 )

// alpha textures should cast shadows in vrad on this model (ONLY prop_static!)
#define STUDIOHDR_FLAGS_CAST_TEXTURE_SHADOWS	( 1 << 18 )


// flagged on load to indicate no animation events on this model
#define STUDIOHDR_FLAGS_VERT_ANIM_FIXED_POINT_SCALE	( 1 << 21 )

// animation frames
union mstudioanimvalue_t
{
	struct
	{
		byte	valid;
		byte	total;
	} num;
	short		value;
};


struct mstudioaxisinterpbone_t
{
	DECLARE_BYTESWAP_DATADESC();
	int				control;// local transformation of this bone used to calc 3 point blend
	int				axis;	// axis to check
	Vector			pos[6];	// X+, X-, Y+, Y-, Z+, Z-
	Quaternion		quat[6];// X+, X-, Y+, Y-, Z+, Z-

	mstudioaxisinterpbone_t() {}
private:
	// No copy constructors allowed
	mstudioaxisinterpbone_t(const mstudioaxisinterpbone_t& vOther);
};

struct mstudioanimblock_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					datastart;
	int					dataend;
};


struct mstudioquatinterpinfo_t
{
	DECLARE_BYTESWAP_DATADESC();
	float			inv_tolerance;	// 1 / radian angle of trigger influence
	Quaternion		trigger;	// angle to match
	Vector			pos;		// new position
	Quaternion		quat;		// new angle

	mstudioquatinterpinfo_t() {}
private:
	// No copy constructors allowed
	mstudioquatinterpinfo_t(const mstudioquatinterpinfo_t& vOther);
};

struct mstudioquatinterpbone_t
{
	DECLARE_BYTESWAP_DATADESC();
	int				control;// local transformation to check
	int				numtriggers;
	int				triggerindex;
	inline mstudioquatinterpinfo_t* pTrigger(int i) const { return  (mstudioquatinterpinfo_t*)(((byte*)this) + triggerindex) + i; };

	mstudioquatinterpbone_t() {}
private:
	// No copy constructors allowed
	mstudioquatinterpbone_t(const mstudioquatinterpbone_t& vOther);
};

struct mstudiojigglebone_t
{
	DECLARE_BYTESWAP_DATADESC();

	int				flags;

	// general params
	float			length;					// how from from bone base, along bone, is tip
	float			tipMass;

	// flexible params
	float			yawStiffness;
	float			yawDamping;
	float			pitchStiffness;
	float			pitchDamping;
	float			alongStiffness;
	float			alongDamping;

	// angle constraint
	float			angleLimit;				// maximum deflection of tip in radians

	// yaw constraint
	float			minYaw;					// in radians
	float			maxYaw;					// in radians
	float			yawFriction;
	float			yawBounce;

	// pitch constraint
	float			minPitch;				// in radians
	float			maxPitch;				// in radians
	float			pitchFriction;
	float			pitchBounce;

	// base spring
	float			baseMass;
	float			baseStiffness;
	float			baseDamping;
	float			baseMinLeft;
	float			baseMaxLeft;
	float			baseLeftFriction;
	float			baseMinUp;
	float			baseMaxUp;
	float			baseUpFriction;
	float			baseMinForward;
	float			baseMaxForward;
	float			baseForwardFriction;

	// boing
	float			boingImpactSpeed;
	float			boingImpactAngle;
	float			boingDampingRate;
	float			boingFrequency;
	float			boingAmplitude;

private:
	// No copy constructors allowed
	//mstudiojigglebone_t(const mstudiojigglebone_t& vOther);
};

struct mstudioanim_t
{
	DECLARE_BYTESWAP_DATADESC();
	byte				bone;
	byte				flags;		// weighing options

	short				nextoffset;
};

struct mstudiomovement_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					endframe;
	int					motionflags;
	float				v0;			// velocity at start of block
	float				v1;			// velocity at end of block
	float				angle;		// YAW rotation at end of this blocks movement
	Vector				vector;		// movement vector relative to this blocks initial angle
	Vector				position;	// relative to start of animation???

	mstudiomovement_t() {}
private:
	// No copy constructors allowed
	mstudiomovement_t(const mstudiomovement_t& vOther);
};


struct mstudioanimdesc_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					baseptr;
	inline studiohdr_t* pStudiohdr(void) const { return (studiohdr_t*)(((byte*)this) + baseptr); }

	int					sznameindex;
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }

	float				fps;		// frames per second	
	int					flags;		// looping/non-looping flags

	int					numframes;

	// piecewise movement
	int					nummovements;
	int					movementindex;
	inline mstudiomovement_t* const pMovement(int i) const { return (mstudiomovement_t*)(((byte*)this) + movementindex) + i; };

	int					unused1[6];			// remove as appropriate (and zero if loading older versions)	

	int					animblock;
	int					animindex;	 // non-zero when anim data isn't in sections
	mstudioanim_t* pAnimBlock(int block, int index) const; // returns pointer to a specific anim block (local or external)
	mstudioanim_t* pAnim(int* piFrame, float& flStall) const; // returns pointer to data and new frame index
	mstudioanim_t* pAnim(int* piFrame) const; // returns pointer to data and new frame index

	int					numikrules;
	int					ikruleindex;	// non-zero when IK data is stored in the mdl
	int					animblockikruleindex; // non-zero when IK data is stored in animblock file
	mstudioikrule_t* pIKRule(int i) const;

	int					numlocalhierarchy;
	int					localhierarchyindex;
	mstudiolocalhierarchy_t* pHierarchy(int i) const;

	int					sectionindex;
	int					sectionframes; // number of frames used in each fast lookup section, zero if not used
	inline void* const pSection(int i) const { return (void**)(((byte*)this) + sectionindex) + i; }

	short				zeroframespan;	// frames per span
	short				zeroframecount; // number of spans
	int					zeroframeindex;
	byte* pZeroFrameData() const { if (zeroframeindex) return (((byte*)this) + zeroframeindex); else return 0; };
	mutable float		zeroframestalltime;		// saved during read stalls

	mstudioanimdesc_t() {}
private:
	// No copy constructors allowed
	mstudioanimdesc_t(const mstudioanimdesc_t& vOther);
};


struct mstudiomouth_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					bone;
	Vector				forward;
	int					flexdesc;

	mstudiomouth_t() {}
private:
	// No copy constructors allowed
	mstudiomouth_t(const mstudiomouth_t& vOther);
};

struct mstudiohitboxset_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					sznameindex;
	inline char* const	pszName(void) const { return ((char*)this) + sznameindex; }
	int					numhitboxes;
	int					hitboxindex;
	inline mstudiobbox_t* pHitbox(int i) const { return (mstudiobbox_t*)(((byte*)this) + hitboxindex) + i; };
};


struct mstudioaimatbone_t
{
	DECLARE_BYTESWAP_DATADESC();

	int				parent;
	int				aim;		// Might be bone or attach
	Vector			aimvector;
	Vector			upvector;
	Vector			basepos;

	mstudioaimatbone_t() {}
private:
	// No copy constructors allowed
	mstudioaimatbone_t(const mstudioaimatbone_t& vOther);
};

// bones
struct mstudiobone_t
{
	// DECLARE_BYTESWAP_DATADESC();
	int					sznameindex;
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }
	int		 			parent;		// parent bone
	int					bonecontroller[6];	// bone controller index, -1 == none

	// default values
	Vector				pos;
	void* quat;
	void*			rot;
	// compression scale
	Vector				posscale;
	Vector				rotscale;

	matrix3x4_t			poseToBone;
	void*			qAlignment;
	int					flags;
	int					proctype;
	int					procindex;		// procedural rule
	mutable int			physicsbone;	// index into physically simulated bone
	inline void* pProcedure() const { if (procindex == 0) return 0; else return  (void*)(((byte*)this) + procindex); };
	int					surfacepropidx;	// index into string tablefor property name
	inline char* const pszSurfaceProp(void) const { return ((char*)this) + surfacepropidx; }
	int					contents;		// See BSPFlags.h for the contents flags

	int					unused[8];		// remove as appropriate

	mstudiobone_t() {}
private:
	// No copy constructors allowed
	mstudiobone_t(const mstudiobone_t& vOther);
};

struct mstudiolinearbone_t
{
	// DECLARE_BYTESWAP_DATADESC();

	int numbones;

	int flagsindex;
	inline int flags(int i) const { Assert(i >= 0 && i < numbones); return *((int*)(((byte*)this) + flagsindex) + i); };
	inline int* pflags(int i) { Assert(i >= 0 && i < numbones); return ((int*)(((byte*)this) + flagsindex) + i); };

	int	parentindex;
	inline int parent(int i) const { Assert(i >= 0 && i < numbones); return *((int*)(((byte*)this) + parentindex) + i); };

	int	posindex;
	inline Vector pos(int i) const { Assert(i >= 0 && i < numbones); return *((Vector*)(((byte*)this) + posindex) + i); };

	int quatindex;
	// inline Quaternion quat(int i) const { Assert(i >= 0 && i < numbones); return *((Quaternion*)(((byte*)this) + quatindex) + i); };

	int rotindex;
	// inline RadianEuler rot(int i) const { Assert(i >= 0 && i < numbones); return *((RadianEuler*)(((byte*)this) + rotindex) + i); };

	int posetoboneindex;
	// inline matrix3x4_t poseToBone(int i) const { Assert(i >= 0 && i < numbones); return *((matrix3x4_t*)(((byte*)this) + posetoboneindex) + i); };

	int	posscaleindex;
	inline Vector posscale(int i) const { Assert(i >= 0 && i < numbones); return *((Vector*)(((byte*)this) + posscaleindex) + i); };

	int	rotscaleindex;
	inline Vector rotscale(int i) const { Assert(i >= 0 && i < numbones); return *((Vector*)(((byte*)this) + rotscaleindex) + i); };

	int	qalignmentindex;
	// inline Quaternion qalignment(int i) const { Assert(i >= 0 && i < numbones); return *((Quaternion*)(((byte*)this) + qalignmentindex) + i); };

	int unused[6];

	mstudiolinearbone_t() {}
private:
	// No copy constructors allowed
	mstudiolinearbone_t(const mstudiolinearbone_t& vOther);
};

enum StudioBoneFlexComponent_t
{
	STUDIO_BONE_FLEX_INVALID = -1,	// Invalid
	STUDIO_BONE_FLEX_TX = 0,		// Translate X
	STUDIO_BONE_FLEX_TY = 1,		// Translate Y
	STUDIO_BONE_FLEX_TZ = 2			// Translate Z
};


struct mstudioboneflexdrivercontrol_t
{
	// DECLARE_BYTESWAP_DATADESC();

	int m_nBoneComponent;		// Bone component that drives flex, StudioBoneFlexComponent_t
	int m_nFlexControllerIndex;	// Flex controller to drive
	float m_flMin;				// Min value of bone component mapped to 0 on flex controller
	float m_flMax;				// Max value of bone component mapped to 1 on flex controller

	mstudioboneflexdrivercontrol_t() {}
private:
	// No copy constructors allowed
	mstudioboneflexdrivercontrol_t(const mstudioboneflexdrivercontrol_t& vOther);
};

struct mstudioboneflexdriver_t
{
	// DECLARE_BYTESWAP_DATADESC();

	int m_nBoneIndex;			// Bone to drive flex controller
	int m_nControlCount;		// Number of flex controllers being driven
	int m_nControlIndex;		// Index into data where controllers are (relative to this)

	inline mstudioboneflexdrivercontrol_t* pBoneFlexDriverControl(int i) const
	{
		Assert(i >= 0 && i < m_nControlCount);
		return (mstudioboneflexdrivercontrol_t*)(((byte*)this) + m_nControlIndex) + i;
	}

	int unused[3];

	mstudioboneflexdriver_t() {}
private:
	// No copy constructors allowed
	mstudioboneflexdriver_t(const mstudioboneflexdriver_t& vOther);
};


struct studiohdr2_t
{
	// NOTE: For forward compat, make sure any methods in this struct
	// are also available in studiohdr_t so no leaf code ever directly references
	// a studiohdr2_t structure
	int numsrcbonetransform;
	int srcbonetransformindex;

	int	illumpositionattachmentindex;
	inline int			IllumPositionAttachmentIndex() const { return illumpositionattachmentindex; }

	float flMaxEyeDeflection;
	inline float		MaxEyeDeflection() const { return flMaxEyeDeflection != 0.0f ? flMaxEyeDeflection : 0.866f; } // default to cos(30) if not set

	int linearboneindex;
	inline mstudiolinearbone_t* pLinearBones() const { return (linearboneindex) ? (mstudiolinearbone_t*)(((byte*)this) + linearboneindex) : 0; }

	int sznameindex;
	inline char* pszName() { return (sznameindex) ? (char*)(((byte*)this) + sznameindex) : 0; }

	int m_nBoneFlexDriverCount;
	int m_nBoneFlexDriverIndex;
	inline mstudioboneflexdriver_t* pBoneFlexDriver(int i) const { Assert(i >= 0 && i < m_nBoneFlexDriverCount); return (mstudioboneflexdriver_t*)(((byte*)this) + m_nBoneFlexDriverIndex) + i; }

	int reserved[56];
};

struct mstudioflexcontrollerui_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					sznameindex;
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }

	// These are used like a union to save space
	// Here are the possible configurations for a UI controller
	//
	// SIMPLE NON-STEREO:	0: control	1: unused	2: unused
	// STEREO:				0: left		1: right	2: unused
	// NWAY NON-STEREO:		0: control	1: unused	2: value
	// NWAY STEREO:			0: left		1: right	2: value

	int					szindex0;
	int					szindex1;
	int					szindex2;

	inline const mstudioflexcontroller_t* pController(void) const
	{
		return !stereo ? (mstudioflexcontroller_t*)((char*)this + szindex0) : 0;
	}
	inline char* const	pszControllerName(void) const { return !stereo ? pController()->pszName() : 0; }
	inline int			controllerIndex(const CStudioHdr& cStudioHdr) const;

	inline const mstudioflexcontroller_t* pLeftController(void) const
	{
		return stereo ? (mstudioflexcontroller_t*)((char*)this + szindex0) : 0;
	}
	inline char* const	pszLeftName(void) const { return stereo ? pLeftController()->pszName() : 0; }
	inline int			leftIndex(const CStudioHdr& cStudioHdr) const;

	inline const mstudioflexcontroller_t* pRightController(void) const
	{
		return stereo ? (mstudioflexcontroller_t*)((char*)this + szindex1) : 0;
	}
	inline char* const	pszRightName(void) const { return stereo ? pRightController()->pszName() : 0; }
	inline int			rightIndex(const CStudioHdr& cStudioHdr) const;

	inline const mstudioflexcontroller_t* pNWayValueController(void) const
	{
		return remaptype == FLEXCONTROLLER_REMAP_NWAY ? (mstudioflexcontroller_t*)((char*)this + szindex2) : 0;
	}
	inline char* const	pszNWayValueName(void) const { return remaptype == FLEXCONTROLLER_REMAP_NWAY ? pNWayValueController()->pszName() : 0; }
	inline int			nWayValueIndex(const CStudioHdr& cStudioHdr) const;

	// Number of controllers this ui description contains, 1, 2 or 3
	inline int			Count() const { return (stereo ? 2 : 1) + (remaptype == FLEXCONTROLLER_REMAP_NWAY ? 1 : 0); }
	inline const mstudioflexcontroller_t* pController(int index) const;

	unsigned char		remaptype;	// See the FlexControllerRemapType_t enum
	bool				stereo;		// Is this a stereo control?
	byte				unused[2];
};

struct studiohdr_t
{
	DECLARE_BYTESWAP_DATADESC();
	int					id;
	int					version;

	int					checksum;		// this has to be the same in the phy and vtx files to load!

	inline const char* pszName(void) const { if (studiohdr2index && pStudioHdr2()->pszName()) return pStudioHdr2()->pszName(); else return name; }
	char				name[64];
	int					length;


	Vector				eyeposition;	// ideal eye position

	Vector				illumposition;	// illumination center

	Vector				hull_min;		// ideal movement hull size
	Vector				hull_max;

	Vector				view_bbmin;		// clipping bounding box
	Vector				view_bbmax;

	int					flags;

	int					numbones;			// bones
	int					boneindex;
	inline mstudiobone_t* pBone(int i) const { Assert(i >= 0 && i < numbones); return (mstudiobone_t*)(((unsigned char*)this) + boneindex) + i; };
	int					RemapSeqBone(int iSequence, int iLocalBone) const;	// maps local sequence bone to global bone
	int					RemapAnimBone(int iAnim, int iLocalBone) const;		// maps local animations bone to global bone

	int					numbonecontrollers;		// bone controllers
	int					bonecontrollerindex;
	inline mstudiobonecontroller_t* pBonecontroller(int i) const { Assert(i >= 0 && i < numbonecontrollers); return (mstudiobonecontroller_t*)(((unsigned char*)this) + bonecontrollerindex) + i; };

	int					numhitboxsets;
	int					hitboxsetindex;

	// Look up hitbox set by index
	mstudiohitboxset_t* pHitboxSet(int i) const
	{
		Assert(i >= 0 && i < numhitboxsets);
		return (mstudiohitboxset_t*)(((unsigned char*)this) + hitboxsetindex) + i;
	};

	// Calls through to hitbox to determine size of specified set
	inline mstudiobbox_t* pHitbox(int i, int set) const
	{
		mstudiohitboxset_t const* s = pHitboxSet(set);
		if (!s)
			return 0;

		return s->pHitbox(i);
	};

	// Calls through to set to get hitbox count for set
	inline int			iHitboxCount(int set) const
	{
		mstudiohitboxset_t const* s = pHitboxSet(set);
		if (!s)
			return 0;

		return s->numhitboxes;
	};

	// file local animations? and sequences
//private:
	int					numlocalanim;			// animations/poses
	int					localanimindex;		// animation descriptions
	inline mstudioanimdesc_t* pLocalAnimdesc(int i) const { if (i < 0 || i >= numlocalanim) i = 0; return (mstudioanimdesc_t*)(((unsigned char*)this) + localanimindex) + i; };

	int					numlocalseq;				// sequences
	int					localseqindex;
	inline mstudioseqdesc_t* pLocalSeqdesc(int i) const { if (i < 0 || i >= numlocalseq) i = 0; return (mstudioseqdesc_t*)(((unsigned char*)this) + localseqindex) + i; };

	//public:
	bool				SequencesAvailable() const;
	int					GetNumSeq() const;
	mstudioanimdesc_t& pAnimdesc(int i) const;
	mstudioseqdesc_t& pSeqdesc(int i) const;
	int					iRelativeAnim(int baseseq, int relanim) const;	// maps seq local anim reference to global anim index
	int					iRelativeSeq(int baseseq, int relseq) const;		// maps seq local seq reference to global seq index

	//private:
	mutable int			activitylistversion;	// initialization flag - have the sequences been indexed?
	mutable int			eventsindexed;
	//public:
	int					GetSequenceActivity(int iSequence);
	void				SetSequenceActivity(int iSequence, int iActivity);
	int					GetActivityListVersion(void);
	void				SetActivityListVersion(int version) const;
	int					GetEventListVersion(void);
	void				SetEventListVersion(int version);

	// raw textures
	int					numtextures;
	int					textureindex;
	inline mstudiotexture_t* pTexture(int i) const { Assert(i >= 0 && i < numtextures); return (mstudiotexture_t*)(((unsigned char*)this) + textureindex) + i; };


	// raw textures search paths
	int					numcdtextures;
	int					cdtextureindex;
	inline char* pCdtexture(int i) const { return (((char*)this) + *((int*)(((unsigned char*)this) + cdtextureindex) + i)); };

	// replaceable textures tables
	int					numskinref;
	int					numskinfamilies;
	int					skinindex;
	inline short* pSkinref(int i) const { return (short*)(((unsigned char*)this) + skinindex) + i; };

	int					numbodyparts;
	int					bodypartindex;
	inline mstudiobodyparts_t* pBodypart(int i) const { return (mstudiobodyparts_t*)(((unsigned char*)this) + bodypartindex) + i; };

	// queryable attachable points
//private:
	int					numlocalattachments;
	int					localattachmentindex;
	inline mstudioattachment_t* pLocalAttachment(int i) const { Assert(i >= 0 && i < numlocalattachments); return (mstudioattachment_t*)(((unsigned char*)this) + localattachmentindex) + i; };
	//public:
	int					GetNumAttachments(void) const;
	const mstudioattachment_t& pAttachment(int i) const;
	int					GetAttachmentBone(int i);
	// used on my tools in hlmv, not persistant
	void				SetAttachmentBone(int iAttachment, int iBone);

	// animation node to animation node transition graph
//private:
	int					numlocalnodes;
	int					localnodeindex;
	int					localnodenameindex;
	inline char* pszLocalNodeName(int iNode) const { Assert(iNode >= 0 && iNode < numlocalnodes); return (((char*)this) + *((int*)(((unsigned char*)this) + localnodenameindex) + iNode)); }
	inline unsigned char* pLocalTransition(int i) const { Assert(i >= 0 && i < (numlocalnodes* numlocalnodes)); return (unsigned char*)(((unsigned char*)this) + localnodeindex) + i; };

	//public:
	int					EntryNode(int iSequence);
	int					ExitNode(int iSequence);
	char* pszNodeName(int iNode);
	int					GetTransition(int iFrom, int iTo) const;

	int					numflexdesc;
	int					flexdescindex;
	inline mstudioflexdesc_t* pFlexdesc(int i) const { Assert(i >= 0 && i < numflexdesc); return (mstudioflexdesc_t*)(((unsigned char*)this) + flexdescindex) + i; };

	int					numflexcontrollers;
	int					flexcontrollerindex;
	inline mstudioflexcontroller_t* pFlexcontroller(int i) const { Assert(numflexcontrollers == 0 || (i >= 0 && i < numflexcontrollers)); return (mstudioflexcontroller_t*)(((unsigned char*)this) + flexcontrollerindex) + i; };

	int					numflexrules;
	int					flexruleindex;
	inline mstudioflexrule_t* pFlexRule(int i) const { Assert(i >= 0 && i < numflexrules); return (mstudioflexrule_t*)(((unsigned char*)this) + flexruleindex) + i; };

	int					numikchains;
	int					ikchainindex;
	inline mstudioikchain_t* pIKChain(int i) const { Assert(i >= 0 && i < numikchains); return (mstudioikchain_t*)(((unsigned char*)this) + ikchainindex) + i; };

	int					nummouths;
	int					mouthindex;
	inline mstudiomouth_t* pMouth(int i) const { Assert(i >= 0 && i < nummouths); return (mstudiomouth_t*)(((unsigned char*)this) + mouthindex) + i; };

	//private:
	int					numlocalposeparameters;
	int					localposeparamindex;
	inline mstudioposeparamdesc_t* pLocalPoseParameter(int i) const { Assert(i >= 0 && i < numlocalposeparameters); return (mstudioposeparamdesc_t*)(((unsigned char*)this) + localposeparamindex) + i; };
	//public:
	int					GetNumPoseParameters(void) const;
	const mstudioposeparamdesc_t& pPoseParameter(int i);
	int					GetSharedPoseParameter(int iSequence, int iLocalPose) const;

	int					surfacepropindex;
	inline char* const pszSurfaceProp(void) const { return ((char*)this) + surfacepropindex; }

	// Key values
	int					keyvalueindex;
	int					keyvaluesize;
	inline const char* KeyValueText(void) const { return keyvaluesize != 0 ? ((char*)this) + keyvalueindex : 0; }

	int					numlocalikautoplaylocks;
	int					localikautoplaylockindex;
	inline mstudioiklock_t* pLocalIKAutoplayLock(int i) const { Assert(i >= 0 && i < numlocalikautoplaylocks); return (mstudioiklock_t*)(((unsigned char*)this) + localikautoplaylockindex) + i; };

	int					GetNumIKAutoplayLocks(void) const;
	const mstudioiklock_t& pIKAutoplayLock(int i);
	int					CountAutoplaySequences() const;
	int					CopyAutoplaySequences(unsigned short* pOut, int outCount) const;
	int					GetAutoplayList(unsigned short** pOut) const;

	// The collision model mass that jay wanted
	float				mass;
	int					contents;

	// external animations, models, etc.
	int					numincludemodels;
	int					includemodelindex;
	inline mstudiomodelgroup_t* pModelGroup(int i) const { Assert(i >= 0 && i < numincludemodels); return (mstudiomodelgroup_t*)(((unsigned char*)this) + includemodelindex) + i; };
	// implementation specific call to get a named model
	const studiohdr_t* FindModel(void** cache, char const* modelname) const;

	// implementation specific back pointer to virtual data
	mutable void* virtualModel;
	virtualmodel_t* GetVirtualModel(void) const;

	// for demand loaded animation blocks
	int					szanimblocknameindex;
	inline char* const pszAnimBlockName(void) const { return ((char*)this) + szanimblocknameindex; }
	int					numanimblocks;
	int					animblockindex;
	inline mstudioanimblock_t* pAnimBlock(int i) const { Assert(i > 0 && i < numanimblocks); return (mstudioanimblock_t*)(((unsigned char*)this) + animblockindex) + i; };
	mutable void* animblockModel;
	byte* GetAnimBlock(int i) const;

	int					bonetablebynameindex;
	inline const byte* GetBoneTableSortedByName() const { return (unsigned char*)this + bonetablebynameindex; }

	// used by tools only that don't cache, but persist mdl's peer data
	// engine uses virtualModel to back link to cache pointers
	void* pVertexBase;
	void* pIndexBase;

	// if STUDIOHDR_FLAGS_CONSTANT_DIRECTIONAL_LIGHT_DOT is set,
	// this value is used to calculate directional components of lighting 
	// on static props
	byte				constdirectionallightdot;

	// set during load of mdl data to track *desired* lod configuration (not actual)
	// the *actual* clamped root lod is found in studiohwdata
	// this is stored here as a global store to ensure the staged loading matches the rendering
	byte				rootLOD;

	// set in the mdl data to specify that lod configuration should only allow first numAllowRootLODs
	// to be set as root LOD:
	//	numAllowedRootLODs = 0	means no restriction, any lod can be set as root lod.
	//	numAllowedRootLODs = N	means that lod0 - lod(N-1) can be set as root lod, but not lodN or lower.
	byte				numAllowedRootLODs;

	byte				unused[1];

	int					unused4; // zero out if version < 47

	int					numflexcontrollerui;
	int					flexcontrolleruiindex;
	mstudioflexcontrollerui_t* pFlexControllerUI(int i) const { Assert(i >= 0 && i < numflexcontrollerui); return (mstudioflexcontrollerui_t*)(((byte*)this) + flexcontrolleruiindex) + i; }

	float				flVertAnimFixedPointScale;
	inline float		VertAnimFixedPointScale() const { return (flags & STUDIOHDR_FLAGS_VERT_ANIM_FIXED_POINT_SCALE) ? flVertAnimFixedPointScale : 1.0f / 4096.0f; }

	int					unused3[1];

	// FIXME: Remove when we up the model version. Move all fields of studiohdr2_t into studiohdr_t.
	int					studiohdr2index;
	studiohdr2_t* pStudioHdr2() const { return (studiohdr2_t*)(((byte*)this) + studiohdr2index); }

	// Src bone transforms are transformations that will convert .dmx or .smd-based animations into .mdl-based animations
	int					NumSrcBoneTransforms() const { return studiohdr2index ? pStudioHdr2()->numsrcbonetransform : 0; }
	const void* SrcBoneTransform(int i) const { Assert(i >= 0 && i < NumSrcBoneTransforms()); return (void**)(((byte*)this) + pStudioHdr2()->srcbonetransformindex) + i; }

	inline int			IllumPositionAttachmentIndex() const { return studiohdr2index ? pStudioHdr2()->IllumPositionAttachmentIndex() : 0; }

	inline float		MaxEyeDeflection() const { return studiohdr2index ? pStudioHdr2()->MaxEyeDeflection() : 0.866f; } // default to cos(30) if not set

	inline mstudiolinearbone_t* pLinearBones() const { return studiohdr2index ? pStudioHdr2()->pLinearBones() : 0; }

	inline int			BoneFlexDriverCount() const { return studiohdr2index ? pStudioHdr2()->m_nBoneFlexDriverCount : 0; }
	inline const mstudioboneflexdriver_t* BoneFlexDriver(int i) const { Assert(i >= 0 && i < BoneFlexDriverCount()); return studiohdr2index ? pStudioHdr2()->pBoneFlexDriver(i) : 0; }

	// NOTE: No room to add stuff? Up the .mdl file format version 
	// [and move all fields in studiohdr2_t into studiohdr_t and kill studiohdr2_t],
	// or add your stuff to studiohdr2_t. See NumSrcBoneTransforms/SrcBoneTransform for the pattern to use.
	int					unused2[1];

	studiohdr_t() {}

private:
	// No copy constructors allowed
	studiohdr_t(const studiohdr_t& vOther);

	friend struct virtualmodel_t;
};

class CStudioHdr
{
public:
	CStudioHdr(void);
	CStudioHdr(const studiohdr_t* pStudioHdr, void* mdlcache = 0);
	~CStudioHdr() { Term(); }

	void Init(const studiohdr_t* pStudioHdr, void* mdlcache = 0);
	void Term();

public:
	inline bool IsVirtual(void) { return (m_pVModel != 0); };
	inline bool IsValid(void) { return (m_pStudioHdr != 0); };
	inline bool IsReadyForAccess(void) const { return (m_pStudioHdr != 0); };
	inline virtualmodel_t* GetVirtualModel(void) const { return m_pVModel; };
	inline const studiohdr_t* GetRenderHdr(void) const { return m_pStudioHdr; };
	const studiohdr_t* pSeqStudioHdr(int sequence);
	const studiohdr_t* pAnimStudioHdr(int animation);

private:
	mutable const studiohdr_t* m_pStudioHdr;
	mutable virtualmodel_t* m_pVModel;

	const virtualmodel_t* ResetVModel(const virtualmodel_t* pVModel) const;
	const studiohdr_t* GroupStudioHdr(int group);
	mutable void* m_pStudioHdrCache;

	mutable int			m_nFrameUnlockCounter;
	int* m_pFrameUnlockCounter;
	void*	m_FrameUnlockCounterMutex;

public:
	inline int			numbones(void) const { return m_pStudioHdr->numbones; };
	inline mstudiobone_t* pBone(int i) const { return m_pStudioHdr->pBone(i); };
	int					RemapAnimBone(int iAnim, int iLocalBone) const;		// maps local animations bone to global bone
	int					RemapSeqBone(int iSequence, int iLocalBone) const;	// maps local sequence bone to global bone

	bool				SequencesAvailable() const;
	int					GetNumSeq(void) const;
	mstudioanimdesc_t& pAnimdesc(int i);
	mstudioseqdesc_t& pSeqdesc(int iSequence);
	int					iRelativeAnim(int baseseq, int relanim) const;	// maps seq local anim reference to global anim index
	int					iRelativeSeq(int baseseq, int relseq) const;		// maps seq local seq reference to global seq index

	int					GetSequenceActivity(int iSequence);
	void				SetSequenceActivity(int iSequence, int iActivity);
	int					GetActivityListVersion(void);
	void				SetActivityListVersion(int version);
	int					GetEventListVersion(void);
	void				SetEventListVersion(int version);

	int					GetNumAttachments(void) const;
	const mstudioattachment_t& pAttachment(int i);
	int					GetAttachmentBone(int i);
	// used on my tools in hlmv, not persistant
	void				SetAttachmentBone(int iAttachment, int iBone);

	int					EntryNode(int iSequence);
	int					ExitNode(int iSequence);
	char* pszNodeName(int iNode);
	// FIXME: where should this one be?
	int					GetTransition(int iFrom, int iTo) const;

	int					GetNumPoseParameters(void) const;
	const mstudioposeparamdesc_t& pPoseParameter(int i);
	int					GetSharedPoseParameter(int iSequence, int iLocalPose) const;

	int					GetNumIKAutoplayLocks(void) const;
	const mstudioiklock_t& pIKAutoplayLock(int i);

	inline int			CountAutoplaySequences() const { return m_pStudioHdr->CountAutoplaySequences(); };
	inline int			CopyAutoplaySequences(unsigned short* pOut, int outCount) const { return m_pStudioHdr->CopyAutoplaySequences(pOut, outCount); };
	inline int			GetAutoplayList(unsigned short** pOut) const { return m_pStudioHdr->GetAutoplayList(pOut); };

	inline int			GetNumBoneControllers(void) const { return m_pStudioHdr->numbonecontrollers; };
	inline mstudiobonecontroller_t* pBonecontroller(int i) const { return m_pStudioHdr->pBonecontroller(i); };

	inline int			numikchains() const { return m_pStudioHdr->numikchains; };
	inline int			GetNumIKChains(void) const { return m_pStudioHdr->numikchains; };
	inline mstudioikchain_t* pIKChain(int i) const { return m_pStudioHdr->pIKChain(i); };

	inline int			numflexrules() const { return m_pStudioHdr->numflexrules; };
	inline mstudioflexrule_t* pFlexRule(int i) const { return m_pStudioHdr->pFlexRule(i); };

	inline int			numflexdesc() const { return m_pStudioHdr->numflexdesc; };
	inline mstudioflexdesc_t* pFlexdesc(int i) const { return m_pStudioHdr->pFlexdesc(i); };

	inline void*			numflexcontrollers() const { return (void*)m_pStudioHdr->numflexcontrollers; };
	inline mstudioflexcontroller_t* pFlexcontroller(int i) const { return m_pStudioHdr->pFlexcontroller(i); };

	inline int			numflexcontrollerui() const { return m_pStudioHdr->numflexcontrollerui; };
	inline mstudioflexcontrollerui_t* pFlexcontrollerUI(int i) const { return m_pStudioHdr->pFlexControllerUI(i); };

	//inline const char	*name() const { return m_pStudioHdr->name; }; // deprecated -- remove after full xbox merge
	inline const char* pszName() const { return m_pStudioHdr->pszName(); };

	inline int			numbonecontrollers() const { return m_pStudioHdr->numbonecontrollers; };

	inline int			numhitboxsets() const { return m_pStudioHdr->numhitboxsets; };
	inline mstudiohitboxset_t* pHitboxSet(int i) const { return m_pStudioHdr->pHitboxSet(i); };

	inline mstudiobbox_t* pHitbox(int i, int set) const { return m_pStudioHdr->pHitbox(i, set); };
	inline int			iHitboxCount(int set) const { return m_pStudioHdr->iHitboxCount(set); };

	inline int			numbodyparts() const { return m_pStudioHdr->numbodyparts; };
	inline mstudiobodyparts_t* pBodypart(int i) const { return m_pStudioHdr->pBodypart(i); };

	inline int			numskinfamilies() const { return m_pStudioHdr->numskinfamilies; }

	inline Vector		eyeposition() const { return m_pStudioHdr->eyeposition; };

	inline int			flags() const { return m_pStudioHdr->flags; };

	inline char* const pszSurfaceProp(void) const { return m_pStudioHdr->pszSurfaceProp(); };

	inline float		mass() const { return m_pStudioHdr->mass; };
	inline int			contents() const { return m_pStudioHdr->contents; }

	inline const byte* GetBoneTableSortedByName() const { return m_pStudioHdr->GetBoneTableSortedByName(); };

	inline Vector		illumposition() const { return m_pStudioHdr->illumposition; };

	inline Vector		hull_min() const { return m_pStudioHdr->hull_min; };		// ideal movement hull size
	inline Vector		hull_max() const { return m_pStudioHdr->hull_max; };

	inline Vector		view_bbmin() const { return m_pStudioHdr->view_bbmin; };		// clipping bounding box
	inline Vector		view_bbmax() const { return m_pStudioHdr->view_bbmax; };

	inline int			numtextures() const { return m_pStudioHdr->numtextures; };

	inline int			IllumPositionAttachmentIndex() const { return m_pStudioHdr->IllumPositionAttachmentIndex(); }

	inline float		MaxEyeDeflection() const { return m_pStudioHdr->MaxEyeDeflection(); }

	inline mstudiolinearbone_t* pLinearBones() const { return m_pStudioHdr->pLinearBones(); }

	inline int			BoneFlexDriverCount() const { return m_pStudioHdr->BoneFlexDriverCount(); }
	inline const mstudioboneflexdriver_t* BoneFlexDriver(int i) const { return m_pStudioHdr->BoneFlexDriver(i); }

	inline float		VertAnimFixedPointScale() const { return m_pStudioHdr->VertAnimFixedPointScale(); }

public:
	int IsSequenceLooping(int iSequence);
	float GetSequenceCycleRate(int iSequence);

	void				RunFlexRules(const float* src, float* dest);


public:
	inline int boneFlags(int iBone) const { return *(int*)((int)m_boneFlags + iBone); }
	inline int boneParent(int iBone) const { return *(int*)((int)m_boneParent + iBone); }

private:
	void* m_boneFlags;
	void* m_boneParent;

public:
	// This class maps an activity to sequences allowed for that activity, accelerating the resolution
	// of SelectWeightedSequence(), especially on PowerPC. Iterating through every sequence
	// attached to a model turned out to be a very destructive cache access pattern on 360.
	// 
	// I've encapsulated this behavior inside a nested class for organizational reasons; there is
	// no particular programmatic or efficiency benefit to it. It just makes clearer what particular
	// code in the otherwise very complicated StudioHdr class has to do with this particular
	// optimization, and it lets you collapse the whole definition down to a single line in Visual
	// Studio.
	class CActivityToSequenceMapping /* final */
	{
	public:
		// A tuple of a sequence and its corresponding weight. Lists of these correspond to activities.
		struct SequenceTuple
		{
			short seqnum;
			short weight; // the absolute value of the weight from the sequence header
		};

		// The type of the hash's stored data, a composite of both key and value
		// (because that's how CUtlHash works):
		// key: an int, the activity #
		// values: an index into the m_pSequenceTuples array, a count of the
		// total sequences present for an activity, and the sum of their
		// weights.
		// Note this struct is 128-bits wide, exactly coincident to a PowerPC 
		// cache line and VMX register. Please consider very carefully the
		// performance implications before adding any additional fields to this.
		// You could probably do away with totalWeight if you really had to.
		struct HashValueType
		{
			// KEY (hashed)
			int activityIdx;

			// VALUE (not hashed)
			int startingIdx;
			int count;
			int totalWeight;

			HashValueType(int _actIdx, int _stIdx, int _ct, int _tW) :
				activityIdx(_actIdx), startingIdx(_stIdx), count(_ct), totalWeight(_tW) {}

			// default constructor (ought not to be actually used)
			HashValueType() : activityIdx(-1), startingIdx(-1), count(-1), totalWeight(-1)
			{
			}


			class HashFuncs
			{
			public:
				// dummy constructor (gndn)
				HashFuncs(int) {}

				// COMPARE
				// compare two entries for uniqueness. We should never have two different
				// entries for the same activity, so we only compare the activity index;
				// this allows us to use the utlhash as a dict by constructing dummy entries
				// as hash lookup keys.
				bool operator()(const HashValueType& lhs, const HashValueType& rhs) const
				{
					return lhs.activityIdx == rhs.activityIdx;
				}

			};
		};

		// These must be here because IFM does not compile/link studio.cpp (?!?)

		// ctor
		CActivityToSequenceMapping(void);

		// dtor -- not virtual because this class has no inheritors
		~CActivityToSequenceMapping()
		{
			if (m_pSequenceTuples != 0)
			{
				delete[] m_pSequenceTuples;
			}
		}

		/// Get the list of sequences for an activity. Returns the pointer to the
		/// first sequence tuple. Output parameters are a count of sequences present,
		/// and the total weight of all the sequences. (it would be more LHS-friendly
		/// to return these on registers, if only C++ offered more than one return 
		/// value....)
		const SequenceTuple* GetSequences(int forActivity, int* outSequenceCount, int* outTotalWeight);

		/// The number of sequences available for an activity.
		int NumSequencesForActivity(int forActivity);

#if STUDIO_SEQUENCE_ACTIVITY_LAZY_INITIALIZE
		inline bool IsInitialized(void) { return m_bIsInitialized; }
#endif

	private:

		/// Allocate my internal array. (It is freed in the destructor.) Also,
		/// build the hash of activities to sequences and populate m_pSequenceTuples.
		void Initialize(CStudioHdr* pstudiohdr);

		/// Force Initialize() to occur again, even if it has already occured.
		void Reinitialize(CStudioHdr* pstudiohdr);

		/// A more efficient version of the old SelectWeightedSequence() function in animation.cpp. 
		int SelectWeightedSequence(CStudioHdr* pstudiohdr, int activity, int curSequence);

		// Actually a big array, into which the hash values index.
		SequenceTuple* m_pSequenceTuples;
		unsigned int m_iSequenceTuplesCount; // (size of the whole array)
#if STUDIO_SEQUENCE_ACTIVITY_LAZY_INITIALIZE
		bool m_bIsInitialized;
#endif

		// we don't store an outer pointer because we can't initialize it at construction time
		// (warning c4355) -- there are ways around this but it's easier to just pass in a 
		// pointer to the CStudioHdr when we need it, since this class isn't supposed to 
		// export its interface outside the studio header anyway.
		// CStudioHdr * const m_pOuter;

		void* m_ActToSeqHash;

		// we store these so we can know if the contents of the studiohdr have changed
		// from underneath our feet (this is an emergency data integrity check)
		const void* m_expectedPStudioHdr;
		const void* m_expectedVModel;

		// double-check that the data I point to hasn't changed
		bool ValidateAgainst(const CStudioHdr* pstudiohdr);
		void SetValidationPair(const CStudioHdr* pstudiohdr);

		friend class CStudioHdr;
	};

	CActivityToSequenceMapping m_ActivityToSequence;

	/// A more efficient version of the old SelectWeightedSequence() function in animation.cpp. 
	/// Returns -1 on failure to find a sequence
	inline int SelectWeightedSequence(int activity, int curSequence)
	{
#if STUDIO_SEQUENCE_ACTIVITY_LAZY_INITIALIZE
		// We lazy-initialize the header on demand here, because CStudioHdr::Init() is
		// called from the constructor, at which time the this pointer is illegitimate.
		if (!m_ActivityToSequence.IsInitialized())
		{
			m_ActivityToSequence.Initialize(this);
		}
#endif
		return m_ActivityToSequence.SelectWeightedSequence(this, activity, curSequence);
	}

	/// True iff there is at least one sequence for the given activity.
	inline bool HaveSequenceForActivity(int activity)
	{
#if STUDIO_SEQUENCE_ACTIVITY_LAZY_INITIALIZE
		if (!m_ActivityToSequence.IsInitialized())
		{
			m_ActivityToSequence.Initialize(this);
		}
#endif
		return (m_ActivityToSequence.NumSequencesForActivity(activity) > 0);
	}

	// Force this CStudioHdr's activity-to-sequence mapping to be reinitialized
	inline void ReinitializeSequenceMapping(void)
	{
		m_ActivityToSequence.Reinitialize(this);
	}

#ifdef STUDIO_ENABLE_PERF_COUNTERS
public:
	inline void			ClearPerfCounters(void)
	{
		m_nPerfAnimatedBones = 0;
		m_nPerfUsedBones = 0;
		m_nPerfAnimationLayers = 0;
	};

	// timing info
	mutable	int			m_nPerfAnimatedBones;
	mutable	int			m_nPerfUsedBones;
	mutable	int			m_nPerfAnimationLayers;
#endif


};
