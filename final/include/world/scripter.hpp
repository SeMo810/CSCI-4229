#ifndef SCRIPTER_HPP_
#define SCRIPTER_HPP_

#include "../config.hpp"

namespace SCRIPT
{

enum class ScriptOpcode
{
  ShipPlace,
  Fire,
  Wait
};

struct ScriptInstruction
{
  ScriptOpcode opcode;
  union
  {
    struct // Ship placement data
    {
      int shiptype;
      int placex;
      int placey;
      int orientation;
    };
    struct // Fire data
    {
      int firex;
      int firey;
    };
    struct // Wait data
    {
      float waittime;
    };
  } data;

  ScriptInstruction() :
    opcode(ScriptOpcode::ShipPlace), data{0}
  { }
};

extern bool load_script_file();
extern void start_script();
extern void end_script();

}

#endif // SCRIPTER_HPP_
