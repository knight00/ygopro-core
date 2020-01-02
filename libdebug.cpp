/*
 * libdebug.cpp
 *
 *  Created on: 2012-2-8
 *      Author: Argon
 */

#include <string.h>
#include "scriptlib.h"
#include "duel.h"
#include "field.h"
#include "card.h"
#include "effect.h"

#ifdef BUILD_WITH_AI
int32 scriptlib::chat_message(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	lua_getglobal(L, "tostring");
	lua_pushvalue(L, -2);
	lua_pcall(L, 1, 1, 0);
	sprintf(pduel->strbuffer, "%s", lua_tostring(L, -1));
	pduel->handle_message(pduel->handle_message_payload, pduel->strbuffer, OCG_LOG_TYPE_FROM_AI);
	return 0;
}

int get_ai_id(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	//return pduel->game_field->infos.turn_player;
	return (pduel->game_field->player[0].isAI) ? 0 : 1;
}
int get_opponent_id(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	//return 1 - pduel->game_field->infos.turn_player;
	return (pduel->game_field->player[0].isAI) ? 1 : 0;
}

int32 scriptlib::get_opp_extra_deck(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	int playerid = get_opponent_id(L);
	player_info::card_vector extra = pduel->game_field->player[playerid].list_extra;
	card* pcard;

	lua_newtable(L);

	for(size_t i = 0; i < extra.size(); i++) {
		lua_pushnumber(L, i + 1);
		if(extra[i] == NULL) {
			//if(true) {
			lua_pushboolean(L, 0); //set a false
			lua_settable(L, -3);
		} else {
			pcard = extra[i];
			pduel->game_field->set_card_to_lua_without_index(L, pcard, pduel, 0, true);
		}
	}

	return 1;
}
int32 scriptlib::get_ai_extra_deck(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	int playerid = get_ai_id(L);
	player_info::card_vector extra = pduel->game_field->player[playerid].list_extra;
	card* pcard;

	lua_newtable(L);

	for(size_t i = 0; i < extra.size(); i++) {
		lua_pushnumber(L, i + 1);
		if(extra[i] == NULL) {
			//if(true) {
			lua_pushboolean(L, 0); //set a false
			lua_settable(L, -3);
		} else {
			pcard = extra[i];
			pduel->game_field->set_card_to_lua_without_index(L, pcard, pduel, 0, true);
		}
	}

	return 1;
}
int32 scriptlib::get_opp_main_deck(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	int playerid = get_opponent_id(L);
	player_info::card_vector extra = pduel->game_field->player[playerid].list_main;
	card* pcard;

	lua_newtable(L);

	for(size_t i = 0; i < extra.size(); i++) {
		lua_pushnumber(L, i + 1);
		if(extra[i] == NULL) {
			//if(true) {
			lua_pushboolean(L, 0); //set a false
			lua_settable(L, -3);
		} else {
			pcard = extra[i];
			pduel->game_field->set_card_to_lua_without_index(L, pcard, pduel, 0, true);
		}
	}

	return 1;
}
int32 scriptlib::get_ai_main_deck(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	int playerid = get_ai_id(L);
	player_info::card_vector extra = pduel->game_field->player[playerid].list_main;
	card* pcard;

	lua_newtable(L);

	for(size_t i = 0; i < extra.size(); i++) {
		lua_pushnumber(L, i + 1);
		if(extra[i] == NULL) {
			//if(true) {
			lua_pushboolean(L, 0); //set a false
			lua_settable(L, -3);
		} else {
			pcard = extra[i];
			pduel->game_field->set_card_to_lua_without_index(L, pcard, pduel, 0, true);
		}
	}

	return 1;
}
int32 scriptlib::get_opp_monster_zones(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	int playerid = get_opponent_id(L);
	player_info::card_vector mzone = pduel->game_field->player[playerid].list_mzone;
	card* pcard;

	lua_newtable(L);

	for(size_t i = 0; i < mzone.size(); i++) {
		lua_pushnumber(L, i + 1);
		if(mzone[i] == NULL) {
			//if(true) {
			lua_pushboolean(L, 0); //set a false
			lua_settable(L, -3);
		} else {
			pcard = mzone[i];
			pduel->game_field->set_card_to_lua_without_index(L, pcard, pduel, 0, true);
		}
	}

	return 1;
}
int32 scriptlib::get_ai_monster_zones(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	int playerid = get_ai_id(L);
	player_info::card_vector mzone = pduel->game_field->player[playerid].list_mzone;
	card* pcard;

	lua_newtable(L);

	for(size_t i = 0; i < mzone.size(); i++) {
		lua_pushnumber(L, i + 1);
		if(mzone[i] == NULL) {
			//if(true) {
			lua_pushboolean(L, 0); //set a false
			lua_settable(L, -3);
		} else {
			pcard = mzone[i];
			pduel->game_field->set_card_to_lua_without_index(L, pcard, pduel, 0, true);
		}
	}

	return 1;
}
int32 scriptlib::get_opp_st_zones(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	int playerid = get_opponent_id(L);
	player_info::card_vector mzone = pduel->game_field->player[playerid].list_szone;
	card* pcard;

	lua_newtable(L);

	for(size_t i = 0; i < mzone.size(); i++) {
		lua_pushnumber(L, i + 1);
		if(mzone[i] == NULL) {
			//if(true) {
			lua_pushboolean(L, 0); //set a false
			lua_settable(L, -3);
		} else {
			pcard = mzone[i];
			pduel->game_field->set_card_to_lua_without_index(L, pcard, pduel, 0, true);
		}
	}

	return 1;
}
int32 scriptlib::get_ai_st_zones(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	int playerid = get_ai_id(L);
	player_info::card_vector mzone = pduel->game_field->player[playerid].list_szone;
	card* pcard;

	lua_newtable(L);

	for(size_t i = 0; i < mzone.size(); i++) {
		lua_pushnumber(L, i + 1);
		if(mzone[i] == NULL) {
			//if(true) {
			lua_pushboolean(L, 0); //set a false
			lua_settable(L, -3);
		} else {
			pcard = mzone[i];
			pduel->game_field->set_card_to_lua_without_index(L, pcard, pduel, 0, true);
		}
	}

	return 1;
}
int32 scriptlib::get_opp_graveyard(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	int playerid = get_opponent_id(L);
	player_info::card_vector mzone = pduel->game_field->player[playerid].list_grave;
	card* pcard;

	lua_newtable(L);

	for(size_t i = 0; i < mzone.size(); i++) {
		lua_pushnumber(L, i + 1);
		if(mzone[i] == NULL) {
			//if(true) {
			lua_pushboolean(L, 0); //set a false
			lua_settable(L, -3);
		} else {
			pcard = mzone[i];
			pduel->game_field->set_card_to_lua_without_index(L, pcard, pduel, 0, true);
		}
	}

	return 1;
}
int32 scriptlib::get_ai_graveyard(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	int playerid = get_ai_id(L);
	player_info::card_vector mzone = pduel->game_field->player[playerid].list_grave;
	card* pcard;

	lua_newtable(L);

	for(size_t i = 0; i < mzone.size(); i++) {
		lua_pushnumber(L, i + 1);
		if(mzone[i] == NULL) {
			//if(true) {
			lua_pushboolean(L, 0); //set a false
			lua_settable(L, -3);
		} else {
			pcard = mzone[i];
			pduel->game_field->set_card_to_lua_without_index(L, pcard, pduel, 0, true);
		}
	}

	return 1;
}
int32 scriptlib::get_opp_banished(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	int playerid = get_opponent_id(L);
	player_info::card_vector mzone = pduel->game_field->player[playerid].list_remove;
	card* pcard;

	lua_newtable(L);

	for(size_t i = 0; i < mzone.size(); i++) {
		lua_pushnumber(L, i + 1);
		if(mzone[i] == NULL) {
			//if(true) {
			lua_pushboolean(L, 0); //set a false
			lua_settable(L, -3);
		} else {
			pcard = mzone[i];
			pduel->game_field->set_card_to_lua_without_index(L, pcard, pduel, 0, true);
		}
	}

	return 1;
}
int32 scriptlib::get_ai_hand(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	int playerid = get_ai_id(L);
	player_info::card_vector mzone = pduel->game_field->player[playerid].list_hand;
	card* pcard;

	lua_newtable(L);

	for(size_t i = 0; i < mzone.size(); i++) {
		lua_pushnumber(L, i + 1);
		if(mzone[i] == NULL) {
			//if(true) {
			lua_pushboolean(L, 0); //set a false
			lua_settable(L, -3);
		} else {
			pcard = mzone[i];
			pduel->game_field->set_card_to_lua_without_index(L, pcard, pduel, 0, true);
		}
	}

	return 1;
}
int32 scriptlib::get_opp_hand(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	int playerid = get_opponent_id(L);
	player_info::card_vector mzone = pduel->game_field->player[playerid].list_hand;
	card* pcard;

	lua_newtable(L);

	for(size_t i = 0; i < mzone.size(); i++) {
		lua_pushnumber(L, i + 1);
		if(mzone[i] == NULL) {
			//if(true) {
			lua_pushboolean(L, 0); //set a false
			lua_settable(L, -3);
		} else {
			pcard = mzone[i];
			pduel->game_field->set_card_to_lua_without_index(L, pcard, pduel, 0, true);
		}
	}

	return 1;
}
int32 scriptlib::get_ai_banished(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	int playerid = get_ai_id(L);
	player_info::card_vector mzone = pduel->game_field->player[playerid].list_remove;
	card* pcard;

	lua_newtable(L);

	for(size_t i = 0; i < mzone.size(); i++) {
		lua_pushnumber(L, i + 1);
		if(mzone[i] == NULL) {
			//if(true) {
			lua_pushboolean(L, 0); //set a false
			lua_settable(L, -3);
		} else {
			pcard = mzone[i];
			pduel->game_field->set_card_to_lua_without_index(L, pcard, pduel, 0, true);
		}
	}

	return 1;
}
int32 scriptlib::get_player_lp(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);

	/* get number of arguments */
	int n = lua_gettop(L);
	int player = lua_tonumber(L, 1);

	int32 lp = 0;
	if(player == 1) { //AI lp
		if(pduel->game_field->player[0].isAI) {
			lp = pduel->game_field->player[0].lp;
		} else {
			lp = pduel->game_field->player[1].lp;
		}
	} else if(player == 2) { //player lp
		if(pduel->game_field->player[0].isAI) {
			lp = pduel->game_field->player[1].lp;
		} else {
			lp = pduel->game_field->player[0].lp;
		}
	} else {
		//error
		sprintf(pduel->strbuffer, "%s", "GetPlayerLP invalid player");
		pduel->handle_message(pduel->handle_message_payload, pduel->strbuffer, OCG_LOG_TYPE_FROM_AI_ERROR);
	}
	/*
	if(pduel->game_field->player[player].isAI) {
	lp = pduel->game_field->player[player].lp; //AI lp
	} else if(player == 2) {
	lp = pduel->game_field->player[0].lp; //player lp
	} else {
	//error
	sprintf(pduel->strbuffer, "%s", "GetPlayerLP invalid player");
	pduel->handle_message(pduel->handle_message_payload, pduel->strbuffer, OCG_LOG_TYPE_FROM_AI_ERROR);
	}
	*/
	lua_pushnumber(L, lp);
	return 1;
}

