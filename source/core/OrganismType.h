/**
 *  @note This file is part of MABE, https://github.com/mercere99/MABE2
 *  @copyright Copyright (C) Michigan State University, MIT Software license; see doc/LICENSE.md
 *  @date 2019
 *
 *  @file  OrganismWrapper.h
 *  @brief Details about how a specific type of organism should function.
 */

#ifndef MABE_ORGANISM_TYPE_H
#define MABE_ORGANISM_TYPE_H

#include <functional>
#include <iostream>

#include "base/unordered_map.h"
#include "data/VarMap.h"
#include "tools/Random.h"

namespace mabe {

  class Organism;

  class OrganismType {
  private:
    std::string name;     ///< Name used for this type of organisms.
    emp::VarMap var_map;  ///< Map of run-time values associated with this organism type.

  public:
    OrganismType(const std::string & in_name) : name(in_name) { ; }
    virtual ~OrganismType() { ; }

    const std::string & GetName() const { return name; }

    // --== Functions to manipulate organisms ==--
    virtual emp::Ptr<Organism> MakeOrganism(emp::Random &) = 0;
    virtual size_t MutateOrg(Organism &, emp::Random &) = 0;
    virtual std::ostream & PrintOrg(Organism &, std::ostream &) = 0;
    virtual bool Randomize(Organism &, emp::Random &) = 0;
  };

  /// An single type of organism that can have many of its qualities manipulated (and will modify
  /// all organisms of this type.)
  /// NOTE: ORG_T must be derived from mabe::Organism.  When we update to C++20, we can enforce
  ///       this requirement using concepts.
  template <typename ORG_T>
  class OrganismWrapper : public OrganismType {
  private:
    // --== Current versions of user-controled functions to manipulate organisms ==--
    std::function<emp::Ptr<Organism>(emp::Random & random)> make_org_fun;
    std::function<size_t(ORG_T &, emp::Random & random)> mut_fun;
    std::function<std::ostream & (ORG_T &, std::ostream &)> print_fun;
    std::function<bool(ORG_T &, emp::Random & random)> randomize_fun;

  public:
    OrganismWrapper(const std::string & in_name) : OrganismType(in_name) {
      /// --== Initial versions of user-defined functions ==--
      make_org_fun = [](emp::Random & random){
                       auto org = emp::NewPtr<ORG_T>();
                       org->Randomize();
                       return org;
                     };
      mut_fun = [](ORG_T & org, emp::Random & random){ return org.Mutate(random); };
      print_fun = [](ORG_T & org, std::ostream & os){ os << org.ToString(); };
      randomize_fun = [](ORG_T & org, emp::Random & random){ return org.Randomize(random); };
    }

    emp::Ptr<Organism> MakeOrg(emp::Random & random) {
      return make_org_fun(random);
    }
    void SetMakeOrgFun(std::function<emp::Ptr<Organism>(emp::Random &)> & in_fun) {
      mut_fun = in_fun;
    }

    size_t MutateOrg(Organism & org, emp::Random & random) {
      emp_assert(org.GetType() == this);
      return mut_fun((ORG_T &) org, random);
    }
    void SetMutateFun(std::function<size_t(ORG_T &, emp::Random &)> & in_fun) {
      mut_fun = in_fun;
    }

    std::ostream & PrintOrg(Organism & org, std::ostream & os) {
      emp_assert(org.GetType() == this);
      print_fun((ORG_T &) org, os);
      return os;
    }
    void SetPrintFun(std::function<void(ORG_T &, std::ostream &)> & in_fun) {
      print_fun = in_fun;
    }

    size_t RandomizeOrg(Organism & org, emp::Random & random) {
      emp_assert(org.GetType() == this);
      return randomize_fun((ORG_T &) org, random);
    }
    void SetRandomizeFun(std::function<size_t(ORG_T &, emp::Random &)> & in_fun) {
      mut_fun = in_fun;
    }

  };

}

#endif
