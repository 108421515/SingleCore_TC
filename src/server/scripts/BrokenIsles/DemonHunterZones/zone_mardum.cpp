/*
 * Copyright (C) 2017-2018 AshamaneProject <https://github.com/AshamaneProject>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Creature.h"
#include "GameObject.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "SceneMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "TemporarySummon.h"

enum eQuests
{
    QUEST_INVASION_BEGIN        = 40077,
    QUEST_ASHTONGUE_FORCES      = 40378,
    QUEST_COILSKAR_FORCES       = 40379,
    QUEST_MEETING_WITH_QUEEN    = 39050,
    QUEST_SHIVARRA_FORCES       = 38765
};

enum eScenes
{
    SPELL_SCENE_MARDUM_WELCOME          = 193525,
    SPELL_SCENE_MARDUM_LEGION_BANNER    = 191677,
    SPELL_SCENE_MARDUM_ASHTONGUE_FORCES = 189261,
    SPELL_SCENE_MARDUM_COILSKAR_FORCES  = 190793,
    SPELL_SCENE_MEETING_WITH_QUEEN      = 188539,
    SPELL_SCENE_MARDUM_SHIVARRA_FORCES  = 190793,
};

enum ePhaseSpells
{
    SPELL_PHASE_170 = 59073,
    SPELL_PHASE_171 = 59074,
    SPELL_PHASE_172 = 59087,
    SPELL_PHASE_173 = 54341,

    SPELL_PHASE_175 = 57569,
    SPELL_PHASE_176 = 74789,
    SPELL_PHASE_177 = 69819,

    SPELL_PHASE_179 = 67789,
    SPELL_PHASE_180 = 68480,
    SPELL_PHASE_181 = 68481
};

enum ePhases
{
    SPELL_PHASE_MARDUM_WELCOME      = SPELL_PHASE_170,
    SPELL_PHASE_MARDUM_AFTER_BANNER = SPELL_PHASE_171,
    SPELL_PHASE_MARDUM_FELSABBER    = SPELL_PHASE_172,
};

class PlayerScript_mardum_welcome_scene_trigger : public PlayerScript
{
public:
    PlayerScript_mardum_welcome_scene_trigger() : PlayerScript("PlayerScript_mardum_welcome_scene_trigger") {}

    uint32 checkTimer = 1000;

    void OnLogin(Player* player, bool firstLogin) override
    {
        if (player->GetZoneId() == 7705 && firstLogin)
        {
            player->RemoveAurasDueToSpell(SPELL_PHASE_MARDUM_WELCOME);
        }
    }

    void OnUpdate(Player* player, uint32 diff) override
    {
        if (checkTimer <= diff)
        {
            if (player->GetZoneId() == 7705 && player->GetQuestStatus(QUEST_INVASION_BEGIN) == QUEST_STATUS_NONE &&
                player->GetPositionY() < 3280 && !player->HasAura(SPELL_SCENE_MARDUM_WELCOME) &&
                !player->HasAura(SPELL_PHASE_MARDUM_WELCOME))
            {
                player->CastSpell(player, SPELL_SCENE_MARDUM_WELCOME, true);
            }

            checkTimer = 1000;
        }
        else checkTimer -= diff;
    }
};

class scene_mardum_welcome : public SceneScript
{
public:
    scene_mardum_welcome() : SceneScript("scene_mardum_welcome") { }

    void OnSceneComplete(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) override
    {
        player->AddAura(SPELL_PHASE_MARDUM_WELCOME);
    }
};

class npc_kayn_sunfury_welcome : public CreatureScript
{
public:
    npc_kayn_sunfury_welcome() : CreatureScript("npc_kayn_sunfury_welcome") { }

    bool OnQuestAccept(Player* /*player*/, Creature* /*creature*/, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_INVASION_BEGIN)
        {
            // Todo : Make creatures wing out
        }
        return true;
    }
};