int32 scriptlib::get_phase(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);

	lua_pushnumber(L, pduel->game_field->infos.phase);
	return 1;
}
int32 scriptlib::get_last_summoned_cards(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	std::list<tevent> private_event = pduel->game_field->core.private_event;

	lua_newtable(L);

	if(private_event.size() > 0) {
		int i = 0;
		for(auto elit = private_event.begin(); elit != private_event.end(); ++elit) {
			lua_pushnumber(L, i + 1);
			i++;
			card* pcard = elit->trigger_card;
			pduel->game_field->set_card_to_lua_without_index(L, pcard, pduel, 0, true);
		}
	}
	return 1;
}
int32 scriptlib::get_script_from_card_object(lua_State *L) {
	lua_pushstring(L, "cardid");
	lua_gettable(L, -2);
	int cardid = lua_tonumber(L, -1);
	lua_pop(L, 1);

	duel* pduel = interpreter::get_duel_info(L);
	card* c;
	int32 returnval = 0;
	bool foundTheCard = false;
	for(auto cit = pduel->cards.begin(); cit != pduel->cards.end(); ++cit) {
		c = *cit;
		if(c->cardid == cardid) {
			foundTheCard = true;
			interpreter::card2value(L, c);
			break;
		}
	}
	if(!foundTheCard) {
		interpreter::card2value(L, c);
	}

	return 1;
}
int32 scriptlib::get_card_object_from_script(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**)lua_touserdata(L, 1);
	duel* pduel = interpreter::get_duel_info(L);
	pduel->game_field->set_card_to_lua_without_index(L, pcard, pduel, 0, false);
	return 1;
}
int32 scriptlib::get_card_name(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_INT, 1);
	int id = lua_tointeger(L, 1);
	const char* result = ""; //TODO
	lua_pushstring(L, result);
	return 1;
}
#endif //BUILD_WITH_AI
int32 scriptlib::debug_message(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	lua_getglobal(L, "tostring");
	lua_pushvalue(L, -2);
	lua_pcall(L, 1, 1, 0);
	interpreter::sprintf(pduel->strbuffer, "%s", lua_tostring(L, -1));
	pduel->handle_message(pduel->handle_message_payload, pduel->strbuffer, OCG_LOG_TYPE_FROM_SCRIPT);
	return 0;
}
int32 scriptlib::debug_add_card(lua_State *L) {
	check_param_count(L, 6);
	duel* pduel = interpreter::get_duel_info(L);
	int32 code = lua_tointeger(L, 1);
	int32 owner = lua_tointeger(L, 2);
	int32 playerid = lua_tointeger(L, 3);
	int32 location = lua_tointeger(L, 4);
	int32 sequence = lua_tointeger(L, 5);
	int32 position = lua_tointeger(L, 6);
	int32 proc = lua_toboolean(L, 7);
	if(owner != 0 && owner != 1)
		return 0;
	if(playerid != 0 && playerid != 1)
		return 0;
	if(pduel->game_field->is_location_useable(playerid, location, sequence)) {
		card* pcard = pduel->new_card(code);
		pcard->owner = owner;
		pcard->sendto_param.position = position;
		if(location == LOCATION_PZONE) {
			int32 seq = pduel->game_field->get_pzone_index(sequence);
			pduel->game_field->add_card(playerid, pcard, LOCATION_SZONE, seq, TRUE);
		} else if(location == LOCATION_FZONE) {
			int32 loc = LOCATION_SZONE;
			pduel->game_field->add_card(playerid, pcard, loc, 5);
		} else
			pduel->game_field->add_card(playerid, pcard, location, sequence);
		pcard->current.position = position;
		if(!(location & (LOCATION_ONFIELD | LOCATION_PZONE)) || (position & POS_FACEUP)) {
			pcard->enable_field_effect(true);
			pduel->game_field->adjust_instant();
		}
		if(proc)
			pcard->set_status(STATUS_PROC_COMPLETE, TRUE);
		interpreter::card2value(L, pcard);
		return 1;
	} else if(location == LOCATION_MZONE) {
		card* pcard = pduel->new_card(code);
		pcard->owner = owner;
		card* fcard = pduel->game_field->get_field_card(playerid, location, sequence);
		fcard->xyz_materials.push_back(pcard);
		pcard->overlay_target = fcard;
		pcard->current.controler = playerid;
		pcard->current.location = LOCATION_OVERLAY;
		pcard->current.sequence = fcard->xyz_materials.size() - 1;
		for(auto& eit : pcard->xmaterial_effect) {
			effect* peffect = eit.second;
			if(peffect->type & EFFECT_TYPE_FIELD)
				pduel->game_field->add_effect(peffect);
		}
		interpreter::card2value(L, pcard);
		return 1;
	}
	return 0;
}
int32 scriptlib::debug_set_player_info(lua_State *L) {
	check_param_count(L, 4);
	duel* pduel = interpreter::get_duel_info(L);
	uint32 playerid = lua_tointeger(L, 1);
	uint32 lp = lua_tointeger(L, 2);
	uint32 startcount = lua_tointeger(L, 3);
	uint32 drawcount = lua_tointeger(L, 4);
	if(playerid != 0 && playerid != 1)
		return 0;
	pduel->game_field->player[playerid].lp = lp;
	pduel->game_field->player[playerid].start_lp = lp;
	pduel->game_field->player[playerid].start_count = startcount;
	pduel->game_field->player[playerid].draw_count = drawcount;
	return 0;
}
int32 scriptlib::debug_pre_summon(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 summon_type = lua_tointeger(L, 2);
	uint8 summon_location = 0;
	if(lua_gettop(L) > 2)
		summon_location = lua_tointeger(L, 3);
	pcard->summon_info = summon_type | (summon_location << 16);
	return 0;
}
int32 scriptlib::debug_pre_equip(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	check_param(L, PARAM_TYPE_CARD, 2);
	card* equip_card = *(card**) lua_touserdata(L, 1);
	card* target = *(card**) lua_touserdata(L, 2);
	if((equip_card->current.location != LOCATION_SZONE)
	        || (target->current.location != LOCATION_MZONE)
	        || (target->current.position & POS_FACEDOWN))
		lua_pushboolean(L, 0);
	else {
		equip_card->equip(target, FALSE);
		equip_card->effect_target_cards.insert(target);
		target->effect_target_owner.insert(equip_card);
		lua_pushboolean(L, 1);
	}
	return 1;
}
int32 scriptlib::debug_pre_set_target(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	check_param(L, PARAM_TYPE_CARD, 2);
	card* t_card = *(card**) lua_touserdata(L, 1);
	card* target = *(card**) lua_touserdata(L, 2);
	t_card->add_card_target(target);
	return 0;
}
int32 scriptlib::debug_pre_add_counter(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 countertype = lua_tointeger(L, 2);
	uint32 count = lua_tointeger(L, 3);
	uint16 cttype = countertype & ~COUNTER_NEED_ENABLE;
	auto pr = pcard->counters.emplace(cttype, card::counter_map::mapped_type());
	auto cmit = pr.first;
	if(pr.second) {
		cmit->second[0] = 0;
		cmit->second[1] = 0;
	}
	if((countertype & COUNTER_WITHOUT_PERMIT) && !(countertype & COUNTER_NEED_ENABLE))
		cmit->second[0] += count;
	else
		cmit->second[1] += count;
	return 0;
}
int32 scriptlib::debug_reload_field_begin(lua_State *L) {
	check_param_count(L, 1);
	duel* pduel = interpreter::get_duel_info(L);
	uint32 flag = lua_tointeger(L, 1);
	int32 rule = lua_tointeger(L, 2);
	bool build = lua_toboolean(L, 3);
	if (!rule)
		rule = 3;
	pduel->clear();
	if(rule && !build) {
		switch (rule) {
		case 1: {
			flag |= DUEL_MODE_MR1;
			break;
		}
		case 2: {
			flag |= DUEL_MODE_MR2;
			break;
		}
		case 3: {
			flag |= DUEL_MODE_MR3;
			break;
		}
		case 4: {
			flag |= DUEL_MODE_MR4;
			break;
		}
		}
	} else if (flag & DUEL_OBSOLETE_RULING) {
		flag |= DUEL_MODE_MR1;
		pduel->game_field->core.duel_options = flag;
		return 0;
	}
	pduel->game_field->core.duel_options = flag;
	return 0;
}
int32 scriptlib::debug_reload_field_end(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	pduel->game_field->core.shuffle_hand_check[0] = FALSE;
	pduel->game_field->core.shuffle_hand_check[1] = FALSE;
	pduel->game_field->core.shuffle_deck_check[0] = FALSE;
	pduel->game_field->core.shuffle_deck_check[1] = FALSE;
	pduel->game_field->reload_field_info();
	return 0;
}
int32 scriptlib::debug_set_ai_name(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_STRING, 1);
	duel* pduel = interpreter::get_duel_info(L);
	auto message = pduel->new_message(MSG_AI_NAME);
	const char* pstr = lua_tostring(L, 1);
	int len = strlen(pstr);
	if(len > 100)
		len = 100;
	message->write<uint16>(len);
	message->write((void*)pstr, len);
	message->write<uint8>(0);
	return 0;
}
int32 scriptlib::debug_show_hint(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_STRING, 1);
	duel* pduel = interpreter::get_duel_info(L);
	auto message = pduel->new_message(MSG_SHOW_HINT);
	const char* pstr = lua_tostring(L, 1);
	int len = strlen(pstr);
	if(len > 1024)
		len = 1024;
	message->write<uint16>(len);
	message->write((void*)pstr, len);
	message->write<uint8>(0);
	return 0;
}
