/*
 Reminder:
 Use CTRL+F to search for "MODIFICATION" and easily spot what's custom 
 versus what's original Valve code.
*/

//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef NPC_COMBINE_H
#define NPC_COMBINE_H
#ifdef _WIN32
#pragma once
#endif

#include "ai_basenpc.h"
#include "ai_basehumanoid.h"
#include "ai_behavior.h"
#include "ai_behavior_assault.h"
#include "ai_behavior_standoff.h"
#include "ai_behavior_follow.h"
#include "ai_behavior_functank.h"
#include "ai_behavior_rappel.h"
#include "ai_behavior_actbusy.h"
#include "ai_sentence.h"
#include "ai_baseactor.h"
#ifdef MAPBASE
#include "mapbase/ai_grenade.h"
#include "ai_behavior_police.h"
#endif
#ifdef EXPANDED_RESPONSE_SYSTEM_USAGE
#include "mapbase/expandedrs_combine.h"
//#define CAI_Sentence CAI_SentenceTalker
#define COMBINE_SOLDIER_USES_RESPONSE_SYSTEM 1
#endif

// Used when only what combine to react to what the spotlight sees
#define SF_COMBINE_NO_LOOK	(1 << 16)
#define SF_COMBINE_NO_GRENADEDROP ( 1 << 17 )
#define SF_COMBINE_NO_AR2DROP ( 1 << 18 )

//=========================================================
//	>> CNPC_Combine
//=========================================================
#ifdef MAPBASE
class CNPC_Combine : public CAI_GrenadeUser<CAI_BaseActor>
{
	DECLARE_DATADESC();
	DEFINE_CUSTOM_AI;
	DECLARE_CLASS(CNPC_Combine, CAI_GrenadeUser<CAI_BaseActor>);
#else
class CNPC_Combine : public CAI_BaseActor
{
	DECLARE_DATADESC();
	DEFINE_CUSTOM_AI;
	DECLARE_CLASS(CNPC_Combine, CAI_BaseActor);
#endif

public:
	CNPC_Combine();

	// Create components
	virtual bool	CreateComponents();

#ifndef MAPBASE // CAI_GrenadeUser
	bool			CanThrowGrenade(const Vector& vecTarget);
	bool			CheckCanThrowGrenade(const Vector& vecTarget);
#endif
	virtual	bool	CanGrenadeEnemy(bool bUseFreeKnowledge = true);
	virtual bool	CanAltFireEnemy(bool bUseFreeKnowledge);
	int				GetGrenadeConditions(float flDot, float flDist);
	int				RangeAttack2Conditions(float flDot, float flDist); // For innate grenade attack
	int				MeleeAttack1Conditions(float flDot, float flDist); // For kick/punch
	bool			FVisible(CBaseEntity* pEntity, int traceMask = MASK_BLOCKLOS, CBaseEntity** ppBlocker = NULL);
	virtual bool	IsCurTaskContinuousMove();

	virtual float	GetJumpGravity() const { return 1.8f; }

	virtual Vector  GetCrouchEyeOffset(void);

#ifdef MAPBASE
	virtual bool	IsCrouchedActivity(Activity activity);
#endif

	void Event_Killed(const CTakeDamageInfo& info);


	void SetActivity(Activity NewActivity);
	NPC_STATE		SelectIdealState(void);

	// Input handlers.
	void InputLookOn(inputdata_t& inputdata);
	void InputLookOff(inputdata_t& inputdata);
	void InputStartPatrolling(inputdata_t& inputdata);
	void InputStopPatrolling(inputdata_t& inputdata);
	void InputAssault(inputdata_t& inputdata);
	void InputHitByBugbait(inputdata_t& inputdata);
#ifndef MAPBASE
	void InputThrowGrenadeAtTarget(inputdata_t& inputdata);
#else
	void InputSetElite(inputdata_t& inputdata);

	void InputDropGrenade(inputdata_t& inputdata);

	void InputSetTacticalVariant(inputdata_t& inputdata);

	void InputSetPoliceGoal(inputdata_t& inputdata);
#endif

	bool			UpdateEnemyMemory(CBaseEntity* pEnemy, const Vector& position, CBaseEntity* pInformer = NULL);

	void			Spawn(void);
	void			Precache(void);
	void			Activate();

