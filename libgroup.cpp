/*
 * libgroup.cpp
 *
 *  Created on: 2010-5-6
 *      Author: Argon
 */

#include "scriptlib.h"
#include "group.h"
#include "card.h"
#include "effect.h"
#include "duel.h"

int32 scriptlib::group_new(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	group* pgroup = pduel->new_group();
	interpreter::group2value(L, pgroup);
	return 1;
}
int32 scriptlib::group_clone(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_GROUP, 1);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	duel* pduel = pgroup->pduel;
	group* newgroup = pduel->new_group(pgroup->container);
	interpreter::group2value(L, newgroup);
	return 1;
}
int32 scriptlib::group_from_cards(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	group* pgroup = pduel->new_group();
	for(int32 i = 0; i < lua_gettop(L); ++i) {
		if(!lua_isnil(L, i + 1)) {
			check_param(L, PARAM_TYPE_CARD, i + 1);
			card* pcard = *(card**)lua_touserdata(L, i + 1);
			pgroup->container.insert(pcard);
		}
	}
	interpreter::group2value(L, pgroup);
	return 1;
}
int32 scriptlib::group_delete(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_GROUP, 1);
	duel* pduel = interpreter::get_duel_info(L);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	if(pgroup->is_readonly != 2)
		return 0;
	pgroup->is_readonly = 0;
	pduel->sgroups.insert(pgroup);
	return 0;
}
int32 scriptlib::group_keep_alive(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_GROUP, 1);
	duel* pduel = interpreter::get_duel_info(L);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	if(pgroup->is_readonly == 1)
		return 0;
	pgroup->is_readonly = 2;
	pduel->sgroups.erase(pgroup);
	return 0;
}
int32 scriptlib::group_clear(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_GROUP, 1);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	if (pgroup->is_readonly != 1) {
		pgroup->container.clear();
	}
	return 0;
}
int32 scriptlib::group_add_card(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_CARD, 2);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	card* pcard = *(card**) lua_touserdata(L, 2);
	if (pgroup->is_readonly != 1) {
		pgroup->container.insert(pcard);
	}
	return 0;
}
int32 scriptlib::group_remove_card(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_CARD, 2);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	card* pcard = *(card**) lua_touserdata(L, 2);
	if (pgroup->is_readonly != 1) {
		pgroup->container.erase(pcard);
	}
	return 0;
}
int32 scriptlib::group_get_next(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_GROUP, 1);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	if(pgroup->it == pgroup->container.end())
		lua_pushnil(L);
	else {
		++pgroup->it;
		if (pgroup->it == pgroup->container.end())
			lua_pushnil(L);
		else
			interpreter::card2value(L, (*(pgroup->it)));
	}
	return 1;
}
int32 scriptlib::group_get_first(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_GROUP, 1);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	if (pgroup->container.size()) {
		pgroup->it = pgroup->container.begin();
		interpreter::card2value(L, (*(pgroup->it)));
	} else
		lua_pushnil(L);
	return 1;
}
int32 scriptlib::group_take_at_pos(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_GROUP, 1);
	group* pgroup = *(group**)lua_touserdata(L, 1);
	int32 pos = lua_tointeger(L, 2);
	if(pos > (int32)pgroup->container.size())
		lua_pushnil(L);
	else {
		auto cit = pgroup->container.begin();
		std::advance(cit, pos);
		interpreter::card2value(L, (*cit));
	}
	return 1;
}
int32 scriptlib::group_get_count(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_GROUP, 1);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	lua_pushinteger(L, pgroup->container.size());
	return 1;
}
int32 scriptlib::group_for_each(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_FUNCTION, 2);
	duel* pduel = interpreter::get_duel_info(L);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	int32 f = interpreter::get_function_handle(L, 2);
	int32 extraargs = lua_gettop(L) - 2;
	for (auto& pcard : pgroup->container) {
		pduel->lua->add_param(pcard, PARAM_TYPE_CARD);
		for(int32 i = 0; i < extraargs; ++i)
			pduel->lua->add_param(-extraargs + i, PARAM_TYPE_INDEX);
		pduel->lua->call_function(f, 1 + extraargs, 0);
	}
	return 0;
}
int32 scriptlib::group_filter(lua_State *L) {
	check_param_count(L, 3);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_FUNCTION, 2);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	field::card_set cset(pgroup->container);
	if(check_param(L, PARAM_TYPE_CARD, 3, TRUE)) {
		card* pexception = *(card**) lua_touserdata(L, 3);
		cset.erase(pexception);
	} else if(check_param(L, PARAM_TYPE_GROUP, 3, TRUE)) {
		group* pexgroup = *(group**) lua_touserdata(L, 3);
		for(auto& pcard : pexgroup->container)
			cset.erase(pcard);
	}
	duel* pduel = pgroup->pduel;
	group* new_group = pduel->new_group();
	uint32 extraargs = lua_gettop(L) - 3;
	for(auto& pcard : cset) {
		if(pduel->lua->check_matching(pcard, 2, extraargs)) {
			new_group->container.insert(pcard);
		}
	}
	interpreter::group2value(L, new_group);
	return 1;
}
int32 scriptlib::group_filter_count(lua_State *L) {
	check_param_count(L, 3);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_FUNCTION, 2);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	field::card_set cset(pgroup->container);
	if(check_param(L, PARAM_TYPE_CARD, 3, TRUE)) {
		card* pexception = *(card**) lua_touserdata(L, 3);
		cset.erase(pexception);
	} else if(check_param(L, PARAM_TYPE_GROUP, 3, TRUE)) {
		group* pexgroup = *(group**) lua_touserdata(L, 3);
		for(auto& pcard : pexgroup->container)
			cset.erase(pcard);
	}
	duel* pduel = pgroup->pduel;
	uint32 extraargs = lua_gettop(L) - 3;
	uint32 count = 0;
	for (auto& pcard : cset) {
		if(pduel->lua->check_matching(pcard, 2, extraargs))
			count++;
	}
	lua_pushinteger(L, count);
	return 1;
}
int32 scriptlib::group_filter_select(lua_State *L) {
	check_action_permission(L);
	check_param_count(L, 6);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_FUNCTION, 3);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	field::card_set cset(pgroup->container);
	if(check_param(L, PARAM_TYPE_CARD, 6, TRUE)) {
		card* pexception = *(card**) lua_touserdata(L, 6);
		cset.erase(pexception);
	} else if(check_param(L, PARAM_TYPE_GROUP, 6, TRUE)) {
		group* pexgroup = *(group**) lua_touserdata(L, 6);
		for(auto& pcard : pexgroup->container)
			cset.erase(pcard);
	}
	duel* pduel = pgroup->pduel;
	uint32 playerid = lua_tointeger(L, 2);
	if(playerid != 0 && playerid != 1)
		return 0;
	uint32 min = lua_tointeger(L, 4);
	uint32 max = lua_tointeger(L, 5);
	uint32 extraargs = lua_gettop(L) - 6;
	pduel->game_field->core.select_cards.clear();
	for (auto& pcard : cset) {
		if(pduel->lua->check_matching(pcard, 3, extraargs))
			pduel->game_field->core.select_cards.push_back(pcard);
	}
	pduel->game_field->add_process(PROCESSOR_SELECT_CARD, 0, 0, 0, playerid, min + (max << 16));
	return lua_yieldk(L, 0, (lua_KContext)pduel, [](lua_State *L, int32 status, lua_KContext ctx) {
		duel* pduel = (duel*)ctx;
		if(pduel->game_field->return_cards.canceled)
			lua_pushnil(L);
		else {
			group* pgroup = pduel->new_group(field::card_set(pduel->game_field->return_cards.list.begin(), pduel->game_field->return_cards.list.end()));
			interpreter::group2value(L, pgroup);
		}
		return 1;
	});
}
int32 scriptlib::group_select(lua_State *L) {
	check_action_permission(L);
	check_param_count(L, 5);
	check_param(L, PARAM_TYPE_GROUP, 1);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	field::card_set cset(pgroup->container);
	if(check_param(L, PARAM_TYPE_CARD, 5, TRUE)) {
		card* pexception = *(card**) lua_touserdata(L, 5);
		cset.erase(pexception);
	} else if(check_param(L, PARAM_TYPE_GROUP, 5, TRUE)) {
		group* pexgroup = *(group**) lua_touserdata(L, 5);
		for(auto& pcard : pexgroup->container)
			cset.erase(pcard);
	}
	duel* pduel = pgroup->pduel;
	uint32 playerid = lua_tointeger(L, 2);
	if(playerid != 0 && playerid != 1)
		return 0;
	uint32 min = lua_tointeger(L, 3);
	uint32 max = lua_tointeger(L, 4);
	pduel->game_field->core.select_cards.clear();
	for (auto& pcard : cset) {
		pduel->game_field->core.select_cards.push_back(pcard);
	}
	pduel->game_field->add_process(PROCESSOR_SELECT_CARD, 0, 0, 0, playerid, min + (max << 16));
	return lua_yieldk(L, 0, (lua_KContext)pduel, [](lua_State *L, int32 status, lua_KContext ctx) {
		duel* pduel = (duel*)ctx;
		if(pduel->game_field->return_cards.canceled)
			lua_pushnil(L);
		else {
			group* pgroup = pduel->new_group(field::card_set(pduel->game_field->return_cards.list.begin(), pduel->game_field->return_cards.list.end()));
			interpreter::group2value(L, pgroup);
		}
		return 1;
	});
}
int32 scriptlib::group_select_unselect(lua_State *L) {
	check_action_permission(L);
	check_param_count(L, 3);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_GROUP, 2);
	group* pgroup1 = *(group**)lua_touserdata(L, 1);
	group* pgroup2 = *(group**)lua_touserdata(L, 2);
	duel* pduel = pgroup1->pduel;
	uint32 playerid = lua_tointeger(L, 3);
	if(playerid != 0 && playerid != 1)
		return 0;
	if(pgroup1->container == pgroup2->container)
		return 0;
	for(auto& pcard : pgroup2->container) {
		for(auto& pcard2 : pgroup1->container) {
			if(pcard2->cardid > pcard->cardid)
				break;
			if(pcard2->cardid == pcard->cardid)
				return 0;
		}
	}
	uint32 finishable = FALSE;
	if(lua_gettop(L) > 3) {
		finishable = lua_toboolean(L, 4);
	}
	uint32 cancelable = FALSE;
	if(lua_gettop(L) > 4) {
		cancelable = lua_toboolean(L, 5);
	}
	uint32 min = 1;
	if(lua_gettop(L) > 5) {
		min = lua_tointeger(L, 6);
	}
	uint32 max = 1;
	if(lua_gettop(L) > 6) {
		max = lua_tointeger(L, 7);
	}
	if(min > max)
		min = max;
	pduel->game_field->core.select_cards.clear();
	pduel->game_field->core.unselect_cards.clear();
	for(auto it = pgroup1->container.begin(); it != pgroup1->container.end(); ++it) {
		pduel->game_field->core.select_cards.push_back(*it);
	}
	for(auto it = pgroup2->container.begin(); it != pgroup2->container.end(); ++it) {
		pduel->game_field->core.unselect_cards.push_back(*it);
	}
	pduel->game_field->add_process(PROCESSOR_SELECT_UNSELECT_CARD, 0, 0, 0, playerid + (cancelable << 16), min + (max << 16), finishable);
	return lua_yieldk(L, 0, (lua_KContext)pduel, [](lua_State *L, int32 status, lua_KContext ctx) {
		duel* pduel = (duel*)ctx;
		if(pduel->game_field->return_cards.canceled)
			lua_pushnil(L);
		else {
			interpreter::card2value(L, pduel->game_field->return_cards.list[0]);
		}
		return 1;
	});
}
int32 scriptlib::group_random_select(lua_State *L) {
	check_param_count(L, 3);
	check_param(L, PARAM_TYPE_GROUP, 1);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	int32 playerid = lua_tointeger(L, 2);
	uint32 count = lua_tointeger(L, 3);
	duel* pduel = pgroup->pduel;
	group* newgroup = pduel->new_group();
	if(count > pgroup->container.size())
		count = pgroup->container.size();
	if(count == 0) {
		interpreter::group2value(L, newgroup);
		return 1;
	}
	if(count == pgroup->container.size())
		newgroup->container = pgroup->container;
	else {
		while(newgroup->container.size() < count) {
			int32 i = pduel->get_next_integer(0, pgroup->container.size() - 1);
			auto cit = pgroup->container.begin();
			std::advance(cit, i);
			newgroup->container.insert(*cit);
		}
	}
	auto message = pduel->new_message(MSG_RANDOM_SELECTED);
	message->write<uint8>(playerid);
	message->write<uint32>(count);
	for(auto& pcard : newgroup->container) {
		message->write(pcard->get_info_location());
	}
	interpreter::group2value(L, newgroup);
	return 1;
}
int32 scriptlib::group_is_exists(lua_State *L) {
	check_param_count(L, 4);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_FUNCTION, 2);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	field::card_set cset(pgroup->container);
	if(check_param(L, PARAM_TYPE_CARD, 4, TRUE)) {
		card* pexception = *(card**) lua_touserdata(L, 4);
		cset.erase(pexception);
	} else if(check_param(L, PARAM_TYPE_GROUP, 4, TRUE)) {
		group* pexgroup = *(group**) lua_touserdata(L, 4);
		for(auto& pcard : pexgroup->container)
			cset.erase(pcard);
	}
	duel* pduel = pgroup->pduel;
	uint32 count = lua_tointeger(L, 3);
	uint32 extraargs = lua_gettop(L) - 4;
	uint32 fcount = 0;
	uint32 result = FALSE;
	for (auto& pcard : cset) {
		if(pduel->lua->check_matching(pcard, 2, extraargs)) {
			fcount++;
			if(fcount >= count) {
				result = TRUE;
				break;
			}
		}
	}
	lua_pushboolean(L, result);
	return 1;
}
int32 scriptlib::group_check_with_sum_equal(lua_State *L) {
	check_param_count(L, 5);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_FUNCTION, 2);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	duel* pduel = pgroup->pduel;
	int32 acc = lua_tointeger(L, 3);
	int32 min = lua_tointeger(L, 4);
	int32 max = lua_tointeger(L, 5);
	if(min < 0)
		min = 0;
	if(max < min)
		max = min;
	int32 extraargs = lua_gettop(L) - 5;
	field::card_vector cv(pduel->game_field->core.must_select_cards);
	int32 mcount = cv.size();
	for(auto& pcard : pgroup->container) {
		auto it = std::find(pduel->game_field->core.must_select_cards.begin(), pduel->game_field->core.must_select_cards.end(), pcard);
		if(it == pduel->game_field->core.must_select_cards.end())
			cv.push_back(pcard);
	}
	pduel->game_field->core.must_select_cards.clear();
	for(auto& pcard : cv)
		pcard->sum_param = pduel->lua->get_operation_value(pcard, 2, extraargs);
	int32 should_continue = TRUE;
	lua_pushboolean(L, field::check_with_sum_limit_m(cv, acc, 0, min, max, mcount, &should_continue));
	lua_pushboolean(L, should_continue);
	return 2;
}
int32 scriptlib::group_select_with_sum_equal(lua_State *L) {
	check_action_permission(L);
	check_param_count(L, 6);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_FUNCTION, 3);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	duel* pduel = pgroup->pduel;
	int32 playerid = lua_tointeger(L, 2);
	if(playerid != 0 && playerid != 1)
		return 0;
	int32 acc = lua_tointeger(L, 4);
	int32 min = lua_tointeger(L, 5);
	int32 max = lua_tointeger(L, 6);
	if(min < 0)
		min = 0;
	if(max < min)
		max = min;
	int32 extraargs = lua_gettop(L) - 6;
	pduel->game_field->core.select_cards.assign(pgroup->container.begin(), pgroup->container.end());
	for(auto& pcard : pduel->game_field->core.must_select_cards) {
		auto it = std::remove(pduel->game_field->core.select_cards.begin(), pduel->game_field->core.select_cards.end(), pcard);
		pduel->game_field->core.select_cards.erase(it, pduel->game_field->core.select_cards.end());
	}
	field::card_vector cv(pduel->game_field->core.must_select_cards);
	int32 mcount = cv.size();
	cv.insert(cv.end(), pduel->game_field->core.select_cards.begin(), pduel->game_field->core.select_cards.end());
	for(auto& pcard : cv)
		pcard->sum_param = pduel->lua->get_operation_value(pcard, 3, extraargs);
	if(!field::check_with_sum_limit_m(cv, acc, 0, min, max, mcount, nullptr)) {
		pduel->game_field->core.must_select_cards.clear();
		group* empty_group = pduel->new_group();
		interpreter::group2value(L, empty_group);
		return 1;
	}
	pduel->game_field->add_process(PROCESSOR_SELECT_SUM, 0, 0, 0, acc, playerid + (min << 16) + (max << 24));
	return lua_yieldk(L, 0, (lua_KContext)pduel, [](lua_State *L, int32 status, lua_KContext ctx) {
		duel* pduel = (duel*)ctx;
		group* pgroup = pduel->new_group(field::card_set(pduel->game_field->return_cards.list.begin(), pduel->game_field->return_cards.list.end()));
		pduel->game_field->core.must_select_cards.clear();
		interpreter::group2value(L, pgroup);
		return 1;
	});
}
int32 scriptlib::group_check_with_sum_greater(lua_State *L) {
	check_param_count(L, 3);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_FUNCTION, 2);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	duel* pduel = pgroup->pduel;
	int32 acc = lua_tointeger(L, 3);
	int32 extraargs = lua_gettop(L) - 3;
	field::card_vector cv(pduel->game_field->core.must_select_cards);
	int32 mcount = cv.size();
	for(auto& pcard : pgroup->container) {
		auto it = std::find(pduel->game_field->core.must_select_cards.begin(), pduel->game_field->core.must_select_cards.end(), pcard);
		if(it == pduel->game_field->core.must_select_cards.end())
			cv.push_back(pcard);
	}
	pduel->game_field->core.must_select_cards.clear();
	for(auto& pcard : cv)
		pcard->sum_param = pduel->lua->get_operation_value(pcard, 2, extraargs);
	int32 should_continue = TRUE;
	lua_pushboolean(L, field::check_with_sum_greater_limit_m(cv, acc, 0, 0xffff, mcount, &should_continue));
	lua_pushboolean(L, should_continue);
	return 2;
}
int32 scriptlib::group_select_with_sum_greater(lua_State *L) {
	check_action_permission(L);
	check_param_count(L, 4);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_FUNCTION, 3);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	duel* pduel = pgroup->pduel;
	int32 playerid = lua_tointeger(L, 2);
	if(playerid != 0 && playerid != 1)
		return 0;
	int32 acc = lua_tointeger(L, 4);
	int32 extraargs = lua_gettop(L) - 4;
	pduel->game_field->core.select_cards.assign(pgroup->container.begin(), pgroup->container.end());
	for(auto& pcard : pduel->game_field->core.must_select_cards) {
		auto it = std::remove(pduel->game_field->core.select_cards.begin(), pduel->game_field->core.select_cards.end(), pcard);
		pduel->game_field->core.select_cards.erase(it, pduel->game_field->core.select_cards.end());
	}
	field::card_vector cv(pduel->game_field->core.must_select_cards);
	int32 mcount = cv.size();
	cv.insert(cv.end(), pduel->game_field->core.select_cards.begin(), pduel->game_field->core.select_cards.end());
	for(auto& pcard : cv)
		pcard->sum_param = pduel->lua->get_operation_value(pcard, 3, extraargs);
	if(!field::check_with_sum_greater_limit_m(cv, acc, 0, 0xffff, mcount, nullptr)) {
		pduel->game_field->core.must_select_cards.clear();
		group* empty_group = pduel->new_group();
		interpreter::group2value(L, empty_group);
		return 1;
	}
	pduel->game_field->add_process(PROCESSOR_SELECT_SUM, 0, 0, 0, acc, playerid);
	return lua_yieldk(L, 0, (lua_KContext)pduel, [](lua_State *L, int32 status, lua_KContext ctx) {
		duel* pduel = (duel*)ctx;
		group* pgroup = pduel->new_group(field::card_set(pduel->game_field->return_cards.list.begin(), pduel->game_field->return_cards.list.end()));
		pduel->game_field->core.must_select_cards.clear();
		interpreter::group2value(L, pgroup);
		return 1;
	});
}
int32 scriptlib::group_get_min_group(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_FUNCTION, 2);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	duel* pduel = pgroup->pduel;
	if(pgroup->container.size() == 0)
		return 0;
	group* newgroup = pduel->new_group();
	int32 min, op;
	int32 extraargs = lua_gettop(L) - 2;
	auto cit = pgroup->container.begin();
	min = pduel->lua->get_operation_value(*cit, 2, extraargs);
	newgroup->container.insert(*cit);
	++cit;
	for(; cit != pgroup->container.end(); ++cit) {
		op = pduel->lua->get_operation_value(*cit, 2, extraargs);
		if(op == min)
			newgroup->container.insert(*cit);
		else if(op < min) {
			newgroup->container.clear();
			newgroup->container.insert(*cit);
			min = op;
		}
	}
	interpreter::group2value(L, newgroup);
	lua_pushinteger(L, min);
	return 2;
}
int32 scriptlib::group_get_max_group(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_FUNCTION, 2);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	duel* pduel = pgroup->pduel;
	if(pgroup->container.size() == 0)
		return 0;
	group* newgroup = pduel->new_group();
	int32 max, op;
	int32 extraargs = lua_gettop(L) - 2;
	auto cit = pgroup->container.begin();
	max = pduel->lua->get_operation_value(*cit, 2, extraargs);
	newgroup->container.insert(*cit);
	++cit;
	for(; cit != pgroup->container.end(); ++cit) {
		op = pduel->lua->get_operation_value(*cit, 2, extraargs);
		if(op == max)
			newgroup->container.insert(*cit);
		else if(op > max) {
			newgroup->container.clear();
			newgroup->container.insert(*cit);
			max = op;
		}
	}
	interpreter::group2value(L, newgroup);
	lua_pushinteger(L, max);
	return 2;
}
int32 scriptlib::group_get_sum(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_FUNCTION, 2);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	duel* pduel = pgroup->pduel;
	int32 extraargs = lua_gettop(L) - 2;
	int32 sum = 0;
	for(auto& pcard : pgroup->container) {
		sum += pduel->lua->get_operation_value(pcard, 2, extraargs);
	}
	lua_pushinteger(L, sum);
	return 1;
}
int32 scriptlib::group_get_class_count(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_FUNCTION, 2);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	duel* pduel = pgroup->pduel;
	int32 extraargs = lua_gettop(L) - 2;
	std::set<uint32> er;
	for(auto& pcard : pgroup->container) {
		er.insert(pduel->lua->get_operation_value(pcard, 2, extraargs));
	}
	lua_pushinteger(L, er.size());
	return 1;
}
int32 scriptlib::group_remove(lua_State *L) {
	check_param_count(L, 3);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_FUNCTION, 2);
	card* pexception = 0;
	if(!lua_isnil(L, 3)) {
		check_param(L, PARAM_TYPE_CARD, 3);
		pexception = *(card**) lua_touserdata(L, 3);
	}
	group* pgroup = *(group**) lua_touserdata(L, 1);
	duel* pduel = pgroup->pduel;
	uint32 extraargs = lua_gettop(L) - 3;
	if(pgroup->is_readonly == 1)
		return 0;
	for (auto cit = pgroup->container.begin(); cit != pgroup->container.end();) {
		auto rm = cit++;
		if((*rm) != pexception && pduel->lua->check_matching(*rm, 2, extraargs)) {
			pgroup->container.erase(rm);
		}
	}
	return 0;
}
int32 scriptlib::group_merge(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_GROUP, 2);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	group* mgroup = *(group**) lua_touserdata(L, 2);
	if(pgroup->is_readonly == 1)
		return 0;
	pgroup->container.insert(mgroup->container.begin(), mgroup->container.end());
	return 0;
}
int32 scriptlib::group_sub(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_GROUP, 2);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	group* sgroup = *(group**) lua_touserdata(L, 2);
	if(pgroup->is_readonly == 1)
		return 0;
	for (auto& pcard : sgroup->container) {
		pgroup->container.erase(pcard);
	}
	return 0;
}
int32 scriptlib::group_equal(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_GROUP, 2);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	group* sgroup = *(group**) lua_touserdata(L, 2);
	if(pgroup->container.size() != sgroup->container.size()) {
		lua_pushboolean(L, 0);
		return 1;
	}
	auto pit = pgroup->container.begin();
	auto sit = sgroup->container.begin();
	for (; pit != pgroup->container.end(); ++pit, ++sit) {
		if((*pit) != (*sit)) {
			lua_pushboolean(L, 0);
			return 1;
		}
	}
	lua_pushboolean(L, 1);
	return 1;
}
int32 scriptlib::group_is_contains(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_CARD, 2);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	card* pcard = *(card**) lua_touserdata(L, 2);
	if(pgroup->container.find(pcard) != pgroup->container.end())
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}
int32 scriptlib::group_search_card(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_FUNCTION, 2);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	duel* pduel = pgroup->pduel;
	uint32 extraargs = lua_gettop(L) - 2;
	for(auto& pcard : pgroup->container)
		if(pduel->lua->check_matching(pcard, 2, extraargs)) {
			interpreter::card2value(L, pcard);
			return 1;
		}
	return 0;
}
int32 scriptlib::group_get_bin_class_count(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_GROUP, 1);
	check_param(L, PARAM_TYPE_FUNCTION, 2);
	group* pgroup = *(group**) lua_touserdata(L, 1);
	duel* pduel = pgroup->pduel;
	int32 extraargs = lua_gettop(L) - 2;
	int32 er = 0;
	for(auto& pcard : pgroup->container) {
		er |= pduel->lua->get_operation_value(pcard, 2, extraargs);
	}
	int32 ans = 0;
	while(er) {
		er &= er - 1;
		ans++;
	}
	lua_pushinteger(L, ans);
	return 1;
}
#ifdef BUILD_WITH_AI
int32 scriptlib::get_targeted_cards(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	if(pduel->game_field->core.current_chain.size() > 0) {
		chain* ch = &pduel->game_field->core.current_chain[pduel->game_field->core.current_chain.size() - 1];
		if(ch) {
			group* pgroup = ch->target_cards;
			if(pgroup && pgroup->container.size()) {
				for(auto& pcard : pgroup->container) {
					if(pcard) {
						printf("*** get_targeted_cards %d ***\n", pcard->get_code());
					}
				}
			}
		}
	}
	return 0;
}
#endif //BUILD_WITH_AI
