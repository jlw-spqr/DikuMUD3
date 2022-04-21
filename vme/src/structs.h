/*
 $Author: All $
 $RCSfile: structs.h,v $
 $Date: 2005/06/28 20:17:48 $
 $Revision: 2.9 $
 */
#pragma once

#include "char_point_data.h"
#include "color.h"
#include "combat.h"
#include "destruct.h"
#include "event.h"
#include "extra.h"
#include "hookmud.h"
#include "namelist.h"
#include "pc_account_data.h"
#include "pc_time_data.h"
#include "protocol.h"
#include "queue.h"
#include "room_direction_data.h"
#include "unit_data.h"

#include <vme.h>

#include <cstring>
#include <forward_list>
#include <map>
#include <ostream>
#include <vector>

#define FI_MAX_ZONENAME 30 /* Max length of any zone-name    */
#define FI_MAX_UNITNAME 15 /* Max length of any unit-name    */

#define PC_MAX_PASSWORD 13 /* Max length of any pc-password  */
#define PC_MAX_NAME 15     /* 14 Characters + Null           */

#define MESS_ATTACKER 1
#define MESS_VICTIM 2
#define MESS_ROOM 3

/* For use in spec_assign.c's g_unit_function_array[] */
#define SD_NEVER 0 /* Never save this function           */
#define SD_NULL 1  /* Ignore fptr->data (save as 0 ptr)  */
#define SD_ASCII 2 /* If pointer, then it's ascii char * */

class char_follow_type;
class descriptor_data;
class dilprg;
class diltemplate;
class file_index_type;
class unit_affected_type;
class unit_data;
class unit_dil_affected_type;
class unit_fptr;
class zone_reset_cmd;

/* ----------------- DATABASE STRUCTURES ----------------------- */

/* ----------------- OTHER STRUCTURES ----------------------- */

/* --------------------- DESCRIPTOR STRUCTURES -------------------- */

/* ----------------- UNIT GENERAL STRUCTURES ----------------------- */

/* ----------------- ROOM SPECIFIC STRUCTURES ----------------------- */

/* ------------------ OBJ SPECIFIC STRUCTURES ----------------------- */

class obj_data : public unit_data
{
public:
    obj_data();
    ~obj_data();

    /**
     * @name Value related functions
     * @{
     */

    /**
     * Gets the value at index for the object
     *
     * WEAPONS
     *     * index = 0 is weapon category
     *     * index = 1 is weapon material bonus
     *     * index = 2 is magic bonus
     *     * index = 3 is slaying race
     *
     * ARMOURS
     *     * index = 0 is armour category
     *     * index = 1 is armour material bonus
     *     * index = 2 is magic bonus
     *
     * SHIELDS
     *     * index = 0 is shield category
     *     * index = 1 is shield material bonus
     *     * index = 2 is magic bonus
     *
     * @throws std::out_of_range if index is greater than array size
     * @param index
     * @return Bonus/Value at index
     */
    sbit32 getValueAtIndex(size_t index) const { return m_value.at(index); }

    /**
     * @throws std::out_of_range if index is greater than array size
     * @param index Array index see getValueAtIndex() for index values
     * @return Pointer to value at index - its a DIL thing
     */
    sbit32 *getValueAtIndexPtr(size_t index) { return &m_value.at(index); }

    /**
     * @return Upper bound of array size (5)
     */
    size_t getValueArraySize() const { return m_value.size(); }

    /**
     * @throws std::out_of_range if index is greater than array size
     * @param index Array index see getValueAtIndex() for index values
     * @param value Value to set
     */
    void setValueAtIndexTo(size_t index, sbit32 value) { m_value.at(index) = value; }
    /// @}

    /**
     * @name Price related functions
     * @{
     */

    /**
     * @return Price in GP
     */
    ubit32 getPriceInGP() const { return m_cost; }
    /**
     * @return Pointer to price in GP
     */
    ubit32 *getPriceInGPPtr() { return &m_cost; }

    /**
     * @param value Price in GP
     */
    void setPriceInGP(ubit32 value) { m_cost = value; }
    /// @}

    /**
     * @name Price per day functions
     * @{
     */
    /**
     * @return Cost to keep per real day
     */
    ubit32 getPricePerDay() const { return m_cost_per_day; }
    /**
     * @return Pointer to cost per real day
     */
    ubit32 *getPricePerDayPtr() { return &m_cost_per_day; }
    /**
     * @param value Price to keep per real day
     */
    void setPricePerDay(ubit32 value) { m_cost_per_day = value; }
    /// @}

    /**
     * @name Object Flag related functions
     * @{
     */
    /**
     * @return All object flags
     */
    ubit8 getObjectFlags() const { return m_flags; }
    /**
     * @return pointer to object flags for DIL
     */
    ubit8 *getObjectFlagsPtr() { return &m_flags; }
    /**
     * @todo Find out why a 32bit value is being passed to this function and the flags are 8bit
     * @param value Sets bits from value in object flags
     */
    void setObjectFlag(ubit8 value) { m_flags |= value; }
    /**
     * @todo Find out why a 32bit value is being passed to this function and the flags are 8bit
     * @param value Unsets bit from value in object flags
     */
    void removeObjectFlag(ubit8 value) { m_flags &= ~value; }
    /**
     * @todo Find out why a 32bit value is being passed to this function and the flags are 8bit
     * @param value Overwrites all flags with value
     */
    void setAllObjectFlags(ubit32 value) { m_flags = static_cast<ubit8>(value); }
    /// @}