	Class_T			Classify(void);
	bool			IsElite() { return m_fIsElite; }
#ifdef MAPBASE
	bool			IsAltFireCapable();
	bool			IsGrenadeCapable();
	const char* GetGrenadeAttachment() { return "lefthand"; }
#else
#endif
#ifndef MAPBASE // CAI_GrenadeUser
	void			DelayAltFireAttack(float flDelay);
	void			DelaySquadAltFireAttack(float flDelay);
#endif
	float			MaxYawSpeed(void);
	bool			ShouldMoveAndShoot();
	bool			OverrideMoveFacing(const AILocalMoveGoal_t& move, float flInterval);;
	void			HandleAnimEvent(animevent_t* pEvent);
	Vector			Weapon_ShootPosition();

	Vector			EyeOffset(Activity nActivity);
	Vector			EyePosition(void);
	Vector			BodyTarget(const Vector& posSrc, bool bNoisy = true);
#ifndef MAPBASE // CAI_GrenadeUser
	Vector			GetAltFireTarget();
#endif

	void			StartTask(const Task_t* pTask);
	void			RunTask(const Task_t* pTask);
	void			PostNPCInit();
	void			GatherConditions();
	virtual void	PrescheduleThink();

#ifdef MAPBASE
	Activity		Weapon_TranslateActivity(Activity baseAct, bool* pRequired = NULL);
	Activity		NPC_BackupActivity(Activity eNewActivity);
#endif
	Activity		NPC_TranslateActivity(Activity eNewActivity);
	void			BuildScheduleTestBits(void);
	virtual int		SelectSchedule(void);
	virtual int		SelectFailSchedule(int failedSchedule, int failedTask, AI_TaskFailureCode_t taskFailCode);
	int				SelectScheduleAttack();

	bool			CreateBehaviors();

	bool			OnBeginMoveAndShoot();
	void			OnEndMoveAndShoot();

	// Combat
	WeaponProficiency_t CalcWeaponProficiency(CBaseCombatWeapon* pWeapon);
	bool			HasShotgun();
	bool			ActiveWeaponIsFullyLoaded();

	bool			HandleInteraction(int interactionType, void* data, CBaseCombatCharacter* sourceEnt);
	const char* GetSquadSlotDebugName(int iSquadSlot);

	bool			IsUsingTacticalVariant(int variant);
	bool			IsUsingPathfindingVariant(int variant) { return m_iPathfindingVariant == variant; }

	bool			IsRunningApproachEnemySchedule();

	// -------------
	// Sounds
	// -------------
#ifdef MAPBASE
	void			DeathSound(const CTakeDamageInfo& info);
	void			PainSound(const CTakeDamageInfo& info);
#else
	void			DeathSound(void);
	void			PainSound(void);
#endif
	void			IdleSound(void);
	void			AlertSound(void);
#ifdef MAPBASE
	void			LostEnemySound(CBaseEntity* pEnemy);
	void			FoundEnemySound(CBaseEntity* pEnemy);
#else
	void			LostEnemySound(void);
	void			FoundEnemySound(void);
#endif
	void			AnnounceAssault(void);
	void			AnnounceEnemyType(CBaseEntity* pEnemy);
	void			AnnounceEnemyKill(CBaseEntity* pEnemy);

	void			NotifyDeadFriend(CBaseEntity* pFriend);

	virtual float	HearingSensitivity(void) { return 1.0; };
	int				GetSoundInterests(void);
	virtual bool	QueryHearSound(CSound* pSound);

	// Speaking
	void			SpeakSentence(int sentType);
#ifdef COMBINE_SOLDIER_USES_RESPONSE_SYSTEM
	bool			SpeakIfAllowed(const char* concept, SentencePriority_t sentencepriority = SENTENCE_PRIORITY_NORMAL, SentenceCriteria_t sentencecriteria = SENTENCE_CRITERIA_IN_SQUAD)
	{
		return SpeakIfAllowed(concept, NULL, sentencepriority, sentencecriteria);
	}
	bool			SpeakIfAllowed(const char* concept, const char* modifiers, SentencePriority_t sentencepriority = SENTENCE_PRIORITY_NORMAL, SentenceCriteria_t sentencecriteria = SENTENCE_CRITERIA_IN_SQUAD);
	bool			SpeakIfAllowed(const char* concept, AI_CriteriaSet& modifiers, SentencePriority_t sentencepriority = SENTENCE_PRIORITY_NORMAL, SentenceCriteria_t sentencecriteria = SENTENCE_CRITERIA_IN_SQUAD);
	void			ModifyOrAppendCriteria(AI_CriteriaSet& set);
#endif

	virtual int		TranslateSchedule(int scheduleType);
	void			OnStartSchedule(int scheduleType);

