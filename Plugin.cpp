// This file added in headers queue
// File: "Sources.h"
#include "resource.h"

namespace GOTHIC_ENGINE {
  
  // Not collide with a oCTriggerScript
  HOOK Hook_oCAIArrow_CanThisCollideWith PATCH_IF(&oCAIArrow::CanThisCollideWith, &oCAIArrow::CanThisCollideWith_Union, false);
  
  int oCAIArrow::CanThisCollideWith_Union(zCVob* vob) {
 
      // Not collide with a oCTriggerScript with a one exception defined below
      zCClassDef* classDef = vob->_GetClassDef();
      if (classDef->className == "oCTriggerScript") {

          // Check whether this trigger should react on shooting
          zCTrigger* trigger = static_cast<zCTrigger*>(vob);

          if (trigger->respondToVobName != "SHOTTOTRIGGER")
          {
              return 0;
          };
      }

      int originalValue = THISCALL(Hook_oCAIArrow_CanThisCollideWith)(vob);

      return originalValue;
  }

  // Add additional damage to the hero spell
  HOOK Hook_oCMag_Book_Spell_Cast PATCH_IF(&oCMag_Book::Spell_Cast, &oCMag_Book::Spell_Cast_Union, false);

  void oCMag_Book::Spell_Cast_Union() {

      // Get spell
      oCSpell* spell = GetSelectedSpell();

      // Get caster
      oCNpc* npc = spell->spellCasterNpc;

      // If is player
      if (npc->IsAPlayer()) {
          int ManaSpellForce = 0;
          zCPar_Symbol* sym = parser->GetSymbol("ManaSpellForce");
          if (sym) sym->GetValue(ManaSpellForce, 0);

          if (0 != ManaSpellForce)
          {
              oCVisualFX* effectCurrent = spell->effect;
              effectCurrent->damage = effectCurrent->damage + float(ManaSpellForce);
          }
      }

      THISCALL(Hook_oCMag_Book_Spell_Cast)();
  }

  // Disable saving
  HOOK Hook_CGameManager_MenuEnabled PATCH_IF(&CGameManager::MenuEnabled, &CGameManager::MenuEnabled_Union, false);

  int CGameManager::MenuEnabled_Union(zBOOL& enableSave) {
      int SAVINGDISABLED = 0;
      zCPar_Symbol* sym = parser->GetSymbol("SAVINGDISABLED");
      if (sym) sym->GetValue(SAVINGDISABLED, 0);

      int enabled = THISCALL(Hook_CGameManager_MenuEnabled)(enableSave);
      if (1 == SAVINGDISABLED)
          enableSave = false;

      return enabled;
  }

  void EnableHook() {
      Hook_oCAIArrow_CanThisCollideWith.Commit();
      Hook_oCMag_Book_Spell_Cast.Commit();
      Hook_CGameManager_MenuEnabled.Commit();
  }

  void Game_Entry() {
  }
  
  void Game_Init() {
      EnableHook();
  }

  void Game_Exit() {
  }

  void Game_PreLoop() {
  }

  void Game_Loop() {
  }

  void Game_PostLoop() {
  }

  void Game_MenuLoop() {
  }

  // Information about current saving or loading world
  TSaveLoadGameInfo& SaveLoadGameInfo = UnionCore::SaveLoadGameInfo;

  void Game_SaveBegin() {
  }

  void Game_SaveEnd() {
  }

  void LoadBegin() {
  }

  void LoadEnd() {
  }

  void Game_LoadBegin_NewGame() {
    LoadBegin();
  }

  void Game_LoadEnd_NewGame() {
    LoadEnd();
  }

  void Game_LoadBegin_SaveGame() {
    LoadBegin();
  }

  void Game_LoadEnd_SaveGame() {
    LoadEnd();
  }

  void Game_LoadBegin_ChangeLevel() {
    LoadBegin();
  }

  void Game_LoadEnd_ChangeLevel() {
    LoadEnd();
  }

  void Game_LoadBegin_Trigger() {
  }
  
  void Game_LoadEnd_Trigger() {
  }
  
  void Game_Pause() {
  }
  
  void Game_Unpause() {
  }

  // Get an item which C_NPC uses to cast a spell
  int Npc_GetActiveSpellSourceItem() {
      oCNpc* instance = (oCNpc*)parser->GetInstance();
      if (instance) {
          if (instance->GetWeaponMode() == NPC_WEAPON_MAG) {
              oCMag_Book* book = instance->GetSpellBook();
              if (book) {
                  oCItem* item = book->GetSpellItem(book->GetSelectedSpellNr());
                  parser->SetInstance("ITEM", item);
              }
          }
      }

      return true;
  }
  
  void Game_DefineExternals() {
      parser->DefineExternal("Npc_GetActiveSpellSourceItem", Npc_GetActiveSpellSourceItem, zPAR_TYPE_VOID, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
  }

  void Game_ApplyOptions() {
  }

  /*
  Functions call order on Game initialization:
    - Game_Entry           * Gothic entry point
    - Game_DefineExternals * Define external script functions
    - Game_Init            * After DAT files init
  
  Functions call order on Change level:
    - Game_LoadBegin_Trigger     * Entry in trigger
    - Game_LoadEnd_Trigger       *
    - Game_Loop                  * Frame call window
    - Game_LoadBegin_ChangeLevel * Load begin
    - Game_SaveBegin             * Save previous level information
    - Game_SaveEnd               *
    - Game_LoadEnd_ChangeLevel   *
  
  Functions call order on Save game:
    - Game_Pause     * Open menu
    - Game_Unpause   * Click on save
    - Game_Loop      * Frame call window
    - Game_SaveBegin * Save begin
    - Game_SaveEnd   *
  
  Functions call order on Load game:
    - Game_Pause              * Open menu
    - Game_Unpause            * Click on load
    - Game_LoadBegin_SaveGame * Load begin
    - Game_LoadEnd_SaveGame   *
  */

#define AppDefault True
  CApplication* lpApplication = !CHECK_THIS_ENGINE ? Null : CApplication::CreateRefApplication(
    Enabled( AppDefault ) Game_Entry,
    Enabled( AppDefault ) Game_Init,
    Enabled( AppDefault ) Game_Exit,
    Enabled( AppDefault ) Game_PreLoop,
    Enabled( AppDefault ) Game_Loop,
    Enabled( AppDefault ) Game_PostLoop,
    Enabled( AppDefault ) Game_MenuLoop,
    Enabled( AppDefault ) Game_SaveBegin,
    Enabled( AppDefault ) Game_SaveEnd,
    Enabled( AppDefault ) Game_LoadBegin_NewGame,
    Enabled( AppDefault ) Game_LoadEnd_NewGame,
    Enabled( AppDefault ) Game_LoadBegin_SaveGame,
    Enabled( AppDefault ) Game_LoadEnd_SaveGame,
    Enabled( AppDefault ) Game_LoadBegin_ChangeLevel,
    Enabled( AppDefault ) Game_LoadEnd_ChangeLevel,
    Enabled( AppDefault ) Game_LoadBegin_Trigger,
    Enabled( AppDefault ) Game_LoadEnd_Trigger,
    Enabled( AppDefault ) Game_Pause,
    Enabled( AppDefault ) Game_Unpause,
    Enabled( AppDefault ) Game_DefineExternals,
    Enabled( AppDefault ) Game_ApplyOptions
  );
}