    /**
     * @name Item type related functions
     * @todo Create ENUM for ITEM_* types
     * @{
     */

    /**
     * @return the ITEM_* type
     */
    ubit8 getObjectItemType() const { return m_type; }
    /**
     * @return pointer to the item type
     */
    ubit8 *getObjectItemTypePtr() { return &m_type; }
    /**
     * @param value ITEM_* value (see vme.h)
     */
    void setObjectItemType(ubit8 value) { m_type = value; }
    /// @}

    /**
     * @name Equipment position related functions
     * @{
     */
    /**
     * @return 0 or position of item in equipment
     */
    ubit8 getEquipmentPosition() const { return m_equip_pos; }
    /**
     * @param value 0 or position of item in equipment
     */
    void setEquipmentPosition(ubit8 value) { m_equip_pos = value; }
    /// @}
private:
    std::array<sbit32, 5> m_value{0}; ///< Values of the item (see list)
    ubit32 m_cost{0};                 ///< Value when sold (gp.)
    ubit32 m_cost_per_day{0};         ///< Cost to keep pr. real day
    ubit8 m_flags{0};                 ///< Various special object flags
    ubit8 m_type{ITEM_TRASH};         ///< Type of item (ITEM_XXX)
    ubit8 m_equip_pos{0};             ///< 0 or position of item in equipment
public:
    ubit8 resistance; ///< Magic resistance
};

/* ----------------- CHAR SPECIFIC STRUCTURES ----------------------- */

class char_data : public unit_data
{
public:
    explicit char_data(ubit8 unit_type);
    virtual ~char_data();

    descriptor_data *descriptor;
    cCombat *Combat;
    unit_data *master;    ///< Must be a char
    unit_data *last_room; ///< Last location of character
    char_point_data points;

    char_follow_type *followers;

    char *last_attacker; ///< Last attacker of character
    char *money;         ///<  Money transfer from db-files.

    ubit8 last_attacker_type; ///< Last attacker type of character
};

/* ------------------  PC SPECIFIC STRUCTURES ------------------------ */

class pc_data : public char_data
{
public:
    pc_data();
    ~pc_data();

    void gstate_tomenu(dilprg *pdontstop);
    void gstate_togame(dilprg *pdontstart);

    void disconnect_game();
    void connect_game();
    void reconnect_game(descriptor_data *d);

    terminal_setup_type setup;

    pc_time_data m_time{};     ///< PCs time info
    pc_account_data account{}; ///< Accounting

    char *guild;     ///< Player's current default guild (guilds in .info)
    char *bank;      ///< How much money in bank?
    char *hometown;  ///< PCs Hometown (symbolic reference)
    char *promptstr; ///< A PC's Prompt

    extra_list info;  ///< For saving Admin information
    extra_list quest; ///< For saving QUEST information

    sbit8 profession; ///< The player's chosen profession, -1 means unknown
    ubit16 vlvl;      ///< Virtual Level for player

    sbit32 id;             ///< Unique identifier for each player (-1 guest)
    sbit32 skill_points;   ///< No of practice points left
    sbit32 ability_points; ///< No of practice points left

    ubit16 flags;          ///< flags for PC setup (brief, noshout...)
    ubit16 nr_of_crimes;   ///< Number of crimes committed
    ubit16 crack_attempts; ///< Number of wrong passwords entered
    ubit16 lifespan{};     ///< How many year to live....

    sbit16 spells[SPL_TREE_MAX];   ///< The spells learned
    ubit8 spell_lvl[SPL_TREE_MAX]; ///< Practiced within that level

    sbit16 skills[SKI_TREE_MAX];   ///< The skills learned
    ubit8 skill_lvl[SKI_TREE_MAX]; ///< The skills practiced within level

    sbit16 weapons[WPN_TREE_MAX];   ///< The weapons learned
    ubit8 weapon_lvl[WPN_TREE_MAX]; ///< The weapons learned

    ubit8 ability_lvl[ABIL_TREE_MAX]; ///< The abilities learned

    sbit8 conditions[3]; ///< Drunk full etc.
    ubit8 nAccessLevel;  ///< Access Level for BBS use

    char pwd[PC_MAX_PASSWORD];  ///< Needed when loaded w/o descriptor
    char filename[PC_MAX_NAME]; ///< The name on disk...
    ubit32 lasthosts[5];        ///< last 5 different IPs
    color_type color;           ///< Players default colors
};

/* ------------------ NPC SPECIFIC STRUCTURES ----------------------- */

class npc_data : public char_data
{
public:
    npc_data();
    ~npc_data();

    std::array<sbit16, WPN_GROUP_MAX> weapons;
    std::array<sbit16, SPL_GROUP_MAX> spells;

    ubit8 default_pos; ///< Default position for NPC
    ubit8 flags;       ///< flags for NPC behavior
};

/* ----------------- Destructed decalrations ----------------------- */

#define DR_UNIT 0
#define DR_AFFECT 1
#define DR_FUNC 2

unit_data *new_unit_data(ubit8 type);

extern int g_world_nochars;
extern int g_world_nonpc;
extern int g_world_noobjects;
extern int g_world_norooms;
extern int g_world_nopc;
extern int g_world_nozones;