	virtual bool	ShouldPickADeathPose(void);

protected:
	void			SetKickDamage(int nDamage) { m_nKickDamage = nDamage; }
#ifndef COMBINE_SOLDIER_USES_RESPONSE_SYSTEM
	CAI_Sentence< CNPC_Combine >* GetSentences() { return &m_Sentences; }
#endif

private:
	//=========================================================
	// Combine S schedules
	//=========================================================
	enum
	{
		SCHED_COMBINE_SUPPRESS = BaseClass::NEXT_SCHEDULE,
		SCHED_COMBINE_COMBAT_FAIL,
		SCHED_COMBINE_VICTORY_DANCE,
		SCHED_COMBINE_COMBAT_FACE,
		SCHED_COMBINE_HIDE_AND_RELOAD,
		SCHED_COMBINE_SIGNAL_SUPPRESS,
		SCHED_COMBINE_ENTER_OVERWATCH,
		SCHED_COMBINE_OVERWATCH,
		SCHED_COMBINE_ASSAULT,
		SCHED_COMBINE_ESTABLISH_LINE_OF_FIRE,
		SCHED_COMBINE_PRESS_ATTACK,
		SCHED_COMBINE_WAIT_IN_COVER,
		SCHED_COMBINE_RANGE_ATTACK1,
		SCHED_COMBINE_RANGE_ATTACK2,
		SCHED_COMBINE_TAKE_COVER1,
		SCHED_COMBINE_TAKE_COVER_FROM_BEST_SOUND,
		SCHED_COMBINE_RUN_AWAY_FROM_BEST_SOUND,
		SCHED_COMBINE_GRENADE_COVER1,
		SCHED_COMBINE_TOSS_GRENADE_COVER1,
		SCHED_COMBINE_TAKECOVER_FAILED,
		SCHED_COMBINE_GRENADE_AND_RELOAD,
		SCHED_COMBINE_PATROL,
		SCHED_COMBINE_BUGBAIT_DISTRACTION,
		SCHED_COMBINE_CHARGE_TURRET,
		SCHED_COMBINE_DROP_GRENADE,
		SCHED_COMBINE_CHARGE_PLAYER,
		SCHED_COMBINE_PATROL_ENEMY,
		SCHED_COMBINE_BURNING_STAND,
		SCHED_COMBINE_AR2_ALTFIRE,
		SCHED_COMBINE_FORCED_GRENADE_THROW,
		SCHED_COMBINE_MOVE_TO_FORCED_GREN_LOS,
		SCHED_COMBINE_FACE_IDEAL_YAW,
		SCHED_COMBINE_MOVE_TO_MELEE,
		SCHED_COMBINE_RETURN_GRENADE,		// [MODIFICATION] The schedule for the "Return to Sender" ability (catching and throwing back)..
		SCHED_COMBINE_KICK_GRENADE,		   // // [MODIFICATION] The schedule for the alternative grenade kick ability.
		NEXT_SCHEDULE,
	};

	//=========================================================
	// Combine Tasks
	//=========================================================
	enum
	{
		TASK_COMBINE_FACE_TOSS_DIR = BaseClass::NEXT_TASK,
		TASK_COMBINE_IGNORE_ATTACKS,
		TASK_COMBINE_SIGNAL_BEST_SOUND,
		TASK_COMBINE_DEFER_SQUAD_GRENADES,
		TASK_COMBINE_CHASE_ENEMY_CONTINUOUSLY,
		TASK_COMBINE_DIE_INSTANTLY,
		TASK_COMBINE_PLAY_SEQUENCE_FACE_ALTFIRE_TARGET,
		TASK_COMBINE_GET_PATH_TO_FORCED_GREN_LOS,
		TASK_COMBINE_SET_STANDING,
		TASK_COMBINE_PULL_GRENADE,		// [MODIFICATION] A long-running task to physically pull a nearby grenade.
		TASK_COMBINE_LAUNCH_GRENADE,	// [MODIFICATION] An instantaneous task that launches the caught grenade back.
		TASK_COMBINE_FACE_GRENADE,		// [MODIFICATION] A stateful task that makes the Combine perform a high-speed turn to face the grenade.
		TASK_COMBINE_KICK_LAUNCH,		// [MODIFICATION] An instantaneous task that applies kick physics to a nearby grenade.
		TASK_COMBINE_MEMORIZE_GRENADE,	// [MODIFICATION] An instantaneous task that "memorizes" the grenade as a target for the kick schedule.