class go_mardum_legion_banner_1 : public GameObjectScript
{
public:
    go_mardum_legion_banner_1() : GameObjectScript("go_mardum_legion_banner_1") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/) override
    {
        if (!player->GetQuestObjectiveData(QUEST_INVASION_BEGIN, 1))
            player->CastSpell(player, SPELL_SCENE_MARDUM_LEGION_BANNER, true);

        return false;
    }
};

class scene_mardum_change_legion_banner : public SceneScript
{
public:
    scene_mardum_change_legion_banner() : SceneScript("scene_mardum_change_legion_banner") { }

    void OnSceneComplete(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) override
    {
        player->AddAura(SPELL_PHASE_MARDUM_AFTER_BANNER);
    }
};

class go_mardum_portal_ashtongue : public GameObjectScript
{
public:
    go_mardum_portal_ashtongue() : GameObjectScript("go_mardum_portal_ashtongue") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/) override
    {
        if (!player->GetQuestObjectiveData(QUEST_ASHTONGUE_FORCES, 0))
        {
            player->KilledMonsterCredit(88872); // QUEST_ASHTONGUE_FORCES storageIndex 0 KillCredit
            player->KilledMonsterCredit(97831); // QUEST_ASHTONGUE_FORCES storageIndex 1 KillCredit
            player->CastSpell(player, SPELL_SCENE_MARDUM_ASHTONGUE_FORCES, true);
        }

        return false;
    }
};

class scene_mardum_welcome_ashtongue : public SceneScript
{
public:
    scene_mardum_welcome_ashtongue() : SceneScript("scene_mardum_welcome_ashtongue") { }

    void OnSceneTriggerEvent(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/, std::string const& triggerName)
    {
        if (triggerName == "SEEFELSABERCREDIT")
        {
            player->KilledMonsterCredit(101534); // QUEST_ASHTONGUE_FORCES storageIndex 1 KillCredit
        }
        else if (triggerName == "UPDATEPHASE")
        {
            player->AddAura(SPELL_PHASE_MARDUM_FELSABBER);
        }
    }
};

// 200176 - Learn felsaber
class spell_learn_felsaber : public SpellScriptLoader
{
public:
    spell_learn_felsaber() : SpellScriptLoader("spell_learn_felsaber") { }

    class spell_learn_felsaber_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_learn_felsaber_SpellScript);

        void HandleMountOnHit(SpellEffIndex /*effIndex*/)
        {
            GetCaster()->RemoveAurasDueToSpell(SPELL_PHASE_MARDUM_FELSABBER);

            // We schedule this to let hover animation pass
            GetCaster()->GetScheduler().Schedule(Seconds(1), [](TaskContext context)
            {
                Unit* contextUnit = context.GetContextUnit();
                contextUnit->CastSpell(contextUnit, 200175, true); // Felsaber mount
            });
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_learn_felsaber_SpellScript::HandleMountOnHit, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_learn_felsaber_SpellScript();
    }
};

class go_mardum_cage : public GameObjectScript
{
public:
    go_mardum_cage(const char* name, uint32 insideNpc, uint32 killCredit = 0) : GameObjectScript(name), _insideNpc(insideNpc), _killCredit(killCredit)
    {
        if (_killCredit == 0)
            _killCredit = insideNpc;
    }

    bool OnGossipHello(Player* player, GameObject* go) override
    {
        if (Creature* creature = go->FindNearestCreature(_insideNpc, 10.0f))
        {
            // TODO : Remove this line when phasing is done properly
            creature->DestroyForPlayer(player);

            if (TempSummon* personalCreature = player->SummonCreature(_insideNpc, creature->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 4000, 0, true))
            {
                float x, y, z;
                personalCreature->GetClosePoint(x, y, z, personalCreature->GetObjectSize() / 3, 50.0f);
                personalCreature->GetMotionMaster()->MovePoint(0, x, y, z);

                // TODO : personalCreature->Talk(0);
            }

            player->KilledMonsterCredit(_killCredit);
        }

        return false;
    }

    uint32 _insideNpc;
    uint32 _killCredit;
};

class npc_mardum_inquisitor_pernissius : public CreatureScript
{
public:
    npc_mardum_inquisitor_pernissius() : CreatureScript("npc_mardum_inquisitor_pernissius") { }

