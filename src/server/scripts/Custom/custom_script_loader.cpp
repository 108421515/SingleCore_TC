/*
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
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

// This is where scripts' loading functions should be declared:
void AddSC_solocraft();
void AddSC_VAS_AutoBalance();
void AddSC_FireWorksOnLevelUp();
void AddSC_Player_Boa();
void AddSC_gon_playerscripts();
void AddSC_login_script();
void AddSC_announce_login();
void AddSC_Boss_Announcer();
void AddSC_XpWeekend();
void AddSC_Npc_Beastmaster();
void AddSC_NPC_Teleport();

// The name of this function should match:
// void Add${NameOfDirectory}Scripts()
void AddCustomScripts()
{
	AddSC_solocraft();
	AddSC_VAS_AutoBalance();
	AddSC_FireWorksOnLevelUp();
	AddSC_Player_Boa();
	AddSC_gon_playerscripts();
	AddSC_login_script();
	AddSC_announce_login();
	AddSC_Boss_Announcer();
	AddSC_XpWeekend();
	AddSC_Npc_Beastmaster();
	AddSC_NPC_Teleport();
}
