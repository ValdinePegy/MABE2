/**
 *  @note This file is part of MABE, https://github.com/mercere99/MABE2
 *  @copyright Copyright (C) Michigan State University, MIT Software license; see doc/LICENSE.md
 *  @date 2019
 *
 *  @file  Organism.h
 *  @brief A base class for all organisms in MABE.
 *  @note Status: PLANNING
 */

#ifndef MABE_ORGANISM_H
#define MABE_ORGANISM_H

#include "base/assert.h"
#include "data/VarMap.h"
#include "tools/string_utils.h"

class OrgTypeBase;

namespace mabe {

  class Organism {
  protected:
    emp::VarMap var_map;
    emp::Ptr<OrgTypeBase> type_ptr;

  public:
    virtual ~Organism() { ; }

    OrgTypeBase & GetType() { return *type_ptr; }
    const OrgTypeBase & GetType() const { return *type_ptr; }

    bool HasVar(const std::string & name) const { return var_map.Has(name); }
    template <typename T> T & GetVar(const std::string & name) { return var_map.Get<T>(name); }
    template <typename T> const T & GetVar(const std::string & name) const { return var_map.Get<T>(name); }

    template <typename T>
    void SetVar(const std::string & name, const T & value) {
      var_map.Set(name, value);
    }

    // --- Functions for overriding ---

    /// We MUST be able to make a copy of organisms for MABE to function.
    virtual emp::Ptr<Organism> Clone() = 0;  

    /// If we are going to print organisms (to screen or file) we need to be able to convert
    /// them to strings.
    virtual std::string ToString() { return "__unknown__"; }

    /// For evolution to function, we need to be able to mutate offspring.
    virtual int Mutate() { emp_assert(false, "No default Mutate() available."); return -1; }
  };

}
#endif