    struct npc_mardum_inquisitor_pernissiusAI : public ScriptedAI
    {
        npc_mardum_inquisitor_pernissiusAI(Creature* creature) : ScriptedAI(creature) { }

        enum Spells
        {
            SPELL_INCITE_MADNESS    = 194529,
            SPELL_INFERNAL_SMASH    = 192709,

            SPELL_LEARN_EYE_BEAM    = 195447
        };

        enum Creatures
        {
            NPC_COLOSSAL_INFERNAL   = 96159
        };

        ObjectGuid colossalInfernalguid;

        void Reset() override
        {
            if (Creature* infernal = me->SummonCreature(NPC_COLOSSAL_INFERNAL, 523.404, 2428.41, -117.087, 0.108873, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 2000))
                colossalInfernalguid = infernal->GetGUID();
        }

        Creature* GetInfernal() const
        {
            return ObjectAccessor::GetCreature(*me, colossalInfernalguid);
        }

        void EnterCombat(Unit*) override
        {
            me->GetScheduler().Schedule(Seconds(15), [this](TaskContext context)
            {
                if (Unit* target = me->GetVictim())
                    me->CastSpell(target, SPELL_INCITE_MADNESS);

                context.Repeat(Seconds(15));
            });

            me->GetScheduler().Schedule(Seconds(10), [this](TaskContext context)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                    if (Creature* infernal = GetInfernal())
                        infernal->CastSpell(target, SPELL_INFERNAL_SMASH);

                context.Repeat(Seconds(10));
            });
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (Creature* infernal = GetInfernal())
                infernal->KillSelf();

            std::list<Player*> players;
            me->GetPlayerListInGrid(players, 50.0f);

            for (Player* player : players)
            {
                player->KilledMonsterCredit(105946);
                player->KilledMonsterCredit(96159);

                if (!player->HasSpell(SPELL_LEARN_EYE_BEAM))
                    player->CastSpell(player, SPELL_LEARN_EYE_BEAM);
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_mardum_inquisitor_pernissiusAI(creature);
    }
};

// 192709 Infernal Smash
class spell_mardum_infernal_smash : public SpellScriptLoader
{
public:
    spell_mardum_infernal_smash() : SpellScriptLoader("spell_mardum_infernal_smash") { }

    class spell_mardum_infernal_smash_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mardum_infernal_smash_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster() || !GetHitUnit())
                return;

            GetCaster()->CastSpell(GetHitUnit(), GetEffectValue(), true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_mardum_infernal_smash_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_mardum_infernal_smash_SpellScript();
    }
};

class npc_mardum_ashtongue_mystic : public CreatureScript
{
public:
    npc_mardum_ashtongue_mystic() : CreatureScript("npc_mardum_ashtongue_mystic") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 /*action*/) override
    {
        player->KilledMonsterCredit(creature->GetEntry());

        // TODO : Remove this line when phasing is done properly
        creature->DestroyForPlayer(player);

        if (TempSummon* personalCreature = player->SummonCreature(creature->GetEntry(), creature->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 4000, 0, true))
            personalCreature->KillSelf();
        return true;
    }
};

class go_mardum_portal_coilskar : public GameObjectScript
{
public:
    go_mardum_portal_coilskar() : GameObjectScript("go_mardum_portal_coilskar") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/) override
    {
        if (!player->GetQuestObjectiveData(QUEST_COILSKAR_FORCES, 1))
        {
            player->KilledMonsterCredit(94406); // QUEST_COILSKAR_FORCES storageIndex 0 KillCredit
            player->KilledMonsterCredit(97831); // QUEST_COILSKAR_FORCES storageIndex 1 KillCredit
            player->CastSpell(player, SPELL_SCENE_MARDUM_COILSKAR_FORCES, true);
        }

        return false;
    }
};