		NEXT_TASK
	};

	//=========================================================
	// Combine Conditions
	//=========================================================
	enum Combine_Conds
	{
		COND_COMBINE_NO_FIRE = BaseClass::NEXT_CONDITION,
		COND_COMBINE_DEAD_FRIEND,
		COND_COMBINE_SHOULD_PATROL,
		COND_COMBINE_HIT_BY_BUGBAIT,
		COND_COMBINE_DROP_GRENADE,
		COND_COMBINE_ON_FIRE,
		COND_COMBINE_ATTACK_SLOT_AVAILABLE,
		NEXT_CONDITION
	};

private:
	// Select the combat schedule
	int SelectCombatSchedule();

	// Should we charge the player?
	bool ShouldChargePlayer();

	// Chase the enemy, updating the target position as the player moves
	void StartTaskChaseEnemyContinuously(const Task_t* pTask);
	void RunTaskChaseEnemyContinuously(const Task_t* pTask);

	class CCombineStandoffBehavior : public CAI_ComponentWithOuter<CNPC_Combine, CAI_StandoffBehavior>
	{
		typedef CAI_ComponentWithOuter<CNPC_Combine, CAI_StandoffBehavior> BaseClass;

		virtual int SelectScheduleAttack()
		{
			int result = GetOuter()->SelectScheduleAttack();
			if (result == SCHED_NONE)
				result = BaseClass::SelectScheduleAttack();
			return result;
		}
	};

	// Rappel
	virtual bool IsWaitingToRappel(void) { return m_RappelBehavior.IsWaitingToRappel(); }
	void BeginRappel() { m_RappelBehavior.BeginRappel(); }

private:
	EHANDLE			m_hPulledObject;			// [MODIFICATION] Safely stores a handle to the grenade being manipulated.
	bool			m_bIsReturningGrenade;		// [MODIFICATION] State flag; true when a special ability is active. Used to block the default grenade throw AnimEvent.
	float			m_flNextGrenadeCatchTime;	// [MODIFICATION] Shared cooldown timer for all special grenade abilities to prevent loops.
	bool			m_bForceFastTurn;			// [MODIFICATION] A temporary flag to enable a high-speed "turbo" turn, used by the MaxYawSpeed() override.
	int				m_nKickDamage;
#ifndef MAPBASE // CAI_GrenadeUser
	Vector			m_vecTossVelocity;
	EHANDLE			m_hForcedGrenadeTarget;
#else
	// Underthrow grenade at target
	bool			m_bUnderthrow;
	bool			m_bAlternateCapable;
#endif
	bool			m_bShouldPatrol;
	bool			m_bFirstEncounter;// only put on the handsign show in the squad's first encounter.

	// Time Variables
	float			m_flNextPainSoundTime;
	float			m_flNextAlertSoundTime;
#ifndef MAPBASE // CAI_GrenadeUser
	float			m_flNextGrenadeCheck;
#endif
	float			m_flNextLostSoundTime;
	float			m_flAlertPatrolTime;		// When to stop doing alert patrol
#ifndef MAPBASE // CAI_GrenadeUser
	float			m_flNextAltFireTime;		// Elites only. Next time to begin considering alt-fire attack.
#endif

	int				m_nShots;
	float			m_flShotDelay;
	float			m_flStopMoveShootTime;

#ifndef COMBINE_SOLDIER_USES_RESPONSE_SYSTEM
	CAI_Sentence< CNPC_Combine > m_Sentences;
#endif

#ifndef MAPBASE // CAI_GrenadeUser
	int			m_iNumGrenades;
#endif
	CAI_AssaultBehavior			m_AssaultBehavior;
	CCombineStandoffBehavior	m_StandoffBehavior;
	CAI_FollowBehavior			m_FollowBehavior;
	CAI_FuncTankBehavior		m_FuncTankBehavior;
	CAI_RappelBehavior			m_RappelBehavior;
	CAI_ActBusyBehavior			m_ActBusyBehavior;
#ifdef MAPBASE
	CAI_PolicingBehavior		m_PolicingBehavior;
#endif

public:
#ifndef MAPBASE // CAI_GrenadeUser
	int				m_iLastAnimEventHandled;
#endif
	bool			m_fIsElite;
#ifndef MAPBASE // CAI_GrenadeUser
	Vector			m_vecAltFireTarget;
#endif

	int				m_iTacticalVariant;
	int				m_iPathfindingVariant;

};


#endif // NPC_COMBINE_H
