#include "world/scripter.hpp"
#include "world/world.hpp"
#include "world/ship.hpp"
#include "log.hpp"
#include <vector>

bool try_SP_parse(const char *buffer, int *type, int *x, int *y, int *dir)
{
  char typebuffer[32];
  char dirbuffer[32];
  int px, py;

  int res = sscanf(buffer, "ship %s %d %d %s", typebuffer, &px, &py, dirbuffer);
  if (res != 4)
    return false;

  String type(typebuffer);
  if (type == "PATROL") *type = SHIPTYPE_PATROL;
  else if (type == "SUBMARINE") *type = SHIPTYPE_SUBMARINE;
  else if (type == "DESTROYER") *type = SHIPTYPE_DESTROYER;
  else if (type == "BATTLESHIP") *type = SHIPTYPE_BATTLESHIP;
  else if (type == "CARRIER") *type = SHIPTYPE_CARRIER;
  else *type = -1;

  String direction(dirbuffer);
  if (direction == "SOUTH") *dir = SHIPORIENT_SOUTH;
  else if (direction == "NORTH") *dir = SHIPORIENT_NORTH;
  else if (direction == "EAST") *dir = SHIPORIENT_EAST;
  else if (direction == "WEST") *dir = SHIPORIENT_WEST;
  else *dir = -1;

  *x = px;
  *y = py;
  return true;
}

bool try_FR_parse(const char *buffer, int *x, int *y)
{

}

bool try_WT_parse(const char *buffer, float *time)
{

}

namespace SCRIPT
{

static std::vector<ScriptInstruction> g_instructions = std::vector<ScriptInstruction>();

bool load_script_file()
{
  FILE *file;
  char buffer[32];

  file = fopen("content/game.dat", "r");
  if (!file)
  {
    LOG::error("Could not open the data file for the game.");
    return false;
  }

  int instructCount = 0;
  int shipCount = 0;
  bool patrol = false, sub = false, dest = false, battle = false, carrier = false;
  while (EOF != fscanf(file, "%[^\n]\n", buffer))
  {
    if (!strcmp(buffer, "\n") || !strcmp(buffer, " \n") || !strcmp(buffer, "  \n") || !strcmp(buffer, "\t\n"))
      continue;

    int type = -1, x = -1, y = -1, dir = -1;
    float tm = -1;
    ++instructCount;

    if (try_SP_parse(buffer, &type, &x, &y, &dir))
    {
      if (shipCount > 5)
      {
        LOG::error("SCRIPTER: Too many ship placement instructions. Line (%d).", instructCount);
        return false;
      }
      ++shipCount;

      ScriptInstruction inst;
      inst.opcode = ScriptOpcode::ShipPlace;
      switch (type)
      {
      case SHIPTYPE_PATROL:
        patrol = true;
        break;
      case SHIPTYPE_SUBMARINE:
        sub = true;
        break;
      case SHIPTYPE_DESTROYER:
        dest = true;
        break;
      case SHIPTYPE_BATTLESHIP:
        battle = true;
        break;
      case SHIPTYPE_CARRIER:
        carrier = true;
        break;
      default:
        LOG::error("SCRIPTER: Invalid ship type specified in line: \"%s\".", buffer);
        return false;
      }
      inst.data.shiptype = type;
      switch (dir)
      {
      case SHIPORIENT_WEST:
      case SHIPORIENT_EAST:
      case SHIPORIENT_NORTH:
      case SHIPORIENT_SOUTH:
        inst.data.orientation = dir;
        break;
      default:
        LOG::error("SCRIPTER: Invalid ship orientation specified in line: \"%s\".", buffer);
        return false;
      }
      if (x < 0 || y < 0 || x >= WORLDTILEWIDTH || y >= WORLDTILEHEIGHT)
      {
        LOG::error("SCRIPTER: Invalid ship coordinates specified in line: \"%s\".", buffer);
        return false;
      }
      inst.data.placex = x;
      inst.data.placey = y;
      g_instructions.push_back(inst);
    }
    else if (try_FR_parse(buffer, &x, &y))
    {
      if (shipCount != 5)
      {
        String error = "SCRIPTER: Not all ships were placed in script. Missing ships:";
        if (!patrol) error += " Patrol";
        if (!sub) error += " Submarine";
        if (!dest) error += " Destroyer";
        if (!battle) error += " Battleship";
        if (!carrier) error += " Carrier";
        error += ".";
        LOG::error("%s", error.c_str());
        return false;
      }
      ScriptInstruction inst;
      inst.opcode = ScriptOpcode::Fire;
      if (x < 0 || y < 0 || x >= WORLDTILEWIDTH || y >= WORLDTILEHEIGHT)
      {
        LOG::error("SCRIPTER: Invalid fire coordinates specified in line: \"%s\".", buffer);
        return false;
      }
      inst.data.firex = x;
      inst.data.firey = y;
      g_instructions.push_back(inst);
    }
    else if (try_WT_parse(buffer, &tm))
    {
      if (shipCount != 5)
      {
        String error = "SCRIPTER: Not all ships were placed in script. Missing ships:";
        if (!patrol) error += " Patrol";
        if (!sub) error += " Submarine";
        if (!dest) error += " Destroyer";
        if (!battle) error += " Battleship";
        if (!carrier) error += " Carrier";
        error += ".";
        LOG::error("%s", error.c_str());
        return false;
      }
      ScriptInstruction inst;
      inst.opcode = ScriptOpcode::Wait;
      if (tm <= 0)
      {
        LOG::warn("SCRIPTER: Invalid negative or zero wait time specified in line: \"%s\". This instruction will be ignored.", buffer);
        continue;
      }
      inst.data.waittime = tm;
      g_instructions.push_back(inst);
    }
    else
    {
      LOG::error("SCRIPTER: Could not parse the line: \"%s\".", buffer);
      return false;
    }
  }

  fclose(file);
  LOG::info("SCRIPTER: Script parsing completed successfully, with (%d) instructions total.", instructCount);
  return true;
}

void start_script()
{

}

void end_script()
{

}

}