class go_meeting_with_queen_ritual : public GameObjectScript
{
public:
    go_meeting_with_queen_ritual() : GameObjectScript("go_meeting_with_queen_ritual") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/) override
    {
        if (player->HasQuest(QUEST_MEETING_WITH_QUEEN) &&
            !player->GetQuestObjectiveData(QUEST_MEETING_WITH_QUEEN, 0))
        {
            player->CastSpell(player, SPELL_SCENE_MEETING_WITH_QUEEN, true);
        }

        return false;
    }
};

class scene_mardum_meeting_with_queen : public SceneScript
{
public:
    scene_mardum_meeting_with_queen() : SceneScript("scene_mardum_meeting_with_queen") { }

    void OnSceneComplete(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) override
    {
        player->KilledMonsterCredit(100722);
    }
};

class npc_mardum_doom_commander_beliash : public CreatureScript
{
public:
    npc_mardum_doom_commander_beliash() : CreatureScript("npc_mardum_doom_commander_beliash") { }

    struct npc_mardum_doom_commander_beliashAI : public ScriptedAI
    {
        npc_mardum_doom_commander_beliashAI(Creature* creature) : ScriptedAI(creature){ }

        enum Spells
        {
            SPELL_SHADOW_BOLT_VOLLEY    = 196403,
            SPELL_SHADOW_RETREAT        = 196625,
            SPELL_SHADOW_RETREAT_AT     = 195402,

            SPELL_LEARN_CONSUME_MAGIC   = 195439
        };

        void EnterCombat(Unit*) override
        {
            me->GetScheduler().Schedule(Milliseconds(2500), [this](TaskContext context)
            {
                me->CastSpell(me, SPELL_SHADOW_BOLT_VOLLEY, true);
                context.Repeat(Milliseconds(2500));
            });

            me->GetScheduler().Schedule(Seconds(10), [this](TaskContext context)
            {
                me->CastSpell(me, SPELL_SHADOW_RETREAT);
                context.Repeat(Seconds(15));

                // During retreat commander make blaze appear
                me->GetScheduler().Schedule({ Milliseconds(500), Milliseconds(1000) }, [this](TaskContext /*context*/)
                {
                    me->CastSpell(me, SPELL_SHADOW_RETREAT_AT, true);
                });
            });
        }

        void JustDied(Unit* /*killer*/) override
        {
            std::list<Player*> players;
            me->GetPlayerListInGrid(players, 50.0f);

            for (Player* player : players)
            {
                player->KilledMonsterCredit(106003);

                if (!player->HasSpell(SPELL_LEARN_CONSUME_MAGIC))
                    player->CastSpell(player, SPELL_LEARN_CONSUME_MAGIC);
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_mardum_doom_commander_beliashAI(creature);
    }
};

class go_mardum_portal_shivarra : public GameObjectScript
{
public:
    go_mardum_portal_shivarra() : GameObjectScript("go_mardum_portal_shivarra") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/) override
    {
        if (!player->HasQuest(QUEST_SHIVARRA_FORCES))
        {
            player->CompleteQuest(QUEST_SHIVARRA_FORCES);
            player->CastSpell(player, SPELL_SCENE_MARDUM_SHIVARRA_FORCES, true);
        }

        return false;
    }
};

void AddSC_mardum()
{
    new PlayerScript_mardum_welcome_scene_trigger();
    new scene_mardum_welcome();
    new npc_kayn_sunfury_welcome();
    new go_mardum_legion_banner_1();
    new scene_mardum_change_legion_banner();
    new go_mardum_portal_ashtongue();
    new scene_mardum_welcome_ashtongue();
    new spell_learn_felsaber();
    new go_mardum_cage("go_mardum_cage_belath",     94400);
    new go_mardum_cage("go_mardum_cage_cyana",      94377);
    new go_mardum_cage("go_mardum_cage_izal",       93117);
    new go_mardum_cage("go_mardum_cage_mannethrel", 93230);
    new npc_mardum_inquisitor_pernissius();
    new spell_mardum_infernal_smash();
    new npc_mardum_ashtongue_mystic();
    new go_mardum_portal_coilskar();
    new go_meeting_with_queen_ritual();
    new scene_mardum_meeting_with_queen();
    new npc_mardum_doom_commander_beliash();
    new go_mardum_portal_shivarra();
}
